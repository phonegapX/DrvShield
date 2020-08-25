#ifndef __STD_H__
#define __STD_H__

#ifdef  __cplusplus
extern "C" 
{
#endif

#include <ntddk.h>
#include <ntimage.h>
#include <stdio.h>

NTSYSAPI
	NTSTATUS
	NTAPI
	ZwCreateEvent (
	OUT PHANDLE EventHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
	IN EVENT_TYPE EventType,
	IN BOOLEAN InitialState
	);

NTSYSAPI
	NTSTATUS
	NTAPI
	ZwWaitForSingleObject(
	IN HANDLE Handle,
	IN BOOLEAN Alertable,
	IN PLARGE_INTEGER Timeout OPTIONAL
	);

NTSYSAPI
	PIMAGE_NT_HEADERS
	NTAPI
	RtlImageNtHeader(
	IN PVOID ImageBase
	);

int __cdecl _snwprintf(wchar_t *, size_t, const wchar_t *, ...);
int __cdecl _snprintf(char *, size_t, const char *, ...);

#ifdef  __cplusplus
}
#endif

#include "common.h"

#define DELAY_ONE_MICROSECOND				(-10)
#define DELAY_ONE_MILLISECOND				(DELAY_ONE_MICROSECOND*1000)
#define DELAY_ONE_SECOND					(DELAY_ONE_MILLISECOND*1000)

#define MAX_PROCESS_PROTECT_COUNT 256

typedef struct _PROCESS_PROTECT
{
	ULONG Count;
	ULONG ProcessID[MAX_PROCESS_PROTECT_COUNT];
} PROCESS_PROTECT, *PPROCESS_PROTECT;

typedef struct _DEVICE_EXTENSION
{
	KSPIN_LOCK       SpinLock;
	PDRIVER_OBJECT   DriverObject;
	PKEVENT          obDriverEntryEvent;
	HANDLE           hDriverEntryEvent;
	PKEVENT          obDriverProcessedEvent;
	HANDLE           hDriverProcessedEvent;
	PKEVENT          obDriverClearedEvent;
	HANDLE           hDriverClearedEvent;
	PPHYSICAL_MEMORY_RANGE PhysicalMemoryRange;
	PROCESS_PROTECT	 ProcessProtectList;		//要保护的进程ID列表
	PDRV_ANTI_DATA   DrvAntiData;				//允许加载的驱动白名单
	BOOLEAN          DeviceOpen;				//设备是否打开
	BOOLEAN          StartSysMon;				//是否开始监控驱动
	BOOLEAN          StartHook;					//是否启用HOOK的SSDT函数
	PVOID            ImageFileBuffer;			//存放要备份的驱动文件的内容
	ULONG            ImageFileSize;				//驱动文件长度
	WCHAR            WorkSavePath[MAX_PATH];	//工作目录
	CHAR             CaptureDriverPathName[MAX_PATH]; //用于应用程序查询的被拦截的驱动程序全路径名
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

#ifdef  DECLARE_UNICODE_STRING
#undef  DECLARE_UNICODE_STRING
#endif
#define DECLARE_UNICODE_STRING(_variablename, _string) \
	WCHAR          _variablename ## _buffer[] = _string; \
	UNICODE_STRING _variablename = { sizeof(_string) - sizeof(WCHAR), sizeof(_string), (PWSTR)_variablename ## _buffer };

EXTERN_C PDEVICE_OBJECT GlobalDeviceObject;

#endif
