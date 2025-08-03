
# Curves

## Continuous Curves

### Geometric Definition

Curves 是最基本的几何对象，在微积分中我们将曲线理解为一个实数域到二维平面的函数 $\gamma: \mathbb {R} \rightarrow \mathbb {R}^2$。问题在于，这种表达方式既不能处理曲线退化成点的情况，也不能保证同一条曲线只能有同一种函数表示。

!!! warning "例如对于曲线 $y=2x$，有以下两种相同表达方式"
	- $\gamma_1(t) =(t,2t)$
	- $\gamma_2(t) = \left \{ \begin {array}l (t,2t)& t\le 1 \\ (2(t-1 \frac{1}{2}), 4(t - \frac{1}{2}))& t \gt 1 \end{array}\right .$

此外，我们还需要关注曲线的光滑性质，而函数表达的方式往往不足以推导出曲线本身的光滑性，例如：

![[SA_3_1.png]]

直观来说，曲线可以理解为满足一些条件的点的集合，这些条件中最重要的一点是其**微分性质**，即在曲线上任意一点都可以用直线来拟合近似。

为了更精确的定义它，数学上通常引入**参数化（parametrization）**：

- 对于曲线上任意一点 $p$，存在一个邻域 $U$
- 存在一个光滑映射，将邻域 $U$ 映射到实数域开区间 $(a,b)$ 上： $\gamma_p : (a,b) \rightarrow U$
- 且满足在每一个点 $t$，都有 $\gamma_p' (t)\ne 0$

在讨论参数化曲线时，常常需要区分两层含义：

- **Trace（轨迹 / 几何像）**
    - 定义为：曲线在空间中的点集
    - $\mathrm{trace}(\gamma) := \{ \gamma(t) \mid t \in (a,b) \} \subset \mathbb{R}^n$
    - 也就是这条曲线在几何空间中实际画出的形状。
    - 不同的参数化（比如 $\gamma(t)$ 和 $\gamma(\phi(t))$，其中 $\phi$ 是单调函数）可能有相同的 trace。

!!! note "trace 只关注“走过了哪些点”，不在乎怎么走"

- **Component（分量 / 参数化函数的坐标表达）**
    - 指的是 $\gamma$ 在各坐标轴上的分量函数：
    - $\gamma(t) = (x(t), y(t), z(t), \dots)$
    - 其中每个 $x(t), y(t), z(t)$ 都是 $\mathbb{R}$ 上的光滑函数，称为 $\gamma$ 的 **components**。

!!! note "component 关心“每个坐标随参数如何变化”"

### Arc Length

几何是研究**不变量(invariant)**的学科，我们关心曲线在不同参数化下哪些量是保持不变的。

例如，曲线的 velocity 就不是不变量，它依赖于函数 $\gamma$ 的选取。假定我们有一个参数化曲线：

$$
\gamma : (a,b) \rightarrow \mathbb R^n, \ \ \gamma (t) = (x(t), y(t), z(t), \dots)
$$
那么速度向量和速度大小为：

$$\begin{array}l
v(t)= \gamma '(t) = \frac{d\gamma}{dt}
\\ \|v(t)\| = \| \frac{d\gamma}{dt} \|
\end{array}$$

这取决于参数 $t$ 的选取。举一个例子，对于如下两条相同圆周曲线：

- 用 $\gamma_1(t)=(\cos t, \sin t)$，速度大小恒为 1。
- 用 $\gamma_2(t)=(\cos t^2, \sin t^2)$，曲线一样，但速度 $\|\gamma_2'(t)\|=2t$，随着 $t$ 改变。

而曲线的**弧长（Arc Length）**则完全与参数化无关，只与曲线本身的几何形状有关：

$$
\int^b_a \| \gamma' (t) \|_2dt
$$

有了弧长这一不变量，我们可以在此基础上进行参数化。记曲线弧长为 $S(t)$：

$$S(t) = \int_{t_0}^t \Vert \gamma'(t) \Vert_2 \ dt$$

显然 $S(t)$ 是一个单调递增的函数，因此必定存在反函数 $\phi = S^{-1}$ 满足 $t = \phi \circ S(t)$。利用 $\phi$ 就可以重新参数化整条曲线：

$$\tilde{\gamma} (s) = \gamma \circ \phi(s)$$

!!! info "这里的参数 $s$ 就是弧长，$\phi (S(t)) = t$"

用弧长参数化后的曲线有许多优良性质。例如曲线的速度变为了常数，即单位向量：

$$
\begin{aligned} \tilde{\gamma}' (s) &= \gamma' (\phi (s)) \cdot \phi' (s) \\ &= \frac{\gamma' (\phi (s))}{\Vert \gamma'(\phi (s)) \Vert_2} \end{aligned}
$$

将该单位向量替换成关于 $s$ 的三角函数形式：

$$
\begin{aligned} \tilde{\gamma}' (s) = \mathbf{T} (s) &= \cos \theta(s) \ \mathbf{e}_1 + \sin \theta(s) \ \mathbf{e}_2 \\ &= \begin{bmatrix} \cos \theta(s) \\ \sin \theta(s) \end{bmatrix} \end{aligned}
$$

继续对速度进行求导：

$$
\begin{aligned} \mathbf{T}' (s) &= \theta'(s) \cdot [ -\sin \theta(s) \ \mathbf{e}_1 + \cos \theta(s) \ \mathbf{e}_2 ] \\ &= \theta'(s) \begin{bmatrix} 0 &-1 \\ 1 & 0 \end{bmatrix} \mathbf{T} (s) \\ &= \kappa(s) \ J \ \mathbf{T}(s) \\ &= \kappa(s) \ \mathbf{N}(s) \end{aligned}
$$

其中 $\kappa(s)=\theta'(s)$ 称为**曲率(curvature)**； $\mathbf{N}(s) = J \ \mathbf{T}(s)$ 表示对速度方向逆时针旋转 $90^{\circ}$，称为**法向(normal)**。上式指出弧长参数化的曲线其垂直速度方向变化的速度，或者说曲线弯曲的程度取决于曲率的大小。

![[SA_3_2.png]]

### Coordinate Invariant

上一节证明了我们只需要切向 $\mathbf{T}$ 和法向 $\mathbf N$ 就可以表示整条曲线。对于平面曲线，我们对 $\mathbf{N}$ 求导得到：

$$
\mathbf N'(s)= J \ \mathbf{T}' (s) = J \bigg( \kappa(s) \ J \ \mathbf{T}(s) \bigg) = -\kappa(s) \mathbf{T}(s)
$$

整理后得到如下 **Frenet** 方程：

$$
\frac{d}{ds} \begin{bmatrix} \mathbf{T} (s) \\ \mathbf{N} (s) \end{bmatrix} = \begin{bmatrix} 0 & \kappa(s) \\ -\kappa(s) & 0 \end{bmatrix} \begin{bmatrix} \mathbf{T} (s) \\ \mathbf{N} (s) \end{bmatrix}
$$

!!! tip "$(\mathbf{T}, \mathbf{N})$ 也被称为二维 Frenet 框架（Frenet Frame）"

因此，如果我们只知道一条曲线的曲率函数 $\kappa(s)$，我们可以通过解 Frenet 方程得到曲线的切向 $\mathbf{T}$ 和法向 $\mathbf N$，从而表示整条曲线。

因此，曲率定义了平面曲线的形状；两条具有相同曲率的曲线最多相差一个刚体变换（平移、旋转）。

!!! quote "事实上，仅知道 $T(s)$ 就可以通过积分得到曲线，因为 $\gamma '(s) = \mathbf{T} (s)$"

### Fundamental Theorem of Plane Curves

本节介绍了一些关于曲率的理论。

<font style="font-weight: 1000;font-size: 20px" color="orange">Gauss Map</font>

将法向理解为单位圆上的点或方向，这样曲线上任意一点都与单位圆 $S^1$ 有映射，这种映射称为 Gauss Map：

![[SA_3_3.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">Winding Number</font>

卷绕数(Winding Number)表示封闭曲线逆时针绕过原点的总次数，可以通过对曲率积分来计算：

![[SA_3_4.png]]

### Space Curves

对于空间中的曲线，除了主法向和曲率外，我们还需要引入**副法向(Binormal)** $\mathbf B$ 和**挠率(torsion)** $\tau$。

副法向是垂直于切向和主法向的单位向量 $B=T\times N$，而挠率则描述了曲线出平面的运动。

!!! note "直观理解，曲率告诉我们曲线是否偏离直线，挠率告诉我们曲线是否偏离所在平面"
	- Curvature: In-plane motion
	- Torsion: Out-of-plane motion

空间曲线的 Frenet 方程如下：

$$
\frac{d}{ds} \begin{bmatrix} \mathbf{T} (s) \\ \mathbf{N} (s) \\ \mathbf{B} (s) \end{bmatrix} = \begin{bmatrix} 0 & \kappa(s) & 0 \\ -\kappa(s) & 0 & \tau(s) \\ 0 & -\tau(s) & 0 \\ \end{bmatrix} \begin{bmatrix} \mathbf{T} (s) \\ \mathbf{N} (s) \\ \mathbf{B} (s) \end{bmatrix}
$$

因此，对于空间曲线，曲率和挠率共同决定了一条曲线的形状。

### First Variation Formula

我们把弧长看作一个 **泛函 (functional)**，它把一条曲线 $\gamma$ 映射到一个实数（弧长）：

$$L[\gamma] = \int_a^b \|\gamma'(t)\| \, dt$$

接着，引入一个 **向量场扰动** $\mathbf{v}(t)$，并且规定端点固定：

$$\mathbf{v}(a)=\mathbf{v}(b)=0$$

定义一族被扰动的曲线：

$$\gamma_r(t) = \gamma(t) + r \mathbf{v}(t), \quad r \in \mathbb{R}$$

!!! note "这表示在曲线上每个点加上一个“小位移”，得到一条新的曲线"

记新的曲线弧长为：

$$l(r) = \int_a^b \|\gamma_r'(t)\| \, dt$$

在 $r=0$ 处的导数就是弧长泛函在方向 $\mathbf{v}$ 上的变分（Gateaux 导数）：

$$dL_\gamma[\mathbf{v}] = l'(0)$$

$$
\begin{aligned} l'(r) &= \frac{d}{dr} \int_a^b \Vert \gamma_r'(t) \Vert_2 \ dt \\ &= \int_a^b \frac{d}{dr} \Vert \gamma_r'(t) \Vert_2 \ dt \\ &= \int_a^b \frac{(\gamma'(t) + r \mathbf{v}'(t)) \cdot \mathbf{v}'(t)}{\Vert \gamma'(t) + r \mathbf{v}'(t) \Vert_2} \ dt \end{aligned}
$$

代入条件 $r=0$ 得到：

$$
\begin{aligned} l'(0) &= \int_a^b \frac{\gamma'(t) \cdot \mathbf{v}'(t)}{\Vert \gamma'(t) \Vert_2} \ dt \\ &= \int_a^b \mathbf{T}(t) \cdot \mathbf{v}'(t) \ dt \end{aligned}
$$

其中 $\mathbf{T}(t)=\frac{\gamma'(t)}{\Vert \gamma'(t) \Vert_2}$ 为曲线上的单位切向量。利用分部积分可以进一步得到：

$$
\begin{aligned} \int_a^b \mathbf{T}(t) \cdot \mathbf{v}'(t) \ dt &= [\mathbf{T} \cdot \mathbf{v}(t)]_a^b - \int_a^b \kappa(t) \mathbf{N}(t) \cdot \mathbf{v}(t) \ dt \\ &= - \int_a^b \kappa(t) \mathbf{N}(t) \cdot \mathbf{v}(t) \ dt \\ &= \langle \mathbf{v}, -\kappa \mathbf{N} \rangle \end{aligned}
$$

联想到方向导数的定义 $df_{x_0} (\mathbf{v}) = \mathbf{v} \cdot \nabla f(x_0)$。

上式说明在曲线上每一点沿法向进行移动可以缩短曲线的长度。同时也说明了我们可以通过设计曲线上的向量场来调整曲线的形状和长短，这样的方法称为 **curve shortening flow**。

## Discrete Curves

我们需要考虑如何在计算机内表示曲线。

Bezier 曲线是工程中常用的一种曲线形式，我们通过一系列控制点来生成一条光滑的曲线。但这种表示方式难以研究曲线的一些几何性质，例如它的弧长是没有定式解的。

![[SA_3_5.png]]

!!! note "大部分常用曲线的弧长都没有定式解，即便存在也往往具有非常复杂的形式"

为了研究曲线的几何性质，这门课中采用**折线（Polyline）**来表达各种类型的曲线。直观来看，折现是对曲线的一种近似，通过以直代曲来离散地表达整条曲线：

![[SA_3_6.png]]

使用折线来近似光滑曲线自然会引入相关问题，例如原本有良好定义的一些几何量会出现未定义的情况。因此我们需要将原本光滑曲线上的各种概念推广到离散的折线上。

一种直观的处理方法是使用**有限差分(finite difference)**来近似微分。但遗憾的是这种离散差分的形式有时也是不可行的。

!!! note "实际上很多微分性质都是通过差分取极限的方式推导的"

### Signed Curvature on Plane Curves

<font style="font-weight: 1000;font-size: 20px" color="orange">Discrete Gauss Map</font>

先前的 Gauss Map 将法向映射为单位圆上的点，而对于 Discrete Gauss Map，它将线段上的法向仍然映射为点，而顶点上的法向映射为相邻两段法向的夹角，即*圆弧*：

![[SA_3_7.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">Integrated Curvature</font>

折线在顶点处的转角 $\theta$ 等于曲率在 dual cell 上的积分。

!!! abstract "离散化表示曲线时，我们有一套原始单元(Primal Cell)，例如顶点、边等，Dual Cell 是定义它们上的对偶结构"

![[SA_3_8.png]]

因此可以说曲率的大小受到了 dual cell 的长度影响：

![[SA_3_9.png]]

!!! warning "对于折线，可以认为线段上的曲率为 0，顶点上的曲率为 $\infty$"

结合外角定理，我们可以得出一段封闭折线关于弧长积分的和为：

![[SA_3_10.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">First Variation Formula</font>

之前我们得出结论是将连续曲线沿着 $\kappa \mathbf{N}$ 方向移动是缩短曲线长度的最快方式。

而对于离散曲线则可以类似地证明将顶点沿法向移动 $2 \sin\frac{\theta}{2}$ 是改变曲线长度最快的方式，从这个角度来看离散曲率的表达式为 $\kappa = 2 \sin\frac{\theta}{2}$。注意到这里离散曲率的表达式不同于前面推导出的结果，这再次说明离散的几何概念是不能保证对于所有的情况都严格成立的。不过当 $\theta$ 比较小时这两种曲率的定义是非常接近的。

![[SA_3_11.png]]

### Discrete Differential Geometry

#### Curves in 3D

三维空间的连续曲线需要满足如下 Frenet 方程：

$$\frac{d}{ds} \begin{bmatrix} \mathbf{T}(s) \\ \mathbf{N}(s) \\ \mathbf{B}(s) \end{bmatrix} = \begin{bmatrix} 0 & \kappa & 0 \\ -\kappa & 0 & \tau \\ 0 & -\tau & 0 \end{bmatrix} \begin{bmatrix} \mathbf{T}(s) \\ \mathbf{N}(s) \\ \mathbf{B}(s) \end{bmatrix}$$

基于这个公式，我们有两种方式对曲线进行离散化：

- <1> Frenet 方程离散化：折线的方向近似为 $T$
	- $T_j=\frac{ p_{j+1} - p_j}{\| p_{j+1} - p_j\| _2 }$
	- $B_j= T_{j-1} \times T_j$
	- $N_j = B_j\times T_j$
- <2> Transfer Matrix 方法：用旋转矩阵 $R_{i,j}$ 表示从一段折线到下一段折线的变换，是有限的传递操作
	- $\begin{bmatrix} \mathbf{T}_{i+1} \\ \mathbf{N}_{i+1} \\ \mathbf{B}_{i+1} \end{bmatrix} = R_{i+1,j} \begin{bmatrix} \mathbf{T}_i \\ \mathbf{N}_i \\ \mathbf{B}_i \end{bmatrix}$

#### Adapted Framed Curve

同时，仅仅用折线近似曲线是不够的。
比如一条“绕着切向量旋转”的螺旋曲线，如果只用折线，我们能近似出它的弯曲，但**无法表达曲线自身的扭转 (twist)**。

为此，需要引入一个 **material frame（材料标架）**：

- 在切向量 $\mathbf{T}$ 的垂直平面上，再选择两个互相正交的单位向量 $\mathbf{m}_1, \mathbf{m}_2$；
- 这样不仅描述了曲线的形状，还能捕捉到“绕着切线旋转”的自由度。

![[SA_3_12.png]]

有了 material frame，我们就能定义：

- **弯曲能量 (bending energy)**：由切向的变化决定；
- **扭转能量 (twisting energy)**：由 material frame 在曲线上的旋转决定。

!!! success "这为模拟弹性曲线（如橡皮筋）提供了能量形式"

#### Bishop Frame

在实际应用 material frame 时，我们需要一个方法来定义和传播 $\mathbf{m}_1, \mathbf{m}_2$​。  
这就是 **Bishop Frame（平行运输标架）** 的思想：

- 在起点选择任意一组正交向量 $(\mathbf{T}, \mathbf{u}, \mathbf{v})$，其中 $\mathbf{T}$ 是切向量；
- 沿曲线移动时，不让 $\mathbf{u}, \mathbf{v}$ 产生不必要的旋转，而是通过 **平行运输 (parallel transport)** 来传播；
- 平行运输意味着：标架变化时，尽量保持旋转最小，只受曲率方向影响。

可以证明，Bishop frame 的变化由 **Darboux vector** 控制：

$$\Omega = \kappa \mathbf{B}$$

通过微分方程来得到整个曲线上的标架。

有了 Bishop frame，就能自然定义 material frame 的旋转角，从而给出 **扭转能量** 的表达式。

$$
\begin{array}l
\mathbf{m}_1 = \mathbf{u} \cos \theta + \mathbf{v}\sin \theta
\\ \mathbf{m}_2= - \mathbf{u}\sin \theta + \mathbf{v} \cos \theta
\end{array}
$$

#### Discrete Kirchoff Rod

为了对弹性曲线进行离散，我们使用**折线**来近似。

- 折线的切向：
	- $$\mathbf{T}_i = \frac{e_i}{\|e_i\|}, \quad e_i = \mathbf{x}_{i+1} - \mathbf{x}_i$$​
- 离散曲率的定义：
	- $$\kappa := 2\tan\left(\frac{\theta}{2}\right), \quad \theta = \arccos(\mathbf{T}_i \cdot \mathbf{T}_{i+1})$$
	- 这里 $\theta$ 是相邻切向之间的转角。


基于这个定义，可以推出**离散 Darboux vector**：  
它包含了离散的曲率与挠率，表示折线段之间的旋转关系。

进而，能量可以写成离散形式：

$$\begin{array}l
E_\text{bend} = \alpha \sum_i \frac{\| (\kappa B)_i \|^2_2}{l_i}
\\ E_\text{twist} = \beta \sum_i \frac{(\theta^i - \theta^{i-1}) ^2}{ l_i}
\end{array}
$$
