# 3 SQL

## 3.1 SQL数据定义

下面是一些操作的说明

- `delete from r` 删除r中的所有元组
- `drop table r` 删除整个表，包括存储的元组和关系本身
- `alter table r add A D` 给r表增加属性A，D是A的域。元组的新属性默认取值是NULL
- `alter table r drop A` 给r表删除属性A

## 3.2 自然联结

自然联结作用于两个关系。笛卡尔积会把第一个关系的所有元组都与第二个关系的所有元组联结。自然联结只会联结在两个关系中都出现的取值相同的元组对。

下面是一个例子

```sql
SELECT name, course_id
FROM instructor, teachers
WHERE instructor.id = teachers.id
```

还可以更自然的写为

```sql
SELECT name, course_id
FROM instructor NATURAL JOIN teachers
```

## 3.3 集合运算

SQL中UNION、INSERT和EXCEPT对应了集合中的并、交和差运算。注意UNION是默认去重的。

## 3.4 空值

- 算术运算：只要操作数中包含null，则该算术表达式的结果为null。
- 比较运算：只要操作数中包含null，则该比较表达式的结果为unknown，这创造了true和false之外的第三个逻辑值

unknown的比较运算规则：

| operatorand1 | operator | operatorand2 | outcome |
| :----------- | :------- | :----------- | :------ |
| true         | and      | unknown      | unknown |
| false        | and      | unknown      | false   |
| unknow       | and      | unknown      | unknown |
|              |          |              |         |
| true         | or       | unknown      | true    |
| false        | or       | unknown      | unknown |
| unknown      | or       | unknown      | unknown |
|              |          |              |         |
|              | not      | unknown      | unknown |

如果where子句对一个记录计算出false或unknown，则该记录不会被加到结果集中。

## 3.5 聚集函数

5个固有聚集函数：

- min
- max
- sum
- avg
- count

sum和avg的参数必须是数字。

除了用聚集函数进行基本的聚集之外，还有`GROUP BY`进行分组聚集。

## 3.6 大对象类型

- clob：字符数据的大对象数据类型，lob表示large object
- blob：二进制数据的大对象数据类型

使用示例

```sql
book_review clob(10KB)
image blob(10MB)
movie blob(2GB)
```

## 3.7 使用程序设计语言访问数据库

编程语言中访问SQL的两种方式：

- 动态SQL：运行时以字符串形式构建SQL查询。Java的接口JDBC，c、c++、c#的接口ODBC。
- 嵌入式SQL：把SQL以宿主语言原生语法的形式嵌入。
