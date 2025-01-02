
# Test-Oriented Review

!!! quote
	- Full Tree: 所有节点要么有两个孩子，要么没有孩子
	- Complete Tree: 除最后一层外都被完全填充，并且最后一层向左对齐

## AVL Trees & Splay Tree

- **(T)** Finding the maximum key from a splay tree will result in a tree with its root having no right subtree.
	- 一直在 Zig-zig
- Delete a node v from an AVL tree T​1​​, we can obtain another AVL tree T​2​​. Then insert v into T​2​​, we can obtain another AVL tree T​3​​. Which statement is true?
	- **[T]** If v is a leaf node in T​1​​, then T​1​​ and T​3​​ might be different.
	- **[F]** If v is not a leaf node in T​1​​, then T​1​​ and T​3​​ must be different.
	- **[F]** If v is not a leaf node in T​1​​, then T​1​​ and T​3​​ must be the same.
- **(F)** There exists an AVL tree of depth (the depth of the root is 0) 6 and 31 nodes.
	- 碰到这题直接拿最少节点数公式算，$n_6 =33 > 31$
	- 不过也要注意定义，有些题会定义 root 的深度为 1

## Red-Black Tree

- **(F)** In a red-black tree, the number of internal nodes in the subtree rooted at x is **no more than** 2​bh(x)​​−1 where bh(x) is the black-height of x.
	- **Lemma 1:** A red-black tree with n internal nodes has height **at most** $2\log (n+1)$
		- $h\le 2\log (N+1)$
	- **Lemma 2:** The subtree rooted at any node x contains **at least** $2^{bh(x)}-1$ internal nodes
		- $sizeof(x) \ge 2^{bh(x})-1$ 全黑节点时取等
	- **Lemma 3:** $bh(x) \ge \frac{1}{2}h(x)$
- **(T)** In a red-black tree, the number of rotations in the DELETE operation is O(1).
	- 详见红黑树章节某表格，除了AVL数的删除所需旋转数为 $O(\log N)$，其它均为 $O(1)$
- **(F)** In a Red-Black tree, the path from the root to the nearest leaf is no more than half as long as the path from the root to the farthest leaf.
	- 没有这个说法
- **(F)** 判断下面这个 BST 是否是红黑树
	- ![[redblacktreepanduan.png]]
	- 红黑树中度为 1 的节点只可能是黑色的！！！

## B+ Tree

- A B+ tree of order 3 with 21 numbers has at least __ nodes of degree 2.
	- 答案大概是 0 个。一种典型错误思考是认为最少情况是有 $21/3=7$ 个叶节点，这样第二层分为 $3,2,2$ ，共两个度为2的节点。但是实际上叶节点数量可以大于 7，因此我们大手调控一下使其有 9 个叶节点，这样一个度为2的节点都不存在了
- **(F)** For a B+ tree with order M and N keys, the time complexity of find operations is $O(\log_M N)$
	- 不考虑 M 的话应该是 $O(\log N)$ ，考虑的话可能 $O(M\log_MN)$ ？搞不懂
- **(T)** The time bound of the FIND operation in a B+ tree containing $N$ numbers is $O(\log N)$, no matter what the degree of the tree is
	- 这个就是对的

## Leftist Heap & Skew Heap

- **(T)** The right path of a skew heap can be arbitrarily long.
	- 斜堆并没有左偏堆这么严格的限制
- Which one of the following statements is TRUE?
	- **[T]** The relationship of skew heaps to leftist heaps is analogous to the relation between splay trees and AVL trees
	- **[F]** For leftist heaps and skew heaps, the worst-case running time of a single insertion are both O(N)
		- 左倾堆插入最坏时间复杂度仍为 $O(\log N)$，而斜堆确实为 $O(N)$
	- **[F]** With the same operations, the resulting skew heap is always more balanced than the leftist heap
		- 倒反天罡
		- 反过来说貌似也是错误的，总之不能这么绝对
- **(T)** A perfectly balanced tree forms if keys 1 to $2^k-1$ are inserted in order into an initally empty leftist heap.
	- 改成 **Skew Heap** 也是对的
- **(T)** For a skew heap with N nodes, the worst-case running time of all operations (insert/delete min/merge) is $O(N)$.
	- 斜堆的合并复杂度 $O(\log N)$ 其实是均摊出来的，在不考虑均摊的情况下，其时间复杂度可以达到 $O(N)$
- **(T)** A leftist heap with the null path length of the root being $r$ must have at least $2^{r+1}-1$ nodes
	- NPL = r 对应着右路径上有 $r+1$ 个节点，所以正确。
- **(F)** By definition, for a light node $p$ in a skew heap, the number of descendants of $p$'s right subtree is no more than 1/2 of the number of descendants of $p$.
	- 刚好等于的情况属于重节点，因此这里要用 less than
- **(F)** After merging two leftist heaps H1 and H2, the NPL of the resulted heap will be no more than min(NPL of H1, NPL of H2)+1
	- 实际上，合并后的 NPL 应该等于 $\max[NPL(H_1), NPL(H_2)] (+1)$
- **(T)** When a leftist heap can be implemented recursively, its counterpart skew heap may not be.

## Binomial Queue

- **(F)** To implement a binomial queue, the subtrees of a binomial tree are linked in increasing sizes
	- 不需要
- **(T)** Inserting a node into a binomial heap with 9 nodes costs less time than inserting a number into a binomial heap with 15 nodes.
	- 9 = 1001; 15 = 1111. 看谁的 0 在前面
- **(T)** Making $N$ insertions into an initially empty binomial queue take $O(N)$ time in worst case.
	- 记答案吧，另外这也说明平均插入时间是常数级的
- **[D]** In a binomial queue, we denote that the total numbers of the nodes at even depth and odd depth as $N_1$ and $N_2$(the root is at depth 0), then which of the following statement is FALSE?
	- A. If $N_1 \gt N_2$, then $N_1 +N_2$ can be even
	- B. If $N_1 +N_2$ is odd, then $N_1 \gt N_2$
	- C. For all cases, $N_1 \ge N_2$
	- D. For all cases, $N_1 \le N_2 +1$

## Amortized Analysis

- 有两个栈组成一个特殊队列，当执行 `enqueue` 时，将元素 $x$ 压入 `SA` ；当执行 `dequeue` 时，若 `SB` 栈顶有元素，则 `pop` 它，若没有，则不断 `pop SA` 并 `push` 进 `SB` 中。假设 `pop, push` 都有最坏复杂度 $O(1)$ ，求均摊分析势能函数。
	- 消耗最大的操作显然就是 `pop` 时 `SB` 为空，此时将 `SA` 所有元素都 `pop,push` 了一遍。因此势能函数为 $2|SA|$。
- **(F)** In amortized analysis, a good potential function should always assume its maximum at the start of the sequence.
	- 应该是 Minimum，即 $\Phi(D_0)$ 为最小值，通常设为 $\Phi(D_0) =0$
- **(T)** Amortized analysis is a technique to provide an upper bound on the actual cost of a sequence of operations
	- 摊还复杂度应该是介于最坏时间和平均时间之间的，不过这题都说了是实际消耗的上界那就是吧
- **(T)** Recall that the worst-case time complexities of insertions and deletions in a heap of size N are both O(logN). Then, without changing the data structure, the amortized time complexity of insertions in a heap is also O(logN), and that of deletions is O(1).
	- 删除摊还复杂度怎么会是 $O(1)$...

## Inverted File Index

- **(T)** While accessing a term by hashing in an inverted file index, range searches are expensive
	- 哈希表在范围查询时需要计算每个单词的哈希值，因此消耗较大
- **(T)** Stemming increases recall while harming precision.
	- 你说是就是吧，二者不可得兼
- 

## Backtracking

- **(T)** In backtracking, if different solution spaces have different sizes, start testing from the partial solution with the smallest space size would have a better chance to reduce the time cost
	- 从搜索空间小的部分开始计算，如果找到结果了则以最小的消耗完成
- Given the following game tree, node _d_ will be pruned with _α_−_β_ pruning algorithm if and only if \_\_\_\_\_.
	- ![[backtracingex2.png]]
	- $65\le b \le 70$ (忘记从哪看来的了，我感觉 $65\le b$ 即可)
- **(T)** For the Turnpike reconstruction algorithm of $N$ points, assuming that the distance set $D$ is maintained as an AVL tree, the running time is $O(N^2\log⁡ N)$ if no backtracking happens.
	- 不是很理解这个，还是记答案吧
- **(T)** There are many solutions to the 5-Queens Problem, and there are 8 solutions that at least one queen is placed in a corner of the chessboard.
	- 还是记答案防止考到原题！
- **(F)** What makes the time complexity analysis of a backtracking algorithm very difficult is that the time taken to backtrack -- that is, to recover the previous state of a solution -- is hard to estimate.
	- 恢复状态这一步骤并非最耗时的，应该是递归搜索或者剪枝

## Divide & Conquer

- **[D]** We can perform BuildHeap for leftist heaps by considering each element as a one-node leftist heap, placing all these heaps on a queue, and performing the following step: Until only one heap is on the queue, dequeue two heaps, merge them, and enqueue the result. Which one of the following statements is FALSE?
	- ![[divideandconquerleftistheap.png]]
	- 合并堆的时间复杂度为 $O(\log N)$，得到递推表达式 $T(N) = 2T\left(\frac{N}{2}\right)+O(\log N)$。根据主定理可以得出总时间复杂度为 $O(N)$
	- 也可记住结论，所有堆的建堆操作都不会超过 $O(N)$
- **(T)** The n-th Fibonacci number can be computed by divide and conquer method of computing $x^n$, where x is the matrix  $\left [\begin{matrix}0 & 1 \\ 1 & 1 \end{matrix}\right ]$ . Then the $n^3$-th Fibonacci number $F_{n^3}$​ can be computed in $O(\log n)$ time.
	- 看不懂喵，记答案吧


计算下面函数的时间复杂度，其中 `calc` 的时间复杂度为 $O(1)$ :

```c
void fun(int l, int r) {
    if(r-l+1<=1234) return;
    int m=(l+r)/2;
    int m1=(l+m)/2, m2=(m+1+r)/2;
    fun(l, m);
    fun(m1+1, m2);

	for(int k=1;k<=r-l+1;k++)
        for(int i=1;i<=r-l+1;i++)
            for(int j=l;j<=r;j+=i)
                calc(j, i);

    fun(m+1, r);
    fun(m1+1, m2);
}
```

**解答：** 四个 `fun` 嵌套调用对应 $4T(N / 2)$ ，主要考虑中间的循环部分。外面两层循环都是 $N$ 次，最内侧循环 $N/ i$ 次，其中 $i$ 从 1 到 $N$ 。由于 $1+ \frac{1}{2}+ \frac{1}{3}+ ... + \frac{1}{N} = \ln N$ ，所以 combine 的时间复杂度为 $O(N^2 \log N)$

$$
T(N) = 4T\left(\frac{ N}{ 2} \right)+ O(N^2 \log N) = O(N^2 \log ^2 N)
$$

## Dynamic Programming

- **(F)** If a problem can be solved by dynamic programming, it must be solved in polynomial time.
	- 背包问题可以用 DP 求解，但由于输入的数据不是多项式的，复杂度也不是多项式
- **(T)** To solve a problem by dynamic programming instead of recursions, the key approach is to store the results of computations for the subproblems so that we only have to compute each different subproblem once. Those solutions can be stored in an array or a hash table.
	- 考的是 DP 算法的两个基本思想
- 考虑 DP 状态转移方程计算顺序：
	- ![[impossibledpquestion.png]]
	- 对于 $f_{i,j,k}=f_{i,j+1,k}+\min (...)$ ，$j$ 的递归要从大到小计算 `for j in n to 0:`

## Greedy Algorithm

- **(F)** Let S be the set of activities in Activity Selection Problem. Then the earliest finish activity $a_m$ must be included in all the maximum-size subset of mutually compatible activities of S.
	- 虽然算法的思想是局部选择结束最早的活动，但是如果有另一个活动稍晚一点结束，但是又没在下一个活动开始后才结束，那么选择这个活动作为替代也是最优解
- **[C]** Which of the following is NOT an elment of greedy strategy?
	- A. optimal substructure
	- B. works only if the local optimum is equal to the global optimum
		- 只有局部最优解导向全局最优解才有效
	- C. overlapping sub-problems
		- 重叠子问题是动态规划的特征
	- D. make a choice before solving the remaining sub-problem
		- 在求解之前作出选择


## NP-Completion


- **(T)** A decision problem in P is also in both NP and co-NP.
	- $co-NP$ 表示其补集语言可在 NP 内验证
- **(T)** The following problem is in co-NP: Given a positive integer k, is k a prime number?
- **(F)** Given that problem A is NP-complete. If problem B is in NP and can be polynomially reduced to problem A, then problem B is NP-complete.
	- 应该是 A 能归约到 B，才能说 B 是 NPC 问题
- **(T)** If $L_1 \le _p L_2$ and $L_2\in NP$ , then $L_1 \in NP$
	- $L_1$ 可以多项式归约至 $L_2$ ，也能够说明 $L_1$ 能够在非确定图灵机上以多项式时间解决。$L_1$ 可能是 $P$ 问题，但是 $P\in NP$
- **(F)** If a decision problem A can be reduced to B, then it means that problem A is strictly easier than B in terms of computational complexity.
	- 并非严格更简单，可能相等(比如都是 NPC 问题)
- **(F)** All the languages can be decided by a non-deterministic machine.
	- 还有不可判定问题，比如 Halting Problem
- **(F)** If a problem can be solved by dynamic programming, it must be solved in polynomial time.
	- 0-1 背包问题不算 P 类
- **(F)** As we know there is a 2-approximation algorithm for the Vertex Cover problem. Then we must be able to obtain a 2-approximation algortithm for the Clique problem, since the Clique problem can be polynomially reduced to the Vertex Cover problem.
	- 就算这两个问题可以相互归约，在计算近似率时 Cost 的取值是不一样的，即评价准则不一样
- **(F)** Without any assumptions on the distances, if P != NP, there is no ρ-approximation for TSP (Travelling Salesman Problem) for any ρ≥1.
	- 这题的关键其实再第一句话，对距离没有任何假设。对于 TSP 问题，可以分为满足三角不等式的 Metric TSP 和不满足三角不等式的 General TSP(非欧几里得空间、非对称、负边权、动态距离等)。对于 Metric TSP，的确存在最小生成树法得到近似比为2 的近似算法，但是 General TSP 的某些情况优化问题和验证问题难度相当，没有多项式时间算法能够保证近似比

## Approximation

- **(F)** There exits an online algorithm for the bin packing problem that uses at most 3/2 the optimal number of bins for an instance
	- 在线的装箱问题算法的上限基本也就 1.7 了...
	- 理论证明，对于online算法，总能构造一组输入使得装箱数**至少**为最优解的 5/3 倍
- **(F)** Suppose ALG is an $\alpha$-approximation algorithm for an optimization problem $\prod$ whose approximation ratio is tight. Then for every $\varepsilon \gt 0$ there is no $(\alpha-\varepsilon)$-approximation algorithm for $\prod$ unless P = NP. (T/F)
	- 对于一种算法而言，近似比为 $\alpha$ ，那么 ∀$\beta \gt \alpha$ ，都可以说 $\beta$ 是其近似比。如果 $\alpha$ 是 tight 的，则 $\alpha$ 是一个下确界。
	- 但这都只是对这一种算法的分析，一个 tight 的近似比只能说明你对这种算法的分析到位了，而不能说明这个问题没有更好的算法。这里完全是两码事。
- **(F)** As we know there is a 2-approximation algorithm for the Vertex Cover problem. Then we must be able to obtain a 2-approximation algorithm for the Clique problem, since the Clique problem can be polynomially reduced to the Vertex Cover problem.
	- 关于 Cost 的衡量标准不一样
	- ![[costhengliangbiaozhun.png]]
- 憋憋，这也太难了，考试的时候也不会有时间想
	- 题目的意思是，如果把每个点最大权值的边加入一个集合，那么这个集合的权值和最大生成树权值之比是多少。
	- ![[shuichugeshabitimu.png]]
- 遍历最小生成树，回溯的时候可能会把一条边走两次(也可以理解成两点之间直线最短)，所以该算法近似比为2。但是如果用层序遍历，就完全不走准确解的最小生成树的边了。
	- ![[duolaAmeng.png]]
- **(F)** In the bin packing problem, we are asked to pack a list of items L to the minimum number of bins of capacity 1. For the instance L, let FF(L) denote the number of bins used by the algorithm First Fit. The instance L′is derived from L by deleting one item from L. Then FF(L′) is at most of FF(L).
	- 说是可以举出反例，但我想不到
	- 改成 NF 算法则是正确的
- **(T)** For any undirected graph G, the weight of its maximum cut is at least half of the total edge weight.
- **(T)** Consider the following variant of the knapsack problem. You are given k identical knapsacks and n items of different sizes. The profit of each item is equal to its size. Your goal is to select items and fill the knapsacks as much as possible subject to the capacity constraints. A greedy approach always packs the largest remaining item as long as there is enough room. As bin packing, if more than one knapsacks have room for the current item, one may use any packing rule (Next Fit, First Fit, and Best Fit). No matter what rule you apply, for any k≥1, the approximation ratio of the greedy algorithm is always 2.
	- 近似比严格来讲都是小于 2 的，但是这是上限就可以了




## Local Search


- **(F)** For an optimization problem, given a neighborhood, if its local optimum is also a global optimum, one can reach an optimal solution with just one step of local improvements.
	- 神秘题目，不清楚错在哪里
- 用 Local Search 求解 K-center 的近似比可以是 2 吗？？？并不能，可以举出反例。
	- ![[localsearchkcenter.png]]
- **(T)** Local search algorithm can be used to solve lots of classic problems, such as SAT and N-Queen problems. Define the configuration of SAT to be X = vector of assignments of N boolean variables, and that of N-Queen to be Y = positions of the N queens in each column. The sizes of the search spaces of SAT and N-Queen are $O(2^N)$ and $O(N^N)$, respectively.
	- Search Space 就是所有情况，其中 SAT 指的是布尔可满足性问题
- 神秘题目。从势能函数上考虑，只有最小生成树问题的 cost 函数只有一个极小值的，所以不会陷入局部最优解
	- ![[shenmitimutimian.png]]
	- ![[shenmitimujiexi.png]]
- **(F)** A Las Vegas algorithm is a randomized algorithm that always gives the correct result, however the runtime of a Las Vegas algorithm differs depending on the input. A Monte Carlo algorithm is a randomized algorithm whose output may be incorrect with a certain (typically small) probability. The running time for the algorithm is fixed however. Then if a Monte Carlo algorithm runs in O(n^2) time, with the probability 50% of producing a correct solution, then there must be a Las Vegas algorithm that can get a solution in O(n^2) time in expectation.

## Randomized Algorithms

- **(F)** Considering the randomized quicksort, there exists some good inputs on which the expected running time of randomized quicksort is $O(n)$ where n is the input size. 
	- Randomized Quicksort随机化的思想就是保证时间复杂度不受输入序列的影响，因此这虽然保证了上限不会达到 $O(n^2)$ ，也使得下限同样不会达到 $O(n)$

## Parallel Algorithms

- Consider two disjoint sorted arrays $A[1…n]$ and $B[1…m]$, we would like to compute the k-th smallest element in the union of the two arrays, where $k≤\min (⁡m,n)$. Please choose the smallest possible running time among the following options:
	- A. $O(\log n)$
	- B. $\min (O(\log m), O(\log n))$
	- C. $O(\log m)$
	- D. $O(\log k)$
	- 答案选D，因为两个并查集都是有序的，所有最后搜索的时间复杂度应该与 $m,n$ 无关
	- https://leetcode.cn/problems/median-of-two-sorted-arrays/solutions/258842/xun-zhao-liang-ge-you-xu-shu-zu-de-zhong-wei-s-114/
	- 实际上就是两个有序数组的二分查找，第一步若有： $A[k / 2] \le B[ k / 2]$ ，则第 k 小的元素一定落在 $A$ 数组 $k/2$ 之后或者 $B$ 数组 $k/2$ 之前。继续二分，在这个缩小的区间内查找： $A[3k / 4] , B[k / 4]$ ，以此类推
- **(F)** If we translate a serial algorithm into a reasonably efficient parallel algorithm, the work load and the worst-case running time are usually reduced.
- **(F)** When we solve the summation problem via designing the parallel algorithms, we shorten the asymptotic time complexity but take more asymptotic work loads comparing with the sequential algorithms.
	- 似乎是 workload 的级别没有降低



## External Sort

- **(T)** If only one tape drive is available to perform the external sorting, then the tape access time for any algorithm will be $\Omega (N^2)$.
	- 说是寻道时间从 $O(1)$ 变成了 $O(n)$ 了，但我不是很理解
- **(D)** Suppose we have the internal memory that can handle 12 numbers at a time, and the following two runs on the tapes:
	- **Run 1**: 1, 3, 5, 7, 8, 9, 10, 12
	- **Run 2**: 2, 4, 6, 15, 20, 25, 30, 32
	- Use 2-way merge with 4 input buffers and 2 output buffers for parallel operations. Which of the following three operations are NOT done in parallel?
	- A. 1 and 2 are written onto the third tape; 3 and 4 are merged into an output buffer; 6 and 15 are read into an input buffer
	- B. 3 and 4 are written onto the third tape; 5 and 6 are merged into an output buffer; 8 and 9 are read into an input buffer
	- C. 5 and 6 are written onto the third tape; 7 and 8 are merged into an output buffer; 20 and 25 are read into an input buffer
	- D. 7 and 8 are written onto the third tape; 9 and 15 are merged into an output buffer; 10 and 12 are read into an input buffer
		- 这里应该是 9 和 10 在合并并输出到 buffer 上。10 和 12 正在读入 buffer 过程中，那么处理器就要等待读取这个过程完成才能进行下一步
	- ![[externalsortbuffer1.png]]