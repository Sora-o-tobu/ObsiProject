
# Soft Body

## Linear Finite Element Method (FEM)

Linear Element 指的是三角形（二维）或者四面体（三维）。在 Linear Element 层面上，图形学的有限元和机械上的有限元是一样的。

称三角形静止状态为 *Reference*。在 FEM 中我们假设三角形内部形变是均匀的，即三角形内部任意点 $X$ 形变后的坐标可以用 $x=FX+c$ 来表示。

!!! info "$F$ 也被称为形变梯度 Deformation Gradient，因为 $F=\partial x /\partial X$"
	我们可以选取三角形内部一个矢量的前后变换来计算得到 $F$，例如 $F=[x_{10}, x_{20}][ X_{10}, X_{20}]^{-1}$

 由于我们希望得到一个只描述了形变的矩阵，不希望包含三角形的旋转信息。我们知道，在 SVD 中，一个三角形的变换 $F=UDV^T$ 由 $Rotation: V^{T},\ Scaling: D,\ Rotation:U$ 组成，为了消除 $U$ 的影响，引入一个新的概念 **Green Strain**：

$$
G= \frac{1}{2}(F^TF-I) =\frac{1}{2}(V D^2 V^T-I)=
\left [\begin{matrix}\varepsilon_{uu} & \varepsilon_{uv} \\ \varepsilon_{uv} & \varepsilon_{vv} \end{matrix} \right ]
$$

!!! question "无形变时，$G=0$；$||G||$ 与形变程度正相关"

有了形变矩阵后，我们希望得到三角形某一形变状态下的能量，定义三角形 Reference 状态下的能量密度为 $W(G)$，总能量为面积乘以能量密度 $E=A^{ref}W(G)$

!!！info "Saint Venant-Kirchhoff Model, StVK 能量密度模型"
	$$\begin{array}lW(G)=W(\varepsilon_{uu}, \varepsilon_{vv}, \varepsilon_{uv})=\frac{\lambda}{2}(\varepsilon_{uu} + \varepsilon_{vv})^2 + \mu(\varepsilon_{uu}^2 +\varepsilon_{vv}^2 + 2\varepsilon_{uv}^2)\\ S=\frac{\partial W(G)}{\partial G} = 2\mu G+ \lambda trace(G)I\end{array}$$

定义了能量之后，我们就能计算形变时作用的力了:

$$
f_i=- \left(\frac{\partial E}{\partial x_i}\right)^T = -A^{ref} \left(\frac{\partial W}{\partial x_i}\right)^T= -A^{ref} \left(\frac{\partial W}{\partial \varepsilon_{uu}} \frac{\partial \varepsilon_{uu}}{ \partial x_i} + \frac{\partial W}{\partial \varepsilon_{vv}} \frac{\partial \varepsilon_{vv}}{ \partial x_i} + \frac{\partial W}{\partial \varepsilon_{uv}} \frac{\partial \varepsilon_{uv}}{ \partial x_i}\right)^T
$$

=== "Force 1"
	![[FEMForce1.png]]
=== "Force 2"
	![[FEMForce2.png]]

总之，最后的结论是：

$$
[f_1, f_2] = -A^{ref} FS[X_{10}, X_{20}]^{-T},\ f_0 = -f_1 - f_2
$$

最后根据力使用显式积分或隐式积分进行更新。

## Finite Volume Method (FVM)

相比于 FEM 用能量对位置求导计算力，FVM 对 **Traction Vector** 积分得到力。

- 在二维空间中，Traction Vector 表示单位长度受到的力 $T= \frac{dF}{dl}$
- 在三维空间中，Traction Vector 表示单位面积受到的力 $T= \frac{dF}{dA}$

在不同的状态、不同的需求下，我们定义三种不同的 Stress：

![[differentstress.png]]

其中 Second Piola-Kirchhoff Stress 是上一小节 FEM 用的，而 Cauchy Stress 是本节 FVM 需要用到的，因为我们需要计算形变后的法线变成形变后的力。

!!! info "$F$ 指的是上一节定义过的 Deformation Gradient 形变梯度"

![[FVMSummary.png]]

得到形变后四面体四个顶点受到的力后，再用积分法更新速度和位置完成模拟即可。

!!! tip "当划分的单元为三角形或四面体时，FEM 和 FVM 等效"

## Hyperelastic Models

在 FEM 一节中，我们使用 Saint Venant-Kirchhoff Model, StVK 来作为能量密度模型。本节再介绍几种不同的模型。

对于各向同性材料，我们认为其 First Piola-Kirchhoff Stress 有如下性质：

$$
P=P(F)= P(UDV^T) =UP(\lambda_0, \lambda_1, \lambda_2) V^T
$$

其中 $\lambda$ 为形变过程中各个维度的缩放系数，即缩放矩阵 $D$ 的对角线元素。

记 $C=D^TD$ 为柯西-格林变形张量，且有：

$$\begin{array}l
I_C & = trace(C) = \lambda_0^2 + \lambda_1^2 + \lambda_2^2 \\
II_C & = det(C^2) = \lambda_0^4 + \lambda_1^4 + \lambda_2^4 \\
III_C & = \frac{1}{2}(trace^2(C) -trace(C^2))= \lambda_0^2 \lambda_1^2 + \lambda_1^2 \lambda_2^2 + \lambda_0^2 \lambda_2^2 
\end{array}
$$

在此前提下，四种不同的弹性模型的能量密度 $W$ 可记为：

![[hyperelasticmodelsW.png]]

!!! quote "StVK 缺点"
	![[StVKDisadvantage.png]]
	
	形变为 1 时记为原长。StVK 模型在压缩时 Stress 反升，最终会导致物体形变不能恢复；而 Neo Hookean 则没有这个问题

## Collision Handling

### Collision Detection

碰撞检测一般分为两个阶段，第一步先用 *Spatial Hashing* 或 *BVH* 排除掉大部分不可能发生碰撞的三角形对；第二步再在局部用 *DCD* 或 *CCD* 进行碰撞检测。

![[CollisionDetectionPipeline.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">Spatial Hashing</font>

Spatial Hashing 又被称为 Spatial Partition，它将空间划分为一个个 bucket，如果三角形与某 bucket 相交，则维护一个该 bucket 到该三角形对象的指向。

![[spatialhashing.png]]

当检测碰撞时，只需要看同一个 bucket 有多个对象的区域即可。

但是直接存储所有 bucket 需要占据大量内存，我们不再维护空格子，而是维护非空的键值对链表，第一个元素是格子的 ID，第二个元素指向三角形对象。

!!! info "可以类比成 List 转成 Linked List"

问题在于，如果使用类似上图的 Grid Order 形式，在我们遍历链表的时候不能很好的访问周边格子，Cache 命中率较低，即缺少 Locality 特性。可以采用 Morton Code 作为编号方式：

![[MortonCode.png]]

Spatial Hashing 的优点是容易执行、对 GPU 友好；缺点是每次物体更新都要重新计算。

<font style="font-weight: 1000;font-size: 20px" color="orange">Bounding Volumn Hierarchy</font>

按照对象的邻近性，我们构造有层次的包围盒：

![[BVHE.png]]

碰撞检测时，从上到下进行判断是否相交，如果大包围盒都没有相交，那么也就不需要判断小包围盒了。

我们一般采用 Axis-aligned 的包围盒，即与坐标轴对齐，这样判断是否相交只需要根据坐标之间的关系比对即可。

包围盒的局限性在于，对于周遭的三角形很难剔除；并且对于大形变物体的模拟，如衣服，较难应用。

<font style="font-weight: 1000;font-size: 20px" color="orange">Discrete Collision Detection</font>

DCD 在离散的时间 $x^{[0]}$ 和 $x^{[1]}$ 中检测物体是否相交。对于一条已知的 edge $<x_a, x_b>$，我们需要检测其是否与三角形 $x_1 x_2 x_3$ 相交，计算步骤如下：

$$\begin{array}l
((1-t)x_a + tx_b - x_0)\cdot (x_{10} \times x_{20}) = 0
\\ \Rightarrow t =\frac{x_{0a}\cdot (x_{10} \times x_{20})}{x_{ba}\cdot (x_{10} \times x_{20})}
\end{array}$$

如果 $t\in [0,1]$，则可判断该 edge 与三角形相交。

对于运动比较快的物体，有可能存在前后两个离散时间状态都没有相交，但是实际上应该相撞了的情况。

<font style="font-weight: 1000;font-size: 20px" color="orange">Continuous Collision Detection</font>

CCD 检测 $x^{[0]}$ 到 $x^{[1]}$ 状态之间，是否发生相交。在 CCD 中，我们认为每个点在两个时间之间的速度是固定不变的，那么两个状态之间的位置都可以表示为 $x(t)$ 一次线性函数。

我们有两种方法进行检测相交：

=== "Vertex-Triangle Test"
	![[Vertex-TriangleTest.png]]
=== "Edge-Edge Test"
	![[edgeedgetest.png]]

!!! note "三次方程尽量用牛顿法或二分法解，如果用求根公式，三次方根的误差非常大"

相比于 DCD，开销更大、更难实现。

### Collision Response Approaches

给定两个状态 $x^{[0]}$ 和 $x^{[1]}$，我们想要找到一个离 $x^{[1]}$ 较近的 $\bar{x}^{[1]}$ ，使得 $x^{[0]} \bar{x}^{[1]}$ 没有发生相交（即没有碰撞）。 此时有 Interior Point Methods 和 Impact Zone Optimization 两种策略：

![[CRAPC.png]]

- **Interior Point Methods：** 细化模拟的步长，提高模拟的真实性，尽量保证每一步都落在合法范围内。
	- 内点法保证永远能模拟到还算合理的结果，但可能要走很多步才能达到边界位置。
	- 由于物体上所有点的模拟需要同步进行，不仅发生碰撞的点需要走这么多步，其他物体上所有的点都需要跟着走很多步，所以内点法运行速度比较慢。
- **Impact Zone Optimization：** 从发生碰撞的位置出发回到合法位置
	- 只需要处理发生碰撞的那些顶点，且模拟的步长可以设得比较大，运行速度较快。
	- 但是可能存在回不到合法区域的情况，尤其是当模拟步长较大，导致碰撞位置离安全区域较远的时候。

Log-Barrier 内点法设置能量为距离的 log，来求解碰撞最终的合法位置：

![[logbarrier.png]]

最后变成一个优化问题。（摆了）

