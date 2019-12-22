# 20 JSON

## 20.1 语法

JSON语法可以表示三种类型的值：

- 简单值：可以表示js中的字符串、数字、布尔值和null，但是不支持undefined
- 对象：属性必须加双引号
- 数组

序列化与解析：

- stringify() 把js值序列化为JSON字符串
- parse() 把JSON字符串解析为js值

## 20.2 stringify()

在序列化时，函数和值为undefined的属性会被忽略。

### 过滤器

stringify()的第二个参数是一个过滤器，可以是数组，也可以是函数。如果是数组，返回的JSON字符串只会包含数组中出现的属性，如果是函数，这个函数需要有两个参数，即键和值。

```js
var book = {
  "title": "Professional Javascript",
  "authors": ["leo", "mike"],
  "edition": 3,
  "year": 2077
};

var jsonText = JSON.stringify(book, ["title", "authors", "edition"]);
// "{"title":"Professional Javascript","authors":["leo","mike"],"edition":3}"
```

```js
var book = {
  "title": "Professional Javascript",
  "authors": ["leo", "mike"],
  "edition": 3,
  "year": 2077
};

var jsonText = JSON.stringify(book, function (key, value) {
  switch (key) {
    case "authors": return value.join(",");
    case "edition": return undefined;
    case "year": return undefined;
    default: return value;
  }
});

// "{"title":"Professional Javascript","authors":"leo,mike"}"
```

### 序列化格式

stringify的第三个参数可以用来控制缩进

```js
var book = {
  "title": "Professional Javascript",
  "authors": ["leo", "mike"],
  "edition": 3,
  "year": 2077
};

var jsonText = JSON.stringify(book, null, 2);

// "{
//   "title": "Professional Javascript",
//   "authors": [
//     "leo",
//     "mike"
//   ],
//   "edition": 3,
//   "year": 2077
// }"
```

### toJSON

对象自己还可以实现一个toJSON方法，stringify如果检测到对象有这个方法，会优先使用这个方法的返回结果

```js
var person = {
  toJSON: function() {
    return "hahah";
  }
}

var jsonText = JSON.stringify(person);
// "hahah"
```


## 20.3 parse()

解析的第二个参数是一个函数，可以用来自定义解析。类似于stringify的过滤器函数，只不过方向反了过来。
