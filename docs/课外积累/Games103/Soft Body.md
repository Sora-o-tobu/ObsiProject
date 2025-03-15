
# Soft Body

## Finite Element Method

### Linear Finite Element Method (FEM)

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

### Finite Volume Method (FVM)

相比于 FEM 用能量对位置求导计算力，FVM 对 **Traction Vector** 积分得到力。

- 在二维空间中，Traction Vector 表示单位长度受到的力 $T= \frac{dF}{dl}$
- 在三维空间中，Traction Vector 表示单位面积受到的力 $T= \frac{dF}{dA}$

在不同的状态、不同的需求下，我们定义三种不同的 Stress：

![[differentstress.png]]

其中 Second Piola-Kirchhoff Stress 是上一小节 FEM 用的，而 Cauchy Stress 是本节 FVM 需要用到的，因为我们需要计算形变后的法线变成形变后的力。

!!! info "$F$ 指的是上一节定义过的 Deformation Gradient 形变梯度"

![[FVMSummary.png]]

### Hyperelastic Models

