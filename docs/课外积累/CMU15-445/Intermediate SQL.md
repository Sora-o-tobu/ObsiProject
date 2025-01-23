
# 2. 中级 SQL

## 简介

SQL(Sequel)是一个语言规范，最初由 IBM 设计。不过很多 DBMS 并不会完全按照规范设计它们的语言，每个人都想要将它们的 feature 加入 SQL 后续的更新中。

作为一个关系型语言，SQL 由以下三个方面组合而成：

- Data Manipulation Language (DML)
	- 数据操作语言，即检索数据或修改数据的命令
- Data Defination Language (DDL)
	- 数据定义语言，可涉及到索引、命名空间、触发器、函数等，通常为数据库声明元数据
- Data Control Language (DCL)
	- 数据控制语言，关于安全和类似内容的规范

!!! note "SQL 基本的数据结构是 bag，它类似于一个没有顺序的集合，但是允许重复元素出现"


## Aggregrations + Group By

聚合(Aggregrations)从 a bag of tuples 中返回单个值：

- AVG(col)
- MIN(col)
- MAX(col)
- SUM(col)
- COUNT(col)
- AND SO ON...

例如，我们想要查询学校一共有多少学生是以 CS 专业注册的，可以使用：

```sql
SELECT COUNT(login) AS cnt FROM student WHERE login LIKE "%@CS"

-- 事实上，这里 COUNT(login) 换成 COUNT(*) 甚至 COUNT(1) 效果都是一样的
```

!!! note "这里是选取正则表达式的形式，找出所有以 `@CS` 结尾的学生"

当我们希望查询计算各个课程内的学生的平均 GPA 时，还需要额外使用 `GROUP BY` 来指定：

```sql
SELECT AVG(s.gpa), e.cid
FROM enrolled AS e, student AS s
WHERE e.sid = s.sid
GROUP BY e.cid
```

这将先通过课程 ID 将每个课程分组，然后再计算该课程内所有学生 GPA 的平均。

!!! warning "在这个例子中，如果你还想在 SELECT 后再加上 `s.name`，这一般是不被允许的"

在此基础上，如果只想输出学生平均 GPA 在 3.9 以上的课程，你不能使用以下做法：

```sql
SELECT AVG(s.gpa) AS avg_gpa, e.cid
FROM enrolled AS e, student AS s
WHERE e.sid = s.sid
AND avg_gpa > 3.9
GROUP BY e.cid
```

因为在查询的时候平均值还没有计算出来。此时应该改用 `having` 关键字，来在聚合之后输出指定内容：

```sql
SELECT AVG(s.gpa) AS avg_gpa, e.cid
FROM enrolled AS e, student AS s
WHERE e.sid = s.sid
GROUP BY e.cid
HAVING avg_gpa > 3.9

-- 一些 DBMS 中该句不起作用，应当修改为：
-- HAVING AVG(s.gpa) > 3.9
-- 实际上，AVG 并不会计算两次
```


## String/Date/Time Operations

正如之前所说的那样，DBMS 可能并不会完全遵循 SQL 设计，因此对于字符串判定等事宜，就可能产生不一样的效果。这也导致了不同管理系统之间不能进行简单移植。

例如，`SQL-92` 对字符串大小写敏感，且要求用单引号包围字符串；而 `MySQL` 则对字符串大小写不敏感，并且既可以用单引号又可以用双引号：

```sql
WHERE UPPER(name) = UPPER('KaNyE')  -- SQL-92
WHERE name = "KaNyE"                -- MySQL
```

对于字符串，我们可以使用 `LIKE` 关键字进行 string matching：

- `%` 取代任意长度的 substring（包括空字符串）
- `_` 取代单个字符

SQL 标准要求使用 `||` 来进行字符串的连接，但这个也在不同 DBMS 中有较大区别：

```sql
WHERE login = LOWER(name) || '@cs'       -- SQL-92
WHERE login = LOWER(name) + '@cs'        -- SQL Server
WHERE login = CONCAT(LOWER(name), '@cs') -- MySQL
```

对于 DATE/TIME 属性，这一问题同样明显。例如，对于不同的 DBMS，要想获取当前的时间戳：

```sql
SELECT NOW();

SELECT CURRENT_TIMESTAMP();

SELECT CURRENT_TIMESTAMP;
```

当我们尝试在 MySQL 中计算两个日期之间的天数差：

```sql
SELECT DATE('2020-09-02')-DATE('2020-01-01') AS days;

RESULT:
+------+
| days |
+------+
|  801 |
+------+
```

可以猜出来，这里的返回结果明显是一个字符串，第一个数字 `8` 表示月份相减。

实际上，对于 MySQL，我们应使用其内置的 `DATEDIFF` 函数：

```sql
SELECT DATADIFF(DATE('2020-09-02') ,DATE('2020-01-01')) AS days
```

## Output Control + Redirection

我们不只希望将查询的结果输出到终端上，有时还需要它们加入一个新的关系中，这时可以用到输出重定向：

```sql
SELECT DISTINCT cid INTO CourseIDs FROM enrolled; -- SQL-92

CREATE TABLE CourseIDs (
SELECT DISTINCT cid FROM enrolled);               -- MySQL
```

如果我们不想要新建一个表，而是希望将结果插入一个已存在的表：

```sql
INSERT INTO CourseIDs (
SELECT DISTINCT cid FROM enrolled);
```

!!! note "尽管该插入语句大部分 DBMS 都是相同的，其内部操作的逻辑也可能不相同"

`ORDER BY` 关键字用来调整元组输出的顺序：

```sql
SELECT sid, grade FROM enrolled
WHERE cid = 'CMU 15-721'
ORDER BY grade DESC, sid ASC
```

!!! info "SQL 标准中，默认是升序"

`LIMIT` 关键字用来限制输出元组的数量：

```sql
SELECT sid, name FROM student
WHERE login LIKE '%@CS'
LIMIT 20 OFFSET 10
```

其中 `OFFSET` 关键字用来指定从第几个元素开始，不加则默认为 0。


## Nested Queries

```sql
SELECT name FROM student WHERE sid IN (SELECT sid FROM enrolled)

-- The Same as:
SELECT name FROM student WHERE sid = ANY(SELECT sid FROM enrolled)
```

在嵌套查询中，常用的关键字有：

- `ALL`
- `ANY`
- `IN` = `ANY()`
- `EXISTS`

## Window Functions

```sql
SELECT *, ROW_NUMBER() OVER() AS row_num FROM enrolled
```

那么在输出的表的最后一列，会加上一列标头为 `row_num` 的行数。

在 `OVER` 中，我们可以用 `PARTITION` 关键字来指定分组：

```sql
SELECT cid, sid, ROW_NUMBER() OVER(PARTITION BY cid)
FROM enrolled ORDER BY cid
```

在上例的情况下，会将每个课程按照 `cid` 划分为 Group，并在组内实现标注行号。

结合以上几个特性，我们如何查询每个课程得分第二高的学生？

```sql
SELECT * FROM (
SELECT *, RANK() OVER(PARTITION BY cid ORDER BY grade) AS rank
FROM enrolled) AS ranking
WHERE ranking.rank = 2;
```

## Common Table Expressions

```sql
WITH cteName (col1, col2) AS (
	SELECT 1,2 
)
SELECT col1 + col2 FROM cteName;
```

使用 CTE 可以简化嵌套查询的结构，例如，同样是查询 student ID 的最大值，CTE 可写作：

```sql
WITH cteSource (maxID) AS (
	SELECT MAX(sid) FROM enrolled
)
SELECT name FROM student, cteSource
WHERE student.sid = cteSource.maxID
```

CTE 同样可以调用自身，但需要加上 `RECURSIVE` 关键字：

```sql
WITH RECURSIVE cteSource (counter) AS (
	(SELECT 1)
	UNION
	(SELECT counter + 1 FROM  cteSource
	WHERE counter < 10)
)

SELECT * FROM cteSource;
```

!!! note "但是 SQLite 和 SQL Server 都不支持递归"

