# 1 start

## 概述

Websocket的最大特点就是，服务器可以主动向客户端推送信息，客户端也可以主动向服务器发送信息，是真正的双向平等对话，属于服务器推送技术的一种。

其他特点：

其他特点包括：

1. 建立在 TCP 协议之上，服务器端的实现比较容易。
2. 与 HTTP 协议有着良好的兼容性。默认端口也是80和443，并且握手阶段采用 HTTP 协议，因此握手时不容易屏蔽，能通过各种 HTTP 代理服务器。
3. 数据格式比较轻量，性能开销小，通信高效。
4. 可以发送文本，也可以发送二进制数据。
5. 没有同源限制，客户端可以与任意服务器通信。
6. 协议标识符是ws（如果加密，则为wss），服务器网址就是 URL。

## A Simple Demo

我们用`ws://echo.websocket.org/`服务器，这是一个别人搭好的实验用的websocket服务器，会返回你发过去的内容。

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Websocket</title>
</head>
<body>
  <h1>Echo test</h1>
  <input id="sendText" type="text"/>
  <button id="sendBtn">发送</button>
  <div id="recv"></div>
  <script type="text/javascript">
    var websocket = new WebSocket("ws://echo.websocket.org/");
    websocket.onopen = function () {
      console.log('websocket open');
      document.getElementById("recv").innerHTML = "Connected";
    }
    websocket.onclose = function () {
      console.log('websocket close');
    }
    websocket.onmessage = function (e) {
      console.log('message = ' + e.data);
      document.getElementById("recv").innerHTML = e.data;
    }

    document.getElementById("sendBtn").onclick = function () {
      var txt = document.getElementById("sendText").value;
      websocket.send(txt);
    }
  </script>
</body>
</html>
```
