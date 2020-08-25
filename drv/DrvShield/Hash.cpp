#include "std.h"
#include "Hash.h"
#include "Arithmetic/hashtable.h"

static HashTable<DRV_INFO> obHashTable;

inline bool operator==(DRV_INFO & lhs, const DRV_INFO & rhs)
{
	if (memcmp(lhs.Md5Digest, rhs.Md5Digest, sizeof(lhs.Md5Digest)) == 0)
	{
		return true;
	}
	return false;
}

ULONG HashKey(const DRV_INFO & key)
{
	ULONG nHash = 0;
	CHAR szBuf[128] = {0};
	PCHAR strkey = szBuf;
	memcpy(szBuf, key.Md5Digest, sizeof(key.Md5Digest));
	while (*strkey)
	{
		nHash = (nHash<<5) + nHash + *strkey++;
	}
	return nHash;
}

void HashTableInit(ULONG Count)
{
	obHashTable.InitHashTable(Count, HashKey);
}

void InsertHashTable(DRV_INFO & DrvInfo)
{
	obHashTable.Insert(DrvInfo);
}

BOOLEAN LookupHashTable(DRV_INFO & DrvInfo)
{
	return obHashTable.Find(DrvInfo);
}
