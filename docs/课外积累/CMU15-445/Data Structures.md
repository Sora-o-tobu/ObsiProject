
# 6. Data Structures

!!! quote "Data Structures"
	DBMS 对系统内部多个不同部分使用不同的数据结构，包括*Internal Meta-Data*，*Core Data Storage*，*Temporary Data Structures*，*Table Indices*。
	
	在为 DBMS 实现数据结构时，需要考虑的两个主要设计决策：
	
	- **Data Organization:** 如何布局内存以及在数据结构中存储哪些信息
	- **Concurrency:** 如何让多个线程同时访问数据结构

## Hash Table

哈希表是实现将键映射到值的关联数组抽象数据结构，它提供平均 $O(1)$ 时间复杂度（最坏情况为 $O(N)$）和 $O(N)$ 空间复杂度。

!!! note "即便是 $O(1)$ 复杂度，也需要考虑常数因子优化"

哈希表的实现由两部分组成：

- **Hash Function:** 哈希函数用来计算索引，将一个较大 Key Space 映射到较小 Domain。我们需要考虑快速执行和冲突率之间的平衡。
	- DBMS 不需要考虑使用加密的哈希函数（如 SHA-256），因为哈希函数仅供数据库内部使用，不会泄露到外面。因此，我们只考虑哈希函数的速度和冲突率。
	- 当前最先进的哈希函数是 Facebook XXHash3
- **Hash Scheme:** 如何处理哈希冲突。
	- 处理冲突的方案见下文

### Static Hashing Schemes

- **Linear Probe Hashing**
- **Robin Hood Hashing**
- **Cuckoo Hashing**


### Dynamic Hashing Schemes

- **Chained Hashing**
- **Extendible Hashing**
- **Linear Hashing**

## B+ Tree Index

B+ 树是一种自平衡树结构，它保持数据排序并允许在 $O(\log N)$ 时间内进行搜索、顺序访问、删除和插入。

B+ 树索引一般用于数据库的 Table Indices 部分，几乎所有支持 Order-preserving indexes 的现代 DBMS 都使用了 B+ Tree。

!!! info "B-Tree 和 B+ Tree 的区别在于 B+ 树将所有值存储在叶节点中"

内部节点存储着指向下层节点的指针；而对于叶节点，一种方法是存储 Record ID，即 tuple location 的指针，另一种方法是存储元组的数据。

!!! note "Non-Unique Indexes"
	对于哈希表和 B+ 树，处理非唯一索引有两种方式：
	
	- **Duplicate Keys:** 重复存储该值
	- **Value Lists:** 维护唯一值的链表

#### Design Choices

<font style="font-weight: 1000;font-size: 20px" color="orange">Node Size</font>

根据存储介质，我们需要选择不同大小的节点。例如，存储在磁盘上的节点大小通常是 MB 级，以便尽可能减少搜索次数。而内存中的节点可以小到 512B 大小，只要能将 Page 放入 CPU 缓存即可。

<font style="font-weight: 1000;font-size: 20px" color="orange">Merge Threshold</font>

按照 B+ 树的原则，删除键值后节点如果不足半满，则要合并下溢节点。但是有时暂时违反规则以减少删除操作的数量可能有更高的收益。

<font style="font-weight: 1000;font-size: 20px" color="orange">Variable Length Keys</font>

一小部分大 Keys 可能会导致大量空间浪费，因此我们考虑使用可变长度键值，有几种方法可以做到这点：

- <1> Pointer: 只存储指向 Keys 的指针。
	- 缺点是效率低下，只适合嵌入式设备使用
- <2> Variable Length Node: 允许可变长度的节点
	- 这实际上是难以执行的，并且处理长度变更会带来巨大的内存管理开销
- <3> Padding: 将每个 Key 的大小设置为最大 Key 大小，填充较小的键
	- 缺点是对内存的巨大浪费
- <4> Key Map/Indirection: 将 Key 替换为指向键值对序列的索引
	- 这是最主流的方法

<font style="font-weight: 1000;font-size: 20px" color="orange">Intra-Node Search</font>

一旦我们到达一个节点，仍需要在节点内进行搜索。

- <1> Linear
	- 最简单的方法是线性扫描知道找到所需的 Key。
	- 优点是，不需要对键进行排序，能够更快地执行删除、插入；缺点是，时间复杂度为 $O(N)$，效率较低。
- <2> Binary
	- 二叉搜索效率较高，但是需要额外维护节点内大小顺序
- <3> Interpolation
	- 在某些特定数据库中，我们知道节点的大致分布结构，此时可以利用插值来找到正确位置的邻近域

### Optimization

<font style="font-weight: 1000;font-size: 20px" color="orange">Pointer Swizzing</font>

看不懂（）

<font style="font-weight: 1000;font-size: 20px" color="orange">Bulk Insert</font>

初始化 B+ 树时，不把节点一个一个插入，而是先排序好再自下而上构建。

<font style="font-weight: 1000;font-size: 20px" color="orange">Prefix Compression</font>

相似的 Keys 在 B+ 树很可能是彼此相邻的，此时它们的前缀会有所重合。此时我们只需要在节点的开头存储一次前缀，Slot 内存储不同的部分即可：

![[prefixcompressionBplus.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">Deduplication</font>

对于重复键，只写入一次，然后在后面添加其它关联值。

<font style="font-weight: 1000;font-size: 20px" color="orange">Suffix Truncation</font>

在大多数情况下，内部节点中的 Key 仅用作路标作用，我们只要搜索到底层叶节点即可。因此，可以只需要 Internal Node 中的键值的最小前缀，即后缀截断。

## Index Concurrency Control

到目前为止，我们假设讨论的 DBMS 都是单线程的。但是大多数 DBMS 需要允许多线程同时安全地访问数据结构。

并发控制协议（Concurrency Control Protocol）是 DBMS 用来确保共享对象上的并发操作正确性的方法：

- **Logical Correctness:** 线程能够读取到它想要的值
- **Physical Correctness:** 对象的内部表示是合理的
	- 例如，数据结构中没有会导致线程读取无效内存区域的指针


!!! quote "Locks vs Latches"
	![[locksvslatch.png]]
	=== "Locks"
		Locks 是一种相对高级的逻辑原语，它保护数据库的内容免受其它事务干扰。数据库系统可以向用户公开在运行查询时持有的 Locks，并且 Locks 需要能够回滚更改。
	=== "Latches"
		Latches 是从其它线程中底层地保护 DBMS 内部关键数据结构的原语。Latches 仅在执行操作时保持，且不需要回滚更改。锁存器有两种模式：
		
		- **READ:** 允许多个线程同时读取同一项。即便一个线程已经获取了 Read Latch，另一个线程也可以获得它
		- **WRITE:** 只允许一个线程访问该项。如果一个线程已经获取了 Write Latch，那么其它线程不再能够获得它。持有 Write Latch 的线程还会阻止其它线程获得 Read Latch。


### Latch Implementations

用于实现 Latch 的底层原语是通过现代 CPU 提供的 atomic compare-and-swap **(CAS)** 指令。通过 CAS，线程可以检查内存某位置的内容，以查看是否含有想要的值，如果存在，则将旧值替换为新值；否则，保持不变。

DBMS 中实现 Latch 有多种方法，每种方法在工程复杂性和运行时性能方面有所权衡。

<font style="font-weight: 1000;font-size: 20px" color="red">Blocking OS Mutex</font>

Latch 的一种实现方式是通过 OS 内置的 Mutex。Linux 系统提供了 Futex(Fast User-space Mutex)，它由用户空间的 spin latch 和操作系统级别的 Mutex 组成。

如果 DBMS 可以获取用户空间锁存器，则设置它。如果 DBMS 不能获取用户空间锁存器，它会进入内核尝试获取开销更大的 Mutex。如果上述两个操作均失败，线程将会通知 OS 并取消调度。

在 DBMS 中，OS Mutex 通常是个坏主意，因为它由 OS 进行管理，并且开销很大，不过优点在于其使用简单，不需要在 DBMS 中额外编码。

<font style="font-weight: 1000;font-size: 20px" color="red">Test-and-Set Spin Latch, TAS</font>

相较于 OS Mutex，由 DBMS 控制的 Spin Latch 是更有效的方案。

一个 Spin Latch 实质上是内存中的一个位置，它可以被线程修改值（通常是布尔值）。DBMS 可以控制在无法获取 Latch 时会发生什么，它可以选择循环重试或者允许 OS 取消它。

因此，这种方案为 DBMS 提供了更多控制权，对 Latch 的控制也更高效。缺点是不可拓展以及对缓存不友好。

<font style="font-weight: 1000;font-size: 20px" color="red">Reader-Writer Latch</font>

Mutex 和 Spin Latch 不区分读写，而 Reader-Writer Latch 允许将 Latch 保持在读/写模式。它跟踪两种模式下有多少线程持有 Latch 以及正在等在获取 Latch。

Reader-Writer Latch 使用前面两种 Latch 实现之一作为基础，并具有额外逻辑来处理 Reader-Writer Queues。不同的 DBMS 有不同的策略来处理该队列。优点在于允许并发读取，缺点是需要额外的开销管理 Reader-Writer Queue 以及元数据。

### Hash Table Latching

### B+ Tree Latching

https://15445.courses.cs.cmu.edu/fall2022/notes/09-indexconcurrency.pd