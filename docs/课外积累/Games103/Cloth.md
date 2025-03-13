
# Cloth

## Mass-Spring System

一个理想的弹簧满足胡克定理，即它的力与拉伸的形变长度成正比。对于多个弹簧组成的系统，系统的能量以及中心点所受的力表示为：

![[multiplespringeffect.png]]

对于一段由四边形组成、结构化的布料，我们添加三种弹簧，构成 Network：

1. 在网格原本的边上添加弹簧，抵抗布料产生的横向和纵向的小褶皱
2. 为每个四边形的对角线方向构造弹簧
3. 跳过一个顶点，抵抗 bending force 的弯曲弹簧，阻止布料向某一方向自由弯曲

!!! info "实际应用中，可以只对四边形一个对角线方向添加弹簧"

![[structurespringnetwork.png]]

但是实际使用时，布料的 Mesh 可能不是很规则、很结构化的方块。对于三角形网格，一种简单的思路就是认为其每条边都是一个弹簧，并为某一边相对的两个顶点都构造一个弯曲弹簧：

![[unstructurespringnetwork.png]]

有了这些弹簧后，我们就可以用显式或隐式的方法搞出 Mass-Spring System 来模拟布料。

### Explicit Integration

首先决定一个粒子状态的更新，将弹簧系统插入到力的计算过程中：

![[particlesystemupdate.png]]

先遍历所有的弹簧，算出每个弹簧对每个顶点施加的力。

然后遍历每个顶点，把弹簧对这个点的力加起来就是这个点受到的合力。

!!! warning "在 $\Delta t$ 或者弹簧系数 $k$ 过大的情况下，显式积分做法很可能会产生 overshooting"

### Implicit Integration

另一种模拟方式称为隐式积分，认为速度的更新依赖于下一状态的力，位置的更新依赖于下一状态的速度，因此隐式积分法在数值上会更加稳定。

由于更新时所用的是下一状态的力 $f^{[1]}$，这是一个与 $x^{[1]}$ 有关的未知量，因此可以考虑将其转换为求解关于变量 $x^{[1]}$ 的方程：

![[implicitintegratione0.png]]

问题在于，力的方程 $f(x)$ 可能不是关于 $x$ 的线性方程。所幸的是，由于 $x$ 是一个 $3\times N$ 的矩阵，里面包含了 $N$ 个顶点的位置信息，因此我们可以采用线性优化的方式简化求解：

![[optimizeproblemofsimplicit.png]]

!!! danger "具体原理听不懂啦"

根据牛顿逼近法，找出最小值所在的点：

```python
for k = 0,...,K
	Delta x = - (F''(x^(k)))^{-1} * F'(x^(k));
	x^(k+1) = x^(k) + Delta x
	if Delta x is small: break
x^{[1]} = x^(k+1)
```

!!! info "牛顿法配合泰勒展开，在一阶导数为 0 时取得最值"
	$$0=F'(x)=F'(x^{(k)}) + F''(x^{(k)}) (x- x^{(k)})$$

而对于 $F(x)$，计算其一阶导和二阶导：

![[simulationbynewtonmethod2.png]]

其中 $H(x)$ 代表 Spring Hessian，它是弹簧切线刚度 Tangent stiffness 的量化，也对应着 $k$：

![[springhession1.png]]

## Dihedral Angle Model

二面角模型不以弹簧的长度来表示弯曲力 Bending Forces，而是以两个 Mesh 之间的夹角来计算：

$$f_i= f(\theta) \mu_i$$

两个面之间的弯曲力涉及到了四个顶点，$\mu_i$ 指的就是这四个顶点的弯曲力分量

![[dihedralanglemodel.png]]

- <1> $u_1$, $u_2$ 的方向应分别与 $n_1$, $n_2$ 相同
- <2> 由于弯曲力不会对边进行拉伸，$u_3- u_4$ 应该与 $x_3 x_4$ 垂直
	- 这也意味着，$u_3 - u_4$ 是 $n_1$, $n_2$ 的线性组合
- <3> 根据牛顿定律，$u_1 +u_2 +u_3 +u_4 = 0$
	- 结合 <2>，则得到 $u_3$ 和 $u_4$ 都是 $n_1$, $n_2$ 的线性组合

加上 $f(\theta)$ 的计算方法，得到：

$$
f_i = k \frac{ ||E||^2 }{ ||N_1|| + ||N_2|| } \left (  \sin \left(\frac{\pi - \theta}{2}\right) - \sin \left(\frac{\pi - \theta_0}{2}\right) \right)\mu_i
$$

!!! info "其中 $\theta_0$ 表示这两个面的初始相位，即它们静止时可能并不是一个平面"

??? note "Locking Issues"
	之前所说的弹簧模型存在问题，用其来模拟不能拉伸的材料时，弹簧的弹性系数非常大，这使得我们进行弯曲时也会受到非常大的阻力。这在模拟一些不能拉伸但是可以弯曲的材料（如纸等）时非常不友好。
	
	!!! info "发生 Locking 问题的本质原因时模拟自由度不足"
	
	- <1> 减小弹簧压缩时的弹性系数
	- <2> 为弹簧设置一段 Buffer，在一定范围内长度改变不会产生弹力

## Constained Approach

真实世界的布料拉伸一定长度后所需的力会陡增。基于胡克定律的弹簧模型需要增大弹性系数 K 来模拟这一现象，因此显式积分和隐式积分都难以应用，带来很多额外计算量。在此前提下，我们提出基于约束的方法来解决这个问题。

对于一个弹簧，我们为其设置约束，认为其长度始终保持不变，即：

$$
Constraint: \ \ ||x_i - x_j|| -L =0
$$

当这个约束被打破时，需要用一个投影方程将其修正。对于此例，我们希望这个投影方程对 Position 的影响尽可能小，并且不改变质心所在的位置，那么其更改则与 $i,j$ 的质量 $m_i, m_j$ 相关：

![[singlespringconstraint.png]]

!!! tip "为定点设置质量无限"

落实到多弹簧系统上，则有两种主要处理方法

<font style="font-weight: 1000;font-size: 20px" color="orange">Gauss-Seidel Approach</font>

依次对每个弹簧进行处理，重复迭代。

```python
for k = 0,...,K # 迭代 K 轮
	for each edge e = <i,j> # 依次处理每一个弹簧
		x_i = x_i - 1/2*(||x_i-x_j||-L_e) * (x_i-x_j).normalize
		x_j = x_j + 1/2*(||x_i-x_j||-L_e) * (x_i-x_j).normalize
```

<font style="font-weight: 1000;font-size: 20px" color="orange">Jacobi Approach</font>

Jacobi 方法希望减少由于边的顺序造成的影响，并且能够尽量在 GPU 上并行。

当我每次计算边的更新时，都将其累加保存下来，最后取平均值

```python
for k = 0,...,K # 迭代 K 轮
	for every vertex i
		x_i_new = 0
		n_i = 0
	for every edge e = <i,j>
		x_i_new = x_i_new + x_i - 1/2*(||x_i-x_j||-L_e) * (x_i-x_j).normalize
		x_j_new = x_j_new + x_j + 1/2*(||x_i-x_j||-L_e) * (x_i-x_j).normalize
		n_i = n_i + 1
		n_j = n_j + 1
	for every vertex i
		x_i  = (x_i_new + a * x_i)/(n_i + a)
```

### Position Based Dynamics

PBD 处理的流程与刚体 Shape Matching 类似：

```c
// A PBD Simulator
// 每个顶点自由运动，更新它们各自的 x 和 v
x = ...;
v = ...;
// Now PBD starts
x_new = projection(x);  // 进行投影恢复约束
v = v + (x_new - x)/dt; // 这里的 x 已经更新过了，所以速度计算是迭加
x = x_new;
```

- **Advantage：**
	- GPU 上很容易并行
	- 执行实现简单
	- 低分辨率上运行很快
		- 不到1000个点的网格能够做到实时
	- 通用性强，还可以用来做弹性体、流体模拟
- **Disadvantage：**
	- 物理上可能不正确，弹性表现受网格、迭代次数影响
	- 高分辨率上运行很慢

### Strain Limiting

Strain Limiting 可以认为是对 PBD 的一个改进（尽管它的出现比 PBD 更早），二者区别在于做投影前不只可以使用粒子系统做模拟，还可以使用弹簧等模型。而最后的投影只是对模拟结果的纠错，增加模拟的稳定性。

相对的，投影时的约束条件可以放宽，例如，设置弹簧的长度限制在一定范围内：

![[SpringStrainLimit.png]]

!!! note "PBD 可以认为是该 Strain Limiting 的特例，即 $\sigma_0 =1$"

除此之外，还可以对三角形的面积等进行约束。

很多物体的力与形变量之间存在非线性关系，当形变量较小时，可以采用正常的物理模拟，此时弹簧弹性系数较小，Locking Issue 不明显；当形变量较大时，改用 Strain Limiting。

