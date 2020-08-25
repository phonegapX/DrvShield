/* 双向结点
 * QA Pass
 */

#ifndef DOUBLY_LINKED_NODE_CLASS
#define DOUBLY_LINKED_NODE_CLASS

#include <stdlib.h>

template <class T>
class DNode
{
    private:
    	// 指向左、右结点的指针
        DNode<T> *left;
        DNode<T> *right;
    public: 
    	// data 为公有成员 
        T data;
        
		// 构造函数
        DNode(void); 
        DNode (const T& item);
        
        
        // 改变表的方法
        void InsertRight(DNode<T> *p);
        void InsertLeft(DNode<T> *p);
        DNode<T> *DeleteNode(void);
        
        
        // 取得指向左、右方向结点的指针
        DNode<T> *NextNodeRight(void) const;
        DNode<T> *NextNodeLeft(void) const;
};

// 创建空表并保留数据未定义，用于 header
template <class T>
DNode<T>::DNode(void)
{
	// 初始化结点使其指向自身
    left = right = this;
}

// 创建空表并初始化数据
template <class T>
DNode<T>::DNode(const T& item)
{
	// 建立一个指向自身的结点并初始化 data 域
    left = right = this;
    data = item;
}
        
// 将结点 p 插入到双向链表中当前结点的右边
template <class T>
void DNode<T>::InsertRight(DNode<T> *p)
{
	// 将 p 和当前结点的右后继结点相连
    p->right = right;
	right->left = p;
	
 	// 将 p 的左边和当前结点相连
    p->left = this;
    right = p;
}

// 将结点 p 插入到当前结点左边
template <class T>
void DNode<T>::InsertLeft(DNode<T> *p)
{
	// 将 p 和当前结点的左后继结点相连
    p->left = left;
    left->right = p;
    
 	// 将 p 的右边和当前结点相连
    p->right = this;
    left = p;
}

// 从链表中删除当前结点并返回其地址
template <class T>
DNode<T> *DNode<T>::DeleteNode(void)
{
	// 左边的结点必须链接到当前结点的右边
    left->right = right;
    
	// 右边的结点必须链接到当前结点的左边
    right->left = left;
    
    // 返回当前结点的指针
    return this;
}

// 返回指向右边结点的指针
template <class T>
DNode<T> *DNode<T>::NextNodeRight(void) const
{
    return right;
}

// 返回指向左边结点的指针
template <class T>
DNode<T> *DNode<T>::NextNodeLeft(void) const
{
    return left;
}

#endif	// DOUBLY_LINKED_NODE_CLASS
