/* 普通链表（与数组类似）
 * Next 函数最终会使 position == size， currPtr = NULL，也就是移到表中最后一个结点之后
 * QA Pass
 */

#ifndef LINKEDLIST_CLASS
#define LINKEDLIST_CLASS

#include <stdlib.h>
#include "node.h"

template <class T>
class LinkedList
{
	private:
		// 指向表头和表尾的指针
		Node<T> *front, *rear;

		// 用于数据访问、插入和删除的指针
		Node<T> *prevPtr, *currPtr;

		// 表中的元素个数
		int size;

		// 表中位置值，用于 Reset
		int position;

		// 将表 L 拷贝到当前表尾
		void CopyList(const LinkedList<T> &L);

	public:
		// 构造函数
		LinkedList(void);
		LinkedList(const LinkedList<T>& L);

		// 析构函数
		~LinkedList(void);

		// 赋值运算符
		LinkedList<T>& operator= (const LinkedList<T>& L);

		// 检查表状态的函数
		int ListSize(void) const;               
		bool ListEmpty(void) const;

		// 遍历表的函数
		void Reset(int pos = 0);
		void Next(void);
		bool EndOfList(void) const;
		int CurrentPosition(void) const;

		// 插入函数
		void InsertFront(const T& item);
		void InsertRear(const T& item);
		void InsertAt(const T& item);
		void InsertAfter(const T& item);

		// 删除函数
		void DeleteFront(void);
		void DeleteAt(void);

		// 访问／修改数据
		T& Data(void);
		bool Find(const T& item);

		// 清空表的函数
		void ClearList(void);
};

// 创建空表，使其指针指向 NULL，size 置为 0，position 置为 -1
template <class T>
LinkedList<T>::LinkedList(void): front(NULL), rear(NULL),
	prevPtr(NULL),currPtr(NULL), size(0), position(-1)
{}

// 将 L 拷贝到当前表尾
template <class T>
void LinkedList<T>::CopyList(const LinkedList<T> &L)
{
	// 用指针 P 遍历表
	Node<T> *p = L.front;

	// 往当前表的表尾插入 L 的每个元素
	while (p != NULL)
	{
		InsertRear(p->data);
		p = p->NextNode();
	}
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& L)
{
	front = rear = prevPtr = currPtr = NULL;
	size = 0;
	position = -1;
	CopyList(L);
}

template <class T>
LinkedList<T>::~LinkedList(void)
{
	ClearList();
}

template <class T>
void LinkedList<T>::ClearList(void)
{
	Node<T> *currPos, *nextPos;
	currPos = front;
	while (currPos != NULL) 
	{
		// 取下一结点指针并删除当前结点
		nextPos = currPos->NextNode();
		delete currPos;
		currPos = nextPos;  //移到下一结点
	}
	front = rear = prevPtr = currPtr = NULL;
	size = 0;
	position = -1;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator= (const LinkedList<T>& L)
{
	if (this == &L)      // 无法赋值给自身
		return *this;
	
	ClearList();
	CopyList(L);
	return *this;
}

template <class T>
int LinkedList<T>::ListSize(void) const
{
	return size;
}

template <class T>
bool LinkedList<T>::ListEmpty(void) const
{
	return (size == 0);
}

// 将表位置设置到 pos
template <class T>
void LinkedList<T>::Reset(int pos)
{
	// 若表为空，则返回
	if (front == NULL)
		return;

	// 若位置非法，退出程序
	if (pos < 0 || pos > size - 1)
		throw "LinkedList::Reset: Invalid list position";
	
	prevPtr = NULL;
	currPtr = front;
	for (position = 0; position != pos; position++)
	{
		// 将两个指针右移
		prevPtr = currPtr;
		currPtr = currPtr->NextNode();
	}
}

// 将 prevPtr 和 currPtr 指针右移一个结点
template <class T>
void LinkedList<T>::Next(void)
{
	// 若已到表尾或表为空，返回
	if (currPtr !=  NULL)
	{
		// 将两个指针右移一个结点
		prevPtr = currPtr;
		currPtr = currPtr->NextNode();
		position++;
	}
}

// 如果已到表尾，返回真
template <class T>
bool LinkedList<T>::EndOfList(void) const
{
	return (currPtr == NULL);
}

// 返回当前结点的位置
template <class T>
int LinkedList<T>::CurrentPosition(void) const
{
	return position;
}

// 往表头插入结点
template <class T>
void LinkedList<T>::InsertFront(const T& item)
{
	Reset();
	InsertAt(item);  // 往表头插入
}

// 在表尾插入结点
template <class T>
void LinkedList<T>::InsertRear(const T& item)
{
	prevPtr = rear;
	currPtr= NULL;
	position = size;

	InsertAt(item);
}

// 往表的当前位置插入结点
template <class T>
void LinkedList<T>::InsertAt(const T& item)
{
	Node<T> *newNode;

	// 两种情况：往表头或表中插入
	if (prevPtr == NULL)
	{
		// 往表头插入，包括往空表中插入
		newNode = new Node<T>(item, front);
		front = newNode;
	}
	else
	{
		// 往表中插入。在 prevPtr 后插入结点
		newNode = new Node<T>(item);
		prevPtr->InsertAfter(newNode);
	}

	// 若 currPtr == NULL，表示往空表中或非空表的表尾插入；应修改 rear 及 position 值
	if (currPtr == NULL)
	{
		rear = newNode;
		position = size;
	}

	// 改变 currPtr 及增加表的大小
	currPtr = newNode;
	size++;
}

// 在当前表位置后插入结点
template <class T>
void LinkedList<T>::InsertAfter(const T& item)
{
	Next();
	InsertAt(item);
}

// 删除表中第一个节点
template <class T>
void LinkedList<T>::DeleteFront(void)
{
   if (front == NULL)
      return;
	
   Reset();
   DeleteAt();
}

// 删除表中当前结点
template <class T>
void LinkedList<T>::DeleteAt(void)
{
	// 如果表为空或已到表尾，则出错退出
	if (currPtr == NULL)
		throw "LinkedList::DeleteAt: Invalid deletion!";
	
	Node<T> *p;

	// 被删除的必是头结点或表中结点
	if (prevPtr == NULL)
	{
		// 保存表头指针并取消其链接。如果这是最后结点，front 变为 NULL
		p = front;
		front = front->NextNode();
	}
	else
		// 取消链接 prevPtr 之后的中间结点，并保存其地址
		p = prevPtr->DeleteAfter();
	
	// 若表尾被删除，则 prevPtr 是新表尾且 position 减 1；否则，position 不变
	// 如果 p 是最后结点，rear = NULL 且 position = -1
	if (p == rear)
	{
		rear = prevPtr;
		position--;
	}
	
	// 将 currPtr 指向下一结点。若 p 为表中最后结点，则 currPtr 为 NULL
	currPtr = p->NextNode();
	
	// 释放结点并将表大小减 1
	delete p;
	size--;
}

// 返回当前结点的数据值
template <class T>
T& LinkedList<T>::Data(void)
{
	// 若表为空或已到表尾，则出错
	if (currPtr == NULL)
		throw "LinkedList::Data: invalid reference!";

	return currPtr->data;
}

// 查找链表中是否有某数据
template <class T>
bool LinkedList<T>::Find(const T& item)
{
	for (Reset(); !EndOfList(); Next())
		if (Data() == item)
			return true;

	return false;
}

#endif  // LINKEDLIST_CLASS