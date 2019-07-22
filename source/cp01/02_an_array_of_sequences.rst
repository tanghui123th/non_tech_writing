2 An Array of Sequences
=======================

2.1 Overview of Build-In Sequences
----------------------------------

我们可以从两个角度对序列进行分类。一是从存储对象的机制来分类

-  容器类序列（container sequences）:list, tuple, collections.deque
-  直接序列（flat sequences）:str, bytes, bytearray, memoryview,
   array.array

容器类序列保存的是对象的引用，而直接序列保存的是对象本身

二是从可变性来分类：

-  可变序列（mutable sequences）: list, bytearray, array.array,
   collections.deque, memoryivew
-  不可变序列（immutable sequences）: tuple, str, bytes

2.2 List Comprehensions and Generator Expressions
-------------------------------------------------

listcomps是的目的是生成列表的，如果要实现其他的功能，还是建议用for循环。

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
   >>> 

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

我们在使用切片时，实际上是创建了一个切片对象\ ``slice(a, b, c)``\ 。表达式\ ``seq[start:stop:step]``\ 会被解释为\ ``seq.__getitem__(slice(start, stop, step))``

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
