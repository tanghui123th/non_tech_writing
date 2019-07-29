13 STL2
=======

13.1 set和multiset
------------------

预备知识：pair模板
~~~~~~~~~~~~~~~~~~

map/multimap容器里放的都是pair模板类的对象，且按照first从小到大排序。

.. code:: cpp

   template<class _T1, class _T2>
   struct pair{
       typedef _T1 first_type;
       typedef _T2 second_type;
       _T1 first;
       _T2 second;
       // 下面是三个构造函数
       pair(): first(), second(){}
       pair(const _T1 & __a, const _T2 & _b): first(__a), second(__b){}
       template<class _U1, class _U2>
       pair(const pair<_U1, _U2> & __p): first(__p.first), second(__p.second){}
   };

第三个构造函数的使用实例

.. code:: cpp

   pair<int, int> p(pair<double, double>(5.5, 4.6)); // p.first = 5, p.second = 6

关联容器
~~~~~~~~

set、multiset、map、multimap

特点：

-  内部元素有序排列，新元素插入的位置取决于它的值，查找速度快。
-  除了各容器都有的函数外，还支持一下成员函数

   -  find：查找等于某个值的元素（x小于y和y小于x同时不成立即相等）
   -  lower_bound：查找某个下界
   -  upper_bound：查找某个上界
   -  equal_range：同时查找上界和下界
   -  count：计算等于某个值的元素的个数
   -  insert：插入一个元素或一个区间

multiset
~~~~~~~~

.. code:: cpp

   template<class Key, class Pred = less<Key>, class A = allocator<Key>>
   class{...}

-  Pred类型的变量决定了multiset中的元素“一个比另一个小”是怎么定义的。multiset运行过程中，比较两个元素x，y的大小的做法，就是生成一个Pred类型的变量(实际中可能是函数指针，也可能是函数对象)，假定为op，若\ ``op(x,y)``\ 返回true，则x比y小。Pred的缺省类型是\ ``less<Key>``\ 。

``less<Key>``\ 的模板定义

.. code:: cpp

   template<class T>
   struct less:public binary_function<T, T, bool>{
       bool operator()(const T &x const T &y){return x < y;};  // 即为用小于号比大小
   };

一个错误的使用例子：

.. code:: cpp

   #include<set>
   using namespace std;

   class A{};

   int main(){
       multiset<A> a;
       a.insert(A());
   }

``multiset<A> a;``\ 实际上还有一个默认的less，即\ ``multiset<A, less<A>> a;``\ 。在插入时，multiset会将被插入元素和已有元素进行比较。less模板用<进行比较，但是这里A类对象不支持<。所以出错。

正确用法实例：

.. code:: cpp

   #include<iostream>
   #include<set>
   using namespace std;

   template<class T>
   void Print(T first, T last){
       for(; first != last; ++first) cout << *first << " ";
       cout << endl;
   }

   class A{
   private:
       int n;
   public:
       A(int n_){n = n_;}
       
   friend bool operator < (const A &a1, const A &a2){return a1.n < a2.n;}
   friend ostream & operator << (ostream &o, const A &a2){o << a2.n; return o;}
   friend class MyLess;
   };

   // 按个位数比大小
   struct MyLess{
       bool operator () (const A &a1, const A &a2){
           return (a1.n % 10) < (a2.n % 10);
       }
   };

   typedef multiset<A> MSET1;
   typedef multiset<A, MyLess> MSET2;

   int main(){
       const int SIZE = 6;
       A a[SIZE] = {4, 22, 19, 8, 33, 40};
       MSET1 m1;
       m1.insert(a, a + SIZE);
       m1.insert(22);

       cout << "1) " << m1.count(22) << endl;

       cout << "2) "; Print(m1.begin(), m1.end());

       MSET1::iterator pp = m1.find(19);
       if(pp != m1.end())
           cout << "found" << endl;

       // lower_bound包括bound，upper_bound则不包括
       cout << "3) " << *m1.lower_bound(22) << ", " << *m1.upper_bound(22) << endl;

       pp = m1.erase(m1.lower_bound(22), m1.upper_bound(22));
       cout << "4) "; Print(m1.begin(), m1.end());

       // pp 指向被删除元素的下一个元素
       cout << "5) "; cout << *pp << endl;

       MSET2 m2;
       m2.insert(a, a + SIZE);
       cout << "6) "; Print(m2.begin(), m2.end());

       return 0;
   }

输出

::

   1) 2
   2) 4 8 19 22 22 33 40 
   found
   3) 22, 33
   4) 4 8 19 33 40 
   5) 33
   6) 40 22 33 4 8 19

set
~~~

没有重复元素。（重复的定义：\ ``a < b``,
``b < a``\ 皆不成立）。插入重复元素时，自动忽略。
