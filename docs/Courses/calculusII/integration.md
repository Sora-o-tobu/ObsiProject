
# 积分

## 如何解常微分方程？

已知 $f(x)=f'(x)-2x$，求 $f(x)$

$$\begin{gather}
(\frac{f(x)}{e^x})'=\frac{f'(x)-f(x)}{e ^x}=\frac{2x}{e ^x} \\
\int\frac{2x}{e^x}dx=-\int2xde ^{-x}=-2xe ^{-x}+2\int e^{-x}dx \\
=-2xe^{-x}-2 e^{-x} \\
f(x)=-2x-2+te^x\ \ \ \ \ t\in R
\end{gather}
$$

!!! info "注意积分出来是有常数的"

## 积分区域为正方形也可以用极坐标

 $D_2=\{(x,y)|1\le x^2 +y^2,|x|+|y|\le 2\}$ ，求二重积分 $\iint_ {D_2} \frac{1}{(x^2 +y^2 )^\frac{3}{2} }dxdy$

**【解答】** 原式= 

$$4\int^\frac{\pi}{2}_0d\theta \int^{\frac{2}{\sin \theta+\cos\theta}}_1 \frac{1}{r^2}dr=4\int^\frac{\pi}{2}_0(1-\frac{\sin\theta+\cos\theta}{2})d\theta=2\pi-4$$

## 三重积分柱面坐标变换

- $x=rcos\theta,y=rsin\theta,z=z\ \Rightarrow \ 柱面坐标(r,\theta,z)$
- $\Delta V =S_{扇形}\Delta z = r\Delta \theta \Delta r\Delta z$

## 三重积分球面坐标变换

规定如下：

- $\rho$：原点到M点的距离 $0\le \rho \le +\infty$
- $\phi$：矢量 $\overrightarrow{OM}$ 与Oz轴正向的夹角 ==纬度== $0\le \phi \le \pi$
- $\theta$：矢量 $\overrightarrow{OM}$ 在xOy平面上的向量与x轴的夹角 ==经度== $0\le \theta \le 2\pi$

$$\begin{gather}
x=\rho \sin\phi \cos \theta \\
y=\rho \sin\phi \sin \theta \\
z=\rho \cos \phi
\end{gather}
$$

根据三重积分的一般曲面坐标变换：

$$\frac{\partial(x,y,z)}{\partial (\rho,\phi,\theta)}=\left |
\begin{matrix}\sin\phi \cos \theta & \sin\phi \sin \theta & \cos \phi  \\ \rho \cos \phi \cos\theta & \rho\cos\phi\sin\theta & -\rho\sin\phi \\ -\rho\sin\phi\sin\theta & \rho\sin\phi\cos\theta & 0\end{matrix} \right | =\rho^2 \sin\phi
$$

所以 $\Delta V=\rho ^2 \sin\phi d\rho d\phi d\theta$

## 积分的时候注意等价量，可以大大减少运算量

![[注意等价量，减少运算量.png]]

![[注意等价量减少运算量2.png]]

## 第一类曲线积分
$$\begin{gather}
\Delta s=\sqrt{[x(t+\Delta t )-x(t)]^2 +[y(t+\Delta) t -y(t)]^2 +[z(t+\Delta t )-z(t)]^2} \\
=\sqrt{x'^2(t) +y'^2(t) +z'^2(t)}\Delta t
\end{gather}
$$

??? example
	![[第一类曲线积分例子1.png]]

### 平面极坐标方程

![[平面极坐标变换.png]]

## 第一类曲面积分

$$
\int\int_S f(x,y,z)dS=\int\int_{ \sigma_{xy}}f(x,y,z(x,y)) \sqrt{1+z'^2_x +z'^2_y} d\sigma
$$

- 若曲面由隐函数 $F(x,y,z)=0$ 确定，由于
	- $\frac{\partial z}{\partial x}=-\frac{F'_x}{F'_z}$
	- $\frac{\partial z}{\partial y}=-\frac{F'_y}{F'_z}$
- 得到：

$$
\int\int_S f(x,y,z)dS=\int\int_{ \sigma_{xy}}f(x,y,z(x,y)) \frac{\sqrt{F'^2_x +F'^2_y +F'^2_z}}{ |F'_z|} d\sigma
$$

??? example
	![[第一类曲面积分例子.png]]
	!!! danger "投影区域复习： 消去z，然后联立 `z = 0`"


## 引力

![[引力积分求法.png]]


!!! danger
	求重心求引力之类的，要注意题目里给的是体还是壳 (例如抛物面壳，就要用二重积分而不是三重积分)


## 也可以利用积分的物理意义解题

![[也可以利用积分的物理意义解题.jpeg]]

## 如何区分第一第二类积分

被积元素为向量则是第二类曲线积分，不是就是第一类

## 格林公式

边界曲线 $\Gamma$ **正向**规定：当人沿边界行走时，区域 $D$ 总在他左边。与上述方向相反，则称为**负方向**，记为 $-\Gamma$

若函数 $P、Q$ 在闭区域 $D\subset R^2$ 上连续且具有连续一阶偏导数，则

$$
\iint_D ( \frac{\partial Q}{\partial x}-\frac{\partial P}{\partial y})dxdy=\oint_\Gamma Pdx+Qdy
$$

这里 $\Gamma$ 为区域D的边界曲线，并取正向。

!!! danger
	历年卷中曾经考过证明格林公式，记住证明要点在于，将第二类曲线积分分成四段，用参数方程的形式加起来即可

??? example "Example 1"
	![[格林公式例子1.png]]

??? example "Example 2"
	![[格林公式例子2.png]]

!!! info "此外，格林公式还可以用来计算平面区域的面积 $S$"
	$$\begin{array}l\oint_\Gamma -ydx+xdy=\iint_D[1-(-1)]dxdy=2S\\ 其中\Gamma 是区域D边界曲线正向 \end{array}$$


## 平面连通区域 & 路径无关性

平面区域 $D$ 内任一封闭曲线所包围的区域包含于 $D$ 内，即没有洞的区域称为**单连通区域**，否则称为**复连通区域**。

设 $D\subset R^2$ 是单连通区域，若函数 $P、Q$ 在 $D$ 内连续，且有一阶连续偏导数 (可微的充分条件) ，则以下四个条件等价：

- 沿 $D$ 中任意光滑的闭曲线 $L$，有 $\oint_LPdx+Qdy=0$
- 对 $D$ 中任意光滑的曲线 $L$，曲线积分 $\int_L Pdx+Qdy$ 与路线无关，只与 $L$ 的终点起点有关
- $Pdx+Qdy$ 是某个函数 $u$ 的全微分，即在 $D$ 内存在一个二元函数 $u$，使得 $du=Pdx+Qdy$
- 在 $D$ 内每一点处，有 $\frac{\partial Q}{\partial x}=\frac{\partial P}{\partial y}$

## 曲线积分第一定理

设 $D\subset R^2$ 是闭区域，若函数 $P(x,y),Q(x,y)$ 在 $D$ 内连续，假设曲线积分 $\int_LPdx+Qdy$ 与路径无关，取一定点 $A(x_0, y_0)\in D$ ，对于任意点 $B(x, y)\in D$ ，称：

$u(x,y)=\int^{B(x,y)}_{A(x _0,y _0)}Pdx+Qdy =\int^x_{x _0}P(x,y _0)dx+ \int^y_{y _0}Q(x,y)dy+C$ 是$Pdx+Qdy$的一个**原函数** (也称势函数) ，且 $du=Pdx+Qdy\ \ \ grad(u)=(P,Q)$，称 $\nabla u=(P,Q)$ 为**保守场**或**梯度场**。

!!! info "若$(0,0)\in D$ ，原函数简化可写成 $\int^x_0P(x,0)dx+ \int^y_0Q(x,y)dy+C$"

??? example
	![[曲线积分第一定理例子.png]]


### 有时候可以直接用全微分解

![[有时候可以直接用全微分解.png]]

### 利用曲线积分和路径相关性可以求解微分方程

![[利用曲线积分和路径相关性可以求解微分方程.png]]


## 定理10.4

在**复连通区域** $D$ 内，$P,Q$ 具有连续的偏导数且 $\frac{\partial P}{\partial y}=\frac{\partial Q}{\partial x}$ ，则环绕同一些洞的任何两条闭曲线(同方向)上的曲线积分均相等。

??? example "Example 1"
	![[定理10.4的应用.jpeg]]

??? example "Example 2"
	![[定理10.4的应用2.png]]


## 高斯公式

设空间区域 $V$ 由分片光滑的双侧封闭曲线 $S$ 围成，若函数 $P,Q,R$ 在 $V$ 上连续，且有一阶连续偏导数，则：

$$
\iiint_V(\frac{\partial P}{\partial{x}}+\frac{\partial Q}{\partial{y}}+\frac{\partial R}{\partial{z}})dV= \iint_SPdydz+Qdzdx+Rdxdy
$$

其中 $S$ 取外侧，且由于 Latex 语法问题，上述二重积分应为封闭的。

??? example "Example 1"
	![[高斯公式应用1.png]]
	
	!!! note "若S为封闭的简单曲面，$\overrightarrow{l}$ 为任意固定方向，则 $\iint_S\cos(\overrightarrow{n},\overrightarrow{l})dS=0$"

## 散度场

设 $\overrightarrow{A}(x,y,z)=\{P(x,y,z),Q(x,y,z),R(x,y,z)\}$ 为空间区域 $V$ 上的向量函数，对 $V$ 上每一点 $(x,y,z)$，称向量函数 $\frac{\partial P}{\partial{x}}+\frac{\partial Q}{\partial{y}}+\frac{\partial R}{\partial{z}}$ 为向量函数 $\overrightarrow{A}$ 在 $(x,y,z)$ 处的**散度**，记作$div\ \overrightarrow{A}(x,y,z)$

则高斯公式可以写成如下：

$$
\iiint_V div\ \overrightarrow{A}dV=\iint_S\overrightarrow{A}\overrightarrow{dS}
$$

!!! tip "散度与坐标系选取无关"

### 散度场的物理意义

当流速为 $\overrightarrow{A}$ 的不可压缩流体，经过封闭曲线S的流量是 $\iint_S\overrightarrow{A}\overrightarrow{dS}$，则 $div\ \overrightarrow{A}(M_0)$ 是流量对体积的变化率，并称它为 $\overrightarrow{A}$ 在 $M_0$ 点的流量密度

- 若 $div\ \overrightarrow{A}(M_0)\gt 0$ ，说明在每一单位时间内都有一定数量流体流出这一点，则称这点为**源**
- 若 $div\ \overrightarrow{A}(M_0)\lt 0$ ，说明流体在这一点被吸收，则称这点为**汇**
- 若在向量场 $\overrightarrow{A}$ 中每一点都有 $div\ \overrightarrow{A}(M_0)= 0$ ，则称 $\overrightarrow{A}$ 为**无源场**

### 推论

如果仅在区域 $V$ 中某些点(或子区域)上，$div\ \overrightarrow{A}\ne 0$ 或不存在，其它的点都有 $div\ \overrightarrow{A}= 0$ ，则通过包围这些点或子区域(称为洞)的 $V$ 内任一封闭曲面积分(物理意义流量)都是相等的，即是一个常数。

![[高斯公式推论应用1.png]]

## 曲面变换下的第二类曲面积分计算

$$\begin{gather}
设曲面S的参数方程为\begin{cases}x=x(u,v) \\ y=y(u,v) \\ z=z(u,v)\end{cases} \\ 则\iint_S\overrightarrow{A}\overrightarrow{dS}=\iint_ {\sigma_{uv}} \left |
\begin{matrix}P & Q & R  \\ x'_u & y'_u & z'_u \\ x'_v & y'_v & z'_v\end{matrix} \right | dudv
\end{gather}
$$

## 斯托克斯公式

$$
\oint_LPdx+Qdy+Rdz=\iint_S(\frac{\partial R}{\partial y}-\frac{\partial Q}{\partial z})dydz+(\frac{\partial P}{\partial z}-\frac{\partial R}{\partial x})dzdx+(\frac{\partial Q}{\partial x}-\frac{\partial P}{\partial y})dxdy
$$

其中，$L$ 与 $S$ 依靠右手定则确定正向。

![[斯托克斯公式1.png]]

- 由于斯托克斯公式变换而来的第二类曲面积分不一定是封闭的，更多时候需要我们运用定义去算积分
	- 具体而言就是将 $\overrightarrow{dS}$ 变成 $(\cos\alpha+\cos\beta+\cos\gamma)dS$ ，其中三角函数来源于曲面的法向量(方向由右手定则确定) 

## 空间曲线积分的路径无关性

设 $\Omega \in R^3$ 为空间**线单连通区域**，若函数P,Q,R在 $\Omega$ 上连续，且有一阶连续偏导数，则下列条件等价：

- 不想写了，Goodbye，祝你考个好成绩!

## 旋度场

!!! info "此部分内容书本上打星号了，但是还是考到过"

设 $\overrightarrow{A}(x,y,z)=\{P(x,y,z),Q(x,y,z),R(x,y,z)\}$ ，定义向量函数 $\overrightarrow{A}$ 在 $(x,y,z)$ 处的**旋度**为:

$$rot(\overrightarrow{A})=(\frac{\partial R}{\partial y}-\frac{\partial Q}{\partial z},\frac{\partial P}{\partial z}-\frac{\partial R}{\partial x},\frac{\partial Q}{\partial x}-\frac{\partial P}{\partial y})$$

**【定义环量密度】**

$$\begin{gather}
\mu =\lim_{S\rightarrow M}\frac{\oint_l \overrightarrow{A}\overrightarrow{ds}}{D}=\lim_{S\rightarrow M}\frac{\iint_S rot\overrightarrow{A}\overrightarrow{dS}}{D} \\
=\lim_{S\rightarrow M}\frac{\iint_S (rot\overrightarrow{A}\cdot \overrightarrow{n_0}){dS}}{D} \\ 中值定理 =(rot\overrightarrow{A}\cdot \overrightarrow{n_0})_M\ \ M为D内一点 
\end{gather}$$

即，矢量场 $\overrightarrow{A}(M)$ 在点 $M$ 处的**环量密度**公式为：

$$
\mu =rot \overrightarrow{A}\cdot n_0 = (\frac{\partial R}{\partial y}-\frac{\partial Q}{\partial z},\frac{\partial P}{\partial z}-\frac{\partial R}{\partial x},\frac{\partial Q}{\partial x}-\frac{\partial P}{\partial y})(\cos \alpha ,\cos \beta,\cos\gamma)
$$


## 按定义求第二类曲面积分

!!! bug "错题!"
	![[按定义求第二类曲面积分.jpeg]]
	
	只有第一步做错了：第二类曲线积分不能随便去绝对值，因此这题正确做法为保留绝对值，然后再按定义求。

!!! danger "用定义求的时候一定要注意!"
	法向量要单位化!
	
	法向量要单位化!
	
	法向量要单位化!

