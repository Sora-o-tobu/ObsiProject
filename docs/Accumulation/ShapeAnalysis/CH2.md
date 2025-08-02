
# Mathematical Background

## Linear Algebra

向量是线性代数的基本概念，图形学中默认向量都是列向量，向量内积和二次型等都可以通过转置和矩阵乘法来表示：

$$\begin {array}l
\text{Dot Product}:& x^Ty \\
\text{Quadratic form}: & x^TAx
\end{array}
$$

!!! info "更严格来说，向量坐标是向量本身和基向量 $e_k$ 的内积"

矩阵被理解为**线性算子**或**向量内积**，我们使用 Einstein 记号来显式区分它们：

=== "Linear Map"
	![[SA_2_1.png]]
=== "Quadratic Form"
	![[SA_2_2.png]]

线性代数的另外一大应用是求解线性系统 $Ax=b$。当矩阵 $A$ 可逆时，我们可以通过高斯消元等方法来求解线性方程组，它的时间复杂度为 $O(n^3)$。

!!! warning "任何情况下我们都应避免对矩阵求逆，这通常是 unstable 和 slower 的"

目前的求解器分为直接求解和迭代求解两类：

- **Direct** explicit matrix
	- 高斯消元法、LU 分解、Cholesky 分解、QR 分解等
	- 数值稳定性更好，但复杂度较高
- **Iterative** apply matrix repeatedly
	- Jacobi、Gauss-Seidel、共轭梯度(CG)等
	- 占用内存更低，计算方式适合 GPU，但可能更难收敛

## Multivariable Calculus

一个优化问题由优化目标，等式约束，不等式约束三部分组成：

$$
\min _{x\in R^n} f(x) \ s.t.\ g(x) =0,\ h(x) \gt 0
$$

微分是求解优化问题的基础，定义函数沿着方向 $v$ 的微分为：

$$d f_{\mathbf{x}_0} (\mathbf{v}) = \lim_{h \rightarrow 0} \frac{f(\mathbf{x}_0 + h \mathbf{v}) - f(\mathbf{x}_0)}{h}$$

我们将其泰勒展开，得到如下等式：

$$d f_{\mathbf{x}_0} (\mathbf{v}) = \sum_k \frac{\partial f}{\partial x^k} \cdot v^k = \nabla f(\mathbf{x}_0) \cdot \mathbf{v}$$

因此 $df_{ x_0}$ 可以看作是关于向量 $v$ 的一个线性算子。

当只有 **等式约束** $g(x)=0$ 时，约束下的极值点必须满足 **Lagrange 条件**：

$$\nabla f(x_0) + \sum_{k=1}^m \lambda_k \nabla g_k(x_0) = 0$$

其中 $\lambda_k$ 称为 **拉格朗日乘子 (Lagrange multipliers)**。

- 引入 **拉格朗日函数**：

$$\Lambda(x; \lambda) = f(x) + \sum_{k=1}^m \lambda_k g_k(x)$$

求解约束优化问题 ⇔ 寻找 $\Lambda$ 的驻点（同时对 $x, \lambda$ 求偏导为零）。

当存在 **不等式约束** $h(x) \leq 0$ 时，Lagrange 乘子法推广为 **Karush–Kuhn–Tucker (KKT) 条件**：

一个点 $x_0$​ 是最优解的必要条件是存在 $\lambda \geq 0$ 使得：

- <1> **原始可行性 (Primal feasibility)**

$$h(x_0) \leq 0$$

- <2> **对偶可行性 (Dual feasibility)**

$$\lambda_k \geq 0, \ \forall k$$

- <3> **互补松弛 (Complementary slackness)**

$$\lambda_k h_k(x_0) = 0, \ \forall k$$

!!! note "如果某约束不活跃 ($h_k(x_0)<0$)，则对应的乘子 $\lambda_k=0$"

- <4> **平稳性 (Stationarity)**

$$\nabla f(x_0) + \sum_{k=1}^p \lambda_k \nabla h_k(x_0) = 0$$

!!! info "表示在约束面上，不存在能继续下降的可行方向"