# Chapter 8 Advanced Counting Techniques
## 8.1 Applications of Recurrence Relations
递归关系的应用

[Definition] A **recurrence relation** is:
$$
\forall n\ge n_0,\ \ a_n=f( a_0, a_1, a_2,..., a_{n-1}) 
$$

!!! 常见的递归关系
	(1) The Fibonacci sequence $a_n= a_{n-1} + a{n-2}$
	(2) Pascal's recursion for binomial coeffcient: $C_{n+1}^k = C_n^k + C_n^{k-1}$
	(3) The Tower of Hanoi: $H_1=1; H_n= 2H_{n-1}+1$

## 8.2 Solving Linear Recurrence Relations

**Linear homogeneous recurrence relation of degree k with constant coeffcients**
$$
a_n= c_1 a_{n-1}+ c_2 a_{n-2}+...+ c_k a_{n-k}
$$

- **Linear:**
linear Combination of previous terms

- **Constant Coeffcients:**
The coeffcient of a<sub>i</sub> are constants

- **Degree k:**
$a_n$ is a function of previous k terms of the sequence

- **Homogeneous:**
不含常量

!!! note
	(1) $a_n=(1.02) a_{n-1}$ linear;constant coeffcients;homogeneous;degree 1.
	(2) $a_n= a_{n-1}+2^n$ linear;constant coeffcients;**nonhomogeneous**;degree 1.
	(3) $a_n = na_{n-1}+ n^2a_{n-2}+ a_{n-1} a_{n-2}$  **nonlinear**;**not constant**;homogeneous;degree 2.


解法:不动点法，高中教过，相信都懂!
此处仅给出一例:
![[不动点法.png]]


