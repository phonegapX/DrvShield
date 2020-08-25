/* AVL 树结点
 * QA Pass
 */

#ifndef AVL_TREENODE_CLASS
#define AVL_TREENODE_CLASS

#include <stdlib.h>

template <class T>
class AVLTree;

template <class T>
class AVLTreeNode
{
	protected:
		// 指向结点左右孩子的指针
		AVLTreeNode<T> *left;
		AVLTreeNode<T> *right;
		
	private:
		// AVLTreeNode 所需的附加数据成员
		int balanceFactor;
		
	public:
		// 公有成员，允许外部修改的数据值
		T data;
		
		// 构造函数
		AVLTreeNode (const T& item, AVLTreeNode<T> *lptr = NULL,
			AVLTreeNode<T> *rptr = NULL, int balfac = 0);
		
		// 访问指针域的函数
		AVLTreeNode<T>* Left(void) const;
		AVLTreeNode<T>* Right(void) const;
		
		// 访问新数据域的方法
		int GetBalanceFactor(void);
		
		friend class AVLTree<T>;
};

// 构造函数
template <class T>
AVLTreeNode<T>::AVLTreeNode (const T& item,
      AVLTreeNode<T> *lptr, AVLTreeNode<T> *rptr, int balfac):
   data(item), left(lptr), right(rptr), balanceFactor(balfac)
{}

// 允许用户访问左孩子
template <class T>
AVLTreeNode<T>* AVLTreeNode<T>::Left(void) const
{
   return left;
}

// 允许用户访问右孩子
template <class T>
AVLTreeNode<T>* AVLTreeNode<T>::Right(void) const
{
   return right;
}

template <class T>
int AVLTreeNode<T>::GetBalanceFactor(void)
{
   return balanceFactor;
}

#endif // AVL_TREENODE_CLASS
