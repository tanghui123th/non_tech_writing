# 1 html

## 1.1 简介

一个网页主要由三部分组成：

- 结构：HTML
- 表现：CSS
- 行为：JS

doctype：用来告诉浏览器使用的html的版本。`<!DOCTYPE html>`就是html5的文档声明。

### html根标签

一个页面中有且只有一个html标签，网页中的所有内容都应该写在html根标签中

### head

head标签中的内容是帮助浏览器解析页面的

### title

显示在浏览器的标题栏中。搜索引擎在检索页面时，会首先检索title中的内容。它是网页中对于搜索引擎来说对重要的内容。

### meta

一些属性：

- charset：指定字符集
- name：和content结合设置关键字，name的值是对content值的描述
- content：如上

keywords和description是给搜索引擎看的。

```html
<meta name="keywords" content="HTML5, java, cpp" />
<meta name="description" content="leo's site" />
```

- http-equiv：可以用来重定向。下面的例子在5秒后跳转到google。

```html
<meta http-equiv="refresh" content="5;url=https://www.google.com" />
```

### body

网页中所有可见的内容都在body中

## 1.2 实体（转义字符）

在HTML中，字符之间写再多空格，浏览器也会当成一个空格解析，换行也会当成一个空格解析。

在HTML中，诸如`<`,`>`等特殊字符是不能直接使用的。实体格式：`&实体名字;`。浏览器解析到实体时，会自动把实体转换为对应的字符。

- 空格的实体`&nbsp;`
- 版权符号的实体`&copy;`

## 1.3 图片格式

- JPEG(JGP)：支持颜色多，可以压缩，但是不支持透明。一般用来保存照片。
- PNG：支持的颜色多，支持复杂透明。
- GIF：支持颜色少，支持简单透明（直线透明），支持动态图。

## 1.4 xHTML语法规范

- HTML中不区分大小写，但是一般使用小写
- HTML中的注释不能嵌套
- HTML标签必须结构完整：要么成对、要么自结束
- HTML标签可以嵌套，但是不能交叉嵌套

## 1.5 内联框架

使用内联框架可以引入一个外部页面，iframe标签创建内联框架。搜索引擎不会检索内联框架中的内容。

## 1.6 文本标签及其样式

文本标签主要指语义化的标签

- `<em>` 显式为斜体
- `<strong>` 显式为粗体
