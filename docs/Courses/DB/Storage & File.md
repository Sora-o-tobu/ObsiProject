
# Storage & File

## Disk

一个 Storage Media 可以按照 **Speed**, **Cost**, **Reliability** 三点进行评价分类。

![[DBStorageHierarchy.png]]

- <1> **Primary Storage:** Fastest but Volatile
	- 包括 cache、main memory
- <2> **Secondary Storage:** Moderately fast and Non-volatile
	- 包括 flash、magnetic disk
- <3> **Tertiary Storage:** Slowest and Non-volatile
	- 包括 optical disk、magnetic tape

与我们 DBMS 的设计相关的主要为 Disk 以及 Main Memory。

下面给出一个简化的 Disk 机械结构：

![[DiskMech.png]]

**Sector** 是磁盘的最小单位，通常大小为 512 Bytes。一个读写请求的发送到实际数据开始传输，所需要的 Access Time，可分为两个部分：

- <1> **Seek Time:** 读写头运动到扇区对应的 Track 所需的时间
- <2> **Rotatianal Latency Time:** Platter 将扇区旋转到读写头下所需的时间
	- Average Latency 即为 1/2 圈

寻道时间和旋转等待时间受机械运动影响，是难以优化的。实际上，硬盘存取时间的速率主要局限就在 Access Time。

!!! info "Disk-arm-scheduling algorithms 其实就是电梯调度算法"

**Block** 是磁盘和主存数据交互的最先单位，它是位于同一个 track 的连续扇区序列，大小通常为 512 Bytes - 16 KB。Block 小，则需要更多次 Transfers；Block 大，则取出一个小数据会浪费很多空间。

## Storage Access

数据库系统中，File 按照定长存储单元 **Block** 逻辑划分。Blocks 是实现 Storage Allocation 和 Data Transfer 的基本单元。

因为系统不能直接在磁盘操作数据，DBMS 要负责如何在非易失性磁盘和易失性内存中移动数据。它通过 **Buffer** 来管理磁盘和内存之间的数据移动。

![[StorageAccessOverview.png]]

这里区分几个概念：

- **Page:** A unit of data
- **Block:** A unit of disk space
- **Frame** A unit of buffer pool

!!! info "一般来讲，都可以将 *Page* 和 *Block* 等价看待。"
	上层的 Execution Engine 根据请求向缓冲池请求特定 Page，缓冲池负责将该 Page 放入内存，并提供指向内存中该 Page 的一个指针

当上层应用向 Buffer Manager 请求 Page 时，按照对应 Page 是否在 Buffer Pool 内有不同的处理：

- **如果 Page 已在 Buffer 中：** 返回该 Page 在内存中的地址
- **如果 Page 不在 Buffer 中：**
	- <1> 在 Buffer 中为新 Page 开辟空间，根据不同策略替换掉一些老 Page
		- 如果没有空位，则增大 Buffer
	- <2> 被替换的 Page 如果是 Dirty 的，则写回
	- <3> 从磁盘中读取 Page，并返回该 Page 在内存中地址

当某个 Page(Block) 正在被使用时，我们标记为 **Pinned Block**，此时它不能被替换。考虑到数据库的并发性，我们为 Pool 中的 Page 维护一个 **Pin Count**，当且仅当 *pin count = 0* 时，该 Page 才能成为*替换策略*的候选人之一。

- **LRU** 最近最少使用策略
	- LRU 在 Cache 等硬件中屡试不爽，但是在数据库场景可能存在的**重复扫描**中并不合适
	- e.g. $borrow \bowtie customer$，类似于二重循环的场景，每读取 $borrow$ 一个 tuple，就对 $customer$ 的 tuple 进行一轮扫描
- **MRU** 最近最常使用策略
	- 更实践性的做法是，根据 *Query Optimizer* 来选择 Mixed Strategy
- **Toss-immediate Strategy** 用完立即丢弃
	- 严格来讲并不属于*替换*策略，但它们都是用于解决空间不够用的问题

## File Organization

数据库 File 由一系列 **records** 组成，其可分为定长 records 和变长 records。

!!! note "相比于逻辑关系模型中的 Tuple，Record 还包含事务、版本、空间管理等元信息"

### Fixed-Length Records

由于 Records 定长，我们可以简单地存储为连续、无空隙、对齐的格式：

![[FLR1.png]]

当某个 Records $i$ 被删除时，有以下三种策略：

- <1> Move $i+1, ...,n$ to $i,...,n$
- <2> Move $n$ to $i$
- <3> 不移动 Records，使用 Free List 来维护空位

对于 Free List，我们在 Header 中维护一个指向第一个被删除的 Record 的指针，每一个被删除的空 Record 上都存储了指向下一个空位的指针。当我们需要插入新 Record 时，直接通过指针找到空位插入，然后更新指针即可。

![[FLR2.png]]

!!! info "不需要额外 Column 来存储指针，可以直接在空位上存储指针，提高了空间效率"

### Variable-Length Records

一个 Record 是变长的，可能有以下原因：

- <1> Storage of multiple record type in a file
- <2> Record Type 允许变长，例如 `varchar`
- <3> Record Type 允许 fields 重复
	- 在一些 Older Data Models 中可能出现

![[VLR1.png]]

前面的变长 Attribute 用 `(offset, length)` 的定长格式存储，后面跟着定长的 Attribute，最后存储实际的数据。二者中间还存在一个 `Null bitmap`，表示对应的 Attribute 是否非空。在此例中，`0000` 表示四个属性均是非空的。

!!! warning "这个 `65000` 是定长的数据，64 bits 的整型数据 `salary`"

对于上述变长的 Records，我们可以按照 **Slotted Page Structure** 存储：

![[VLR2.png]]

定长的 Slot 从开头增长到末尾，变长的 Records 从末尾增长到开头，当中间的 Free Space 为 0 时，视为该 Page 已满。

Header 中存储了：

- <1> Record Entries 的数量
- <2> Free Space 结束的位置
	- 用来分配内存
- <3> 各个 Record 的位置以及长度

对于变长 Records，我们也可以将其以定长的方式来存储（但是一般使用的还是 slotted-structure 的形式），此处只简单给出几个样例：

=== "Reserved Space"
	![[VLR3.png]]
=== "Pointer Method 1"
	![[VLR4.png]]
=== "Pointer Method 2"
	![[VLR5.png]]

!!! info "Organization of Records in Files"
	- **Heap File:** 任何有空位的地方都可以放
	- **Sequential File:** 根据 record 的 search key 顺序存放
	- **Hashing File:** 根据 record 的某些 attribute 的哈希值指定位置存放
	- **Clustering File Organization:** 允许来自多个不同的 Relation 的 records 存放在同一个 File 中

## Index

索引用来加速数据获取的时间。定义 Records 中用于索引的一组 Attributes 为 **Search Key**。

一个 Index File 由 Index Entries 组成，它们有如下形式：

```
+------------+---------+
| Search-key | pointer |
+------------+---------+
```

!!! note "Index File 远小于 Original File"

Index File 中索引记录如何组织，取决于索引的类型：

- **<1> Ordered Indices:** Search Key 按照某种排序顺序排列
- **<2> Hash Indices:** Seach Key 在哈希函数下均匀的分布

### Ordered Indices

在 Sequential Order File 中，与对应的数据文件本身排列顺序相同的的索引称为 **Primary Index**(also clustering index)。除了主索引外的索引都称为辅助索引 **Secondary Index**。

!!! warning "主索引的 Seach Key 通常但并非一定是 Primary Key"

Ordered Indices 可以分为稠密索引和稀疏索引两类。

**Dense Index** 要求 File 中所有 Search Key 都在 Index Entry 中出现过。

=== "Dense Index 1"
	![[DenInd1.png]]
=== "Dense Index 2"
	![[DenInd2.png]]

**Sparse Index** 则只存储 Search Key 的一部分，即给出一个索引范围，然后顺序扫描：

![[SpaInd1.png]]

!!! info "如果 Data File 太大而位于多个 Block 上，优秀实践是将每个 Block 的最小值存储为稀疏索引，这样访问 Block 的次数较少"

Sparse Index 要求 File 已经事先按照 Search Key 顺序存储；而 Dense Index 则允许用于无序 File。

因此，对于 Secondary Index，我们不仅不能使用稀疏索引，还必须保证 File 中每条 Record 都有指针指向。由于 Index Entries 不能有重复，我们在中间增加一层 **bucket**：

![[SecondIndexBucket.png]]

!!! note "即 Secondary Indices have to be dense"
	对 Secondary Index 对应的属性顺序扫描可能是及其昂贵的，最坏情况下每访问一个 record 可能都要从磁盘中 fetch 一个 block。

有时，我们需要索引的数据大到连 Index File 所占的 Block 都很多。假设我们有 1000 个 Blocks 用来存储一个稀疏索引，那么使用二分法的平均搜索时间为：

$$
\left(\lceil \log_2(1000)\rceil + 1\right)*15ms = 150ms
$$

这明显是不可接受的。解决方法是建立多层索引 **Multilevel Index**，即为这 1000 个 Blocks 再建立一个外层稀疏索引，将 Inner Index 的每个 Block 的第一个值放入：

![[MultiLevelIndex.png]]

这种思想可以推广到任意多层索引。

### B+ Tree Indices

B+ Tree Indices 是 Indexed-Sequential Files 的一个替代方案。

- **Advantage**
- **Disadvantage**

!!! abstract "A simple review of B+ Tree of order M"
	- 根节点要么是叶节点，要么有 2 到 M 个子节点
	- 所有除了根节点的非叶节点都有 ⌈M2⌉ 到 M 个子节点
	- 所有叶节点深度相同

### Hash Indices *
