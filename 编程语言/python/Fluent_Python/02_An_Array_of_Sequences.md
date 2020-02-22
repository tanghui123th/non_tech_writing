# 2 An Array of Sequences

## 2.1 Overview of Built-in Sequences

从存储形式上可以分成：

- `container sequences`：存放对不同对象的引用
  - 包括：list, tuple, collections.deque
- `flat sequences`：直接在自己的内存中存放同一类型的元素
  - 包括：str, bytes, bytearray, memoryview, array.array

从可变性来分：

- `mutable sequences`
  - 包括：list, bytearray, array.array, collections.deque, memoryview
- `immutable sequences`
  - 包括：tuple, str, bytes

## 2.2 List Comprehensions and Generator Expressions

略

## 2.3 Tuple Are Not Just Immutable Lists

Tuple其实有两种职能，而第二种往往被忽略：

- 作为不可变的list
- 作为无字段名的数据记录。这时各个字段数据的意义由其所在的位置决定

### Tuple Unpacking

```python
>>> t1 = (1,2,3)
>>> a, b, c = t1
>>> a
1
>>> b
2
>>> c
3
```

除了tuple，任意的iterable对象其实都可以unpacking

```python
>>> l1 = [1,2,3]
>>> x, y, z = l1
>>> x
1
>>> y
2
>>> z
3
```

如果不想一次unpack所有元素，可以用*把剩下的元素一起获取

```python
>>> a, *e = t1
>>> a
1
>>> e
[2, 3]
```

还有一个使用unpack的场景是在调用函数时，作为函数的参数

```python
>>> t1 = (20, 8)
>>> divmod(*t1)
(2, 4)
```

### Named Tuples

`namedtuple`对象可以给tuple的字段加上名字。`namedtuple`对象比一般类的实例占用的内存少，因为没有`__dict__`。

```python
>>> from collections import namedtuple
>>> City = namedtuple('City', 'name country population coordinates') # 第二个参数也可以是一个field name的list
>>> tokyo = City('Tokyo', 'JP', 36.9, (35, 139))
>>> tokyo.name
'Tokyo'
>>> tokyo.coordinates
(35, 139)
```

## 2.4 Slicing

