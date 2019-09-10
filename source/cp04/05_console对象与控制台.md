# 5 console对象与控制台

## 5.1 console对象与其静态方法

console对象是 JavaScript 的原生对象，它有点像 Unix 系统的标准输出stdout和标准错误stderr，可以输出各种信息到控制台，并且还提供了很多有用的辅助方法。

### console.log()、console.info()、console.debug()

console.log方法支持以下占位符，不同类型的数据必须使用对应的占位符。

- %s 字符串
- %d 整数
- %i 整数
- %f 浮点数
- %o 对象的链接
- %c CSS 格式字符串

%c接受css字符串

```js
console.log(
  '%cThis text is styled!',
  'color: red; background: yellow; font-size: 24px;'
)
```

console.info是console.log方法的别名，用法完全一样。只不过console.info方法会在输出信息的前面，加上一个蓝色图标。

console.debug方法与console.log方法类似，会在控制台输出调试信息。但是，默认情况下，console.debug输出的信息不会显示，只有在打开显示级别在verbose的情况下，才会显示。

### console.warn()、console.error()

log方法是写入标准输出（stdout），warn方法和error方法是写入标准错误（stderr）。

### console.table()

table会把数据按照表格的样式输出。

```js
var languages = [
  { name: "JavaScript", fileExtension: ".js" },
  { name: "TypeScript", fileExtension: ".ts" },
  { name: "CoffeeScript", fileExtension: ".coffee" }
];

console.table(languages);
```

### console.count()

count方法用于计数，输出它被调用了多少次。该方法可以接受一个字符串作为参数，作为标签，对执行次数进行分类。

```js
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
```

### console.dir()，console.dirxml()

dir方法用来对一个对象进行检查（inspect），并以易于阅读和打印的格式显示。（其实现在chrome中log对对象的显示已经非常完善了）

### console.assert()

它接受两个参数，第一个参数是表达式，第二个参数是字符串。只有当第一个参数为false，才会提示有错误，在控制台输出第二个参数，否则不会有任何结果。

```js
console.assert(false, '判断条件不成立')
// Assertion failed: 判断条件不成立
```

### console.time(), console.timeEnd()

time方法表示计时开始，timeEnd方法表示计时结束。它们的参数是计时器的名称。调用timeEnd方法之后，控制台会显示“计时器名称: 所耗费的时间”。

```js
console.time('Array initialize');

var array= new Array(1000000);
for (var i = array.length - 1; i >= 0; i--) {
  array[i] = new Object();
};

console.timeEnd('Array initialize');
// Array initialize: 1914.481ms
```

### console.group(), console.groupEnd()

console.group和console.groupEnd这两个方法用于将显示的信息分组。它只在输出大量信息时有用，分在一组的信息，可以用鼠标折叠/展开。

```js
console.group('一级分组');
console.log('一级分组的内容');

console.group('二级分组');
console.log('二级分组的内容');

console.groupEnd(); // 二级分组结束
console.groupEnd(); // 一级分组结束
一级分组
  一级分组的内容
  二级分组
    二级分组的内容
```

console.groupCollapsed类似console.group，不过默认是收起的。

```js
console.groupCollapsed('Fetching Data');

console.log('Request Sent');
console.error('Error: Server not responding (500)');

console.groupEnd();
```

### console.trace()

显示当前执行的代码在堆栈中的调用路径。

### console.clear()

console.clear方法用于清除当前控制台的所有输出，将光标回置到第一行。

## 5.2 控制台命令行 API

浏览器控制台中，除了使用console对象，还可以使用一些控制台自带的命令行方法。

