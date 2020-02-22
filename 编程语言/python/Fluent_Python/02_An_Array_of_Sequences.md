# 2 An Array of Sequences

## 2.1 Overview of Built-in Sequences

从存储形式上可以分成：

- `container sequences`：存放对不同对象的引用
  - 包括：list, tuple, collections.deque
- `flat sequences`：直接在自己的内存中存放同一类型的元素
  - 包括：str, bytes, bytearray, memoryview, array.array

从可变性来分：

- `mutable sequences`
  - 包括：list, bytearray, array.array, collections.deque, memoryview
- `immutable sequences`
  - 包括：tuple, str, bytes


