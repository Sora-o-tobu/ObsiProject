
# SQL

!!! quote "一定要记住查询语句的执行顺序，才能理清逻辑"
	$$from\rightarrow where \rightarrow group(aggregate)\rightarrow having \rightarrow select \rightarrow order\ by$$

## Overview

SQL(Sequel)是一个语言规范，最初由 IBM 设计。不过很多 DBMS 并不会完全按照规范设计它们的语言，每个人都想要将它们的 feature 加入 SQL 后续的更新中。

!!! warning "我不想我的 SQL 语句能被别的厂商直接使用（）"

按照标准符合度(Conformance Levels)，可以分为四类：

- Entry Level SQL 入门级
- Transitional SQL 过渡级
- Intermediate SQL 中间级
- Full SQL 完全级

到了现在，已经没有 DBMS 产品会实现 FULL LEVEL 的标准符合度了。

作为一个关系型语言，SQL 主要由以下三个方面组合而成：

- Data Manipulation Language (DML)
	- 数据操作语言，即检索数据或修改数据的命令
	- `select ... from ...`
	- `insert`, `delete`, `update`
- Data Defination Language (DDL)
	- 数据定义语言，可涉及到索引、命名空间、触发器、函数等，通常为数据库声明元数据
	- `create table`, `alter table`, `drop table`
	- `create index`, `drop index`
	- `create view`, `drop view`
	- `create trigger`, `drop trigger`
- Data Control Language (DCL)
	- 数据控制语言，关于安全和类似内容的规范
	- `grant`, `revoke`

## Data Definition Language

一个典型的 DDL 语句如下：

```sql
CREATE TABLE branch
			(branch_name char(15) not null,
			branch_city  varchar(30),
			assets       numeric(8,2),
			primary key(branch_name))
```

SQL 的 Domain Type 大体有以下几类：

- `char(n)`: 用户指定的定长字符串
	- C语言中字符串一定以 0 结尾，但是 DB 中并不需要
- `varchar(n)`: 用户指定最大长度的可变长度字符串
- `int`: 整数，根据机器的不同指定的范围不同
- `smallint`: 整数，比 `int` 范围更小
- `numeric(p,d)`: Fixed Point Number，p 表示有效数字位数，d 表示小数点后多少位
	- 例如，`numeric(3,1)` 允许 44.5，但不允许 444.5 或者 0.32
- `real, double`: 浮点数，根据机器的不同精度不同
- `float(n)`: 浮点数，由用户指定最小精度
- `null values`: 空
- `date`: 包含年(4 digit)、月、日
	- E.g. `2007-2-27`
- `time`: Time of day, in hours, minutes, and seconds
	- E.g. `11:18:16` or `11:18:16.28`
- `timestamp`: 时间戳，通常是 Data + Time
	- E.g. `2007-2-27 11:18:16.28`

!!! info "SQL 有许多函数用来处理各种类型的数据及其类型转换，但各数据库系统中函数的标准化程度不高"

除了在 `CREATE` 语句中指定 Domain Type 外，还需要加入必要的 Integrity Constraints 来确保数据库的完整性：

- `not null`: 要求非空
- `default`: 设定缺省值
- `check(p)`: 检测谓词 p 是否成立
- `Primary Key(A1, A2,...)`: 括号内不能为空、不能有重复 Keys
- `Foreign Key(A1, A2,...) references r`: 引用对应表的主键


```sql
DROP TABLE branch; # 完全删除 Relation branch

ALTER TABLE loan ADD loan_date date; # 为 loan 添加新属性 loan_date
# 所有新增属性的初始值均为 null

ALTER TABLE loan DROP loan_date # 删除 loan_date 属性
# 很多 DBMS 不支持这一功能

ALTER TABLE branch MODIFY(branch_name char(30), assets not null)
# 调整属性的 domain
```

!!! warning "不要轻易使用 DROP 指令，DELETE 起码还会把 SCHEMA 留下来，DROP 了什么都没了"

```sql
CREATE INDEX index_user_id ON user_t (user_id)
```

在不读取整个表的情况下，索引使得数据库应用程序可以更快查找数据，并且是*用户不可见的*。

由于索引本身也需要更新，更新一个带索引的表要比没有索引的同一表消耗更多时间，因此，理想的做法是只在常常被搜索的 Column 上建立索引。

!!! info "To Drop an index"
	```sql
	DROP INDEX index_user_id ON user_t
	```

## Basic Structure

```sql
SELECT A1, A2, ..., An
FROM r1, r2, ..., rm
WHERE P
```

等价于逻辑代数：

$$
\Pi_{ A_1, A_2,..., A_n}( \sigma_{ P} (r_1 \times r_2 \times ... \times r_n))
$$

!!! note "SQL 允许 Relation 中出现重复元组，使用 `distinct` 关键字来强制筛选"
	```sql
	SELECT DISTINCT branch_name FROM loan;
	SELECT ALL branch_name FROM loan; -- 默认不加就是 all
	```

```sql
SELECT customer_name, borrower.loan_number as loan_id
FROM borrowerm, loan
WHERE borrower.loan_number = loan.loan_number;
```

在 SQL Server 中，可以使用等于号来代替上式的 `as`

SQL 还提供 String-Matching Operator，通常在 `where` 语句中使用 `LIKE` 运算符：

- `%` : 匹配任意字串
- `_` : 匹配任意单字符

## Set Operations

在 SQL 中，集合操作有 `UNION`, `INTERSECT`, `EXCEPT`，分别对应着并集 $\cup$，交集 $\cap$，以及差集 $-$。与其他操作不同的是，set operations 会自动隐去重复的元素，如果我们想保留重复元组，则需使用 `UNION ALL` 等关键字。

!!! question "Suppose a tuple occurs m times in r and n times in s, then it occurs:"
	- $m+n$ times in `r UNION ALL s`
	- $\min (m,n)$ times in `r INTERSECT ALL s`
	- $\max (0, m-n)$ times in `r EXCEPT ALL s`

<font style="font-weight: 1000;font-size: 20px" color="red">Example. Find all customers who have an account but no loan.</font>

```sql
(SELECT customer_name FROM depositor)
EXCEPT
(SELECT customer_name FROM borrower)
```

!!! note "注意不同 DBMS 之间差别，例如 Oracle 使用 `MINUS` 代替 `EXCEPT`，SQL Server 2000 只支持 `UNION` 和 `UNION ALL` 等"


## Aggregate Functions

带聚合函数的 `SELECT` 之后只能跟着 `GROUP BY` 子句中的属性或者聚合函数。

<font style="font-weight: 1000;font-size: 20px" color="red">Example. Find the average account balance for each branch</font>

```sql
SELECT branch_name, avg(balance) AS avg_bal
FROM account
GROUP BY branch_name
```

由于 `WHERE` 的计算顺序比聚合函数前，所有当执行 `WHERE` 判断时聚合函数还没有得到结果，因此我们不能在 `WHERE` 中加入聚合函数。相对的，我们使用 `HAVING` 关键字来作为代替：

<font style="font-weight: 1000;font-size: 20px" color="red">Example. Find the names of all branches located in city Brooklyn where the average account balance is more than $1,200</font>

```sql
SELECT A.branch_name, avg(balance)
FROM account AS A, branch AS B
WHERE A.branch_name = B.branch_name AND
	branch_city = 'Brooklyn'
GROUP BY A.branch_name
HAVING avg(balance) > 1200;
```

除了 `COUNT` 的聚合函数默认忽略 Null Values，除非该值全部为 `null`，此时也会返回 `null`。

!!! note "Null Values"
	任意带 `null` 的算术计算结果都是 `null`，任意带 `null` 的逻辑比较结果都是 `unknown`。此外，三种带 `unknown` 的逻辑计算结果为：
	![[unknownoperations.png]]
	
	同时，作为 `WHERE` 子句中的谓词，需要使用 `WHERE amount is not null` 的形式来判断 null values，而不能直接用等于号


## Nested Query

我们知道 `FROM` 甚至 `WHERE` 子句中都可以以一张表作为对象，而我们 `SELECT` 语句得到的结果就是一张表，因此我们完全可以进行嵌套查询，下面是一个简单例子：

```sql
SELECT distinct customer_name
FROM borrower
WHERE customer_name not in (SELECT customer_name
							FROM depositor);
```

