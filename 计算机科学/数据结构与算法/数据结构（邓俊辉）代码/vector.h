typedef int Rank;
#define DEFAULT_CAPACITY 3

// 公共方法大体分成下面几类
// 构造函数
// 析构函数
// 只读访问接口：size、empty、find、search
// 可写访问接口：[]、=、remove、insert、sort、unsort、deduplicate、uniquify
// 遍历

template<typename T>
class Vector {
protected:
    Rank _size; int _capacity; T* _elem;            // 规模、容量、数据区

    void copyFrom(T const* A, Rank lo, Rank hi);    // 复制数组区间 A[lo, hi)
    void expand();                                  // 空间不足时扩容
    void shrink();                                  // 装填因子过小时压缩

    // 下面是排序算法相关的方法
    bool bubble(Rank lo, Rank hi);           // 扫描交换
    void bubbleSort(Rank lo, Rank hi);       // 冒泡排序

    Rank max(Rank lo, Rank hi);              // 选取最大元素
    void selectionSort(Rank lo, Rank hi);    // 选择排序

    void merge(Rank lo, Rank mi, Rank hi);   // 归并算法
    void mergeSort(Rank lo, Rank hi);        // 归并排序算法

    void heapSort(Rank lo, Rank hi);         // 堆排序
    
    Rank partition(Rank lo, Rank hi);        // 轴点构造算法
    void quickSort(Rank lo, Rank hi);        // 快速排序

    void shellSort(Rank lo, Rank hi);        // 希尔排序

public:
    // 构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
    {
        //容量为c、规模为s、所有元素初始为v
        _elem = new T[_capacity=c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }
    // 下面四个复制构造函数分别是
    // 数组整体复制、数组区间复制、向量整体复制、向量区间复制
    Vector(T cosnt* A, Rank n)
    {
        copyFrom(A, 0, n);
    }
    Vector(T const* A, Rank lo, Rank hi)
    {
        copyFrom(A, lo, hi);
    }
    Vector(Vector<T> const& V)
    {
        copyFrom(V._elem, V._size);
    }
    Vector(Vector<T> const& V, Rank lo, Rank hi)
    {
        copyFrom(V._elem, lo, hi);
    }

    // 析构函数
    ~Vector() { delete [] _elem; }

    // 只读访问接口，
    // find是无需查找，查找等于e且秩最大的元素
    // search是有序查找，查找小于等于e，且秩最大的元素
    Rank size() const
    {
        return _size;
    }
    bool empty() const
    {
        return !_size;
    }

    Rank find(T const& e) const
    {
        return find(e, 0, _size);
    }
    Rank find(T const& e, Rank lo, Rank hi);

    Rank search(T const& e)
    {
        return _size <= 0 ? -1 : search(e, 0, _size);
    }
    Rank search(T const& e, Rank lo, Rank hi);

    // 可写访问接口
    T& operator[] (Rank r);                   // 重载下标运算符，使之可以向数组一样使用
    const T& operator[] (Rank r) const;       // 仅限于做右值的下标重载
    Vector<T>& operator= (Vector<T> const&);  //重载赋值操作符，以便直接克隆向量

    T remove(Rank r);
    int remove(Rank lo, Rank hi);

    Rank insert(Rank r, T const& e);
    Rank insert(T const& e)
    {   
        // 插在末尾
        return insert(_size, e);
    }

    void sort(Rank lo, Rank hi);
    void sort()
    {
        sort(0, _size);
    }

    void unsort(Rank lo, Rank hi);
    void unsort()
    {
        unsort(0, _size);
    }

    int deduplicate();      // 无需去重
    int uniquify();         // 有序去重

    // 遍历
    void traverse(void(*)(T&));   // 使用函数指针，只读或局部性修改

    template<typename VST>
    void traverse(VST&);          // 使用函数对象，可以全局修改
};

template<typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)
{
    //以数组区间A[lo, hi)为蓝本复制向量
    _elem = new T[_capacity = 2 * (hi - lo)];   // 重新分配空间，并把规模清零
    _size = 0;
    while (lo < hi)
        _elem[_size++] = A[lo++];
}
