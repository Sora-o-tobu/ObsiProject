
# 随机变量的数字特征

![[suijibianliangshuzitezhengbiao.png]]

## 数学期望

### 定义

数学期望简称期望，又称均值。

=== "离散型"
	设离散型随机变量 $X$ 的分布律为 $P(X= x_k)= p_k$ ，若级数 $\sum _{k=1}^{+\infty} |x_k| p_k <\infty$ ，则称变量的数学期望 $E(X)$ 为：
	$$E(X)=\sum _{k=1}^{+\infty} x_k p_k$$
=== "连续型"
	设连续型随机变量 $X$ 的概率密度函数为 $f(x)$，若积分 $\int _{-\infty}^{+\infty} |x|f(x)dx <\infty$ ，则称变量的数学期望 $E(X)$ 为：
	$$E(X)=\int _{-\infty}^{+\infty} xf(x)dx$$

### 随机变量函数的数学期望

!!! warning "离散型变量懒得说了"

设 $Y=g(X)$，且 $\int _{-\infty}^{+\infty} |g(x)|f(x)dx <\infty$ ，则有：

$$
E(Y)=E(g(X)) = \int _{-\infty}^{+\infty} g(x)f(x)dx
$$

!!! note "应用该定理，就不需要专门求出 $Y$ 的分布律或密度函数"

**拓展到多元随机变量：**

设 $Z=h(X,Y)$ ，二元型随机变量 $(X,Y)$ 的密度函数为 $f(x,y)$，$E(Z)$ 存在，则有：

$$
E(Z)=E(h(X,Y))=\int _{-\infty}^{+\infty} \int _{-\infty}^{+\infty} h(x,y)f(x,y)dxdy
$$

!!! info "数学期望的性质"
	设 $X,Y$ 相互独立，则有 $E(XY)=E(X)E(Y)$

## 方差

### 定义

方差 $Var(X)$ 刻画了变量取值的分散程度，若取值较集中，则 $Var(X)$ 较小。

设 $X$ 是随机变量，若 $E\{[X-E(X)]^2\}$ 存在，则称方差 $Var(X)$ 或 $D(X)$ 为：

$$
Var(X)=E\{[X-E(X)]^2\}
$$

将 $\sqrt{Var(X)}$ 记为 $\sigma(X)$ ，称为变量 $X$ 的**标准差**或**均方差**，它与 $X$ 具有相同的量纲。

=== "离散型"
	$$Var(X)=\sum _{i=1}^{\infty} [x_i-E(X)]^2 p_i$$
=== "连续型"
	$$Var(X)=\int _{-\infty}^{+\infty} [x-E(X)]^2f(x) dx$$

实际上有重要计算公式：

$$
Var(X)=E(X^2) -E^2(X)
$$

??? example "泊松分布方差的计算"
	![[泊松分布方差计算.png]]

### 性质

- <1> 若 $C$ 是常数，则 $Var(C)=0$
- <2> 设 $X$ 是随机变量，$C$ 是常数，则有 $D(CX)=C^2D(X)$
- <3> 设 $X,Y$ 是两个随机变量，则有 $$Var(X+Y)=Var(X)+Var(Y)+2E\{[X-E(X)][Y-E(Y)]\}$$
	- 特别的，若 $X,Y$ 相互独立，则有 $D(X+Y)=D(X) +D(Y)$


综合以上三项，可以有如下定理：

设 $X,Y$ **相互独立**，$a,b,c$ 是常数，则

$$
Var(aX+bY+c)=a^2Var(X) +b^2Var(Y)
$$

!!! note "该定理可推广到任意有限个独立随机变量的线性组合"

!!! tip "标准化变量"
	若随机变量 $E(X)=\mu, Var(X)=\sigma^2$ ，记 $X$ 的标准化变量 $X^*= \frac{X-\mu}{\sigma}$ 。
	
	显然有 $E(X^*)=0, Var(X^*)=1$

## 协方差

### 定义

定义随机变量 $X,Y$ 之间的协方差 $Cov(X,Y)$ 为：

$$
Cov(X,Y)=E\{[X-E(X)][Y-E(Y)]\}
$$

则我们易得下面两条重要等式：

$$\begin{array}l
Cov(X,Y)=E(XY)-E(X)E(Y) \\
Var(X+Y) =Var(X)+Var(Y) +2Cov(X,Y)
\end{array}
$$

### 性质

- <1> $Cov(X,Y)=Cov(Y,X)$
- <2> $Cov(X,X)=Var(X)$
- <3> $Cov(aX,bY)=ab\cdot Cov(X,Y)$
- <4> $Cov(X_1 +X_2,Y)=Cov(X _1,Y) +Cov(X_2, Y)$
- <5> 当 $Var(X)\cdot Var(Y)\ne 0$ 时，有 $(Cov(X,Y))^2\le Var(X)Var(Y)$
	- 其中等号当且仅当 $X,Y$ 之间有严格线性关系，即存在常数 $a,b$ 使得 $P(Y=a+bX)=1$
- <6> $X,Y$ 独立，则 $Cov(X,Y)=0$

!!! note "均匀分布的协方差一般为0，若区域是一个与坐标轴平行的矩形，则二者相互独立"

## 相关系数

定义 $X,Y$ 的相关系数 $\rho_{XY}$ 为：

$$
\rho _{XY}= \frac{Cov(X,Y)}{\sqrt{Var(X)\cdot Var(Y)}}
$$

- <1> $|\rho_{XY}| \le 1$
- <2> $|\rho_{XY}|=1$ 等效于 存在常数使得 $P(Y=a+bX)=1$
	- 若 $\rho_{XY}=1$，则 $b\gt 0$
	- 若 $\rho_{XY}=-1$，则 $b\lt 0$

相关系数 $\rho_{XY}$ 是一个用来表征 $X,Y$ 之间线性关系紧密程度的量

- 当 $|\rho_{XY}|$ 较大时， $e(a_0, b_0)$ 较小， 表明 $X,Y$ 线性关系的程度较好；
- 当 $|\rho_{XY}|=1$ 时， $e(a_0, b_0)=0$ ，表明 $X,Y$ 之间以概率1存在线性关系；
- 当 $|\rho_{XY}|$ 较小时， $e(a_0, b_0)$ 较大， 表明 $X,Y$ 线性关系的程度较差；

其中，若 $\rho_{XY}\gt 0$，称 $X,Y$ 正相关；若 $\rho_{XY}\lt 0$，称 $X,Y$ 负相关；若 $\rho_{XY}= 0$，称 $X,Y$ 不相关或零相关。

随机变量 $X,Y$ 不相关，即 $\rho_{XY}=0$ 的等价条件有：

- <1> $Cov(X,Y)=0$
- <2> $E(XY)=E(X)E(Y)$
- <3> $Var(X+Y)=Var(X) +Var(Y)$

!!! danger "独立一定不相关，不相关不一定独立"
	不过在**联合正态分布**下，$X,Y$ 不相关等价于相互独立。


## 特别探究：n元联合正态分布

n元正态分布的概率密度函数由各元均值和协方差矩阵决定。

- n元正态 $\Rightarrow$ 边际正态
- 边际正态 && 相互独立 $\Rightarrow$ n元正态
- n元正态的线性组合也符合n元正态
- $X_1, X_2, ...,X_n$ 相互独立 $\Leftrightarrow$ $X_1, X_2, ...,X_n$ 两两不相关 $\Leftrightarrow$ 协方差矩阵为对角矩阵

!!! example ""
	=== "题面"
		![[nyuanlianhezhengtaitimian.png]]
	=== "(1)解答"
		![[nyuanzhengtaijieda1.png]]
	=== "(2)解答"
		![[nyuanzhengtaijieda2.png]]
	=== "(3)解答"
		![[nyuanzhengtaijieda3.png]]


