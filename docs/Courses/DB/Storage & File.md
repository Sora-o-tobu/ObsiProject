
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

**Block** 是磁盘和主存数据交互的最小单位，它是位于同一个 track 的连续扇区序列，大小通常为 512 Bytes - 16 KB。Block 小，则需要更多次 Transfers；Block 大，则取出一个小数据会浪费很多空间。

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

??? abstract "总共可以按照四个角度来分类索引"
	- 按「数据结构」分类：**B+tree索引、Hash索引、Full-text索引**。
	- 按「物理存储」分类：**聚簇索引（主键索引）、二级索引（辅助索引）**。
	- 按「字段特性」分类：**主键索引、唯一索引、普通索引、前缀索引**。
	- 按「字段个数」分类：**单列索引、联合索引**。

### Ordered Indices

在 Sequential Order File 中，与对应的数据文件本身排列顺序相同的的索引称为 **Primary Index**(also clustering index)。除了主索引外的索引都称为辅助索引 **Secondary Index**。

!!! warning "主索引的 Seach Key 通常但并非一定是 Primary Key"
	非 Sequential File 没有 Primary Index，但是可以包含 Primary Key

??? tip "物理存储角度来看主键索引和辅助索引"
	- 主键索引的 B+ Tree 的叶子节点存放整个 Record 数据
	- 辅助索引的 B+ Tree 的叶子节点存放主键值，而不是实际数据
	
	所以查询使用辅助索引时，如果查询的数据能在叶子节点中得到，就不需要回表，称为**覆盖索引**；如果查询的数据不在辅助索引中，则要根据查询得到的主键值再去检索主键索引，这个访问两次索引的步骤称为**回表**。

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

B+ Tree Indices 是 Index-Sequential Files 的一个替代方案，也是数据库存储引擎采用最多的索引类型。

!!! info "InnoDB 在 MySQL 5.5 之后成为默认的存储引擎"
	- 如果有主键，则默认使用主键作为 Primary Index
	- 如果没有主键，则选择第一个不包含 `NULL` 值的唯一列作为 Primary Index
	- 上面两个都没有，则 InnoDB 自动创建一个隐式自增 id 列作为 Primary Index

!!! abstract "*Index-Sequential Files*: Sequentially Ordered File with a Primary Index"

对于 Index-Sequential Files，当文件大小增长，会产生很多 overflow blocks，使得性能下降；这也要求我们阶段性地对整个文件进行重排。

与之相比，B+-Tree Index Files 的优点在于：

- <1> 插入、删除等操作会自动重排
- <2> 不需要重排整个 File

!!! abstract "A simple review of B+ Tree of order M"
	- 根节点要么是叶节点，要么有 2 到 $M$ 个子节点
	- 所有除了根节点的非叶节点都有 $\lceil \frac{M}{2}\rceil$ 到 $M$ 个子节点
	- 所有叶节点深度相同

![[B+TreeEx1.png]]

与我们数据结构中的 B+ Tree 略有不同的是，此处叶节点跟内节点一样，都只能有 $M-1$ 个 Key。这是因为我们需要最后一个指针位置指向下一个叶节点，

!!! info "所有 Internal Nodes 都有 $\lceil \frac{M}{2}\rceil$ 到 $M$ 个指针；指针数量称为该节点的 Fanout"
	叶节点的 Values 数量在 $\lceil \frac{M-1}{2}\rceil$ 到 $M-1$ 之间，这又与上面的表述有所不同，记得注意。

对于一个 Order $n$，有 $K$ 个 Search Key 的 File，其 B+ Tree Indices 的高度**小于等于** $\lceil \log_{\lceil \frac{n}{2}\rceil} K\rceil$。

!!! tip "$\lceil \log_n \left( \frac{m}{n-1} \right) \rceil + 1 \le h \le \lceil \log_{\lceil \frac{n}{2}\rceil} \left ( \frac{m}{\lceil (n-1) / 2 \rceil} \right)\rceil+1$"
	问你 B+ 树节点的数量怎么求？利用最后一层叶节点的数量倒推回去，最小情况是所有叶节点全满（$n-1$）；最大情况是所有叶节点半满（$\lceil (n-1) / 2 \rceil$）。
	
	或者你也可以像这么算，将每种 height 的最小、最大能包含的 Search-Key 数量都算出来，然后进行比较：
	
	=== "Height Estimate"
		![[B+TreeHeightCal.png]]
	=== "Size Estimate"
		![[B+TreeSizeCal.png]]


通常，我们希望 B+ Tree 的一个 Node 的大小刚好是一个 Block，一个 Block 一般为 4KB。一种实践是设置 $n=100$，则一个 index entry 大小为 40B。

!!! example "考虑 1 million Search Key，n = 100"
	最多搜索 $\log_{50} (1,000,000)=4$ 个 Nodes。

插入、删除的例子，请去 PPT 上看。

- 某一节点少于 min\_size 时，优先看能否与邻居合并，如果合并会导致溢出，即大于 max\_size，才选择“借”一个值过来

提高空间利用率，B+ Tree File Organization：

- 叶节点不再存放 pointer，而是直接存整个 record
	- 这里其实相当于把叶子节点对应的 Block 直接看作 Data File，内节点和根节点仍然为 Index File
	- 对于 Secondary Index，叶节点可以改为存储 Primary Index 的值，这样按照辅助索引进行查询时，再根据查询到的主键值查询主键 B+ Tree，这个过程称为*回表*，也就是至少需要查询两个 B+ Tree 才能查到数据
- 调整节点“半满”的要求，例如要求每个 node 至少有 $\lfloor 2n / 3 \rfloor$ entries

??? question "[Why B+ Tree?](https://xiaolincoding.com/mysql/index/index_interview.html#%E4%B8%BA%E4%BB%80%E4%B9%88-mysql-innodb-%E9%80%89%E6%8B%A9-b-tree-%E4%BD%9C%E4%B8%BA%E7%B4%A2%E5%BC%95%E7%9A%84%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84)"
	- **<1> B+ Tree vs B Tree**
		- B+ Tree 只在叶子节点存储数据，而 B 树的非叶子节点也要存储数据，所以在存储量相同的情况下，B+ Tree 的非叶子节点可以存放更多的索引，查询*底层*节点的磁盘I/O次数更少
		- 另外，B+ Tree 叶子节点采用的是双链表连接，适合 SQL 中常见的基于范围的顺序查找，而 B 树范围查询只能遍历树
		- B+ Tree 插入和删除操作效率更高
	- **<2> B+ Tree vs 二叉树**
		- 对于有 N 个叶子节点的 B+ Tree，其搜索复杂度为 $O(\log (d*N))$，其中 d 表示节点允许的最大子节点个数为 d 个。在实际的应用当中， d 值是大于 100 的，这样就保证了，即使数据达到千万级别时，B+Tree 的高度依然维持在 3~4 层左右，也就是说一次数据查询操作只需要做 3~4 次的磁盘 I/O 操作就能查询到目标数据。
		- 而二叉树的每个父节点的儿子节点个数只能是 2 个，意味着其搜索复杂度为 $O(\log N)$，这已经比 B+Tree 高出不少，因此二叉树检索到目标数据所经历的磁盘 I/O 次数要更多。
	- **<3> B+ Tree vs Hash**
		- Hash 在做等值查询时搜索复杂度为 $O(1)$。但是 Hash 表不适合做范围查询，它更适合做等值的查询，这也是 B+Tree 索引要比 Hash 表索引有着更广泛的适用场景的原因。

### Hash Indices *

- Static Hashing
- Dynamic Hashing

### LSM Tree

Log-Structured Merge Tree 设计的初衷是优化**写入性能**，它将随机小写操作转化为顺序大块写入，从而提升磁盘吞吐。

LSM Tree 的核心思想是不直接修改磁盘上的 Index File，而是先将写操作缓存在内存中，称为 Level 0。

![[LSMTree1.png]]

当 Level 0 的数据达到阈值时，则中序遍历 Level 0 树，将数据写入 Level 1 的新 Block 中。然后进行递归 Merge。

- Benefits of LSM approach
    - Inserts are done using only **sequential I/O** operations
    - **Leaves are full**, avoiding space wastage
    - Reduced number of I/O operations per record inserted as compared to normal B+-tree (up to some size)
- Drawback of LSM approach
    - Queries have to search multiple trees
	    - 即牺牲部分读性能
    - Entire content of each level copied multiple times
