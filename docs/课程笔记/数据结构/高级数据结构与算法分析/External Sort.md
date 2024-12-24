
# 外部排序

!!! quote "硬件背景"
	一般来讲，普通的排序算法都是对内存中的数组进行操作。然而有些时候要被排序的数组无法完整存储在内存中，可能会存放于硬盘之类的I/O设备中，但是访问这些设备的开销是非常大的，并且我们只能从中一块一块取出数据。我们能做的是每次将这段小块数据排成有序的，最后再将各个有序的小段 Merge 成有序的长段。

为了简化问题，本章内容认为:

- 数据存储在只能顺序访问的 Tape 上
- 最少有 3 个 Tape 可供使用

## A Simple Example

假定内部内存一次可以从 Tape 中读取 $M=3$ 个 Records(即内存只有 3 records 大小)，那么我们处理如下存储在 $T_1$ 中的 $N=13$ 个数据的步骤：

![[externalsortsimpleexample.png]]

由于磁带只能顺序读取，所以在第二次 Pass 的时候，我们将 $T_2$ 的前三个数据和 $T_3$ 的前三个数据进行 Merge，并存储到空闲的 $T_1$ 磁带中。

我们将归并后的一段有序数据称为一个 **run** ，

按照这种思想，Pass 操作的次数为 $1+\lceil \log_2 (N / M) \rceil$ 

!!! example
	- **Question:** Given 10,000,000 records of 128 bytes each, and the size of the internal memory is 4MB.  How many passes we have to do?
	- **Answer:** 1+ log(320 runs) = 1 + 9

四个优化方法：

- <1> 减少 Pass
- <2> 使用 Buffer 使其能够并行
- <3> 增长 Run
- <4> 加快 Merge

## 1. Reduce Number of Pass

使用 K-Way Merge，对应的 Pass 数为 $1+\lceil \log_k (N / M)\rceil$ 。但是问题在于，K-Way 需要至少 $2k$ 个 Tapes。正如上面那个简单例子，我们使用了 2-Way Merge，那么至少需要 4 个 Tapes。

那么如何减少所需的磁带数量呢？我们发现，当初始的 run 数量为斐波那契数时，可以通过 Split Unevenly 的方式，交替进行 Merge：

![[externalfibonaccimerge.png]]

以第一次 Pass 为例子，$T_2$ 的前 13 个 Run 和 $T_3$ 的前十三个 Run 进行 Merge 并将新的十三个 Merge 存储到 $T_1$ 上，此时 $T_2$ 剩余 8 个 Run 不进行 Merge 操作，然后进入下一轮 Pass。

通过这种方式，我们只需要使用 $k+1$ 个 Tapes 就能满足 K-Way Merge 的需求，Pass 个数满足 $F_{\#pass}^{(k)} = \#Run$ 。

!!! info "k阶斐波那契数"
	$$F_{0\sim k-2}^{(k)}=0, F_{k=1}^{(k)} =1, F_N^{(k)} = F_{N-1}^{(k)} +... + F_{N-k}^{(k)}$$

!!! warning "如果初始 Run 数量不满足斐波那契数，那么添加空数据直到满足即可"

## 2. Handle Buffers to Parallel

接下来我们使用一个更接近于真实常见应用的例子，假设内存容量为 750 records，Block 大小为 250 records，那么内存中最多能存放 3 个 Block。

对于 K-Way Merge，我们需要将内存划分为 2k 个 input buffer 和 2 个 output buffer。

每个 Buffer 原则上来讲应为一个 Block 的大小。但是由于内存大小一定，当 K 越大时，Buffer 数目变多，那么单个 Buffer 的大小变小，对应读取数据的基本单位 Block Size 也就变小，造成单次 Pass 时要处理的 I/O 开销增加。

因此，要权衡 K 值增加带来的 Pass 减少与 I/O 开销增加带来的影响。这将视实际使用的硬盘和内存参数决定。

![[handlebuffertoparallel.png]]


## 3. Generate a longer Run

上面我们分析过，对于 K-Way Merge，它的 Pass 数为 $1+\lceil \log_k (N / M)\rceil$，其中 $M$ 就是初始 Run 的大小。因此我们将 Run 的长度变大也能使 Pass 数减少。

本节继续使用第一节的 Simple Example，即内存容量为 3 records。但是与之前每个 Run 大小为内存大小 $M$ 不同，我们将内存改造成一个 min-heap，按照如下逻辑构造 Run：

- <1> 初始化: 读取内存容量大小个数据，并将其构造成 min-heap
- <2> 循环POP: pop最小值并置入 Run 中，读取下一个数据，循环操作
	- 如果下一个数据比当前 Run 最后一个数据大，则正常压入最小堆
	- 否则，将其置于堆底部，不再移动，记为死点
- <3> 清空堆: 当最小堆全是死点时，结束当前 Run 的置入，并清空堆回到步骤一

![[generatealongerrun1.png]]

最终得到一个平均长度为原来两倍的 Run 序列。如果原来的数据本来已经近似排完序了，那么这个策略的优化效果会更加明显。

## 4. Minimize the Merge Time

接续上一节内容，在我们使用最小堆生成 Run 时，每个 Run 的大小长度变得不一致，那么我们该如何安排使得 Merge Time 最小化呢？

答案是哈夫曼树！我们希望越长的 Run 越晚被 Merge，即它的 Depth 越小。

对于四个长度分别为 2,4,5,15 的 Run，我们对比以下两种合并顺序的开销差异：

![[huffmantreemerge.png]]

$$
\text{Total merge time} =O(\text{the weighted external path length})
$$

!!! note "External Path Length"
	即根到叶节点的路径长度，如上面的哈夫曼树总合并时间的计算应该为 2\*3+4\*3+5\*2+15\*1 = 43

