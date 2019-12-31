# 5 First-Class Function

`First-Class Function`是`Functions as First-Class Objects`的缩写。

第一类对象要满足特征：

- 可以被存放在变量和数据结构中
- 可以当做参数传递给子过程
- 可以当子过程的返回值
- 可以在运行期间被创建

Guido本人不把python视为函数式编程语言，但函数是第一类对象的特征，使得在python中可以进行函数式编程。

## 5.1 高阶函数

### 概述

高阶函数：以函数作为参数，或者返回一个函数的函数

最常见的高阶函数有map, reduce, filter等。这里举一个sorted的例子，他的第二个参数是一个函数。

```python
>>> fruits = ['strawberry', 'fig', 'apple', 'cherry', 'raspberry', 'banana']
>>> sorted(fruits, key=len)       # 按照长度排序
['fig', 'apple', 'cherry', 'banana', 'raspberry', 'strawberry']
>>> def reverse(word):
...     return reverse[::-1]
... 
>>> sorted(fruits, key=reverse)   # 按照最后一个字母的字典顺序排序
['banana', 'apple', 'fig', 'raspberry', 'strawberry', 'cherry']
```

### map, filter和reduce的现代替换物

列表生成式和生成器表达式是这三个高阶函数的现代替换物。前者的语法更加简洁易读。

python3中，map和filter返回生成器，所以生成器表达式是它们的替换物。而python2中，这两个函数返回的是列表。

在python3中reduce不是内建函数，需要`from functools import reduce`来导入。如果要求和的话，直接用sum，而不再用reduce。除sum之外还有两个reducing功能的内建函数

- all(iterable)：所有元素为True，则返回True。`all([])`返回True
- any(iterable)：存在一个元素为True，则返回TRue。`any([])`返回True

## 5.2 匿名函数

与在js中不同，python中的匿名函数不常用。由于语法限制，lambda表达式仅仅是个表达式，中间不能有赋值语句，不能有逻辑控制语句。lambda关键字同def一样，创建了一个函数对象。

用lambda表达式重写前面水果按照最后一个字母的字典顺序排序

```python
>>> fruits = ['strawberry', 'fig', 'apple', 'cherry', 'raspberry', 'banana']
>>> sorted(fruits, key=lambda word: word[::-1])
['banana', 'apple', 'fig', 'raspberry', 'strawberry', 'cherry']
```

## 5.3 7种可调用对象

- 用户自定义函数  用def或lambda创建
- 内建函数  在CPython中，这些函数用c实现
- 内建方法  CPython中用c实现，如dict.get
- 方法  类中的方法
- 类  先调用`__new__`，在调用`__init__`，返回类的一个实例
- 类实例  如果一个类实现了`__call__`，则可以像函数一样被调用
- 生成器函数

判断一个对象是否可调用，用内建函数`callable()`

```python
>>> [callable(obj) for obj in (abs, str, 13)]
[True, True, False]
```

## 5.4 自定义可调用类型

python中函数是对象，那么反过来，对象也可以实现得像函数一样。下面的例子中`bingo()`是shortcut of `bingo.pick()`

```python
import random

class BingoCage:

    def __init__(self, items):
        self._items = list(items)   # copy of items in local
        random.shuffle(self._items)

    def pick(self):
        try:
            return self._items.pop()
        except IndexError:
            raise LookupError('pick from empty BingoCage')

    def __call__(self):
        return self.pick()

if __name__ == '__main__':
    bingo = BingoCage(range(3))
    print('bingo.pick() = {}'.format(bingo.pick()))
    print('bingo() = {}'.format(bingo()))
```

## 5.5 函数内窥

> `dir()`函数不带参数时，返回当前范围内的变量、方法和定义的类型列表；带参数时，返回参数的属性、方法列表

由于函数时对象，为了研究函数的个性，我们看看那些属性和方法是函数特有而对象没有的。

```python
>>> class C:pass
...
>>> obj = C()
>>> def func():pass
...
>>> sorted(set(dir(func)) - set(dir(obj)))
['__annotations__', '__call__', '__closure__', '__code__', '__defaults__', '__get__', '__globals__', '__kwdefaults__', '__name__', '__qualname__']
```

除了以上列出的属性方法之外，还有一个`__dict__`属性用来存放用户赋予函数的属性。

