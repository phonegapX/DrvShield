/* AVL 树（为了效率不从二叉搜索树继承）
 * 
 */

#ifndef AVL_TREE_CLASS
#define AVL_TREE_CLASS

#include <stdlib.h>
#include "avltreenode.h"

// 表明结点平衡因子的常量
const int leftheavy = -1;
const int balanced = 0;
const int rightheavy = 1;

template <class T>
class AVLTree
{
	private:
		// 指向树根及当前结点的指针
		AVLTreeNode<T> *root;
		AVLTreeNode<T> *current;
		
		// 树中数据项个数
		int size;
		
		// 用于复制构造函数及赋值运算符
		AVLTreeNode<T> *CopyTree(AVLTreeNode<T> *t);
		
		// 用于析构函数，赋值运算符及 ClearList 方法
		void DeleteTree(AVLTreeNode<T> *t);
		
		// 在函数 Find 和 Delete 中用来定位结点及其双亲在树中的位置
		AVLTreeNode<T> *FindNode(const T& item, AVLTreeNode<T>* & parent) const;
		
		// 供 Insert 和 Delete 方法在结点加入子树或从子树中删除时重建 AVL 树
		void SingleRotateLeft (AVLTreeNode<T>* &p);
		void SingleRotateRight (AVLTreeNode<T>* &p);
		void DoubleRotateLeft (AVLTreeNode<T>* &p);
		void DoubleRotateRight (AVLTreeNode<T>* &p);
		void UpdateLeftTree (AVLTreeNode<T>* &tree,	bool &reviseBalanceFactor);
		void UpdateRightTree (AVLTreeNode<T>* &tree, bool &reviseBalanceFactor);
		
		// AVL 树的 Insert 和 Delete 方法
		void AVLInsert(AVLTreeNode<T>* &tree,
            AVLTreeNode<T>* newNode, bool &reviseBalanceFactor);
		void AVLDelete(AVLTreeNode<T>* &tree,
            AVLTreeNode<T>* newNode, bool &reviseBalanceFactor);
		
	public:
		// 构造函数，析构函数
		AVLTree(void);
		AVLTree(const AVLTree<T>& tree);
		~AVLTree(void);
		
		// 赋值运算符
		AVLTree<T>& operator= (const AVLTree<T>& tree);
		
		// 标准的表处理函数
		bool Find(T& item);
		void Insert(const T& item);
		void Delete(const T& item); 
		void ClearList(void);
		bool ListEmpty(void) const;
		int ListSize(void) const;
		
		// 树的特殊方法
		void Update(const T& item);
		AVLTreeNode<T> *GetRoot(void) const;
};

template <class T>
AVLTreeNode<T> *AVLTree<T>::CopyTree(AVLTreeNode<T> *t)
{
	AVLTreeNode<T> *newlptr, *newrptr, *newNode;
	
	if (t == NULL)
		return NULL;
	
	if (t->left != NULL) 
		newlptr = CopyTree(t->left);
	else
		newlptr = NULL;
	
	if (t->right != 0) 
		newrptr = CopyTree(t->right);
	else
		newrptr = NULL;
	
	newNode = new AVLTreeNode(t->data, newlptr, newrptr);
	return newNode;
}

// 删除当前对象存储的树
template <class T>
void AVLTree<T>::DeleteTree(AVLTreeNode<T> *t)
{
    if (t != NULL)
    {
        DeleteTree(t->left);
        DeleteTree(t->right);
        delete t;
    }
}

// 删除树中的所有结点
template <class T>
void AVLTree<T>::ClearList(void)
{
    DeleteTree(root);
    root = current = NULL;
    size = 0;
}

template <class T>
AVLTree<T>& AVLTree<T>::operator= (const AVLTree<T>& rhs)
{
    // 不能将树复制到自身
    if (this == &rhs)
        return *this;
	
	ClearList();
	root = CopyTree(ths.root);
	
	current = root;
	size = ths.size;
	return *this;
}

// 在树中搜索 item，若找到，则将结点数据赋给 item
template <class T>
bool AVLTree<T>::Find(T& item)
{
    // 使用 FindNode，它需要 parent 参数
    AVLTreeNode<T> *parent;
	
    // 在树中搜索 item，将匹配的结点赋给 current
    current = FindNode(item, parent);
    
    // 若找到，则将数据赋给 item 并返回 True
    if (current != NULL)
    {
        item = current->data;
        return true;
    }
    else
		// 在树中没找到 item，返回 False
        return false;
}

// 指示树是否为空
template <class T>
bool AVLTree<T>::ListEmpty(void) const
{
    return (size == 0);
}

// 返回树中的数据项个数
template <class T>
int AVLTree<T>::ListSize(void) const
{
    return size;
}

template <class T>
AVLTree<T>::AVLTree(void):root(NULL), current(NULL), size(0)
{}

template <class T>
AVLTree<T>::AVLTree(const AVLTree<T>& tree)
{
	root = CopyTree(tree.root);
	current = root;
	size = tree.size;
}

template <class T>
AVLTree<T>::~AVLTree(void)
{
    ClearList();
}

// 在树中搜索数据项，若找到，则返回结点地址及指向其双亲的指针；否则，返回 NULL
template <class T>
AVLTreeNode<T> *AVLTree<T>::FindNode(const T& item, AVLTreeNode<T>* & parent) const
{   
    // 用指针 t 从根开始遍历树
    AVLTreeNode<T> *t = root;
    
    // 根的双亲为 NULL
    parent = NULL;
    
    // 若子树为空，则循环结束
    while(t != NULL)
    {
        // 若找到键值，则退出
        if (item == t->data)
            break;
        else 
        {
            // 修改双亲指针，并移到左子树或右子树
            parent = t;
            if (item < t->data)
                t = t->left;
            else 
                t = t->right;
        }
    }
    
    // 返回指向结点的指针；若没找到，则返回 NULL
    return t;
}

template <class T>
void AVLTree<T>::SingleRotateLeft (AVLTreeNode<T>* &p)
{
	AVLTreeNode<T> *rc = p->right;
	
	p->balanceFactor = balanced;
	rc->balanceFactor = balanced;
	
	p->right = rc->left;
	rc->left = p;
	p = rc;
} 

// 绕结点 p 顺时针旋转；使 lc 成为新轴
template <class T>
void AVLTree<T>::SingleRotateRight (AVLTreeNode<T>* & p)
{
	// p 的左子树“超重”，将 p 的左子树给 lc
	AVLTreeNode<T> *lc = p->left;
	
	// 修改双亲结点及左孩子的平衡因子
	p->balanceFactor = balanced;
	lc->balanceFactor = balanced;
	
	// lc 的右子树 st 应继续为 lc 右子树的一部分，将它改为 p 的左子树
	p->left = lc->right;
	
	// 旋转 p 使其为 lc 的右子树，lc 成为新轴
	lc->right = p;
	p = lc;
}

template <class T>
void AVLTree<T>::DoubleRotateLeft (AVLTreeNode<T>* &p)
{
	AVLTreeNode<T> *rc, *np;
	
	rc = p->right;
	np = rc->left;
	if (np->balanceFactor == leftheavy)
	{
		p->balanceFactor = balanced;
		rc->balanceFactor = leftheavy;
	}
	else if (np->balanceFactor == balanced)
	{
		p->balanceFactor = balanced;
		rc->balanceFactor = balanced;
	}
	else
	{
		p->balanceFactor = leftheavy;
		rc->balanceFactor = balanced;
	}
	np->balanceFactor = balanced;
	rc->left = np->right;
	np->right = rc;
	p->right = np->left;
	np->left = p;
	p = np;
}

// 绕结点 p 双右旋
template <class T>
void AVLTree<T>::DoubleRotateRight (AVLTreeNode<T>* &p)
{
	// 被旋转的两个子树
	AVLTreeNode<T> *lc, *np;
	
	// 在树中，结点(lc) < 结点(np) < 结点(p)
	lc = p->Left();			// lc 为双亲的左孩子
	np = lc->Right();		// np 为 lc 的右孩子
	
	// 修改 p, lc 和 np 的平衡因子
	if (np->balanceFactor == rightheavy)
	{
		p->balanceFactor = balanced;
		lc->balanceFactor = rightheavy;
	}
	else if (np->balanceFactor == balanced)
	{
		p->balanceFactor = balanced;
		lc->balanceFactor = balanced;
	}
	else
	{
		p->balanceFactor = rightheavy;
		lc->balanceFactor = balanced;
	}
	np->balanceFactor = balanced;
	
	// 在 np 替代双亲 p 之前，注意卸掉其老子树，连上新子树
	lc->right = np->left;
	np->left = lc;
	p->left = np->right;
	np->right = p;
	p = np;
} 

template <class T>
void AVLTree<T>::UpdateLeftTree (AVLTreeNode<T>* &p, bool &reviseBalanceFactor)
{
	AVLTreeNode<T> *lc = p->left;	// 左子树边偏重
	
	if (lc->balanceFactor == leftheavy) 
	{
		SingleRotateRight(p);    // 需单旋转
		reviseBalanceFactor = false;
	}
	// 右子树偏重吗？
	else if (lc->balanceFactor == rightheavy)
	{
		// 做一次双旋转
		DoubleRotateRight(p);
		// 此时，根结点平衡了
		reviseBalanceFactor = false;
	}
}

template <class T>
void AVLTree<T>::UpdateRightTree (AVLTreeNode<T>* &p, bool &reviseBalanceFactor)
{
	AVLTreeNode<T> *rc = p->right;
	
	if (rc->balanceFactor == rightheavy) 
	{
		SingleRotateLeft(p);
		reviseBalanceFactor = false;
	}
	else if (rc->balanceFactor == leftheavy) 
	{
		DoubleRotateLeft(p);
		reviseBalanceFactor = false;
	}
}

template <class T>
void AVLTree<T>:: AVLInsert(AVLTreeNode<T>* & tree,
							AVLTreeNode<T>* newNode, bool &reviseBalanceFactor)
{
	// 是否需修改结点的 balanceFactor 值的标志
	bool rebalanceCurrNode;
	
	// 扫描到空子树；此时应插入新节点
	if (tree == NULL)
	{
		// 更新双亲结点使其指向新节点     
		tree = newNode;
		
		// 将新结点的 balanceFactor 赋值为 0
		tree->balanceFactor = balanced;
		
		// 广播消息；balanceFactor 值被改变
		reviseBalanceFactor = true;       
	}
	// 若新结点的数据值 < 当前数据值，则递归遍历左子树
	else if (newNode->data < tree->data)
	{
		AVLInsert(tree->left, newNode, rebalanceCurrNode);
		
		// 检查是否应修改 balanceFactor 值
		if (rebalanceCurrNode)
		{
			// 从左偏重的子树往左，将违背 AVL 条件，进行旋转（情况3） 
			if (tree->balanceFactor == leftheavy)
				UpdateLeftTree(tree,reviseBalanceFactor);         

			// 从平衡结点往左，往左子树增加结点，满足 AVL 条件（情况 1）
			else if (tree->balanceFactor == balanced)
			{
				tree->balanceFactor = leftheavy;
				reviseBalanceFactor = true;
			}
			// 从右偏重子树往左，将产生平衡子树，满足 AVL 条件（情况 2）
			else
			{
				tree->balanceFactor = balanced;
				reviseBalanceFactor = false;
			}
		}
		else
			// 不需平衡此结点，也不用平衡上结点
			reviseBalanceFactor = false;
	}
	// 否则，递归遍历右子树
	else
	{
		AVLInsert(tree->right, newNode, rebalanceCurrNode);
		
		// 检查是否应修改 balanceFactor 值
		if (rebalanceCurrNode)
		{
			// 从左偏重子树往右，将平衡结点，满足 AVL 条件（情况 2）
			if (tree->balanceFactor == leftheavy)
			{
				// 扫面右子树，结点左偏重，则将成为平衡结点
				tree->balanceFactor = balanced;     
				reviseBalanceFactor = false;
			}
			// 从平衡子树往右，将产生右偏重结点，满足 AVL 条件（情况 1）
			else if (tree->balanceFactor == balanced)
			{
				// 结点原为平衡；将成为右偏重
				tree->balanceFactor = rightheavy;
				reviseBalanceFactor = true;
			}
			else
				// 从右偏重结点向右，将违背 AVL 条件，应进行旋转（情况 3）
				UpdateRightTree(tree, reviseBalanceFactor);
		}
		else
			reviseBalanceFactor = false;
	}
}

template <class T>
void AVLTree<T>::Insert(const T& item)
{
	// 定义指向 AVL 树结点的指针
	AVLTreeNode<T> *treeRoot = root, *newNode;
	
	// 供 AVLInsert 重新平衡结点的标志
	bool reviseBalanceFactor = false;
	
	newNode = new AVLTreeNode<T>(item,NULL,NULL);
	
	// 调用递归函数实际插入元素
	AVLInsert(treeRoot, newNode, reviseBalanceFactor);
	
	// 赋新值给基类中的数据成员
	root = treeRoot;
	current = newNode;
	size++;
}

// 若当前结点已定义且数据值与给定数据值相等，则将结点值赋给 item；否则，将 item 插入到树中
template <class T>
void AVLTree<T>::Update(const T& item)
{   
    if (current != NULL && current->data == item)
		current->data = item;
    else
        Insert(item);
}

// 返回根结点的地址
template <class T>
AVLTreeNode<T> *AVLTree<T>::GetRoot(void) const
{
    return root;
}

template <class T>
void AVLTree<T>::AVLDelete(AVLTreeNode<T>* &tree, AVLTreeNode<T>* newNode, bool &reviseBalanceFactor)
{
	// 太过复杂，又不常用（AVL 树应主要用于初始化，因为其插入或删除的成本过高）
	throw "AVLTree::AVLDelete: Function not implement";
}

template <class T>
void AVLTree<T>::Delete(const T& item)
{
	throw "AVLTree::Delete: Function not implement";
}

#endif  // AVL_TREE_CLASS
