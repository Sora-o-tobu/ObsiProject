
# Test-Oriented Review

!!! quote
	- Full Tree: 所有节点要么有两个孩子，要么没有孩子
	- Complete Tree: 除最后一层外都被完全填充，并且最后一层向左对齐

## AVL Trees & Splay Tree

- Finding the maximum key from a splay tree will result in a tree with its root having no right subtree.
	- **True**
	- 一直在 Zig-zig
- Delete a node v from an AVL tree T​1​​, we can obtain another AVL tree T​2​​. Then insert v into T​2​​, we can obtain another AVL tree T​3​​. Which statement is true?
	- **[T]** If v is a leaf node in T​1​​, then T​1​​ and T​3​​ might be different.
	- **[F]** If v is not a leaf node in T​1​​, then T​1​​ and T​3​​ must be different.
	- **[F]** If v is not a leaf node in T​1​​, then T​1​​ and T​3​​ must be the same.
- There exists an AVL tree of depth (the depth of the root is 0) 6 and 31 nodes.
	- **False** 碰到这题直接拿最少节点数公式算，$n_6 =33 > 31$
	- 不过也要注意定义，有些题会定义 root 的深度为 1

## Red-Black Tree

- In a red-black tree, the number of internal nodes in the subtree rooted at x is **no more than** 2​bh(x)​​−1 where bh(x) is the black-height of x.
	- **False**
	- **Lemma 1:** A red-black tree with n internal nodes has height **at most** $2\log (n+1)$
	- **Lemma 2:** The subtree rooted at any node x contains **at least** $2^{bh(x)}-1$ internal nodes
- In a red-black tree, the number of rotations in the DELETE operation is O(1).
	- **True**
	- 详见红黑树章节某表格，除了AVL数的删除所需旋转数为 $O(\log N)$，其它均为 $O(1)$
- In a Red-Black tree, the path from the root to the nearest leaf is no more than half as long as the path from the root to the farthest leaf.
	- **False** 没有这个说法

## B+ Tree

- A B+ tree of order 3 with 21 numbers has at least __ nodes of degree 2.
	- 答案大概是 0 个。一种典型错误思考是认为最少情况是有 $21/3=7$ 个叶节点，这样第二层分为 $3,2,2$ ，共两个度为2的节点。但是实际上叶节点可以大于 7，因此我们大手调控一下使其有 9 个叶节点，这样一个度为2的节点都不存在了
- For a B+ tree with order M and N keys, the time complexity of find operations is $O(\log_M N)$
	- **False** 不考虑 M 的话应该是 $O(\log N)$ ，考虑的话可能 $O(M\log_MN)$ ？搞不懂

## Leftist Heap & Skew Heap

- The right path of a skew heap can be arbitrarily long.
	- **True**
	- 斜堆并没有左偏堆这么严格的限制
- Which one of the following statements is TRUE?
	- **[T]** The relationship of skew heaps to leftist heaps is analogous to the relation between splay trees and AVL trees
	- **[F]** For leftist heaps and skew heaps, the worst-case running time of a single insertion are both O(N)
		- 左倾堆插入最坏时间复杂度仍为 $O(\log N)$，而斜堆确实为 $O(N)$
	- **[F]** With the same operations, the resulting skew heap is always more balanced than the leftist heap
		- 倒反天罡
		- 反过来说貌似也是错误的，总之不能这么绝对
- A perfectly balanced tree forms if keys 1 to $2^k-1$ are inserted in order into an initally empty leftist heap.
	- **True** 记答案吧
	- 改成 **Skew Heap** 也是对的
- For a skew heap with N nodes, the worst-case running time of all operations (insert/delete min/merge) is O(N).
	- **True** 记答案吧
- A leftist heap with the null path length of the root being $r$ must have at least $2^{r+1}-1$ nodes
	- **True** NPL = r 对应着右路径上有 $r+1$ 个节点，所以正确。

## Binomial Queue

- To implement a binomial queue, the subtrees of a binomial tree are linked in increasing sizes
	- **False** 不需要
- Inserting a node into a binomial heap with 9 nodes costs less time than inserting a number into a binomial heap with 15 nodes.
	- **True** 9 = 1001; 15 = 1111. 看谁的 0 在前面
- Making $N$ insertions into an initially empty binomial queue take $O(N)$ time in worst case.
	- **True** 记答案吧，另外这也说明平均插入时间是常数级的

## Amortized Analysis

- 有两个栈组成一个特殊队列，当执行 `enqueue` 时，将元素 $x$ 压入 `SA` ；当执行 `dequeue` 时，若 `SB` 栈顶有元素，则 `pop` 它，若没有，则不断 `pop SA` 并 `push` 进 `SB` 中。假设 `pop, push` 都有最坏复杂度 $O(1)$ ，求均摊分析势能函数。
	- 消耗最大的操作显然就是 `pop` 时 `SB` 为空，此时将 `SA` 所有元素都 `pop,push` 了一遍。因此势能函数为 $2|SA|$。
- In amortized analysis, a good potential function should always assume its maximum at the start of the sequence.
	- **False** 应该是 Minimum，记住答案！！！

## Inverted File Index

- While accessing a term by hashing in an inverted file index, range searches are expensive
	- **True** 记答案吧

## Backtracing

- In backtracking, if different solution spaces have different sizes, start testing from the partial solution with the smallest space size would have a better chance to reduce the time cost
	- **True**
- Given the following game tree, node _d_ will be pruned with _α_−_β_ pruning algorithm if and only if \_\_\_\_\_.
- ![[backtracingex2.png]]
	- $65\le b \le 70$

## Divide & Conquer

- We can perform BuildHeap for leftist heaps by considering each element as a one-node leftist heap, placing all these heaps on a queue, and performing the following step: Until only one heap is on the queue, dequeue two heaps, merge them, and enqueue the result. Which one of the following statements is FALSE?
	- ![[divideandconquerleftistheap.png]]
	- 合并堆的时间复杂度为 $O(\log N)$，得到递推表达式 $T(N) = 2T\left(\frac{N}{2}\right)+O(\log N)$。根据主定理可以得出总时间复杂度为 $O(N)$
	- 也可记住结论，所有堆的建堆操作都不会超过 $O(N)$

## Dynamic Programming

- If a problem can be solved by dynamic programming, it must be solved in polynomial time.
	- **False**
	- 背包问题可以用 DP 求解，但由于输入的数据不是多项式的，复杂度也不是多项式
- To solve a problem by dynamic programming instead of recursions, the key approach is to store the results of computations for the subproblems so that we only have to compute each different subproblem once. Those solutions can be stored in an array or a hash table.
	- **True**
- ![[impossibledpquestion.png]]
	- 对于 $f_{i,j,k}=f_{i,j+1,k}+\min (...)$ ，$j$ 的递归要从大到小计算 `for j in n to 0:`

## Greedy Algorithm

- Let S be the set of activities in Activity Selection Problem. Then the earliest finish activity am​ must be included in all the maximum-size subset of mutually compatible activities of S.
	- **False** 


## NP-Completion

- Given that problem A is NP-complete. If problem B is in NP and can be polynomially reduced to problem A, then problem B is NP-complete.
	- **False**
- If $L_1 \le _p L_2$ and $L_2\in NP$ , then $L_1 \in NP$
	- **True** $L_1$ 可以多项式归约至 $L_2$ ，也能够说明 $L_1$ 能够在非确定图灵机上以多项式时间解决
- All the languages can be decided by a non-deterministic machine.
	- **False** 还有不可判定问题，比如 Halting Problem
- If a problem can be solved by dynamic programming, it must be solved in polynomial time.
	- **True** 背包问题

## Approximation

- Suppose ALG is an $\alpha$-approximation algorithm for an optimization problem $\prod$ whose approximation ratio is tight. Then for every $\varepsilon \gt 0$ there is no $(\alpha-\varepsilon)$-approximation algorithm for $\prod$ unless P = NP. (T/F)
	- **False**
	- 对于一种算法而言，近似比为 $\alpha$ ，那么 ∀$\beta \gt \alpha$ ，都可以说 $\beta$ 是其近似比。如果 $\alpha$ 是 tight 的，则 $\alpha$ 是一个下确界。
	- 但这都只是对这一种算法的分析，一个 tight 的近似比只能说明你对这种算法的分析到位了，而不能说明这个问题没有更好的算法。这里完全是两码事。
- As we know there is a 2-approximation algorithm for the Vertex Cover problem. Then we must be able to obtain a 2-approximation algorithm for the Clique problem, since the Clique problem can be polynomially reduced to the Vertex Cover problem.
	- **False** 关于 C 的衡量标准不一样
	- ![[costhengliangbiaozhun.png]]
- 憋憋，这也太难了，考试的时候也不会有时间想
	- 题目的意思是，如果把每个点最大权值的边加入一个集合，那么这个集合的权值和最大生成树权值之比是多少。
	- ![[shuichugeshabitimu.png]]
- 遍历最小生成树，回溯的时候可能会把一条边走两次(也可以理解成两点之间直线最短)，所以该算法近似比为2。但是如果用层序遍历，就完全不走准确解的最小生成树的边了。
	- ![[duolaAmeng.png]]
- In the bin packing problem, we are asked to pack a list of items L to the minimum number of bins of capacity 1. For the instance L, let FF(L) denote the number of bins used by the algorithm First Fit. The instance L′is derived from L by deleting one item from L. Then FF(L′) is at most of FF(L).
	- **False** 说是可以举出反例，但我想不到
	- 改成 NF 算法则是正确的



## Local Search

- 用 Local Search 求解 K-center 的近似比可以是 2 吗？？？并不能，可以举出反例。
	- ![[localsearchkcenter.png]]
- Local search algorithm can be used to solve lots of classic problems, such as SAT and N-Queen problems. Define the configuration of SAT to be X = vector of assignments of N boolean variables, and that of N-Queen to be Y = positions of the N queens in each column. The sizes of the search spaces of SAT and N-Queen are $O(2^N)$ and $O(N^N)$, respectively.
	- **True** Search Space 就是所有情况，其中 SAT 是布尔可满足性问题
- 神秘题目。从势能函数上考虑，只有最小生成树问题的 cost 函数只有一个极小值的，所以不会陷入局部最优解
	- ![[shenmitimutimian.png]]
	- ![[shenmitimujiexi.png]]


## External Sort

- **(T)** If only one tape drive is available to perform the external sorting, then the tape access time for any algorithm will be $\Omega (N^2)$.
	- 不是很理解
- **(D)** Suppose we have the internal memory that ca### n handle 12 numbers at a time, and the following two runs on the tapes:
	- **Run 1**: 1, 3, 5, 7, 8, 9, 10, 12
	- **Run 2**: 2, 4, 6, 15, 20, 25, 30, 32
	- Use 2-way merge with 4 input buffers and 2 output buffers for parallel operations. Which of the following three operations are NOT done in parallel?
	- A. 1 and 2 are written onto the third tape; 3 and 4 are merged into an output buffer; 6 and 15 are read into an input buffer
	- B. 3 and 4 are written onto the third tape; 5 and 6 are merged into an output buffer; 8 and 9 are read into an input buffer
	- C. 5 and 6 are written onto the third tape; 7 and 8 are merged into an output buffer; 20 and 25 are read into an input buffer
	- D. 7 and 8 are written onto the third tape; 9 and 15 are merged into an output buffer; 10 and 12 are read into an input buffer
		- 这里应该是 9 和 10 在合并并输出到 buffer 上。10 和 12 正在读入 buffer 过程中，那么处理器就要等待读取这个过程完成才能进行下一步
	- ![[externalsortbuffer1.png]]