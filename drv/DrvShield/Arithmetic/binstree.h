/* 二叉搜索树
 * 
 */

#ifndef BINARY_SEARCH_TREE_CLASS
#define BINARY_SEARCH_TREE_CLASS

#include <stdlib.h>
#include "treenode.h"

template <class T>
class BinSTree
{
    private:
        // 指向树根及当前结点的指针
        TreeNode<T> *root;
        TreeNode<T> *current;
        
        // 树中数据项个数
        int size;
      
        // 用于复制构造函数及赋值运算符
        TreeNode<T> *CopyTree(TreeNode<T> *t);
        
        // 用于析构函数，赋值运算符及 ClearList 方法
        void DeleteTree(TreeNode<T> *t);

        // 在函数 Find 和 Delete 中用来定位结点及其双亲在树中的位置
        TreeNode<T> *FindNode(const T& item, TreeNode<T>* & parent) const;

    public:
        // 构造函数，析构函数
        BinSTree(void);
        BinSTree(const BinSTree<T>& tree);
        ~BinSTree(void);
        
        // 赋值运算符
        BinSTree<T>& operator= (const BinSTree<T>& rhs);
        
        // 标准的表处理方法
        bool Find(T& item);
        void Insert(const T& item);
        void Delete(const T& item);
        void ClearList(void);
        bool ListEmpty(void) const;
        int ListSize(void) const;
        
        // 树的特殊方法
        void Update(const T& item);
        TreeNode<T> *GetRoot(void) const;
};

// 复制树 t 并使其存储在当前对象中
template <class T>
TreeNode<T> *BinSTree<T>::CopyTree(TreeNode<T> *t)
{
    TreeNode<T> *newlptr, *newrptr, *newNode;
   
    // 如果树分支为空，返回 NULL
    if (t == NULL)
        return NULL;
        
    // 复制树 t 的左子树并将其根分配给 newlptr
    if (t->left != NULL) 
        newlptr = CopyTree(t->left);
    else
        newlptr = NULL;
 
    // 复制树 t 的右子树并将其根分配给 newrptr
    if (t->right != NULL) 
        newrptr = CopyTree(t->right);
    else
        newrptr = NULL;
 
    // 为当前根结点分配存储器并将其数据值和指针分配给它的子树，返回其指针
    newNode = new TreeNode<T>(t->data, newlptr, newrptr);
    return newNode;
}

// 删除当前对象存储的树
template <class T>
void BinSTree<T>::DeleteTree(TreeNode<T> *t)
{
    if (t != NULL)
    {
        DeleteTree(t->left);
        DeleteTree(t->right);
        delete t;
    }
}

// 在树中搜索数据项，若找到，则返回结点地址及指向其双亲的指针；否则，返回 NULL
template <class T>
TreeNode<T> *BinSTree<T>::FindNode(const T& item, TreeNode<T>* & parent) const
{   
    // 用指针 t 从根开始遍历树
    TreeNode<T> *t = root;
    
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

// 构造函数，初始化 root，current 为空，size 为 0
template <class T>
BinSTree<T>::BinSTree(void):root(NULL), current(NULL), size(0)
{}

// 复制构造函数
template <class T>
BinSTree<T>::BinSTree(const BinSTree<T>& tree)
{
    // 将 tree 复制到当前对象，分配 current 和 size
    root = CopyTree(tree.root);
    current = root;
    size = tree.size;
}

// 析构函数
template <class T>
BinSTree<T>::~BinSTree(void)
{
    ClearList();
}

// 删除树中的所有结点
template <class T>
void BinSTree<T>::ClearList(void)
{
    DeleteTree(root);
    root = current = NULL;
    size = 0;
}

// 赋值运算符
template <class T>
BinSTree<T>& BinSTree<T>::operator= (const BinSTree<T>& rhs)
{
    // 不能将树复制到自身
    if (this == &rhs)
        return *this;
        
    // 清除当前树，将新树复制到当前对象
    ClearList();
    root = CopyTree(rhs.root);
    
    // 将 current 指针指向 root 并设置树的 size 值
    current = root;
    size = rhs.size;
    
    // 返回当前对象的指针
    return *this;
}

// 在树中搜索 item，若找到，则将结点数据赋给 item
template <class T>
bool BinSTree<T>::Find(T& item)
{
    // 使用 FindNode，它需要 parent 参数
    TreeNode<T> *parent;

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
bool BinSTree<T>::ListEmpty(void) const
{
    return (size == 0);
}

// 返回树中的数据项个数
template <class T>
int BinSTree<T>::ListSize(void) const
{
    return size;
}

// 往查找树中插入数据项，若元素重复，则更新现有元素
template <class T>
void BinSTree<T>::Insert(const T& item)
{
    // t 为遍历过程中的当前结点，parent 为前一结点
    TreeNode<T> *parent = NULL;

	current = FindNode(item, parent);
    
	if (current != NULL)
		current->data = item;
	else
	{
		// 创建新的叶子结点
		TreeNode<T> *newNode = new TreeNode<T>(item,NULL,NULL);
		
		// 若 parent 为 NULL，则将其作为根结点插入
		if (parent == NULL)
			root = newNode;
        
		// 若 item < parent->data，则将其作为左孩子插入        
		else if (item < parent->data)                   
			parent->left = newNode;
        
		else
			// 若 item >= parent->data，作为右孩子插入     
			parent->right = newNode;
        
		// current 赋值为新结点的地址并将 size 加 1
		current = newNode;
		size++;
	}
}

// 如果 item 在树中，将其删除
template <class T>
void BinSTree<T>::Delete(const T& item)
{
    // DNodePtr = 指向被删除结点 D 的指针
    // PNodePtr = 指定结点 D 的双亲节点 P 的指针
    // RNodePtr = 指向替换 D 的结点 R 的指针
    TreeNode<T> *DNodePtr, *PNodePtr, *RNodePtr;
    
    // 搜索数据值为 item 的结点，并保存该结点的双亲结点的指针
    if ((DNodePtr = FindNode (item, PNodePtr)) == NULL)
        return;
    
    // 如果 D 有一个指针为 NULL，则替换结点为其另一枝的某一结点
    if (DNodePtr->right == NULL)
        RNodePtr = DNodePtr->left;
    else if (DNodePtr->left == NULL)
        RNodePtr = DNodePtr->right;
        
    // DNodePtr 的两个指针均不为 NULL
    else
    {
        // 寻找并卸下 D 的替换结点。从结点 D 的左子树开始，找数据值小于 D 的数据值的
        // 最大值，将该结点从树中断开
        
        // PofRNodePtr = 指向替换结点双亲的指针
        TreeNode<T> *PofRNodePtr = DNodePtr;
        
        // 第一种可能的替换为 D 的左孩子
        RNodePtr = DNodePtr->left;
    
        // 从 D 的左孩子的右子树继续往下搜索最大值，并记录当前结点及其双亲结点的
        // 指针，最后，我们将找到替换结点
        while(RNodePtr->right != NULL)
        {
            PofRNodePtr = RNodePtr;
            RNodePtr = RNodePtr->right;
        }
        
        if (PofRNodePtr == DNodePtr)
            // 被删除结点的左孩子为替换结点，将 D 的右子树赋给 R
            RNodePtr->right = DNodePtr->right;
        else
        {
            // 至少往右子树移动了一个结点，从树中删除替换结点，将其左子树赋给其双亲
            PofRNodePtr->right = RNodePtr->left;
            
            // 用替换结点代替 DNodePtr
            RNodePtr->left = DNodePtr->left;
            RNodePtr->right = DNodePtr->right;
        }
    }

    // 完成到双亲结点的连接。删除根结点，并给新更赋值
    if (PNodePtr == NULL)
        root = RNodePtr;
        
    // 将 R 连到 P 的正确一枝上
    else if (DNodePtr->data < PNodePtr->data)
        PNodePtr->left = RNodePtr;
    else
        PNodePtr->right = RNodePtr;
        
    // 释放被删结点内存并将树的大小减 1
    delete DNodePtr;
    size--;
}

// 若当前结点已定义且数据值与给定数据值相等，则将结点值赋给 item；否则，将 item 插入到树中
template <class T>
void BinSTree<T>::Update(const T& item)
{   
    if (current != NULL && current->data == item)
            current->data = item;
    else
        Insert(item);
}

// 返回根结点的地址
template <class T>
TreeNode<T> *BinSTree<T>::GetRoot(void) const
{
    return root;
}

#endif  // BINARY_SEARCH_TREE_CLASS
