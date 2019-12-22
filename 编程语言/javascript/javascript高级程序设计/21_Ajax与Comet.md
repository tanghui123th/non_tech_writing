# 21 Ajax与Comet

Ajax是`Asynchronous JavaScript + XML`的缩写。虽然名字中有XML，但Ajax通信与数据格式无关。这种技术能够向服务器请求额外的数据而无需卸载页面。Ajax的核心是XMLHttpRequest

## 21.1 XMLHttpRequest对象

### 基本用法

先看一个例子，下面详细解释这个例子

```js
let xhr = new XMLHttpRequest();
xhr.onreadystatechange = function () {
  if (xhr.readyState == 4) {
    // 状态码304表示请求的资源没有没修改
    if ((xhr.status >= 200 && xhr.status < 300) || xhr.status == 304) {
      alert(xhr.responseText);
      var allHeaders = xhr.getAllResponseHeaders();
      console.log(allHeaders);
    } else {
      alert("Request was unsuccessful: " + xhr.status);
    }
  }
}

xhr.open("get", "txt", true);
xhr.send(null);
```

执行上面的代码，浏览器会弹出对话框，显示字符`I am txt`，并在console中输出一下内容

```txt
accept-ranges: bytes
cache-control: public, max-age=0
connection: keep-alive
content-length: 8
content-type: text/plain; charset=UTF-8
date: Sat, 21 Dec 2019 12:21:10 GMT
etag: W/"8-16f281f6247"
last-modified: Sat, 21 Dec 2019 11:02:23 GMT
vary: Accept-Encoding
x-powered-by: Express
```

open函数有三个参数，第一个参数是请求方法。第二个参数是url。第三个参数是是否异步发送请求。这里第二个参数是`txt`，我们index.html同级目录下有文件txt，内容就是`I am txt`。如果要跨域请求，还需要另外处理。

一般我们都是用异步请求，xhr对象的readyState属性表示了请求的状态：

- 0：未初始化，尚未调用open方法
- 1：启动，调用了open，但是没有send
- 2：发送，调用了send，但是尚未收到响应
- 3：接收，已经收到部分响应数据
- 4：完成，接收全部响应数据

send方法的参数是请求的主体内容。

收到响应后，响应的数据会自动填充xhr对象：

- responseText：响应的主体文本
- responseXML：如果响应类型是`text/xml`或`application/xml`，这个属性将保存响应的XML DOM文档
- status：响应http状态
- statusText：状态说明

另外，在收到响应之前，还可以调用`abort()`来取消异步请求。

### HTTP头部信息

- setRequestHeader()：设置头部信息。接受两个参数：头部字段名和头部字段值
- getResponseHeader()：获取响应头部信息，参数是头部字段名
- getAllResponseHeaders()：获取所有响应头部

### get与post请求

get请求中url末尾的查询参数必须用encodeURIComponent()进行编码。

post请求如果要模拟web表单数据，需要把Content-Type的头部改成`application/x-www-form-urlencoded`

## 21.2 XMLHttpRequest 2级

### FormData

FormData为序列化表单以及创建与表单格式相同的数据创造了便利。我们不用再手动指定头部，XHR对象能识别FormData对象的数据并自动加上头部

```js
// user-info 是一个表单元素
var form = document.getElementById("user-info");
xhr.send(new FormData(form));
```

### 超时设定

```js
xhr.open("get", "someurl", true);
xhr.timeout = 1000;
xhr.ontimeout = function () {
  alert("Request did not return in a second.");
}
xhr.send(null);
```

### overrideMimeType()

这个方法用于重写XHR响应的MIME类型

```js
xhr.open("get", "someurl", true);
xhr.overrideMimeType("text/xml");  // 这里强迫xhr将响应对象当做XML来处理
xhr.send(null);
```

## 21.3 进度事件

- loadstart：接收到响应的第一个字节时触发
- progress：在接受响应期间不断的触发
- error：在请求发生错误时触发
- abort：在因为调用abort()方法终止时触发
- load：在收到完整的响应数据时触发
- loadend

每个请求都从loadstart开始，接下来是一个或多个progress事件，然后触发error、abort、load中的一个，最后触发loadend

## 21.4 跨域资源共享

现代浏览器XHR大多都实现了对CORS的原生支持，见[01 跨域请求](../../../实用技术/web开发常见问题/01_跨域请求.md)。
