#ifndef ARRAY_CLASS
#define ARRAY_CLASS

#include <stdlib.h>

template <class T>
class Array
{
	private:
		// 一个动态申请的包含 size 个元素的表
		T *alist;
		int size;

	public:
		// 构造函数和析构函数
		Array(int sz = 50);
		Array(const Array<T>& A);
		~Array(void);

		// 赋值，下标和指针转换操作
		Array<T>& operator= (const Array<T>& rhs);
		T& operator[](int i);
		operator T* (void) const;

		// 有关表大小的函数
		int ListSize(void) const;  // 取表的大小
		void Resize(int sz);       // 修改表的大小
};

// 构造函数
template <class T>
Array<T>::Array(int sz)
{
	// 检查数组的大小参数是否合法
	if (sz <= 0)
		ExRaiseStatus(STATUS_INVALID_PARAMETER);

	size = sz;
	alist = new T[size];
}

// 析构函数
template <class T>
Array <T>::~Array(void)
{
	delete [] alist;
}

// 复制构造函数
template <class T>
Array <T>::Array(const Array<T> &A)
{
	// 取得对象 A 的大小并将其赋值给当前对象
	int n = A.size;
	size = n;
	
	// 为对象申请新内存并进行出错检查
	alist = new T[n];  // 申请动态内存
	
	T *srcptr = A.alist;  // A.alist 的首地址
	T *destptr = alist;   // alist 的首地址
	while (n--)			  // 拷贝表
		*destptr++ = *srcptr++;
}

// 赋值操作，将 rhs 分配到当前对象
template <class T>
Array<T>& Array<T>::operator= (const Array<T>& rhs)
{
	// 记录 rhs 的大小
	int n = rhs.size;
	
	// 如果 sizes 不相同，删除内存并重新分配
	if (size != n)
	{
		delete [] alist;   // 释放原始内存
		alist = new T[n];  // 分配新数组
		size = n;
	}
	
	// 将数组项目从 rhs 拷贝到当前对象
	T* destptr = alist;
	T* srcptr = rhs.alist;
    while (n--) 
        *destptr++ = *srcptr++;
	
	// 返回当前对象的引用
	return *this;
}

// 重载下标运算符
template <class T>
T& Array<T>::operator[](int n)
{
	// 数据越界检查
	if (n < 0 || n > size - 1)
		ExRaiseStatus(STATUS_INVALID_PARAMETER);//throw "Array::operator[]: invalid index";
	
	// 从私有数组中返回元素值
	return alist[n];
}

// 指针转换运算符
template <class T>
Array<T>::operator T* (void) const
{
	// 返回当前对象中私有数组的首地址
	return alist;
}

template <class T>
int Array<T>::ListSize(void) const
{
	return size;
}

// 调整数组大小运算符
template <class T>
void Array<T>::Resize(int sz)
{
	// 检查新的大小参数；若其小于等于0，则退出程序
	if (sz <= 0)
		ExRaiseStatus(STATUS_INVALID_PARAMETER);//throw "Array::Array: Invalid array size";
	
	// 若大小不变，则简单返回
	if (sz == size)
		return;
	
	// 需申请新的内存；确认系统已分配所需内存
	T *newlist = new T[sz];
	
	// n 为需拷贝元素的个数
	int n = (sz < size) ? sz : size;
	
	// 从旧表中拷贝 n 个数组元素的新表 
	T* srcptr = alist;		// alist 的首地址
	T* destptr = newlist;	// newlist 的首地址
    while (n--)				// 拷贝表
        *destptr++ = *srcptr++;
	
	// 删除旧表
	delete [] alist;
	
	// 将 alist 指针指向 newlist 并改变大小值
	alist = newlist;
	size = sz;
}

#endif  // ARRAY_CLASS
