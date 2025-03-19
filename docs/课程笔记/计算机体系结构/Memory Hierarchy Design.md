
# Memory Hierarchy Design Basics

!!! quote ""
	![[MemoryHierarchyQuote.png]]

## Memory Technology & Optimization

<font style="font-weight: 1000;font-size: 20px" color="orange">SRAM</font>

相比于 DRAM，SRAM 不需要刷新来维持数据，其只需要很少的功耗来维持电荷。SRAM 通常使用 6 个晶体管来保存一位数据。

现在处理器芯片上一般都会集成多级 SRAM 缓存。大型 L3 Cache 的访问时间通常是 L2 Cache 的 2~8 倍，尽管如此，L3 Cache 的访问时间通常也比 DRAM 快 5 倍。

<font style="font-weight: 1000;font-size: 20px" color="orange">DRAM</font>

DRAM 通常用于 Main Memory，其内部基本结构如下图：

![[DRAMConstr.png]]

处于封装技术和成本的原因，DRAM 选择复用地址线的方式来减少管脚数量。寻找地址分为两个阶段，分别为 **Row Access Strobe(RAS)** 和 **Column Access Strobe(CAS)**。在 RAS 阶段传送一半地址，取出对应的 ROW 载入行缓冲区；在 CAS 阶段传送另一半地址，进行数据的读写操作。

DRAM 使用一个晶体管(其实是电容器)来存储一位数据。相比于 SRAM 使用六个晶体管存储一位数据，DRAM 更易获得更大的容量。

为了防止电容器的电荷流失导致数据丢失，必须定期“刷新”所有位。存储器系统中，DRAM 必须在一段时间内进行刷新，具体操作为对每一个 ROW 进行读取再写回原位置。

!!! danger "刷新使得存储器系统偶尔不可用，设计人员尽可能将刷新时间保持在总时间 5% 以下"

<font style="font-weight: 1000;font-size: 20px" color="orange">SDRAM</font>

非常早期的 DRAM 有一个缓冲区，用来实现对一个 ROW 进行多次 Column Access，而不用每次都重启 ROW Access。但是此策略的接口是异步的，这要求每次 Column Access 时都要消耗额外时间与控制器同步。

20世纪90年代中期，DRAM 接口中增加了一个时钟信号来消去同步时间的影响，即 Synchronous DRAM(SDRAM) 。

!!! note "现代 SDRAM 支持断电模式，即手动关闭时钟。时钟只用于进行自动刷新"

21世纪早期推出了一项新的创新，即 Double Date Rate, DDR。它能够让 DRAM 在时钟周期上边沿和下边沿都能传输数据，从而使得峰值数据传输率翻倍。

最后，SDRAM 引入了 Bank 来减少功耗、缩短访问时间。处理器可以同时访问多个不同的 Bank。

!!! info "在这种技术下，地址将被译码为 Bank Number，Row Address，Column Address"

<font style="font-weight: 1000;font-size: 20px" color="orange">Graphics Data RAMs</font>

GDRAM 是一种特殊的 RAM，它以 SDRAM 为设计基础，为 GPU 的高带宽需求进行定制。GDDR 和 DDR 的具体区别有：

- <1> GDDR 的接口更宽
- <2> GDDR 数据管脚的最大时钟频率更高，并且 GDRAM 通常与 GPU 直接相连，焊接在电路板上
	- DRAM 则通常放置于可扩展的 DIMM 阵列中

<font style="font-weight: 1000;font-size: 20px" color="orange">Flash</font>

Flash Memory 是一种 Electronically Erasable Programmable Read-Only Memory，它通常只读、可擦除，并且断电不会丢失内容。

!!! tip "只读和可擦除并不矛盾，只读指读取时内容不会被修改，但是仍然可以在特定条件下擦除数据重写"

闪存与标准 DRAM 体系结构差异较大，主要有：

- <1> 闪存读取顺序进行，并且会读取整个 Page
	- 1 个 Page 可能是 512 Bytes、2KiB甚至更大
	- 因此闪存速度远小于 DRAM，但仍然比磁盘快 300~500 倍
- <2> 重写闪存前，必须擦除该处的数据
	- 擦除仍然以 Page 为单位，因此写入时要将原来的数据和我们想要写入的数据组装起来
- <3> 闪存是非易失性的，未进行读写时功耗非常低
- <4> 高密度闪存比 SDRAM 便宜

现代 PMD 和笔记本电脑多使用固态硬盘，基于 Flash 的存储常用在台式机或大型服务器上。


## Optimization of Cache

将缓存优化的十种高级方法分为以下五类：

- <1> **Reducing Hit Time：** Small, Simple L1 Cache & Way Prediction
	- 这两种技术通常还能降低功耗
- <2> **Increasing Cache Bandwidth：** Pipelined Cache & Multibanked Caches & Non-Blocking Caches(非阻塞缓存)
- <3> **Reducing Missing Penalty：** Critical Word First(关键字优先) & Merging Write Buffer(合并写缓冲)
	- 这两种技术对功耗影响很小
- <4> **Reducing Miss Rate：** Compiler Optimizations(编译器优化)
	- 在编译时进行优化显然能够降低功耗
- <5> **Parallelism：** Hardware Prefetching & Compiler Prefetching
	- 通过并行减小 Miss Penalty 或 Miss Rate
	- 由于提前取出了未使用的数据，这两种技术通常会增加功耗

## Virtual Memory & Virtual Machine

虚拟内存部分见[计组笔记](https://www.nimisora.top/%E8%AF%BE%E7%A8%8B%E7%AC%94%E8%AE%B0/%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%BB%84%E6%88%90/Chapter5/)

虚拟机最早在 20 世纪 60 年代后期提出，多年以来一直是大型计算机的重要组成部分，并基于以下原因在近年来得到广泛关注：

- <1> 隔离与安全在现代系统的重要性提高
- <2> 标准操作系统的安全性与可靠性存在问题
- <3> 多用户共享一个计算机
- <4> 处理器性能的进步，使得虚拟机的开销能够接受

广义的虚拟机包括提供标准软件接口的仿真方法，比如 Java VM。本书所讲的虚拟机主要为在 ISA 级别提供完整系统级环境的虚拟机。

VM 使用的 ISA 一般与其底层硬件匹配。在传统平台上，一个操作系统拥有所有硬件资源，但在使用虚拟机时，多个操作系统共享硬件资源。

为虚拟机提供支持的软件称为 Virtual Machine Monitor 或 Hypervisor。VMM 决定了如何将虚拟资源映射到物理资源，它比传统操作系统小得多，隔离部分大约只有 10 000 行代码。

!!! info "处理器虚拟化的开销取决于工作负载，系统调用少，则虚拟化成本小"

