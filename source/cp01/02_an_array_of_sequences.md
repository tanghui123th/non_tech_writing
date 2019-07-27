# 2 An Array of Sequences

## 2.1 Overview of Build-In Sequences

我们可以从两个角度对序列进行分类。一是从存储对象的机制来分类

- 容器类序列（container sequences）:list, tuple, collections.deque
- 直接序列（flat sequences）:str, bytes, bytearray, memoryview, array.array

容器类序列保存的是对象的引用，而直接序列保存的是对象本身

二是从可变性来分类：

- 可变序列（mutable sequences）: list, bytearray, array.array, collections.deque, memoryivew
- 不可变序列（immutable sequences）: tuple, str, bytes

## 2.2 List Comprehensions and Generator Expressions

listcomps是的目的是生成列表的，如果要实现其他的功能，还是建议用for循环。

> [], {}, ()这三种括号中的换行是会被解释器忽略的

在python3中listcomps, genexps和set and dict comprehensions的变量都有局部作用域，且不会和同名的外部变量混淆

```python
>>> x = 'ABC'
>>> dummy = [ord(x) for x in x]
>>> x
'ABC'
>>> dummy
[65, 66, 67]
```

### Listcomps Versus map and filter

listcomps的所有功能都能由map/filter实现，但是map/filter可能还需要用到lambda函数。

> ord('￥') = 65509

```python
>>> symbols = '$￥'
>>> beyond_ascii = [ord(s) for s in symbols if ord(s) > 127]
>>> beyond_ascii
[65509]
>>> beyond_ascii = list(filter(lambda c:c > 127, map(ord, symbols)))
>>> beyond_ascii
[65509]
```

### Cartesian Products

我们用listcomps来生成两种颜色，三种大小的t-shirts，我们先用colors * sizes

```python
>>> colors = ['black', 'white']
>>> sizes = ['S', 'M', 'L']
>>> tshirts = [(color, size) for color in colors
...                          for size in sizes]
>>> tshirts
[('black', 'S'), ('black', 'M'), ('black', 'L'), ('white', 'S'), ('white', 'M'), ('white', 'L')]
>>> for color in colors:
...     for size in sizes:
...         print((color, size))
... 
('black', 'S')
('black', 'M')
('black', 'L')
('white', 'S')
('white', 'M')
('white', 'L')
```

也可以反过来用sizes * colors

```python
>>> tshirts = [(color, size) for size in sizes
...                          for color in colors]
>>> tshirts
[('black', 'S'), ('white', 'S'), ('black', 'M'), ('white', 'M'), ('black', 'L'), ('white', 'L')]
```

### Generator Expressions

listcomps只能生成列表，如果要生成其他的序列，则要使用genexps。

如果genexp是一个函数的唯一参数，则不需要再加一个圆括号。

```python
>>> symbols = '$￥'
>>> tuple(ord(symbol) for symbol in symbols)
(36, 65509)
>>> import array
>>> array.array('I', (ord(symbol) for symbol in symbols))
array('I', [36, 65509])
```

genexps的最大一个特点是它不是在内存中一次性生成所有元素，而是迭代一次生成一个

## 2.3 Tuples Are Not Just Immutable Lists

tuple有两个作用：一是作为无字段名的记录，二是作为不可变的列表。第一个作用往往被人忽视。

### Tuples as Records

```python
>>> lax_coordinates = (33.9425, -118.4080)
>>> city, year, pop, chg, area = ('Tokyo', 2003, 32450, 0.66, 8041)
>>> traveler_ids = [('USA', '31195855'),('BRA', 'CE342567'),('ESP', 'XDA205856')]
>>> for passport in sorted(traveler_ids):
...     print('%s/%s' % passport)
... 
BRA/CE342567
ESP/XDA205856
USA/31195855
>>> for country, _ in traveler_ids:
...     print(country)
... 
USA
BRA
ESP
```

### Using * to grab excess items

使用*可以把tuple多返回的元素捕获。

```python
>>> a, b, *rest = range(5)
>>> a, b, rest
(0, 1, [2, 3, 4])
>>> a, b, *rest = range(3)
>>> a, b, rest
(0, 1, [2])
>>> a, b, *rest = range(2)
>>> a, b, rest
(0, 1, [])
```

如果左边的元素少于右边的元素，不管带*的元素有没有在最右边，都会捕捉到合适数量的元素

```python
>>> a, *body, c, d = range(5)
>>> a, body, c, d
(0, [1, 2], 3, 4)
```

### Nested Tuple Unpacking

嵌套到tuple也支持unpacking

```python
metro_areas = [
    ('Tokyo', 'JP', 36.933, (35.689722, 139.691667)),
    ('Delhi', 'IN', 21.935, (23.613889, 77.208889)),
    ('Mexico City', 'MX', 20.142, (19.433333, -99.133333)),
    ('New York-Newark', 'US', 20.104, (40.808611, -74.020386)),
    ('Sao Paulo', 'BR', 19.649, (-23.547778, -46.635833)),
]

# ^表示居中
print('{:15} | {:^9} | {:^9}'.format('', 'lat.', 'long.'))
fmt = '{:15} | {:9.4f} | {:9.4f}'
for name, cc, pop, (latitude, longitude) in metro_areas:
    if longitude <= 0:
        print(fmt.format(name, latitude, longitude))
```

上面的程序输出

```python
                |   lat.    |   long.  
Mexico City     |   19.4333 |  -99.1333
New York-Newark |   40.8086 |  -74.0204
Sao Paulo       |  -23.5478 |  -46.6358
```

### Named Tuples

tuple作为记录是没有字段名的，但是有时候我们又需要字段名，于是Named Tuples就出现了。

```python
>>> from collections import namedtuple
>>> City = namedtuple('City', 'name country population coordinates')
>>> tokyo = City('Tokyo', 'JP', 36.933, (35.689722, 139.691667))
>>> tokyo
City(name='Tokyo', country='JP', population=36.933, coordinates=(35.689722, 139.691667))
>>> tokyo.population
36.933
>>> tokyo.coordinates
(35.689722, 139.691667)
>>> tokyo[1]
'JP'
```

namedtuple()构造函数需要两个参数。第一个是类名，第二个是字段名的列表（使用`iterable of strings`或者用空格隔开的一个字符串也可以，就像我们上面使用的那样）

> namedtuple的实例占用的内存和tuple一样大。因为字符名是存储在类中的。

namedtuple的类继承自tuple，继承的属性中有3个最有用

- 类属性`_fields`：返回字段名
- 类方法`_make()`：由可迭代对象生成tuple
- 实例方法`_asdict()`：返回一个`collections.OrderedDict`

```python
>>> City._fields
('name', 'country', 'population', 'coordinates')
>>> LatLong = namedtuple('LatLong', 'lat long')
>>> delhi_data = ('Delhi NCR', 'IN', 21.935, LatLong(28.613889, 77.208889))
>>> delhi = City._make(delhi_data)
>>> delhi._asdict()
OrderedDict([('name', 'Delhi NCR'), ('country', 'IN'), ('population', 21.935), ('coordinates', LatLong(lat=28.613889, long=77.208889))])
>>> for key, value in delhi._asdict().items():
...     print(key + ':', value)
... 
name: Delhi NCR
country: IN
population: 21.935
coordinates: LatLong(lat=28.613889, long=77.208889)
```

## 2.4 Slicing

所有序列类型的对象都能进行切片。

### Why Slices and Range Exclude the Last Item

- 可以一眼看出切片或范围的长度，如`range(3)` `my_list[:3]`
- 当起始和结束都给出时，长度就是`stop - start`
- 同一个位置把序列分成两部分而不重叠，如`my_list[:x]`和`my_list[x:]`

### Slice Objects

我们在使用切片时，如果加了step，实际上是创建了一个切片对象`slice(a, b, c)`。表达式`seq[start:stop:step]`会被解释为`seq.__getitem__(slice(start, stop, step))`

```python
invoice = """
0.....6.................................40...........52...55........
1909  Pimoroni PiBrella                        $17.50    3    $52.50
1489  6mm Tactile Switch x20                    $4.95    2     $9.90
1510  Panavise Jr. - PV-201                    $28.00    1    $28.00
1601  PiTFT Mini Kit 320x240                   $34.95    1    $34.95
"""

SYU = slice(0, 6)
DESCRIPTION = slice(6, 40)
UNIT_PRICE = slice(40, 52)
QUANTITY = slice(52, 55)
ITEM_TOTAL = slice(55, None)

line_items = invoice.split('\n')[2:]
for item in line_items:
    print(item[UNIT_PRICE], item[DESCRIPTION])
```

输出

```
       $17.5 Pimoroni PiBrella                 
        $4.9 6mm Tactile Switch x20            
       $28.0 Panavise Jr. - PV-201             
       $34.9 PiTFT Mini Kit 320x240
```

### Multidimensional Slicing and Ellipsis

切片还可以是多维的，形如`a[m:n, k:l]`的切片在NumPy中很有用。在切片时如果没有给出step，则是把两个元素当做tuple的对待的。如`a[i, j]`会被解释为`a.\_\_getitem__((i, j))`

省略号`...`实际上是`ellipsis`类的对象。在NumPy中对多维数组切片可以用省略号，比如x是一个四维数组，则`x[i, ...]`是`x[i, :, :, :,]`的缩写。

### Assigning to Slices

可变序列就像其名字一样，当然是可以被改变值得。

```python
>>> l = list(range(10))
>>> l
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
>>> l[2:5] = [20,30]
>>> l
[0, 1, 20, 30, 5, 6, 7, 8, 9]
>>> del l[5:7]
>>> l
[0, 1, 20, 30, 5, 8, 9]
>>> l[3::2] = [11, 22]
>>> l
[0, 1, 20, 11, 5, 22, 9]
>>> l[2:5] = 100
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: can only assign an iterable
>>> l[2:5] = [100]
>>> l
[0, 1, 100, 22, 9]
```

> 当赋值语句的左值是切片时，右值必须是一个可迭代对象

## 2.5 Using + and * with Sequence

这两个运算符都不会改变原来的对象，而是创造一个新的对象。

### Building Lists of Lists

```python
>>> board = [['_'] * 3 for i in range(3)]
>>> board
[['_', '_', '_'], ['_', '_', '_'], ['_', '_', '_']]
>>> board[1][2] = 'X'
>>> board
[['_', '_', '_'], ['_', '_', 'X'], ['_', '_', '_']]
```

上面的代码用listcomps生成了嵌套的列表，实际上也相当于下面这段代码

```python
>>> board = []
>>> for i in range(3):
...     row = ['_'] * 3
...     board.append(row)
... 
>>> board
[['_', '_', '_'], ['_', '_', '_'], ['_', '_', '_']]
>>> board[2][0] = 'X'
>>> board
[['_', '_', '_'], ['_', '_', '_'], ['X', '_', '_']]
```

我们再来看看错误的例子。外层list的3个元素实际上引用到了同一个内层的list

```python
>>> weird_board = [['_'] * 3] * 3
>>> weird_board
[['_', '_', '_'], ['_', '_', '_'], ['_', '_', '_']]
>>> weird_board[1][2] = '0'
>>> weird_board
[['_', '_', '0'], ['_', '_', '0'], ['_', '_', '0']]
```

上面的代码相当于下面这代代码，每个元素指向的是同一个row

```python
>>> row = ['_'] * 3
>>> board = []
>>> for i in range(3):
...     board.append(row)
... 
>>> board[1][2] = 'X'
>>> board
[['_', '_', 'X'], ['_', '_', 'X'], ['_', '_', 'X']]
```

## 2.6 Augmented Assignment with Sequences

对于+=，解释器实际上是调用了`\_\_iadd__`，(in-place addition)，如果没有实现`\_\_iadd__`，则解释器会调用`\_\_add__`。两者区别：

- `\_\_iadd__`：在原有对象上操作，不会创建新的对象
- `\_\_add__`：`a = a + b`，`a + b`会先被进行计算，生成一个新对象，然后让a指向新的对象

一般来说可变序列都实现了`\_\_iadd__`，而不可变序列在+=运算时，调用的是`\_\_add__`。

上面对+=的讨论同样适用于*=，*=是调用的`\_\_imul__`

```python
>>> l = [1,2,3]
>>> id(l)
4372405576
>>> l *= 2
>>> id(l)
4372405576
>>> t = (1,2,3)
>>> id(t)
4372272616
>>> t *= 2
>>> id(t)
4372007432
```

由上面的讨论可以得出结论：对不可变序列进行多次增强赋值是非常低效的。因为每次都会创建一个新对象，并把原对象中的内从拷贝到新对象中。

> 但是str类的对象是个例外，Cpython专门对str的+=做了优化。

### A += Assignment Puzzler


```python
>>> t = (1, 2, [30, 40])
>>> t[2] += [50, 60]
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: 'tuple' object does not support item assignment
>>> t
(1, 2, [30, 40, 50, 60])
```

抛出了异常，但是同时完成了计算。分析一下`s[a] += b`的字节码

```python
>>> dis.dis('s[a] += b')
  1           0 LOAD_NAME                0 (s)
              2 LOAD_NAME                1 (a)
              4 DUP_TOP_TWO
              6 BINARY_SUBSCR                   # 放s[a]放在TOS（栈顶 top of stack）
              8 LOAD_NAME                2 (b)
             10 INPLACE_ADD                     # 计算TOS += s[a]
             12 ROT_THREE
             14 STORE_SUBSCR                    # s[a] = TOS，异常就是在这里出的
             16 LOAD_CONST               0 (None)
             18 RETURN_VALUE
```

建议：

- 不要把可变对象放到tuple中
- 分析字节码可以解释很多底层的问题

## 2.7 list.sort and the sorted Built-In Function

python中程序API设计的一个原则：如果函数或方法是改变对象本身（in-place），则返回None。

- list.sort：in-place排序，返回None
- 内建的sort：返回一个排好序的list对象。参数可以使不可变序列，或者生成器。

以上两个函数都接受两个参数：

- reverse：默认是False
- key：接受一个参数的函数。待排对象的每个元素都会作用于这个函数。

```python
>>> fruits = ['grape', 'raspberry', 'apple', 'banana']
>>> sorted(fruits)
['apple', 'banana', 'grape', 'raspberry']
>>> fruits
['grape', 'raspberry', 'apple', 'banana']
>>> sorted(fruits, reverse=True)
['raspberry', 'grape', 'banana', 'apple']
>>> sorted(fruits, key=len)
['grape', 'apple', 'banana', 'raspberry']      # 按照长度排序
>>> sorted(fruits, key=len, reverse=True)
['raspberry', 'banana', 'grape', 'apple']      # 按照长度逆序排序，注意grape还是在apple前面，排序算法是稳定的
>>> fruits
['grape', 'raspberry', 'apple', 'banana']
>>> fruits.sort()
>>> fruits
['apple', 'banana', 'grape', 'raspberry']
```

Python中用的排序算法是Timsort，是稳定的。

## 2.8 Managing Ordered Sequences with biset

### Searching with bisect

`bisect(haystack, needle)`，haystack必须是一个排好序的序列，函数返回needle在haystack的索引。

- bisect：本身是bisect_right的化名。如果遇到相同的字符，返回右边的索引
- bisect_left：如果遇到相同的字符，则返回当前（即插入后左边）的索引

```python
import bisect
import sys

HAYSTACK = [1,4,5,6,8,12,15,20,21,23,23,26,29,30]
NEEDLES = [0,1,2,5,8,10,22,23,29,30,31]

ROW_FMT = '{0:2d} @ {1:2d}    {2}{0:<2d}'   # 最后一个占位符把0位置上的参数在输出一次

def demo(bisect_fn):
    for needle in reversed(NEEDLES):
        position = bisect_fn(HAYSTACK, needle)
        offset = position * '  |'
        print(ROW_FMT.format(needle, position, offset))

if __name__ == '__main__':
    if sys.argv[-1] == 'left':
        bisect_fn = bisect.bisect_left
    else:
        bisect_fn = bisect.bisect_right
    
    print('DEMO:', bisect_fn.__name__)
    print('haystack ->', ' '.join('%2d' % n for n in HAYSTACK))
    demo(bisect_fn)
```

相同按右边输出：

```python
(play_ground) leo@leodeMacBook-Pro play_ground $ python t.py
DEMO: bisect_right
haystack ->  1  4  5  6  8 12 15 20 21 23 23 26 29 30
31 @ 14      |  |  |  |  |  |  |  |  |  |  |  |  |  |31
30 @ 14      |  |  |  |  |  |  |  |  |  |  |  |  |  |30
29 @ 13      |  |  |  |  |  |  |  |  |  |  |  |  |29
23 @ 11      |  |  |  |  |  |  |  |  |  |  |23
22 @  9      |  |  |  |  |  |  |  |  |22
10 @  5      |  |  |  |  |10
 8 @  5      |  |  |  |  |8 
 5 @  3      |  |  |5 
 2 @  1      |2 
 1 @  1      |1 
 0 @  0    0 
```

相同按左边输出：

```python
(play_ground) leo@leodeMacBook-Pro play_ground $ python t.py left
DEMO: bisect_left
haystack ->  1  4  5  6  8 12 15 20 21 23 23 26 29 30
31 @ 14      |  |  |  |  |  |  |  |  |  |  |  |  |  |31
30 @ 13      |  |  |  |  |  |  |  |  |  |  |  |  |30
29 @ 12      |  |  |  |  |  |  |  |  |  |  |  |29
23 @  9      |  |  |  |  |  |  |  |  |23
22 @  9      |  |  |  |  |  |  |  |  |22
10 @  5      |  |  |  |  |10
 8 @  4      |  |  |  |8 
 5 @  2      |  |5 
 2 @  1      |2 
 1 @  0    1 
 0 @  0    0 
```

再来看一个把成绩分数转换为对应等级的例子：


```python
>>> def grade(score, breakpoints=[60,70,80,90], grades='FDCBA'):
...     i = bisect.bisect(breakpoints, score)
...     return grades[i]
... 
>>> [grade(score) for score in [33,99,77,70,89,90,100]]
['F', 'A', 'C', 'C', 'B', 'A', 'A']
```