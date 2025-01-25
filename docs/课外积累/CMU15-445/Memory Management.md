
# 5. Memory Management

## Introduction

DBMS 负责管理内存，并从磁盘中来回移动数据。在大多数情况下，数据不能直接在磁盘中操作，数据库系统需要有效地将磁盘中的数据移动到内存中。

DBMS 面临的一个障碍是尽可能减少移动数据速度的影响，在理想情况下，数据"看起来"好像全部在内存中，Execution Engine 不需要担心数据如何移动到内存中。

考虑该问题的角度分为 Spatial 和 Temporal：

- **Spatial Control:** 考虑 Pages 在磁盘上的物理位置
	- 目标是使经常一起使用的 Pages 在磁盘上尽可能靠近
- **Temporal Control:** 考虑什么时候读取 Pages 到内存中，什么时候将 Pages 写回磁盘
	- 目标是尽可能减少读取数据造成的 Stalls

## Buffer Pool

缓冲池是从磁盘读取 Pages 的 in-memory cache，它本质是数据库内部分配的一个大型内存区域，用于存储从磁盘获取的 Pages。

缓冲池被划分组织为固定大小的 Pages 数组，每一个数组项被称为 Frame。数据库请求页面时，会先扫描一遍缓冲池，如果未能找到该 Page，则从磁盘中将其副本加载进 Frame 中。

缓冲池必须维护一些元数据才能被高效、正确地使用。

Page Table 是位于内存中的一个哈希表，用于跟踪当前处于内存中的 Page，它将 Page ID 映射到缓冲池中对应的帧位置。

!!! note "区分 Page Directory: 从 Page ID 映射到 File 内 Page 位置"

Page Table 还需要维护每个 Page 的额外元数据、Dirty Flag、pin/reference counter等。

每当线程修改 Page，其 Dirty Flag 都会被该线程重新设置。pin/reference counter 跟踪当前正在访问该 Page 的线程数，线程在访问 Page 时必须先递增计数器。

!!! info "如果计数器大于 0，则不允许 Storage Manager 从内存中移除该 Page"

数据库中的内存根据两个策略分配给缓冲池：

- **Global Policies:** 全局策略要求 DBMS 作出能够使正在执行的全部工作负载整体受益的决策，它考虑所有活动事务以找到分配内存的最佳决策。
- **Local Policies:** 局部策略使得单个查询运行得更快，并且有可能不利于全局最优实现。本地策略会将 Frame 分配给一些特定的事务，而不考虑并发行为。

大多数系统同时采用全局策略和局部策略。

!!! info "OS Page Cache"
	大多数对磁盘操作需要用 OS 的 API 进行，这将产生 OS 自己的 filesystem cache。而 DBMS 一般需要使用直接 I/O 操作来绕过操作系统的缓存，以避免 Page 的冗余副本和额外的驱逐策略。

## Buffer Pool Optimizations

有多种方法可以优化缓冲池，使其适应应用程序的工作负载。

<font style="font-weight: 1000;font-size: 20px" color="orange">Multiple Buffer Pool</font>

DBMS 可以维护多个缓冲池用于不同目的，每个缓冲池都可以采用为其内部存储数据量身定做的本地策略。这种方式可以减少 Latch 争用以及改善局部性(Locality)。

我们可以通过额外维护一个 Object ID 或者直接使用哈希表来选择某一 Page 要访问的缓冲池。

<font style="font-weight: 1000;font-size: 20px" color="orange">Pre-fetching</font>

DBMS 可以根据查询计划预取 Page 来进行优化。然后，在处理第一组 Pages 时，预先将第二组 Pages 取到缓冲池中。

!!! note "DBMS 在顺序访问多个 Pages 时经常使用此方法"

<font style="font-weight: 1000;font-size: 20px" color="orange">Scan Sharing (Synchronized Scan)</font>

允许多个查询共用一个查询游标。如果查询扫描启动时，已经有别的扫描在进行中，则可以将第二个查询游标附加到现有游标上，以节省扫描成本。

<font style="font-weight: 1000;font-size: 20px" color="orange">Buffer Pool Bypass</font>

The sequential scan operator will not store fetched pages in the buffer pool to avoid overhead.

- Memory is local to running query
- Works well if operator needs to read a large sequence of pages that are contiguous on disk
- Can also be used for temporary data (sorting, jsons, ...)

## Buffer Replacement Policies

当 DBMS 需要释放一个 Frame 来为 Page 腾出空间，它必须决定从缓冲池中驱逐哪个 Page。

- LRU
	- 维护上一次访问的时间戳，驱逐时间戳最早的 Page
- CLOCK
	- LRU 的变种，不再需要维护时间戳，改为一个 Reference Bit。
	- 访问时设置 Reference Bit 为 1。当需要驱逐 Page 时，循环访问 Buffer，如果当前游标指向的 Page 的 Ref 为 1，则将其设置为 0，游标移动；否则，驱逐它
- Dirty Pages
	- 处理带 Dirty Bit 的 Page 有两个方法，较快的方法是删除缓冲池中所有非脏页面，另一个是将脏页面写回磁盘，以确保更改持久化保存
	- 避免不必要写回的一种方法是 background writing。DBMS 定期遍历 Page Table 并将脏页面写回磁盘