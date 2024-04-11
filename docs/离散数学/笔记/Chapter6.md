# Chapter 6 : Counting
# 6.1 The Basic of Counting
## Basic Counting Principles

1. **The Sum Rule**
	If a first task can be done in n<sub>1</sub> ways and a second task in n<sub>2</sub> ways, and if these tasks cannot be done at the same time , then there are n<sub>1</sub> + n<sub>2</sub> ways to do one of these tasks.
也可以用集合表示: $|S\cup T|\ =\ |S|\ +\ |T|\ where\ S\ and\ T\ are\ two\ disjoint\ finite\ sets$

2. **The Product Rule**
	Suppose that a procedure can be broken down into two tasks. If there are n<sub>1</sub> ways to do the first task and n<sub>2</sub> ways to do the second after the first task has been done, then there are n<sub>1</sub>n<sub>2</sub> ways to complete the procedure.
用集合表示就是: $|S\times T|\ =\ |S|\ \cdot \ |T|$

## The Inclusion-Exclusion Principle (Subtraction Rule) 容斥原理

$$
|A\cup B|=|A|+|B|-|A\cap B|
$$

## Tree Diagrams
在Counting中应用树，我们可以使用each branch表示可能的选择，用叶子表示可能的结果

>[!NOTE] Example
>![[Treediagramexample1.png]]

# 6.2 The Pigeonhole Principle

**[Theorem]** The pigeonhole principle states that if there are more pigeons than pigeonholes, then there must be at least one pigeonhole with at least two pigeons in it. 如果鸽子数量大于鸽笼，那么一定至少有一个鸽棚里面有至少两只鸽子

实际上，如果要将N objects放入k boxes中，那么一定会有至少一个box中保存了至少$\lceil \frac{N}{k}\rceil$ 个objects

>[!NOTE] An elegant application of pigeonhole principle
>Show that among any n+1 positive integers not exceeding 2n there must be an integer that divides one of the other integers n+1个不超过2n的正整数必定存在一个数是另一个数的整数倍
>**证明过程：**
>$$\begin{gather}
>Let\ n+1\ positive\ integers\ be\ a_1,a_2,...,a_{n+1},(1\le a_i\le 2n)\\
>Write\ a_i=2^{k_i}q_i,where\ k_i\ is\ a nonnegative\ integer\ and\ q_i\ is an\ odd\ integer\ less\ than\ 2n\\
>Since\ there\ are\ only\ n\ odd\ number\ less\ than\ 2n,\\ by\ the\ pigeonhole\ principle\ it\ follows\ that\ there\ exist\ integer\ i\ and\ j\ that\ q_i=q_j\\
>then\ a_i=2^{k_i}q_i\ ,\ a_j=2^{k_j}q_j\\
>Then\ we\ can\ conclude\ it
>\end{gather}
$$

> [!NOTE] Example 2
> ![[鸽笼原理例子2.png]]


**[Theorem]** Every sequence of n<sup>2</sup>+1distinct integers contains a subsequence of length n + 1 that is either strictly increasing or strictly decreasing.
**证明：**
$$\begin{gather}
Let\ the\ sequence\ be\ the\ a_1,a_2,...,a_{n^2+1}\\
Associate\ (i_k,d_k)\ to\ the\ term\ a_k\\其中i_k，d_k分别是从a_k开始的最大严格递增序列的长度和最大严格递减序列的长度\\
我们假定没有i_k和d_k能超过n+1，即(i_k,d_k)共有n\times n =n^2个组合\\
因为共有n^2+1个数，所以必定存在s\lt t,a_s,a_t使得(i_s,d_s)=(i_t,d_t).\\
如果a_s<a_t,那么就存在从a_s开始长度为i_t+1的严格递增序列（从a_t开始长度为i_t的子序列加上a_s）\\
因此我们可以得到矛盾contradiction,Q.E.D
\end{gather}
$$


# 6.3 Permulations and Combinations

## Permulations 排列
[Notation] P(n,r)
$$
P(n,r)=n(n-1)(n-2)...(n-r+1)=\frac{n!}{(n-r)!}
$$
In particular , $P(n,n)=n!$ $P(n,0)=1$
用函数来表示就是:
$$\begin{gather}
B=\{b_1,b_2,...,b_r\}\ ,\ A=\{a_1,a_2,...,a_n\}\\
P(n,r)\ is\ the\ number\ of\ injection(单射)\ from\ B\ to\ A 
\end{gather}
$$


## Combinations 组合
[Notation] C(n,r)
$$
\frac{n!}{{r!(n-r)!}}=n(n-1)(n-2)...(n-r+1)/r!
$$

$$\begin{gather}
A=\{a_1,a_2,...,a_n\},B=\{0,1\}\\
f\ is\ the\ function\ from\ A\ to\ B\ such\ that\ the\ image\ of\ r\ elements\ in\ A\ is\ 1\ \\ \Leftrightarrow An\ r-combination\ of\ A.\\ \\
C(n,r)=|\{f|f:A\to B\land r=|\{a|a\in A\land f(a)=1\}|\}|
\end{gather}
$$
用集合来表示就是: 势为n的集合的势为r的子集的个数

[Corollary 1] $$
C(n,r)=C(n,n-r)
$$
# 6.4 Binomial Coeffcients 二项式系数

$$
(x+y)^n=\sum_{j=0}^n C(n,j)x^{n-j}y^j
$$
[Corollary 1]
$$
\sum_{k=0}^{n}C(n,k)=2^n
$$
[Corollary 2]
$$
\sum_{k=0}^{n}(-1)^kC(n,k)=0
$$

[Theorem] PASCAL'S Identity
$$
C(n+1,k)=C(n,k)+C(n,k-1)
$$

> [!NOTE] PASCAL's triangle
> ![[Pasted image 20240410193551.png]]

[Theorem] 
$$
C(m+n,r)=\sum_{k=0}^rC(m,r-k)C(n,k)
$$

[Corollary 4] $$
C(2n,n)=\sum_{k=0}^nC^2(n,k)$$
[Theorem] $$
C(n+1,r+1)=\sum_{j=r}^nC(j,r)
$$
证明:
假设第 j+1 个元素被选上，但是j+1后面的所有元素都没被悬赏，则共有$C(j,r)$种选择，同理，j可从r选取到n，Q.E.D



# 6.5 Generalized Permutations and Combinations

