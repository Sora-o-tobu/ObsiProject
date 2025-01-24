
# 1. Course Overview

## Defination

根据一般定义，数据库指的是一组以某种关系相互关联的数据。我们常说的 MySQL、Oracle、MongoDB、Snowflake 等实际上属于数据库管理系统(Database Management Systems, **DBMS**)，是用来管理数据库的软件。

对于一个音乐软件，如果我们需要为其设计数据库，可以想到把 artist 和 album 放在两个不同的 csv 文件中，当我们执行查询等操作时可以进行逐行解析：

```sql
-- artist.csv
--- Artist, Year, Country
"Wu-Tang Clan", 1992, "USA"
"Notorious BIG", 1992, "USA"
"GZA", 1990, "USA"

-- album.csv
--- Album, Artist, Year
"Enter the Wu-Tang", "Wu-Tang Clan", 1993
"St.Ides Mix Tape", "Wu-Tang Clan", 1994
"Liquid Swords", "GZA", 1990
```

例如，当我们想要查询什么年代 GZA 出道，可以使用如下简单的 python 代码：

```python
for line in file.readlines():
	record = parse(line)
	if record[0] == "GZA":
		print(int(record[1])

# Actually in sql:
SELECT year FROM artists WHERE name=="GZA";
```

!!! danger "众所周知，这种简单的线性扫描并不能带来完整的功能和性能"
	- data intergrity
	- implementation
	- durability
	- ...

## Data Models

- **Relational**
- Key/Value
- Graph
- Document/Object
- Wide-Column/Column-family
- Array/Matrix/Vectors
- Hierarchical
- Network
- Multi-Value

!!! warning "最后三种几乎没有人再使用"

本课程主要涉及的数据库建立在关系型模型的基础上，它有如下三点关键：

- Store database in simple data structures (relations).
- Access data through high-level language, DBMS figures out best execution strategy.
- Physical storage left up to the DBMS implementation.

因此，对于 Ralational Model 设计的三个重要概念在于：

- <1> **Structure:** The defination of the dababase's relations and their contents
- <2> **Intergrity:** Ensure the database's contents satisfy constraints
- <3> **Manipulation:** Programming interface for accessing and modifying a database's contents

继续使用我们上一节中的数据，不过这次将其存储在一个 Table（Relation）中：

| Name          | Year | Country |
| ------------- | ---- | ------- |
| Wu-Tang Clan  | 1992 | USA     |
| Notorious BIG | 1992 | USA     |
| GZA           | 1990 | USA     |

对于一个关系，我们需要为其设定一个 Primary Key 作为各个元组的独特标识。在本例中，由于一般情况下 Artist 不会重名，我们可以将 Name 作为主键。

!!! info "如果不定义主键，一些 DBMS 会自动生成一个内部主键"

不过我们不能保证 Artist 的名字不会重复，因此，更好的解决办法是，额外设置一个 ID 字段来作为主键，例如：

| ID  | Name          | Year | Country |
| --- | ------------- | ---- | ------- |
| 123 | Wu-Tang Clan  | 1992 | USA     |
| 456 | Notorious BIG | 1992 | USA     |
| 789 | GZA           | 1990 | USA     |

除此之外，对于多个关联的 Relation，我们需要维护一个 Foreign Key 来进行类似"映射"的操作。一个最常用的方法是创建一个交叉引用表：

![[foreignkeys.png]]

## Data Manipulation Languages, DMLs

DMLs 是用来从数据库中存取数据的语言，即与数据库交互的方式。它可以分为两类：

- **Procedural Method:** 对数据库的请求专注于更高层次上对如何查询想要结果的策略
	- Relational Algebra
- **Non-Procedural (Declarative):** 对数据库的请求只指出我们想要的数据，而不是怎么去找到它
	- Relational Calculus

### Relational Algebra

对于关系代数，我们定义以下七个最基本的关系符号，它们的操作数是关系中的元组：


| Symbols   | Operations   |
| --------- | ------------ |
| $\sigma$  | Select       |
| $\Pi$     | Projection   |
| $\cup$    | Union        |
| $\cap$    | Intersection |
| -         | Difference   |
| $\times$  | Product      |
| $\bowtie$ | Join         |

接下来我们以 $R(a\_id, b\_id)$ 和 $S(a\_id, b\_id)$ 两个关系来演示各个操作的效果：

![[RStable.png]]

<font style="font-weight: 1000;font-size: 20px" color="red">1. Select</font>

语法为：

$$
\sigma_{predicate}(R)
$$

```sql
SELECT * FROM R WHERE a_id='a2' AND b_id > 102;
```

$$
\sigma_{ a\_id ='a2' \land b\_id \gt 102}(R)
$$


| a\_id | b\_id |
| ----- | ----- |
| a2    | 103   |

<font style="font-weight: 1000;font-size: 20px" color="red">2. Projection</font>

语法：

$$
\Pi _{A1, A2, ...,An}(R)
$$

```sql
SELECT b_id-100, a_id FROM R WHERE b_id > 101;
```

$$
\Pi _{b \_id -100, a\_id} (\sigma_{ b\_id \gt 101}(R))
$$


| b\_id-100 | a\_id |
| --------- | ----- |
| 2         | a\_2  |
| 3         | a\_3  |

<font style="font-weight: 1000;font-size: 20px" color="red">3. Union</font>

语法：

$$
R\cup S
$$

```sql
(SELECT * FROM R)
	UNION ALL
(SELECT * FROM S);
```


| a\_id | b\_id |
| ----- | ----- |
| a1    | 101   |
| a2    | 102   |
| a3    | 103   |
| a3    | 103   |
| a4    | 104   |
| a5    | 105   |

<font style="font-weight: 1000;font-size: 20px" color="red">4. Intersect</font>

语法：

$$
R\cap S
$$

```sql
(SELECT * FROM R)
	INTERSECT
(SELECT * FROM S);
```


| a\_id | b\_id |
| ----- | ----- |
| a3    | 103   |

<font style="font-weight: 1000;font-size: 20px" color="red">5. Difference</font>

语法：

$$
R-S
$$

```sql
(SELECT * FROM R)
	EXCEPT
(SELECT * FROM S);
```


| a\_id | b\_id |
| ----- | ----- |
| a1    | 101   |
| a2    | 102   |


<font style="font-weight: 1000;font-size: 20px" color="red">6. Product</font>

语法：

$$
R\times S
$$

```sql
SELECT * FROM S CROSS JOIN S;

SELECT * FROM S, R;
```


| R.a\_id | R.b\_id | S.a\_id | S.b\_id |
| ------- | ------- | ------- | ------- |
| a1      | 101     | a3      | 103     |
| a1      | 101     | a4      | 104     |
| a1      | 101     | a5      | 105     |
| a2      | 102     | a3      | 103     |
| a2      | 102     | a4      | 104     |
| a2      | 102     | a5      | 105     |
| a3      | 103     | a3      | 103     |
| a3      | 103     | a4      | 104     |
| a3      | 103     | a5      | 105     |


<font style="font-weight: 1000;font-size: 20px" color="red">7. Join</font>

语法：

$$
R\bowtie S
$$

```sql
SELECT * FROM R NATURAL JOIN S;

SELECT * FROM R JOIN S USING (a_id, b_id);
```


| a\_id | b\_id |
| ----- | ----- |
| a3    | 103   |

