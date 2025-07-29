
# Progressive Shell Quasistatics for Unstructured Meshes

> https://pcs-sim.github.io/psq/


## Progressive Simulation

### Introduction

渐进式模拟通过一系列分辨率不断提升的网格层次生成具有一致性且逐渐改进的解决方案预览，并在最终的高分辨率网格上生成收敛的模拟输出。

每层结构由一组三角形网格和对应的**延拓算子**（prolongation operators）组成，延拓算子将该层的网格映射到下一层更高分辨率的网格上。

!!! note "将粗粒度编辑传播到细粒度（或等价地，将细粒度细节重新引入变形粗表面）需要在粗表面和细表面之间建立映射"

每层网格通过下标 $l\in [0,L]$ 索引，$x_l, \bar{x}_l$ 分别表示第 $l$ 层网格 $n_l$ 的 deformed position 和 rest position，$P_{l+1}^l \in R^{3n_{l+1}\times 3n_l}$ 表示将层级 $l$ 映射到 $l+1$ 的延拓算子。

!!! tip "那么最粗糙（coarest）的网格的变形位置就存储于 $x_0$"
	为了简化讨论，我们在论文中所用的不带下标的变量通常指最高层（finest-level）分辨率的属性，例如 $x=x_L, \bar{x} =\bar{x}_L, n=n_L$。

对于每一个 simulation mesh，论文定义了多种势能项，并将它们加总构成总势能：

$$E_l(x) \;=\; \Psi_l(x) \;+\; B_l(x) \;+\; D_l(x) \;+\; S_l(x).$$

- $\Psi_l$​：薄壳弹性势能（Shell energy），刻画网格本身的拉伸、弯曲等弹性变形。
- $B_l$​：接触屏障势能（Barrier potential），用于保证不同部位或不同物体之间不发生穿透。
- $D_l$​：摩擦耗散势能（Dissipation potential），用于在接触面上引入摩擦力。
- $S_l$​：应变限制势能（Strain-limiting potential），可选项，用以防止网格单元产生过大应变。

材料参数、边界条件等统一收录在参数向量 $u$ 中，这些参数在仿真过程中可能会发生改变（例如施加载荷、转动边界等）。

在一个 time-stepping 中，有当前状态 $x^t$ 和当前参数 $u$，progressive simulation 使用隐式欧拉法计算梯度流（gradient flow），将梯度流演化为一系列增量的**能量最小化**：

直接模拟连续流较为复杂，论文用隐式欧拉（Backward Euler）做时间离散，将梯度流演化为一系列增量的**能量最小化**：

$$x^{t+1} =\arg\min_{x}\; \underbrace{\frac{1}{2h^2}\,\|x - x^t\|_M^2}_{\text{惯性项}} \;+\; E(x,\,\bar x,\,u^{t+1}) \tag{1}$$
- $h$: 时间步长
- $\frac{1}{2h^2}\,\|x - x^t\|_M^2$: 惯性项，防止新状态偏离太远
- 当满足梯度范数足够小 $\nabla E(x^*)\|\le\varepsilon$ 时，认为已收敛到平衡态

为了避免直接在高分辨率网格上进行昂贵的求解，论文提出一种**单向非线性多分辨率求解**（one-way nonlinear multiresolution），包含**两大子流程**：

- **<1> Preview:** $x_l^t \rightarrow x_l^{t+1}$，在第 $l$ 层计算结果，分辨率越粗，计算越快，但精度越低
- **<2> Refinement:** $x_l^{t+1} \rightarrow x_{l+1}^{t+1}$，将 $l$ 层结果延拓到第 $l+1$ 层

### Coarse-Level Proxy Energies

在多分辨率仿真中，直接在粗网格上使用最细网格的完整势能（包括壳弹性 Ψ、接触屏障 B、摩擦 D、应变限 S）往往不够合理——

- **壳弹性** Ψ 依赖细网格的曲率和拉伸模式，在粗网格上无法准确评估；
- **接触／应变限制** 相关的屏障项 B、D、S 则完全丢失了最细格可能出现的局部接触与过伸展风险。

因此，论文在每个粗层 $l\lt L$ 定义了一个“代理”（proxy）势能 $F_l(x_l)$，用它在粗层上同时近似又保证可行性地模拟细层行为。公式(2)给出了第 $l$ 层的代理势能：

$$\tag{2}
F_l(x_l) = \underbrace{B_l(x_l) + D_l(x_l) + S_l(x_l)}_{C_l(x_l)\;(=\text{Coarse barrier terms})} \;+\; \underbrace{\Psi_l\bigl(P^l(x_l)\bigr)}_{\text{Shell elastic at fine level}}
$$

- $C_l(x_l)$（粗层屏障项）：
    - $B_l$：接触屏障，防止粗层节点出现自穿透或与其他物体相交；
    - $D_l$​：摩擦耗散，粗层接触面上的摩擦能量；
    - $S_l$​：应变限制，在粗层上抑制过度拉伸（用粗网格上的单元应变计算）。
    - 这三者合起来保证了解在当前网格拓扑下的**可行性**（feasibility），即没有穿透、没有过伸展。
- $\Psi_l(P^l(x_l))$（延拓后在最细层评估的壳弹性能量）：
    - 在最细层上计算壳弹性 Ψ，能够准确捕捉弯曲、扭曲等机制。

这样，代理势能既保留了顶层（最细层）壳弹性对形状质量的严格约束，又只用粗网格上的计算来维持整体可行性。

因此，每层的 Preview 步骤可以表示为如下优化方程：

$$
x^{t+1}_l =\arg\min_{ x_l}\; {\frac{1}{2h^2}\,\|x_l - x^t_l \|_{M _l}^2} \;+\; F_l (x_l) \tag{3}
$$

### Refinement and Safe Initialization

在得到 $l$ 层的 Preview 后，我们根据延拓算子计算得到下一层的 Safe Initializer：

$$
x_{l+1}^t = P_{l+1}^t (x_l^*)
$$

!!! note "下一层的 Initializer 要尽可能靠近 $x_{l+1}^t$"

对于精细化的过程，可能存在精细化后的 mesh 违反了 contact constraints 或 strain limits 等限制。因此我们在 $l+1$ 层也要继续进行如 $(3)$ 的 Progressive Simulation。

## Progressive Simulation For Shells

论文的目标是实现一个渐进式（progressive）模拟框架，适用于 **结构化** 和 **非结构化的三角网格**。这个目标要求他们设计出：

- 一个合适的**细化（prolongation）操作符**；
- 一种**安全的初始化方式**，满足两个条件：
	- **内在形变**（in-plane，主要是拉伸）和**外在形变**（extrinsic，主要是弯曲）都能被模拟出来，且符合物理壳体模型；
    - 模型在层级中的**每一层都保留未变形的参考形状**（rest shape），防止因下采样或插值导致“幽灵力”或不真实的形变反应。

### Decimation

我们将初始 Mesh 设置为 $M_L$，即 finest level，并通过一系列 Edge-Collapse Decimation 建立 $M_{L-1},...,M_0$，从而得到各个层初始的 rest positions $\bar{x}_l$。

!!! quote "Edge-Collapse Decimation"
	- **Quadric Error Metrics (QEM)** [Garland and Heckbert 1997]；
	- **Probabilistic Quadrics** [Trettner and Kobbelt 2020]，使得简化效果更自然、更高质量。

然而，现存的几种细化方法（包括 Intrinsic Prolongation 和 Affine Prolongation）都难以在壳体模拟中应用，并且它们提供的 Linear Operator 缺乏稀疏性，导致难以在 High Resolution Mesh Simulation 中应用。

对于我们希望的 Prolongation Operator，我们希望具有如下三个属性：

- **<1> Rest Shape Recovery:** Prolongation 需要在所有层级上保持 Rest Shape
- **<2> Shell-model Consistency:** Prolongation 需要同时放大内在（拉伸）和外在（弯曲）变形模式的采样
- **<3> Efficiency:** 渐进式模拟在每次 force and refinement evaluation 都需要调用 Prolongation，因此这个操作必须要在这两种操作中都保持 Inexpensive

#### Special Case

为了便于理解，我们先只考虑最简单的层级构建方法：Single-Edge Collapse 开始。那么 Rest-Shape Prolongation 可以简单使用 Edge Expansion 进行恢复。

??? quote "Single-Edge Collapse & Intrinsic Coordinate"
	![[speedup_2_1.png]]

我们使用 Intrinsic Prolongation 中的 Self-Parameterization 方法去定义内在坐标（Intrinsic Coordinates），从而锚定 $l+1$ 层两个分裂的新节点的位置 $\bar v_{l+1}^i, \bar v_{l+1}^j$。

接下来我们还要记录 $l$ 层和 $l+1$ 层之间的 Extrinsic Difference $\bar d^i = \bar x_{l+1}^i - \bar v_{l+1}^i$，其中 $\bar v_{l+1}^i$ 是从 $l$ 层精细化映射后的位置，$\bar x_{l+1}^i$ 是对应点在 $l+1$ 层的 rest shape 中的位置。

!!! note "Extrinsic Difference 只记录一次，记录其在 3D 空间中偏移了多少"

对于包含 vertex $i$ 的三角形，它的三个顶点的静止位置表示为 $\bar x_0, \bar x_1, \bar x_2$，我们将 Extrinsic Difference 分解为法向贡献（out-of-plane）和切平面内贡献（in-plane）：

- **out-of-plane:** $\gamma^i = \bar{n}^T \bar{d}^i$
- **in-plane**：$t^i \in \mathbb{R}^2$
	- 是一个二维“向量” $(a,b)$

那么 $\bar d^i$ 可以表示为两个贡献的线性组合：

$$\begin{array}l
& \bar d^i = \bar n \gamma^i + \left ( \bar e , \bar n \times \bar e \right) t^i
\\ with &  \bar e = (\bar x_1 -\bar x_0) / ||\bar x_1 -\bar x_0 ||
\end{array}
$$

在模拟过程中，Shell Deformation 导致如上两个组成部分相应改变：

$$
\begin{array}l
x_{l+1}^i = v_{l+1}^i + a^i(x _l) \\
a^i(x_l) = n^i(x_l) \gamma^i + R^i(x_l) t^i 
\end{array}
$$

其中 $(\gamma^i, t^i)\in \mathbb{R}^3$ 只用计算一次。

#### Common Case

在一般例子中，Edge-Collapse 可能会进行多次（通常嵌套进行），但是我们仍然能够使用 Linear Operation 来计算 *anchor location*：$v_{l+1} = U_{l+1}^l x_l$。

对于 $m_l$ expanded nodes，我们将所有非线性偏移计算表示为：

$$
a_{l+1}^l (x_l) = \left [ a^1 (x_l)^T, a^2 (x_l)^T,...,a^{m_l} (x_l)^T\right ]^T \tag{6}
$$

那么每层的 shell-prolongation operator 为：

$$
x_{l+1} = P_{l+1}^l (x_l) = U_{l+1}^l x_l + a_{l+1}^l (x_l) \tag 7
$$

#### Direct Prolongation

上节讲的都是两个相邻层级之间的 Prolongation $P_{l+1}^l(.)$，实际上 mapping 也允许我们建立不相邻层级之间的 **Direct Prolongation**，定义 $P^l(.)$ 表示从层级 $l<L$ 直接延拓到 finest mesh 的算子。

和之前类似，我们预先计算 level $l$ 和 level $L$ 之间的 extrinsic differences 和 pre-vertex displacements $(\gamma^i, t^i)$。通过 repeated composition，得到相应的线性算子 $U^l = U_L^{L-1} ...U_{l+1}^l$。接下来的公式则类似 (6) 和 (7)：

$$
a^l (x_l) = \left [ a^1 (x_l)^T, a^2 (x_l)^T,...,a^{m} (x_l)^T\right ]^T \tag{8}
$$

Direct Prolongation Operators 为：

$$
x = P^l (x_l) = U^l x_l + a^l (x_l) \tag 9
$$

需要注意，这种 Direct Prolongation 并不等效于逐层应用延拓，虽然它们都能正确还原出相同的 rest-shape，但是在 simulation 中施加的形变路径不同。

### Safe Initialization

!!! quote "Safe Initialization"
	![[speedup_2_2.png]]

在逐层细化的仿真过程，我们需要提供一个安全（即没有穿透或违反应变限制）的初始化状态。

总体思路是先构造 $l+1$ 层的一个非退化、安全的、无自相交的 initial expansion，然后向 Prolongation 的目标位置做 CCD 搜索。

#### Safe Initial Expansion

在 Initial Expansion 的过程中，我们对初始要分开为 vertex $i,j$ 的节点 $k$ 做一个初始扰动，从而确保不会影响到 CCD 的碰撞检测。遗憾的是，在 3D 空间中，即便是一个微小的扰动也有可能造成严重的 self-intersection 或者 global intersection。

为了获得一个 initial safe perturbation per expansion，作者采用随机分层采样法（randomized stratified sampling）：

第一步：first-attempt 试探扰动

- 尝试先按目标方向扰动 i 和 j，但**只在 patch 局部内检查是否交叉**。
- 扰动幅度由某个安全上界控制，确保**不会和别的 patch 交叉**（global intersection-free）。

如果成功：

- 即局部也无交叉，则用此作为 safe initial，进入 CCD 搜索阶段。

如果失败（存在局部自交）：

- 开启**随机采样阶段**：对 j 随机选取扰动方向（仍然用小幅度），一一尝试。
- 每次采样后检查是否产生局部交叉。
- 如果没有交叉，就停止，采样成功。

!!! danger "为什么对 j 随机选取扰动方向尝试？还没搞懂"

#### CCD-filtered Geometric Expansion

在完成如上 safe initial expansion 操作，确保边长非零的基础上，我们接下来详细讲述如何通过 CCD safe-stepping 安全地扩张这些 edges 到它们的 prolonged geometry targets。

!!! tip "在下一节我们将讲述最终的并行扩张算法，本节先讲述它们的 sequential 做法"

我们应注意到，在进行 refinement 操作前，上一层通过 IPC 得到的方法可以确保表面两两之间存在一定 gap distance，我们计算得到 minimal-gap $g$，它的范围通常是 1e-3 ~ 1e-4。

随后我们将每个新插入的顶点沿着它们的 safe perturbation direction 移动一小段距离，这个距离要远小于 $g$。这能够确保全局非相交安全性，而无需进行昂贵的全局相交检测和处理。

!!! note "Safe Perturbation Direction：即上一步中的扰动方向"

- 在正式 CCD 前，会用普通的空间哈希 + CCD 粗略阶段（broad-phase）来提前筛除相距很远的元素对。
- Additive-CCD 接受一个参数 $r$，用来控制“允许靠近程度”。对于留下的不同距离碰撞候选对，我们为其采取不同策略：
	- 初始距离 $d_0$，新得到的距离 $d_1$，ACCD 保证 $d_0 \ge rd_0$，即 $r$ 越接近于 1 则对步长限制得更多

| 初始距离 $d_0$                    | 对应 r 值​   | 原因                 |
| ----------------------------- | --------- | ------------------ |
| $d_0 \ll g$                   | r = 0.999 | 不希望再让这对靠得更近，步长要非常小 |
| $d_0 \approx g$ 或 $d_0 \gg g$ | r = 0.1   | 可以允许大步一点，推进速度更快    |

- 这种策略在不同类型的顶点对上做 trade-off：防止相交 vs 保持速度。

根据 ACCD 移动过后，我们还需检查每对碰撞候选的之间的最终距离 $d_1$，如果 $d_1 \lt g$，或者出现了网格退化等不合理情况，我们会在局部 patch 区域内增大 contact stiffness 进行多次牛顿迭代，从而恢复过近碰撞对之间的间隙。

!!! info "这种按块优化的运算是一种微小且局部的开销，由于我们 ACCD $r$ 的设置，它在大多数例子中不会进行"

#### Safe Parallel Expansion

TODO

#### Stain Limiting Feasibility

虽然经过上述 Edge-Expansion 已经得到了一个 intersection-free 的目标网格，但这个网格仍然有可能不满足材料的 Strain Limits。

!!! warning "特别是在处理具有最大伸展限制的壳体材料中"

具体方法直接采用 PCS 论文，该论文中没有详细讲解。

### Quasistatic Stepping

牛顿迭代中，projected Hessian 矩阵的形式如下：

$$
H(x_l) = \frac{1}{h^2} M_l + ProjectPSD\left(\nabla^2 E(x_l)\right)
$$

每轮迭代的搜索方向 $p$ 由如下线性系统求解得到：

$$
H(x_l) p = - \left( \frac{1}{h^2}M + (U^l) ^T \nabla \Psi (U^l x_l + d_l) + \nabla C(x_l) \right) \tag {10} 
$$

右侧是当前增量势能的梯度项（包括变形势能 $\Psi$ 和约束项 $C$）：

- $\Psi$：主势能（如弹性能）
- $C$：硬约束（如不可穿透条件）
- $U^l$：prolongation 相关变换矩阵 
- $d^l$：当前位移下的偏移

这一阶段并不需要精确解，我们可以为其设置最大牛顿迭代次数，目标是快速收敛 + 稳定过渡 + 中间状态可视化。最后一层我们才需要完整迭代至收敛，要求最后一次迭代步长小于容差。



### Progressive Collision Objects

