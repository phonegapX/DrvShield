#ifndef HASH_TABLE_CLASS
#define HASH_TABLE_CLASS

#include <stdlib.h>
#include "array.h"
#include "dclinkedlist.h"

template <class T>
class HashTable
{
	protected:
		// “桶”的个数，表示哈希表的大小
		int numBuckets;

		// 哈希表为链表构成的数组
		Array< DCLinkedList<T> > buckets;

		// 哈希函数及指向当前数据项的指针、当前的哈希值
		unsigned long (*hf)(const T & key);  // 除以哈希表大小的工作在内部完成
		T *current;
		int hashval;

		// 表中元素个数
		int size;

	public:
		//默认构造
		HashTable(){}

		// 参数为哈希表大小及哈希函数的构造函数
		HashTable(int nbuckets, unsigned long hashf(const T & key));

		//我加的
		void InitHashTable(int nbuckets, unsigned long hashf(const T & key))
		{
			numBuckets = nbuckets;
			hf = hashf;
			buckets.Resize(nbuckets);
			current = NULL;
			hashval = 0;
		}

		// 处理表的方法
		void Insert(const T& key);
		bool Find(T& key);			// 检索时只要设置结构中与关键字有关的成员，该函数会返回哈希表中对应的完整项
		void DeleteAt(void);		// 删除当前成员
		void Delete(const T& key);
		void ClearList(void);
		void Update(const T& key);  // 对已经在哈希表中的元素进行更新
};

template <class T>
HashTable<T>::HashTable(int nbuckets, unsigned long hashf(const T & key)):
			numBuckets(nbuckets), buckets(nbuckets), hf(hashf), current(NULL), hashval(0) 
{}

// 如果数据已存在，则更新它，否则将其插入
template <class T>
void HashTable<T>::Insert(const T& key)
{
	// hashval 为哈希值（桶索引）
	hashval = int(hf(key) % numBuckets);

	// lst 为 buckets[hashval] 的别名，可避免下标寻址
	DCLinkedList<T>& lst = buckets[hashval];

	for (lst.Reset(); !lst.EndOfList(); lst.Next())
	{
		// 若找到匹配值，修改其数据后返回（/* 如果关键字相同即认为是同一条目（认为两者相等）*/）
		if (lst.Data() == key)	//这里就需要重载==操作符
		{
			lst.Data() = key;  // 将数据区也设为相等
			current = &lst.Data();
			return;
		}
	}
	// 若没有找到，则将数据项加入表中
	lst.InsertRear(key);
	current = &lst.Data();
	size++;
}

template <class T>
bool HashTable<T>::Find(T& key)
{
	// 计算键值的哈希值并将 lst 指向它对应的 DCLinkedList
	hashval = int(hf(key) % numBuckets);
	DCLinkedList<T>& lst = buckets[hashval];

	// 在键表中扫描结点并寻找与 key 匹配的记录
	for (lst.Reset(); !lst.EndOfList(); lst.Next())
	{
		// 若找到匹配值，则取其数据值，将 current 指向该记录
		if (lst.Data() == key)
		{
			key = lst.Data();
			current = &lst.Data();
			return true;
		}
	}
	return false;
}

template <class T>
void HashTable<T>::Delete(const T& key)
{
	hashval = int(hf(key) % numBuckets);
	DCLinkedList<T>& lst = buckets[hashval];

	for (lst.Reset(); !lst.EndOfList(); lst.Next())
	{
		if (lst.Data() == key)  // 如果关键字匹配，删除结点
		{
			lst.DeleteAt();
			current = &lst.Data();
			size--;
			return;
		}
	}
}

template <class T>
void HashTable<T>::DeleteAt(void)
{
	DCLinkedList<T>& lst = buckets[hashval];
	if (current != NULL && !lst.EndOfList())
	{
		lst.DeleteAt();
		current = &lst.Data();
		size--;
	}
}

template <class T>
void HashTable<T>::ClearList(void)
{     
	for (int i = 0; i < numBuckets; i++)
		buckets[i].ClearList();
	size = 0;
	current = NULL;
}

template <class T>
void HashTable<T>::Update(const T& key)
{     
	DCLinkedList<T>& lst = buckets[hashval];
	if (current != NULL && !lst.EndOfList() && *current == key)
		*current = key;
	else
		Insert(key);
}

#endif	// HASH_TABLE_CLASS
