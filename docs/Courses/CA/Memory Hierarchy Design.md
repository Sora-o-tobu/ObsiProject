
# Memory Hierarchy Design Basics

!!! quote "第二章的重点是 Cache，并且在考试中基本以选择题形式出现"

!!! quote ""
	![[MemoryHierarchyQuote.png]]

!!! danger "与计组重合的部分自然略过了"

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

## Cache 相关概念补充

### Write Buffer

即便用了 Cache，程序运行过程中不可避免的会访问内存。此时就会发生 *Write Stall*，程序必须等待 CPU 写回内存结束。

为了减少 Write Stall，我们引入 Write Buffer 作为缓冲区。它可以临时保存需要写回主存的 Cache 数据，等到 CPU 空闲时再进行写回操作。

由于 Buffer 大小有限，因此也不能完全避免 Write Stall 发生。

对于多级缓存，Write Buffer 一般位于 L1 Cache 和 L2 Cache 之间。

### 统一缓存和分离缓存

=== "Unified Cache"
	- 指令和数据共用一个 cache
	- 更少的硬件，更低的命中率
	- 指令和数据访问竞争缓存端口和带宽
	
	![[unifiedcache.png]]
=== "Split Cache"
	- 指令和数据分开存储
	- 一般是 L1 Cache 分开，L2 Cache 统一
	- 指令流和数据流独立，互不干扰
	
	![[splitcache.png]]


## Optimization of Cache

将缓存优化的十种高级方法分为以下五类：

- <1> **Reducing Hit Time（2 + 2）**
	- Small, Simple L1 Cache \\ Way Prediction
		- 这两种技术通常还能降低功耗
	- Avoiding Address Translation \\ Trace Cache
- <2> **Increasing Cache Bandwidth（3）**
	- Pipelined Cache \\ Multibanked Caches \\ Non-Blocking Caches(非阻塞缓存)
- <3> **Reducing Missing Penalty（3 + 2）**
	- Critical Word First(关键字优先) \\ Merging Write Buffer(合并写缓冲) \\ Victim Caches
		- 这两种技术对功耗影响很小
	- Multi-Level Cache \\ Read miss prior to writes
- <4> **Reducing Miss Rate（1 + 3）**
	- Compiler Optimizations(编译器优化)
		- 在编译时进行优化显然能够降低功耗
	- Larger Block Size \\ Larger Cache Size \\ Higher Associativity
- <5> **Parallelism（2）**
	- Hardware Prefetching \\ Compiler Prefetching
		- 由于提前取出了未使用的数据，这两种技术通常会增加功耗
	- 通过并行减小 Miss Penalty 或 Miss Rate

!!! danger "建议直接抄 A4 上，很有可能出选择题，具体的内容最好简单了解一下"

### Reducing Hit Time

<font style="font-weight: 1000;font-size: 20px" color="red">Small and Simple Caches</font>

小而简单的缓存。

- 硬件小，访问速度自然快。
- 直接映射的 Hit Time 比组相联更快
- 减少芯片和 Cache 的物理距离也是更好的选择

<font style="font-weight: 1000;font-size: 20px" color="red">Way Prediction</font>

组相联时，在高速缓存中保留额外 bit 用来进行 Way-Prediction，预测下一次可能被访问的路。

- 如果预测错误，反而会增加 Hit Time
- 加大了高速缓存访问流水化的难度
- 关于预测正确率：
	- (> 90%) for two-way, (> 80%) for four-way
	- I-Cache > D-Cache

<font style="font-weight: 1000;font-size: 20px" color="red">Avoiding Address Translation</font>

- 虚拟内存使用 TLB
- Cache 的三种地址转换方式
	- **PIPT:** Physically Indexed, Physically Tagged
		- 即我们之前所讲的 Cache，缺点是不管 Cache 是否命中，都要先经过 TLB 或 PTE 转换地址
	- **VIVT:** Virtually Indexed, Virtually Tagged，可以称为 Virtual Cache
		- 歧义 Ambiguity：两个不同的进程使用了相同虚拟地址，在 Virtual Cache 中对应了同一块（实际应该为不同物理地址）
		- 重名 Aliasing：一个物理地址对应了多个进程不同的虚拟地址，导致 Cache 的命中率较低以及可能存在的 Dirty 问题
		- 以上两个问题维护成本极高，因此基本没有硬件还在用 VIVT Cache 了。
	- **VIPT:** Virtually Indexed, Physically Tagged
		- 使用虚拟地址的一部分作为 Cache 的 Index；使用物理地址的一部分作为 Cache 的 Tag
		- 此时 VIPT 既有 PIPT 的正确性，又有 VIVT 的速度

!!! warning "VIPT 什么条件下不会重名还存疑，没搞明白"

> Reference: [关于Cache的歧义/别名问题和VIVT/VIPT/PIPT架构](https://zhuanlan.zhihu.com/p/577138649)

![[VIPT.png]]

<font style="font-weight: 1000;font-size: 20px" color="red">Trace Cache</font>

追踪高速缓存是一种特殊的 I-Cache，它不存储物理上连续的指令块，而是存储动态执行的指令序列，称为 Trace。

- 根据程序执行情况动态记录实际指令序列
- 获取指令时，处理器首先在 Trace Cache 中搜索，如果命中即可一次性获取一个无需担心分支预测等停顿的较长指令序列
- 由于局部性原理，大部分分支预测都是正确的

### Increase Cache Bandwidth

L1 Cache 的带宽限制了程序，因此以下三个策略一般都应用于 L1 Cache。

<font style="font-weight: 1000;font-size: 20px" color="red">Pipelined Caches</font>

- 把缓存访问的过程流水线化，提高吞吐量
- 但是会增加分支预测错误的 Penalty 以及提高了 Latency
- 同样在 I-Cache 上更容易实现

<font style="font-weight: 1000;font-size: 20px" color="red">Multi-Banked Caches</font>

- 把缓存划分为多个独立的 bank，每个 bank 都可被独立访问，从而增加带宽
- 当访问序列恰好按照 bank 数量展开时，即在这些 bank 中平均分布，带宽提升最高
	- 因此设计者要考虑地址怎么分配给各个分区，一种简单策略是 *Sequential Interleaving**

![[SeqInterleaving.png]]

<font style="font-weight: 1000;font-size: 20px" color="red">Nonblocking Caches</font>

- 如果 CPU 支持乱序执行，可以实现 Hit Under Miss，即 Cache Miss 后可以继续执行，从而发挥出乱序执行的优势
	- 乱序执行的 CPU 不会因为一个部件失效而停顿，例如在等待 D-Cache Miss 的时候还能继续从 I-Cache 中获取指令
- 一个 Cache Line 可以同时处理多个请求

!!! note "通常，能够隐藏 L1 失效但是 L2 命中带来的 Penalty，但再往下可能仍然无法完全覆盖延迟"

```asm
1: Reg1:=LoadMem(A);
2: Reg2:=LoadMem(B);
3: Reg3:=Reg1 + Reg2;
```

执行 1 时，发生了 Cache Miss，CPU 需要从主存中取数据；但读主存时间很长，这段时间 CPU 继续执行 2。

如果 2 也发生了 Cache Miss，那么，多余 Nonblocking Cache，它会同时去主存中读 B，最终 A 和 B 一起进入 Cache。

### Reducing Missing Penalty

<font style="font-weight: 1000;font-size: 20px" color="red">Multi-Level Caches</font>

- 一级缓存可以减少 hit time，二级缓存可以减少 miss penalty
- 一级缓存的 miss penalty 就是二级缓存的 AMAT
- 区分两个概念
	- **Local Miss Rate:** 本级缓存的 Miss Rate，即本级的 Miss 量除以对本级的总访问量
	- **Global Miss Rate:** 本级的 Miss 量除以 CPU 的总访问量
	- Global Miss Rate < Local Miss Rate

<font style="font-weight: 1000;font-size: 20px" color="red">Giving Priority to Read Misses over Writes</font>

- 如果有 Write Buffer，可以暂缓处理写缺失，优先处理读缺失
- 但是处理读缺失时要检查写缓冲区，确保数据的一致性

<font style="font-weight: 1000;font-size: 20px" color="red">Critical Word First & Early Restart</font>

- 不需要等到一个 Block 都从 Cache 中取出来之后再执行，可以优先取出我们需要的 Word
- 取出关键 Word 后，CPU 即可重启继续执行了

<font style="font-weight: 1000;font-size: 20px" color="red">Merging Write Buffer</font>

- 发生 Write Miss 时，如果要写的地址恰好在 Write Buffer 内，则直接在 Write Buffer 内写，这一步骤叫做写合并 Write Merging
- 一定程度缓解了 Write Buffer 装满时造成的 Stall

<font style="font-weight: 1000;font-size: 20px" color="red">Victim Caches</font>

- 相当于多加一个 Level 的小容量全相联缓存
- Cache 中 Block 被替换时，会把该块放入 Victim Cache 中
- 如果下次要访问的时候发现在 Victim Cache 中，就可以减少 Miss Penalty

### Reducing Miss Rate

回顾一下我们的 Miss Rate 分类，在计算机体系中我们会涉及如下四种：

- **Compulsory Misses:** 冷启动 Miss，第一次访问数据时发生的缺失
- **Capacity Misses:** 容量 Miss，缓存容量不足导致的缺失
- **Conflict Misses:** 冲突 Miss，多个数据映射到同一个 Cache Block 导致的缺失
- **Coherence Misses:** 一致性 Miss，多个 Cache 的一致性问题导致的缺失

<font style="font-weight: 1000;font-size: 20px" color="red">Block Size & Cache Size & Associativity</font>

- **Larger Block Size**
	- 利用空间局部性，减少了 Compulsory Misses
	- 但是会增加 Miss Penalty
	- (在 Cache 总大小不变的情况下)还会增加 Conflict Misses 甚至 Capacity Misses
- **Larger Cache Size**
	- 减少 Capacity Misses
	- 代价是更高的 Hit Time、更高的 Cost
- **Higher Associativity**
	- 减少 Conflict Misses
	- 经验法则：大小为 N 的直接映射 Cache 的 Miss Rate 约等于大小为 N/2 的二路组相联 Cache 的 Miss Rate
	- 不同的组相联度可能会影响 Clock Cycle Time，从而影响整体性能

<font style="font-weight: 1000;font-size: 20px" color="red">Compiler Optimization</font>

不做硬件优化，只通过编译器优化来降低 Miss Rate

- 指令级别
	- 指令重排、冲突分析
- 数据级别
	- Merging Arrays 数组合并，以提升空间局部性
	- Loop Interchange 循环交换，交换内外层循环，改进空间局部性
	- Loop Fusion 循环融合，合并两个具有相同循环，且部分变量重叠的独立循环
	- Blocking 数据块化，同时利用了空间局部性和时间局部性

### Parallelism

<font style="font-weight: 1000;font-size: 20px" color="red">Hardware Prefetching</font>

- 提前把数据和指令放入 Cache 中
- 能够减少 **Compulsory Misses**
- 由于可能把有用的 Block 的挤出 Cache，也许会增加其它 Miss

<font style="font-weight: 1000;font-size: 20px" color="red">Compiler-controlled Prefetch</font>

- 由编译器控制的 Prefetch，真正获取数据之前，插入请求数据的预取指令
- 需要在预取的开销与其获得的性能之间取得平衡

## Virtual Memory & Virtual Machine

!!! quote "虚拟内存部分在体系结构中涉及较少，可见[计组笔记](https://www.nimisora.top/Courses/CO/Chapter5/)"

虚拟机最早在 20 世纪 60 年代后期提出，多年以来一直是大型计算机的重要组成部分，并基于以下原因在近年来得到广泛关注：

- <1> 隔离与安全在现代系统的重要性提高
- <2> 标准操作系统的安全性与可靠性存在问题
- <3> 多用户共享一个计算机
- <4> 处理器性能的进步，使得虚拟机的开销能够接受

广义的虚拟机包括提供标准软件接口的仿真方法，比如 Java VM。本书所讲的虚拟机主要为在 ISA 级别提供完整系统级环境的虚拟机。

VM 使用的 ISA 一般与其底层硬件匹配。在传统平台上，一个操作系统拥有所有硬件资源，但在使用虚拟机时，多个操作系统共享硬件资源。

为虚拟机提供支持的软件称为 Virtual Machine Monitor 或 Hypervisor。VMM 决定了如何将虚拟资源映射到物理资源，它比传统操作系统小得多，隔离部分大约只有 10 000 行代码。

!!! info "处理器虚拟化的开销取决于工作负载，系统调用少，则虚拟化成本小"

