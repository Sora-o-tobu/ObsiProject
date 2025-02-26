
# SQL

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