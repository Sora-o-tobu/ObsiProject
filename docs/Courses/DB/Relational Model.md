
# Relational Model

## Structure of Relational Databases

一个 Relational Database 是基于 Relational Model 的，一个或多个 Relation 的集合。Relation 表示为拥有 Row 和 Column 的二维表。

在更正式的定义下，Relation 表示为一组 Domain 的笛卡尔积的**子集**，即 $r\subseteq D_1 \times D_2 \times ...\times D_n$：

![[dikaerjiex.png]]

二维表的 Column 被称为 **attribute**，每个 attribute 允许取值范围即为该 attribute 的 **domain**。

!!! info "特殊值 `null` 是每个 domain 的元素"

Relation 有两个相关概念：

- **Database schema** -- the logical structure of the database.
- **Database instance** -- the snapshot of the data in the database at a given instant in time.

??? example
	![[schemainstanceex1.png]]

其中，relation 中 tuples 的位置顺序是任意的，并且不允许有重复的 tuples 出现。

有几类特殊的 attribute 被称为 Key（码/键）：

- **Superkey:** Values for $K$ are sufficient to identify a unique tuple of each possible relation $r(R)$
	- e.g.: $\{ID\}$ or $\{ID, name\}$
- **Candidate Key:** the minimal superkey
	- e.g.: $\{ID\}$
- **Primary Key:** 用户指定的 Candidate Key，通常带有下划线标识
- **Foreign Key:** Relation $r_2$ 的主键同时为另一个 Relation $r_1$ 的属性，则它在 $r_1$ 中称为外键
	- 此时称 $r_1$ 为 Referencing Relation(参照关系)，$r_2$ 为 Referenced Relation(被参照关系)

!!! note "Primary key and foreign key are integrated constraints."

??? example
	![[foreignkeyex.png]]

## Relational-Algebra

关系代数有 6 种基本运算符和 3 种(或更多)拓展运算符。其中拓展运算符可以由基本运算符组合得到。


| Operator                   | Notation                       | Addition                                         |
| -------------------------- | ------------------------------ | ------------------------------------------------ |
| **Select** 选择              | $\sigma_p(r)$                  | $\sigma_p(r)=\{ t \| t\in r \land p(t)\}$        |
| **Project** 投影             | $\Pi _{A1, A2, ...,Ak}(r)$     | $A_1, ...,A_k$ 是关系 r 的 attribute                 |
| **Union** 并                | $r\cup s$                      | $r\cup s =\{t\| t\in r \lor t\in s\}$            |
| **Set Difference** 集合差     | $r-s$                          | $r-s=\{t\| t\in r\land t\notin s\}$              |
| **Cartesian Product** 笛卡尔积 | $r\times s$                    | $r\times s = \{\{t\ q\}\| t\in r \land q\in s\}$ |
| **Rename** 重命名             | $\rho_{X(A1, A2, ..., An)}(E)$ | 对 Relation E 及其 attribute 都重命名                   |
| ++++++++++                 | +++++                          | ++++++++++                                       |
| **Set Intersection** 交     | $r\cap s$                      | $r\cap s=\{t\| t\in r \land t\in s\}$            |
| **Natural Join** 自然连接      | $r\bowtie s$                   | 把相同的两个属性合并                                       |
| **Division** 除             | $r\div s$                      | 相当于笛卡尔积的逆运算                                      |
| **Assignment** 赋值          | $r\leftarrow s$                | 用来对数据库进行更改                                       |

在不加括号的情况下，它们的大致优先级为：Project ＞ Select ＞ Cartesion Product ＞ Join, Division ＞ Intersection ＞ Union, Difference 

??? example "Division 例子"
	![[divisionex1.png]]

接下来以一个 Bank 的 Example 来演示各个关系代数操作的运用。

```sql
-- 分支名, 分支所在城市, 总财产
branch(branch-name, branch-city, assets)

-- 客户名, 客户住址, 客户所在城市
customer(customer-name, customer-street, customer-city)

-- 存款ID, 分支名, 存款金额
account(account-number, branch-name, balance)

-- 贷款ID, 分支名, 贷款金额
loan(loan-number, branch-name, amount)

-- 客户名, 存款ID
depositor(customer-name, account-number)

-- 客户名, 贷款ID
borrower(customer-name, loan-number)
```

<font style="font-weight: 1000;font-size: 20px" color="red">Ex1. Find all loans of over $1200</font>

查询所有大于 1200 的贷款账单：

$$
\sigma_{amount \gt 1200}(loan)
$$

<font style="font-weight: 1000;font-size: 20px" color="red">Ex2. Find the loan number for each loan of an amount greater than $1200</font>

查询所有大于 1200 的贷款账单的 loan number：

$$
\Pi_{loan-number}(\sigma_{amount \gt 1200}(loan))
$$

<font style="font-weight: 1000;font-size: 20px" color="red">Ex3. Find the names of all customers who have a loan, or an account, or both, from the bank</font>

查询所有的客户名，他起码拥有一个贷款账单或者存款账单：

$$
\Pi_{customer-name}(borrower) \cup \Pi _{customer-name}(depositer)
$$

<font style="font-weight: 1000;font-size: 20px" color="red">Ex4. Find the names of all customers who have a loan at the Perryridge branch</font>

查询所有客户名，并且他在 Perryridge 分行中有贷款：

$$\begin{array}l
Query1: \Pi_{customer-name}( \sigma_{branch-name='Perryridge'}(\\ \sigma_{borrower.loan-number=loan.loan-number}(borrower\times loan)))
\\ Query2:\Pi_{customer-name}( \sigma_{borrower.loan-number = loan.loan-number}\\
(borrower\times (\sigma_{branch-name='Perryridge'}(loan))))
\end{array}$$

其中 Query2 执行速度更快，因为笛卡尔积的计算量相比其它操作是相当大的，因此先把笛卡尔积的两个操作数调整到尽可能小执行效果才最佳。

!!! tip "当然，你也可以使用 natural join 来简化这一操作"
	$$\Pi_{customer-name}(borrower \bowtie (\sigma_{branch-name='Perryridge'}(loan)))$$

<font style="font-weight: 1000;font-size: 20px" color="red">Ex5. Find the largest account balance</font>

找到存款账单中 balance 最大的值：

找出最大值要求自比较，因此需要重命名操作来“复制”一张表：

$$
\Pi_{balance}(account)- \Pi_{account.balance}( \sigma_{account.balance < d.balance}( account \times \rho_{d}(account)))
$$

其思路为先把所有非最大值的 balance 找出来，再用原来的表做集合差找到最大值。

<font style="font-weight: 1000;font-size: 20px" color="red">Ex6. Find all customers who have an account at all branches located in Brooklyn city</font>

查询所有满足在 Brooklyn 市中的所有分行拥有存款账单的客户名：

$$\begin{array}l
\Pi _{customer-name, branch-name}(depositor \bowtie account) \div \\
\Pi_{branch-name}( \sigma_{branch-city='Brooklyn'}(branch))
\end{array}
$$

<font style="font-weight: 1000;font-size: 20px" color="red">Ex7. Delete all loan records with amount in the range of 0 to 50</font>

删除所有贷款金额小于 50 的贷款记录：

$$
load \leftarrow loan- \sigma_{amount \ge 0\ and\ amount \le 50}(loan)
$$

<font style="font-weight: 1000;font-size: 20px" color="red">Ex8. Insert information in the database specifying that Smith has $1200 in account A-973 at the Perryridge branch</font>

插入一条信息，Smith 在 Perryridge 分行存储了 $1200，存款号为 A-973： 

$$
\begin{array}l
account \leftarrow account \cup \{('Perryridge', A\_973, 1200)\}
\\ depositor\leftarrow depositor \cup \{('Smith', A\_973)\}
\end{array}
$$

<font style="font-weight: 1000;font-size: 20px" color="red">Ex8. Pay all accounts with balances over $10,000 6 percent interest and pay all others 5 percent</font>

为所有账户存款分发年利润，要求大于 $10000 的有 6% 利润，其它 5%：

$$
\begin{array}l account\leftarrow & \Pi_{ account\_number, branch\_name, balance * 1.06}(\sigma_{balance \gt 10000}(account)) \\
& \cup \Pi_{ account\_number, branch\_name, balance * 1.05}( \sigma_{balance \le 10000}(account))
\end{array}
$$

!!! info "在大部分 DBMS 中 '-' 是不允许的，因此要改为下划线 '\_'"

此外，还有一个特殊的 Relational-Algebra Operators，叫做 Aggregate Functions(聚合函数)，其基本格式为：

$$
_{G_1 ,G_2,..., G_n} g_{F_1( A_1), F_2( A_2),..., F_n( A_n)}(E)
$$

其中 $G_n$ 为要 group 的 attribute 名，$F_n( A_n)$ 为要对某一属性进行的聚合函数操作，包括：

- **avg:** average value
- **min:** minimum value
- **max:** maximum value
- **sum:** sum of values
- **count:** number of values

??? example "Aggregate Function Example"
	![[AggregateFunctionEx.png]]

