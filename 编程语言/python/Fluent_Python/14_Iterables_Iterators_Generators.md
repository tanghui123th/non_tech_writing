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

在上面的例子中，虽然我们的Sentence对象确实是可迭代对象，但是isinstance和issubclass这两个函数返回的是False。这时因为这两个函数是通过有没有`__iter__`来判断一个对象是不是可迭代的。

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

## 14.3 Sentence Take #2: A Classic Interator

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

## 14.5 Sentence Take #4: A Lazy Implementation

Iterator按照设计应该是lazy的。与lazy相反的是eager。之前我们对Sentence的实现都是eager的，因为我们再`__init__`中把所有的words都在list中存好了。

re.finditer会返回一个generator。这个generator生成一个`re.MatechObject`对象。这个对象的`group()`方法返回实际匹配的字符串。

```python
import re
import reprlib
from collections import abc

RE_WORD = re.compile('\w+')


class Sentence:

    def __init__(self, text):
        self.text = text

    def __repr__(self):
        return 'Sentence(%s)' % reprlib.repr(self.text)

    def __iter__(self):
        for match in RE_WORD.finditer(self.text):
            yield match.group()
```

## 14.6 Sentence Take #5: A Generator Expression

`generator expression`可以理解为`list comprehension`的lazy版本。

```python
>>> def gen_AB():
...     print('start')
...     yield 'A'
...     print('continue')
...     yield 'B'
...     print('en。d')
... 
>>> res1 = [x*3 for x in gen_AB()]
start
continue
end
>>> for i in res1:
...     print('-->', i)
... 
--> AAA
--> BBB
>>> res2 = (x*3 for x in gen_AB())
>>> for i in res2:
...     print('-->', i)
... 
start
--> AAA
continue
--> BBB
end
```

只有当for遍历res2时，gen_AB()才会被执行到。（for会调用next(res2)）

注意`generator expression`实际上是`generator function`的语法糖。

使用生成器表达式，前面的Sentence可以这样实现：

```python
class Sentence:
    
    def __init__(self, text):
        self.text = text
        
    def __repr__(self):
        return 'Sentence(%s)' % replib.repr(self.text)
    
    def __iter__(self):
        return (match.group() for match in RE_WORD.finditer(self.text))
```

## 14.7 Generator Expression: When to Use Them

`generator expression`一般是one use，要重复使用，或者内部逻辑比较复杂，则用`generator function`

## 14.8 Another Example: Arithmetic Progression Generator

前面那我们对生成器的用法是把其作为遍历数据的iterator来使用（生成器本身是一种特殊的迭代器，yield是一个语法糖，内部实现支持了迭代器协议）

现在我们使用generator来实现一个算数级数

```python
class ArithmeticProgression:

    def __init__(self, begin, step, end=None):
        self.begin = begin
        self.step = step
        self.end = end

    def __iter__(self):
        result = type(self.begin+self.step)(self.begin)
        index = 0
        while self.end is None or result < self.end:
            yield result
            index += 1
            result = self.begin + self.step * index

if __name__ == '__main__':
    ap = ArithmeticProgression(0, 1, 3)
    print(list(ap))
    ap = ArithmeticProgression(0, 1/3, 2)
    print(list(ap))
    ap = ArithmeticProgression(0, 1)
    for i in ap:
        print(i)
```

输出

```python
[0, 1, 2]
[0.0, 0.3333333333333333, 0.6666666666666666, 1.0, 1.3333333333333333, 1.6666666666666665]
0
1
2
3
# ... 到无限
```

## 14.9 Generator Functions in the Standard Library

待记

## 14.10 New Syntax in Python 3.3: yield from

直接来看一个例子，下面chain1和chain2的结果一样

```python
def chain1(*iterables):
    for iterable in iterables:
        for i in iterable:
            yield i

def chain2(*iterables):
    for iterable in iterables:
        yield from iterable

s = 'ABC'
t = tuple(range(3))

print(list(chain1(s, t)))   # ['A', 'B', 'C', 0, 1, 2]
print(list(chain2(s, t)))   # ['A', 'B', 'C', 0, 1, 2]
```

这样看起来`yield from`像是一个语法糖。但不是这样的！`yield from`还创造一个内部generator和外部generator之间的管道。这在协程中非常有用。

## 14.11 Iterable Reducing Functions

## 14.12 A Closer Look at the iter Function

## 14.13 Case Study: Generators in a Database Conversion Utility

## 14.14 Generators as Coroutines

当generator用send()时，变成了coroutine。注意，next()是从generator中获取数据，而send()是向yield表达式发送数据。方向正好相反。

注意`David Beazley`的几点建议：

- Generators produce data ofr iteration
- Coroutines are consumers of data
- To keep your brain from exploding, you don't mix the two concept together.
- Coroutines are not related to iteration.
- Note: There is a use of having yield produce a value in a coroutine, but it's not tied to iteration.s


