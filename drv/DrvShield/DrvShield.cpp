#include "std.h"
#include "Cpprtl.h"
#include "DrvShield.h"
#include "Sysmon.h"
#include "HookSSDT.h"
#include "md5.h"
#include "Hash.h"

PDEVICE_OBJECT GlobalDeviceObject = NULL;

KDrvShieldDriver theDriver;

NTSTATUS KDrvShieldDriver::IrpMajorCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
	if (!DevExtsion->DeviceOpen)
	{
		KIRQL Irql;
		KeAcquireSpinLock(&DevExtsion->SpinLock, &Irql);
		DevExtsion->DeviceOpen = TRUE;
		KeReleaseSpinLock(&DevExtsion->SpinLock, Irql);
	}
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status      = STATUS_SUCCESS;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS KDrvShieldDriver::IrpMajorClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
	if (DevExtsion->DeviceOpen)
	{
		KIRQL Irql;
		KeAcquireSpinLock(&DevExtsion->SpinLock, &Irql);
		DevExtsion->DeviceOpen  = FALSE;
		KeReleaseSpinLock(&DevExtsion->SpinLock, Irql);
	}
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status      = STATUS_SUCCESS;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS KDrvShieldDriver::IrpMajorDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	KIRQL Irql;
	ULONG uInformation = 0;
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;

	switch(irpSp->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_SYS_DRV_MON_SET:
		if (DevExtsion->DeviceOpen && !DevExtsion->StartSysMon)
		{
			CHAR Md5Digest[128] = {0};
			ANSI_STRING anWorkPath;
			UNICODE_STRING unWorkPath;
			ULONG Length = irpSp->Parameters.DeviceIoControl.InputBufferLength;
			PPARAMETER_BLOCK_DATA ParamBlock = (PPARAMETER_BLOCK_DATA)Irp->AssociatedIrp.SystemBuffer;
			if (
				Length >= sizeof(PARAMETER_BLOCK_DATA) + 32        && 
				ParamBlock->ParamBlockMagic == ParamBlockMagicFlag && 
				ParamBlock->DrvAntiData.DataMagic == FileMagicFlag
				)
			{
				RtlInitAnsiString(&anWorkPath, ParamBlock->WorkPath);
				RtlAnsiStringToUnicodeString(&unWorkPath, &anWorkPath, TRUE);
				unWorkPath.Buffer[unWorkPath.Length/sizeof(WCHAR)] = 0;
				wcscpy(DevExtsion->WorkSavePath, L"\\??\\");
				wcscat(DevExtsion->WorkSavePath, unWorkPath.Buffer);
				RtlFreeUnicodeString(&unWorkPath);

				DevExtsion->DrvAntiData = (PDRV_ANTI_DATA)ExAllocatePool(NonPagedPool, ParamBlock->DataSize);
				memcpy(DevExtsion->DrvAntiData, &ParamBlock->DrvAntiData, ParamBlock->DataSize);

				md5(DevExtsion->DrvAntiData, ParamBlock->DataSize-32, Md5Digest);

				if (memcmp(((PCHAR)DevExtsion->DrvAntiData)+(ParamBlock->DataSize-32), Md5Digest, 32) == 0)
				{
					ULONG i;
					HashTableInit(DevExtsion->DrvAntiData->Count);
					for (i = 0; i < DevExtsion->DrvAntiData->Count; i++)
					{
						InsertHashTable(DevExtsion->DrvAntiData->DrvInfoEntry[i]);
					}
					KeAcquireSpinLock(&DevExtsion->SpinLock, &Irql);
					DevExtsion->StartSysMon = TRUE;
					KeReleaseSpinLock(&DevExtsion->SpinLock, Irql);
				}
			}
		}
		else
		{
			status = STATUS_INVALID_PARAMETER;
		}
		break;

	case IOCTL_SYS_ADD_PROCESSID:
		if (DevExtsion->DeviceOpen)
		{
			PPARAMETER_BLOCK_PROCESS_ID ParamBlock = (PPARAMETER_BLOCK_PROCESS_ID)Irp->AssociatedIrp.SystemBuffer;
			if (ParamBlock->ParamBlockMagic == ParamBlockMagicFlag)
			{
				KeAcquireSpinLock(&DevExtsion->SpinLock, &Irql);
				AddProcessProtectList(ParamBlock->ProcessID);
				DevExtsion->StartHook = TRUE;
				KeReleaseSpinLock(&DevExtsion->SpinLock, Irql);
			}
			else
			{
				status = STATUS_INVALID_PARAMETER;
			}
		}
		break;

	case IOCTL_SYS_QUERY_DRIVER:
		if (
			DevExtsion->StartSysMon &&
			irpSp->Parameters.DeviceIoControl.OutputBufferLength >= sizeof(DevExtsion->CaptureDriverPathName)
			)
		{
			strcpy((PCHAR)Irp->AssociatedIrp.SystemBuffer, DevExtsion->CaptureDriverPathName);
			uInformation = strlen(DevExtsion->CaptureDriverPathName);
		}
		else
		{
			status = STATUS_INVALID_PARAMETER;
		}
		break;

	case IOCTL_SYS_QUERY_COMPLETE:
		if (DevExtsion->StartSysMon)
		{
			KeSetEvent(DevExtsion->obDriverClearedEvent, IO_NO_INCREMENT, FALSE);	
		}
		break;

	default:
		status = STATUS_INVALID_PARAMETER;
		break;
	}
	Irp->IoStatus.Information = uInformation;
	Irp->IoStatus.Status      = status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS KDrvShieldDriver::DriverEntry()
{
	BOOLEAN bSymbolicLinkClear = FALSE;
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_EXTENSION DevExtsion;
	DECLARE_UNICODE_STRING(unDeviceName, L"\\Device\\KeydoneSysMon")
	DECLARE_UNICODE_STRING(unSymbolicLinkName, L"\\DosDevices\\Global\\KeydoneSysMon")
	DECLARE_UNICODE_STRING(unDriverEntryEvent, L"\\BaseNamedObjects\\KDDriverEntryEvent")
	DECLARE_UNICODE_STRING(unDriverProcessedEvent, L"\\BaseNamedObjects\\KDDriverProcessedEvent")
	DECLARE_UNICODE_STRING(unDriverClearedEvent, L"\\BaseNamedObjects\\KDDriverClearedEvent")

	__try
	{
		ULONG uCount = 0;
		PPHYSICAL_MEMORY_RANGE CurPhyMemRange      = NULL;
		PPHYSICAL_MEMORY_RANGE PhysicalMemoryRange = NULL;
		status = SWHookInit();
		if (!NT_SUCCESS(status))
		{
			__leave;
		}
		status = IoCreateDevice(m_DriverObject, sizeof(DEVICE_EXTENSION), &unDeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &GlobalDeviceObject);
		if (!NT_SUCCESS(status))
		{
			__leave;
		}
		GlobalDeviceObject->Flags |= DO_DIRECT_IO;
		DevExtsion = (PDEVICE_EXTENSION)GlobalDeviceObject->DeviceExtension;
		memset(DevExtsion, 0, sizeof(DEVICE_EXTENSION));
		KeInitializeSpinLock(&DevExtsion->SpinLock);
		DevExtsion->DriverObject = m_DriverObject;
		DevExtsion->DeviceOpen  = FALSE;
		DevExtsion->StartSysMon = FALSE;
		DevExtsion->StartHook   = FALSE;
		CurPhyMemRange = PhysicalMemoryRange = MmGetPhysicalMemoryRanges();
		for (; CurPhyMemRange->BaseAddress.QuadPart!=0&&CurPhyMemRange->NumberOfBytes.QuadPart!=0; CurPhyMemRange++)
		{
			uCount++;
		}
		uCount++;
		DevExtsion->PhysicalMemoryRange = (PPHYSICAL_MEMORY_RANGE)ExAllocatePool(NonPagedPool, uCount*sizeof(PHYSICAL_MEMORY_RANGE));
		if (DevExtsion->PhysicalMemoryRange == NULL)
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			__leave;
		}
		memcpy(DevExtsion->PhysicalMemoryRange, PhysicalMemoryRange, uCount*sizeof(PHYSICAL_MEMORY_RANGE));
		status = IoCreateSymbolicLink(&unSymbolicLinkName, &unDeviceName);
		if (!NT_SUCCESS(status))
		{
			__leave;
		}
		bSymbolicLinkClear = TRUE;
		status = InstallSysMonHandler();
		if (!NT_SUCCESS(status))
		{
			__leave;
		}
		DevExtsion->obDriverEntryEvent = IoCreateNotificationEvent(&unDriverEntryEvent, &DevExtsion->hDriverEntryEvent);
		DevExtsion->obDriverProcessedEvent = IoCreateNotificationEvent(&unDriverProcessedEvent, &DevExtsion->hDriverProcessedEvent);
		DevExtsion->obDriverClearedEvent = IoCreateNotificationEvent(&unDriverClearedEvent, &DevExtsion->hDriverClearedEvent);
		if (
			DevExtsion->obDriverEntryEvent     == NULL ||
			DevExtsion->obDriverProcessedEvent == NULL ||
			DevExtsion->obDriverClearedEvent   == NULL
			)
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			__leave;
		}
		KeClearEvent(DevExtsion->obDriverEntryEvent);
		KeClearEvent(DevExtsion->obDriverProcessedEvent);
		KeClearEvent(DevExtsion->obDriverClearedEvent);

		SWHookSSDT();
		m_DriverObject->DriverUnload = NULL;
	}
	__finally
	{
		if (status != STATUS_SUCCESS)
		{
			UnInstallSysMonHandler();
			if (DevExtsion->obDriverEntryEvent != NULL)
			{
				ZwClose(DevExtsion->hDriverEntryEvent);
			}
			if (DevExtsion->obDriverProcessedEvent != NULL)
			{
				ZwClose(DevExtsion->hDriverProcessedEvent);
			}
			if (DevExtsion->obDriverClearedEvent != NULL)
			{
				ZwClose(DevExtsion->hDriverClearedEvent);
			}
			if (DevExtsion->PhysicalMemoryRange != NULL)
			{
				ExFreePool(DevExtsion->PhysicalMemoryRange);
			}
			if (bSymbolicLinkClear)
			{
				IoDeleteSymbolicLink(&unSymbolicLinkName);
			}
			if (GlobalDeviceObject != NULL)
			{
				IoDeleteDevice(GlobalDeviceObject);
			}
		}
	}
	return status;
}
