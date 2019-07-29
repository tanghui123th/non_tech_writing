# 13 STL2

## 13.1 set和multiset

### 预备知识：pair模板

map/multimap容器里放的都是pair模板类的对象，且按照first从小到大排序。

```cpp
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
```

第三个构造函数的使用实例

```cpp
pair<int, int> p(pair<double, double>(5.5, 4.6)); // p.first = 5, p.second = 6
```

### 关联容器

set、multiset、map、multimap

特点：

- 内部元素有序排列，新元素插入的位置取决于它的值，查找速度快。
- 除了各容器都有的函数外，还支持一下成员函数
   - find：查找等于某个值的元素（x小于y和y小于x同时不成立即相等）
   - lower_bound：查找某个下界
   - upper_bound：查找某个上界
   - equal_range：同时查找上界和下界
   - count：计算等于某个值的元素的个数
   - insert：插入一个元素或一个区间

### multiset

```cpp
template<class Key, class Pred = less<Key>, class A = allocator<Key>>
class{...}
```

- Pred类型的变量决定了multiset中的元素“一个比另一个小”是怎么定义的。multiset运行过程中，比较两个元素x，y的大小的做法，就是生成一个Pred类型的变量(实际中可能是函数指针，也可能是函数对象)，假定为op，若`op(x,y)`返回true，则x比y小。Pred的缺省类型是`less<Key>`。

`less<Key>`的模板定义

```cpp
template<class T>
struct less:public binary_function<T, T, bool>{
    bool operator()(const T &x const T &y){return x < y;};  // 即为用小于号比大小
};
```

一个错误的使用例子：

```cpp
#include<set>
using namespace std;

class A{};

int main(){
    multiset<A> a;
    a.insert(A());
}
```

`multiset<A> a;`实际上还有一个默认的less，即`multiset<A, less<A>> a;`。在插入时，multiset会将被插入元素和已有元素进行比较。less模板用<进行比较，但是这里A类对象不支持<。所以出错。

正确用法实例：

```cpp
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
```

输出

```
1) 2
2) 4 8 19 22 22 33 40 
found
3) 22, 33
4) 4 8 19 33 40 
5) 33
6) 40 22 33 4 8 19
```

### set

没有重复元素。（重复的定义：`a < b`, `b < a`皆不成立）。插入重复元素时，自动忽略。

set使用的例子

```cpp
#include<iostream>
#include<set>
using namespace std;

int main(){
    typedef set<int>::iterator IT;
    int a[5] = {3, 4, 6, 1, 2};
    set<int> st(a, a + 5);
    pair<IT, bool> result;
    result = st.insert(5);  // insert返回一个pair，first类型是迭代器，second是bool，插入成功为true，否则为false
    if(result.second)
        cout << *result.first << " inserted" << endl;
    if(st.insert(5).second) cout << *result.first << " inserted again" << endl;
    else cout << *result.first << " already exists" << endl;

    pair<IT, IT> bounds = st.equal_range(4);
    cout << *bounds.first << ", " << *bounds.second << endl;
    return 0;
}
```

输出

```
5 inserted
5 already exists
4, 5
```

## 13.2 map和multimap

### multimap

定义如下

```cpp
template<class Key, class T, class Pred = less<Key>, class A = allocator<T>>
class multimap{
    ...
    typdef pair<const Key, T> value_type;
    ...
};
```

- multimap中的元素有`<关键字，值>`组成，每个元素是一个pair对象，关键字就是first成员变量，其类型是Key。
- multimap中允许多个元素的关键字相同。元素按照first成员变量从小到大排列。缺省情况下用less定义小于关系。

一个multimap的例子

```cpp
#include<iostream>
#include<map>
using namespace std;

int main(){
    typedef multimap<int, double, less<int>> mmid;
    mmid pairs;
    cout << "1) " << pairs.count(15) << endl;

    pairs.insert(mmid::value_type(15, 2.7));
    pairs.insert(mmid::value_type(15, 99.3));
    cout << "2) " << pairs.count(15) << endl;  // 求key等于15的元素个数

    pairs.insert(mmid::value_type(30, 111.11));
    pairs.insert(mmid::value_type(10, 22.22));
    pairs.insert(mmid::value_type(25, 33.333));
    pairs.insert(mmid::value_type(20, 9.3));
    for(mmid::const_iterator i = pairs.begin(); i != pairs.end(); i++){
        cout << "(" << i->first << ", " << i->second << ")" << ", ";
    }
    cout << endl;
}
```

输出

```
1) 0
2) 2
(10, 22.22), (15, 2.7), (15, 99.3), (20, 9.3), (25, 33.333), (30, 111.11), 
```

### map

定义如下

```cpp
template<class Key, class T, class Pred = less<Key>, class A = allocator<T>>
class map{
    ...
    typdef pair<const Key, T> value_type;
    ...
};
```

map中的元素都是pair模板类对象。map不同于multimap两点：

- 关键字（first）各不相同。
- 有[]成员函数。

`paris[key]`返回对关键字key的元素的值（second成员变量）的引用。若没有关键字为key的元素，则会往pairs中插入一个关键字为key的元素，并返回其值得引用。

```cpp
#include<iostream>
#include<map>
using namespace std;

template<class Key, class Value>
ostream &operator << (ostream &o, const pair<Key, Value> &p){
    o << "(" << p.first << ", " << p.second << ")";
    return o;
}

int main(){
    typedef map<int, double, less<int>> mmid;
    mmid pairs;

    cout << "1) " << pairs.count(15) << endl;

    pairs.insert(mmid::value_type(15, 2.7));
    pairs.insert(make_pair(15, 99.3));  // 用make_pair生成一个pair对象，插入会失败
    cout << "2) " << pairs.count(15) << endl;

    pairs.insert(mmid::value_type(20, 9.3));
    mmid::iterator i;
    cout << "3) " ;
    for(i = pairs.begin(); i != pairs.end(); i++){
        cout << *i << ", ";
    }
    cout << endl;

    cout << "4) ";
    int n = pairs[40];
    for(i = pairs.begin(); i != pairs.end(); i++)
        cout << *i << ", ";
    cout << endl;

    cout << "5) ";
    pairs[15] = 6.28;
    for(i = pairs.begin(); i != pairs.end(); i++)
        cout << *i << ", ";
    cout << endl;
}
```

输出

```
1) 0
2) 1
3) (15, 2.7), (20, 9.3), 
4) (15, 2.7), (20, 9.3), (40, 0), 
5) (15, 6.28), (20, 9.3), (40, 0),
```

## 13.3 容器适配器

容器适配器是没有迭代器的，因此stl中的算法不能作用于容器适配器，只能使用容器适配器自己的成员函数。

### stack

stack内部可以用vecor、list、deque实现。缺省情况下是用deque实现的。用vector和deque实现，比用list实现性能好。

```cpp
template<class T, class Cont = deque<T>>
class stack{
    ...
};
```

### queue

和stack基本类似。可用list和vector实现，缺省情况下用deque实现。

### priority_queue

```cpp
template<class T, class Container = vector<T>, class Compare = less<T>>
class priority_queue;
```

可以用vector和deque实现，缺省情况下用vector实现。

priority_queue通常用堆排序基数实现，保证最大的元素总是排在最前面。默认比较器是less<T>。push，pop时间复杂度是O(logn)，top()时间复杂度是O(1)。

```cpp
#include<queue>
#include<iostream>
using namespace std;

int main(){
    priority_queue<double> pq1;
    pq1.push(3.2); pq1.push(9.8); pq1.push(9.8); pq1.push(5.4);
    while(!pq1.empty()){
        cout << pq1.top() << " ";
        pq1.pop();
    }

    cout << endl;
    priority_queue<double, vector<double>, greater<double>> pq2; // 为了写第三个参数，必须也写第二个参数
    pq2.push(3.2); pq2.push(9.8); pq2.push(9.8); pq2.push(5.4);
    while(!pq2.empty()){
        cout << pq2.top() << " ";
        pq2.pop();
    }
    return 0;
}
```

输出

```
9.8 9.8 5.4 3.2 
3.2 5.4 9.8 9.8
```