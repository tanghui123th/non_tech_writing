# 13 STL2

## 13.1 set和multiset

### 关联容器

set、multiset、map、multimap

特点：

- 内部元素有序排列，新元素插入的位置取决于它的值，查找速度快。
- 除了各容器都有的函数外，还支持一下成员函数
   - find：查找等于某个值的元素（x小于y和y小于x同时不成立即相等）
   - lower_bound：查找某个下界
   - upper_bound：查找某个上界
   - equal_range：同时查找上界和下界
   - count：计算等于某个值的元素的个数
   - insert：插入一个元素或一个区间