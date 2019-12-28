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
    # random.choice(deck) = Card(rank='2', suit='spades')  choice方法随机去一个sequence中的对象
```



仅仅实现了两个特殊方法，我们的对象就可以像内置的sequence一样作用于len、random.choice、reverse和sorted等方法。


