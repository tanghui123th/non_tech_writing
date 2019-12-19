# es6 let const

## 2.1 let命令

### 基本用法

var声明的变量是全局变量，在代码块外还会被引用到。下面a中的每个元素都会输出10。let声明的变量的作用域只在代码块内。

```js
var a = [];
for (var i = 0; i < 10; i++) {
  a[i] = function () {
    console.log(i);
  };
}
ƒ () {
    console.log(i);
  }
a[1]();  // 10
a[0]();  // 10
a[9]();  // 10
```

### 不存在变量提升

var命令会发生“变量提升”现象，即变量可以在声明之前使用，值为undefined。let命令所声明的变量一定要在声明后使用，否则报错。

如果有var和let声明的同名变量，则在代码块内，以let为准。如下。这称为“暂时性死区”（temporal dead zone，简称 TDZ）。在代码块内，使用let命令声明变量之前，该变量都是不可用的。

```js
var tmp = 123;

if (true) {
  tmp = 'abc'; // ReferenceError
  let tmp;
}
```
