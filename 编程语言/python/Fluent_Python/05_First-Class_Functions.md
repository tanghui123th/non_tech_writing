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
