# 1 flask小知识

## 1.1 路由最后的/

视图的路由定义为@app.route('/leo/')，则/leo和/leo/都可以访问到这个视图。/leo实际上是被重定向到了/leo/

## 1.2 flask自动重启

flask自动重启：flask如果开启debug模式，改了代码则会自动重启。

## 1.3 注册路由的两种方法

flask其实也可以像django那样集中注册路由

```python
def hello():
    return 'Hello, Leo'

app.add_url_rule('/hello', view_func=hello)
```

其实`app.route`装饰器内部就是调用了`add_url_rule`

## 1.4 __name__ == '__main__'的真正作用

我们一般在开发中会这样写

```python
if __name__ == '__main__':
    app.run(debug=True)
```

在生产环境中一般是uwsgi或gunicorn来启动服务器。这样就保证了flask自带的这个服务器不会被启动。

## 1.5 视图函数返回的内容

视图函数返回一个response对象。

- status code
- content-type：默认是text/html。所以如果返回
- 主体内容
- 其他内容...

如果只在视图函数中写字符串，flask会自动帮我们封装成response对象。下面这个例子会重定向页面

```python
@app.route('/leooo/')
def hello():
    headers = {
        'content-type': 'text/plain',
        'location': 'http://www.leosirius.com'
    }
    response = make_response('<html></html>', 301)
    response.headers = headers
    return response
```

如果要作为api返回json格式的数据，则按上面的写法，需要改一下`content-type`

```python
'content-type': 'application/json',
```