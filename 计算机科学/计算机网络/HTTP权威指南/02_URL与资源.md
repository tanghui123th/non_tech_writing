# 2 URL与资源

## 2.2 URL的语法

除http之外，ftp、smtp等协议都可以使用url提供的方案。url的9大组成部分如下

`<scheme>://<user>:<password>@<host>:<port>/<path>;<params>?<query>#<frag>`

### params

一个ftp参数的例子

```
ftp://pre.edu/pub/gnu;type=d
```

http中路径每一段都可以有自己的参数，如

```
http://www.123.com/hammers;sale=false/index.html;graphics=true
```

### frag

片段在客户端内容使用，不会传到服务器。
