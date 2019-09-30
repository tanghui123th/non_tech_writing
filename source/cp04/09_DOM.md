# 9 DOM

## 9.1 概述

### DOM

DOM 是 JavaScript 操作网页的接口。它的作用是将网页转为一个 JavaScript 对象，从而可以用脚本进行各种操作（比如增删内容）。

浏览器会根据 DOM 模型，将结构化文档（比如 HTML 和 XML）解析成一系列的节点，再由这些节点组成一个树状结构（DOM Tree）。

DOM 只是一个接口规范，可以用各种语言实现。所以严格地说，DOM 不是 JavaScript 语法的一部分，但是 DOM 操作是 JavaScript 最常见的任务，离开了 DOM，JavaScript 就无法控制网页。另一方面，JavaScript 也是最常用于 DOM 操作的语言。

### 节点

文档的树形结构（DOM 树），就是由各种不同类型的节点组成。每个节点可以看作是文档树的一片叶子。

节点的类型有七种。

- Document：整个文档树的顶层节点
- DocumentType：doctype标签（比如`<!DOCTYPE html>`）
- DocumentFragment：文档的片段
- Element：网页的各种HTML标签（比如`<body>`、`<a>`等）
- Attribute：网页元素的属性（比如class="right"）
- Text：标签之间或标签包含的文本
- Comment：注释

浏览器提供一个原生的节点对象Node，上面这七种节点都继承了Node，因此具有一些共同的属性和方法。

### 节点树

浏览器原生提供顶层document节点，代表整个文档。文档的第一层有两个节点，第一个是文档类型节点（`<!doctype html>`），第二个是 HTML 网页的顶层容器标签`<html>`。后者构成了树结构的根节点（root node）

除了根节点，其他节点都有三种层级关系。

- 父节点关系（parentNode）：直接的那个上级节点
- 子节点关系（childNodes）：直接的下级节点
- 同级节点关系（sibling）：拥有同一个父节点的节点

DOM 提供操作接口，用来获取这三种关系的节点。比如，firstChild, lastChild, nextSibling, previousSibling等。

## 9.2 Node接口

所有 DOM 节点对象都继承了 Node 接口，拥有一些共同的属性和方法。这是 DOM 操作的基础。

### 属性

##### Node.prototype.nodeType

nodeType属性返回一个整数值，表示节点的类型。Node 对象定义了几个常量，对应这些类型值。

- 文档节点（document）：9，对应常量Node.DOCUMENT_NODE
- 元素节点（element）：1，对应常量Node.ELEMENT_NODE
- 属性节点（attr）：2，对应常量Node.ATTRIBUTE_NODE
- 文本节点（text）：3，对应常量Node.TEXT_NODE
- 文档片断节点（DocumentFragment）：11，对应常量Node.DOCUMENT_FRAGMENT_NODE
- 文档类型节点（DocumentType）：10，对应常量Node.DOCUMENT_TYPE_NODE
- 注释节点（Comment）：8，对应常量Node.COMMENT_NODE

##### Node.prototype.nodeName

返回节点名

##### Node.prototype.nodeValue

返回节点值。只有文本节点（text）、注释节点（comment）和属性节点（attr）有文本值，因此这三类节点的nodeValue可以返回结果，其他类型的节点一律返回null。

```js
// <div id="d1">hello world</div>
var div = document.getElementById('d1');
div.nodeValue // null
div.firstChild.nodeValue // "hello world"
```

##### Node.prototype.textContent

textContent属性返回当前节点和它的所有后代节点的文本内容。

```js
// HTML 代码为
// <div id="divA">This is <span>some</span> text</div>

document.getElementById('divA').textContent
// This is some text
```

该属性是可读写的，设置该属性的值，会用一个新的文本节点，替换所有原来的子节点。它还有一个好处，就是自动对 HTML 标签转义。

```js
document.getElementById('foo').textContent = '<p>GoodBye!</p>';
```

对于文本节点（text）、注释节点（comment）和属性节点（attr），textContent属性的值与nodeValue属性相同。文档节点（document）和文档类型节点（doctype）的textContent属性为null。如果要读取整个文档的内容，可以使用document.documentElement.textContent。

##### Node.prototype.baseURI

baseURI属性返回当前网页的绝对路径。只读。

```js
// 当前网页的网址为
// http://www.example.com/index.html
document.baseURI
// "http://www.example.com/index.html"
```

该属性的值一般由当前网址的 URL（即window.location属性）决定，但是可以使用 HTML 的<base>标签，改变该属性的值。设置了以后，baseURI属性就返回<base>标签设置的值。

```html
<base href="http://www.example.com/page.html">
```

##### Node.prototype.ownerDocument

返回当前节点所在的顶层文档对象，即document对象。

##### Node.prototype.nextSibling

注意，该属性还包括文本节点和注释节点（`<!-- comment -->`）。因此如果当前节点后面有空格，该属性会返回一个文本节点，内容为空格。

##### Node.prototype.parentNode

parentNode属性返回当前节点的父节点。对于一个节点来说，它的父节点只可能是三种类型：元素节点（element）、文档节点（document）和文档片段节点（documentfragment）。文档节点（document）和文档片段节点（documentfragment）的父节点都是null。

##### Node.prototype.parentElement

parentElement属性返回当前节点的父元素节点。如果当前节点没有父节点，或者父节点类型不是元素节点，则返回null。由于父节点只可能是三种类型：元素节点、文档节点（document）和文档片段节点（documentfragment）。parentElement属性相当于把后两种父节点都排除了。

##### Node.prototype.childNodes

childNodes属性返回一个类似数组的对象（NodeList集合），成员包括当前节点的所有子节点。文档节点（document）就有两个子节点：文档类型节点（docType）和 HTML 根元素节点。



