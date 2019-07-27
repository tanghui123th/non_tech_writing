# 12 STL1

## 12.1 string类

string类实际上是一个模板类：

```cpp
typedef basic_string<char> string;
```

`string_obj.at(i)`会做范围检查，如果超出范围，会抛出out_of_range异常，而[]不做范围检查。

### 转换成c语言式char *字符串

```cpp
string s1("hello world");
printf("%s\n", s1.c_str());  // c_str()返回const char *类型字符串，且以\0结尾
```

另外还有一个函数data()，返回的是char *的字符串

### 字符串流处理

类似istream和ostream进行标准流输入输出，我们用istringstream和ostringstream进行字符串上的输入输出，也称内存输入输出。

把字符串作为输出的内容

```cpp
#include<iostream>
#include<string>
#include<sstream>
using namespace std;

int main(){
    string input("Input test 123 4.7 A");
    istringstream inputString(input);
    string string1, string2;
    int i;
    double d;
    char c;
    inputString >> string1 >> string2 >> i >> d >> c;
    cout << string1 << endl << string2 << endl;
    cout << i << endl << d << endl << c << endl;
    long L;
    if(inputString >> L) cout << "long\n";
    else cout << "empty\n";
    return 0;
}
```

输出

```
Input
test
123
4.7
A
empty
```

字符串输出流

```cpp
ostringstream outputString;
int a = 10;
outputString << "This " << a << "ok" << endl;
cout << outputString.str();     // 成员函数str()返回一个string对象
```

## 12.2 标准模板库STL概述

### 泛型程序设计

c++语言的核心优势之一就是便于软件的重用。这体现在两个方面：

1. 面向对象的思想：继承和多态，标准类库
2. 泛型程序设计：模板机制、标准模板库

将一些常用的数据结构和算法写成模板，以后则不论数据结构里放的是什么对象，算法针对什么对象，都不必重新实现数据结构，重新编写算法。标准模板库就是一些常用数据结构和算法的模板的集合。

### STL的基本概念

- 容器：可容纳各种数据类型的通用数据结构，是类模板
- 迭代器：可用于依次存取容器中的元素，类似指针
- 算法：用来操作容器中的元素的函数模板。

### 容器概述

1. 顺序容器：vector, deque, list
2. 关联容器：set, multiset, map, multimap
3. 容器适配器：stack, queue, priority_queue

### 顺序容器

特点：顺序容器并非排序的，元素的插入位置同元素的值无关。

- vector：动态数组。头文件vector
  随机存取是常数时间。尾端增删是常数时间（如需扩容则不是）。中间增删是O(n)时间。
- deque：双向队列（动态的循环队列）。头文件deque
  随机存取元素是常数时间（比vector稍慢，因为是循环队列）。首尾增删都是常数时间（扩容则不是）。
- list：双向链表。头文件list
  任何位置增删都是常数时间。不支持随机存取。


### 关联容器

特点：关联容器元素是有序的。插入任何元素，都按相应的排序规则来确定其位置。在查找时性能很好。通常以平衡二叉树方式实现，插入和检索的时间都是O(log(n))

- set/multiset 头文件set
  set中不允许相同元素。multiset中可以有。
- map/multimap 头文件map
  map和set的不同之处在于map中存放的元素有且仅有两个成员变量。第一个名为first，第二个名为second。map根据first值对元素进行从小到大的排序。map与multimap不同之处在于是否允许相同的first值得元素。

### 容器适配器

- stack 头文件stack
- queue 头文件queue
- priority_queue 头文件queue。最高优先级的元素总是排在队头

### 顺序容器和关联容器中都有的成员函数

- begin：返回指向容器中第一个元素的迭代器
- end：返回指向容器中最后一个元素后面的位置的迭代器
- rbegin：返回指向容器中最后一个元素的迭代器
- rend：返回指向容器中第一个元素前面的位置的迭代器
- erase：从容器中删除一个或几个元素
- clear：从容器中删除所有元素

### 顺序容器常用成员函数

- front：返回容器中第一个元素的引用
- back：返回容器中最后一个元素的引用
- push_back：在容器末尾增加元素
- pop_back：删除容器末尾的元素
- erase：删除迭代器指向的元素（可能会使该迭代器失效），或删除一个区间，返回被删除元素后面的那个元素的迭代器

## 12.3 迭代器

- 用于指向顺序容器和关联容器中的元素
- 用法和指针类似（实际上就是指针实现的）

定义迭代器：

```cpp
容器类名::iterator 变量名;
// 或者
容器类名::const_iterator 变量名;
```

```cpp
#include<iostream>
#include<vector>
using namespace std;

int main(){
    vector<int> v;
    v.push_back(1); v.push_back(2); v.push_back(3); v.push_back(4);

    vector<int>::const_iterator i;
    for(i = v.begin(); i != v.end(); ++i){
        cout << *i << ", ";          // 输出1, 2, 3, 4, 
    }
    cout << endl;

    vector<int>::reverse_iterator r; // 反向迭代器, ++实际上是后退
    for(r = v.rbegin(); r != v.rend(); ++r){
        cout << *r << ", ";          // 输出4, 3, 2, 1, 
    }
    cout << endl;

    vector<int>::iterator j;  // 非常量迭代器
    for(j = v.begin(); j != v.end(); j++){
        *j = 100;
    }
    for(i = v.begin(); i != v.end(); ++i){
        cout << *i << ", ";         // 输出100, 100, 100, 100
    }
}
```

### 双向迭代器

若p和p1是双向迭代器，则可以对p、p1进行以下操作：

- ++p, p++：使p指向容器中下一个元素
- --p, p--：使p指向容器中上一个元素
- *p：取p指向的元素
- p = p1：赋值
- p == p1, p != p1：判断是否相等、不等

### 随机访问迭代器

若p和p1是随机访问迭代器，则可对p、p1进行以下操作：

- 双向迭代器的所有操作
- p += i 将p向后移动i个元素
- p -= i 将p向前移动i个元素
- p + i  值为：指向p后面的第i个元素的迭代器
- p - i  值为：指向p前面的第i个元素的迭代器
- p[i]  值为：p后面的第i个元素的引用
- p \< p1, p \<= p1, p \> p1, p \>= p1

| 容器           | 容器上的迭代器类别 |
| :----------- | :-------- |
| vector       | 随机访问      |
| deque        | 随机访问      |
| list         | 双向        |
| set/multiset | 双向        |
| map/multimap | 双向        |
| stack        | 不支持迭代器    |
| queue        | 不支持迭代器    |
| queue        | 不支持迭代器    |

> 有些算法，如sort， binary_search需要通过随机访问迭代器来访问容器中的元素，那么list以及关联容器就不支持该算法。

例如正确遍历list的方法：

```cpp
list<int> v;
list<int>::const_iterator ii;
for(ii = v.begin(); ii != v.end(); ++ii){
    cout << *ii;
}
```

以下是错误的用法：

```cpp
for(ii = v.begin(); ii < v.end(); ++ii){      // 双向迭代器不支持大小运算
    cout << *ii;
}

for(int i = 0; i < v.size(); ++i){
    cout << v[i];           // list没有[]成员函数
}
```

## 12.4 算法

算法就是一个个函数模板，大多数在`<algorithm>`中定义。算法通过迭代器来操纵容器中的元素。算法可以处理容器，也可以处理普通数组。

## 12.5 vector, deque, list

### vector

```cpp
#include<iostream>
#include<vector>
using namespace std;

template<class T>
void PrintVector(T s, T e){
    for(; s != e; ++s){
        cout << *s << " ";
    }
    cout << endl;
}

int main(){
    int a[5] = {1,2,3,4,5};
    vector<int> v(a, a+5);
    cout << "1) " << v.end() - v.begin() << endl;

    cout << "2) "; PrintVector(v.begin(), v.end());

    v.insert(v.begin()+2, 13);
    cout << "3) "; PrintVector(v.begin(), v.end());

    v.erase(v.begin()+2);
    cout << "4) "; PrintVector(v.begin(), v.end());

    vector<int> v2(4, 100);
    v2.insert(v2.begin(), v.begin()+1, v.begin()+3);
    cout << "5) "; PrintVector(v2.begin(), v2.end());

    v.erase(v.begin()+1, v.begin()+3);
    cout << "6) "; PrintVector(v.begin(), v.end());

    return 0;
}
```

输出

```cpp
1) 5
2) 1 2 3 4 5 
3) 1 2 13 3 4 5 
4) 1 2 3 4 5 
5) 2 3 100 100 100 100 
6) 1 4 5
```

### deque

所有适用于vector的操作都是用于deque。除此外，deque还有`push_front`, `pop_front`

### list

- 在任何位置插入和删除都是常数时间，不支持随机存取。
- 除了所有顺序容器都有的成员函数以外，还有8个成员函数
  - push_front
  - pop_front
  - sort (是自己的成员函数，不是algorithm中的sort)
  - remove：删除和指定值相等的所有元素
  - unique：删除所有和前一个元素相同的元素（要做到去重，可以先sort，再unique）
  - merge：合并两个链表，并清空被合并和那个
  - reverse：反转链表
  - splice：在指定位置的前面，插入另外一个链表的一个或多个元素，并在另外一个链表中删除被插入的元素。

```cpp
#include<iostream>
#include<list>
using namespace std;

class A{
private:
    int n;
public:
    A(int n_) {n = n_;}
    friend bool operator < (const A &a1, const A &a2);
    friend bool operator == (const A &a1, const A &a2);
    friend ostream & operator << (ostream &o, const A &a);
};

bool operator < (const A &a1, const A &a2){
    return a1.n < a2.n;
}

bool operator == (const A &a1, const A &a2){
    return a1.n == a2.n;
}

ostream &operator << (ostream &o, const A &a){
    o << a.n;
    return o;
}

template<class T>
void PrintList(const list<T> &lst){
    typename list<T>::const_iterator i;
    for(i = lst.begin(); i != lst.end(); i++){
        cout << *i << ",";
    }
}

int main(){
    list<A> lst1, lst2;

    lst1.push_back(1); lst1.push_back(3); lst1.push_back(2); lst1.push_back(4);
    lst1.push_back(2);

    lst2.push_back(10); lst2.push_front(20); lst2.push_back(30); lst2.push_back(30);
    lst2.push_back(30); lst2.push_back(40); lst2.push_front(40);

    cout << "1) "; PrintList(lst1); cout << endl; // 1,3,2,4,2
    cout << "2) "; PrintList(lst2); cout << endl; // 40,20,10,30,30,30,40,

    lst2.sort();
    cout << "3) "; PrintList(lst2); cout << endl; // 10,20,30,30,30,40,40,
    lst2.pop_front();
    cout << "4) "; PrintList(lst2); cout << endl; // 20,30,30,30,40,40,

    lst1.remove(2);
    cout << "5) "; PrintList(lst1); cout << endl; // 1,3,4,

    lst2.unique();
    cout << "6) "; PrintList(lst2); cout << endl; // 20,30,40,

    lst1.merge(lst2);
    cout << "7) "; PrintList(lst1); cout << endl; // 1,3,4,20,30,40,
    cout << "8) "; PrintList(lst2); cout << endl; // 

    lst1.reverse();
    cout << "9) "; PrintList(lst1); cout << endl; // 40,30,20,4,3,1,
}
```

## 12.6 函数对象

若一个类重载了运算符()，则该类的对象就成为函数对象

```cpp
class CMyAverage{
public:
    double operator () (int a1, int a2, int a3){
        return (double)(a1+a2+a3)/3;
    }
};

CMyAverage average; // 函数对象
cout << average(3,2,3); // 输出2.66667
```

我们来看一个稍微复杂点的例子。首先明确accumulate的作用

```cpp
template <class InputIterator, class T>
T accumulate (InputIterator first, InputIterator last, T init)
{
  while (first!=last) {
    init = init + *first;  // or: init=binary_op(init,*first) for the binary_op version
    ++first;
  }
  return init;
}
```

```cpp
#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<functional>
using namespace std;

int SumSquares(int total, int value){
    return total + value * value;
}

template<class T>
void PrintInterval(T first, T last){
    // 输出 [first, last)区间中的元素
    for(; first != last; ++first){
        cout << *first << " ";
    }
    cout << endl;
};

template<class T>
class SumPowers{
private:
    int power;
public:
    SumPowers(int p):power(p){}
    const T operator () (const T &total, const T &value){
        // 计算value的power次方，加到total上
        T v = value;
        for(int i = 0; i < power - 1; ++i){
            v = v * value;
        }
        return total + v;
    }
};

int main(){
    const int SIZE = 10;
    int a1[] = {1,2,3,4,5,6,7,8,9,10};
    vector<int> v(a1, a1+SIZE);
    cout << "1) "; PrintInterval(v.begin(), v.end());
    
    int result = accumulate(v.begin(), v.end(), 0, SumSquares);
    cout << "2) 平方和：" << result << endl;

    result = accumulate(v.begin(), v.end(), 0, SumPowers<int>(3));  // SumPowers<int>(3)是个对象，用3初始化SumPowers中的power
    cout << "3) 立方和：" << result << endl;

    result = accumulate(v.begin(), v.end(), 0, SumPowers<int>(4));
    cout << "4) 4次方和：" << result << endl;

    return 0;
}
```

输出

```cpp
1) 1 2 3 4 5 6 7 8 9 10 
2) 平方和：385
3) 立方和：3025
4) 4次方和：25333
```

STL中的函数对象类模板：equal_to, greater, less等。在头文件`<functional>`

我们来看看greater

```cpp
template<class T>
struct greater: public binary_function<T, T, bool>{
    bool operator () (const T &x, const T &y) const {
        return x > y;
    }
}
```

list的迭代器是双向迭代器，不能使用algorithm中的sort进行排序。只能用自身的成员函数sort进行排序。list有两个sort成员函数

```cpp
void sort();  // 将list中的元素按 < 规定的比较方法进行排序

template<class Compare>
void sort(Compare op);  // 将list中的元素按照op规定的比较方法排序。若op(x, y)返回true，则认为x<y
```

greater的应用

```cpp
#include<iostream>
#include<list>
using namespace std;

class MyLess{
public:
    bool operator () (const int &c1, const int &c2){
        return (c1 % 10) < (c2 % 10);
    }
};

template<class T>
void Print(T first, T last){
    for(; first != last; ++first){
        cout << *first << " ";
    }
}

int main(){
    const int SIZE = 5;
    int a[SIZE] = {5, 21, 14, 2, 3};
    list<int> lst(a, a + SIZE);
    lst.sort(MyLess());
    Print(lst.begin(), lst.end());
    cout << endl;

    lst.sort(greater<int>()); // greater<int>()是个对象
    Print(lst.begin(), lst.end());
    cout << endl;

    return 0;
}
```

输出

```cpp
21 2 3 14 5 
21 14 5 3 2
```