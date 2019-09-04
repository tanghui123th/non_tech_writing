3 JupyterNotebook,numpy,matplotlib
==================================

3.1 JupyterNotebook的一些魔法命令
---------------------------------

-  %run：执行python代码
-  %timeit：测试单个语句的时间（跑多次，取平均值）
-  %%timeit：测试整个块的时间（跑多次，取平均值）
-  %time：测试时间，但是只跑一次
-  %%time：测试时间，整个块，只跑一次
-  %lsmagic：查看所有魔法命令

3.2 numpy
---------

python
list的特点：list中可以存不同元素，因此操作的效率较低。而numpy.array中元素类型相同

创建numpy.array矩阵
~~~~~~~~~~~~~~~~~~~

zeros和ones
'''''''''''

zeros创建为0的数组，默认是float64类型。可以指定其他类型。

.. code:: python

   >>> np.zeros(10)
   array([0., 0., 0., 0., 0., 0., 0., 0., 0., 0.])
   >>> np.zeros(10).dtype
   dtype('float64')
   >>> np.zeros(10, dtype=int)
   array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

zeros第一个参数的shape，可以自己指定

.. code:: python

   >>> np.zeros((3,5))
   array([[0., 0., 0., 0., 0.],
          [0., 0., 0., 0., 0.],
          [0., 0., 0., 0., 0.]])

同zeros，还有ones，创建全是1的矩阵。

创建指定值的矩阵full
''''''''''''''''''''

.. code:: python

   >>> np.full(shape=(3,5), fill_value=666)
   array([[666, 666, 666, 666, 666],
          [666, 666, 666, 666, 666],
          [666, 666, 666, 666, 666]])

arange
''''''

类似原生的range，不同的是步长还可以是浮点数

.. code:: python

   >>> np.arange(0, 1, 0.2)
   array([0. , 0.2, 0.4, 0.6, 0.8])

linspace
''''''''

linspace的第三个参数意思是：在0和20之间等长的截出10个点。（这10个点包括0和20）

.. code:: python

   >>> np.linspace(0, 20, 10)
   array([ 0.        ,  2.22222222,  4.44444444,  6.66666667,  8.88888889,
          11.11111111, 13.33333333, 15.55555556, 17.77777778, 20.        ])

random
''''''

下面的意思是生成从[0,10)的随机数10个，第三个参数就是个数，默认是1。

.. code:: python

   >>> np.random.randint(0,10,10)
   array([2, 5, 4, 9, 3, 6, 8, 2, 7, 2])

为避免混淆，我们最好显示的指明第三个参数的名字size

.. code:: python

   >>> np.random.randint(4,8,size=10)
   array([6, 4, 7, 4, 6, 4, 7, 4, 5, 6])

size也可以是元素，来生成随机矩阵

.. code:: python

   >>> np.random.randint(4,8,size=(3,5))
   array([[4, 5, 6, 6, 6],
          [7, 4, 7, 5, 6],
          [7, 6, 5, 7, 4]])

为了让n次使用的随机矩阵元素一样，我们可以在每次生成的随机矩阵前指定random.seed()，这样n次的随机矩阵是一样的。

前面生成的是整数，random.random生成浮点数（均匀的分布在0-1之间）。要生成服从正太分布的浮点数矩阵，可以使用random.normal（前两个参数分别是均值和方差）。

numpy.array的基础操作
~~~~~~~~~~~~~~~~~~~~~

我们的x和X分别是，下面的操作在x和X上进行。

.. code:: python

   >>> x = np.arange(10)
   >>> x
   array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
   >>> X = np.arange(15).reshape(3,5)
   >>> X
   array([[ 0,  1,  2,  3,  4],
          [ 5,  6,  7,  8,  9],
          [10, 11, 12, 13, 14]])

ndim, shape和size
'''''''''''''''''

ndim返回维度

.. code:: python

   >>> x.ndim
   1
   >>> X.ndim
   2

shape返回形状

.. code:: python

   >>> x.shape
   (10,)
   >>> X.shape
   (3, 5)

size返回元素个数

.. code:: python

   >>> x.size
   10
   >>> X.size
   15

array数据访问
'''''''''''''

和原生list一样可以用[索引]。
