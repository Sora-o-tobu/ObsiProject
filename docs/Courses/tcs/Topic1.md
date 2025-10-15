
# Basics

## Sets

!!! danger "集合部分和以前课程重复率较高，仅简单记载"

**Set** 是元素的无序集合，空集用 $\emptyset$ 表示。

两个集合相等当且仅当它们包含的元素相同，即：

$$
S=T \Leftrightarrow (S\subseteq T) \land (T\subseteq S)
$$

集合的 Operations 包括：

- **Union:** $A\cup B$
- **Intersection:** $A\cap B$
	- 又叫 **disjoint**
- **Complement**
- **Difference:** $B-A$
- **Symmetric Difference**

集合的 Identities：

![[t1_1.png]]

Power Set 为 the set of all subset，表示为 $P(A)$ 或 $2^A$：

$$
2^A= \{T|T\subseteq A\}
$$

集合的势为集合内不同元素的数量，表示为 $|A|$。任何有限的集合或者与整数集等势的集合都称作 Countable。

对于 Power Set 的势，有如下两点性质：

- $|S|=n\Rightarrow |P(S)|=2^n$
- $S$ is finite and so is $P(S)$

## Relations

不同于 Set，一个 Ordered Pair 相等当且仅当：

$$
(a,b)=(c,d) \Leftrightarrow (a=c) \land (b=d)
$$

!!! note "表示方式上区分为 Set 为花括号，Pair 为小括号"

Binary Relation 是两个集合的笛卡尔积的子集，表示为：

$$
R\subseteq A\times B=\{(a,b)|a\in A\land b\in B\}
$$

!!! info "$R\subseteq A\times B \Rightarrow R^{-1} \subseteq B\times A$"

对于一个 Relation，它的 *input values* 的集合称为其的 **Domain**；它的 *output values* 的集合称为其 **Range**。通常对应着笛卡尔积的左乘子和右乘子两个部分。

除此之外，Binary Relation 还有一些特殊 Properties：

- **Reflexive** 自反性 $\forall a\in A \Rightarrow (a,a)\in R$
- **Irreflexive** 反自反性  
- **Symmetric** 对称性 $(a,b) \in R\land a\ne b \Rightarrow  (b,a) \in R$ 
- **Antisymmetric** 反对称性 $(a,b)\in R \land a\ne b \Rightarrow (b,a)\notin R$
- **Transitive** 传递性 $(a,b)\in R, (b,c)\in R\Rightarrow (a,c)\in R$

!!! abstract "Direct Graph、Matrix 都可以看作是 Binary Relation 的特殊类型"

- **Partial Order (偏序关系)**  
	- Relation $R$ on $A$ is a **partial order** if it is:
	    - Reflexive
	    - Antisymmetric
	    - Transitive
    - 例如集合包含关系 $\subseteq$ 在所有集合上构成偏序。
    
- **Total Order (全序关系)**
	- 若偏序关系 $R$ 还满足：
	- $$\forall a,b\in A,\ (a,b)\in R \lor (b,a)\in R$$
	- 即任意两元素可比较，则称为 **Total Order**。  
	- 例如自然数集上的 $\le$。

Function $f: A\rightarrow B$ 在 Relation 的基础上，要求对于任意 $a\in A$，最多存在一个 $b\in B$ 且 $(a,b) \in f$。我们将 $(a,b) \in f$ 写作 $f(a) =b$。

Function 分为单射、满射和双射：

- **One-to-one** / injection / 单射
	- $\forall a,b \in A,\ (f(a)=f(b)\to a=b)$
- **Onto** / surjection / 满射
	- $\forall b\in B,\ \exists a \in A, \ (f(a)=b)$
- **One-to-one and Onto** or **one-to-one correspondence** / bijection / 双射、一一对应
	- 等于 One-to-one + Onto

有关函数，有一系列相关理论可以应用：

- **Cantor's Theorem**
	- 对于集合 $A$，其 Power Set 的势严格大于 $A$ 的势
	- $|A|\lt |P(A)|$
- **Pigeonhole Principle**
	- 若将 $N$ 个对象放入 $k$ 个盒子，则至少有一个盒包含 $\lceil N / k \rceil$ 个对象
- Mathematical Induction, Diagonalization Principle...

!!! addition "Closure"
	设集合 $A$ 在某种运算 $\star$ 下满足：
	
	$$\forall a,b\in A,\ a\star b \in A$$
	
	则称集合 $A$ 对于该运算封闭。例如：
	
	- 整数集 $\mathbb{Z}$ 对加法、乘法封闭；
	- 自然数集 $\mathbb{N}$ 对减法不封闭。

## Language

在讲解 Language 之前，我们需要先定义形式语言理论中的几个概念。

任何*有限非空集合*均称作 **alphabet**，用符号 $\Sigma$ 表示；alphabet 内元素被称为 **symbols**。

在 alphabet 之上，我们称 symbols 的有限序列为 **Strings**，其中根据所包含内容不同，使用不同符号表示：

- 使用 $e$ 表示 empty String over $\Sigma$
	- 很多教材（包括后面章节的 PPT 都是用 $\upepsilon$ 表示）
- 使用 $\Sigma^*$ 表示 the set of all Strings over $\Sigma$
	- 这也意味着如果 $\Sigma$ 是一个有限集合，那么 $\Sigma^*$ 为 *Countably Infinite Set*

!!! note "我们使用 $w\in \Sigma^*$ 来表示 $w$ 是 $\Sigma^*$ 中的一个 String"

String 之间有连接运算（**Concatenation**），其本质是有限序列之间的拼接，使用 $x\circ y$ 或 $xy$ 表示。对于任意 String $w$，都有 $w^0=e$，并且 $w^{i+1}= w^i \circ w$。

!!! example "$\forall w, we=ew=w$"

**Language** 是 $\Sigma^*$ 的子集，表示为 $L\subseteq \Sigma^*$。有些 Language 不能通过列举有限个 Strings 来表示，必须通过指定 scheme 的方式定义，例如：

$$
L=\{ab, aabb, aaabbb,...\}=\{a^n b^n| n\ge 1\}
$$

!!! tip "$\emptyset, \Sigma, \Sigma^*$ 都是 Languages"

与 Strings 类似，我们也为 Language 定义 Concatenation 运算：

$$
L_1 L_2 = \{w_1 w_2 | w_1 \in L_1\land 2_2 \in L_2\}
$$

**【Example】**

- $L_1 = \{w\in \{0,1\}^* : w\text{ has an even number of 0}\}$
- $L_2 = \{w\in \{0,1\}^* : w\text{ starts with 0, the rest symbol are all 1}\}$
- $L_1 L_2 = \{w\in \{0,1\}^* : w\text{ has an odd number of 0}\}$

另外我们定义 Language 的 Kleene Star 运算：

$$\begin{array}l
L^* = \{w\in \Sigma^* : w=w_1 ... w_k,\ k\ge 0,\ w_1,...,w_k\in L\}= L^0 \cup L^1 \cup L^2\cup ...
\\ 
L^+ = LL^* = L^1 \cup L^2\cup L^3\cup ...
\end{array}
$$

那么此时 $L^+$ 即为 $L$ 关于函数 *Concatenation* 的 Closure。

!!! question
	- Can $e\in L^+$ for some $\Sigma$ ?
		- $L^+$ = $L\cup L^2\cup...$，而 $L^0=\{e\}$ 被从 $L^*$ 中剔除，因此 $e\in L^+$ 的充要条件是 $e\in L$
	- When we have $L^+= L^*$
		- 同上，我们知道 $L^* = \{e\} \cup L^+$, 以你 $L^+= L^*$ 的充要条件也是 $e\in L$
	- Can $L^*$ or $\emptyset^*$ equal to $\emptyset$ ?
		- 由于 $L^*=\{e\} \cup L^+$，一定包含 $e$，所以它不可能等于空集
		- $\emptyset ^*=\{e\} \cup \emptyset \cup \emptyset \cup ... = \{e\}$

