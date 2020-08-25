#ifndef __HASH_H__
#define __HASH_H__

EXTERN_C void HashTableInit(ULONG Count);

EXTERN_C void InsertHashTable(DRV_INFO & DrvInfo);

EXTERN_C BOOLEAN LookupHashTable(DRV_INFO & DrvInfo);

#endif
