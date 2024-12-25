
# NP-Completeness

!!! quote "NP-Complete Problem"
	对于 NP 完全问题，目前还没发现多项式时间算法，但也没人能证明任意 NP 完全问题不存在多项式级别的算法。一些我们熟知的问题稍微改动，可能就变成了一个 NP 完全问题：
	
	- **Longest Simple Path** 我们可以使用迪杰斯特拉算法在一个有向图中找到单源最短路径，但是却不能使用多项式时间解决有向图的最长简单路径。
		- 事实上，判断一个图是否含有至少有给定数量边的简单路径也是一个 NP 完全问题
	- **Hamiltonian Cycle** 我们可以通过遍历每一条边来简单判断一个图是否存在 Euler Cycle，但是判断是否存在 Hamiltonian Cycle 却是 NP 的。


## NP-Completeness

我们将问题分为三类，`P`(Polinomial time),`NP`(Nondeterministc Polinomial time),`NPC`(NP Complete)，并给出如下简单但不规范的定义：

- Class P 中的问题能在多项式时间内解决，即它们的时间复杂度应为 $O(n^k)$
- Class NP 中的问题可以在多项式时间内验证
	- 例如，对于哈密顿回路的问题，虽然我们不能在多项式时间解决它，但是给出一个答案序列，我们可以在多项式时间内验证该序列是否是哈密顿回路，因此它属于 NP 类
	- P 内任意问题也属于 NP，即 $P\subseteq NP$ 
- Class NPC 中的问题定义为，属于 $NP$ 问题，且与 $NP$ 问题中任一问题难度相同，即任意 $NP$ 问题都可以多项式归约于该问题。
	- 如果任一 NPC 问题能在多项式时间内解决，则所有 $NP$ 问题均能在多项式时间内解决

!!! info "多项式归约 Polynomially Reduce"
	将一个问题通过有限步多项式复杂度转换为另一个问题。若某 NPC 问题可以多项式归约至另一个问题，则该问题属于 NP-Hard 问题。

## Turing Machine

图灵机由无限长纸带(Infinite Memory)和读写头(Scanner)组成，纸带被划分为一个个存储数据的小格子，而读写头则根据当前状态和指向格子的符号决定下一步移动到哪里。

- **Deterministic Turing Machine** 在每一点执行一个命令，然后根据该命令读写头会移动到唯一的下一状态。
- **Nondeterministic Turing Machine** 则可以从一个有限列表中自由选择下一步指令，并且会从中选择能够解决问题的那一步。
	- 假设每一步都有 $k$ 步可以选择，对于多项式时间 $O(n)$ ，那么总时间复杂度为 $O(k^n)$ ，为超多项式级别。

在图灵机语境下，$NP$ 问题的定义为非确定图灵机能够在多项式时间内解决的问题。

## HCP & TSP

- **Hamiltonian cycle problem** Given a graph $G=(V, E)$, is there a simple cycle that visits all vertices?
- **Traveling salesman problem** Given a complete graph $G=(V, E)$, with edge costs, and an integer $K$, is there a simple cycle that visits all vertices and has total cost $\le K$ ?

在已知哈密顿回路问题属于 $NPC$ 情况下，利用多项式归约证明旅行商问题也是 $NPC$。

如果给出旅行商问题的一组答案，我们只需要验证其是否满足遍历每一个顶点以及总消耗是否小于 $K$ 两个要求，因此 TSP 显然是一个 $NP$ 问题。接下来我们尝试证明可以将 HCP 问题多项式归约至 TSP 问题，亦等价于证明 $HCP\le _P TSP$ ：

![[hcpandtsp.png]]

假设我们已有一图 $G$ 包含哈密顿回路（图中黑色，且不一定是完全图），将 $G$ 填充至完全图 $G'$ ，并设置原来的边权值为 $1$ ，增加的边权值为 $2$ ，则对于该有权完全图旅行商问题的最优解为图中的黑色哈密顿回路，其消耗总和为 $|V|$ 。

而实际的 TSP 问题可能比该简单权值设置的更为困难，且补全完全图的时间复杂度最多为 $O(N^2)$ ，因此我们可以说在困难度上 $HCP \le _P TSP$ ，证明完毕。

## Clique Problem & Vertex Cover Problem

- **Clique Problem** 给定无向图 $G=(V,E)$ 以及整数 $K$ ，判断 $G$ 是否存在包含至少 $K$ 个节点的完全子图
- **Vertex Cover Problem** 给定无向图 $G=(V,E)$ 以及整数 $K$ ，判断 $G$ 是否有一顶点子集 $V'\subseteq V$ 满足 $|V'|\le K$ 以及每条边都有至少一个端点位于 $V'$ 中

在已知 Clique Problem 是 $NPC$ 的情况下，证明 Vertex Cover Problem 也是 $NPC$

![[cliqueandvcp.png]]

仍然先验证该问题是否属于 $NP$ ，仅需要看 $|V'|\le K$ 以及遍历每条边检查端点是否在 $|V'|$ 内即可，时间复杂度为 $O(N^3)$ 。

假设已有满足 Clique 问题的无向图 $G$ ，其包含一个节点数为 $\ge K$ 的最大子图。对 $G$ 关于完全图取补集 $\bar{G}$ ，则 $\bar{G}$ 中不在上述最大子图中的顶点可构成 $V'$ 满足每条边都至少有一个端点位于 $V'$ 中，且 $|V'|\le |V|-K$ 。

而取补集的操作时间复杂度为 $O(N^2)$ ，属于多项式级别；且上述定义的 Vertex Cover Problem 属于 Vertex Cover Problem 的子问题，那么我们可以说在困难度上 $Clique \le _P Vertex\ Cover$ ，证明完毕。


!!! quote "从 Carton 前辈那里偷来的图"
	![[cartonsenpai.png]]

