#include "$$Root$$.h"
$$IF(USE_WDF)
#pragma data_seg(".text")
#include <initguid.h>
// {308EE180-A522-4f39-9717-060708808E08}
DEFINE_GUID(GUID_DEVICEINTERFACE, 
			0x308ee180, 0xa522, 0x4f39, 0x97, 0x17, 0x6, 0x7, 0x8, 0x80, 0x8e, 0x8);
#pragma data_seg()
$$ENDIF

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject,PUNICODE_STRING RegistryPath);
$$IF(USE_WDF)
NTSTATUS
EvtDeviceAdd(
			 IN WDFDRIVER        Driver,
			 IN PWDFDEVICE_INIT  DeviceInit
    );
VOID  EvtIoDeviceControl (
						  IN WDFQUEUE  Queue,
						  IN WDFREQUEST  Request,
						  IN size_t  OutputBufferLength,
						  IN size_t  InputBufferLength,
						  IN ULONG  IoControlCode
							 );
VOID DriverUnload(WDFDRIVER Driver);
$$ELSE
NTSTATUS DispatchNull(PDEVICE_OBJECT DeviceObject,PIRP Irp);
NTSTATUS DispatchIoCtl(PDEVICE_OBJECT DeviceObject,PIRP Irp);
VOID DriverUnload(PDRIVER_OBJECT DriverObject);
$$ENDIF

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, DriverUnload)
$$IF(USE_WDF)
#pragma alloc_text (PAGE, EvtDeviceAdd)
#pragma alloc_text (PAGE, EvtIoDeviceControl)
$$ELSE
#pragma alloc_text (PAGE, DispatchNull)
#pragma alloc_text (PAGE, DispatchIoCtl)
$$ENDIF
#endif

$$IF(USE_WDF)

NTSTATUS  DriverEntry(PDRIVER_OBJECT DriverObject,PUNICODE_STRING RegistryPath)
{
	NTSTATUS            status = STATUS_SUCCESS;
    WDF_DRIVER_CONFIG   config;
	
	PAGED_CODE();
	
    KdPrint(("$$Root$$ Driver - using Windows Driver Frame"));
	
    //
    // Initialize the Driver Config structure.
    //
    WDF_DRIVER_CONFIG_INIT( &config, EvtDeviceAdd );
	config.EvtDriverUnload = DriverUnload;
	
    //
    // Register a cleanup callback so that we can call WPP_CLEANUP when
    // the framework driver object is deleted during driver unload.
    //
	
    status = WdfDriverCreate( DriverObject,
		RegistryPath,
		NULL,
		&config,
		WDF_NO_HANDLE);
	
    if (!NT_SUCCESS(status))
	{
        KdPrint(("WdfDriverCreate failed with status %!STATUS!", status));
    }
	
    return status;

}

NTSTATUS
EvtDeviceAdd(
				IN WDFDRIVER        Driver,
				IN PWDFDEVICE_INIT  DeviceInit
    )
{
	NTSTATUS                    status = STATUS_SUCCESS;
    WDF_OBJECT_ATTRIBUTES       DeviceAttributes;
    WDFDEVICE                   Device;
	PDEVICE_CONTEXT				Context;
    WDF_IO_QUEUE_CONFIG         IoQueueConfig;
    WDFQUEUE                    hQueue;
	WDF_DEVICE_POWER_CAPABILITIES   PowerCaps;
	
	PAGED_CODE();
	
    UNREFERENCED_PARAMETER( Driver );

	RtlZeroMemory(&PowerCaps,sizeof(PowerCaps));

    KdPrint(("EvtDeviceAdd routine PDO: %p (%p)\n",
				WdfDriverWdmGetDriverObject(Driver)));

	RtlZeroMemory(&DeviceAttributes,sizeof(DeviceAttributes));
    WDF_OBJECT_ATTRIBUTES_SET_CONTEXT_TYPE(&DeviceAttributes, DEVICE_CONTEXT);
	DeviceAttributes.Size = sizeof(WDF_OBJECT_ATTRIBUTES);
	DeviceAttributes.SynchronizationScope	= WdfSynchronizationScopeInheritFromParent;
	DeviceAttributes.ExecutionLevel			= WdfExecutionLevelPassive;

    status = WdfDeviceCreate(&DeviceInit,&DeviceAttributes,&Device);
	
    if(NT_SUCCESS(status))
    {
        Context = GetDeviceContext(Device);
        RtlZeroMemory(Context,sizeof(DEVICE_CONTEXT));	
		Context->Device = Device;

        status = WdfDeviceCreateDeviceInterface(Device,&GUID_DEVICEINTERFACE,NULL);
		
        if(NT_SUCCESS(status))
        {
            WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
				&IoQueueConfig,
				WdfIoQueueDispatchParallel
				);

            IoQueueConfig.EvtIoDeviceControl = EvtIoDeviceControl;
			
            status = WdfIoQueueCreate(
				Device,
				&IoQueueConfig,
				WDF_NO_OBJECT_ATTRIBUTES,
				&hQueue
				);
			
            if(NT_SUCCESS(status))
            {
                WDF_DEVICE_POWER_CAPABILITIES_INIT(&PowerCaps);
                WdfDeviceSetPowerCapabilities(Device,&PowerCaps);
				
            }
            else
            {
                KdPrint(("WdfIoQueueCreate failed with Status code 0x%x\n",status));
            }
        }
        else
        {
            KdPrint(("WdfDeviceCreateDeviceInterface failed with Status code 0x%x\n",status));
        }
	}

    KdPrint(("TpmEvtDeviceAdd exited with Status code 0x%x\n",status));
	
    return status;
}

VOID  EvtIoDeviceControl (
							 IN WDFQUEUE  Queue,
							 IN WDFREQUEST  Request,
							 IN size_t  OutputBufferLength,
							 IN size_t  InputBufferLength,
							 IN ULONG  IoControlCode
							 )
{
    NTSTATUS			status;
    PDEVICE_CONTEXT		Context;
    WDFDEVICE			Device;
	
	PAGED_CODE();
	
    Device = WdfIoQueueGetDevice(Queue);
    Context = GetDeviceContext(Device);
	
    KdPrint(("EvtIoDeviceControl called - IoControlCode: 0x%x \n",IoControlCode));
	
    switch(IoControlCode)
    {
		
    default:
        KdPrint(("EvtIoDeviceControl: Invalid request 0x%x\n",IoControlCode));
        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }
	
    WdfRequestComplete( Request, status);
}

VOID DriverUnload(WDFDRIVER Driver)
{

}

$$ELSE

NTSTATUS  DriverEntry(PDRIVER_OBJECT DriverObject,PUNICODE_STRING RegistryPath)
{
	NTSTATUS			Status = STATUS_SUCCESS;
	UNICODE_STRING		DeviceName;
	UNICODE_STRING		SymbolicName;
	PDEVICE_OBJECT		DeviceObject;
	PDEVICE_EXTENSION	DevExt;
	ULONG				i;

	PAGED_CODE();

	RtlInitUnicodeString(&DeviceName,NT_DEVICE_NAME);
	Status = IoCreateDevice(DriverObject,
							sizeof(DEVICE_EXTENSION),
							&DeviceName,
							FILE_DEVICE_UNKNOWN,
							0,
							FALSE,
							&DeviceObject);
	if(NT_SUCCESS(Status))
	{
		RtlInitUnicodeString(&SymbolicName,DOS_DEVICE_NAME);
		Status = IoCreateSymbolicLink(&SymbolicName,&DeviceName);
		if(!NT_SUCCESS(Status))
		{
			IoDeleteDevice(DeviceObject);
		}
		else
		{
			DevExt = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
			RtlZeroMemory(DevExt,sizeof(DEVICE_EXTENSION));
			DevExt->Device = DeviceObject;
			
			for(i = 0 ; i < IRP_MJ_MAXIMUM_FUNCTION ; i++)
			{
				DriverObject->MajorFunction[i] = DispatchNull;
			}

			DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoCtl;
			
			DriverObject->DriverUnload = DriverUnload;
		}
	}

	return Status;
}

NTSTATUS DispatchIoCtl(PDEVICE_OBJECT DeviceObject,PIRP Irp)
{
	PDEVICE_EXTENSION	DevExt;
	PIO_STACK_LOCATION	IrpSp;
	
	PAGED_CODE();
	
	IrpSp = IoGetCurrentIrpStackLocation(Irp);
	DevExt = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;

	switch(IrpSp->Parameters.DeviceIoControl.IoControlCode)
	{
		
		
	default:
		break;
	}

	IoCompleteRequest(Irp,0);
	return STATUS_SUCCESS;
}

NTSTATUS DispatchNull(PDEVICE_OBJECT DeviceObject,PIRP Irp)
{
	PDEVICE_EXTENSION	DevExt;
	PIO_STACK_LOCATION	IrpSp;

	PAGED_CODE();

	IrpSp = IoGetCurrentIrpStackLocation(Irp);
	DevExt = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;


	IoCompleteRequest(Irp,0);
	return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING	SymbolicName;
	PAGED_CODE();

	RtlInitUnicodeString(&SymbolicName,DOS_DEVICE_NAME);
	IoDeleteSymbolicLink(&SymbolicName);
	IoDeleteDevice(DriverObject->DeviceObject);
	
}

$$ENDIF
