
# Local Search

## Overview

局部搜索算法是另外一种解决 Hard 问题的套路，它不覆盖整个搜索空间而对局部进行搜索，取得局部最优解，因此，Local Search 也是 **Approximately** 的。

- **Local Optimum:** 在可行集 FS 的某邻域 neighborhood 中最优的解
- **Search:** 从某个可行解 $S\in FS$ 开始，进行微小修改 $S\sim S'$ ，使得其仍为可行解且更优 ($cost(S') \lt cost(S)$)；当无法修改时 search 结束
	- $N(S)$：neighborhood of $S$ - the set $\{S':S\sim S'\}$ 

!!! note "邻域定义越小，搜索速度越快，但越容易卡在局部最优解；邻域越大，则越接近暴力搜索，搜索速度慢"

## 经典案例：Vertex Cover Problem

定义 Local Search 的梯度下降算法基本框架：

```c
SolutionType Gradient_descent()
{   Start from a feasible solution S in FS ;
    MinCost = cost(S);
    while (1) {
        S’ = Search( N(S) ); /* find the best S’ in N(S) */
        CurrentCost = cost(S’);
        if ( CurrentCost < MinCost ) {
            MinCost = CurrentCost;
            S = S’;
        }
        else  break;
    }
    return S;
}
```

**Vertex Cover Problem(Opti.Ver)** Given an undirected graph $G(V,E)$. Find a minimum subset $S\in V$ such that for each edge $(u,v)$ in $E$, either $u$ or $v$ is in $S$.

!!! note "这个版本的顶点覆盖问题并不是给出参数K判断能否用K个顶点覆盖所有边，所以这是NPC问题"

对于 Local Search ，搜索的起点可以是任意的可行解。因此，顶点覆盖问题的起点可以选择所有的点集（同时这也是最差的答案）。

- $cost(S)= |S|$
- $S\sim S'$：$S'$ can be obtained from $S$ by adding or deleting a single node
	- **思考:** 为什么有 adding 操作？这个操作是否让我们远离了局部最优解？
- **Search**：每次删除一个顶点，并判断此时的 $S'$ 是否是一个定点覆盖子集
	- 每次局部搜索的范围是当前子集顶点的个数，因此该算法的运行是有限的

但是显而易见的，在梯度下降法随机选取节点删去的情况下，由于存在很多通向次优解的路径，损失函数的值不再变化可能是陷入了局部最优解，而不是全局最优解。

![[vertexcoverfault.png]]

!!! info "梯度下降法的 Search"
	尽管梯度下降法是选择局部最优的新解，但是对于顶点覆盖问题，无论删去哪个点减少的 $cost$ 均相同，那么局部最优解可以是所有能删去的顶点，因此也就相当于随机选择了。

Metropolis Algorithm 是对该算法的一个优化：

```c
SolutionType Metropolis()
{   Define constants k and T;
    Start from a feasible solution S in FS ;
    MinCost = cost(S);
    while (1) {
        S’ = Randomly chosen from N(S); // allow adding
        CurrentCost = cost(S’);
        if ( CurrentCost < MinCost ) {
            MinCost = CurrentCost;    S = S’;
        }
        else {
            With a probability e^{-Δcost/kT}, let S = S’;
            else  break;
        }
    }
    return S;
}
```

其中参数 $k$ 是一个常数，唯一可调节的参数是温度 $T$。对于概率 $e^{-\Delta cost / kT}$ ，$T$ 越大，概率就越接近 1。即，温度越低，该算法就越接近梯度下降算法；温度越高，该算法就越倾向于跳出局部最优解。

所以我们需要一个合理的温度设计，理想情况下，在越接近全局最优解时温度 $T$ 应该越低。为了实现这个功能，提出一个新的算法 **Simulated Annealing(模拟退火)** 。

模拟退火算法正如字面意思，是模仿现实中退火降温的温度变化，其思想在于给出一个温度下降表： $T=\{T_1, T_2,...\}$

!!! danger "不是我戛然而止了，而是课件讲了这些就过了"

## 经典案例：Hopfield Neural Networks

对于一个带权图 $G=(V,E)$，如果一个边的权重小于 0，那它的两端顶点需要处于同一个状态；如果一个边的权重大于 0，那么它的两端顶点需要处于不同状态。边权的绝对值越大，就越需要满足该要求。

!!! note "简化来看，可以使用 ±1 来表示各个状态，那么可以用 $w_e S_u S_v\lt 0$ 来判断是否满足条件"

很多时候，我们不能得到满足所有要求的图，因此我们退而求其次，期望得到一个 Stable 的结果：

- [Definition](#) In a configuration $S$, edge $e=(u,v)$ is *good* if $w_e S_u S_v\lt 0$; otherwise, it is *bad*.
- [Definition](#) In a configuration $S$, a node $u$ is *satisfied* if the weight of incident good edge $\ge$ weight of incident bad edge.
	- $\sum_{v:e=(u,v)} w_e S_u S_v\le 0$
- [Definition](#) A configuration is **stable** if all nodes are statisfied.

使用一个非常简单的状态跳转算法来实现将不稳定的配置转换到稳定的配置：

```c
ConfigType State_flipping()
{
    Start from an arbitrary configuration S;
    while ( ! IsStable(S) ) {
        u = GetUnsatisfied(S);
        s_u = - s_u;
    }
    return S;
}
```

置反一个顶点，那么它的 Good Edge 会变成 Bad Edge，相应的，Bad Edge 变成 Good Edge；由于置反该点的前提条件是它的坏边权重大于好边权重，那么置反后它的好边权重就大于坏边权重了。不过此时与该点相连的顶点的是否 Satisfied 就有待判断了。

这也说明了，每次置反一个顶点，整体上好边权重会增加，坏边权重会减少。由于一个图的边权重和有上限，所以该算法也一定存在终点，即该算法是 Finite 的，且最多执行 $\sum _e W_e$ 步。

将其转换为 Local Search 问题，我们不仅希望得到 Stable 的解，还希望解的 Good Edge 权重和最大化：

- **Problem**：最大化好边的权重和
- **Feasible Solution set FS**：Configurations
- $S\sim S'$：$S'$ can be obtained from $S$ by flipping a single state

## 经典案例：The Maximum Cut Problem

给定一个正边权无向图 $G=(V,E)$，将节点 partition 成两个 cut $(A,B)$，我们希望两个 cut 之间的所有边权和最大。

$$
w(A,B):= \sum_{u\in A, v\in B} w_{uv}
$$

该问题实际上就是 Hopfield Neural Networks 的特例，即所有 $w_e\gt 0$，此时将所有状态为 +1 的节点分为 $A$，状态为 -1 的节点分为 $B$，则 $w(A,B)$ 就是好边权重之和：

- **Problem**：最大化 $w(A,B)$
- **Feasible Solution set FS**：Any partition $(A,B)$
- $S\sim S'$：$S'$ can be obtained from $S$ by moving one node from $A$ to $B$, or $B$ to $A$

!!! warning "时间复杂度"
	虽然可以保证该算法是 Finite 的，但是时间复杂度关于 $N$ 和 $\log \sum w$ 都是线性关系，这意味着只有 $\log \sum w$ 为 $N$ 的 Polynomial 关系时，整体时间复杂度才是多项式级别。

[Approximation](#) $(A,B)$ 为局部搜索得到的 Local Optimal Partition，$(A^*, B^*)$ 为全局最优划分，那么一定有 $w(A,B)\ge \frac{1}{2} w(A^*, B^*)$，即该算法的近似比为 2。

!!! note "事实上，这个问题的近似比理论下限为 $17 / 16$ （Unless P=NP）"

如果我们为翻转设置一个阈值，即一个 Node 会被翻转当且仅当它会增加总 cut value 至少：

$$
\frac{2\varepsilon}{|V|} w(A,B)
$$

那么有以下两点性质（可能会考）：

- 最终结果 $(2+\varepsilon) w(A,B)\ge w(A^*, B^*)$，即近似比为 $2+\varepsilon$
- 算法会在 $O(\frac{n}{\varepsilon} \log N)$ 次翻转内结束