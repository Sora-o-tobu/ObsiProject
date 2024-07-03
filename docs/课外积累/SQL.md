# SQL

!!! quote "Structured Query Language 结构化查询语言"
	SQL 是用于访问和处理数据库的标准的计算机语言。


## 基础语法

### SELECT

```sql
SELECT column1, column2, ... FROM table_name;

SELECT * FROM table_name (where Condition);
```

- 参数说明
	- **column1, column2, ...**：要选择的字段名称，可以为多个字段。如果不指定字段名称，则会选择所有字段。
	- **table_name**：要查询的表名称。
	- **Condition**:筛选满足条件的记录
		- 注意，SQL中采用单引号环绕文本值 (大部分数据库也接受双引号)
		- Condition中还支持 not,and,or 等逻辑运算
		- 特殊条件
			- is NULL `Select * from emp where comm is null;`
			- between and  `Select * from emp where sal between 1500 and 3000;` 包含上下限
			- in `Select * from emp where sal in (5000,3000,1500);`
			- like 模糊查询 `Select * from emp where ename like 'M%';`
				- 后面接类正则表达式，**%** 表示多个字值，_ 下划线表示一个字符；
				- **M%** : 为能配符，正则表达式，表示的意思为模糊查询信息为 M 开头的。
				- **%M%** : 表示查询包含M的所有内容。
				- **%M_** : 表示查询以M在倒数第二位的所有内容。


演示如下
```sql
-- create
CREATE TABLE EMPLOYEE (
  empId INTEGER PRIMARY KEY,
  name TEXT NOT NULL,
  dept TEXT NOT NULL
);

-- insert
INSERT INTO EMPLOYEE VALUES (0001, 'Clark', 'Sales');
INSERT INTO EMPLOYEE VALUES (0002, 'Dave', 'Accounting');
INSERT INTO EMPLOYEE VALUES (0003, 'Ava', 'Sales');
INSERT INTO EMPLOYEE VALUES (0004, 'Ava', 'Sales');
INSERT INTO EMPLOYEE VALUES (0005, 'Ava', 'Accounting');
INSERT INTO EMPLOYEE VALUES (0006, 'Ava', 'Sales');
INSERT INTO EMPLOYEE VALUES (0007, 'Ava', 'Sales');
INSERT INTO EMPLOYEE VALUES (0008, 'Dave', 'Accounting');
INSERT INTO EMPLOYEE VALUES (0009, 'Nimisora', 'Accounting');

-- fetch 
SELECT * FROM EMPLOYEE;

SELECT * FROM EMPLOYEE WHERE dept = 'Sales' and empID >5;

SELECT name,dept FROM EMPLOYEE;

-- Output

+-------+----------+------------+
| empId | name     | dept       |
+-------+----------+------------+
|     1 | Clark    | Sales      |
|     2 | Dave     | Accounting |
|     3 | Ava      | Sales      |
|     4 | Ava      | Sales      |
|     5 | Ava      | Accounting |
|     6 | Ava      | Sales      |
|     7 | Ava      | Sales      |
|     8 | Dave     | Accounting |
|     9 | Nimisora | Accounting |
+-------+----------+------------+
+-------+------+-------+
| empId | name | dept  |
+-------+------+-------+
|     6 | Ava  | Sales |
|     7 | Ava  | Sales |
+-------+------+-------+
+----------+------------+
| name     | dept       |
+----------+------------+
| Clark    | Sales      |
| Dave     | Accounting |
| Ava      | Sales      |
| Ava      | Sales      |
| Ava      | Accounting |
| Ava      | Sales      |
| Ava      | Sales      |
| Dave     | Accounting |
| Nimisora | Accounting |
+----------+------------+
```

- `SELECT DINSTINCT` 用于返回唯一不同的值，即去掉重复值

演示如下
```sql
-- 同一个 Table 不再重复

-- fetch 
SELECT * FROM EMPLOYEE;

SELECT DISTINCT name FROM EMPLOYEE;

SELECT DISTINCT dept FROM EMPLOYEE;

-- Output

+-------+----------+------------+
| empId | name     | dept       |
+-------+----------+------------+
|     1 | Clark    | Sales      |
|     2 | Dave     | Accounting |
|     3 | Ava      | Sales      |
|     4 | Ava      | Sales      |
|     5 | Ava      | Accounting |
|     6 | Ava      | Sales      |
|     7 | Ava      | Sales      |
|     8 | Dave     | Accounting |
|     9 | Nimisora | Accounting |
+-------+----------+------------+
+----------+
| name     |
+----------+
| Clark    |
| Dave     |
| Ava      |
| Nimisora |
+----------+
+------------+
| dept       |
+------------+
| Sales      |
| Accounting |
+------------+
```


- `ORDER BY column1 , column2 (ASC|DESC)`  用于对结果集进行排序，默认升序

演示如下
```sql
-- 同一个 Table 不再重复

-- fetch 
SELECT * FROM EMPLOYEE;

SELECT * FROM EMPLOYEE WHERE dept = 'Sales' order by name;

SELECT * FROM EMPLOYEE order by empId desc;

-- Output

+-------+----------+------------+
| empId | name     | dept       |
+-------+----------+------------+
|     1 | Clark    | Sales      |
|     2 | Dave     | Accounting |
|     3 | Ava      | Sales      |
|     4 | Ava      | Sales      |
|     5 | Ava      | Accounting |
|     6 | Ava      | Sales      |
|     7 | Ava      | Sales      |
|     8 | Dave     | Accounting |
|     9 | Nimisora | Accounting |
+-------+----------+------------+
+-------+-------+-------+
| empId | name  | dept  |
+-------+-------+-------+
|     3 | Ava   | Sales |
|     4 | Ava   | Sales |
|     6 | Ava   | Sales |
|     7 | Ava   | Sales |
|     1 | Clark | Sales |
+-------+-------+-------+
+-------+----------+------------+
| empId | name     | dept       |
+-------+----------+------------+
|     9 | Nimisora | Accounting |
|     8 | Dave     | Accounting |
|     7 | Ava      | Sales      |
|     6 | Ava      | Sales      |
|     5 | Ava      | Accounting |
|     4 | Ava      | Sales      |
|     3 | Ava      | Sales      |
|     2 | Dave     | Accounting |
|     1 | Clark    | Sales      |
+-------+----------+------------+
```

### INSERT

```sql
INSERT INTO table_name (column1,column2,column3,...)
VALUES (value1,value2,value3,...);
```


- INSERT INTO 语句用于向表中插入新记录。
- 参数说明
	- **table_name**：需要插入新记录的表名。
	- **column1, column2, ...**：需要插入的字段名，可以不加
	- **value1, value2, ...**：需要插入的字段值。

### UPDATE

```sql
UPDATE table_name
SET column1 = value1, column2 = value2, ...
WHERE condition;
```

- UPDATE 用于更新表中记录

演示如下
```sql
-- 同一个 Table 不再重复

-- fetch 
SELECT * FROM EMPLOYEE;

-- Update
update EMPLOYEE set dept = 'GirlFriend' where name = 'Nimisora';

-- fetch again
SELECT * FROM EMPLOYEE;

-- Output

+-------+----------+------------+
| empId | name     | dept       |
+-------+----------+------------+
|     1 | Clark    | Sales      |
|     2 | Dave     | Accounting |
|     3 | Ava      | Sales      |
|     4 | Ava      | Sales      |
|     5 | Ava      | Accounting |
|     6 | Ava      | Sales      |
|     7 | Ava      | Sales      |
|     8 | Dave     | Accounting |
|     9 | Nimisora | Accounting |
+-------+----------+------------+
+-------+----------+------------+
| empId | name     | dept       |
+-------+----------+------------+
|     1 | Clark    | Sales      |
|     2 | Dave     | Accounting |
|     3 | Ava      | Sales      |
|     4 | Ava      | Sales      |
|     5 | Ava      | Accounting |
|     6 | Ava      | Sales      |
|     7 | Ava      | Sales      |
|     8 | Dave     | Accounting |
|     9 | Nimisora | GirlFriend |
+-------+----------+------------+
```

### DELETE

```sql
DELETE FROM table_name
WHERE condition;
```

- DELETE 用于删除表中的行

演示如下

```sql
-- 同一个 Table 不再重复

-- fetch 
SELECT * FROM EMPLOYEE;

-- Delete
delete from EMPLOYEE where dept = 'Sales';

-- fetch again
SELECT * FROM EMPLOYEE;

-- Output

+-------+----------+------------+
| empId | name     | dept       |
+-------+----------+------------+
|     1 | Clark    | Sales      |
|     2 | Dave     | Accounting |
|     3 | Ava      | Sales      |
|     4 | Ava      | Sales      |
|     5 | Ava      | Accounting |
|     6 | Ava      | Sales      |
|     7 | Ava      | Sales      |
|     8 | Dave     | Accounting |
|     9 | Nimisora | Accounting |
+-------+----------+------------+
+-------+----------+------------+
| empId | name     | dept       |
+-------+----------+------------+
|     2 | Dave     | Accounting |
|     5 | Ava      | Accounting |
|     8 | Dave     | Accounting |
|     9 | Nimisora | Accounting |
+-------+----------+------------+
```

## 高级教程

有需要再说吧
