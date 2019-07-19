# 11 模板

## 函数模板

我们写一个交换两个变量值得函数，如果没有模板，则对每种类型，都要实现一遍。

函数模板写法（class也可以写成typename）：

```
template <class 类型参数1, class 类型参数2, ...>
返回值类型 模板名(形参表){
    函数体;
}
```

交换两个变量：

```cpp
template <class T>
void Swap(T &x, T &y){
    T tmp = x;
    x = y;
    y = tmp;
}
```

> 模板的原理是编译器根据传入的变量类型自动生成一个对应类型的函数来调用。

模板函数中可以有不止一个类型参数。

```cpp
template<class T1, class T2>
T2 print(T1 arg1, T2 arg2){
    cout << arg1 << " " << arg2 << endl;
    return arg2;
}
```

也可以不通过参数实例化函数模板，而是直接写上类型

```cpp
#include<iostream>
using namespace std;

template<class T>
T Inc(T n){
    return 1 + n;
}

int main(){
    cout << Inc<double>(4) / 2;  // 输出2.5
}
```

函数模板也可以重载，只要他们的形参表或类型参数表不同即可。

```cpp
template<class T1, class T2>
void print(T1 arg1, T2 arg2){
    cout << arg1 << " " << arg2 << endl;
}

template<class T>
void print(T arg1, T arg2){
    cout << arg1 << " " << arg2 << endl;
}

template<class T, class T2>
void print(T arg1, T arg2){
    cout << arg1 << " " << arg2 << endl;
}
```

在有多个函数和函数模板名字相同的情况下，编译器如下处理一条函数调用语句

1. 先找参数完全匹配的普通函数（非由模板实例化而得的函数）
2. 再找参数完全匹配的模板函数
3. 再找实参经过自动类型转换后能匹配的普通函数。
4. 上面的都找不到，则报错。

匹配模板函数时，不会进行自动类型转换

```cpp
template<class T>
T myFunction(T arg1, T arg2){
    cout << arg1 << " " << arg2 << endl;
    return arg1;
}

...

myFunction(5, 7);     // ok: replace T with int
myFunction(5.8, 8.4); // ok: replace T with double
myFunction(5, 8.4);   // error: no matching function for call to 'myFunction(int, double)'
```

函数模板实例：Map（把从s到e，不包括e，的元素进行op转换，放到以x开始的目标位置）

```cpp
#include<iostream>
using namespace std;

template<class T, class Pred>
void Map(T s, T e, T x, Pred op){    // T的类型应该是指针，Pred的类型应该是函数指针
    for(; s != e; ++s, ++x){
        *x = op(*s);
    }
}

int Cube(int x){
    return x * x * x;
}

double Square(double x){
    return x * x;
}

int a[5] = {1,2,3,4,5}, b[5];
double c[5] = {1.1, 2.2, 3.3, 4.4, 5.5}, d[5];

int main(){
    Map(a, a+5, b, Square);
    for(int i = 0; i < 5; ++i) cout << b[i] << ", ";
    cout << endl;

    Map(a, a+5, b, Cube);
    for(int i = 0; i < 5; ++i) cout << b[i] << ", ";
    cout << endl;

    Map(c, c+5, d, Square);
    for(int i = 0; i < 5; ++i) cout << d[i] << ", ";
    cout << endl;
    return 0;
}
```

输出

```
1, 4, 9, 16, 25, 
1, 8, 27, 64, 125, 
1.21, 4.84, 10.89, 19.36, 30.25,
```

## 类模板

类模板的定义

```
template<class 类型参数1, class 类型参数2, ...>
class 类模板名{
    成员函数和成员变量
};
```

类模板示例：Pair类模板实现

```cpp
#include<iostream>
#include<string>
using namespace std;

template<class T1, class T2>
class Pair{
public:
    T1 key;
    T2 value;
    Pair(T1 k, T2, v):key(k), value(v){};
    bool operator < (const Pair<T1, T2> &p) const;
};

template<class T1, class T2>
bool Pair<T1, T2>::operator < (const Pair<T1, T2> &p) const{
    return key < p.key;
}

int main(){
    Pair<string, int> student("Tom", 19);
    cout << student.key << " " << student.value; // 输出Tom 19
    return 0;
}
```

编译器由类模板生成类的过程叫类模板的实例化。由类模板实例化得到的类，叫模板类。

- 同一个类模板的两个模板类是不兼容的。

如

```cpp
Pair<string, int> *p;
Pair<string, double> a;
p = &a;   // 会出错
```

函数模板作为类模板成员

```cpp
#include<iostream>
using namespace std;

template<class T>
class A{
public:
    template<class T2>
    void Func(T2 t){cout << t;} // 成员函数模板
};

int main(){
    A<int> a;
    a.Func('K');
    a.Func("hello");
    return 0;           // 输出Khello
}
```

类模板与非类型参数：非类型参数可以直接传入

```cpp
template<class T, int size>
class CArray{
    T array[size];
public:
    void Print(){
        for(int i = 0; i < size; ++i)
            cout << array[i] << endl;
    }
};

CArray<double, 40> a2;
CArray<int, 50> a3;
```

## 类模板与继承

有四种情况

- 类模板从类模板派生
- 类模板从模板类派生
- 类模板从普通类派生
- 普通类从模板类派生

### 类模板从类模板派生

```cpp
template<class T1, class T2>
class A{
    T1 v1;
    T2 v2;
};

template<class T1, class T2>
class B:public A<T2, T1>{
    T1 v3;
    T2 v4;
};

template<class T>
class C:public B<T, T>{
    T v5;
};

int main(){
    B<int, double> obj1;
    C<int> obj2;
    return 0;
}
```

### 类模板从模板类派生

```cpp
template<class T1, class T2>
class A{
    T1 v1;
    T2 v2;
};

template<class T>
class B:public A<int, double>{      // 这里的A不再是模板，而是模板生成的具体的类
    T v;
};

int main(){
    B<char> obj1;
    return 0;
}
```

### 类模板从普通类派生

```cpp
class A{
    int v1;
};

template<class T>
class B:public A{
    T v;
};

int main(){
    B<char> obj1;
    return 0;
}
```

### 普通类从模板类派生

```cpp
template<class T>
class A{
    T v1;
    int n;
};

class B:public A<int>{
    double v;
};

int main(){
    B obj1;
    return 0;
}
```

## 类模板与友元

可以类比继承，注意模板变量即可。我们来看一个函数模板作为类模板友元的例子

```cpp
#include<iostream>
#include<string>
using namespace std;

template<class T1, class T2>
class Pair{
private:
    T1 key;
    T2 value;
public:
    Pair(T1 k, T2 v):key(k), value(v){};
    bool operator < (const Pair<T1, T2> &p)const;
    template<class T3, class T4>
    friend ostream & operator <<(ostream &o, const Pair<T3, T4> &p);
};

template<class T1, class T2>
bool Pair<T1, T2>::operator < (const Pair<T1, T2> &p)const{
    return key < p.key;
}

template<class T1, class T2>
ostream & operator << (ostream &o, const Pair<T1, T2> &p){
    o << "(" << p.key << ", " << p.value << ")";
    return o;
}

int main(){
    Pair<string, int> student("Tom", 29);
    Pair<int, double> obj(12, 3.14);
    cout << student << " " << obj;      // 输出(Tom, 29) (12, 3.14)
    return 0;
}
```

## 类模板与静态成员

类模板中可以定义静态成员，从该类模板实例化得到的所有类，都包含相同的静态成员。但是注意实例化出的不同的类的静态成员是不同的。

```cpp
#include<iostream>
using namespace std;

template<class T>
class A{
private:
    static int count;
public:
    A(){count++;}
    ~A(){count--;}
    A(A &){count++;}
    static void PrintCount(){cout << count << endl;}
};

template<> int A<int>::count = 0;   // 有些编译器不用写template<>
template<> int A<double>::count = 0;

int main(){
    A<int> ia;
    A<double> da;
    ia.PrintCount();
    da.PrintCount();
    return 0;
}
```

输出

```
1
1
```