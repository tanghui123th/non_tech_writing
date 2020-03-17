# 3 HTTP报文

## 3.2 报文的组成部分

请求和响应报文都由三部分组成：

- 起始行 start line
- 首部 header
- 主体 body

起始行和首部都是由行分割的ascii文本。每一行的结尾是两个ascii字符：回车符和换行符

主体可以是文本、二进制数据或空

### 请求方法概述

除了下面的常用方法之外，HTTP支持方法扩展，可以实现自己的方法。

|方法|描述|是否包含主体|
|-|-|-|
|GET|从服务器获取一份文档|否|
|HEAD|从服务器获取文档的首部|否|
|POST|向服务器发送需要处理的数据|是|
|PUT|将强求的主题部分存储在服务器上|是|
|TRACE|对可能进过代理服务器传送到服务器上的报文进行追踪|否|
|OPTIONS|获得可以在服务器上执行哪些方法|否|
|DELETE|从服务器上删除一份文档|否|

## 3.3 方法

### 安全方法

GET和HEAD被称为安全方法，因为它们被设计为不应该修改服务器上的资源（但实际上能不能修改，由Web开发者决定）

### GET

### HEAD

HEAD和GET类似，区别是服务器只返回首部。这样客户端可以在未获取实际资源的情况下，对资源的首部进行检查。

### TRACE

一个请求可能经过防火墙、代理、网关等才能到达服务器。trace允许客户端查看请求报文在最后达到服务器后变成了上面样。

原始报文

```
TRACE /some-url HTTP/1.1
Accept: *
Host: www.leosirius.com
```

经过代理的转发后

```
TRACE /some-url HTTP/1.1
Accept: *
Host: www.leosirius.com
Via: 1.1 proxy3.company.com
```

响应报文把服务器收到的请求报文封装包主体里

```
HTTP/1.1 200 OK
Content-type: text/plain
Content-length: 96

TRACE /some-url HTTP/1.1
Accept: *
Host: www.leosirius.com
Via: 1.1 proxy3.company.com
```

### OPTIONS

客户端可以询问服务器支持哪些方法

请求报文

```
OPTIONS * HTTP/1.1
Host: www.leosirius.com
Accept: *
```

响应报文

```
HTTP/1.1 200 OK
Allow: GET, POST, PUT, OPTIONS
Content-length: 0
```

### DELETE

### 扩展方法

下面是WebDAV的一些扩展方法

|方法|描述|
|-|-|
|LOCK|允许用户锁定资源|
|MKCOL|允许用户创建资源|
|COPY|复制资源|
|MOVE|移动资源|
