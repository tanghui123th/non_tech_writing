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

3.2 numpy基础
-------------

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

切片
''''

numpy中还可以对多维数组进行切片

.. code:: python

   >>> X[:2, :3]
   array([[0, 1, 2],
          [5, 6, 7]])

对多维数组进行翻转

.. code:: python

   >>> X[::-1, ::-1]
   array([[14, 13, 12, 11, 10],
          [ 9,  8,  7,  6,  5],
          [ 4,  3,  2,  1,  0]])

取列向量

.. code:: python

   >>> X[:,0]
   array([ 0,  5, 10])

注意numpy用切片取子矩阵，子矩阵是原矩阵的部分的引用。python中list切片的话，是创建了一个副本。如果要创建不相关的子矩阵，可以使用：

.. code:: python

   >>> subX = X[:2, :3].copy()
   >>> subX
   array([[0, 1, 2],
          [5, 6, 7]])

numpy.array合并
~~~~~~~~~~~~~~~

contatenate
'''''''''''

一维数组的合并

.. code:: python

   >>> x = np.array([1,2,3])
   >>> y = np.array([3,2,1])
   >>> x
   array([1, 2, 3])
   >>> y
   array([3, 2, 1])
   >>> np.concatenate([x,y])
   array([1, 2, 3, 3, 2, 1])

下面的例子可以理解为，原来的有两个样本，样本有三个特征。这时又来了两个样本，要把这两个样本合并成4个样本。

.. code:: python

   >>> A = np.array([[1,2,3],
   ...               [4,5,6]])
   >>> np.concatenate([A,A])
   array([[1, 2, 3],
          [4, 5, 6],
          [1, 2, 3],
          [4, 5, 6]])

下面的例子可以理解为，原来的有两个样本，样本有三个特征。这时这两个样本又多了三个特征，即两个样本六个特征。(axis默认是0，即沿着第一个维度拼接。这里设置成1，即沿着第二个维度，即列进行拼接)

.. code:: python

   >>> np.concatenate([A,A], axis=1)
   array([[1, 2, 3, 1, 2, 3],
          [4, 5, 6, 4, 5, 6]])

把两个样本三个特征的矩阵合并一个样本三个特征的矩阵。先用reshape把z从1维转换成1*3的二维矩阵，不然会报错

.. code:: python

   >>> z = np.array([666,666,666])
   >>> z
   array([666, 666, 666])
   >>> np.concatenate([A, z.reshape(1,-1)])
   array([[  1,   2,   3],
          [  4,   5,   6],
          [666, 666, 666]])

vstack, hstack智能识别维度
''''''''''''''''''''''''''

也可以使用vstack智能垂直叠加

.. code:: python

   >>> np.vstack([A,z])
   array([[  1,   2,   3],
          [  4,   5,   6],
          [666, 666, 666]])

当然还有hstack水平叠加

.. code:: python

   >>> B = np.full((2,2),100)
   >>> B
   array([[100, 100],
          [100, 100]])
   >>> A
   array([[1, 2, 3],
          [4, 5, 6]])
   >>> np.hstack([A, B])
   array([[  1,   2,   3, 100, 100],
          [  4,   5,   6, 100, 100]])

numpy.array分割
~~~~~~~~~~~~~~~

split
'''''

第二个参数是分割点，这里是3,7两个点，分成三段

.. code:: python

   >>> x = np.arange(10)
   >>> x
   array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
   >>> np.split(x, [3,7])
   [array([0, 1, 2]), array([3, 4, 5, 6]), array([7, 8, 9])]
   >>> 

分割矩阵

.. code:: python

   >>> A = np.arange(16).reshape((4,4))
   >>> A
   array([[ 0,  1,  2,  3],
          [ 4,  5,  6,  7],
          [ 8,  9, 10, 11],
          [12, 13, 14, 15]])
   >>> A1, A2 = np.split(A, [2])
   >>> A1
   array([[0, 1, 2, 3],
          [4, 5, 6, 7]])
   >>> A2
   array([[ 8,  9, 10, 11],
          [12, 13, 14, 15]])

传递axis指定维度

.. code:: python

   >>> A1, A2 = np.split(A, [2], axis=1)
   >>> A1
   array([[ 0,  1],
          [ 4,  5],
          [ 8,  9],
          [12, 13]])
   >>> A2
   array([[ 2,  3],
          [ 6,  7],
          [10, 11],
          [14, 15]])

vsplit, hsplit
''''''''''''''

类似拼接的vstack和hstack

.. code:: python

   >>> upper, lower = np.vsplit(A, [2])
   >>> upper
   array([[0, 1, 2, 3],
          [4, 5, 6, 7]])
   >>> lower
   array([[ 8,  9, 10, 11],
          [12, 13, 14, 15]])

.. code:: python

   >>> left, right = np.hsplit(A, [2])
   >>> left
   array([[ 0,  1],
          [ 4,  5],
          [ 8,  9],
          [12, 13]])
   >>> right
   array([[ 2,  3],
          [ 6,  7],
          [10, 11],
          [14, 15]])

使用分割的一个作用是把数据集的样本和标签进行分割

.. code:: python

   X, y = np.hsplit(data, [-1])

3.3 numpy矩阵运算
-----------------

矩阵和矩阵的运算
~~~~~~~~~~~~~~~~

numpy中的矩阵非常方便，所有运算符都是对应元素做相应的运算，且速度很快。如果要实现矩阵的乘法，可以用dot

向量和矩阵的运算
~~~~~~~~~~~~~~~~

numpy会自动把低维元素和高维元素中的每一个进行运算。矩阵和向量用dot乘，会自动判断向量用行向量还是列向量。

矩阵的逆
~~~~~~~~

.. code:: python

   >>> A = np.arange(4).reshape(2,2)
   >>> A
   array([[0, 1],
          [2, 3]])
   >>> np.linalg.inv(A)
   array([[-1.5,  0.5],
          [ 1. ,  0. ]])
