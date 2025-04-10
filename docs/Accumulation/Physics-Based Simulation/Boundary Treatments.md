
# Boundary Treatments

## Dirichlet Boundary Conditions

### Equality Constraint Formulation

Dirichlet Boundary Conditions(DBC) 集成到优化器中时，通常表示为 Linear Equality Constraints：

$$\tag{5.1.1}
Ax=b
$$

- $A$ 是一个 $m\times dn$ 的矩阵，$m\le dn$。
	- $d$ 是顶点的 Dimension，$n$ 是顶点个数
- $b$ 是一个 $m\times 1$ 的向量，它制定了约束规定的精确值

Dirichlet Boundary Conditions 最常见的两种类型是 Sticky 和 Slip (粘性和滑动)：

- **<1> Sticky DBC:** 在时间步内将某些节点的位置固定
- **<2> Slip DBC:** 将边界条件节点约束到特定的线性子空间内，例如一个平面或一条直线

!!! example "Sticky Dirichlet Boundary Conditions Example"
	对于一个包含两个 Node $(x_{11}, x_{12})$, $(x_{21}, x_{22})$ 的 2D System，要将第二个节点固定在点 $(1,2)$ 上，则边界条件表示为：
	
	$$\left [\begin{matrix}0 & 0 & 1 & 0 \\ 0 & 0 & 0 & 1\end{matrix}\right ] \left [ \begin{matrix} x_{11} \\ x_{12} \\ x_{21} \\ x_{22} \end{matrix} \right]= \left[ \begin{matrix} 1 \\ 2 \end{matrix} \right].$$

!!! example "Slip Dirichlet Boundary Conditions Example"
	同上节点系统，想要约束第一个节点到直线 $2x+3y=4$ 上，则边界条件表示为：
	
	$$\left [\begin{matrix}2 & 3 & 0 & 0 \end{matrix}\right ] \left [ \begin{matrix} x_{11} \\ x_{12} \\ x_{21} \\ x_{22} \end{matrix} \right]= 4.$$

如果所有边界条件在每个时间步的开始已经满足，那么优化过程中我们只需要控制边界条件的保持即可，表示为：

$$\tag{5.1.2}
A\Delta x=0
$$

其中，$\Delta x$ 是优化迭代中的**搜索方向**（节点更新时用到搜索方向和搜索步长 $x+\alpha \Delta x$）

为了在时间步强制执行 (5.1.2)，我们在求解搜索方向 $\Delta x$ 的牛顿迭代中处理这个问题。这个过程涉及使用 Incremental Potential 的二次近似来形成拉格朗日函数：

$$
L(\Delta x, \lambda)= \frac{1}{2}\Delta x^T H\Delta x+ g^T \Delta x + \lambda^T A\Delta x
$$

在这里，$g,H$ 分别是 IP 的 Grad 和 Hessian（一阶导和二阶导）；$\lambda$ 是一个 $m\times 1$ 的拉格朗日乘子向量。

通过最大-最小优化 $L(\Delta x, \lambda)$ 来处理这个问题：

$$
\max_{\lambda} \min_{\Delta x} L(\Delta x, \lambda)
$$

!!! info "我们一方面想要 IP 最小，一方面想要保持 (5.1.2)"

经过合理求解后，转换成如下重要 KKT 系统：

$$\tag{5.1.3}
\left[\begin{matrix} H & A^T \\ A &  \end{matrix}\right]\left[\begin{matrix} \Delta x \\ \lambda \end{matrix}\right] = \left[ \begin{matrix} -g \\ 0\end{matrix} \right].
$$

!!! warning "该系统不是对称正定的，并且其大小锁着边界节点数量增加而增加"

### DOF Elimination Method

对于最简单的 Sticky BC，其约束矩阵 $A$ 仅起到了选择的作用，具体来说就是只在 BC 节点对应处为 1，其余为 0。

由此，可以证明得到 $AA^T$ 为一个 $m\times m$ 的单位矩阵；$A^TA$ 是一个 $dn\times dn$ 的对角矩阵，只有 BC 节点的对应项为 1，其余为 0。

知道如上信息后，我们对 (5.1.3) 的上半部分左乘一个 $A$：

$$
\left[\begin{matrix} AH & AA^T \\ A &  \end{matrix}\right]\left[\begin{matrix} \Delta x \\ \lambda \end{matrix}\right] = \left[ \begin{matrix} -Ag \\ 0\end{matrix} \right].
$$

可以直接解出 $\lambda$：

$$\tag{5.2.1}
\lambda = -AH \Delta x - Ag
$$

将 (5.2.1) 代回 (5.1.3) 解出 $\Delta x$：

$$\tag{5.2.2}
(I-A^TA)H \Delta x = (I-A^TA) (-g)
$$

此处 $I-A^TA$ 矩阵相当于“屏蔽”了那么 BC 节点的行，即只保留了非约束节点的信息。

实际上，由于第二个块行在原方程中已经给出了 BC 节点的 $\Delta x$（这些节点是已知的，一般为 0，即保持原位置），因此（5.2.2）是一个欠约束系统。考虑到这点，可以将整个系统按照 BC 节点和非 BC 节点（记作 B 和 U）的方式进行分块：

$$H=\left[\begin{matrix}H_{BB} & H_{UB} \\ H_{BU} & H_{UU} \end{matrix}\right],Δx=\left[\begin{matrix}Δx_B \\ Δx_U\end{matrix}\right],g=\left[\begin{matrix}g_B \\ g_U\end{matrix}\right].$$

由于 BC 节点的位移变化 $\Delta x_B = 0$（固定边界条件），我们可以把系统简化为只求解非约束节点的方程：

$$H_{UU}\Delta x_U = -g_U. \tag{5.2.3}$$

这就是一个对称正定（SPD）的线性系统，只包含非 BC 节点，从而大大降低了求解规模和复杂度。

上述方法主要用于数学解释，在实际应用中，构建 (5.2.3) 方程涉及到重新排列自由度等繁琐低效的过程，通常被避免。

为了避免重新排列自由度的需要，可以对原始线性系统直接进行修改，使其与方程 (5.2.3) 一致。具体而言：

- 将 $g$ 内对应边界条件节点的行都置 0
- 对于 $H$ 中与 BC 节点相关的行和列，对角项设置为 1，其余置 0

![[DOFEME521.png]]

## Slip Dirichlet Boundary Conditions

与 Sticky DBC 不同，一般的 Slip DBC 不能通过 DOF Elimination Method 解决。为了应对这种复杂性，我们可以采用基变换策略。

### Axis-Aligned Slip DBC

在深入研究更一般的情况前，我们首先考察那些轴对齐的滑动 DBC。

轴对齐滑动 DBC 的一个优点就是它们的约束矩阵和粘性 DBC 相似，因此也可以通过相同的自由度消除方法进行管理求解：

![[DOFESlipDBCE1.png]]


### Change of Variables

在处理一般 Slip DBC 时，一个局限性体现在 $AA^T$ 不是一个 $m\times m$ 的单位矩阵，我们同上进行拉格朗日处理：

$$\tag{6.2.1}
\lambda =-(AA^T) ^{-1} AH\Delta x - (AA^T) ^{-1} Ag
$$

$$\tag{6.2.1}
(I-A^T (AA^T) ^{-1} A) H\Delta x = (I- A^T (AA^T) ^{-1} A) -g
$$

这里 $I-A^T (AA^T) ^{-1} A$ 没有特殊的结构，这使得自由度消除方法难以应用。

为了简化约束，我们对约束矩阵 $A$ 进行**奇异值分解(SVD)**：

$$
A=USV^T
$$

- $U$ 是 $m\times m$ 的正交矩阵
- $V$ 是 $dn\times dn$ 的正交矩阵
- $S$ 是 $m\times dn$ 的对角矩阵

通过定义 $y=V^T\Delta x$ 和 $\lambda ' =U^T\lambda$，我们将 KKT 系统 (5.1.3) 重新格式化：

$$\tag{6.2.3}
\left[\begin{matrix} V^THV & S^T \\ S & 0 \end{matrix}\right]\left [\begin{matrix}y \\ \lambda ' \end{matrix}\right]= \left[\begin{matrix} -V^Tg \\ 0\end{matrix} \right]
$$

一旦我们解出 $y$ ，那么 $\Delta x$ 可以通过 $\Delta x=Vy$ 轻松恢复。

!!! danger "Limitations of SVD"
	- 对大矩阵执行 SVD 具有挑战性，impractical
	- 正交矩阵 $V$ 通常是密集的，那么对 $V^THV$ 进行变量替换计算效率较低、成本较高

### General Slip DBC

对于一个在 3D 平面上互动的节点 $i$ 的 DBC，可以表示为：

$$
n_i^T \Delta x_i =0
$$

其中 $n_i$ 是滑动平面的法线。对向量 $n_i$ 进行 SVD，我们得到：

$$\tag{6.3.1}
n_i^T= U_i S_i V_i^T =1 \left[\begin{matrix}1 & 0 & 0\end{matrix}\right] \left[\begin{matrix} n_i^T \\ m_i^T \\ l_i^T\end{matrix}\right]
$$

单位向量 $n_i$, $m_i$ 和 $l_i$ 组成了 3D orthonormal basis（三维正交归一基），这种分解有助于我们在后续处理中有针对性的将“法向”方向变化置 0，而保留切向方向。

!!! info "正交归一基"
	内积空间中一组向量基底中每个向量模长都为 1，且每两个向量都正交

接下来对 Hessian 矩阵等进行处理，得到可以使用 DOF 消除法进行处理的系统。

!!! danger "此处处理没能理解清楚，因此就不写到这了，请自己去看[原文](https://phys-sim-book.github.io/lec6.3-general_slip_DBC.html)"

![[GeneralSlipDBCE1.png]]

## Distance Barrier for Nonpenetration

Contact Modeling 是确保固体不与障碍物或自身相交的关键。本节我们将深入探讨 IPC 框架中非穿透性具体细节。

