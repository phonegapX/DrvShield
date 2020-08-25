/* 双向循环链表
 * 
 */

#ifndef DOUBLE_CIRCULAR_LINKEDLIST_CLASS
#define DOUBLE_CIRCULAR_LINKEDLIST_CLASS

#include <stdlib.h>
#include "dnode.h"

template <class T>
class DCLinkedList
{
	private:
		// 指向表头的指针和当前结点的指针
		DNode<T> *header, *currPtr;
		
		// 表中的元素个数和当前的位置值
		int size;

		// 将表 L 拷贝到当前表尾
		void CopyList(const DCLinkedList<T> &L);
	
	public:
		// 构造函数
		DCLinkedList(void);
		DCLinkedList(const DCLinkedList<T> &L);

		// 析构函数
		~DCLinkedList(void);

		// 赋值运算符
		DCLinkedList<T>& operator= (const DCLinkedList<T> &L);

		// 检查表状态的函数
		int ListSize(void) const;               
		bool ListEmpty(void) const;

		// 遍历表的函数
		void Reset(bool bheader = true);	// 是从表头开始遍历，还是表尾（反向遍历）
		void Next(void);
		void Prev(void);
		bool EndOfList(void) const;

		// 插入函数
		void InsertFront(const T &item);
		void InsertRear(const T &item);
		void InsertAt(const T &item);
		void InsertAfter(const T &item);

		// 删除函数
		void DeleteFront(void);
		void DeleteRear(void);
		void DeleteAt(void);

		// 访问／修改数据
		T& Data(void);
		bool Find(const T& item);

		// 清空表的函数
		void ClearList(void);
};

// 构造函数
template <class T>
DCLinkedList<T>::DCLinkedList(void):size(0)
{
	// 创建“哨位”结点，注意 new 可能会抛出异常
	currPtr = header = new DNode<T>();
}

// 构造函数
template <class T>
DCLinkedList<T>::DCLinkedList(const DCLinkedList<T>& L):size(0)
{
	// 创建“哨位”结点，注意 new 可能会抛出异常
	currPtr = header = new DNode<T>();
	CopyList(L);
}

// 析构函数
template <class T>
DCLinkedList<T>::~DCLinkedList(void)
{
	ClearList();
	delete header;
}

// 将 L 拷贝到当前表尾
template <class T>
void DCLinkedList<T>::CopyList(const DCLinkedList<T> &L)
{
	// 用指针 P 遍历表
	DNode<T> *p = L.header->NextNodeRight();

	// 往当前表的表尾插入 L 的每个元素
	while (p != L.header)
	{
		InsertRear(p->data);
		p = p->NextNodeRight();
	}
}

template <class T>
int DCLinkedList<T>::ListSize(void) const
{
	// 不包括哨位结点
	return size;
}

template <class T>
bool DCLinkedList<T>::ListEmpty(void) const
{
	return (size == 0);
}

template <class T>
void DCLinkedList<T>::Reset(bool bheader)
{
	if (bheader) 
		currPtr = header->NextNodeRight();	// 表头
	else
		currPtr = header->NextNodeLeft();	// 表尾
}

template <class T>
void DCLinkedList<T>::Next(void)
{
	currPtr = currPtr->NextNodeRight();
}

template <class T>
void DCLinkedList<T>::Prev(void)
{
	currPtr = currPtr->NextNodeLeft();
}

template <class T>
bool DCLinkedList<T>::EndOfList(void) const
{
	return (currPtr == header);
}

// 插入函数
template <class T>
void DCLinkedList<T>::InsertFront(const T &item)
{
	Reset();
	InsertAt(item);
}

template <class T>
void DCLinkedList<T>::InsertRear(const T &item)
{
	currPtr = header;
	InsertAt(item);
}

template <class T>
void DCLinkedList<T>::InsertAt(const T &item)
{
	DNode<T> *newNode = new DNode<T>(item);
	currPtr->InsertLeft(newNode);
	currPtr = newNode;
	size++;
}

template <class T>
void DCLinkedList<T>::InsertAfter(const T &item)
{
	Next();
	InsertAt(item);
}

// 删除函数
template <class T>
void DCLinkedList<T>::DeleteFront(void)
{
	Reset();
	DeleteAt();
}

template <class T>
void DCLinkedList<T>::DeleteRear(void)
{
	Reset(false);
	DeleteAt();
}

template <class T>
void DCLinkedList<T>::DeleteAt(void)
{
	// 若表为空或已到表尾，则返回
	if (currPtr == header)
		return;

	DNode<T> *p = currPtr->NextNodeRight();
	delete (currPtr->DeleteNode());
	currPtr = p;
	size --;
}

// 访问／修改数据
template <class T>
T& DCLinkedList<T>::Data(void)
{
	// 若表为空或已到表尾，则出错
	if (currPtr == header)
		ExRaiseStatus(STATUS_INVALID_PARAMETER);//throw "DCLinkedList::Data: invalid reference";

	return currPtr->data;
}

template <class T>
bool DCLinkedList<T>::Find(const T& item)
{
	for (Reset(); !EndOfList(); Next())
		if (Data() == item)
			return true;

	return false;
}

template <class T>
DCLinkedList<T>& DCLinkedList<T>::operator= (const DCLinkedList<T>& L)
{
	if (this == &L)      // 无法赋值给自身
		return *this;
	
	ClearList();
	CopyList(L);
	return *this;
}

template <class T>
void DCLinkedList<T>::ClearList(void)
{
	Reset();
	while (currPtr != header)
		DeleteAt();
}

#endif	// DOUBLE_CIRCULAR_LINKEDLIST_CLASS