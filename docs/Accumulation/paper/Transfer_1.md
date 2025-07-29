
# Self-Supervised Collision Handling via Generative 3D Garment Models for Virtual Try-On

> From https://mslab.es/projects/SelfSupervisedGarmentCollisions/

### Overview

!!! abstract "Abstract"
	一种新的用于 3D 服装变形的生成模型。与现有的使用后期处理步骤来修复服装与身体穿模的方法不同，该论文给出的方法直接输出不与身体碰撞的服装。

数字化 3D 服装在日常生活中很多领域已有应用，传统的基于物理的方法已经给出了准确、适用于任何服装的解决方案，但是其庞大的计算量不足以支持实时运行。

Data-driven Methods 已经成为 Physics-based Methods 的一种流行替代方案，其核心思想是学习一个大数据集下观察到的服装模拟行为的函数。现有的基于数据方法存在一个基本限制：尽管训练时设置了穿模的 Penalty，在测试时仍然会出现服装与身体互相穿透的现象。这就需要后期额外遍历查找然后解决它。

我们的模型主要有以下三点贡献：

- <1> 通过学习将 Surface Parameter 平滑拓展到任意 3D Point 来增强现有的人体模型。
	- 这使我们能够对任意 3D Point 建模
	- 经过拓展的人体模型是我们方法的基本构建块
- <2> 针对现有的数据驱动模型，它们常假设服装变形紧跟着底层身体变形
	- 这种简化经常用在定义静止姿势下使用基于 Closest Body Vertex 的 Skinning Parameters ，然后通过 Standard Linear Blend Skinning(LBS) 来表示服装的 Garment Models。
	- 我们认为这种简化不能正确表示复杂形变，以及容易出现意外的伪影
	- 我们提出的 Garment Model 在 *unposed*, *deshaped* 的 canonical space 中表示形变，方法是去除已经被我们增强过了的 Expanded Human Body Model 捕获到的形变
- <3> 第三点也是最主要的一点，是利用服装的 Canonical 表示去学习*形变的生成式子空间*
	- 在这个 Canonical Space 中，服装关于一个常数的 Reference Body Configuration 进行编码，这不仅优化了服装形变表现，还允许我们通过自我监督来可靠学习碰撞解决。
	- 然后我们学习一个 Regressor，该回归器输出在这个子空间编码的 Mesh Deformations，然后用我们的 Garment Model 将其投影到最终的形变状态。

!!! info "由于形变的生成式子空间以及最后的投影步骤都没有穿模，因此最终变形的 3D 服装也不会穿模"

![[SSCHPipeline.png]]

我们的目标是学习一个函数，使其能够预测一个 3D 服装如何在给定的不同姿势、不同形态的人体模型上动态变形。

- **Section 4.1** 介绍了标准静态人体模型的拓展，它学习了平滑地 diffuse skinning surface parameters 到 3D 空间内任意点
	- skinning parameters 包括 rigging weights 和 blendshape correctives
- **Section 4.2** 中我们利用扩散了的 skinning parameters 去定义一个服装变形模型，
	- 在我们构建的 Unposed & Deshaped canonical space of garments 中，服装以休息姿势和平均形状出现，但保留与姿势和形状相关的皱纹细节
- **Section 4.3** 给出了基于优化的新策略，用来将基于物理的仿真投影到我们定义的 canonical space 中

## Implementation

### 4.1 Diffused Human Model

当今的 Body Model 模板为：

$$\tag {2} 
M(\beta, \theta) = W(T_b(\beta, \theta), J(\beta), \theta, \mathcal{W})
$$

- $W$ 为 Skinning Function，例如 linear blend skinning、dual quaternion 等
- $\mathcal{W}$ 为 Skinning Weight
- $\theta$ 为 Pose Parameter
- $T_b(\beta, \theta)$ 为 Unposed Parametric Body Mesh

目前标准 SMPL 模型定义的 $T_b(\beta, \theta)$ 如下；

$$\tag {3}
T_b(\beta, \theta) = T_b + B_s(\beta) + B_p(\theta)
$$

- $T_b\in R^{N_B \times 3}$ 为具有 $N_B$ 个顶点的 Body Mesh Template
- $B_s(\beta)\in R^{N_B \times 3}$ 表示用于改变 Body Shape 的形变
- $B_p(\theta)\in R^{N_B \times 3}$ 表示用于修正 Skinning Artifacts 的形变

现有的数据驱动服装模型利用 (2) 中定义的 Body Model，假设衣服紧跟着身体的变形。一种常见的方法是借用 Skinning Weight $\mathcal{W}$ 来模拟衣服的关节，通常需要在静止姿态下穷举搜索每一个 Garment Vertex 离得最近的 Body Vertex。

我们认为这种简单的静态分配不能正确模拟复杂的非刚性服装效果，原因有如下两点：

- <1> Garment-Body Nearest Vertex 的分配必须动态更新
	- 例如，衣服在皮肤表面滑动时
- <2> Garment-Body Vertex 不能仅由 Closest Vertex 驱动，因为这会引起 Medial-axis areas 意外的不连续性

为了解决这些缺点，我们提出将 (2) 中的 Body Model 平滑扩散到 Body 周围任意 3D Point 中。我们定义了以下函数来将 Skinning Weights、Shape Blendshape Offset、Pose Blendshape Offset 分别扩散到任意点 $p\in R^3$ :

$$\begin{align}
\tag{4}  \widetilde{\mathcal W}(p)= \frac{1}{N} \sum_{ q_n\sim \mathcal N (p,d)} \mathcal W (\phi (q_n))
\\ \tag {5} \widetilde {B_s} (p, \beta) =\frac{1}{N} \sum_{ q_n\sim \mathcal N (p,d)} B_s (\phi(q_n), \beta) \\
\tag {6} \widetilde {B_p} (p, \theta) = \frac{1}{n} \sum_{ q_n\sim \mathcal N (p,d)} B_p (\phi (q_n), \theta)
\end{align}
$$

- $d$ 为点 $p$ 到 surface body 的距离
- $\phi(p)$ 用于计算离 $p$ 最近的 surface point

对于每一个点 $p$ ，我们取其周围 $N$ 个邻点的相应值的平均，从而减轻了潜在不连续性的影响。

### 4.2 Garment Model

我们的下一个目标是定义一个能够表示现实衣服自然形变的 Garment Model。

我们利用 Section 4.1 中计算的 Diffused Skinning Functions 来制定 Unposed & deshaped Garment Model：

$$\begin{align}
M_G(X, \beta, \theta) = W(T_G (X, \beta, \theta), J(\beta), \theta, \widetilde{\mathcal W} (X)) \tag 7 \\
T_G(X,\beta, \theta) = X+ \widetilde{B_s} (X,\beta) + \widetilde{B_p} (X,\theta) \tag 8
\end{align}
$$

- $T_G$ 为应用了 Diffused Blendshapes Correctives 的 deformed garment
- $X$ 为 Garment Deformation in Canonical Space
