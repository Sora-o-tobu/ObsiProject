# Chapter 5 : Induction and Recursion 归纳与递归
# 5.1 Mathematical Induction 数学归纳法
To prove  $\forall n\ P(n)$ by mathematical induction:
	(1) **Basic Step:** Establish P(1)
	(2) **Inductive Step:** Prove that $P(k)\to P(k+1)\ for\ k\ge 1$
	(3) **Conclusion:** $\forall n\ P(n)$ , where the domain is the set of positive integers
**[Theorem]** The first principle of Mathematical Induction:
$$
(p(1)\land \forall k(p(k)\to p(k+1)))\to \forall n\ p(n)
$$
# 5.2 Strong Induction and Well-ordering Property
## Strong Induction 强归纳法、归纳法第二原理
To prove  $\forall n\ P(n)$ by strong induction:
	(1) **Basic Step:** Establish $P(n_0)$
	(2) **Inductive Step:** Prove that $P(n_0)\land P(n_0+1)\land ... \land P(k)\to P(k+1)$
	(3) **Conclusion:** $\forall n\ge n_0\ P(n)$ 

**[Theorem]** The second principle of Mathematical Induction:
$$
(P(n_0)\land \forall k\ge n_0\ (P(n_0)\land P(n_0+1)\land ... \land P(k)\to P(k+1)))\to \forall n\ge n_0 (P(n))
$$


## Well-ordering Property 良序性原理
The validity of mathematical induction follows from the well-ordering property for the set of ==positive integers==.

**[Theorem]** A set S is well ordered  if every nonempty subset of S has a least element.
一个集合S是良序的当它任何一个非空子集都有最小的元素

>[!NOTE] Example
>(1) N is well ordered
>(2) Z is not well ordered under $\le$ relation(Z has no smallest element)
>(3) (0,1) is not well ordered since (0,1) does not have a least element

对于由非负整数构成的集合，总存在最小的元素，因此良序性成立

> [!NOTE] Note!
> 1. The validities of both mathematical induction and strong induction follow from the well-ordering property. (良序性原理是前提)
> 2. 1.In fact, mathematical induction, strong induction, and  well-ordering are all equivalent principles.

# 5.3 Recursive Definitions and Structural Induction 递归定义和结构归纳法

**[Example]** 求最大公约数
	[Lemma 1] Let a = bq + r , then gcd(a,b) = gcd(b,r).
	$$\begin{gather}
	Suppose\ that\ a\ and\ b\ are\ positve\ integers\ with\ a\ge b.\\
	Let\ r_0=a\ and\ r_1=b.
	\\ r_0=r_1q_1+r_2,\ 0\le r_2\lt r_1;
	\\ r_1=r_2q_2+r_3,\ 0\le r_3\lt r_2;
	\\ ......
	\\ r_{n-2}=r_{n-1}q_{n-1}+r_{n},\ 0\le r_{n-1}\lt r_{n};
	\\ r_{n-1}=r_nq_n.
	\\ It\ follows\ Lemma\ 1\ that\ \\
	gcd(a,b)\ =\ gcd(r_0,r_1)\ =\ gcd(r_1,r_2)\ =...=\ gcd(r_{n-1},r_{n})\ =\ gcd(r_n,0)\ =\ r_n  
\end{gather}$$


> [!NOTE] Table
> ![[三种归纳的对比.png]]


# 5.4 Recursive Algorithms

**Recursive algorithm vs. iterative algorithm**
1. 对于任何递归算法，都有相应等价的迭代算法
2. 递归算法通常更加短、优雅、并且易于理解
3. 迭代算法通常比递归在时间空间的运用效率上更优秀

