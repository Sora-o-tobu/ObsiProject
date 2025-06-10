
# Query Processing

!!! quote "Parsing & Translation => Optimization => Evaluation"
	![[QueryProcessingOverview.png]]


## Query Cost

Cost 通常通过时间来衡量，受到很多因素影响：

- Disk Access
- CPU
	- 本课程简单考虑
- Network Communication
	- 在分布式中受到影响

一般来说， Disk Access 是 Cost 的主要部分，并且相对容易估计：

$$
b*t_T + S*t_S
$$

- $t_T$ ：time to transfer one block
	- 数据传输时间
	- e.g. 0.1 ms
- $t_S$ ：time for one seek
	- 读写头寻址时间
	- e.g. 4 ms

我们将 $t_T, t_S$ 都看作常量，实际的估计只与 $b$ 和 $S$ 有关。

!!! warning "本 Cost Model 中也没有考虑将最终结果写回磁盘需要的时间"

由于磁盘的 IO 次数依赖于操作系统分配给数据库的 Buffer 大小，而这一大小又是运行时动态分配的，我们难以确定一个执行时的确切值。因此，我们估计时通常以 Worst Case 和 Best Case 两个方面估计。

### SELECT

<font style="font-weight: 1000;font-size: 20px" color="orange">File Scan</font>

File Scan 的算法在不使用 Index 的条件下进行搜索。

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 1】 Linear Search</font>
	
	- 扫描每个 Block，检查 Records 是否满足 Selection Condition
	- **Cost Estimate:** cost = $b_r$ transfers + 1 seek
	- 如果查询条件是 key attribute 的特定值，则平均查找一半 Blocks 就能停止，此时：
		- cost = $b_r / 2$ transfers + 1 seek

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 2】 Binary Search</font>
	
	- 仅查找 File 排序依据的属性时可用，要求 Blocks 连续
	- **Cost Estimate:** cost = $\lceil \log_2 b_r\rceil$ transfers + $\lceil \log_2 b_r \rceil$ seeks
		- 找到第一个满足条件的 tuple 的 cost，如果该属性不是 Key Attribute，则要乘以包含满足查询条件的 Records 的 Block 的个数

<font style="font-weight: 1000;font-size: 20px" color="orange">Index Scan & Equality</font>

Index Scan 的算法的 Selection Condition 必须是 index 的 search-key。

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 3】 Primary Key & Equality On Key</font>
	
	- 只需要找到一个 record，因此与索引 B+ Tree 的高度 $h_i$ 有关
	- **Cost Estimate:** cost = $(h_i +1) *(t_T +t_S)$

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 4】 Primary Key & Equality On NonKey</font>
	
	- 与 A3 的区别在于可能有很多 records 符合条件，但是它们仍在连续的 Blocks 中
	- **Cost Estimate:** cost = $h_i *(t_T +t_S) +t_S +t_T*b$
		- $b$ 为包含符合条件 Records 的 Block 个数，约等于 $\lceil sc(A,r)/f_r\rceil$
		- 如果 B+ 树索引叶节点存储了整个 Tuple，则可以节省最后一次 Seek 时间

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 5】 Secondary Key & Equality On NonKey</font>
	
	- 如果 Search-Key 是 Candidate Key，则为单个 Record
		- **Cost Estimate:** cost = $(h_i +1)*(t_T+ t_S)$
	- 如果 Search-Key 非 Candidate Key，则为分布在不连续 Blocks 上的 $n$ 个 Records
		- **Cost Estimate:** cost = $(h_i +n)*(t_T+ t_S)$
		- 性能有可能弱于 Linear Search !!!

<font style="font-weight: 1000;font-size: 20px" color="orange">Comparisons</font>

$\sigma_{A\le V} (r)$, $\sigma_{A\ge V}(r)$ 等比较 Selection，可以采用：

- <1> Linear Search
- <2> Binary Search
- <3> 使用下面的 Indices 方法

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 6】 Primary Key & Comparisons</font>
	
	- 对于 $\sigma_{A\ge V} (r)$，使用索引找到第一个满足条件的 Records，随后从该位置开始顺序扫描。此时同 A4
		- **Cost Estimate:** cost = $h_i *(t_T +t_S) +t_S +t_T*b$
	- 对于 $\sigma_{A\le V} (r)$，从第一个 Records 开始顺序扫描，不使用索引
		- 考试似乎不考虑这种情况

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 7】 Secondary Key & Comparisons</font>
	
	- 对于 $\sigma_{A\ge V} (r)$，使用索引找到第一个满足条件的 Index Entry，然后顺序扫描 Index，取出对应指针，此时同 A5
		- **Cost Estimate:** cost = $(h_i +n)*(t_T+ t_S)$
	- 对于 $\sigma_{A\le V} (r)$，从第一个叶节点开始顺序扫描取出指针
	- 这种情况下 Linear Search 有可能更优

<font style="font-weight: 1000;font-size: 20px" color="orange">Conjunction & Disjunction</font>

!!! info "Conjunction：$\sigma_{\theta _1\land \theta_2 \land...\land \theta_n}(r)$"

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 8】 Conjunction & One Index</font>
	
	- 试运行条件 $\theta_i$，从中选取 Cost 最小的先执行；下一步再对该结果执行别的 $\theta$

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 9】 Conjunction & Composite Index</font>
	
	- 如果存在的话，使用合适的 Composite Index(Multi-Key)

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 10】 Conjunction & Intersection</font>
	
	- 对于各个 $\theta$，获取到对应 Index 集，然后取交集，最后根据指针从 File 中 Fetch Records
	- 如果一些条件不存在合适的 Index，则放进 Memory 中进行测试
	- 要求 Index 中存放了指向对应 Records 的指针


!!! info "Disjunction：$\sigma_{\theta _1\lor \theta_2 \lor...\lor \theta_n}(r)$"

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Algorithm 11】 Disjunction & Union</font>
	
	- 对于各个 $\theta$，获取到对应 Index 集，然后取并集，最后根据指针从 File 中 Fetch Records
	- 要求**所有**条件都存在相应可用的 Index，否则采用 Linear Search

### Sorting *

为什么要 Sorting？

- <1> 输出要求
- <2> 使 `JOIN` 操作执行更快

如果待排序数据能够全放进 Memory，则可以使用快速排序等算法；如果不能，则使用外部归并排序。

对于 $M$ 大小的内存、$b_r$ 个待排序 Block：

- Total Number of **runs**: $\lceil b_r / M \rceil$
- Total Number of merge **passes** required: $\lceil \log_{M-1} (b_r  / M) \rceil$
- Total Number of **Transfer**: $b_r (2 \lceil \log_{M-1} (b_r  / M) \rceil +1)$
- Total Number of **Seeks**: $2\lceil b_r / M \rceil + b_r ( 2 \lceil \log_{M-1} (b_r  / M) \rceil -1)$

### JOIN


!!! quote "$r\bowtie s$"
	- Nested-Loop Join
	- Block Nested-Loop Join
		- 以 Block 为单位的嵌套链接
	- Indexed Nested-Loop Join
	- Merge-Join
	- Hash-Join


<font style="font-weight: 1000;font-size: 20px" color="orange">Nested-Loop Join</font>

计算 $r \bowtie_\theta s$ 的步骤为：

```c
for each tuple t_r in r do begin
	for each tuple t_s in s do begin
		test pair (t_r, t_s) if they satisfy the join condition
		if they do, add t_r * t_s to result
	end
end
```

外层每选择一个 Record，内层遍历整个 File。遍历整个内层 File 只用一次 Seek。

- 在最坏情况，即 Memory 中 Buffer 只能刚好放下各个 Relation 的一个 Block
	- **Block Transfers:** $n_r *b_s + b_r$
	- **Seeks:** $n_r + b_r$
- 在最好情况，即所有数据都能放入 Memory 中
	- **Block Transfers:** $b_s +b_r$
	- **Seeks:** $2$

<font style="font-weight: 1000;font-size: 20px" color="orange">Blocked Nested-Loop Join</font>

```c
for each block B_r of r do begin
	for each block B_s of s do begin
		for each tuple t_r in B_r do begin
			for each tuple t_s in B_s do begin
				test pair (t_r, t_s) if they satisfy the join condition
				if they do, add t_r * t_s to result
			end
		end
	end
end
```

外层先取一个 Block，内层也取一个 Block，然后再在外层取的 Block 中选取一个 Record，遍历内层取的 Block 中的 Record。这样外层在更换 Record 时不需要重新 Seek。这里要求内存起码能放得下 3 个 Block。

- Worst Case
	- **Block Transfers:** $b_r *b_s +b_r$
	- **Seeks:** $2b_r$
- Best Case
	- **Block Transfers:** $b_r +b_s$
	- **Seeks:** $2$
- Common Case（Memory 中可以放得下 $M$ 个 Blocks）
	- **Block Transfers:** $\lceil b_r / (M-2)\rceil *b_s + b_r$
	- **Seeks:** $2\lceil b_r / (M-2) \rceil$

!!! note "对于 LRU 策略的 Buffer，扫描内层循环时可以选择前后交替进行，以使 Blocks 留在 Buffer 中"
	即第 N 轮扫描内层从前往后，第 N+1 轮扫描内层从后往前。

<font style="font-weight: 1000;font-size: 20px" color="orange">Indexed Nested-Loop Join</font>

在 `JOIN` 操作是一个等于条件（包括自然连接）并且 Inner Relation 的 Join Attribute 存在可用的 Index 的条件下，可以使用 Index Lookup 代替 File Scan。

!!! warning "如果不存在，也可以当场排序构造一个 Index"

该算法流程复杂多变，因此只能粗略给出一个估计：

$$\
b_r( t_T +t_S) + n_r *c
$$

其中 $c$ 指对 $s$ 单次查询的平均开销。

!!! info "如果 $r$ 和 $s$ 都有对应 Index，则选取 Tuple 数量更少的作为外层"

<font style="font-weight: 1000;font-size: 20px" color="orange">Merge Join</font>

对两个表分别按照 Join Attribute 进行排序，然后归并连接两个表。理想情况下，每个 Blocks 只需要读进 Memory 一次，那么其消耗估计为：

- **Block Transfers:** $b_r+b_s$ +(排序)
- **Seeks:** $\lceil b_r / b_b\rceil+\lceil b_s / b_b\rceil$ +(排序)

其中，$b_b$ 指 Buffer 中能够缓存的用来归并的 Block 数，即每轮顺序读取 $b_b$ 个块进入内存。通常情况下设置 $b_b= \frac{M}{2}$，但是也允许两个表的 Buffer 大小不同，但二者之和仍为 $M$。

!!! info "与归并排序算法的主要区别在于，需要匹配相同的 Join Attribute，即对重复值的处理"

<font style="font-weight: 1000;font-size: 20px" color="orange">Hash Join</font>

对两个表的 Join Attribute 同时使用哈希函数分片，能够 Equi-Join 的数据一定位于同一个 Partition 中，但是同一个 Partition 中的数据不一定相等。

!!! note "核心思想在于大关系转换成小关系"

![[HashJoinOV.png]]

做完哈希后，依次重复进行**构建与探测(Build & Probe)**：

1. 把 $s_i$ 分区的所有数据读入 Memory，用一个新的哈希函数根据 Join Attribute 为其构造 Hash Index
2. 依次读入相应的 $r_i$ 分区的所有元组 $t_r$，根据 Hash Index 找到其对应位置，将匹配的结果输出

因此，$s$ 称为 *Build Input*，$r$ 称为 *Probe Input*。

!!! warning "根据以上步骤，可知每个 Partition $s_i$ 的大小都要小于 $M$，对于 $r_i$ 则无这个要求"
	Partition 的个数 $n$ 通常设置为 $n=\lceil \frac{b_s}{M}\rceil* f$，$f$ 称为修正因子(Fudge Factor)，一般为 $1.2$
	
	如果内存放不下 $s_i$，则可以对超大分区再做多次哈希分区，直到每个子分区足够小，这种做法称为 Recursive Partition。（不做要求）

![[HashJoinCost.png]]

!!! danger "What is $n_h$ and $b_b$?"
	$n_h$ 是写 Partial Filled Blocks 造成的额外 Cost，写题时一般不考虑，即 $n_h =0$；$b_b$ 指磁盘 I/O 一次 Seeks 能顺序读写的连续块个数。

??? example
	![[HashJoinEx1.png]]

## Query Optimization

Optimizer 做什么？

1. 应用等价变换
2. 生成多种等价执行计划
3. 使用静态策略评估各个计划的 Cost
4. 选择最有效率的执行计划

两个方面：

- <1> 逻辑代数层面
	- 找到等价且更 efficient 的逻辑代数
- <2> 执行层面
	- 使用什么算法执行？

因此最后生成的 Execution Plan 决定了 Operation 的排列以及其使用的具体算法，一个示例如下：

![[QueryOptimizerEx1.png]]

!!! info "给定一个 Query Expression 树，我们从底向上执行"

### Transformation

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 1】 Conjunctive => Individual</font>
	
	$$\sigma_{ \theta_1\land \theta_2}(E)=\sigma_{ \theta_1}( \sigma_{ \theta_2}(E))$$
	
	??? example
		![[TRule1Ex.png]]

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 2】 Commutative Selection</font>
	
	$$\sigma_{ \theta_1}( \sigma_{ \theta_2}(E)) = \sigma_{ \theta_2}( \sigma_{ \theta_1}(E))$$

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 3】 Only Final Projection</font>
	
	$$\Pi_{ L_1}(\Pi_ {L_2} (...(\Pi_ {L_n} (E)))) = \Pi_{ L_1}(E)$$

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 4】 Cartesian Products => Theta Join</font>
	
	$$\begin{array}l a. & \sigma_{\theta} (E_1 \times E_2) = E_1 \bowtie_\theta E_2 \\ b. & \sigma_{\theta _1} (E_1 \bowtie_{ \theta_2} E_2) = E_1 \bowtie_{ \theta_1 \land \theta_2} E_2\end{array}$$

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 5】 Commutative Theta Join</font>
	
	$$E_1 \bowtie_\theta E_2 =E_2 \bowtie_\theta E_1$$

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 6】 Assoiative Natural Join</font>
	
	$$(E_1 \bowtie E_2) \bowtie E_3 = E_1 \bowtie (E_2 \bowtie E_3)$$

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 7】 Selection Distributes Join</font>
	
	- <7.a> 如果 $\theta_1$ 只包含 Expression $E_1$ 中的属性
		- $\sigma_{\theta _1} (E_1 \bowtie_\theta E_2) = (\sigma_{\theta _1}(E_1)) \bowtie_\theta E_2$
	- <7.b> $\theta_1, \theta_2$ 分别只包含 $E_1, E_2$ 中的属性
		- $\sigma_{\theta _1\land \theta_2} (E_1 \bowtie_\theta E_2) = (\sigma_{\theta _1}(E_1)) \bowtie_\theta (\sigma_{\theta _2} (E_2))$
	
	??? example
		![[TR7Ex.png]]

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 8】 Projection Distributes Join</font>
	
	如果条件 $\theta$ 只包含 $L_1 \cup L_2$ 中的属性：
	
	$$\Pi_{ L_1\cup L_2} (E_1 \bowtie_\theta E_2) = (\Pi_{ L_1}(E_1)) \bowtie_\theta (\Pi_{ L_2} (E_2))$$

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 9】 Commutative Set Operation</font>
	
	$$\begin{array}l E_1 \cup E_2 = E_2 \cup E_1 \\ E_1 \cap E_2 = E_2 \cap E_1\end{array}$$

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 10】 Associative Set Operation</font>
	
	$$\begin{array}l (E_1 \cup E_2)\cup E_3 = E_1 \cup (E_2 \cup E_3) \\ (E_1 \cap E_2) \cap E_3 = E_1 \cap (E_2 \cap E_3)\end{array}$$

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">【Rule 11】 Selection Distributes Set Operation</font>
	
	- <1> $\sigma_\theta (E_1 \cup E_2) = \sigma_\theta (E_1) \cup \sigma_\theta (E_2)$
	- <2> $\sigma_\theta (E_1 \cap E_2) = \sigma_\theta (E_1) \cap \sigma_\theta (E_2)$
	- <3> $\sigma_\theta (E_1 - E_2) =\sigma_\theta (E_1)-\sigma _\theta (E_2)$
	- <4> $\sigma_\theta (E_1 \cap E_2) = \sigma_\theta (E_1) \cap E_2$
	- <5> $\sigma_\theta (E_1 - E_2) =\sigma_\theta (E_1)- E_2$

### Statistics for Cost Estimation

静态估计策略可能不准确，但是结果相差不会太大。一个 Lowest Estimated Execution 也可能不是 Lowest Actual Execution。

- $n_r$: 关系 $r$ 的 tuple 个数
- $b_r$: 关系 $r$ 的 Block 个数
- $l_r$: 关系 $r$ 的一个 tuple 的大小(size)
- $f_r$: 关系 $r$ 中，一个 Block 中 tuple 的个数
	- 通常 $b_r = \lceil \frac{n_r}{f_r} \rceil$
- $V(A,r)$: 关系 $r$ 的属性 $A$ 中 **Distinct Values** 的个数
	- 等价于 $\Pi_A(r)$ 的大小

我们首先估计不同操作输出结果的 Tuple 个数：

- <1> $\sigma_{A=V} (r)$
	- $n_r / V(A,r)$
	- 如果 $A$ 是 Key Attribute，则为 $1$
- <2> $\sigma_{A\le V} (r)$
	- $n_r \times \frac{V -min(A,r)}{max(A,r) -min(A,r)}$
	- 如果 $V \le min(A,r)$，则为 $0$
- <3> $\sigma_{ \theta_1 \land \theta_2\land ...\land \theta_n}(r)$
	- $n_r \times \frac{s_1 \times s_2 \times ...\times s_n}{ n_r^n}$
	- 其中 $s_i$ 指的是 $\theta_i$ 的估计输出数量
	- 该公式的前提是 $\theta_i$ 之间相互独立
- <4> $\sigma_{ \theta_1 \lor \theta_2\lor ...\lor \theta_n}(r)$
	- $n_r \left ( 1- (1- \frac{s_1}{n_r})(1- \frac{s_2}{n_r})...(1- \frac{s_n}{n_r}) \right)$
- <5> $R\bowtie S$
	- 如果 $R\cap S =\emptyset$，则等价于 $R\times S$
	- 如果 $R\cap S= \{A\}$ 为 $R$ 的 Candidate Key，则小于等于关系 $S$ 的 Tuple 个数 $n_s$
		- 即 $t_s$ 对应 0 或 1 个 $t_r$
	- 如果 $R\cap S=\{A\}$ 不为二者的 Key，则估计为 $\min \left(\frac{n_r \times n_s}{V(A,r)}, \frac{n_r \times n_s}{V(A,s)}\right)$
- <6> $\Pi_A(r)$
	- $V(A,r)$
- <7> $_Ag_F(r)$
	- $V(A, r)$
- <8> *Outer Join*
	- 估计为自然连接的结果加上 *Outer* 方的大小
	- 例如 size of (r ⟗ s) = size of (r ⋈ s) + size of r + size of s
- <9> *Set Operations*
	- size of $r \cup s$ = size of r + size of s
	- size of $r\cap s$ = min (size of r + size of s)
	- size of $r-s$ = size of r
	- 上述三个都是按照上限估计