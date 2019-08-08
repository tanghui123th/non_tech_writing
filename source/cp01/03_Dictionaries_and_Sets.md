# 3 Dictionaries and Sets

## 3.1 Generic Mapping Types

所有`mapping types`的键都必须是可哈希的

### 关于hash()

在 hash() 对对象使用时，所得的结果不仅和对象的内容有关，还和对象的 id()，也就是内存地址有关。

```python
class TestObj:
    def __init__(self, i):
        self.i = i


obj1 = TestObj(1)
obj2 = TestObj(1)
print(id(obj1))   # 4313486224
print(id(obj2))   # 4313486336
print(hash(obj1)) # 269592889
print(hash(obj2)) # 269592896
```

### What is Hashable?

实现了两种特殊方法的：

- __hash__：使其有在生命周期内不变的哈希值。
- __eq__：可比较。

总的来说，不可变的内建类型都是可哈希的。但有个例外，就是tuple如果以可变类型作为成员，则不可哈希。

```python
>>> tt = (1, 2, (30, 40))
>>> hash(tt)
8027212646858338501
>>> tl = (1, 2, [30, 40])
>>> hash(tl)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: unhashable type: 'list'
>>> tf = (1, 2, frozenset([30, 40]))
>>> hash(tf)
985328935373711578
```

### dict Comprehensions

除了列表可以用生成式，字典也可以。我们先来看看生成字典的5中方法

```python
>>> a = dict(one=1, two=2, three=3)
>>> b = {'one': 1, 'two': 2, 'three': 3}
>>> c = dict(zip(['one', 'two', 'three'], [1, 2, 3]))
>>> d = dict([('two', 2), ('one', 1), ('three', 3)])
>>> e = dict({'three': 3, 'one': 1, 'two': 2})
>>> a == b == c == d == e
True
```

使用字典生成式：

```python
>>> DIAL_CODES = [
...         (86, 'China'),
...         (91, 'India'),
...         (1, 'United States'),
...         (62, 'Indonisia'),
...         (55, 'Brazil'),
...         (92, 'Pakistan'),
...         (880, 'Bangladesh'),
...         (234, 'Nigeria'),
...         (7, 'Russia'),
...         (81, 'Japan'),
...     ]
>>> country_code = {country: code for code, country in DIAL_CODES}
>>> country_code
{'China': 86, 'India': 91, 'United States': 1, 'Indonisia': 62, 'Brazil': 55, 'Pakistan': 92, 'Bangladesh': 880, 'Nigeria': 234, 'Russia': 7, 'Japan': 81}
>>> {code: country.upper() for country, code in country_code.items() if code < 66}
{1: 'UNITED STATES', 62: 'INDONISIA', 55: 'BRAZIL', 7: 'RUSSIA'}
```