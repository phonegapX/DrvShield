#include "std.h"
#include "Sysmon.h"
#include "HookSSDT.h"
#include "md5.h"
#include "Hash.h"

NTSTATUS SWCreateDirectory(PWCHAR wpDirectoryPath)
{
	NTSTATUS           status     = STATUS_SUCCESS;
	HANDLE             hDirectory = NULL;
	IO_STATUS_BLOCK    IoStatusBlock;
	UNICODE_STRING     unDirectoryPath;
	OBJECT_ATTRIBUTES  ObjectAttributes;
	RtlInitUnicodeString(&unDirectoryPath, wpDirectoryPath);
	InitializeObjectAttributes(&ObjectAttributes, &unDirectoryPath, OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ZwCreateFile(&hDirectory, SYNCHRONIZE, &ObjectAttributes, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN_IF, FILE_DIRECTORY_FILE|FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (NT_SUCCESS(status))
	{
		if (IoStatusBlock.Information == FILE_CREATED)
		{
			DbgPrint("Directory created\r\n");
		}
		else if (IoStatusBlock.Information == FILE_OPENED)
		{
			DbgPrint("Directory exists and was opened\r\n");
		}
		ZwClose(hDirectory);
	}
	return status;
}

NTSTATUS SWReadDriverFileToBuffer(PWCHAR FileName, PDEVICE_EXTENSION DevExtsion)
{
	UNICODE_STRING unFileName;
	OBJECT_ATTRIBUTES ObjectAttributes;
	FILE_STANDARD_INFORMATION FileInformation;
	IO_STATUS_BLOCK IoStatusBlock;
	LARGE_INTEGER ByteOffset;
	NTSTATUS status = STATUS_SUCCESS;
	HANDLE hFile    = NULL;
	HANDLE hEvent   = NULL;

	DevExtsion->ImageFileSize   = 0;
	DevExtsion->ImageFileBuffer = NULL;
	__try
	{
		RtlInitUnicodeString(&unFileName, FileName);
		InitializeObjectAttributes(&ObjectAttributes, &unFileName, OBJ_CASE_INSENSITIVE|OBJ_KERNEL_HANDLE, NULL, NULL);
		status = ZwCreateFile(
			&hFile, 
			GENERIC_READ, 
			&ObjectAttributes, 
			&IoStatusBlock, 
			NULL, 
			FILE_ATTRIBUTE_NORMAL, 
			FILE_SHARE_READ, 
			FILE_OPEN, 
			FILE_NON_DIRECTORY_FILE|FILE_RANDOM_ACCESS,
			NULL, 
			0);
		if (!NT_SUCCESS(status))
		{
			__leave;
		}
		status = ZwQueryInformationFile(hFile, &IoStatusBlock, &FileInformation, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);
		if (!NT_SUCCESS(status))
		{
			__leave;
		}
		if (
			(ULONG)FileInformation.EndOfFile.QuadPart == 0 ||
			(ULONG)FileInformation.EndOfFile.QuadPart > 1024 * 1024 * 40
			)
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			__leave;
		}
		DevExtsion->ImageFileSize = (ULONG)FileInformation.EndOfFile.QuadPart;
		DevExtsion->ImageFileBuffer = ExAllocatePool(NonPagedPool, DevExtsion->ImageFileSize);
		if (DevExtsion->ImageFileBuffer == NULL)
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			__leave;
		}
		ZwCreateEvent(&hEvent, GENERIC_ALL, NULL, NotificationEvent, FALSE);
		status = ZwReadFile(hFile, hEvent, NULL, NULL, &IoStatusBlock, DevExtsion->ImageFileBuffer, DevExtsion->ImageFileSize, (ByteOffset.QuadPart = 0, &ByteOffset), NULL);
		if (status == STATUS_PENDING)
		{
			ZwWaitForSingleObject(hEvent, TRUE, NULL);
			status = STATUS_SUCCESS;
		}
		if (NT_SUCCESS(status) && IoStatusBlock.Information < DevExtsion->ImageFileSize)
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			__leave;
		}
	}
	__finally
	{
		if (hFile != NULL)
		{
			ZwClose(hFile);
		}
		if (hEvent != NULL)
		{
			ZwClose(hEvent);
		}
		if (!NT_SUCCESS(status) && DevExtsion->ImageFileBuffer != NULL)
		{
			ExFreePool(DevExtsion->ImageFileBuffer);
			DevExtsion->ImageFileSize   = 0;
			DevExtsion->ImageFileBuffer = NULL;
		}
	}
	return status;
}

NTSTATUS SWWriteBufferToDriverFile(PWCHAR FileName, PDEVICE_EXTENSION DevExtsion)
{
	UNICODE_STRING unFileName;
	OBJECT_ATTRIBUTES ObjectAttributes;
	FILE_STANDARD_INFORMATION FileInformation;
	FILE_END_OF_FILE_INFORMATION EndOfFileInfo;
	IO_STATUS_BLOCK IoStatusBlock;
	LARGE_INTEGER ByteOffset;
	NTSTATUS status = STATUS_SUCCESS;
	HANDLE hFile    = NULL;
	HANDLE hEvent   = NULL;

	if (DevExtsion->ImageFileBuffer == NULL)
	{
		return STATUS_INVALID_PARAMETER;
	}
	__try
	{
		RtlInitUnicodeString(&unFileName, FileName);
		InitializeObjectAttributes(&ObjectAttributes, &unFileName, OBJ_CASE_INSENSITIVE|OBJ_KERNEL_HANDLE, NULL, NULL);
		status = ZwCreateFile(
			&hFile, 
			GENERIC_WRITE, 
			&ObjectAttributes, 
			&IoStatusBlock, 
			NULL, 
			FILE_ATTRIBUTE_NORMAL, 
			0, 
			FILE_OVERWRITE_IF, 
			FILE_NON_DIRECTORY_FILE|FILE_RANDOM_ACCESS,
			NULL, 
			0);
		if (!NT_SUCCESS(status))
		{
			__leave;
		}
		ZwCreateEvent(&hEvent, GENERIC_ALL, NULL, NotificationEvent, FALSE);
		status = ZwWriteFile(hFile, hEvent, NULL, NULL, &IoStatusBlock, DevExtsion->ImageFileBuffer, DevExtsion->ImageFileSize, (ByteOffset.QuadPart = 0, &ByteOffset), NULL);
		if (status == STATUS_PENDING)
		{
			ZwWaitForSingleObject(hEvent, TRUE, NULL);
			status = STATUS_SUCCESS;
		}
		if (NT_SUCCESS(status) && IoStatusBlock.Information != DevExtsion->ImageFileSize)
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			__leave;
		}
		if (!NT_SUCCESS(status))
		{
			__leave;
		}
		EndOfFileInfo.EndOfFile.QuadPart = DevExtsion->ImageFileSize;
		status = ZwSetInformationFile(hFile, &IoStatusBlock, &EndOfFileInfo, sizeof(FILE_END_OF_FILE_INFORMATION), FileEndOfFileInformation);
	}
	__finally
	{
		if (hFile != NULL)
		{
			ZwClose(hFile);
		}
		if (hEvent != NULL)
		{
			ZwClose(hEvent);
		}
		ExFreePool(DevExtsion->ImageFileBuffer);
		DevExtsion->ImageFileSize   = 0;
		DevExtsion->ImageFileBuffer = NULL;
	}
	return status;
}

NTSTATUS SWCopyDriverFile(PWCHAR SourceFileName, PWCHAR DescPathName, PDEVICE_EXTENSION DevExtsion)
{
	NTSTATUS status;
	status = SWReadDriverFileToBuffer(SourceFileName, DevExtsion);
	if (NT_SUCCESS(status))
	{
		status = SWWriteBufferToDriverFile(DescPathName, DevExtsion);
	}
	return status;
}

NTSTATUS SWPickupDriverFile(PUNICODE_STRING  FullImageName, PDEVICE_EXTENSION DevExtsion)
{
	WCHAR SourceFilePath[MAX_PATH];
	WCHAR DescFilePath[MAX_PATH];
	ULONG i = 0;
	ULONG Length = FullImageName->Length/sizeof(WCHAR);
	for (; i < Length; i++)
	{
		if (FullImageName->Buffer[i] == '\\')
		{
			break;
		}
	}
	if (i == Length) { return STATUS_INVALID_PARAMETER; }
	_snwprintf(SourceFilePath, Length - i, L"%s", &FullImageName->Buffer[i]);
	SourceFilePath[Length - i] = 0;
	wcscpy(DescFilePath, DevExtsion->WorkSavePath);
	if (DescFilePath[wcslen(DescFilePath)-1] == '\\')
	{
		DescFilePath[wcslen(DescFilePath)-1] = 0;
	}
	wcscat(DescFilePath, L"\\DrvAntiBak");
	SWCreateDirectory(DescFilePath);
	wcscat(DescFilePath, wcsrchr(SourceFilePath, '\\'));
	wcscat(DescFilePath, L".bak");
	return SWCopyDriverFile(SourceFilePath, DescFilePath, DevExtsion);
}

NTSTATUS InvalidDriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	return STATUS_ACCESS_DENIED;
}

BOOLEAN DriverLookThrough(PUNICODE_STRING FullImageName, PDEVICE_EXTENSION DevExtsion, ULONG TimeDateStamp, ULONG CheckSum)
{
	BOOLEAN bResult = FALSE;
	WCHAR SourceFilePath[MAX_PATH];
	ULONG i = 0;
	ULONG Length = FullImageName->Length/sizeof(WCHAR);
	DRV_INFO DrvInfo;
	CHAR Md5Digest[128] = {0};

	__try
	{
		for (; i < Length; i++)
		{
			if (FullImageName->Buffer[i] == '\\')
			{
				break;
			}
		}
		if (i == Length) { __leave; }
		_snwprintf(SourceFilePath, Length - i, L"%s", &FullImageName->Buffer[i]);
		SourceFilePath[Length - i] = 0;
		if (!NT_SUCCESS(SWReadDriverFileToBuffer(SourceFilePath, DevExtsion)))
		{
			__leave;
		}
		md5(DevExtsion->ImageFileBuffer, DevExtsion->ImageFileSize, Md5Digest);
		memcpy(DrvInfo.Md5Digest, Md5Digest, sizeof(DrvInfo.Md5Digest));
		if (!LookupHashTable(DrvInfo))
		{
			__leave;
		}
		if (DrvInfo.CheckSum == CheckSum && DrvInfo.TimeDateStamp == TimeDateStamp)
		{
			bResult = TRUE;
		}
	}
	__finally
	{
		if (DevExtsion->ImageFileBuffer != NULL)
		{
			ExFreePool(DevExtsion->ImageFileBuffer);
			DevExtsion->ImageFileSize   = 0;
			DevExtsion->ImageFileBuffer = NULL;
		}
	}
	return bResult;
}

VOID SWSysMonNotifyRoutine (IN PUNICODE_STRING FullImageName, IN HANDLE ProcessId, IN PIMAGE_INFO ImageInfo)
{
	ULONG uDr0;
	USHORT wFlagRegValue;
	LARGE_INTEGER WaitTime;
	PIMAGE_NT_HEADERS ImageNtHead;
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)GlobalDeviceObject->DeviceExtension;
	if (!DevExtsion->StartSysMon) { return; }
	if (ImageInfo->SystemModeImage == 1 && MmIsAddressValid(ImageInfo->ImageBase) && (ULONG)ImageInfo->ImageBase > *MmUserProbeAddress)
	{
		KIRQL Irql;
		ANSI_STRING anPath;
		ImageNtHead = RtlImageNtHeader(ImageInfo->ImageBase);
		if (!MmIsAddressValid(ImageNtHead)) { return; }
		//验证驱动是不是在白名单中
		if (DriverLookThrough(FullImageName, DevExtsion, ImageNtHead->FileHeader.TimeDateStamp, ImageNtHead->OptionalHeader.CheckSum))
		{
			return; //驱动在白名单中,允许加载
		}
		RtlUnicodeStringToAnsiString(&anPath, FullImageName, TRUE);
		KeAcquireSpinLock(&DevExtsion->SpinLock, &Irql);
		_snprintf(DevExtsion->CaptureDriverPathName, anPath.Length/sizeof(CHAR), "%s", anPath.Buffer);
		DevExtsion->CaptureDriverPathName[anPath.Length/sizeof(CHAR)] = 0;
		KeReleaseSpinLock(&DevExtsion->SpinLock, Irql);
		RtlFreeAnsiString(&anPath);

		KeSetEvent(DevExtsion->obDriverEntryEvent, IO_NO_INCREMENT, FALSE);
		KeClearEvent(DevExtsion->obDriverEntryEvent);

		KeWaitForSingleObject(DevExtsion->obDriverClearedEvent, Executive, IO_NO_INCREMENT, FALSE, (WaitTime.QuadPart = DELAY_ONE_SECOND*3, &WaitTime));
		KeClearEvent(DevExtsion->obDriverClearedEvent);

		SWPickupDriverFile(FullImageName, (PDEVICE_EXTENSION)GlobalDeviceObject->DeviceExtension);

		DisableIntermitAndSaveFlagRegister(&wFlagRegValue);
		DisableWriteProtect(&uDr0);
		ImageNtHead->OptionalHeader.AddressOfEntryPoint = (ULONG)InvalidDriverEntry - (ULONG)ImageInfo->ImageBase;
		__asm
		{
			push eax
			mov eax, uDr0
			mov cr0, eax
			pop eax
			mov ax, wFlagRegValue
			push ax
			popf //popfw
		}
		DbgPrint("%wZ \n", FullImageName);
	}
}

NTSTATUS InstallSysMonHandler()
{
	NTSTATUS status;
	status = PsSetLoadImageNotifyRoutine(SWSysMonNotifyRoutine);
	if (!NT_SUCCESS(status))
	{
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	return status;
}

VOID UnInstallSysMonHandler()
{
	PsRemoveLoadImageNotifyRoutine(SWSysMonNotifyRoutine);
}
