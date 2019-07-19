# 1 The Python Data Model

## 1.1 A Pythonic Card Deck

```python
# Example 1-1. A deck as a sequence of cards
import collections

Card = collections.namedtuple('Card', ['rank', 'suit'])


class FrenchDeck:
    ranks = [str(n) for n in range(2,11)] + list('JQKA')
    suits = 'spades diamonds clubs hearts'.split()

    def __init__(self):
        self._cards = [Card(rank, suit) for suit in self.suits 
                                        for rank in self.ranks]

    def __len__(self):
        return len(self._cards)

    def __getitem__(self, position):
        return self._cards[position]
```

namedtuple可以用来创建没有方法，只有属性的简单的类。这里我们创建Card类来表示单个的卡牌。

实现__len__方法，我们便可以使用内建的len()函数来求一个FrenchDeck对象的长度

```
>>> deck = FrenchDeck()
>>> len(deck)
52
```

实现__getitem__方法，那么这个类的对象都是可迭代的（iterable），可以被作用于循环。且可以使用[]运算符，进而有切片的功能。

```
>>> deck[-1]
Card(rank='A', suit='hearts')
>>> deck[12::13]
[Card(rank='A', suit='spades'), Card(rank='A', suit='diamonds'), Card(rank='A', suit='clubs'), Card(rank='A', suit='hearts')]
```

虽然我们没有实现__contains__方法，但是还是可以使用in运算符。这时会对整个对象的元素进行遍历。

```
>>> Card('Q', 'hearts') in deck
True
>>> Card('Q', 'bearts') in deck
False
```

同样我们也可以实现排序（扑克牌从小到大的序列，四种花色分别是方块、钻石、心、黑桃）：

```python
suit_values = dict(spades=3, hearts=2, diamonds=1, clubs=0)

def spades_high(card):
    rank_value = FrenchDeck.ranks.index(card.rank)
    return rank_value * len(suit_values) + suit_values[card.suit]

```

```
>>> for card in sorted(deck, key=spades_high):
...     print(card)
... 
Card(rank='2', suit='clubs')
Card(rank='2', suit='diamonds')
Card(rank='2', suit='hearts')
Card(rank='2', suit='spades')
Card(rank='3', suit='clubs')
...
Card(rank='A', suit='clubs')
Card(rank='A', suit='diamonds')
Card(rank='A', suit='hearts')
Card(rank='A', suit='spades')
```

> 但是到目前为止，我们还不能洗牌，因为对象的是不可变的（immutable）。有两种方法可以是改变对象，一是违背封装的原则直接操作私有成员self._cards，二是实现__setitem__方法

## 1.2 How Special Methods Are Used

要明确一点，特殊方法不是给程序员调用的，而是给python解释器调用的。如果需要调用特殊方法，那么也要使用对应的内建函数，如`len, iter, str`等。

> 另外，对内建对象使用内建函数，python会有很有优化。比如len会直接返回内建对象c语言结构体的ob_size变量，速度会快很多。

## 1.3 Emulating Numeric Types

```python
# Example 1-2. A simple two-dimensional vector class
from math import hypot


class Vector:

    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def __repr__(self):
        return 'Vector(%r, %r)' % (self.x, self.y)

    def __abs__(self):
        return hypot(self.x, self.y)

    def __bool__(self):
        return bool(abs(self))

    def __add__(self, other):
        x = self.x + other.x
        y = self.y + other.y
        return Vector(x, y)

    def __mul__(self, scalar):
        return Vector(self.x * scalar, self.y * scalar)
```

**`__repr__`和`__str__`的区别:**

简单来说，`__repr__`是给程序员看的，`__str__`是给最终用户看的。交互式控制台和debugger会调用repr，而`str()`和`print()`会调用`__str__`。如果这两个方法只想实现一个的话，那一定是`__repr__`。因为如果没有实现`__str__`，那么调用`__str__`的地方会去调用`__repr__`。

我们使用`__add__`和`__mul__`实现了加法和乘法的重载。但是这里的乘法只能是`Vector * scalar`，且顺序不能更改。如果想该顺序，还得实现`__rmul__`方法。另外`__bool__`使得我们的对象可以在使用`bool`值得地方使用。我们规定如果对象的距离是0表示`False`，反之为`True`

