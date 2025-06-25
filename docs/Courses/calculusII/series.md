
# 级数

!!! quote
	![[常见展开.png]]
	
	- 另外补充一个 $\arctan x=\sum_{n=0}^\infty (-1)^n\frac{x^{2n+1}}{2n+1}$


## 柯西收敛准则
$$\begin{gather}
\forall \varepsilon \gt 0,\exists N,当n\gt N时,对一切自然数P都有 \\
|U_{n+1}+ U_{n+2}+...+U_{n+P}|\lt \varepsilon
\end{gather}
$$

由此逆否命题得到**级数发散的充要条件** 为：

$$
\begin{gather}
\exists \varepsilon_0 \gt 0,\forall N,\exists n\gt N时,\exists P\in Z^+都有 \\
|U_{n+1}+ U_{n+2}+...+U_{n+P}|\ge \varepsilon_0
\end{gather}
$$

## 莱布尼茨定理及其证明

若交错级数 $\sum_{n=1}^\infty (-1)^nu_n$ 满足下列条件：

1. $u_1\ge u_2 \ge u_3\ge ... \ge 0$
2. $\lim_{n\rightarrow \infty}{u_n}=0$

则 $\sum_{n=1}^\infty (-1)^nu_n$ 收敛，并且它的和 $S\le u_1$ ；以 $S_n$ 为级数和的近似值时，其误差 $R_n$ 不超过 $u_{n+1}$ ，即 $|R_n|=|S- S_n|\le u_{n+1}$


**【Prove】**

$$\begin{array}c
0\le S_{2m}=(u_1-u _2)+(u _3-u _4)+...+(u _{2m-1}-u _{2m}) \\
S_{2m+2}=S _{2m}+( u_{2m+1}- u_{2m+2}) \ge S_{2m} \\
并且S_{2m}=u_1-(u _2- u _3)-...-(u _{2m-2}-u _{2m-1})-u _{2m}\le u_1 \\
即数列 S_{2m} 单调递增有上界，所以其极限存在，记为 S \\
则 \lim_{m\rightarrow\infty} S_{2m+1}= \lim_{m\rightarrow\infty} (S_{2m}+u _{2m+1})=S+0 \\
\Rightarrow \ \lim_{n\rightarrow \infty} S_n =S \\ 
级数收敛
\end{array}
$$

## 有阶乘考虑 $e^x$

![[有阶乘考虑ex.png]]


无数个有理数相加可以是：

- 不是一个数： $\sum n$
- 有理数： $\sum\frac{1}{2^n}$
- 无理数： $\sum\frac{(-1)^n}{n}=\ln 2$


## 重要级数

![[重要级数.png]]

## 推广判别法

### Dirichlet 判别法

若数列 $\{a_n\}$ 单调且 $\lim_{n\rightarrow\infty}a _n=0$ ，级数 $\sum_{n=1}^\infty b_n$ 的部分和有界，则 $\sum_{n=1}^\infty a_n b_n$ 收敛

### Abel 判别法

若数列 $\{a_n\}$ 单调有界，级数 $\sum_{n=1}^\infty b_n$ 收敛，则 $\sum_{n=1}^\infty a_n b_n$ 收敛

!!! info "Abel变换"
	以上两个定理的证明需要用到Abel变换：
	$\sum_{k=1}^{n} a_k b_k =a_n B_n + \sum_{k=1}^{n-1} (a_k -a_{k+1}) B_k$ ，其中 $B_k =\sum_{n=1}^k b_n$
	
	此变换证明要点在于将 $b_k= B_k -B_{k_1}$ 代入

## 收敛半径、收敛区间、收敛域

- $(-r,r)$ 为 ==收敛区间==
- $(-r,r)$ 加上收敛端点为 ==收敛域==

!!! info
	无限多个连续函数的和不一定是连续函数，例如：
	
	$S(x)=x+\sum_{n=2}^\infty (x^n-x ^{n-1})$ 在 $x\in (-1,1)$ 时等于0，在 $x=1$ 时等于1

!!! danger "求和函数请注意!"
	求和函数的时候一定记得专门标注不存在的点。例如在有 $1/x$ 的时候，要写出 $S(x)$ 在 $x=0$ 时，等于 0。

## 幂级数展开

$f(x)$ 在区间 $|x-x_0|\lt R$ 内存在任意阶导数，幂级数 $\sum_{n=0}^\infty \frac{f^{(n)}(x_0)}{n!}(x-x _0)^n$ 的收敛区间为 $|x-x_o|\lt R$ ，则在 $|x-x_o|\lt R$ 内， $f(x)=\sum_{n=0}^\infty \frac{f^{(n)}(x_0)}{n!}(x-x _0)^n$ 的**充要条件**是：

$$
\lim_{n\rightarrow \infty} R_n(x) = \lim_{n\rightarrow \infty} \frac{f^{(n+1)}(\varepsilon)}{(n+1)!}(x-x _0)^{n+1}=0
$$

### 幂级数展开的步骤

- Step 1: 计算 $f^{(n)}(x_0),n=1,2,3,...$
- Step 2: 写出对应的泰勒级数 $\sum_{n=0}^\infty \frac{f^{(n)}(x_0)}{n!}(x-x _0)^n$ ，并求出收敛区间
- Step 3: 验证 $|x-x_0|\lt R$ 时，$\lim_{n\rightarrow \infty} R_n(x) =0$
	- 特别地，$\lim_{n\rightarrow \infty} \frac{|x|^{n+1}}{(n+1)!}=0$
- Step 4: $f(x)=\sum_{n=0}^\infty \frac{f^{(n)}(x_0)}{n!}(x-x _0)^n\ \ |x-x_o|\lt R$
- Step 5: ==验证端点收敛性==

## 傅里叶级数公式

$$\begin{gather}
a_n=\frac{1}{l}\int^l _{-l}f(x)\cos \frac{n\pi x}{l}dx\ \ (n=0,1,2,...) \\ b_n=\frac{1}{l} \int^l_{-l}f(x)\sin \frac{n\pi x}{l}dx\ \ (n=1,2,3,...)
\end{gather}
$$

### 傅里叶展开的步骤

- Step 1: 求出最小正周期 T 和半周期 l
- Step 2: 判定 $f(x)$ 奇偶性，简化系数计算，并求出 $a_n\ b_n$
- Step 3: 得到傅里叶级数(此时还不能取等，用 `~` 连接)
- Step 4: 判定满足迪利克雷定理的条件（$f(x)$ 连续或只有有限个第一类间断点与极值点）

!!! tip "三角函数傅里叶展开就是它自己"

!!! note "中心点不在原点也正常求即可"
	![[中心点不在原点也正常求即可.jpeg]]


## 帕塞瓦不等式

若 $f(x)$ 可积且平方可积，则 $f(x)$ 的傅里叶级数系数 $a_n,b _n$ 的平方所构成的级数 $\frac{a_0^2}{2} +\sum_{n=1}^\infty (a_n^2 +b_n^2)$ 收敛，且等于 $\frac{1}{l} \int^l_{-l} f^2(x)dx$


## 展开为傅里叶级数后再求和函数，可能与原函数并不相同

![[展开为傅里叶级数然后和回去.webp]]


## 一些课后题 & 历年题

![[好解！.png]]

![[n方分之一和的求法.png]]

![[好题3.png]]

![[好题4.webp]]

![[好题5.webp]]
