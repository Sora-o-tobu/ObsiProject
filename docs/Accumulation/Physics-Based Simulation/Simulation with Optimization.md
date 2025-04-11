
# Simulation With Optimization

> Range: Chapter 1~4

## Discrete Space and Time

### Solid Geometry

在现代固体模拟领域，连接定义的描述方法是至关重要的。

- **<1> Mesh of edges or triangles**
	- 是有限元方法(FEM)模拟的基础
	- FEM 在 Simulation Accuracy Control 方面表现出色， 它直接在网格上操作，无需信息传输，从而确保更高的精度
	- 本书集中在基于网格的有限元结构
- **<2> Uniform grid to compute solid density**
	- 用于模拟 Material Point Method(MPM)
	- MPM 在管理 Solid Topology 方面较为出色，例如模拟固体断裂等动态事件


### Time Integration

牛顿第二定律为 $f=ma$，我们将其应用到每一个采样点上，用矢量形式简洁表示为：

$$\tag{1.2.1}\begin{align}
\frac{dx}{dt}=v, \\ M \frac{dv}{dt}=f.
\end{align}
$$

其中 $M\in R^{dn\times dn}$ 是 Mass Matrix。

在我们基于物理的模拟中，总是假设我们拥有初始值 $x^0$ 和 $v^0$，以及基于物理模型可以计算的每个时间步长的 $f^n$，以此来更新 $x$ 和 $v$。

#### Explicit Time Integration

显示积分通过将已知值代入简单公式来计算 $x^{n+1}$ 和 $v^{n+1}$ ，本节介绍了 **Forward Euler** 和 **Symplectic Euler Methods** 两种显式方案。

<font style="font-weight: 1000;font-size: 20px" color="orange">Forward Euler</font>

我们将连续的时间系统转换为离散形式，采用前向差分近似，即：

$$\tag{1.4.1}
\begin{align}
\frac{x^{n+1} -x^n}{\Delta t}= v^{n},
\\ M\frac{v^{n+1} - v^n}{\Delta t}=f^n.
\end{align}
$$

计算时，则可以通过已知量 $x^n$, $v^n$, $f^n$ 来确定 $x^{n+1}$ 和 $v^{n+1}$ :

$$\tag{1.4.2}
\begin{array}l
x^{n+1} = x^n + \Delta t v^n, \\
v^{n+1} = v^n + \Delta tM^{-1} f^n.
\end{array}
$$

<font style="font-weight: 1000;font-size: 20px" color="orange">Symplectic Euler</font>

Symplectic Euler 相比于 Forwar Euler 作了一点小小的改进，它采用 $v^{n+1}$ 来更新 $x^{n+1}$，即：

$$\tag{1.4.3}
\begin{array}l
x^{n+1} = x^n + \Delta t v^{n+1}, \\
v^{n+1} = v^n + \Delta tM^{-1} f^n.
\end{array}
$$

#### Implicit Time Integration

与显式积分相比，隐式积分需要求解一个方程组以确定 $x^{n+1}$ 和 $v^{n+1}$ 的值，这种方法的显著优点就是较高的稳定性。

隐式积分最简单的形式为 **Backward Euler**，它的更新规则为：

$$
\tag{1.5.1}
\begin{array}l
x^{n+1} = x^n + \Delta t v^{n+1}, \\
v^{n+1} = v^n + \Delta tM^{-1} f^{n+1}.
\end{array}
$$

在大多数场景中，力依赖于位置而产生，即力向量是位置向量的一个函数：$f^{n+1} =f(x^{n+1})$。那么对于 (1.5.1)，$x^{n+1}$ 的计算依赖于 $v^{n+1}$，$v^{n+1}$ 的计算依赖于 $f^{n+1}$，$f^{n+1}$ 的计算依赖于 $x^{n+1}$，这种相互依赖可以将问题转换为求解一个方程组：

$$\tag{1.5.2}
M(x^{n+1} - x^n - \Delta tv^n) -\Delta t^2 f(x^{n+1}) = 0
$$

由于 $f$ 和 $x$ 的关系通常是非线性的，因此求解 (1.5.2) 我们也需要使用非线性根查找技术，例如牛顿法。

牛顿法在迭代过程中，可线性近似 $f(x^{n+1})$ 为 $f(x^i)+ (x^{n+1} -x^i) \nabla f(x^i)$，其中 $x^i$ 是第 i 次迭代得到的值，它在循环开始前被赋值为 $x^n$。伪代码如下：

![[NewtonMFE.png]]

## Optimization Framework

### Optimization Time Integrator

使用 Backward Euler 时，每个时间步都需要求解一个非线性方程组，也相当于求解一个优化问题：

$$\tag{2.1.1}
\begin{array}l
x^{n+1}= argmin_x E(x) \\
E(x) = \frac{1}{2} ||x- \widetilde{x}^n||_M ^2 + \Delta t^2 P(x)
\end{array}
$$

- $\widetilde{x}^n = x^n +\Delta t v^n$
- $||x-\widetilde{x}^n||_M ^2 = \frac{1}{2}(x-\widetilde{x}^n) ^T M(x- \widetilde{x}^n)$ 
	- 表示惯性项
- $\frac{\partial P}{\partial x}(x) = -f(x)$
	- $P(x)$ 即为力 $f(x)$ 对应的 Potential Energy

我们称 $E(x)$ 为 [Incremental Potential](https://ipc-sim.github.io/)，IP 的 Local Minimum 处，即 $\frac{\partial E}{\partial x}(x ^{n+1}) =0$，对应着公式 (1.5.2)。

!!! note "将时间积分问题视为优化问题，是我们能够利用成熟的优化方法稳健的获取解"

### Dirichlet Boundary Conditions

在模拟过程中，通常需要在每个时间步控制固体上某些点的位置，例如将一组不可移动节点固定或是移动特定节点以固定路径。这种控制称为 **Dirichlet Boundary Conditions(BC)**，这些条件可以在优化中表示为 Linear Equality Constraints：

$$\tag{2.2.1}
min _x E(x),\ \ \  s.t.\ Ax=b
$$

方程 (2.2.1) 的矩阵 $A$ 通常是一个 $m\times dn$ 矩阵，其中 $m\le dn$, $m\ mod\ d =0$；相应的，$b$ 是一个 $m\times 1$ 的向量。

可以通过拉格朗日算法转换成 KKT Condition：

$$\tag{2.2.2}
\begin{align}
\nabla E(x)- A^T \lambda =0\\
Ax=b
\end{align}
$$

其中 $\lambda \in R^m$ 表示拉格朗日乘子 (Lagrange Multiplier)。

### Contact

为了准确模拟固体，确保它们不相互穿透是至关重要的。

为了避免碰撞或穿透，我们可以确保物体表面的距离永远不会减到零。我们使用不等式约束来模拟这个场景，因此时间积分问题可以表述如下：

$$\tag{2.3.1}
min_x E(x)\ \ s.t.\ Ax=b\ and\ \forall k, c_k(x) \ge \epsilon
$$

$c_k$ 表示表面特定区域对之间的 Distance，$\epsilon \rightarrow 0$ 是一个很小的正数。

对于优化问题 (2.3.1) 的局部最小处，我们遵循如下 KKT Condition：

$$\tag{2.3.2}
\begin{align}
\nabla E(x) -A^T\lambda -\sum_k \gamma_k \nabla c_k(x)=0,
\\ Ax=b, \\
\forall k,\ c_k(x) \ge \epsilon,\ \gamma_k \ge 0,\ \gamma_k (c_k(x)-\epsilon) =0.
\end{align}
$$

$\gamma_k$ 是约束 $c_k(x) \ge \epsilon$ 的拉格朗日乘子。$\nabla c_k(x)$ 指向接触对 $k$ 的接触力方向，将这个方向与 $\gamma_k$ 的大小结合，我们得到该点实际的接触力。

!!! info "Complementarity Slackness Condition $\gamma_k (c_k(x) -\epsilon) =0$"
	- 互补松弛条件确保了只有当固体接触时($c_k(x)=\epsilon$)才存在接触力，而未接触时不存在接触力($\gamma_k=0$)。
	- 互补松弛条件表明，只有 Active Set 内的约束才会表现出非零拉格朗日乘子 $\gamma_k$。这也为不等式约束优化问题带来了组合难度。

在解决不等式约束优化问题方面，有各种技术可用，每种方法都有其独特的解决方案：

- **<1> Primal-Dual Methods:** 通过迭代，利用原始问题和对偶问题之间的关系来改进初始解。
	- 有效但不一定高效、简单
- **<2> Projected Steepest Descent Methods:** 是经典最速下降法的改进，用于解决约束条件。
	- 在每次迭代中，算法沿着最速下降方向移动。如果因为约束条件而偏移，则将其投影回可行集(Feasible Set)
	- 简单直接，但是某些约束条件难以投影、收敛缓慢
- **<3> Interior-Point Methods:** 也成为 Barrier Methods，它通过 Barrier Function 来将解引导至可行区域的内部。
	- 这种方法将约束问题转换为传统无约束问题

本书专注于一个稳健而准确的 Contact Modeling Method，称为 **Incremental Potential Contact(IPC)**。IPC 特点是将接触过程近似为 Smooth Potential Energy，这种转换有效地将问题转换为无约束问题，从而可以应用各种高效的优化技术。

## Projected Newton

### Line Search

在迭代最小化的方法中，线搜索能够确保在新的点处目标能量减少，它将步长进行了划分。

具体而言，是将每一步迭代的更新 $x^i \leftarrow x$ 修改为 $x^i\leftarrow x+ \alpha( x- x^i)$ ，其中 $\alpha \in (0,1]$ 对能量 $E(x)$ 的减少是必不可少的。

Backtracing Line Search 给定了一个下降方向，通过简单的减半来找出一个大致合理的 $\alpha$ 取值：

![[BacktracingLineSearch.png]]

!!! info "Other Line Search"
	一些其它的 Line Search 方法通过插值的方式找到 $\alpha$，使得新位置能够接近局部最小值。然而，这些方法会带来更高的计算成本，并且不一定能够优化总体时间。

在结合了 Line Search 的牛顿法中，如果 Newton's Method 始终生成一个下降的搜索方向，则该方法对于一个有下界的函数一定保证收敛。

我们知道对于第 i 次迭代，搜索方向 $p= -(\nabla ^2 E(x^i))^{-1} \nabla E(x^i)$。那么，对于凸函数能量（Convex Energies），$p$ 始终是下降方向；对于非凸函数能量，则不一定总是成立。

### Gradient-Based Optimization

标准牛顿法的搜索方向通过如下优化方程计算：

$$\tag{3.3.1}
p=argmin_{\Delta x} \left( E(x^i) + \Delta x^T \nabla E(x^i) + \frac{1}{2} \Delta x^T P\Delta x\right)
$$

其中 $P=\nabla ^2 E(x^i)$。在一般情况下，我们可以使用使用其它矩阵来“代理” $P$，例如取 $P=I$，此时解得 $p=-\nabla E(x^i)$。这种近似虽然简单，但是只保留了一阶信息，收敛较慢。

理想情况下，我们希望代理矩阵 $P$ 能够更接近 Hession 矩阵 $\nabla^2 E(x^i)$。一种可行的实践是使用 Hession 矩阵的 SPD 作为代理，这可以确保搜索方向始终下降的同时，保持较高的收敛速度。

!!! info "SPD Matrix: 对称正定矩阵"
	当二次项的系数矩阵 $P$ 是 SPD 的时候，则函数的凸性得以保证，这样解出来的搜索方向始终是下降的方向。

获取此类 SPD 的简单方法是将 $\nabla^2 E(x^i)$ 投影到最近的半正定矩阵上，通过求解如下约束优化方程：

$$
min_P ||P-\nabla^2 E(x^i)||_F,\ \ s.t.\ v^TPv \ge 0,\ v\ne 0
$$

直观上来看，$P$ 是将 $\nabla^2 E(x^i)$ 的所有负特征值清零得到的：

- <1> 先对 Hession 矩阵做特征值分解 $\nabla^2 E(x^i)= Q\Lambda Q^{-1}$
	- 其中 $Q$ 的列向量是归一化的特征向量，$\Lambda$ 是特征对角矩阵
- <2> 对于 $\Lambda$ 中每个特征值，如果该特征值为负，则置零，得到新的对焦矩阵 $\hat{\Lambda}$
	- $\hat{\Lambda}_{ii} =\Lambda_{ii} >0?\Lambda_{ii}:0$
- <3> 用更新后的特征值重构代理矩阵
	- $P=Q\hat{\Lambda} Q^{-1}$

现在，我们将确保全局收敛的投影牛顿法应用到隐式积分的 Backward Euler 时间积分中：


![[ProjectedNewtonBackwardInt.png]]

!!! warning "停止条件的选取还不是很理解，TBD"

## Case Study: 2D Mass Spring *

TBD...