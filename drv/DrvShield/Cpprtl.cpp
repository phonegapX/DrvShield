#include "std.h"
#include "Cpprtl.h"

//========================================================================
// 重载C++内存操作函数
inline void __cdecl operator delete(void* p)
{
	if (p) ExFreePool(p);
}

inline void __cdecl operator delete [] (void* p)
{
	if (p) ExFreePool(p);
}

inline void * __cdecl operator new(size_t nSize)
{
	return nSize ? ExAllocatePool(NonPagedPool, nSize) : NULL;
}

inline void * __cdecl operator new [] (size_t nSize)
{
	return nSize ? ExAllocatePool(NonPagedPool, nSize) : NULL;
}
//========================================================================

typedef void (__cdecl *_PVFV)(void);

#define _ATTRIBUTES read,write

#pragma section(".CRT$XCA", long, _ATTRIBUTES)
#pragma section(".CRT$XCZ", long, _ATTRIBUTES)

#define _CRTALLOC(x) __declspec(allocate(x))

//编译器产生的所有全局对象的构造函数列表将会出现在下面2个变量的中间

_CRTALLOC(".CRT$XCA") _PVFV __xc_a[] = { NULL };

_CRTALLOC(".CRT$XCZ") _PVFV __xc_z[] = { NULL };

static void __cdecl _initterm (_PVFV * pfbegin, _PVFV * pfend)
{
	while (pfbegin < pfend)
	{
		if (*pfbegin != NULL)
			(**pfbegin)();	//调用构造函数
		++pfbegin;
	}
}

static void InitializeCppRunTime()
{
	_initterm(__xc_a, __xc_z);
}

////////////////////////////////////////////////////////////////////////
// AtExitCall
//
// This object is used to maintain a list of functions to be called
// at unload time.
class AtExitCall
{
public:
	// The constructor links the object into the global list

	AtExitCall(void ( __cdecl *func )( void ))
	{
		m_Function = func;
		m_Next = m_ListHead;
		m_ListHead = this;
	}

	// The destructor removes the head of the list and calls the function.

	~AtExitCall(void)
	{
		m_Function();
		m_ListHead = this->m_Next;
	}

	// link to next list item
	AtExitCall * m_Next;

	// function pointer
	void ( __cdecl * m_Function)( void );

	// head of the list
	static AtExitCall * m_ListHead;;
};

AtExitCall * AtExitCall::m_ListHead = NULL;

/*
对于全局对象的构造函数,编译器会自动在构造函数中间加入对下面函数调用的代码,函数参数就是这个对象析构函数的指针
下面这个函数将析构函数的指针保存下来，在程序退出的时候将调用它们,来达到程序退出的时候可以析构全局对象的目的.
*/
int __cdecl atexit(void ( __cdecl *func )( void ))
{
	// Allocate an AtExitCall object
	int i = (new AtExitCall(func) == NULL) ? 0 : 1;
	// Check if the allocation failed
	if (i==0) 
	{
		(*func)();
	}
	return i;
}

static void TerminateCppRunTime()
{
	// call termination handlers
	while (AtExitCall::m_ListHead)
	{
		// invoking the destructor causes the termination handler to be called
		delete AtExitCall::m_ListHead;
	}
}

static NTSTATUS IopInvalidDeviceRequest(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	UNREFERENCED_PARAMETER( DeviceObject );
	if ((IoGetCurrentIrpStackLocation(Irp))->MajorFunction == IRP_MJ_POWER) 
	{
		PoStartNextPowerIrp(Irp);
	}
	Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	return STATUS_INVALID_DEVICE_REQUEST;
}

static KDriver * GlobalDriverObject;

KDriver::KDriver()
{ 
	GlobalDriverObject = this;
}

KDriver::~KDriver()
{
}

VOID KDriver::DriverUnload()
{
}

NTSTATUS KDriver::IrpMajorCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorCreateNamedPipe(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorRead(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorQueryInformation(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorSetInformation(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorQueryEa(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorSetEa(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorFlushBuffers(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorQueryVolumeInformation(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorSetVolumeInformation(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorDirectoryControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorFileSystemControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorInternalDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorShutdown(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorLockControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorCleanup(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorCreateMailslot(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorQuerySecurity(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorSetSecurity(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorPower(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorSystemControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorDeviceChange(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorQueryQuota(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorSetQuota(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::IrpMajorPnp(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return IopInvalidDeviceRequest(DeviceObject, Irp);
}

NTSTATUS KDriver::MajorFunction(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS status;
	PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);
	switch(irpSp->MajorFunction)
	{
	case IRP_MJ_CREATE:
		status = IrpMajorCreate(DeviceObject, Irp);
		break;
	case IRP_MJ_CREATE_NAMED_PIPE:
		status = IrpMajorCreateNamedPipe(DeviceObject, Irp);
		break;
	case IRP_MJ_CLOSE:
		status = IrpMajorClose(DeviceObject, Irp);
		break;
	case IRP_MJ_READ:
		status = IrpMajorRead(DeviceObject, Irp);
		break;
	case IRP_MJ_WRITE:
		status = IrpMajorWrite(DeviceObject, Irp);
		break;
	case IRP_MJ_QUERY_INFORMATION:
		status = IrpMajorQueryInformation(DeviceObject, Irp);
		break;
	case IRP_MJ_SET_INFORMATION:
		status = IrpMajorSetInformation(DeviceObject, Irp);
		break;
	case IRP_MJ_QUERY_EA:
		status = IrpMajorQueryEa(DeviceObject, Irp);
		break;
	case IRP_MJ_SET_EA:
		status = IrpMajorSetEa(DeviceObject, Irp);
		break;
	case IRP_MJ_FLUSH_BUFFERS:
		status = IrpMajorFlushBuffers(DeviceObject, Irp);
		break;
	case IRP_MJ_QUERY_VOLUME_INFORMATION:
		status = IrpMajorQueryVolumeInformation(DeviceObject, Irp);
		break;
	case IRP_MJ_SET_VOLUME_INFORMATION:
		status = IrpMajorSetVolumeInformation(DeviceObject, Irp);
		break;
	case IRP_MJ_DIRECTORY_CONTROL:
		status = IrpMajorDirectoryControl(DeviceObject, Irp);
		break;
	case IRP_MJ_FILE_SYSTEM_CONTROL:
		status = IrpMajorFileSystemControl(DeviceObject, Irp);
		break;
	case IRP_MJ_DEVICE_CONTROL:
		status = IrpMajorDeviceControl(DeviceObject, Irp);
		break;
	case IRP_MJ_INTERNAL_DEVICE_CONTROL:
		status = IrpMajorInternalDeviceControl(DeviceObject, Irp);
		break;
	case IRP_MJ_SHUTDOWN:
		status = IrpMajorShutdown(DeviceObject, Irp);
		break;
	case IRP_MJ_LOCK_CONTROL:
		status = IrpMajorLockControl(DeviceObject, Irp);
		break;
	case IRP_MJ_CLEANUP:
		status = IrpMajorCleanup(DeviceObject, Irp);
		break;
	case IRP_MJ_CREATE_MAILSLOT:
		status = IrpMajorCreateMailslot(DeviceObject, Irp);
		break;
	case IRP_MJ_QUERY_SECURITY:
		status = IrpMajorQuerySecurity(DeviceObject, Irp);
		break;
	case IRP_MJ_SET_SECURITY:
		status = IrpMajorSetSecurity(DeviceObject, Irp);
		break;
	case IRP_MJ_POWER:
		status = IrpMajorPower(DeviceObject, Irp);
		break;
	case IRP_MJ_SYSTEM_CONTROL:
		status = IrpMajorSystemControl(DeviceObject, Irp);
		break;
	case IRP_MJ_DEVICE_CHANGE:
		status = IrpMajorDeviceChange(DeviceObject, Irp);
		break;
	case IRP_MJ_QUERY_QUOTA:
		status = IrpMajorQueryQuota(DeviceObject, Irp);
		break;
	case IRP_MJ_SET_QUOTA:
		status = IrpMajorSetQuota(DeviceObject, Irp);
		break;
	case IRP_MJ_PNP:
		status = IrpMajorPnp(DeviceObject, Irp);
		break;
	default:
		status = STATUS_INVALID_PARAMETER;
		break;
	}
	return status;
}

EXTERN_C
NTSTATUS MajorFunction(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	return GlobalDriverObject->MajorFunction(DeviceObject, Irp);
}

EXTERN_C
VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
	GlobalDriverObject->DriverUnload();
	TerminateCppRunTime();
}

EXTERN_C
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	ULONG i;
	NTSTATUS status;
	InitializeCppRunTime();
	DriverObject->DriverUnload = DriverUnload;
	GlobalDriverObject->m_DriverObject = DriverObject;
	GlobalDriverObject->m_RegistryPath.Buffer = GlobalDriverObject->unStrBuf;
	GlobalDriverObject->m_RegistryPath.MaximumLength = sizeof(GlobalDriverObject->unStrBuf);
	RtlCopyUnicodeString(&GlobalDriverObject->m_RegistryPath, RegistryPath);
	for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = MajorFunction;
	}
	status = GlobalDriverObject->DriverEntry();
	if (!NT_SUCCESS(status))
	{
		TerminateCppRunTime();
	}
	return status;
}
