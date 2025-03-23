
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
\begin{array}l F_{Y|X}(y|x) & =\lim _{\delta \rightarrow 0^+} P\{Y\le y| x\lt X\ \le x+\delta\} \\ &
=\lim _{\delta \rightarrow 0^+} \frac{P\{x\lt X\le x+\delta, Y\le y\}}{P\{x\lt X\le x+\delta\}} \\ &
=\lim _{\delta \rightarrow 0^+}\frac{F(x+\delta, y)-F(x,y)}{F_X(x+\delta)- F_X(x)} \\ &
= \lim _{\delta \rightarrow 0^+} \frac{(F(x+\delta, y)-F(x,y))/\delta  }{(F_X(x+\delta)- F_X(x))/\delta   }
\\ & =\int_{-\infty}^y \frac{f(x,v)}{f_X(x)}dv
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

!!! info "这里的 $\rho$ 为 $X,Y$ 的相关性，具体在 Chapter 4 中涉及"

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

## 二元随机变量的函数分布

<font style="font-weight: 1000;font-size: 20px" color="orange">例 1：</font>

设 $X$ 的密度函数 $f(x)=\left \{ \begin{array}l e^{-x}, & x\gt 0 \\ 0, & x\le 0 \end{array} \right .$

令：

$$
U = \left \{\begin{array}l 1, x\gt 1 \\ 0, x\le 1\end{array} \right .
\ ,\ \ \ V=\left \{\begin{array}l 1, x\gt 2 \\ 0, x\le 2\end{array} \right .
$$

求 $(U,V)$ 的联合分布律

$$\begin{array}l
P(U=1,V=1)=P(X\gt 1,X\gt 2)=P(X\gt 2)=e^{-2} \\
P(U=1,V=0)=P(X\gt 1,X\le 2)=P(1\lt X\le 2)=e^{-1}- e^{-2} \\
P(U=0,V=1)=P(X\le 1,X\gt2)=0 \\
P(U=0,V=0)=P(X\le 1, X\le 2)=P(X\le 1)=1-e^{-1}
\end{array}
$$

<font style="font-weight: 1000;font-size: 20px" color="orange">例 2：</font>

设随机变量满足泊松分布：

$$
X\sim P(\lambda_1) ,\ \ Y\sim P(\lambda_2)
$$

且 $X,Y$ 相互独立。若 $Z=X+Y$，求 $Z$ 的概率分布律：

$$\begin{array}c
P(X=i)= \frac{\lambda_1^i e^{-\lambda_1}}{i!} \\
P(Y=j)= \frac{\lambda_2^j e^{-\lambda_2}}{j!} \\
\begin{array}l
P(Z=k)& =\sum ^k_{i=0} P(X=i)P(Y=k-i) \\
& =\sum ^k_{i=0} \frac{\lambda_1^i e^{-\lambda_1}}{i!} \frac{\lambda_2^{k-i} e^{-\lambda_2}}{(k-i)!} \\
& = \frac{e^{-(\lambda_1 +\lambda_2)}}{k!} \sum^k_{i=0} \frac{k!}{i!(k-1)!} \lambda_1^{i} \lambda_2^{k-i} \\
& =\frac{(\lambda_1+\lambda _2)^k e^{-(\lambda_1 +\lambda_2)}}{k!} \\
\end{array}
\\ \Rightarrow Z=X+Y\sim P(\lambda_1 +\lambda_2)
\end{array}
$$

<font style="font-weight: 1000;font-size: 20px" color="orange">例 3：</font>

设随机变量 $X\sim U(0,1)$，$Y$ 的密度函数为

$$
f_Y(y)=\left \{\begin{array}l 2y, & 0\lt y\lt 1 \\
0, & \text{其他}
\end{array}\right .
$$

且 $X$ 和 $Y$ *相互独立*。记 $M=\max \{X,Y\},N=\min \{X,Y\}$，分别求其密度函数：

$$\begin{array}c
F_X(x)=\left \{\begin{array}l 1, & 1\lt x\\
x, & 0\lt x \lt 1 \\
0, & x\lt 0
\end{array}\right . , \ \ \ \ 
F_Y(y)=\left \{\begin{array}l 1, & 1\lt y\\
y^2, & 0\lt y \lt 1 \\
0, & y\lt 0
\end{array}\right . \\
F_M(m)=P(M\le m)=P(X\le m,Y\le m)=F_X(m)\cdot F_Y(m) \\
\Rightarrow \ f_M(m) = F'_M(m)=\left \{\begin{array}l 3m^2, & 0\lt m\lt 1 \\
0, & \text{其他}\end{array} \right . \\
F_N(n)=P(\min \{X,Y\}\le n)=F_X(n)+ (1-F_X(n))\cdot F_Y(n) \\
\Rightarrow \ f_N(n)=F'_N(n)=\left \{ \begin{array}l 
1+2n-3n^2, & 0\lt n\lt 1 \\
0, & \text{其他}\end{array}\right .
\end{array}
$$



### 卷积公式

对于密度函数为 $f(x,y)$ 的连续型随机变量 $X,Y$ ，$Z=X+Y$ 的分布函数为：

$$\begin{array}l
F_Z(z)=P(Z\le z) & = \iint_{x+y\le z} f(x,y)dxdy \\
& =\int^{+\infty}_{-\infty}[ \int_{-\infty}^{z-y}f(x,y)dx]dy \\
\text{(Let u=x+y)}& =\int^{+\infty}_{-\infty} [ \int_{-\infty}^{z}f(u-y,y)du]dy \\
& = \int^z_{-\infty}[ \int_{-\infty}^{+\infty}f(u-y,y)dy]du \\
& = \int^z_{-\infty} f_Z(u)du
\end{array}
$$

故得出结论：

$$
f_Z(z)=\int_{-\infty}^{+\infty}f(z-y,y)dy = \int_{-\infty}^{+\infty}f(x,z-x)dx
$$

若 $X,Y$ 相互独立,则：

$$
f_Z(z)=\int_{-\infty}^{+\infty} f_X(z-y) f_Y(y)dy =\int_{-\infty}^{+\infty} f_X(x) f_Y(z-x)dx
$$

<font style="font-weight: 1000;font-size: 20px" color="orange">例 3：</font>

$$
f(x,y)=\left \{\begin{array}l 3x , & 0\lt y\lt x\lt 1\\0, & \text{其他} \end{array} \right .
$$

求 $Z=X+Y$ 的概率密度函数

$$\begin{array}c
f_Z(z)=\int ^{+\infty}_{-\infty} f(x,z-x)dx \\
f(x,z-x)=\left \{\begin{array}l 3x, & 0\lt z-x\lt x\lt 1 \\
0, & \text{其他}
\end{array} \right . \\
0\lt z-x\lt x\lt 1 \Leftrightarrow \frac{z}{2}\le x\le \min (z,1) \\
f_Z(z) = \left \{\begin{array}l 
\int ^z_{\frac{z}{2}} 3xdx = \frac{9}{8}z^2 , & 0\lt z\le 1 \\
\int ^1_{\frac{z}{2}} 3xdx = \frac{3}{2}(1- \frac{z^2}{4}), & 1\lt z\lt 2 \\
0, & \text{其他}
\end{array} \right .
\end{array}$$

!!! note "对谁积分就写出谁的取值范围，如该题的 $dx$，而上下限则是关于 $z$ 的函数"

<font style="font-weight: 1000;font-size: 20px" color="orange">例 4：</font>

相互独立的随机变量 $X\sim U(0,1), Y\sim U(0,1)$ ，求 $Z=X+Y$ 的概率密度函数：

**解答**：

（1）方法一，卷积公式：

$$\begin{array}c
f_Z(z)=\int _{-\infty}^{+\infty} f_X(x) f_Y(z-x) dx \\
\text{当且仅当：} \begin{cases} 0\le x\le 1 \\
0\le z-x\le 1\end{cases} \text{ 时被积函数不等于0}\\
f_Z(z)= \begin{cases}\int_0^z dx = z, & 0\le z\le 1 \\
\int _{z-1}^1 dx = 2-z, & 1\lt  z\le 2  \\
0, & \text{其他}\end{cases}
\end{array}$$

（2）方法二，分布函数定义(画图)：

![[方法2画图.png]]

- 当 $z\lt 0$ 时，$F_Z(z)=P(X+Y\le z) =0$
- 当 $z\ge 2$ 时，$F_Z(z)=P(X+Y\le z)=1$
- 当 $0\le z\le 1$ 时，$F_Z(z)= P(X+Y\le z)=\frac{1}{2}z^2$
- 当 $1\lt z\le 2$ 时，$F_Z(z)=P(X+Y\le z)= 1- \frac{1}{2}(1-(z-1)^2)$

对分布函数求导可得概率密度函数。