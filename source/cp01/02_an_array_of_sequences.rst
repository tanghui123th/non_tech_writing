2 An Array of Sequences
=======================

2.1 Overview of Build-In Sequences
----------------------------------

我们可以从两个角度对序列进行分类。一是从存储对象的机制来分类

-  容器类序列（container sequences）:list, tuple, collections.deque
-  直接序列（flat sequences）:str, bytes, bytearray, memoryview,
   array.array

``container sequences``\ 可以保存任意类型的对象，当然也包括自身类型的对象。\ ``flat sequences``\ 则只能保存基本类型，如整形、浮点型、字符型等。这也意味着\ ``container sequences``\ 可以嵌套，而\ ``flat sequences``\ 是无法嵌套的。

二是从可变性来分类：

-  可变序列（mutable sequences）: list, bytearray, array.array,
   collections.deque, memoryivew
-  不可变序列（immutable sequences）: tuple, str, bytes

2.2 List Comprehensions and Generator Expressions
-------------------------------------------------

listcomps是的目的是生成列表，如果要实现其他的功能，还是建议用for循环。

   [], {}, ()这三种括号中的换行是会被解释器忽略的

在python3中listcomps, genexps和set and dict
comprehensions的变量都有局部作用域，且不会和同名的外部变量混淆

.. code:: python

   >>> x = 'ABC'
   >>> dummy = [ord(x) for x in x]
   >>> x
   'ABC'
   >>> dummy
   [65, 66, 67]

Listcomps Versus map and filter
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

listcomps的所有功能都能由map/filter实现，但是map/filter可能还需要用到lambda函数。

   ord(‘￥’) = 65509

.. code:: python

   >>> symbols = '$￥'
   >>> beyond_ascii = [ord(s) for s in symbols if ord(s) > 127]
   >>> beyond_ascii
   [65509]
   >>> beyond_ascii = list(filter(lambda c:c > 127, map(ord, symbols)))
   >>> beyond_ascii
   [65509]

Cartesian Products
~~~~~~~~~~~~~~~~~~

我们用listcomps来生成两种颜色，三种大小的t-shirts，我们先用colors \*
sizes

.. code:: python

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

也可以反过来用sizes \* colors

.. code:: python

   >>> tshirts = [(color, size) for size in sizes
   ...                          for color in colors]
   >>> tshirts
   [('black', 'S'), ('white', 'S'), ('black', 'M'), ('white', 'M'), ('black', 'L'), ('white', 'L')]

Generator Expressions
~~~~~~~~~~~~~~~~~~~~~

listcomps只能生成列表，如果要生成其他的序列，则要使用genexps。

如果genexp是一个函数的唯一参数，则不需要再加一个圆括号。

.. code:: python

   >>> symbols = '$￥'
   >>> tuple(ord(symbol) for symbol in symbols)
   (36, 65509)
   >>> import array
   >>> array.array('I', (ord(symbol) for symbol in symbols))
   array('I', [36, 65509])

genexps的最大一个特点是它不是在内存中一次性生成所有元素，而是迭代一次生成一个

2.3 Tuples Are Not Just Immutable Lists
---------------------------------------

tuple有两个作用：一是作为无字段名的记录，二是作为不可变的列表。第一个作用往往被人忽视。

Tuples as Records
~~~~~~~~~~~~~~~~~

.. code:: python

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

Using \* to grab excess items
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

使用*可以把tuple多返回的元素捕获。

.. code:: python

   >>> a, b, *rest = range(5)
   >>> a, b, rest
   (0, 1, [2, 3, 4])
   >>> a, b, *rest = range(3)
   >>> a, b, rest
   (0, 1, [2])
   >>> a, b, *rest = range(2)
   >>> a, b, rest
   (0, 1, [])

如果左边的元素少于右边的元素，不管带*的元素有没有在最右边，都会捕捉到合适数量的元素

.. code:: python

   >>> a, *body, c, d = range(5)
   >>> a, body, c, d
   (0, [1, 2], 3, 4)

Nested Tuple Unpacking
~~~~~~~~~~~~~~~~~~~~~~

嵌套到tuple也支持unpacking

.. code:: python

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

上面的程序输出

.. code:: python

                   |   lat.    |   long.  
   Mexico City     |   19.4333 |  -99.1333
   New York-Newark |   40.8086 |  -74.0204
   Sao Paulo       |  -23.5478 |  -46.6358

Named Tuples
~~~~~~~~~~~~

tuple作为记录是没有字段名的，但是有时候我们又需要字段名，于是Named
Tuples就出现了。

.. code:: python

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

namedtuple()构造函数需要两个参数。第一个是类名，第二个是字段名的列表（使用\ ``iterable of strings``\ 或者用空格隔开的一个字符串也可以，就像我们上面使用的那样）

   namedtuple的实例占用的内存和tuple一样大。因为字符名是存储在类中的。

namedtuple的类继承自tuple，继承的属性中有3个最有用

-  类属性\ ``_fields``\ ：返回字段名
-  类方法\ ``_make()``\ ：由可迭代对象生成tuple
-  实例方法\ ``_asdict()``\ ：返回一个\ ``collections.OrderedDict``

.. code:: python

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

2.4 Slicing
-----------

所有序列类型的对象都能进行切片。

Why Slices and Range Exclude the Last Item
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

-  可以一眼看出切片或范围的长度，如\ ``range(3)`` ``my_list[:3]``
-  当起始和结束都给出时，长度就是\ ``stop - start``
-  同一个位置把序列分成两部分而不重叠，如\ ``my_list[:x]``\ 和\ ``my_list[x:]``

Slice Objects
~~~~~~~~~~~~~

我们在使用切片时，如果加了step，实际上是创建了一个切片对象\ ``slice(a, b, c)``\ 。表达式\ ``seq[start:stop:step]``\ 会被解释为\ ``seq.__getitem__(slice(start, stop, step))``

.. code:: python

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

输出

::

          $17.5 Pimoroni PiBrella                 
           $4.9 6mm Tactile Switch x20            
          $28.0 Panavise Jr. - PV-201             
          $34.9 PiTFT Mini Kit 320x240

Multidimensional Slicing and Ellipsis
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

切片还可以是多维的，形如\ ``a[m:n, k:l]``\ 的切片在NumPy中很有用。在切片时如果没有给出step，则是把两个元素当做tuple的对待的。如\ ``a[i, j]``\ 会被解释为\ ``a.__getitem__((i, j))``

省略号\ ``...``\ 实际上是\ ``ellipsis``\ 类的对象。在NumPy中对多维数组切片可以用省略号，比如x是一个四维数组，则\ ``x[i, ...]``\ 是\ ``x[i, :, :, :,]``\ 的缩写。

Assigning to Slices
~~~~~~~~~~~~~~~~~~~

可变序列就像其名字一样，当然是可以被改变值得。

.. code:: python

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

..

   当赋值语句的左值是切片时，右值必须是一个可迭代对象

2.5 Using + and \* with Sequence
--------------------------------

这两个运算符都不会改变原来的对象，而是创造一个新的对象。

Building Lists of Lists
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

   >>> board = [['_'] * 3 for i in range(3)]
   >>> board
   [['_', '_', '_'], ['_', '_', '_'], ['_', '_', '_']]
   >>> board[1][2] = 'X'
   >>> board
   [['_', '_', '_'], ['_', '_', 'X'], ['_', '_', '_']]

上面的代码用listcomps生成了嵌套的列表，实际上也相当于下面这段代码

.. code:: python

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

我们再来看看错误的例子。外层list的3个元素实际上引用到了同一个内层的list

.. code:: python

   >>> weird_board = [['_'] * 3] * 3
   >>> weird_board
   [['_', '_', '_'], ['_', '_', '_'], ['_', '_', '_']]
   >>> weird_board[1][2] = '0'
   >>> weird_board
   [['_', '_', '0'], ['_', '_', '0'], ['_', '_', '0']]

上面的代码相当于下面这代代码，每个元素指向的是同一个row

.. code:: python

   >>> row = ['_'] * 3
   >>> board = []
   >>> for i in range(3):
   ...     board.append(row)
   ... 
   >>> board[1][2] = 'X'
   >>> board
   [['_', '_', 'X'], ['_', '_', 'X'], ['_', '_', 'X']]

2.6 Augmented Assignment with Sequences
---------------------------------------

对于+=，解释器实际上是调用了\ ``__iadd__``\ ，(in-place
addition)，如果没有实现\ ``__iadd__``\ ，则解释器会调用\ ``__add__``\ 。两者区别：

-  ``__iadd__``\ ：在原有对象上操作，不会创建新的对象
-  ``__add__``\ ：\ ``a = a + b``\ ，\ ``a + b``\ 会先被进行计算，生成一个新对象，然后让a指向新的对象

一般来说可变序列都实现了\ ``__iadd__``\ ，而不可变序列在+=运算时，调用的是\ ``__add__``\ 。

上面对+=的讨论同样适用于\ *=，*\ =是调用的\ ``__imul__``

.. code:: python

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

由上面的讨论可以得出结论：对不可变序列进行多次增强赋值是非常低效的。因为每次都会创建一个新对象，并把原对象中的内从拷贝到新对象中。

   但是str类的对象是个例外，Cpython专门对str的+=做了优化。

A += Assignment Puzzler
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

   >>> t = (1, 2, [30, 40])
   >>> t[2] += [50, 60]
   Traceback (most recent call last):
     File "<stdin>", line 1, in <module>
   TypeError: 'tuple' object does not support item assignment
   >>> t
   (1, 2, [30, 40, 50, 60])

抛出了异常，但是同时完成了计算。分析一下\ ``s[a] += b``\ 的字节码

.. code:: python

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

建议：

-  不要把可变对象放到tuple中
-  分析字节码可以解释很多底层的问题

2.7 list.sort and the sorted Built-In Function
----------------------------------------------

python中程序API设计的一个原则：如果函数或方法是改变对象本身（in-place），则返回None。

-  list.sort：in-place排序，返回None
-  内建的sort：返回一个排好序的list对象。参数可以是不可变序列，或者生成器。

以上两个函数都接受两个参数：

-  reverse：默认是False
-  key：接受一个参数的函数。待排对象的每个元素都会作用于这个函数。

.. code:: python

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

Python中用的排序算法是Timsort，是稳定的。

2.8 Managing Ordered Sequences with biset
-----------------------------------------

Searching with bisect
~~~~~~~~~~~~~~~~~~~~~

``bisect(haystack, needle)``\ ，haystack必须是一个排好序的序列，函数返回needle在haystack的索引。

-  bisect：本身是bisect_right的化名。如果遇到相同的字符，返回右边的索引
-  bisect_left：如果遇到相同的字符，则返回当前（即插入后左边）的索引

.. code:: python

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

相同按右边输出：

.. code:: python

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

相同按左边输出：

.. code:: python

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

再来看一个把成绩分数转换为对应等级的例子：

.. code:: python

   >>> def grade(score, breakpoints=[60,70,80,90], grades='FDCBA'):
   ...     i = bisect.bisect(breakpoints, score)
   ...     return grades[i]
   ... 
   >>> [grade(score) for score in [33,99,77,70,89,90,100]]
   ['F', 'A', 'C', 'C', 'B', 'A', 'A']

Inserting with bisect.insort
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

同\ ``bisect``\ ，\ ``insort``\ 也有一个\ ``insort_left``\ 使用\ ``bisect_left``\ 来找到相同情况下左边的索引插入。

.. code:: python

   import bisect
   import random

   SIZE = 7

   random.seed(7)

   my_list = []
   for i in range(SIZE):
       new_item = random.randrange(SIZE*2)
       bisect.insort(my_list, new_item)
       print('%2d ->' % new_item, my_list)

.. code:: python

    5 -> [5]
    2 -> [2, 5]
    6 -> [2, 5, 6]
   10 -> [2, 5, 6, 10]
    0 -> [0, 2, 5, 6, 10]
    1 -> [0, 1, 2, 5, 6, 10]
   13 -> [0, 1, 2, 5, 6, 10, 13]

2.9 When a List is Not the Answer
---------------------------------

下面介绍几种在特定场合可以替代列表的可变序列

-  array
-  memorview
-  numpy中的数组
-  几种queue

..

   如果需要大量的包含检查（判断元素是否在其中），可以用set。set包含检查速度很快。但是set不是序列。

Arrays
~~~~~~

array存储的是按c语言类型的字节，而list存储的是对象。

.. code:: python

   >>> from array import array
   >>> from random import random
   >>> floats = array('d', (random() for i in range(10**7))) # a million floats, 这里array第二个参数是个生成器
   >>> floats[-1]
   0.4139464046747857
   >>> with open('floats.bin', 'wb') as fp:
   ...     floats.tofile(fp)
   ... 
   >>> floats2 = array('d')
   >>> with open('floats.bin', 'rb') as fp:
   ...     floats2.fromfile(fp, 10**7)
   ... 
   >>> floats2[-1]
   0.4139464046747857
   >>> floats == floats2
   True

``array.tofile``\ 和\ ``array.fromfile``\ 可以非常快速方便的操作。

Memory Views
~~~~~~~~~~~~

mevorview对象是一个共享内存的序列

.. code:: python

   >>> numbers = array.array('h', [-2, -1, 0, 1, 2])   # 'h' short int 类型
   >>> memv = memoryview(numbers)
   >>> len(memv)
   5
   >>> memv[0]
   -2
   >>> memv_oct = memv.cast('B')           # 'B' unsigned char 类型
   >>> memv_oct.tolist()
   [254, 255, 255, 255, 0, 0, 1, 0, 2, 0]  # 注意字节顺序在每一个元素中是反着的
   >>> memv_oct[5] = 4
   >>> numbers
   array('h', [-2, -1, 1024, 1, 2])        # 4的二进制位是100，这时第三个元素的二进制形式是 00000000 00000100 而字节顺序是反的，所以就是2^10即1024

NumPy and SciPy
~~~~~~~~~~~~~~~

NumPy可以方便的操作多维数组和矩阵。SciPy是基于NumPy的一个库，可以很方便的进行线性代数和统计学方面的计算。

.. code:: python

   >>> import numpy
   >>> a = numpy.arange(12)
   >>> a
   array([ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11])
   >>> type(a)
   <class 'numpy.ndarray'>
   >>> a.shape
   (12,)
   >>> a.shape = 3,4
   >>> a
   array([[ 0,  1,  2,  3],
          [ 4,  5,  6,  7],
          [ 8,  9, 10, 11]])
   >>> a[2]
   array([ 8,  9, 10, 11])
   >>> a[2,1]
   9
   >>> a[:, 1]
   array([1, 5, 9])
   >>> a.transpose()
   array([[ 0,  4,  8],
          [ 1,  5,  9],
          [ 2,  6, 10],
          [ 3,  7, 11]])

Deques and Other Queues
~~~~~~~~~~~~~~~~~~~~~~~

list虽然在右边插入和删除很快，但是要在左边插入删除的话，要移动整个序列。deque则可以很快地操作两端的元素。deque设置了最大长度的话，超过长度的插入元素会把元素从另一端挤出去。

::

   >>> from collections import deque
   >>> dq = deque(range(10), maxlen=10)
   >>> dq
   deque([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], maxlen=10)
   >>> dq.rotate(3)
   >>> dq
   deque([7, 8, 9, 0, 1, 2, 3, 4, 5, 6], maxlen=10)
   >>> dq.rotate(-4)
   >>> dq
   deque([1, 2, 3, 4, 5, 6, 7, 8, 9, 0], maxlen=10)
   >>> dq.appendleft(-1)
   >>> dq
   deque([-1, 1, 2, 3, 4, 5, 6, 7, 8, 9], maxlen=10)
   >>> dq.extend([11,22,33])
   >>> dq
   deque([3, 4, 5, 6, 7, 8, 9, 11, 22, 33], maxlen=10)
   >>> dq.extendleft([10,20,30,40])
   >>> dq
   deque([40, 30, 20, 10, 3, 4, 5, 6, 7, 8], maxlen=10)

deque的操作是线程安全的，在多线程环境下可以放心使用。

其他几个实现队列的标准库：

-  queue：有类Queue、LiFoQueue、PriorityQueue。这些是为多线程编程设计的。这些类都是线程安全的。且如果已满的话，新插入的元素会等待，直到其他线程把queue的空间又空出来。
-  multiprocessing：有类Queue，为多进程编程而设计。
-  asynio：为异步编程设计。
-  heapq：用来进行堆的操作。

2.10 Mixed Bag Lists
--------------------

list和tuple的微妙区别
~~~~~~~~~~~~~~~~~~~~~

list中是可以存放不同类型的元素的。但是最好不要这样用，因为我们往往会对list中的每个元素进行同样的操作，如果类型不同，则有些支持而有些不支持。而tuple支持存放不同类型的元素看起来就很自然了。

.. code:: python

   >>> l = [28, 14, '28', 5, '9', '1', 0, 6, '23', 19]
   >>> sorted(l)
   Traceback (most recent call last):
     File "<stdin>", line 1, in <module>
   TypeError: '<' not supported between instances of 'str' and 'int'

Key Is Brilliant
~~~~~~~~~~~~~~~~

list.sort, sorted, max,
min都支持一个key参数，这个参数是个接受一个函数的函数，每个元素都会作用于这个函数。不同于其他语言用两个参数的比较函数来定义比较规则，python的key参数既简洁又高效。因为key这个函数只会被调用一次，而其他语言的比较函数在每次比较时都会被调用。

.. code:: python

   >>> l = [28, 14, '28', 5, '9', '1', 0, 6, '23', 19]
   >>> sorted(l, key=int)
   [0, '1', 5, 6, '9', 14, 19, '23', 28, '28']
   >>> sorted(l, key=str)
   [0, '1', 14, 19, '23', 28, '28', 5, 6, '9']

Timsort
~~~~~~~

这时一种自适应的排序算法，会根据待排元素的有序性自动选择插入排序或归并排序。
