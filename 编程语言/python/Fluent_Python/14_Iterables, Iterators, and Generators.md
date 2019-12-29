# 14 Iterables, Iterators, and Generators

## 14.1 Sentence Take #1: A Sequence of Words

能被内建iter()函数调用且不报错的对象就是可迭代对象Iterable。iter()函数会先尝试调用对象的`__iter__`，如果没有，则会调用对象的`__getitem__`。python会从索引0开始，一个一个取出item。如果这两个特殊方法都没实现，则会报错`TypeError: C object is not iterable`

```python
import re
import reprlib
from collections import abc

RE_WORD = re.compile('\w+')


class Sentence:

    def __init__(self, text):
        self.text = text
        self.words = RE_WORD.findall(text)

    def __getitem__(self, index):
        return self.words[index]

    def __len__(self):
        return len(self.words)

    def __repr__(self):
        return 'Sentence(%s)' % reprlib.repr(self.text)


if __name__ == '__main__':
    s = Sentence('Welcome to America and have a nice day.')
    print(s)
    print(s.words)
    print(len(s))

    print('isinstance(s, abc.Iterable) = {}'.format(isinstance(s, abc.Iterable)))
    print('issubclass(Sentence, abc.Iterable) = {}'.format(issubclass(Sentence, abc.Iterable)))

    for word in s:
        print(word)
```

输出

```bash
s = Sentence('Welcome to A...e a nice day.')
s.words = ['Welcome', 'to', 'America', 'and', 'have', 'a', 'nice', 'day']
len(s) = 8
isinstance(s, abc.Iterable) = False
issubclass(Sentence, abc.Iterable) = False
word = Welcome
word = to
word = America
word = and
word = have
word = a
word = nice
word = day
```

在上面的例子中，虽然我们的Sentence对象确实是可迭代对象，但是isinstance和issubclass这两个函数返回的是False。这时因为这两个函数是通过有没有__iter__来判断一个对象是不是可迭代的。

因此判断一个对象是否可迭代的最好方式，就是在迭代这个对象时使用try/catch，这样可以照顾到那些只实现了`__getitem__`的可迭代对象。

最后，实现一个可迭代对象的标准建议还是去实现`__iter__`，通过`__getitem__`实现iterable是历史遗留的产物。

## 14.2 Iterables vs Iterators

一句话总结：Python obtains iterators from iterables.

这里摘抄书中的两个定义：

- iterable
  Any object from which the iter built-in function can obtain an iterator. Objects implementing an `__iter__` method returning an iterator are iterable. Sequences are aways iterable; as are objects implementing a `__getitem__` method that takes 0-based indexes.

- iterator
  Any object that implement the `__next__` no-argument method that returns the next item in a series or raises StopIteration when there are no more items. Python iterators also implement the `__iter__` methdo so they are iterables as well.

```python
>>> s = 'ABC'
>>> for char in s:
...     print(char)
...
A
B
C
>>> s = 'ABC'
>>> it = iter(s)                # 从iterable构建一个iterator
>>> while True:
...     try:
...         print(next(it))     # 使用next()不断获取iterator中的下一个元素
...     except StopIteration:   # 没有下一个元素后，抛出StopIteration
...         del it
...         break
...
A
B
C
```

iterator需要实现两个特殊方法`__next__`和`__iter__`，其中`__iter__`返回self。与iterable的类型判断不同，判断iterator对象可以直接用`isinstance(x, abc.Iterator)`或者issubclass

## 14.3 Sentence Take #2: A Classic Interior

现在用`__iter__`重新实现Sentence。这个例子可以更好的帮我们理解iterable和iterator的关系

```python
import re
import reprlib

RE_WORD = re.compile('\w+')

class Sentence:

    def __init__(self, text):
        self.text = text
        self.words = RE_WORD.findall(text)

    def __repr__(self):
        return 'Sentence(%s)' % reprlib.repr(self.text)

    def __iter__(self):
        return SentenceIterator(self.words)

class SentenceIterator:

    def __init__(self, words):
        self.words = words
        self.index = 0

    def __next__(self):
        try:
            word = self.words[self.index]
        except IndexError:
            raise StopIteration()
        self.index += 1
        return word

    def __iter__(self):
        return self

if __name__ == '__main__':
    s = Sentence('Welcome to America and have a nice day.')
    for word in s:
        print('word = {}'.format(word))
```

输出

```bash
word = Welcome
word = to
word = America
word = and
word = have
word = a
word = nice
word = day
```

## 14.4 Sentence Take #3: A Generator Function

```python
>>> def gen_123():
...     yield 1
...     yield 2
...     yield 3
... 
>>> gen_123
<function gen_123 at 0x10ff6d510>           # 生成器函数本身也是一个函数对象，
>>> gen_123()
<generator object gen_123 at 0x10ff137c8>   # 生成器函数调用后，返回一个生成器
>>> for i in gen_123():
...     print(i)
... 
1
2
3
>>> g = gen_123()
>>> next(g)
1
>>> next(g)
2
>>> next(g)
3
>>> next(g)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
StopIteration
```

生成器函数是函数体内有yield关键字的函数。生成器函数调用后，returns一个生成器。而生成器yields值。

```python
def gen_AB():
    print('start')
    yield 'A'
    print('continue')
    yield 'B'
    print('end')

idx = 0
for c in gen_AB():
    idx += 1
    print('--> {}. times:{}'.format(c, idx))
```

输出

```bash
start
--> A. times:1
continue
--> B. times:2
end
```

现在再用最pythonic的形式来改造一下我们的Sentence。用生成器函数来实现`__iter__`方法

```python
import re
import reprlib

RE_WORD = re.compile('\w+')


class Sentence:

    def __init__(self, text):
        self.text = text
        self.words = RE_WORD.findall(text)

    def __repr__(self):
        return 'Sentence(%s)' % reprlib.repr(self.text)

    def __iter__(self):           # Sentence的特殊方法__iter__是一个生成器函数，返回一个生成器
        for word in self.words:
            yield word
        return                    # 这个return不是必要的，即是不写，这里也会返回。且返回的是生成器


if __name__ == '__main__':
    s = Sentence('Welcome to America and have a nice day.')
    for word in s:
        print('word = {}'.format(word))
```

输出同前面一样。
