
# Relational Database Design

## Functional Dependencies

用 $R$ 表示一个 Relational Schema，$\alpha \subseteq R$ 和 $\beta \subseteq R$ 分别为 $R$ 的一组属性。

**【Definition】** 
仿照数学上的函数概念，我们定义 $R$ 上的一个 Functional Dependency $\alpha \rightarrow \beta$ 当且仅当对于任意合法关系 $r(R)$ 的两个 Tuples $t_1, t_2$ ，其满足：

$$
t_1[\alpha] = t_[\alpha] \Rightarrow t_1[\beta] =t_2[\beta]
$$

此时称：

- $\beta$ functionally dependant on $\alpha$
- $\alpha$ functionally determines $\beta$

我们用 $F$ 表示一组 Functional Dependencies，例如：

![[FunctionalDependenciesEx.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">Closure of a Set of Functional Dependencies</font>


定义能用 $F$ 逻辑推理出来的最大 Functional Dependencies 集合为 $F$ 的 Closure，用 $F^+$ 表示。在这里给出用于计算函数依赖的 Armstrong's Axioms 及其补充定律：

- **<1> 自反律：** if $\beta \subseteq \alpha$, then $\alpha \rightarrow \beta$
- **<2> 增补律：** if $\alpha \rightarrow \beta$, then $\gamma \alpha \rightarrow \gamma \beta$
- **<3> 传递律：** if $\alpha \rightarrow \beta$ and $\beta \rightarrow \gamma$, then $\alpha \rightarrow \gamma$
- **<4> 合并律：** if $\alpha \rightarrow \beta$ and $\alpha \rightarrow \gamma$, then $\alpha \rightarrow \beta \gamma$
- **<5> 分解律：** if $\alpha \rightarrow \beta\gamma$, then $\alpha \rightarrow \beta$ and $\alpha \rightarrow \gamma$

一种根据 $F$ 计算 $F^+$ 的思想是不断重复遍历 $F$ 内的 FDs(Functional Dependency)，为其应用自反律、增补律、传递律，直到 $F$ 的内容不再改变为止。对于一个 n attribute 的 Schema，其最大可能 FDs 可达 $2^n \times 2^n$ 个。

另一种替代策略是利用 Closure of Attribute Sets，其计算步骤伪代码如下：

```python
for γ in R:
	Compute γ+
	for S in γ+:
		add γ->S to F+
```

<font style="font-weight: 1000;font-size: 20px" color="orange">Closure of Attribute Sets</font>

**Closure of Attribute Sets** 常用来检测属性集 $\alpha$ 是否为 Superkey，用 $\alpha ^+$ 表示，其内容为在 Functional Dependencies $F$ 下由 $\alpha$ 直接或间接决定的属性的集合。

!!! info "与 $F^+$ 不同，Closure of Attribute Sets 本身也是 Attribute 的集合"

计算 $\alpha^+$ 的伪代码如下：

```python
result := α
while (changes to result):
	for each β -> γ in F:
		if β in result: result += γ
α+ := result
```

![[COASEx1.png]]

定义 $\alpha^+$ 后，就可以用其来进行 Super Key 和 Candidate Key 的判断。如果 Relational Schema $R$ 属于 $\alpha^+$ ，则属性集合 $\alpha$ 为 $R$ 的一个 Super Key。

![[candidatekeyCOASEx1.png]]

??? quote "通过画图判断 Candidate Key"
	通过将依赖关系绘制成 Graph，那么没有被任何箭头指向的 Attribute 一定是 Candidate 的一部分（只有它自己决定自己），例如，下例中 $A$ 和 $G$ 都没有被指向，则 $AG \in Candidate\ Key$（实际上 $AG$ 就是）
	
	![[CandidateKeyByGraph.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">Canonical Cover</font>

DBMS 需要保持对数据库进行关于 $F$ 的检查，以确保数据库的完整性。如果 $F$ 过于巨大，检查操作将会变得 Costly。因此，我们需要尽可能简化 the set of FDs。

我们将逻辑等价于 $F$ 的最小 FDs 集合称为 a canonical cover of $F$，记为 $F_c$，其中“最小”一词体现为：

- <1> 不包含冗余 FDs，没有 FDs 包含冗余属性
- <2> FDs 左侧部分为 Unique 的
	- $\alpha \rightarrow \beta_1, \alpha \rightarrow \beta_2$ 合并为 $\alpha \rightarrow \beta_1 \beta_2$

!!! example "$F=\{A\rightarrow B, B\rightarrow C, AC\rightarrow D\}$, Calculate $F_c$"
	```python
	B ->C => AB->AC
	AC->D => AB->D
	A ->B => A ->AB
	AB->D => A ->D
	So AC->D is extraneous => A->D
	F_c = {A->B, B->C, A->D}
	```

将问题标准化来看待，对于一个 Functional Dependency $\alpha \rightarrow \beta$：

- 测试左侧 $\alpha$ 内的 Attribute $A$ 是否多余？
	- 检查 $(\alpha - A)^+$ 是否包含 $\beta$，如果包含，则 $A$ 多余
- 测试右侧 $\beta$ 内的 Attribute $A$ 是否多余？
	- 检查 $\alpha^+$ 是否包含 $A$，如果包含，则 $A$ 多余

## Decomposition

一个 Bad Relation 的设计可能存在以下缺点：

- <1> Information Redundancy 信息冗余
- <2> Updating Difficulty 更新困难
- <3> Insert/Delete Anomalies 插入/删除异常

对于这些不是 "good" form 的 Relation $R$，我们需要将其分解为一组关系 $\{R_1, R_2,..., R_n\}$，它们有以下特点：

- <1> The decomposition is a lossless-join decomposition 无损连接分解
- <2> The decomposition is dependency preservation 依赖保持
- <3> Each relation $R_i$ is in good form
	- such as **BCNF** or **Third Normal Form(3NF)**

!!! tip "Design Method"
	Universal Relation(泛关系) -> Decomposition -> Good Database Schema

将 $R$ 分解为 $R_1$ 和 $R_2$ 是**无损连接**的充要条件是，至少有以下一个依赖关系在 $F^+$ 中满足：

$$\begin{array}l
\{R_1 \cap R_2\} \rightarrow R_1 \\
\{R_1 \cap R_2\} \rightarrow R_2
\end{array}
$$

亦即：分解后两个子 Schema 的**共同属性**必须是 $R_1$ 或 $R_2$ 的 Super Key（适用一分为二的分解）。

**Dependency Preservation** 则要求对于 $F$ 中的每一个 $\alpha \rightarrow \beta$ 都能有一个 $R_i$ 满足 Functional Dependency，则称该分解满足依赖保持，其必然满足：

$$
(F_1 \cup F_2 \cup ... \cup F_n)^+ = F^+
$$

一个典型的**错误**分解例子为：

$R=(A,B,C), F=\{A\rightarrow B, B\rightarrow C\}$，我们将其分解为 $R_1 = (A,B), R_2=(A,C)$。

- **Check lossless-join:** $R_1 \cap R_2 = \{A\}$ and $R_1 \subset (A)^+=\{ABC\}$
- **Check dependency preservation:** $F_1=\{A\rightarrow B\}, F_2=\{A\rightarrow C\}$，but $(F_1 \cup F_2)^+ = \{A\rightarrow B, A\rightarrow C\}^+ \ne F^+$
	- 少了 $B\rightarrow C$

因此该无损分解不满足 Dependency Preservation。

## Normal Form

### First Normal Form

元素不可分的 Domain 称为 Atomic Domain。如果一个 Relational Schema $R$ 的所有 attribute 的 domains 都是 atomic 的，则称这个 Schema 属于 **First Normal Form(1NF)**。

对于 Relational Database，要求所有 Relations 都属于 1NF。

!!! info "Example of non-atomic Domains"
	- Composite Attribute
	- Multi-value Attribute
	- Complex Data Type

### Boyce-Codd Normal Form

**【Definition】** 一个属于 BCNF 的 Relation Schema $R$ ，对于其函数依赖 $F^+$，任意 $\alpha \rightarrow \beta$ 都满足至少下面一条：

- <1> $\alpha \rightarrow \beta$ is trivial
- <2> $\alpha$ 是 $R$ 的 superkey
	- 即 $R\subseteq \alpha^+$ or $\alpha \rightarrow R$

!!! info "Trivial Dependency"
	对于函数依赖 $\alpha \rightarrow \beta$ ，如果 $\beta \subseteq \alpha$ ，则称其为 Trivial Functional Dependency。例如 $A\rightarrow A, \ AB\rightarrow A$

!!! note "任何只有两个 Attribute 的 Schema 都属于 BCNF"

有时候，我们不能让一个分解同时满足其既保持了依赖，又是 BCNF 形式，例如：

$R=(J,K,L)$，$J$ 是学生，$K$ 是课程，$L$ 是老师；其函数依赖为 $F=\{JK\rightarrow L, L\rightarrow  K\}$ (一个老师只教一门课)。那么对于此例，任何分解都不能保持 $JK\rightarrow L$，例如 $R_1=(L,K)$, $R_2=(J,L)$，它们都属于 BCNF，但不满足 Dependency Preservation。

**【Solution】** 事实上，针对 BCNF 的分解有如下计算方法：

- <1> 找出结果集合中还没能满足 BCNF 定义的 Schema $R_i$
	- 它一定存在一个非 Trivial 函数 $\alpha \rightarrow \beta$，并且 $\alpha$ 不是 $R_i$ 的 superkey
- <2> 依据该函数，将 $R_i$ 分解为 $R_{i1}$ 和 $R_{i2}$
	- $R_{i1} = (\alpha, \beta)$, $R_{i2}= R_i - \beta$
- <3> 回到 <1>

!!! warning "不能保证一定满足 Dependency Preserving，只能确保所有子 Schema 都是 BCNF"

### Third Normal Form

为了解决 BCNF 有时不能满足函数依赖保持的问题，我们定义了一个更 Weak 的 Normal Form，称为 3NF，即对于任意 $\alpha \rightarrow \beta$ in $F^+$，至少满足以下一条：

- <1> $\alpha \rightarrow \beta$ is trivial
- <2> $\alpha$ 是 $R$ 的 superkey
	- 即 $R\subseteq \alpha^+$ or $\alpha \rightarrow R$
- <3> 对于每个 Attribute $A\in \beta -\alpha$，$A$ 都被 $R$ 某个 Candidate Key 包含
	- 当 $\beta$ 不属于所有 Candidate Key 时，$\alpha$ 必须是 superkey
	- 当 $\beta$ 属于任意 Candidate Key 时，$\alpha$ 无限制

根据定义看出，BCNF 包含在 3NF 内，也可以说 3NF 是 Minimum relaxation of BCNF to ensure dependency preservation。

!!! abstract "There is always a lossless-join, dependency-preserving decomposition into 3NF"

!!! warning "测试是否为 3NF 是一个 NPH 问题，它需要检查所有 Candidate Keys"
	不过 Decomposition into 3NF 可以在线性时间内完成

![[BCNFvs3NF.png]]

对于我们的 Relational Database Design，目标一般为：

- <1> BCNF
- <2> Lossless Join
- <3> Dependency Preservation

如果我们不能同时满足以上三点，则我们接受以下其中一个缺点：

- <1> Lack of Dependency Preservation
- <2> 3NF instead of BCNF
	- 通常会带来一点 Redundancy

**【Solution】** 针对 3NF 的分解有如下计算方法：

- <1> 计算 $F_c$，将 $F_c$ 中每一个 $\alpha \rightarrow \beta$ 都分解为 Schema $R_i:= (\alpha, \beta)$
- <2> 检查是否存在 $R_i \rightarrow R$，即保证 Lossless-join
	- 如果不存在，则添加 Schema $R_i:= \text{any candidate key of}\ R$
- <3> 返回结果

### Fourth Normal Form

4NF 是 BCNF 的一个缩紧。在介绍 4NF 之前，我们需要引入 **Multivalued Dependencies** 的概念。

假定我们有表 classes(course, teacher, book)，其中 course:teacher，course:book 都是一对多的关系，即一门课有多个老师上，一门课有多个课本。

在这个关系中，三个 Attribute 都是 Independent 的，即它们的 $F$ 中只包含 *Trival* Functional Dependencies。根据定义，该表属于 BCNF。

但是，在实际上看到该表时，我们能够看到其重复信息的冗余：

![[4NFclasses.png]]

!!! info "如果想要为一门课添加一门课本，则需要插入这门课老师数量的 Tuples"

**【Definition】** 对于 Schema $R$，$\alpha \subseteq R$，$\beta \subseteq R$，定义一个 Multivalued Dependency $\alpha \rightarrow \rightarrow \beta$ 当且仅当对于任意合法关系 $r(R)$ 的四个 Tuple $t_1, t_2, t_3, t_4$，其满足：

$$
\begin{cases}t_1[\alpha]= t_2[\alpha] = t_3[\alpha] = t_4[\alpha] \\ 
t_1[\beta] = t_3[\beta] \\
t_2[\beta] = t_4[\beta] \end{cases} \Rightarrow \begin{cases} t_2[R-\alpha -\beta] = t_3[R-\alpha-\beta] \\
t_1[R-\alpha-\beta] = t_4[R-\alpha-\beta]  \end{cases}
$$

![[TabulerRepresent.png]]

??? abstract "Another Definition in 3 tuples"
	![[AnotherDefiMVD.png]]

!!! note "If $\alpha \rightarrow \beta$, then $\alpha \rightarrow \rightarrow \beta$"
	**Every functional dependency is also a multivalued dependency**

**【Definition】** 一个属于 4NF 的 Relation Schema $R$ ，对于其函数依赖和多值依赖 $D^+$，任意 $\alpha \rightarrow \rightarrow \beta$ 都满足至少下面一条：

- <1> $\alpha \rightarrow \rightarrow \beta$ is trivial
- <2> $\alpha$ 是 $R$ 的 superkey
	- 即 $R\subseteq \alpha^+$ or $\alpha \rightarrow R$

现在我们想要将 $R$ 以 4NF 的形式 Decomposition，$D$ 对 $R_i$ 有如下约束：

- <1> $D^+$ 中的任意 Functional Dependency 只包含某个 $R_i$ 的 Attribute
- <2> 对于所有形式为 $\alpha \rightarrow \rightarrow (\beta \cap R_i)$ 的 Multivalued Dependency，如果 $\alpha \subseteq R_i$，则 $\alpha \rightarrow \rightarrow \beta$ 一定位于 $D^+$ 中


