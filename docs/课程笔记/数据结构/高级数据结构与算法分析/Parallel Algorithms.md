
# Parallel Algorithms

!!! quote "并行算法"
	- 处理器并行 Machine Parallelism:
		- Processor Parallelism / Pipeline / 长指令集
	- 算法并行 Parallel Algorithms: 两个模型
		- Parallel Random Access Machine(PRAM)
		- Work-Depth(WD)

## Basic Model

### PRAM

为了解决处理器的 Access Conflict，可以使用以下三种策略:

- Exclusive-Read Exclusive-Write(EREW) 独占读，独占写
- Concurrent-Read Exclusive-Write(CREW) 并发读，独占写
- Concurrent-Read Concurrent-Write(CRCW) 并发读，并发写

而读写共有三种不同的 Rule：

- <1> Arbitrary Rule: 随机选择一个 processor 让它写
- <2> Priority Rule: 优先级高的 processor 让它写
- <3> Common Rule: 只允许在写相同值时并发写

以简单的求和为例，假设我们需要从内存中读取并求和数据 $A[0]+A[1]+...+ A[n]$。

假定我们只有一个处理器，那么该处理器只能单步取出数据并单步求和，时间复杂度显而易见为 $O(N)$。

对于多处理器并行，我们先取特例，假设我们的处理器数量以及数据数量均为 $n=8$：

![[summationproblempram.png]]

在一个时钟周期内，我们将临近的两个处理器内数据进行求和，那么时间复杂度理论上为树的高度，即 $O(\log N)$，对应的伪代码可以概括如下：

```c
for P_i ,  1 <= i <= n  pardo
  B(0, i) := A( i ) // B 即上图的二叉树
  for h = 1 to log n do
    if i <= n/2^h
      B(h, i) := B(h-1, 2i-1) + B(h-1, 2i)
    else stay idle
  for i = 1: output B(log n, 1);
  else i > 1: stay idle
```

!!! info "Stay Idle 也是对处理器的一个指令操作，因此该算法每个时钟周期的执行操作数均为 $O(N)$，这里的 N 为处理器数量"

由此例还可以看出，PRAM有两个缺点：

- <1> 处理器数量变化时特定算法不再适用，即可迁移性差
- <2> 所有处理器指令都由程序员逐一分配，细节太多

### WD

为此，我们提出了 Work-Depth 模型，其主要区别在于并行优化由编译器完成，减少了对冗余的处理器的调度操作：

```c
for P_i ,  1 <= i <= n  pardo
   B(0, i) := A( i )
for h = 1 to log n 
    for P_i, 1 <= i <= n/2^h  pardo
        B(h, i) := B(h-1, 2i-1) + B(h-1, 2i)
for i = 1 pardo
   output  B(log n, 1)
```


### Measuring Performance

并行算法的性能度量要从时间复杂度和工作复杂度(work load)来考虑。

- Work load - total number of operations: W(n)
- Worst-case running time: T(n)

对于 PRAM 模型，理论上需要时间复杂度 $T(n)$ 和工作复杂度 $W(n)$ 的算法：

- 如果处理器数量为 $p(n)= \frac{W(n)}{T(n)}$，那么需要花费 $T(n)$ 时间
- 如果处理器数量 $p\le \frac{W(n)}{T(n)}$，那么需要花费 $\frac{W(n)}{p}$ 时间
- 对于任意处理器数量 $p$，需要花费 $\frac{W(n)}{p}+T(n)$ 时间
	- 即 $O(\frac{W(n)}{p}+T(n))$ ，谁大时间复杂度就是谁

而对于 WD 模型，对于任意处理器数量 $P(n)$，在使用 concurrent-wrie 的前提下，时间复杂度均为 $O(\frac{W(n)}{P(n)}+T(n))$。

!!! danger "有什么区别？我也不明白😓"

## 经典案例：Prefix-Sums

给出输入: $A(1),A(2),...,A(n)$

求: $\sum_{i=1}^1 A(i), \sum_{i=2}^1 A(i),..., \sum_{i=n}^1 A(i)$

回顾我们在 Summation Problem 中构造的平衡二叉树，我们对其进行改造，希望能将最终的前缀和输出到叶子节点上。为此，我们需要定义一个新的二叉树 $C(h,i)=\sum_{i=1}^\alpha A(i)$，其中 $\alpha$ 为该节点的 Rightmost Descendant Leaf。例如，对于节点 $C(2,1)$，它对应着前缀和 $\sum_{i=4}^1 A(i)$：

![[prefixsumproblembt.png]]

那么可以分类得到原来的 $B(h,i)$ 和 $C(h,i)$ 的对应关系：

```c
if (i==1) C(h,i) = B(h,i);
else if (i % 2 == 0) C(h,i) = C(h+1,i/2); // 等于父节点的值
else C(h,i) = C(h+1,(i-1)/2) + B(h,i); // 等于父节点兄弟加上自己
```

可以看出是自顶向下进行处理，对应算法为：

```c
// Initialize
for P_i , 1 <= i <= n pardo
  B(0, i) := A(i)
// 向上算 B
for h = 1 to log n
  for i , 1 <= i <= n/2^h pardo
    B(h, i) := B(h - 1, 2i - 1) + B(h - 1, 2i)

// 向下算 C
for h = log n to 0
  for i even, 1 <= i <= n/2h pardo
    C(h, i) := C(h + 1, i/2)
  for i = 1 pardo
    C(h, 1) := B(h, 1)
  for i odd, 3 <= i <= n/2^h pardo
    C(h, i) := C(h + 1, (i - 1)/2) + B(h, i)
for Pi , 1 <= i <= n pardo
  Output C(0, i)
```

## 经典案例：Merge

将两个非递减数组 $A,B$ 进行合并。

解决思想为划分(Partition)，我们取特例：

- $A,B$ 内元素无重复值
- $A,B$ 等长
- $\log n$ 和 $n\log n$ 均为整数值，其中 $n$ 为数组长度

首先，如果我们已经知道 $B$ 数组中各个元素在 $A$ 数组中的排序位置，那么可以得到并行算法：

```c
for P_i , 1 <= i <= n  pardo
    C(i + RANK(i, B)) := A(i)
for P_i , 1 <= i <= n  pardo
    C(i + RANK(i, A)) := B(i)
```

分别对两个数组并发读写实现 Merge，即在知道排序关系的前提下，只需要 $O(n+m)\text{work, \&}O(1)\text{time}$ 即可并行计算完成。

那么问题的重点在于如何解决排序 `RANK`？可以参考以下两个算法：

![[howtorank.png]]

!!! note "串行算法减少了W，但增大了T"

而本章节希望了解的是并行的排序方法：

![[parallelranking1.png]]

## 经典案例：Maximum Funding

**Solution 1:** 大功率跑车算法

```c
for P_i , 1 <= i <= n  pardo
    B(i) := 0
for i and j, 1 <= i, j <= n  pardo // 并行这一步需要 n^2 个处理器
    if ( (A(i) < A(j)) || ((A(i) = A(j)) && (i < j)) )
            B(i) = 1
    else B(j) = 1
for Pi , 1 <= i <= n  pardo
    if B(i) == 0
       A(i) is a maximum in A
```

- 时间复杂度 $T(n)=O(1)$
- 工作复杂度 $W(n)=O(n^2)$

!!! info "大功率跑车算法是很多硬件最大值寻找自带的算法，其优点就是速度很快"

**Solution 2:** Doubly-logarithmic Paradigm

将元素划分为 $\sqrt{n}$ 个长 $\sqrt{n}$ 的段，然后对每个段再调用 Doubly-logarithmic Paradigm 算法递归计算。每一次递归完成，都能得到该段内元素的最大值，对这些最大值进行大功率跑车算法求的总体的最大值。

对 $\sqrt{n}$ 个段并行执行 Doubly-logarithmic Paradigm 算法所需时间为 $T_1= T(\sqrt{n})$，所需的处理器数量为 $W_1 =\sqrt{n} W(\sqrt{n})$，并得到 $\sqrt{n}$ 个段最大值。对段最大值应用大功率跑车算法，所需时间为 $T_2=O(1)$，所需处理器为 $W_2=O(\sqrt{n}^2)=O(n)$，那么总时间消耗为二者相加：

$$\begin{array}l
\begin{cases}T(n)= T(\sqrt{n}) \\ W(n)=\sqrt{n}W(\sqrt{n}) +O(n)\end{cases} \\
\begin{cases} T(n)=O(\log \log n)\\ W(n)=O(n\log \log n)
\end{cases}
\end{array}
$$

![[doublelogpr1.png]]

在以上 Doubly-logarithmic Paradigm 的基础上还可以进行适当优化，我们可以将元素划分为长 $h=\log \log n$ 的小段，总共分为 $n / h$ 段，对每个段进行时间复杂度 $T_1=O(h)$，工作复杂度 $W_1 =O(h)$ 的算法并行找出段最大值，那么这一步骤的总时间复杂度为 $O(h)$，总工作复杂度为 $\frac{n}{h}O(  h)$。最后，对得出的 $\frac{n}{h}$ 个段最大值应用 Doubly-logarithmic Paradigm 算法，这一步骤的时间复杂度和工作复杂度分别为：$T_2=O(\log \log \frac{n}{h}), W_2=O( \frac{n}{h}\log \log \frac{n}{h})$ ：

$$
\begin{cases}T(n)=O(h)+O\left(\log \log \frac{n}{h}\right)=O(\log \log n) \\
W(n)= \frac{n}{h}O(h) +O( \frac{n}{h}\log \log \frac{n}{h})=O(n) \end{cases}
$$

![[doublelogp2.png]]

!!! danger "怎么样时间和空间复杂度才会都为 $O(h)$ 呢？我不是很理解"

**Solution 3:** Random Sampling

目标是得到一个在极大概率保证正确性的前提下，时间复杂度为 $O(1)$，工作复杂度为 $O(n)$ 的随机算法。

从元素每 $n^{1 / 8}$ 个数中随机抽取一个数，共 $n^{ 7 / 8}$ 个数。在将这些数按照每段 $n^{ 1 / 8}$ 划分为 Block，共分为 $n^{ 6 / 8}$ 个块。那么对于每个块并行应用大功率跑车算法：

$$
\begin{cases}T=O(1) \\ W=O((n^{1 / 8})^2) = O( n^{ 1 / 4})\end{cases} \xrightarrow {\text{并行}} \begin{cases} T=O(1)   \\
W=n^{ 6 / 8} O(n^{1 /4}) =O(n)  \end{cases}
$$

这样我们得到每个块的段最大值，共有 $n^{6 / 8}$ 个值。对于这些值，再次按照每段 $n^{1/ 4}$ 来划分为 Block，共划分为 $n^{1 / 2}$ 个块，再次对每个块并行应用大功率跑车算法：

$$
\begin{cases}T=O(1) \\ W=O((n^{1 / 4})^2) = O( n^{ 1 / 2})\end{cases} \xrightarrow {\text{并行}} \begin{cases} T=O(1)   \\
W=n^{ 1 / 2} O(n^{1 /2}) =O(n)  \end{cases}
$$

得到 $n^{1 / 2}$ 个最大值。此时不需要再进行分块了，直接对者 $n^{1 / 2}$ 个数进行大功率跑车算法：

$$
\begin{cases} T=O(1)  \\
W=O(n)\end{cases}
$$

综上所述，从 $n$ 个数中采样 $n^{7 / 8}$ 个数并找出最大值的时间、工作复杂度分别为 $O(1), O(n)$。

在我们得到 $n^{7 / 8}$ 个数中最大值 $M$ 的基础上，还可以对其进行下面检验以寻找真正的最大值。其思想在于用 n 个处理器并行检验每一个元素，如果该元素大于 $M$ ，则将其置于一个新域 $B$。如果 $B$ 的规模再次达到 $n^{7 / 8}$ ，则重复对它进行上述算法，得到一个新的、更准确的最大值:

```c
while (there is an element larger than M) {
     for (each element larger than M)
         Throw it into a random place in a new B(n^7/8);
     Compute a new M;
}
```

[Theorem](#) Random Sampling 以极高的概率可以在 $T=O(1),W=O(n)$ 找到最大值；不能在这个规模运行的概率为 $O( \frac{1}{n^c})$，其中 $c$ 是一个较大常数。

!!! quote
	![[summaryparaal.png]]

