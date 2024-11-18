
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
	- 例如，对于哈密顿圆的问题，虽然我们不能在多项式时间解决它，但是给出一个答案序列，我们可以在多项式时间内验证该序列是否是哈密顿圆，因此它属于 NP 类
	- P 内任意问题也属于 NP，即 $P\subseteq NP$ 
- Class NPC 中的问题定义为，属于 $NP$ 问题，且与 $NP$ 问题中任一问题难度相同
	- 如果任一 NPC 问题能在多项式时间内解决，则所有 $NP$ 问题均能在多项式时间内解决