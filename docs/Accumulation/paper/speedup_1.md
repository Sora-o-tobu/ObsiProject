
# Linear Subspace Design for Real-Time Shape Deformation

> https://dl.acm.org/doi/10.1145/2766952

## Linear Subspace

我们基于 non-convex input subregion $\Omega \in R^d\ (d=2,3)$，静止状态的 vertices 用 $\bar{V} \in R^{n\times d}$ 表示，变形后的 vertices 用 $V\in R^{n\times d}$ 表示，其中 $n$ 是顶点的个数。

本方法通过在子空间中结合 region handles 和 point handles 进行控制来同时获得以下两点优点：

- 只有 region handles 时，子空间类似于传统的 skinning
- 只有 point handles 时，子空间对应于广义重心坐标

假定 user 手动选取了 $m_p$ 个 control points 和 $r$ 个 regions（对应有 $m_r$ 个 vertices），我们将与控制柄相关的所有**静止**状态的顶点位置聚集起来，用 $\bar{C} \in R^{m\times d}$，其中 $m= m_p + m_r$。

- 对于 $m_p$ 个独立控制点，每个点是一个自由点，在一个 $d$ 维空间移动，贡献了 $d$ 个自由度（DoFs）。
- 对于 $r$ 个控制区域，每个区域通过仿射变换进行控制，包括 $d^2$ 自由度的旋转、缩放、剪切等线性变换以及 $d$ 个自由度的平移变换，因此每个区域贡献 $d^2+d$ 自由度

将上述两部分相加，我们得到这个子空间的总维度：

$$
m_p\times d + r(d+1)d
$$

我们将控制点的目标位置按行存储于 $C_1 \in R^{m_p\times d}$，将控制区域中点的目标位置存储为 $C_2 = J_2 H_2 \in R^{m_r\times d}$，其中 $J_2\in R^{m_r \times r(d+1)}$ 是一个 Rigid Skinning Matrix。我们将目标位置 $C$ 组合成如下矩阵积：

$$
C = \left [\begin{matrix} C_1 \\ C_2\end{matrix}\right ] = \left [\begin{matrix} I_{m_p} & 0 \\ 0 & j_2 \end{matrix}\right ]\left [\begin{matrix} C_1 \\ H_2 \end{matrix}\right ] \tag{1}
$$
结果矩阵 $C$ 包含了控制柄的所有 target positions，而矩阵 $H\in R^{h\times d }\ (h=m_p + r(d+1))$ 汇集了子空间的所有参数，它的维度即为我们之前所推导的自由度。

### Variational form

现在假定我们有一般的等式约束二次优化问题，有如下类似形式：

$$
V=arg\min_{X\in R^{n\times d}} \frac{1}{2}trace(X^T AX)\ \ subject\ to\ \ SX=JH \tag{2}
$$


| 符号                               | 含义                                                |
| -------------------------------- | ------------------------------------------------- |
| $X \in \mathbb{R}^{n \times d}$  | 网格顶点的位置矩阵（优化变量）                                   |
| $A \in \mathbb{R}^{n \times n}$  | 二次能量矩阵，衡量变形“自然性”（如 Laplacian、biharmonic、ARAP 等）   |
| $S \in \mathbb{R}^{m \times n}$  | 行选择矩阵，选出控制柄对应的顶点（包括 control points 和 regions 中的点） |
| $JH \in \mathbb{R}^{m \times d}$ | 控制柄目标位置。来自公式 (1)：将控制参数 $H$ 映射为控制顶点的目标位置           |
| $V \in \mathbb{R}^{n \times d}$  | 最终的解，即满足变形能量最小和控制约束的顶点位置                          |

定义矩阵 $T\in R^{(n-m)\times n}$ 为选择除了控制柄以外的所有顶点，那么此时有：

- $S X \in \mathbb{R}^{m \times d}$：**受控制的点**，由控制参数决定；
- $T X \in \mathbb{R}^{(n-m) \times d}$：**自由变量(要优化的部分)**，未知变量。

接下来，我们将 $X=SX+TX$ 代入并对优化变量 $TX$ 求导，得到如下答案：

$$\begin{array}l
W= S^TJ - T^T (TAT^T) ^{-1} TAS^TJ \\
V= WH
\end{array} \tag{3}
$$

!!! note "$V$ 即优化方程得到的变形后位置"

### Affine invariance

我们希望证明这个优化方法具有仿射不变性。

- **旋转不变性:** 如果我们对参数 $H$ 右乘一个旋转矩阵 $R$，即对控制柄整体做一个刚体旋转，那么最终的变形网格也会被施加同一个相同的旋转：$V=W(HR)=(WH)R$
- **平移不变性:** 要求能量项的选取 $A$ 的每一行的元素加起来为 0，即 $A$ 的零空间包含全 1 向量（$A1_n=0$）。
	- 传统的二次能量项不一定能够保证上述零空间假设，不满足 Linear Precision

## Linearly precise smoothness energy

仿射不变性一节中提到对于优化的能量项需要满足 Linear Precision，本论文接下来介绍了一个 Linearly Precise 平滑能量项。

!!! info "并且保持矩阵的稀疏性，以保证求解线性系统的速度"

对于经典的拉普拉斯项，它惩罚每个点和邻居之间的不一致，从而鼓励表面平滑变化。它的能量形式一般为：

$$
x^T L^T M^{-1}Lx \tag{4}
$$

- $L$：离散拉普拉斯矩阵（常用 cotangent 拉普拉斯）
- $M$：质量矩阵（对角线矩阵，存储每个顶点的体积或面积）
- $x$：当前所有顶点的一维坐标（比如 $x \in \mathbb{R}^n$）

理想的平滑能量不应该惩罚一个全局平移或者线性变换（缩放、旋转等），它们都应该满足常数精确性，即 $L1_n=0$。但是，大多数标准 cotangent laplacian 在**边界点**处并不满足线性精确性，这会导致一个全局平移或线性变换在边界点附近会被判定为不平滑。

!!! example "以前的一些能量项设置导致的结果"
	![[speedup_1_1.png]]

实际上，这是由于标准 cotangent laplacian 的边界边特殊定义导致的：

- 对于内部边：

$$
L_{ij}= \frac{1}{2} (\cot \alpha _{ij} + \cot \beta _{ij}) 
$$

- 对于边界边：

$$
L_{ij} = \frac{1}{2} \cot \alpha _{ij}
$$

也就是说，对于边界项少了一个 cotangent，因为它只被一个三角形包含。这会导致能量不对称，不能完全捕捉到线性函数的行为。

为了修复这一行为，我们希望对边界项增加法向导数补偿。论文中引入了新的矩阵：

$$
K= L +N
$$

我们将 Normal Derivative $N$ 添加到所有边界点对应的 row 上，从而得到一个 Linear Precise Cotangent Laplacian $K$。

$N_ix$ 表示顶点 $i$ 的法向导数估计值，是四个 cotangent 乘以相邻顶点差值权重：

$$
N_ix = \frac{\cot \gamma_{ik}}{2} (x_i - x_k) + \frac{\cot \gamma_{jk}}{2} (x_j - x_k) + \frac{\cot \gamma_{iq}}{2} (x_i - x_q) + \frac{\cot \gamma_{pq}}{2} (x_p - x_q)
$$

其中 $j,k$ 是与 $i$ 相邻的边界点，而 $p,q$ 为相应的 triangles 的第三个点。

!!! info "因为是边界点，所以只有两个相邻点；$\gamma_{ab}$ 表示边 $ab$ 对面的角"

??? warning "对于四面体形 mesh，会更加复杂一些，此处不再打字了"
	![[speedup_1_2.png]]

将 $K$ 替换原拉普拉斯平滑能量项中的 $L$ 后，得到我们想要的 Linearly Precise Laplacian Smoothness Energy：

$$\begin{array}l
A & = K^T M^{-1} K \\
E & = x^T Ax
\end{array}
$$

这个能量具备两个好处：

1. **常数精确性**：即 $A \cdot \mathbf{1} = 0$
2. **线性精确性**：即 $A \cdot x = 0$ 对所有线性函数 $x$ 成立

??? danger "上述定义的 $K$ 可能导致负权重，但这并非劣势，强制非负会导致插值不平滑、求解更慢等"
	![[speedup_1_3.png]]

## Domain reduction for non-linear optimization

很多几何建模或物理仿真的方法都涉及非线性优化，例如 As-Rigid-As-Possible 形变(ARAP)、弹性模拟等。这类优化在高分辨率网格下十分耗时，**Domain Reduction** 将问题限制在一个低维线性子空间中，从而显著加快计算速度。

关于这些应用，可以看原论文中的 Section 4. Results 部分。

