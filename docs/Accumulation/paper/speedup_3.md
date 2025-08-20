
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

