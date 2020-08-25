#ifndef __CPPRTL_H__
#define __CPPRTL_H__

EXTERN_C
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath);

EXTERN_C
VOID DriverUnload(IN PDRIVER_OBJECT DriverObject);

EXTERN_C
NTSTATUS MajorFunction(PDEVICE_OBJECT DeviceObject, PIRP Irp);

class KDriver
{
	friend NTSTATUS DriverEntry(IN PDRIVER_OBJECT, IN PUNICODE_STRING);
	friend VOID DriverUnload(IN PDRIVER_OBJECT DriverObject);
	friend NTSTATUS MajorFunction(PDEVICE_OBJECT DeviceObject, PIRP Irp);
public:
	KDriver();
	virtual ~KDriver();
protected:
	virtual NTSTATUS DriverEntry() = 0;
	virtual VOID DriverUnload();
	virtual NTSTATUS MajorFunction(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorCreateNamedPipe(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorRead(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorQueryInformation(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorSetInformation(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorQueryEa(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorSetEa(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorFlushBuffers(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorQueryVolumeInformation(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorSetVolumeInformation(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorDirectoryControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorFileSystemControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorInternalDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorShutdown(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorLockControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorCleanup(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorCreateMailslot(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorQuerySecurity(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorSetSecurity(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorPower(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorSystemControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorDeviceChange(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorQueryQuota(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorSetQuota(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorPnp(PDEVICE_OBJECT DeviceObject, PIRP Irp);

protected:
	PDRIVER_OBJECT m_DriverObject;
	UNICODE_STRING m_RegistryPath;
	WCHAR unStrBuf[512];
};

#endif
