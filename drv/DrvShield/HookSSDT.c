#include "std.h"
#include "HookSSDT.h"

typedef enum _SYSDBG_COMMAND 
{
	//以下5个在Windows NT各个版本上都有
	SysDbgGetTraceInformation = 1,
	SysDbgSetInternalBreakpoint = 2,
	SysDbgSetSpecialCall = 3,
	SysDbgClearSpecialCalls = 4,
	SysDbgQuerySpecialCalls = 5,

	// 以下是NT 5.1 新增的
	SysDbgDbgBreakPointWithStatus = 6,

	//获取KdVersionBlock
	SysDbgSysGetVersion = 7,

	//从内核空间拷贝到用户空间，或者从用户空间拷贝到用户空间
	//但是不能从用户空间拷贝到内核空间
	SysDbgCopyMemoryChunks_0 = 8,
	//SysDbgReadVirtualMemory = 8,

	//从用户空间拷贝到内核空间，或者从用户空间拷贝到用户空间
	//但是不能从内核空间拷贝到用户空间
	SysDbgCopyMemoryChunks_1 = 9,
	//SysDbgWriteVirtualMemory = 9,

	//从物理地址拷贝到用户空间，不能写到内核空间
	SysDbgCopyMemoryChunks_2 = 10,
	//SysDbgReadVirtualMemory = 10,

	//从用户空间拷贝到物理地址，不能读取内核空间
	SysDbgCopyMemoryChunks_3 = 11,
	//SysDbgWriteVirtualMemory = 11,

	//读写处理器相关控制块
	SysDbgSysReadControlSpace = 12,
	SysDbgSysWriteControlSpace = 13,

	//读写端口
	SysDbgSysReadIoSpace = 14,
	SysDbgSysWriteIoSpace = 15,

	//分别调用RDMSR@4和_WRMSR@12
	SysDbgSysReadMsr = 16,
	SysDbgSysWriteMsr = 17,

	//读写总线数据
	SysDbgSysReadBusData = 18,
	SysDbgSysWriteBusData = 19,

	SysDbgSysCheckLowMemory = 20,

	// 以下是NT 5.2 新增的

	//分别调用_KdEnableDebugger@0和_KdDisableDebugger@0
	SysDbgEnableDebugger = 21,
	SysDbgDisableDebugger = 22,

	//获取和设置一些调试相关的变量
	SysDbgGetAutoEnableOnEvent = 23,
	SysDbgSetAutoEnableOnEvent = 24,
	SysDbgGetPitchDebugger = 25,
	SysDbgSetDbgPrintBufferSize = 26,
	SysDbgGetIgnoreUmExceptions = 27,
	SysDbgSetIgnoreUmExceptions = 28
} SYSDBG_COMMAND, *PSYSDBG_COMMAND;

NTSYSAPI
NTSTATUS
NTAPI
NtSystemDebugControl (
					  IN SYSDBG_COMMAND Command,
					  IN PVOID InputBuffer,
					  IN ULONG InputBufferLength,
					  OUT PVOID OutputBuffer,
					  IN ULONG OutputBufferLength,
					  OUT PULONG ReturnLength
					  );

typedef enum _THREAD_STATE
{
	StateInitialized,
	StateReady,
	StateRunning,
	StateStandby,
	StateTerminated,
	StateWait,
	StateTransition,
	StateUnknown
} THREAD_STATE;

typedef struct _SYSTEM_THREADS
{
	LARGE_INTEGER KernelTime;               //CPU内核模式使用时间；
	LARGE_INTEGER UserTime;                 //CPU用户模式使用时间；
	LARGE_INTEGER CreateTime;               //线程创建时间；
	ULONG         WaitTime;                 //等待时间；
	PVOID         StartAddress;             //线程开始的虚拟地址；
	CLIENT_ID     ClientId;                 //线程标识符；
	KPRIORITY     Priority;                 //线程优先级；
	KPRIORITY     BasePriority;             //基本优先级；
	ULONG         ContextSwitchCount;       //环境切换数目；
	THREAD_STATE  State;                    //当前状态；
	KWAIT_REASON  WaitReason;               //等待原因；
} SYSTEM_THREADS, *PSYSTEM_THREADS;

typedef struct _SYSTEM_PROCESSES
{
	ULONG          NextEntryDelta;          //构成结构序列的偏移量；
	ULONG          ThreadCount;             //线程数目；
	ULONG          Reserved1[6];           
	LARGE_INTEGER  CreateTime;              //创建时间；
	LARGE_INTEGER  UserTime;                //用户模式(Ring 3)的CPU时间；
	LARGE_INTEGER  KernelTime;              //内核模式(Ring 0)的CPU时间；
	UNICODE_STRING ProcessName;             //进程名称；
	KPRIORITY      BasePriority;            //进程优先权；
	ULONG          ProcessId;               //进程标识符；
	ULONG          InheritedFromProcessId;  //父进程的标识符；
	ULONG          HandleCount;             //句柄数目；
	ULONG          Reserved2[2];
	VM_COUNTERS    VmCounters;              //虚拟存储器的结构，见下；
	IO_COUNTERS    IoCounters;              //IO计数结构，见下；
	SYSTEM_THREADS Threads[1];              //进程相关线程的结构数组，见下；
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemNotImplemented1,
	SystemProcessesAndThreadsInformation,
	SystemCallCounts,
	SystemConfigurationInformation,
	SystemProcessorTimes,
	SystemGlobalFlag,
	SystemNotImplemented2,
	SystemModuleInformation,
	SystemLockInformation,
	SystemNotImplemented3,
	SystemNotImplemented4,
	SystemNotImplemented5,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPagefileInformation,
	SystemInstructionEmulationCounts,
	SystemInvalidInfoClass1,
	SystemCacheInformation,
	SystemPoolTagInformation,
	SystemProcessorStatistics,
	SystemDpcInformation,
	SystemNotImplemented6,
	SystemLoadImage,
	SystemUnloadImage,
	SystemTimeAdjustment,
	SystemNotImplemented7,
	SystemNotImplemented8,
	SystemNotImplemented9,
	SystemCrashDumpInformation,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemLoadAndCallImage,
	SystemPrioritySeparation,
	SystemNotImplemented10,
	SystemNotImplemented11,
	SystemInvalidInfoClass2,
	SystemInvalidInfoClass3,
	SystemTimeZoneInformation,
	SystemLookasideInformation,
	SystemSetTimeSlipEvent,
	SystemCreateSession,
	SystemDeleteSession,
	SystemInvalidInfoClass4,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemAddVerifier,
	SystemSessionProcessesInformation
} SYSTEM_INFORMATION_CLASS;

NTSYSAPI
NTSTATUS
NTAPI
NtQuerySystemInformation(
						 IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
						 IN OUT PVOID SystemInformation,
						 IN ULONG SystemInformationLength,
						 OUT PULONG ReturnLength OPTIONAL
						 );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryDirectoryObject(
					   IN HANDLE DirectoryHandle,
					   OUT PVOID Buffer,
					   IN ULONG BufferLength,
					   IN BOOLEAN ReturnSingleEntry,
					   IN BOOLEAN RestartScan,
					   IN OUT PULONG Context,
					   OUT PULONG ReturnLength OPTIONAL
					   );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationProcess (
						   IN HANDLE           ProcessHandle,
						   IN PROCESSINFOCLASS ProcessInformationClass,
						   OUT PVOID           ProcessInformation,
						   IN ULONG            ProcessInformationLength,
						   OUT PULONG          ReturnLength OPTIONAL
						   );

NTSYSCALLAPI
NTSTATUS
NTAPI
NtClose(
		IN HANDLE Handle
		);

typedef enum _OBJECT_INFORMATION_CLASS
{ 
	ObjectBasicInformation, 
	ObjectNameInformation, 
	ObjectTypeInformation, 
	ObjectAllTypesInformation, 
	ObjectHandleInformation 
} OBJECT_INFORMATION_CLASS;

NTSTATUS ZwQueryObject(
					   IN HANDLE ObjectHandle,
					   IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
					   OUT PVOID ObjectInformation,
					   IN ULONG ObjectInformationLength,
					   OUT PULONG ReturnLength OPTIONAL
					   );

typedef struct _DIRECTORY_BASIC_INFORMATION 
{
	UNICODE_STRING ObjectName;
	UNICODE_STRING ObjectTypeName;
} DIRECTORY_BASIC_INFORMATION, *PDIRECTORY_BASIC_INFORMATION;

/////////////////////////////////////////////////////////////////////////////////////////

VOID AddProcessProtectList(ULONG ProcessID)
{
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)GlobalDeviceObject->DeviceExtension;
	PPROCESS_PROTECT ProcessProtectList = &DevExtsion->ProcessProtectList;
	if (ProcessProtectList->Count >= MAX_PROCESS_PROTECT_COUNT - 1) { return; }
	ProcessProtectList->ProcessID[ProcessProtectList->Count] = ProcessID;
	ProcessProtectList->Count++;
}

BOOLEAN ExistProcessIdInProtectList(ULONG ProcessID)
{
	ULONG i;
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)GlobalDeviceObject->DeviceExtension;
	PPROCESS_PROTECT ProcessProtectList = &DevExtsion->ProcessProtectList;
	if (ProcessID == 0) { return FALSE; }
	for (i = 0; i < ProcessProtectList->Count; i++)
	{
		if (i >= MAX_PROCESS_PROTECT_COUNT) { return FALSE; }
		if(ProcessProtectList->ProcessID[i] == ProcessID)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
typedef NTSTATUS (NTAPI *NTPROC)();
typedef NTPROC * PNTPROC;

NTPROC fpNtOpenProcess;
NTPROC fpNtTerminateProcess;
NTPROC fpNtQuerySystemInformation;
NTPROC fpNtSystemDebugControl;
NTPROC fpNtMapViewOfSection;
NTPROC fpNtQueryDirectoryObject;

ULONG dwNtOpenProcess;
ULONG dwNtTerminateProcess;
ULONG dwNtQuerySystemInformation;
ULONG dwNtSystemDebugControl;
ULONG dwNtMapViewOfSection;
ULONG dwNtQueryDirectoryObject;

typedef struct _KSERVICE_TABLE_DESCRIPTOR 
{
	PNTPROC ServiceTable;
	PULONG CounterTable;
	ULONG ServiceLimit;
	PUCHAR ArgumentTable;
} KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;

#define SystemService(_function) KeServiceDescriptorTable->ServiceTable[_function]

extern PKSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;	//ntoskrnl.exe导出的符号

/*
防止Ring3层进Ring0层
*/
NTSTATUS SWNtSystemDebugControl (
								 IN SYSDBG_COMMAND Command,
								 IN PVOID InputBuffer,
								 IN ULONG InputBufferLength,
								 OUT PVOID OutputBuffer,
								 IN ULONG OutputBufferLength,
								 OUT PULONG ReturnLength
								 )
{
	switch(Command)
	{
	case SysDbgCopyMemoryChunks_0:
	case SysDbgCopyMemoryChunks_1:
	case SysDbgCopyMemoryChunks_2:
	case SysDbgCopyMemoryChunks_3:
	case SysDbgSysWriteControlSpace:
	case SysDbgSysWriteIoSpace:
	case SysDbgSysWriteMsr:
	case SysDbgSysWriteBusData:
	case SysDbgSysReadBusData:
		return STATUS_ACCESS_DENIED;
	}
	return fpNtSystemDebugControl(Command, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength, ReturnLength);
}

/*
防止Ring3层进Ring0层
*/
NTSTATUS SWNtMapViewOfSection(
							  IN HANDLE SectionHandle,
							  IN HANDLE ProcessHandle,
							  IN OUT PVOID *BaseAddress,
							  IN ULONG ZeroBits,
							  IN ULONG CommitSize,
							  IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
							  IN OUT PSIZE_T ViewSize,
							  IN SECTION_INHERIT InheritDisposition,
							  IN ULONG AllocationType,
							  IN ULONG Protect
							  )
{
	NTSTATUS status;
	UNICODE_STRING unMemoryDeviceName;
	WCHAR Buffer[1024] = {0};
	POBJECT_NAME_INFORMATION ObjectNameInfo = (POBJECT_NAME_INFORMATION)Buffer;
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)GlobalDeviceObject->DeviceExtension;
	RtlInitUnicodeString(&unMemoryDeviceName, L"\\Device\\PhysicalMemory");
	status = ZwQueryObject(SectionHandle, ObjectNameInformation, ObjectNameInfo, 1024, NULL);
	if (NT_SUCCESS(status))
	{
		if (RtlCompareUnicodeString(&ObjectNameInfo->Name, &unMemoryDeviceName, TRUE) == 0)
		{
			PPHYSICAL_MEMORY_RANGE PhysicalMemoryRange = DevExtsion->PhysicalMemoryRange;
			for (; PhysicalMemoryRange->BaseAddress.QuadPart!=0&&PhysicalMemoryRange->NumberOfBytes.QuadPart!=0; PhysicalMemoryRange++)
			{
				if (
					SectionOffset->QuadPart >= PhysicalMemoryRange->BaseAddress.QuadPart ||
					SectionOffset->QuadPart <  PhysicalMemoryRange->BaseAddress.QuadPart + PhysicalMemoryRange->NumberOfBytes.QuadPart
					)
				{
					return STATUS_ACCESS_VIOLATION;
				}
			}
		}
	}
	return fpNtMapViewOfSection(SectionHandle, ProcessHandle, BaseAddress, ZeroBits, CommitSize, SectionOffset, ViewSize, InheritDisposition, AllocationType, Protect);
}

/*
隐藏防火墙主进程
*/
NTSTATUS SWNtQuerySystemInformation(
									IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
									IN OUT PVOID SystemInformation,
									IN ULONG SystemInformationLength,
									OUT PULONG ReturnLength OPTIONAL
									)
{
	NTSTATUS status;
	BOOLEAN bExist;
	PSYSTEM_PROCESSES PreEntry = NULL;
	PSYSTEM_PROCESSES CurEntry = NULL;
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)GlobalDeviceObject->DeviceExtension;
	status = fpNtQuerySystemInformation(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	if (!DevExtsion->StartHook)
	{
		return status;
	}
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	if (SystemInformationClass != SystemProcessesAndThreadsInformation)
	{
		return status;
	}
	CurEntry = SystemInformation;
	while (TRUE)
	{
		if (PreEntry != NULL)
		{
			KIRQL Irql;
			KeAcquireSpinLock(&DevExtsion->SpinLock, &Irql);
			bExist = ExistProcessIdInProtectList(CurEntry->ProcessId);
			KeReleaseSpinLock(&DevExtsion->SpinLock, Irql);
			if (bExist) //这个进程要隐藏
			{
				if (CurEntry->NextEntryDelta != 0)
				{
					ULONG uEntryDelta;
					PreEntry->NextEntryDelta += CurEntry->NextEntryDelta;
					uEntryDelta = CurEntry->NextEntryDelta;
					memset(CurEntry, 0, uEntryDelta);
					CurEntry->NextEntryDelta = uEntryDelta;
				}
				else
				{
					PreEntry->NextEntryDelta = 0;
				}
			}
		}
		if (CurEntry->NextEntryDelta == 0)
		{
			break;
		}
		PreEntry = CurEntry;
		CurEntry = ((PCHAR)CurEntry) + CurEntry->NextEntryDelta;
	}
	return status;
}

/*
防止防火墙主进程被打开
*/
NTSTATUS SWNtOpenProcess (
						  OUT PHANDLE ProcessHandle,
						  IN ACCESS_MASK DesiredAccess,
						  IN POBJECT_ATTRIBUTES ObjectAttributes,
						  IN PCLIENT_ID ClientId OPTIONAL
						  )
{
	BOOLEAN bExist;
	NTSTATUS status;
	ULONG uHostProcessId;
	ULONG uOpenProcessId;
	PROCESS_BASIC_INFORMATION BasicInfo;
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)GlobalDeviceObject->DeviceExtension;
	status = fpNtOpenProcess(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId);
	if (!DevExtsion->StartHook)
	{
		return status;
	}
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	status = ZwQueryInformationProcess(*ProcessHandle, ProcessBasicInformation, &BasicInfo, sizeof(BasicInfo), NULL);
	if (status == STATUS_SUCCESS)
	{
		uOpenProcessId = BasicInfo.UniqueProcessId;
		status = ZwQueryInformationProcess(NtCurrentProcess(), ProcessBasicInformation, &BasicInfo, sizeof(BasicInfo), NULL);
		if (status == STATUS_SUCCESS)
		{
			uHostProcessId = BasicInfo.UniqueProcessId;
			if (uHostProcessId != uOpenProcessId)
			{
				KIRQL Irql;
				KeAcquireSpinLock(&DevExtsion->SpinLock, &Irql);
				bExist = ExistProcessIdInProtectList(uOpenProcessId);
				KeReleaseSpinLock(&DevExtsion->SpinLock, Irql);
				if (bExist)
				{
					NtClose(*ProcessHandle);
					return STATUS_ACCESS_DENIED;
				}
			}
		}
	}
	return status;
}

/*
防止防火墙主进程被结束
*/
NTSTATUS SWNtTerminateProcess( 
							  IN HANDLE ProcessHandle OPTIONAL, 
							  IN NTSTATUS ExitStatus 
							  )
{
	BOOLEAN bExist;
	NTSTATUS status;
	ULONG uHostProcessId;
	ULONG uKillProcessId;
	PROCESS_BASIC_INFORMATION BasicInfo;
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)GlobalDeviceObject->DeviceExtension;
	if (!DevExtsion->StartHook)
	{
		return fpNtTerminateProcess(ProcessHandle, ExitStatus);
	}
	status = ZwQueryInformationProcess(ProcessHandle, ProcessBasicInformation, &BasicInfo, sizeof(BasicInfo), NULL);
	if (status == STATUS_SUCCESS)
	{
		uKillProcessId = BasicInfo.UniqueProcessId;
		status = ZwQueryInformationProcess(NtCurrentProcess(), ProcessBasicInformation, &BasicInfo, sizeof(BasicInfo), NULL);
		if (status == STATUS_SUCCESS)
		{
			uHostProcessId = BasicInfo.UniqueProcessId;
			if (uHostProcessId != uKillProcessId)
			{
				KIRQL Irql;
				KeAcquireSpinLock(&DevExtsion->SpinLock, &Irql);
				bExist = ExistProcessIdInProtectList(uKillProcessId);
				KeReleaseSpinLock(&DevExtsion->SpinLock, Irql);
				if (bExist)
				{
					return STATUS_ACCESS_DENIED;
				}
			}
		}
	}
	return fpNtTerminateProcess(ProcessHandle, ExitStatus);
}

#ifdef  DECLARE_UNICODE_STRING
#undef  DECLARE_UNICODE_STRING
#endif
#define DECLARE_UNICODE_STRING(_variablename, _string) \
	WCHAR          _variablename ## _buffer[] = _string; \
	UNICODE_STRING _variablename = { sizeof(_string) - sizeof(WCHAR), sizeof(_string), (PWSTR)_variablename ## _buffer };

NTSTATUS SWNtQueryDirectoryObject(
								  IN HANDLE DirectoryHandle,
								  OUT PVOID Buffer,
								  IN ULONG BufferLength,
								  IN BOOLEAN ReturnSingleEntry,
								  IN BOOLEAN RestartScan,
								  IN OUT PULONG Context,
								  OUT PULONG ReturnLength OPTIONAL
								  )
{
	NTSTATUS status;
	ULONG uNewInfoLength;
	PDIRECTORY_BASIC_INFORMATION NewDirBasicInfo = NULL;
	PDIRECTORY_BASIC_INFORMATION TempInfo = Buffer;
	PDEVICE_EXTENSION DevExtsion = (PDEVICE_EXTENSION)GlobalDeviceObject->DeviceExtension;
	WCHAR NameBuffer[1024] = {0};
	POBJECT_NAME_INFORMATION ObjectNameInfo = (POBJECT_NAME_INFORMATION)NameBuffer;
	DECLARE_UNICODE_STRING(unDriverDirectoryName, L"\\Driver")
	//DECLARE_UNICODE_STRING(unDeviceDirectoryName, L"\\Device")
	DECLARE_UNICODE_STRING(unHarddiskDirectoryName, L"Harddisk0")
	DECLARE_UNICODE_STRING(unHarddiskFDirectoryName, L"\\Device\\Harddisk0")
	DECLARE_UNICODE_STRING(unSymLinkXpDirectoryName, L"\\GLOBAL??")
	DECLARE_UNICODE_STRING(unSymLink2000DirectoryName, L"\\??")
	DECLARE_UNICODE_STRING(unDiskDriverName, L"Disk")
	DECLARE_UNICODE_STRING(unAtapiDriverName, L"atapi")
	DECLARE_UNICODE_STRING(unFtdiskDriverName, L"Ftdisk")
	DECLARE_UNICODE_STRING(unBoostdskDriverName, L"boostdsk")
	DECLARE_UNICODE_STRING(unDr0DeviceName, L"DR0")
	DECLARE_UNICODE_STRING(unPhysicalDriveSymlinkName, L"PhysicalDrive0")
	DECLARE_UNICODE_STRING(unDrvShieldDriverName, L"DrvShield")
	DECLARE_UNICODE_STRING(unSysMonName, L"KeydoneSysMon")
	__try
	{
		status = fpNtQueryDirectoryObject(DirectoryHandle, Buffer, BufferLength, ReturnSingleEntry, RestartScan, Context, ReturnLength);
		if (!NT_SUCCESS(status))
		{
			__leave;
		}
		status = ZwQueryObject(DirectoryHandle, ObjectNameInformation, ObjectNameInfo, 1024, NULL);
		if (!NT_SUCCESS(status))
		{
			__leave;
		}
		uNewInfoLength = BufferLength < 1024*1024*5 ? BufferLength : 1024*1024*5;
		NewDirBasicInfo = (PDIRECTORY_BASIC_INFORMATION)ExAllocatePool(NonPagedPool, uNewInfoLength);
		if (NewDirBasicInfo == NULL)
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			__leave;
		}
		memset(NewDirBasicInfo, 0, uNewInfoLength);
		if (RtlCompareUnicodeString(&ObjectNameInfo->Name, &unDriverDirectoryName, TRUE) == 0) //查询的是驱动目录
		{
			ULONG i = 0;
			for (; TempInfo->ObjectName.Length != 0 && TempInfo->ObjectTypeName.Length != 0; TempInfo++)
			{
				if (
					RtlCompareUnicodeString(&TempInfo->ObjectName, &unDiskDriverName, TRUE)  != 0    &&
					RtlCompareUnicodeString(&TempInfo->ObjectName, &unAtapiDriverName, TRUE) != 0    &&
					RtlCompareUnicodeString(&TempInfo->ObjectName, &unFtdiskDriverName, TRUE) != 0   &&
					RtlCompareUnicodeString(&TempInfo->ObjectName, &unBoostdskDriverName, TRUE) != 0 &&
					RtlCompareUnicodeString(&TempInfo->ObjectName, &unDrvShieldDriverName, TRUE) != 0
					)
				{
					NewDirBasicInfo[i] = *TempInfo;
					i++;
				}
			}
			memcpy(Buffer, NewDirBasicInfo, i * sizeof(DIRECTORY_BASIC_INFORMATION));
		}
		else if (																				//查询的是Harddisk0目录
			RtlCompareUnicodeString(&ObjectNameInfo->Name, &unHarddiskDirectoryName, TRUE) == 0 ||
			RtlCompareUnicodeString(&ObjectNameInfo->Name, &unHarddiskFDirectoryName, TRUE) == 0
			) 
		{
			ULONG i = 0;
			for (; TempInfo->ObjectName.Length != 0 && TempInfo->ObjectTypeName.Length != 0; TempInfo++)
			{
				if (RtlCompareUnicodeString(&TempInfo->ObjectName, &unDr0DeviceName, TRUE) != 0)
				{
					NewDirBasicInfo[i] = *TempInfo;
					i++;
				}
			}
			memcpy(Buffer, NewDirBasicInfo, i * sizeof(DIRECTORY_BASIC_INFORMATION));
		}
		else if (																					//查询的是符号连接目录
			RtlCompareUnicodeString(&ObjectNameInfo->Name, &unSymLinkXpDirectoryName, TRUE) == 0 || 
			RtlCompareUnicodeString(&ObjectNameInfo->Name, &unSymLink2000DirectoryName, TRUE) == 0 
			)
		{
			ULONG i = 0;
			for (; TempInfo->ObjectName.Length != 0 && TempInfo->ObjectTypeName.Length != 0; TempInfo++)
			{
				if (
					RtlCompareUnicodeString(&TempInfo->ObjectName, &unPhysicalDriveSymlinkName, TRUE) != 0 &&
					RtlCompareUnicodeString(&TempInfo->ObjectName, &unSysMonName, TRUE) != 0
					)
				{
					NewDirBasicInfo[i] = *TempInfo;
					i++;
				}
			}
			memcpy(Buffer, NewDirBasicInfo, i * sizeof(DIRECTORY_BASIC_INFORMATION));
		}
	}
	__finally
	{
		if (NewDirBasicInfo != NULL)
		{
			ExFreePool(NewDirBasicInfo);
		}
	}
	return status;
}

VOID DisableWriteProtect(PULONG Dr0)
{
	ULONG uDr0;
	__asm
	{
		push eax
		mov eax, cr0
		mov uDr0, eax
		and eax, 0FFFEFFFFh
		mov cr0, eax
		pop eax
	}
	*Dr0 = uDr0;
}

VOID DisableIntermitAndSaveFlagRegister(PUSHORT FlagRegValue)
{
	USHORT uResult;
	__asm
	{
		pushf //pushfw
		cli
		pop ax
		mov uResult, ax
	}
	*FlagRegValue = uResult;
}

NTSTATUS SWHookInit()
{
	NTSTATUS status;
	ULONG MajorVersion;
	ULONG MinorVersion;
	ULONG BuildNumber;
	PsGetVersion(&MajorVersion, &MinorVersion, &BuildNumber, NULL);
	if (MajorVersion != 5)
	{
		return STATUS_NOT_SUPPORTED;
	}
	if (MinorVersion == 0)
	{
		dwNtOpenProcess = 0x6A;
		dwNtTerminateProcess = 0x0E0;
		dwNtQuerySystemInformation = 0x97;
		dwNtSystemDebugControl = 0x0DE;
		dwNtMapViewOfSection = 0x5D;
		dwNtQueryDirectoryObject = 0x7E;
	}
	else if (MinorVersion == 1)
	{
		dwNtOpenProcess = 0x7A; 
		dwNtTerminateProcess = 0x101; 
		dwNtQuerySystemInformation = 0x0AD; 
		dwNtSystemDebugControl = 0x0FF; 
		dwNtMapViewOfSection = 0x6C; 
		dwNtQueryDirectoryObject = 0x92; 
	}
	else if (MinorVersion == 2)
	{
		dwNtOpenProcess = 0x81;
		dwNtTerminateProcess = 0x10A;
		dwNtQuerySystemInformation = 0x0B5;
		dwNtSystemDebugControl = 0x108;
		dwNtMapViewOfSection = 0x71;
		dwNtQueryDirectoryObject = 0x98;
	}
	else
	{
		return STATUS_NOT_SUPPORTED;
	}
	return STATUS_SUCCESS;
}

VOID SWHookSSDT()
{
	ULONG uDr0;
	USHORT wFlagRegValue;
	DisableIntermitAndSaveFlagRegister(&wFlagRegValue);
	DisableWriteProtect(&uDr0);
	fpNtOpenProcess = SystemService(dwNtOpenProcess);
	SystemService(dwNtOpenProcess) = SWNtOpenProcess;
	fpNtTerminateProcess = SystemService(dwNtTerminateProcess);
	SystemService(dwNtTerminateProcess) = SWNtTerminateProcess;
	fpNtQuerySystemInformation = SystemService(dwNtQuerySystemInformation);
	SystemService(dwNtQuerySystemInformation) = SWNtQuerySystemInformation;
	fpNtSystemDebugControl= SystemService(dwNtSystemDebugControl);
	SystemService(dwNtSystemDebugControl) = SWNtSystemDebugControl;
	fpNtMapViewOfSection = SystemService(dwNtMapViewOfSection);
	SystemService(dwNtMapViewOfSection) = SWNtMapViewOfSection;
	fpNtQueryDirectoryObject= SystemService(dwNtQueryDirectoryObject);
	SystemService(dwNtQueryDirectoryObject) = SWNtQueryDirectoryObject;
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
}
