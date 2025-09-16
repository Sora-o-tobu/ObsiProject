
# Surface Multigrid via Intrinsic Prolongation

> [Surface Multigrid via Intrinsic Prolongation](https://www.dgp.toronto.edu/projects/intrinsic-prolongation/)
>
> 传统 Linear Solver 经常成为几何处理算法的瓶颈。特别是在每次迭代线性系统都会改变的场景中，direct solvers 都需要昂贵的 re-factorization。对于结构化场景上的问题，可以使用 **geometric multigrid** 方法，它的求解器只根据网格几何进行预处理，而不关心线性系统的改变，从而保证了线性处理时间。
>
> 本论文提出了为流形网格设计的 **Galerkin Geometric Multigrid Solver**，使用我们的求解器代替 direct solver 可以带来数量级上的提升，从而将运算瓶颈从 Linear Solver 上移开。


## Multigrid Overview

Multigrid 是一种可拓展的 Iterative Solver，用来求解大型线性系统 $Ax=b$。其核心的操作有两个：*relaxation* 和 *coarse-grid correction*。

- Relaxation 涉及将经典迭代方法应用到校正当前解和系统精确解之间的**高频误差**
- Coarse-grid correction 使用限制将**低频误差**传递到更粗糙的网格，在粗网格系统上求解系统，然后通过 prolongation 将修正传递回细网格

!!! note "因此如何建立 Multigrid Hierarchy 以及如何在网格层级之间传递信息是该求解器效率的关键"

我们的 geometric multigrid 基于 *Garlerkin coarse grid approximation*，它在每个粗网格层次上建立线性系统：

$$
A_{\text{coarse}}= RA_{\text{fine}}P
$$

其中 $A_{\text{fine}}$ 和 $A_{\text{coarse}}$ 分别是细网格和对应粗网格上的系统矩阵，$P$ 是从粗网格到细网格的延拓算子，$R$ 是对应的限制算子（当 $A_{\text{fine}}$ 为对称矩阵时，通常取为 $R=P^T$）。

!!! note "延拓算子 $P$ 是多重网格性能的关键，它既要在细网格上准确插值低频解，又要保持稀疏以提高效率"

## Intrinsic Prolongation

我们通过保持 intrinsic parameterization 来计算 prolongation。

粗细网格之间的映射时 bijective 的。给定细网格上一个点，我们可以得到它在粗网格上相应的 barycentric coordinates，反过来也是如此。

### Successive Self-Parameterization

本论文中，$\mathcal M^0$ 表示 finest mesh，$\mathcal M^L$ 表示 coarsest mesh，$f_{l+1}^l: \mathcal M^l \rightarrow \mathcal M^{l+1}$ 表示两层之间的 bijective map。那么 final map 可以表示为：

$$\begin{array}l
f_L^0: \mathcal M^0 \rightarrow \mathcal M^L \\
f_L^0= f_L^{L-1}\circ \dots \circ f_1^0
\end{array} \tag 2
$$

对于每一次 edge collapse，除了 collapsed edge 邻边，大部分三角形都保持不变。$\mathcal N^l(k)$ 表示 level $l$ 上顶点 $k$ 的相邻点（包括 $k$ 自己），相应的，使用 $\mathcal N^l(i,j)= \mathcal N^l(i) \cup \mathcal N^l(j)$ 表示 edge $i,j$ 的相邻点。

!!! info "$\mathcal N^l(k)$ 称为 1-ring 邻域，形成一个局部 patch"

假定 $l$ 层的 edge $i,j$ 折叠成 $l+1$ 层的 vertex $k$，那么有一个关键观察为 $\mathcal N^l(i,j )= \mathcal N^{l+1} (k)$

### Joint Flattening

如果我们只是随机选取一个 coarse 三角形作为 fine 顶点的重心坐标基准，选取过程会变成随机/启发式，缺乏一致性，导致误差积累严重。

因此我们直接 flatten 整个局部 patch，在边折叠前后，这个 patch 的闭合曲线完全一致，即可以在 UV 域上对齐，因此这个映射是 bijective 的。

> 接下来的内容由 GPT 总结生成，因为我自己的实现没有展平整个 patch，因此以后再来看如何实现的

第一步：构造联合优化问题（共享边界的双补丁展平）

**变量与补丁：**

+   令 $V^l, F^l$ 是折叠前补丁（边 $(i,j)$ 的 1-ring）里的顶点/三角形集合；$V^{l+1}, F^{l+1}$ 是折叠后补丁（顶点 kkk 的 1-ring）。
+   令 $\partial P^l$ 与 $\partial P^{l+1}$ 是两块补丁的**边界顶点环**（注意：折叠前后这两个边界环在 3D 上是同一圈点，只是中间的三角剖分不同——这是关键观察）。
+   我们为两块补丁各自引入 UV 变量：$\mathbf{U}^l\in\mathbb{R}^{2|V^l|}$。

**共享边界：joint variable**

+   为了强制“边界一致”，作者不把它当成约束去加拉格朗日乘子，而是**直接引入一组“联合边界变量”** $\mathbf{U}\_b$​，把两块补丁边界顶点的 UV **都引用同一份自由度**。这样，原来“$\mathbf{U}^l_{\partial} = \mathbf{U}^{l+1}_{\partial}$​”的等式约束被“消元”成共享变量，从而把**带约束最小化**改写为**无约束最小化**：

$$\min_{\;\mathbf{U}^l_{\text{int}},\;\mathbf{U}^{l+1}_{\text{int}},\;\mathbf{U}_b} \;\; E_{\text{param}}(P^l;\;\mathbf{U}^l_{\text{int}},\mathbf{U}_b)\;+\; E_{\text{param}}(P^{l+1};\;\mathbf{U}^{l+1}_{\text{int}},\mathbf{U}_b).$$

直观上，两块补丁**共同拉平**，但**共享一条同样的边界曲线**。

**选哪种参数化能量 $E_{\text{param}}$​：**

+   论文支持**可插拔**的能量：如 LSCM（最小二乘共形）或 ARAP（尽可能刚性）。小补丁上两者都很稳健；作者展示了两者都可用、并比较了不同组合对多重网格收敛的影响。实验里“**均匀 decimation + LSCM**”收敛最好。实现上你可以先用 LSCM（线性最小二乘、一次求解），要更小面积畸变可用 ARAP（local–global 迭代）。

**规范（gauge）与可解性：**

+   像 LSCM/ARAP 这类能量对**平移/旋转/尺度**往往不敏感（有零空间）。实际中要**固定规范**：常见做法是在共享边界 $\mathbf{U}_b$​ 上**钉住两个顶点**（例如设为 $(0,0)$ 与 $(1,0)$），或给出“去刚体”约束。由于我们最终只用**重心坐标**，它对相似变换（旋转/统一缩放/平移）不敏感，所以你钉哪两个点都可以，只要**不翻转（Jacobian>0）**即可。

第二步：边界在“边界边”情形的特殊处理

当要折叠的 $(i,j)$ 在**网格边界**上时，直接共享一整圈边界并不总是充分/适用。论文对两类边界情形使用了**“贴边/共线”**的附加约束，并在“**双端都在边界**”时枚举三种可能的联合变量分组，分别加入不同的**共线（colinearity）**约束，最后**选能量最小**的那种作为联合展平解：

+   **单端在边界**：把另一端对应的 UV 也**“吸附”到边界线上**，保证共享边界曲线闭合一致；
+   **双端在边界**：考虑三种“谁落在哪个端点/或落在这条边直线”上的分组方案，各自形成不同的线性约束（把涉及的顶点在 UV 中强制共线），计算三次能量，取最小。  
	+ 这样无论内点边还是边界边，**联合展平后的边界曲线始终一致**，从而保持双射。

第三步：求解联合展平

**以 LSCM 为例（推荐打底实现）：**

1.  为 $P^l$ 和 $P^{l+1}$ 分别装配 LSCM 的稀疏最小二乘矩阵（每个三角形贡献一组近似 Cauchy–Riemann 线性方程）。
2.  把两边的系统**堆叠**在一起，但对边界顶点引用**同一份** $\mathbf{U}_b$​（即“变量别名”）：这一步把“边界一致”作为**变量层面的合并**而非约束。
3.  加上规范（固定两点或去刚体），然后一次性做最小二乘求解 $\min\|A\mathbf{u}-b\|^2$。LSCM 的公式和装配细节见原始论文（Lévy 2002）。ARAP 版本则是 standard 的 local–global：局部拟合每个三角的最佳刚体变换，全球解一次，交替几轮收敛即可。


**为什么这样做能降低畸变？**  
如果你先把 $P^l$ 拉平，再把 $P^{l+1}$ 拉平并**硬性“对齐”边界**，第二个补丁常为迎合边界而“吃下”更多畸变；而**联合优化**等价于“在边界一致的前提下，**同时**给两边分配合理的内点 UV”，使总体畸变最小、也更均衡（论文 Fig.9 的统计展示了失真更低）。

第四步：用联合 UV 传递重心坐标（核心实现落地）

假设有一点 $p_l\in P^l$，我们知道它在**折叠前网格**某个三角形 $\triangle v_0v_1v_2\in F^l$ 上的**重心系数** $(\lambda_0,\lambda_1,\lambda_2)$（这正是论文存的“点用重心坐标表示”的含义——它天然跨相似变换稳定）。如何把它映射到折叠后 $P^{l+1}$？

**步骤 A：把 $p_l$​ 送到共享 UV**

1.  取该三角形三个顶点在**折叠前补丁的 UV**：$\mathbf{u}_0,\mathbf{u}_1,\mathbf{u}_2\in\mathbb{R}^2$（来自上面联合求解的 $\mathbf{U}^l$）。
2.  线性插值得到 $p$ 在 UV 平面的坐标：

$$\mathbf{u}_p=\lambda_0\mathbf{u}_0+\lambda_1\mathbf{u}_1+\lambda_2\mathbf{u}_2.$$

**步骤 B：从共享 UV 回到折叠后补丁**

1.  在**折叠后补丁**的三角网 ($V^{l+1},F^{l+1}$) 的 UV（$\mathbf{U}^{l+1}$）里，定位 $\mathbf{u}_p$​ 落在哪个三角形 $\triangle w_0w_1w_2​$ 的 UV 三角 $(\mathbf{u}'_0,\mathbf{u}'_1,\mathbf{u}'_2)$ 内（常规点-在-三角形测试）。
    
2.  解一个 2×2 线性系统得到其在该三角形的重心坐标 $(\mu_0,\mu_1,\mu_2)$：
    

$$\begin{aligned} \mathbf{v}_0 &= \mathbf{u}'_1-\mathbf{u}'_0,\quad \mathbf{v}_1 = \mathbf{u}'_2-\mathbf{u}'_0,\quad \mathbf{v} = \mathbf{u}_p-\mathbf{u}'_0,\\ \begin{bmatrix}\mathbf{v}_0 & \mathbf{v}_1\end{bmatrix} \begin{bmatrix}\mu_1\\ \mu_2\end{bmatrix} &= \mathbf{v},\quad \mu_0 = 1-\mu_1-\mu_2. \end{aligned}$$

3.  这组 $(\mu_0,\mu_1,\mu\_2)$ 就是 $p_{l+1}$ 在折叠后网格三角形 $\triangle w_0w_1w_2​$ 上的**重心坐标**。完成 $p_l\mapsto p_{l+1}$。反向映射同理。


第五步：把“单次折叠映射”串起来

整张网的细↔粗映射就是把**每次折叠的一环映射**按发生顺序**复合**起来。实现上，作者在预处理阶段为每次折叠**存下联合 UV**与涉及的面索引；查询时（把任意点从细层推进到更粗层或反过来）就**顺序套用**上面的“UV 中转 + 重心坐标更新”。这是论文“Successive self-parameterization”的使用方式。


<!--
### Boundary Edges

边界处理

### Decimation Strategies & Distortion Energies

讨论 edge collapse 算法的不同策略

-->

## Prolongation Operator

$P\in \mathbb R^{N_f\times N_c}$，矩阵中每个元素都是一个 3-vector，即 fine vertex 在 coarse triangle 中的重心坐标。

!!! tip "实际实现中，可以将 $P$ 形状设置为 $\mathbb R^{3N_f \times 3N_c}$ 的稀疏矩阵，以适应 $3N_f$ 长度的 gradient 和 hessian"

