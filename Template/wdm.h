$$IF(USE_WDF)
#define WIN9X_COMPAT_SPINLOCK
#include <ntddk.h>
#pragma warning(disable:4201)  // nameless struct/union warning

#include <stdarg.h>
#include <wdf.h>

#pragma warning(default:4201)

#include <initguid.h> // required for GUID definitions
#include <wdmguid.h>  // required for WMILIB_CONTEXT


typedef struct _DEVICE_CONTEXT
{
	WDFDEVICE       Device;
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext)

typedef struct _TIMER_CONTEXT
{
    PDEVICE_CONTEXT    TpmContext;
} TIMER_CONTEXT,*PTIMER_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(TIMER_CONTEXT, GetTimerContext)

$$ELSE
#include <ntddk.h>

#define NT_DEVICE_NAME			L"\\Device\\$$Root$$"
#define DOS_DEVICE_NAME			L"\\DosDevices\\$$Root$$"

typedef struct _DEVICE_EXTENSION
{
	PDEVICE_OBJECT       Device;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

$$ENDIF
