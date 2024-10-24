
# 多元随机变量及其分布

## 二维离散型随机变量

称二维离散型随机变量 $(X,Y)$ 的联合概率分布律如下：

$$
P\{X=x_i, Y=y_j\} =p_{ij}, \ \ i,j=1,2,...
$$

类似一维离散型随机变量的概率分布律的性质，易知二维离散型随机变量的联合分布律满足：

- <1> $p_{ij} \ge 0$
- <2> $\sum_i \sum_j p_{ij}=1$

不同的是，二维随机变量具有边际分布，即固定其中一个变量值不变，求另一个变量的分布律，此处不再列出。

## 二维随机变量的分布函数

二维随机变量的联合分布函数如下：

$$
F(x,y)=P\{X\le x, Y\le y\}
$$

!!! info "其关于 x 和 y 均右连续"

我们称二维随机变量中每一个分量的概率分布律为边际分布律，在此同样称每一个分量的分布函数为边际分布函数：

$$
F_X(x) = P\{X\le x\} =P\{X\le x, y\le +\infty\} = F(x, +\infty)
$$

同样，可以定义二维随机变量的条件分布函数如下：

$$
F_{Y|X}(y| x_i) =P\{Y\le y|X= x_i\}
$$

称为在给定 $X=x_i$ 情况下，$Y$ 的条件分布函数。

由于二维连续型随机变量的单个点概率为零，无法作为除数，通常需要写成一段区间的形式：

$$
F_{Y|X}(y| x) =\lim _{\delta \rightarrow 0^+} P\{Y\le y|x\lt X\le x +\delta\}
$$

## 二维连续型随机变量

### 联合分布

若存在二元非负函数 $f(x,y)$ ，使对任意实数 $x,y$ 有：

$$
F(x,y) = \int ^x_\infty \int _\infty ^y f(u,v) dudv
$$

则称 $(X,Y)$ 为二维连续型随机变量，$f(x,y)$ 为联合密度函数。

!!! note "在三维坐标中画出联合密度函数，则其余xy平面围成的面积为 1"

### 边际分布

单个随机变量 $X$ 的边际密度函数可由它的边际分布函数 $F_X(x)$ 求导得到：

$$\begin{array}c
F_X(x)=P\{X\le x\} = P\{X\le x,Y\in (-\infty,+\infty)\} = \int_{-\infty} ^x \left[\int _{-\infty}^{+\infty}f(x,y)dy\right]dx \\
\Rightarrow f_X(x) = \int _{-\infty}^{+\infty}f(x,y)dy
\end{array}
$$

即边际密度函数为联合密度函数关于另一个变量在 $(-\infty, +\infty)$ 上的积分。

### 条件分布

上面谈过，对于二维连续型随机变量的条件分布函数，需要写成一段区间形式来计算：

$$
F_{Y|X}(y|x) \begin{array}l =\lim _{\delta \rightarrow 0^+} P\{Y\le y| x\lt X\ \le x+\delta\} \\
=\lim _{\delta \rightarrow 0^+} \frac{P\{x\lt X\le x+\delta, Y\le y\}}{P\{x\lt X\le x+\delta\}} \\ 
=\lim _{\delta \rightarrow 0^+}\frac{F(x+\delta, y)-F(x,y)}{F_X(x+\delta)- F_X(x)} \\
= \lim _{\delta \rightarrow 0^+} \frac{(F(x+\delta, y)-F(x,y))/\delta  }{(F_X(x+\delta)- F_X(x))/\delta   }
\\ =\int_{-\infty}^y \frac{f(x,v)}{f_X(x)}dv
\end{array}
$$

对其关于 $y$ 求导，得到给定 $\{X=x\}$ 条件下，$Y$ 的条件密度函数为：

$$
f_{Y|X}(y|x) = \frac{f(x,y)}{f_X(x)}
$$

### 二元均匀分布和二元正态分布

#### 均匀分布

设二维随机变量 $(X,Y)$ 在二维有界区间 $D$ 上取值，且具有联合密度函数：

$$
f(x,y) =\left\{ \begin{array}l 
\frac{1}{S_D} , & (x,y) \in D
\\ 0, & \text{其它}
\end{array}   \right .
$$

则称 $(x,y)$ 服从 $D$ 上均匀分布。

#### 正态分布

设二维随机变量 $(X,Y)$ 具有联合密度函数：

$$
f(x,y)=\frac{1}{2\pi \sigma_1 \sigma_2\sqrt{1-\rho ^2}}exp\{- \frac{1}{2(1-\rho ^2)}\left[ \frac{(x-\mu_1^2)}{\sigma _1^2} - 2\rho \frac{(x-\mu_1) (y-\mu_2)}{\sigma_1\sigma _2}+ \frac{(y-\mu_2)^2}{\sigma _2^2} \right]\}
$$

其中 $|\rho| \lt 1$ ，则称 $(X,Y)$ 服从参数为 $(\mu_1, \mu_2; \sigma_1, \sigma_2;\rho)$ 的二元正态分布，记为 $(X,Y) \sim N(\mu_1, \mu_2; \sigma_1, \sigma_2;\rho)$

### 随机变量的独立性

对任意两个实数集合 $D_1,D _2$ ，若

$$
P\{ X\in D_1, Y\in D_2 \} = P\{X\in D_1\}\cdot P\{Y\in D_2\}
$$

则称随机变量 $X,Y$ 相互独立，其一般写成如下形式：

$$\begin{array}c
P\{ X\le x, Y\le y \} = P\{X\le x\}\cdot P\{Y\le y\}
\\ F(x,y) = F_X(x)\cdot F_Y(y)
\end{array}
$$

实际上，由微积分知识可以计算得到：

$$
f(x,y) = f_X(x)\cdot f_Y(y)
$$

这条等式与连续型随机变量 $X,Y$ 相互独立等价。

另有定理，二维连续型随机变量 $X,Y$ 相互独立的充要条件是 $X,Y$ 的联合密度函数几乎处处可写成 $x$ 的函数 $m(x)$ 与 $y$ 的函数 $n(y)$ 的乘积，即：

$$
f(x,y) =m(x)\cdot n(y), \ \ -\infty \lt x\lt +\infty, \ -\infty \lt y\lt +\infty 
$$

