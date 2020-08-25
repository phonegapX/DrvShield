/* 链表结点
 * QA Pass
 */

#ifndef NODE_CLASS
#define NODE_CLASS

#include <stdlib.h>

template <class T>
class Node
{
	private:
		// next 为指向下一结点的指针
		Node<T> *next;
	public:
		// data 为公有成员
		T data;

		// 构造函数
		Node(const T& item, Node<T> *ptrnext = NULL);

		// 修改表的方法
		void InsertAfter(Node<T> *p);
		Node<T> *DeleteAfter(void);

		// 取得下一结点的指针
		Node<T> *NextNode(void) const;
};

// 构造函数。初始化数据及指针成员
template <class T>
Node<T>::Node(const T& item, Node<T> *ptrnext):
	          data(item), next(ptrnext)
{}

// 返回私有成员 next 的值
template <class T>
Node<T> *Node<T>::NextNode(void) const
{
	return next;
}

// 在当前结点之后插入结点 p
template <class T>
void Node<T>::InsertAfter(Node<T> *p)
{
	// p 指向当前结点的后继结点，然后将当前结点指向 p
	p->next = next;
	next = p;
}

// 删除当前结点的后继结点并返回其指针
template <class T>
Node<T> *Node<T>::DeleteAfter(void)
{
	// 若没有后继结点，返回 NULL
	if (next == NULL)
		return NULL;

	// 保存指向被删除结点的指针
	Node<T> *tempPtr = next;
	// 使当前结点指向 tempPtr 的后继结点
	next = tempPtr->next;
	// 返回被删除结点的指针
	return tempPtr;
}

#endif // NODE_CLASS