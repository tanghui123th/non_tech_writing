# 16 Coroutines

协程的语法和生成器一样：都是在一个函数里用yield。但协程的yield可以用来接收数据`datum = yield`。yield右边没有表达式，表示返回出来的结果是None。

yield其实可以完全不考虑数据流，单纯作为一个控制程序顺序的工具。

## 16.1 How Coroutines Evolved from Generators