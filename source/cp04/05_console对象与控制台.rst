5 console对象与控制台
=====================

5.1 console对象与其静态方法
---------------------------

console对象是 JavaScript 的原生对象，它有点像 Unix
系统的标准输出stdout和标准错误stderr，可以输出各种信息到控制台，并且还提供了很多有用的辅助方法。

console.log()、console.info()、console.debug()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

console.log方法支持以下占位符，不同类型的数据必须使用对应的占位符。

-  %s 字符串
-  %d 整数
-  %i 整数
-  %f 浮点数
-  %o 对象的链接
-  %c CSS 格式字符串

%c接受css字符串

.. code:: js

   console.log(
     '%cThis text is styled!',
     'color: red; background: yellow; font-size: 24px;'
   )

console.info是console.log方法的别名，用法完全一样。只不过console.info方法会在输出信息的前面，加上一个蓝色图标。

console.debug方法与console.log方法类似，会在控制台输出调试信息。但是，默认情况下，console.debug输出的信息不会显示，只有在打开显示级别在verbose的情况下，才会显示。

console.warn()、console.error()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

log方法是写入标准输出（stdout），warn方法和error方法是写入标准错误（stderr）。

console.table()
~~~~~~~~~~~~~~~

.. code:: js

   var languages = [
     { name: "JavaScript", fileExtension: ".js" },
     { name: "TypeScript", fileExtension: ".ts" },
     { name: "CoffeeScript", fileExtension: ".coffee" }
   ];

   console.table(languages);

console.count()
~~~~~~~~~~~~~~~

count方法用于计数，输出它被调用了多少次。该方法可以接受一个字符串作为参数，作为标签，对执行次数进行分类。

.. code:: js

   function greet(user) {
     console.count(user);
     return "hi " + user;
   }

   greet('bob')
   // bob: 1
   // "hi bob"

   greet('alice')
   // alice: 1
   // "hi alice"

   greet('bob')
   // bob: 2
   // "hi bob"

console.dir()，console.dirxml()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

dir方法用来对一个对象进行检查（inspect），并以易于阅读和打印的格式显示。（其实现在chrome中log对对象的显示已经非常完善了）
