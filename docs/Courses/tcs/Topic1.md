
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

对于一个 Relation，它的 input values 的集合称为其的 **Domain**；它的 output values 的集合称为其 **Range**。通常对应着笛卡尔积的左乘子和右乘子两个部分。

除此之外，Binary Relation 还有一些特殊 Properties：

- **Reflexive** 自反性 $\forall a\in A \Rightarrow (a,a)\in R$
- **Irreflexive** 反自反性  
- **Symmetric** 对称性 $(a,b) \in R\land a\ne b \Rightarrow  (b,a) \in R$ 
- **Antisymmetric** 反对称性 $(a,b)\in R \land a\ne b \Rightarrow (b,a)\notin R$
- **Transitive** 传递性 $(a,b)\in R, (b,c)\in R\Rightarrow (a,c)\in R$

!!! abstract "Direct Graph、Matrix 都可以看作是 Binary Relation 的特殊类型"

TODO: Partial Order, Total Order, Cardinality

Function $f: A\rightarrow B$ 在 Relation 的基础上，要求对于任意 $a\in A$，最多存在一个 $b\in B$ 且 $(a,b) \in f$。我们将 $(a,b) \in f$ 写作 $f(a) =b$。

Function 分为单射、满射和双射：

- **One-to-one** / injection / 单射
	- $\forall a,b \in A,\ (f(a)=f(b)\to a=b)$
- **Onto** / surjection / 满射
	- $\forall b\in B,\ \exists a \in A, \ (f(a)=b)$
- **One-to-one and Onto** or **one-to-one correspondence** / bijection / 双射、一一对应
	- 等于 One-to-one + Onto

TODO: Cantor's Theorem, Mathematical Induction, Pigeonhole Principle, Diagonalization Principle

TODO: Closure

## Language

TODO: Alphabet, Strings, 