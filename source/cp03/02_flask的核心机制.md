# 2 flask的核心机制

## 2.1 flask中两个上下文

- 应用上下文AppContext：对Flask的封装
- 请求上下文RequestContext：对Request的封装

4个概念

- Flask：核心对象。包括配置文件信息，注册路由、视图函数等功能
- AppContext：对Flask对象的封装，并加了一些额外参数
- Request：保存请求信息
- RequestContext：对Request的封装

在编码中如果需要操作两个对象，务必使用上下文间接操作。

在请求中，如果访问请求上下文，flask会自动检查应用上下文栈中有没有对应的应用上下文，如果没有，flask如把应用上下文入栈。所以在视图函数中使用current_app没有问题。

## 2.2 python中上下文的知识

我们可以对实现了上下文协议的对象使用with。实现了上下文协议的对象叫上下文管理器。一个对象实现`__enter__`和`__exit__`就是上下文协议，也就是上下文管理器。
离线应用或单元测试时，就需要我们手动把应用上下文推入栈。而flask的实现了上下文的两个方法，并在两个方法中分别有push和pop。

```python
with app.app_context():
    ...
```

注意一点，as后边的对象不是上下文管理器，而是上下文管理器的`__enter__`方法返回的值。在下面的例子中，A的对象`A()`才是上下文管理器，`obj_A`的值是1。如果`__enter__`没有`return`，则`obj_A`是`None`

```python
class A:
    def __enter__(self):
        a = 1
        return a

    def __exit__(self):
        b = 2

with A() as obj_A:
    pass
```

```python
class MyResource:
    def __enter__(self):
        print('connect to resource')
        return self
    
    # 如有有异常，tb不为None
    # __exit__是返回true或false，如果是true，with语句的外面不会抛出异常，false则会
    # 如果什么都不返回，实际上也是false，会在外面也抛出异常
    def __exit__(self, exc_type, exc_value, tb):
        if tb:
            print('process execption')
        else:
            print('no execption')
        print('close resource connection')
        b = 2

    def query(self, exc_type, exc_value, tb):
        print('query data')

with MyResource() as resource:
    resource.query()
```

上面的会输出

```
connect to resource
query data
close resource connection
```