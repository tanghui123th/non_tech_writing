import collections
from collections.abc import Iterable, Iterator, Generator

# 字符串
astr = "XiaoMing"
print("字符串：{}".format(astr))
print(isinstance(astr, Iterable))
print(isinstance(astr, Iterator))
print(isinstance(astr, Generator))

# 列表
alist = [21, 23, 32,19]
print("列表：{}".format(alist))
print(isinstance(alist, Iterable))
print(isinstance(alist, Iterator))
print(isinstance(alist, Generator))

# 字典
adict = {"name": "小明", "gender": "男", "age": 18}
print("字典：{}".format(adict))
print(isinstance(adict, Iterable))
print(isinstance(adict, Iterator))
print(isinstance(adict, Generator))

# deque
adeque=collections.deque('abcdefg')
print("deque：{}".format(adeque))
print(isinstance(adeque, Iterable))
print(isinstance(adeque, Iterator))
print(isinstance(adeque, Generator))