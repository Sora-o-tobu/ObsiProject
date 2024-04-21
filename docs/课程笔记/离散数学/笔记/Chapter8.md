# CH 8 : Advanced Counting Techniques
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

### 不含常量的解法 Homogeneous

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


例如:
> (1) $a_n=(1.02) a_{n-1}$ linear;constant coeffcients;homogeneous;degree 1.
> 
> (2) $a_n= a_{n-1}+2^n$ linear;constant coeffcients;**nonhomogeneous**;degree 1.
> 
> (3) $a_n = na_{n-1}+ n^2a_{n-2}+ a_{n-1} a_{n-2}$  **nonlinear**;**not constant**;homogeneous;degree 2.


解法:不动点法，高中教过，相信都懂!
此处仅给出一例:
![[不动点法.png]]

### 含常量的解法 Nonhomogeneous

这一部分课本上的例题非常好，推荐去看。

- **Definition**
    - $a_n =c_1 a_{n-1}+ c_2 a_{n-2}+ ...+c_k a_{n-k} +F(n)$
    - $c_k\ne 0,c_k\in R$
    - $F(n)$ 非零，且只由$n$决定
- **Solve**: 找特解 $a_n^{(p)}$(Particular)，再求不含 $F(n)$ 的递推的通解 $a_n^{(h)}$，则通解为 $a_n^{(p)} + a_n^{(h)}$
    - 特别地，当 $F(n)$ 的形式是$(b_tn^t +b_{t-1}n^{t-1} +...+b_1n+ b_0)s^n,\ b_i,s\in R$
        - (Case 1) $s$不为特征根，则特解为 $(p_tn^t +p_{t-1}n^{t-1} +...+p_1n+ p_0)s^n$
        - (Case 2) $s$为特征根且重复了$m$次，则特解为$n^m (p_tn^t +p_{t-1}n^{t-1} +...+p_1n+ p_0)s^n$
    - 求解的时候注意特解的形式：取到哪个幂次

> 例题1
> ![[含常量例题1.png]]


--- 


> 例题2
> ![[含常量例题2.png]]
> 注意，可以将 $n$ 看作 $n*1^n$ ，则$s$取1，$m$取1，$n$转换成一次的一般形式$p_1n^2 +p_o$



## 8.4 Generating Functions

生成函数的本质是处理排列组合的有效工具。

- **Definition**
    - For sequence $a_0, a_1, a_2, ...,a_k,...$，its generating function is: $G(x)=a_o +a_1x+ ...+a_kx^k+ ...=\sum_{i=0}^{\infty} a_kx^k$
    - eg: $1+x+...+x^k= \sum_{k=0}^\infty x^k =\frac{1}{1-x},|x|<1$


![[生产函数table_webp.webp]]

**重要等式**

若$b_k= \sum_{i=0}^k a_i\ \ a_k\leftrightarrow G(x)\ \ b_k\leftrightarrow F(x)$，则$F(x)=\frac{G(x)}{{1-x}}$

$C_{-n}^k =(-1)^k C_{n+k-1}^k$

### Counting with Generating Functions

#### r-combinations from a set with n elements with repetitions allowed

在term中，$x^i$代表这个元素有被选了i次，共有n个元素，因此我们:
$G(x)=(1+x+x^2 +x^3+... )^n= (\frac{1}{{1-x}})^n =\sum_{k=0}^\infty C_{n+k-1}^k x^k$
其中我们要找的答案就是$x^r$的系数$C_{n+r-1}^r$

[Example](#)
Find the number of solutions of $e_1+ e_2+ e_3 =17\ \ where\ 2\le e_1\le 5,3\le e_2\le 6,4\le e_3\le 7$

$G(x)=(x^2+ x^3 +x^4+ x^5)( x^3 +x^4+ x^5+ x^6)( x^4+ x^5 +x^6+ x^7)$

答案就是$x^{17}$的系数。

??? note "如何快速求系数?"
	在这里我们以$G(x)=(1+x+...+x^{2r} )^3$求$x^{3r}$的系数为例:
	
	$G(x)=(\frac{1-x^{2r+1}}{1-x})^3 =\frac{1-3x^{2r+1}+ 3x^{4r+2}- x^{6r+3}}{(1-x)^3}$
	
	令$F(x)=\frac{1}{(1-x)^3}=\sum C_{i+2}^{i} x^i$，因为$G(x)=({1-3x^{2r+1}+ 3x^{4r+2}- x^{6r+3}})F(x)$只需要找到i=3r和i=r-1(2r+1+i=3r)的情况即可
	
	因此，$a^{3r}=C_{3r+2} ^{3r}-3C_{r+1} ^{r-1}$


