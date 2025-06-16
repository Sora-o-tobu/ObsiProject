
# 1. Fundamentals of Computer Design

!!! quote
	![[CAquote.png]]

!!! warning "第一章第二章和计组重合度有点高，且书上讲的比较杂，最好还是看看期末会考什么"

## Introduction

### Class of Computer

计算使用方式的变化促进了 5 种计算机市场的形成，其中每一种都有自己不同的应用、需求和计算技术。下表总结了主流计算环境及其重要特征：

| Computer                             | Critical Design Issue | Addition                                                             |
| ------------------------------------ | --------------------- | -------------------------------------------------------------------- |
| Personal Mobile Device<br>(PMD)      | 成本、能耗、媒体性能、响应速度       | PMD与嵌入式不同的是，它可以运行外部开发第三方软件<br>(externally developed software)        |
| Desktop                              | 性价比、能耗、图形性能           | 最大的计算机市场，趋向于寻找最优性价比                                                  |
| Server                               | 吞吐量、可用性、可扩展性、能耗       | 服务器的三个特征：不间断工作，Availablity 至关重要；需求日益增多，Scalability 很重要；整体性能(吞吐量)很重要。 |
| Cluster/warehouse-<br>scale computer | 性价比、吞吐量、能耗均衡性         | 集群指一组计算机通过局域网连接，每个节点都有自己的 OS，节点之间使用网络协议通信。非常昂贵。                      |
| Internet of<br>things/embedded       | 价格、能耗、特殊功能的性能         | 物联网(IoT)通常指以无线方式连接互联网的嵌入式计算机，设备之间处理能力和价格相差极大。                        |

### Class of Parallelism

多种级别的并行已成为计算机设计的主要驱动力，能耗和成本是其主要约束。应用层面中，有以下两种基本并行：

- **<1> Data-Level Parallelism(DLP):** 同时操作多个数据项
- **<2> Task-Level Parallelism(TLP):** 并行地单独执行多个工作任务

在计算机硬件中，以如下四种主要方式来利用、实现这两种类型的应用并行：

- **<1> Instruction-Level Parallelism(ILP):** 在流水线、推测执行(Speculative Execution)两个层面上对 DLP 进行利用
	- Chapter 3
- **<2> Vector architectures, graphic processor unit(GPUs), and multimedia instruction sets:** 将单条指令并行应用于一组数据，以利用数据级并行 DLP
	- Chapter 4
- **<3> Thread-Level Parallelism:** 在 Tightly Coupled Hardware 中利用 DLP 或 TLP 。这种模型允许并行线程之间进行交互 
	- Chapter 5
- **<4> Request-Level Parallelism(RLP):** 利用程序员或操作系统指定的大量解耦任务之间的并行性
	- Chapter 6

**Flynn** 在研究并行计算工作量时，提出了一种简单的分类方式：

**S**ingle/**M**ultiple **I**nstruction/**D**ata stream

- **SISD:** 单处理器，但是可以利用指令级并行 ILP
	- Chapter 3
- **SIMD:** 同一条指令由多个使用不同数据流的处理器执行，利用数据级并行 DLP
	- Chapter 4
- **MISD:** 没有这种架构的商用处理器
	- 不存在
- **MIMD:** 每个处理器都提取自己的指令，对自己的数据进行操作，针对任务级并行 TLP
	- Chapter 5, 6

!!! info "这个分类很粗略，很多并行处理器其实是 SISD、SIMD、MIMD 的混合类型"

### CA Definition

CA must design the organization and hardware to meet functional requirements as well as price, power, performance, and availability goals

!!! quote "Definition: Instruction Design & Implementation 指令集及其实现"

ISA 相当于软硬件之间的界限，下面将从七个方面来简要介绍以下 ISA：

- **<1> Class of ISA:** 几乎所有 ISA 都是通用寄存器体系结构，即 Register Machine，操作数要么是寄存器，要么是存储地址。这一类别又可分为两种主流版本
	- **Register-Memory ISA:** 80x86, 许多指令可以直接访问内存
	- **Load-Store ISA:** RISC-V, ARMv8, 只有 load, store 能够访问内存
		- 自 1985 年后发布的 ISA 都是 Load-Store ISA
- **<2> Memory Addressing:** 几乎所有计算机都采用字节寻址来访问存储器。
	- 有些体系结构(如 ARMv8)要求操作对象对齐。80x86 和 RISC-V 不要求对齐，但是如果操作数是对齐的，访问速度通常会快一些
- **<3> Addressing Modes:** 寄存器寻址、立即数寻址、位移量寻址等
- **<4> Types and sizes of operands:** 8位(ASCII字符)、16位(Unicode字符或 half word)、32位(integer 或 word)、64位(double-word 或 long integer)、IEEE 浮点数(32bit for single, 64bit for double, 80bit for extended double)
- **<5> Operations:** 常见的操作有数据传输指令、算术逻辑指令、控制指令和浮点指令
- **<6> Control FLow Instructions:** 条件分支、无条件跳转、过程调用及返回
- **<7> Encoding:** 有固定长度和可变长度两种基本的编码选择。
	- ARMv8 和 RISC-V 都是固定 32-bit 长度指令
	- 80x86 的编码是可变长度，变化范围是 1-18 Bytes

??? tip "Class of ISA (据说出过一道题)"
	- **<1> Register Machine** 拥有显式的操作数
		- Register-Memory ISA，如 x86
		- Load-Store ISA(Register-Register ISA)，如 RISC-V
	- **<2> Stack Machine** 隐式操作数，操作数位于栈顶
		- 操作数从栈顶弹出，计算结果压回栈顶
	- **<3> Accumulator Machine**
		- 有一个临时的累加器，操作数放在累加器内计算
	
	![[classofisa.png]]

## Trends

!!! quote "Performance-Driven: Quantatives 🤔"

### Technology

要为计算机的发展做长远计划，设计人员必须了解 Implementation 技术的快速变化，以下五种实现技术是现代计算机不可或缺的：

- 芯片工艺 **Integrated Circuit Logic Technology**
	- 摩尔定律逐渐失效
- Memory 工艺 **Semiconductor DRAM**
	- Dynamic Random-Access Memory
	- 容量、延迟、带宽
- SSD 工艺 **Semiconductor Flash**
	- Electrically Erasable Programmable Read-Only Memory
	- 非易失性半导体存储
- 磁盘工艺 **Magnetic Disk Technology**
	- 是服务器存储和仓库级存储的核心技术
- 网络工艺 **Network Technology**
	- 延迟、带宽
	- 网络性能取决于交换机和传输系统的性能

这些快速发展的技术左右着计算机设计的命运。由于计算速度的激增和技术的迅猛发展，一种计算机设计的生存期可能只有 3-5 年。

Bandwidth 或 Throughput 指给定时间完成的总工作量，比如磁盘读写的每秒传输兆字节数。
相对的，Latency 或 Response Time 指一个事件从开始到完成所经过的时间，比如一次磁盘访问需要的毫秒数。

!!! info "带宽与延迟的改进里程碑的双对数曲线（直线是 1：1）"
	![[loglogbandwidthlatency.png]]
	
	在技术的发展过程中，Bandwidth 的发展速度超过了 Latency，并且这一趋势很可能会继续持续下去。

### Power and Energy

对于 CMOS 芯片来说，传统的主要能耗是开关晶体管，即动态能耗(**Dynamic Energy**)。每个晶体管的能耗跟该晶体管驱动的容性负载与电压平方的乘积平方成正比：

$$
Energy_{Dynamic} \propto Capacity\ Load \times Voltage^2 
$$

!!! info "上式表示逻辑转换脉冲 0=>1=>0 或 1=>0=>1 所需的能耗，单次转换乘上 $\frac{1}{2}$ 即可"

每个晶体管所需的**功率**就是一次转换的能耗乘上转换频率：

$$
Power_{Dynamic} \propto \frac{1}{2}\times Capacity\ Load \times Voltage^2 \times Frequency\ Switched
$$

对于一项 *Fixed Task* ，降低时钟频率可以降低 $Power$，但不一定能降低 $Energy$。(课本原话)

!!! note "个人认为，只有降压才能减少一个固定工作量的能耗；但是实际上降频和降压往往是相伴出现的"
	$$Energy =Power \times Execution\ Time \propto Capacity\ Load \times Voltage^2 $$

配电、散热和防热点的难度日益增加，能耗是现在晶体管的主要限制因素。因此，现代微处理器提供了许多技术，试图提高能效（**Energy-Effiency**）：

- **<1> Do nothing well:** 关闭 inactive modules 的时钟，例如当前没有执行浮点指令，则浮点单元的时钟将被禁用
- **<2> Dynamic Voltage-Frequency Scaling, DVFS:** 在一些活跃程度较低的时段，不以最高频率和电压运转
- **<3> Design for the typical case:** PMD 和笔记本等设备经常空闲，所以内外存储器都提供了低功耗模式，以减少能耗，延长电池寿命
- **<4> Overlocking:** 即超频，大部分高性能微处理器都提供了短时超频的功能

除了上述所说的占主要部分的动态功耗，即便晶体管处于关闭状态也存在的功耗称为静态功耗：

$$
Power_{static} \propto Current_{static} \times Voltage
$$

!!! note "静态功耗和元器件数目成正比"

### Cost

尽管成本在一些计算机设计（特别是超级计算机）中不是特别重要，但对成本敏感的设计却越来越重要。

Cost Trend 的影响因素主要有三点：Time（时间）、Volumn（产量）、Commoditization（大众化，商品化）

!!! danger "Cost 部分偏商业，不讲"

## Dependability

系统在 SLA(Service Level Agreement) 规定的两种服务状态中转换：

- <1> Service Accomplishment，即提供了指定服务
- <2> Service Interruption，即所提供的服务与 SLA 不符

!!! info "服务等级协议 SLA 是基础设施供应商为客户提供的保证，当不满足 SLA 时可能需要向客户作补偿"

两种 SLA 状态通过 *failures*($1\Rightarrow 2$) 或者 *restorations*($2\Rightarrow 1$) 进行转换。对这两种转换进行量化，得到 dependability 的两种度量：

- **MTTF** mean time to failure 平均无故障时间
	- 故障率即为 $\frac{1}{MTTF}$
- **MTTR** mean time to repair 平均修复时间
- **MTBF** mean time between failures
	- = MTTF + MTTR
- **Module Reliability** 可靠性
	- 从参照点至出现故障经过时间的量度，即通常通过 MTTF 作为量度
	- 在计算机组成中该处为 Dependability，均翻译为可靠性
- **Module Availability** 可用性
	- $= \frac{MTTF}{MTTF+MTTR}= \frac{MTTF}{MTBF}$

通过冗余提高 Dependability，RAID。见计组部分。

!!! example "MTTF 例题"
	=== "题面"
		A system consists of the following components, calculate the MTTF of the system:
		
		- 10 disks, 1000000 hour MTTF
		- 1 SCSI Controller, 500000 hour MTTF
		- 1 power supply, 1 fan, both 200000 hour MTTF
		- 1 SCSI cable, 1000000 hour MTTF
	=== "答案"
		可以简单认为一个系统的故障率是所有组件的故障率之和，故障率即为 MTTF 的倒数，那么：
		
		$$\frac{1}{MTTF}= 10\times \frac{1}{1000000} + \frac{1}{500000} + 2\times \frac{1}{200000} + \frac{1}{1000000}$$
		
		计算得到 $MTTF=43500 hour$

!!! example "历年曾考过的原题"
	=== "题面"
		If a company has 10,000 computers, each with a MTTF of 30 days, and it experences catastrophic failure only if 1/3 of the computers fail. What is the MTTF of the system?
	=== "答案"
		- 电脑每天故障的概率 $\frac{1}{30}$
		- 每天故障的电脑数量 $10000\times \frac{1}{30}$
		- 当有 $10000 \times \frac{1}{3}$ 电脑发生故障时，系统发生故障
		- 崩溃天数为 $\frac{10000\times \frac{1}{3} }{10000\times \frac{1}{30}} = 10$ 天

## Performance

对计算机性能的直观估计就是执行时间的倒数：

$$
Peformance = \frac{1}{Execution\ time}
$$

根据测量方式，Execution Time 也会有不同的定义方式：

- **wall-clock time/response time/elapsed time** 完成一项任务的 latency，包括 storage accesses、memory accesses、I/O activities、OS overheading 等所有相关时间
- **CPU Time** 只指 CPU 执行计算的时间

此外，用户计算他们的计算机相对性能的方式使用 Benchmark Program

!!! example "24 秋冬期末"
	Which one of the following performances generates consistent result, no matter which machine is the reference?
	
	- A. arithmetic mean
		- $AM= \frac{1}{n} \sum_{i=1}^n Time_i$
	- B. weighted arithmetic mean
		- $WAM=\sum_{i=1}^n Weight_i \times Time_i$
		- 也可以缩写为 *WEM* (Weighted Execution Time)
	- **C. normalized geometric mean**
		- 几何平均值 $GM$ 具有统一的结果，无论参考机器是什么
		- $GM=\sqrt[n]{\Pi_{i=1}^n Relative\_Rate_i}$
		- $\frac{GM(X)}{GM(Y)}=GM(\frac{X}{Y})$
	- D. harmonic mean
		- 本质和 arithmetic mean 相同
		- $HM=\frac{n}{\sum_{i=1}^n \frac{1}{Rate_i}}$

## Quantitative Principles

以下几点为计算机设计与分析中非常有用的指导原则：

- **Take advantage of Parallelism**
- **Principle of Locality**
- **Focus on Common Case**
- **Amdahl's Law**

