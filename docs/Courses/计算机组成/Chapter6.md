
# Chapter 6. The Hardware/Software Interface

!!! quote "Typical Collection of I/O Device"
	![[typicalcollectionofiodevice.png]]

## Introduction

IO设备的三个特性：

- **Bahavior**
	- Input(只读), output(只写，不能被读取), or storage(can be reread and usually rewritten)
- **Partner**
	- Human, or machine
- **Data rate**
	- 与存储部件或处理器交换数据的最大速率

IO设备的性能指标：

- **Throughput** 吞吐量
	- 在某一时间内我们能从系统传出多少数据
	- 在单位时间内能做多少I/O请求
- **Response Time** 响应时间


最好的性能测量方法取决于具体应用。在很多多媒体应用中，大部分I/O请求用以处理长数据流，因此我们更注重 Throughput；在另一些应用中我们更重视 Response Time，当 I/O 请求非常大时，响应时间严重受限于带宽，但对于 PC 这种数据访问量较小的单用户机器，访问操作延迟短的 I/O 系统将获得更快的响应时间。

!!! info "实际上，I/O系统的重点为：可靠性、可扩充性、性能、成本"
	归纳来说，台式机和嵌入式系统更注重响应时间和I/O设备的多样性；而服务器更关系吞吐量和I/O设备的可扩充性。

## Disk Storage

!!! quote "再放送"
	![[memoryhierarchy1.png]]

!!! warning "相比于 SRAM，DRAM 速度更慢，并且需要定时刷新避免漏电损失数据"

磁盘主要分为两大类：

- **Floppy Disks** 软盘
	- 存储容量小
- **Hard Disks** 硬盘
	- 存储容量大
	- 数据密度大
	- 传输速率高
	- 拥有多个盘片

### Hard Disk

机械硬盘也被称为旋转磁盘，它主要依靠盘片来存储数据，盘片表面涂有磁性的记录材料，盘片正反两面都可用来存储数据。通常情况下，硬盘由一个或多个盘片组成，这些盘片被固定在一个可以旋转的主轴上，主轴带动盘片以固定速率进行高速旋转。

!!! info "Platter"
	盘片表面被划分为一圈一圈的磁道(**tracks**)，每个磁道又被划分为多个扇区(**sectors**)，通常情况下每个扇区可以存储 512 字节的数据（近年来也有增加到 4096 字节的）。扇区与扇区之间又存在间隙(**gaps**)，用来存放扇区的标识信息。
	
	![[磁盘结构.png]]

除了容量之外，磁盘的读写速度也是一个非常重要的指标，对扇区的访问时间主要分为三部分，分别是寻道时间、旋转时间以及传送时间

$$
T_{access} =T_{seek} +T_{rotation} +T_{transfer}
$$

- **Seek time** 寻道时间
	- 当目标扇区所处磁道与当前读写头所处磁道不同时，传动臂需要将读写头移动到目标扇区所在磁道，传动臂移动所需时间即寻道时间
	- 寻道时间取决于读写头当前所在位置与目标位置的距离
	- 通过对随机扇区的访问测试，通常平均寻道时间为 3~9ms 左右
- **Rotation  latency** 旋转时间
	- 目标扇区的第一个数据位旋转到读写头下所需的时间
	- 这个过程由当前读写头所在扇区位置跟目标扇区的位置以及盘片旋转速度来决定
	- 平均旋转半圈 0.5 Rotation
- **Transfer time** 传送时间
	- 一个扇区的传送时间依赖于旋转速度以及每条磁道的扇区数目，即读写头经过该扇区所需要的时间

!!! success "访问一个磁盘扇区所花费的时间主要是寻道时间和旋转时间"
	![[readdisckex1.png]]

### Flash

Flash 属于非易失性半导体存储器(Nonvolatile semiconductor storage):

- 速度远大于 Disk(100 ~ 1000 倍)
- 更小、耗能更低、更耐用
- 更贵

!!! note "非易失性"
	Flash 和磁盘存储都是非易失性的，即去掉电源后数据不会丢失。

### Dependability & Availability

- **MTTF** mean time to failure 平均无故障时间
- **MTTR** mean time to repair 平均修复时间
- **MTBF** mean time between failures
	- = MTTF + MTTR
- **Dependability** 可靠性
	- 从参照点至出现故障经过时间的量度，即通常通过 MTTF 作为量度
- **Availability** 可用性
	- $= \frac{MTTF}{MTTF+MTTR}= \frac{MTTF}{MTBF}$

如何提高 Availability ？？？可以尝试使用磁盘阵列，利用冗余来增加磁盘存储的可用性，例如 Redundant Arrays of Inexpensive Disks(**RAID**):

| RAID Level                                    | 允许几个盘损坏 | 数据盘个数 | 校验盘个数 | 注释                       |
| --------------------------------------------- | ------- | ----- | ----- | ------------------------ |
| 0 Non-redundant striped                       | 0       | 8     | 0     | 仅把数据分散到多个磁盘上             |
| 1 Mirrored                                    | 1       | 8     | 8     | 每一个盘对应一个校验盘，即镜像          |
| 2 Memory-style ECC                            | 1       | 8     | 4     | 路边也够，无人问津                |
| 3 Bit-interleaved parity                      | 1       | 8     | 1     | 位交叉奇偶校验                  |
| 4 Block-interleaved parity                    | 1       | 8     | 1     | 块交叉奇偶校验                  |
| 5 Block-interleaved parity distributed parity | 1       | 8     | 1     | 分布式块交叉奇偶校验，校验块分布于各个数据盘之中 |
| 6 P+Q redundancy                              | 2       | 8     | 2     | 二次校验块技术，需要多一倍校验盘         |

!!! info "目前最常用的是 RAID 0 和 RAID 5"

!!! example "RAID 3 位交叉奇偶校验"
	![[raid3ex1.png]]
	
	校验位等于所有数据盘对应位的奇函数。当有一个盘损坏时，通过其它几个数据盘以及校验盘的数据，可以恢复损坏的数据。
	
	缺点在于对校验盘的读写是最频繁的，因此校验盘才是最容易损坏的，这个问题在 RAID 5中有所改善。

!!! example "RAID 5 分布式块交叉奇偶校验"
	![[raid5ex1.png]]
	
	相比于 RAID 3,4，其优点还有可以同时写两个磁盘，小数据写效率更高。

**课本题目：** Which of the following are true about RAID levels 1,3,4,5 and 6?

- **(True)** RAID systems rely on redundancy to achieve high availability
- **(True)** RAID 1 (mirroring) has the higheset check disk overhead
- **(True)** For small writes, RAID 3 has the worst throughput
- **(True)** For large writes, RAID 3,4 and 5 have the same throughput

由于可以同时读写多个磁盘，即便是 RAID 0 也增加了数据大规模访问的性能。

## MIO Bus

### Bus Basics

不同的子系统需要具有连接彼此的接口，这个工作通常由总线完成。总线是一条共享的通信链路，其带宽限制了I/O的最大吞吐量，因此总线系统的设计也是计算机系统优化的关键。

Bus 包含两种 lines:

- **Control lines:** 传递请求和控制信号，并表示数据线信息的类型
- **Data lines:** 传输信息（包括 data、address、command）
	- 一些总线会用两套线路分别传输数据和地址

总线通信的基本方式分为同步和异步。如果总线是同步的，控制线上就有一个时钟以及与时钟相关的固定通信协议，这种协议是预先确定的，设计较少逻辑控制，所以总线可以运行很快，且接口上的逻辑电路也比较少。

同步总线的缺点在于总线上所有设备都必须运行在同一时钟周期频率下，且由于时钟偏差原因，同步总线的长度不能太长。

!!! info "处理器-内存总线通常是同步的，因为设备通信距离短、数量少，时钟频率高"

异步总线不需要时钟，因此它可以适应更多设备。为了协调发送者和接收者之间的数据传输，异步总线采用握手协议，只有当发送者和接收者达成一致时，才能进行下一步。

!!! example "异步握手示例"
	![[yibuwoshoushili.png]]

### Communication with Processor

<font style="font-weight: 1000;font-size: 24px">1. Polling</font>

处理器周期性检查I/O设备寄存器的状态位来决定能否执行下一个I/O操作，这个方法称为**轮询**。

轮询是I/O设备与处理器通信最简单的方式，I/O设备在完成工作后更改状态寄存器，而处理器通过访问这个寄存器从而完全控制和执行所有这些工作。

对于实时嵌入式系统，由于其I/O速度是预定的，I/O开销是可预测的，因此轮询是一个简单方便的策略。但是轮询缺点在于它浪费了大量处理器时间，因为处理器的速度要比I/O设备快的多。一个较慢I/O操作期间，处理器会多次访问状态寄存器。

<font style="font-weight: 1000;font-size: 24px">2. Interrupt</font>

**中断驱动I/O**方式几乎被所有系统使用，当一个I/O设备想要通知处理器它已经完成某种操作或发生某些错误时，就会向处理器发送中断信号，引起处理器中断。中断机制消除了处理器轮询设备的需求，从而使处理器能够集中精力运行程序。

!!! info "中断是异步的，控制单元只需要在执行下一条指令前检查是否有未决的中断即可"

当数据正在从设备或向设备中传输时，系统可以进行别的工作。当数据传输完成，设备发送一个I/O中断信号，处理器再检查状态寄存器等操作。

!!! tip "Polling 和 Interrupt 传输数据的方式都是以较小 Unit 或 Chunk 为单位传输的"

<font style="font-weight: 1000;font-size: 24px">3. DMA</font>

当I/O设备与内存需要传输数据时，数据都需要经过处理器才能到达对方。尽管我们使用中断的方式驱动数据传输，I/O设备与处理器进行数据交换消耗的处理器时间仍然是难以接受的。因此我们有一种不需要处理器，而让设备控制器直接向内存或从内存传输数据的机制，称为**直接内存访问(Direct Memory Access)**

!!! note "设备和处理器通信时仍然采用I/O中断，但仅限I/O操作完成或出现错误时"

<font style="font-weight: 1000;font-size: 24px;color:red">课件例题</font>

假定我们有一个轮询操作需要 400 时钟周期的 500-Mhz 的处理器，对于以下三个设备：

- <1> 需要轮询鼠标每秒30次以保证用户的操作不会丢失
- <2> 软盘(Floppy Disk)与处理器传输数据以 16-bit unit 为单位，并且它的传输速度为 $50KB/s$ 使得数据传输没有遗漏
- <3> 硬盘与处理器传输数据以 4-word chunk 为单位，并且它的传输速度为 $4MB/s$ 使得数据传输没有遗漏

|             | Mouse          | Floppy Disk                    | Hard Disk                   |
| ----------- | -------------- | ------------------------------ | --------------------------- |
| 每秒轮询占用周期数   | $30\times 400$ | $\frac{50KB}{16bit}\times 400$ | $\frac{4MB}{16B}\times 400$ |
| 轮询时间占总时间的比例 | 0.002%         | 2%                             | 20%                         |

从数据中可以看出，要保证硬盘数据传输不丢失数据，处理器单位时间内得有 0.2s 用来进行轮询操作，这是难以接受的性能损失。

对于同样的CPU和硬盘，我们对比使用中断驱动的方式查看数据传输这一操作占处理器总时间的比例。其中，我们认为处理器只有 5% 的时间需要用到硬盘的数据，而中断信号的传输与处理总共需要 500 个时钟周期。

由于硬盘在每传输完一个 Chunk 时都会发送一个中断信号给处理器，表示该 Chunk 传输完成以提醒下一个 Chunk 的传输，所以单位时间内硬盘发送的中断信号所占时钟周期数为：

$$\begin{array}l
\frac{4MB}{16B} \times 500 \times 5\% =6.25\times 10^6\\
\Rightarrow \frac{6.25\times 10^6}{ 500\times 10^6} \times 100\% =1.25\%
\end{array}$$

!!! warning "轮询需要不间断进行以保证不错过信号，而中断只在需要数据时访问磁盘"

仍然是上面的处理器和硬盘，我们使用DMA作为数据传输策略，假定：

- **Initial Setup of a DMA Transfer** 需要 1000 个时钟周期
- **Handling Interrupt of DMA Completion** 需要 500 个时钟周期
- **Average Transfer from Disk** 为 8KB
- 硬盘全程保持传输数据状态

那么：

![[DMAexample1.png]]

