#ifndef __HOOKSSDT_H__
#define __HOOKSSDT_H__

EXTERN_C VOID AddProcessProtectList(ULONG ProcessID);

EXTERN_C BOOLEAN ExistProcessIdInProtectList(ULONG ProcessID);

EXTERN_C VOID DisableWriteProtect(PULONG Dr0);

EXTERN_C VOID DisableIntermitAndSaveFlagRegister(PUSHORT FlagRegValue);

EXTERN_C NTSTATUS SWHookInit();

EXTERN_C VOID SWHookSSDT();

#endif
