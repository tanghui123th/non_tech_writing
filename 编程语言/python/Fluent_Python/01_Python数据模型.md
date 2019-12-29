# 1 Python数据模型

Python的数据模型让Python的语法有高度的一致性。可以把Python理解为一个框架，Python数据模型规范了这个框架的API。

## 1.1 A Pythonic Card Deck

```python
import collections

# namedtuple可以快速实现一个没有方法的类，主要用来保存记录。
Card = collections.namedtuple('Card', ['rank', 'suit'])


class FrenchDeck:
    ranks = [str(n) for n in range(2, 11)] + list('JQKA')
    suits = 'spades diamonds clubs hearts'.split()

    def __init__(self):
        self._cards = [Card(rank, suit) for suit in self.suits
                                        for rank in self.ranks]

    def __len__(self):
        return len(self._cards)

    def __getitem__(self, position):
        # 实现了这个方法，便可使用[]运算符。且这个对象是iterable
        return self._cards[position]

if __name__ == '__main__':
    deck = FrenchDeck()
    print('len(deck) = {}'.format(len(deck)))
    print('deck[3] = {}'.format(deck[3]))
    print('random.choice(deck) = {}'.format(random.choice(deck)))

    # 输出
    # len(deck) = 52
    # deck[3] = Card(rank='5', suit='spades')
    # random.choice(deck) = Card(rank='2', suit='spades')  choice方法随机取一个sequence中的元素
```

仅仅实现了两个特殊方法，我们的对象就可以像内置的sequence一样作用于len、random.choice、reverse和sorted等方法。

## 1.2 何时调用特殊方法

首先明确一点，特殊方法是给解释器调用的，而不是给python程序员使用的。除非是在进行元编程。如果像触发对象的特殊方法，使用python的内建函数，如len, iter, str等

另外最好不要在对象中定义`__xxx__`形式的属性，以免python未来实现同名的特殊方法。

### `__repr__`和`__str__`

`__repr__`会被内建的repr()函数调用，`__str__`会被内建的`str()`调用。而console和debugger会调用`repr()`，print则会调用`str()`。

如果`__str__`没有被实现的话，则python会去调用`__repr__`

### 自定义类型的布尔值

python在判断一个对象的布尔值时，调用函数bool(x)。

一般来说，一个对象如果`__bool__`和`__len__`都没有被实现的话，这个对象总是True。否则，bool()会先调用`__bool__`，如果没有`__bool__`，则会调用`__len__`。长度是0就是False，否则是True。

```python
class A:
    def __len__(self):
        return 0

class B:
    pass

if __name__ == '__main__':
    a = A()
    if a:
        print('a is True')
    else:
        print('a is False')

    b = B()
    if b:
        print('b is True')
    else:
        print('b is False')
```

输出

```bash
a is False
b is True
```

## 1.3 特殊方法总结

见[特殊方法](../简单总结/05_特殊方法.md)
