#ifndef __DRVSHIELD_H__
#define __DRVSHIELD_H__

class KDrvShieldDriver:public KDriver
{
public:

protected:
	virtual NTSTATUS DriverEntry();
	virtual NTSTATUS IrpMajorCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);
	virtual NTSTATUS IrpMajorDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);

private:
};

#endif
