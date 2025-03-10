# Chapter 1. Computer Abstractions and Technology

!!! warning "该章大部分内容都仅作了解，在后续章节会更深入讲解 (只需注意 CPU 的性能)"

## Introduction

在 Pre-computer 时期，计数手段基本都是 Non-electrical, non-programmable (非电子，不可编程) 的

对计算机发展影响巨大的迭代基本可以概括为如下阶段：

- <font color="orange">First Generation</font> - 1946 ~ 1956
	- 电子管(亦称真空管)，可编程，图灵完全
	- 第一台通用计算机 **ENIAC** in 1946
		- 通用指令集、十进制、可编程、图灵完全
		- 没有程序存储能力
- <font color="orange">Second Generation</font> - 1956 ~ 1964
	- 晶体管，编程语言开始应用
	- **IBM 1401** in 1959
		- 可变字长、十进制
- <font color="orange">Third Generation</font> - 1964 ~ 1971
	- 集成电路(IC)，操作系统开始应用
- <font color="orange">Fourth Generation</font> - 1971 ~ now
	- 大规模微处理器(Microprocessor)，图形用户界面，个人电脑兴起
	- **Mac OS** with GUI in 1984
	- **Windows 1.0** in 1985

!!! info "von Neumann architecture (1945)"
	- 计算与存储分离
	- 数据和指令保存在**同一个**存储器
	- 采用指令集构造
	
	![[冯诺依曼架构.png]]


指令集主要分为两类：

- **RISC**: Reduced Instruction Set Computer，例如 *ARM*,*MIPS*,*RISC-V* 等。
- **CISC**: Complex Instruction Set Computer，例如 *x86*

!!! note "RISC-V的优势"
	精简指令集对指令数目和寻址方式都进行了精简，使其执行更容易，指令并行执行程度更好，编译器效率更高。例如，精简指令集的指令编码长度为定长，更方便了取指。

现在绝大部分芯片均采用 RISC 架构，包括 ARM 芯片、智能手机，我们课上需要了解的即 **RISC-V 321 指令集**

## Computer Organization

!!! danger ""
	![[计算机组织.png]]

对于一个 iPad ，其主板构造如下：

![[ipad主板构造.png]]

关于内存，可以参照 [我在 CSAPP 内的相关笔记](https://www.nimisora.top/%E8%AF%BE%E5%A4%96%E7%A7%AF%E7%B4%AF/CSAPP/Chapter6/) 

---

一个程序的执行到达硬件，需要经过 高级编程语言 → 汇编语言 → 机器语言

而指令集可以看作硬件和最底层软件的接口，如图所示：

![[指令集是接口.png]]

## How to build processor

![[芯片制造流程.png]]

!!! extract "摩尔定律"
	每隔 18-24 个月，集成电路上可容纳的元器件的数目便会增加一倍，性能也将提升一倍

目前集成芯片制造面临的主要困难为：

- 集成度越来越高，复杂度快速增加
- 工艺尺寸达到物理极限，加工精度越来越高，制造难度越来越大
- 多个技术的集成：电子，光学，机械，计算机，每一个技术都能难突破


而处理器面临的困境为：

- 处理器和存储器性能的提高不成比例，“内存墙”问题日趋严重，“冯诺依曼”架构面领挑战
- 单位面积功耗增大，散热问题日趋严峻，“功耗墙”问题凸显
- 受物理规律限制，摩尔定律面临失效，靠工艺进步获得的性能红利逐渐减弱


## Performance and Idea

计算机的性能一般由以下两点衡量：

- **Response Time / Execution Time** 响应时间/执行时间
	- 执行任务所需的时间
- **Throughput / Bandwidth** 吞吐率
	- 单位时间内完成的总任务量

一般地，我们将计算机的 Performance 定义为：

$$
Performance = \frac{1}{Execution\ Time}
$$

这里的 Execution Time 可以有两种概念，一是该任务的总运行时间，英文为 Elapsed Time ，包括处理时间、I/O 延迟、操作系统响应时间、idle time 等；另一类是 CPU Time ：

- CPU clock cycle 时钟周期数
- clock rate 时钟频率
- clock cycle time 时钟周期长度

那么 CPU Time 即为：

$$
CPU\ Time =CPU\ Clock\ Cycles \times Clock\ Cycle\ Time=\frac{CPU\ Clock\ Cycles}{Clock\ Rate} 
$$

又很容易想到 $Clock\ Cycles=Instruction\ Count\times Cycles\ per\ Instruction$ **(CPI)**

将上面两条公式整合得到：

$$
CPU\ Time = Instruction\ Count\times CPI\times Clock\ Cycle\ Time= \frac{Instruction\ Count\times CPI} {Clock\ Rate}
$$

!!! info "MIPS: 百万指令数每秒"
	即 $\frac{Instruction\ Count}{Execution\ Time \times 10^6}$ ，但是 MIPS 只是描述同一个处理器或同一个指令集对于任务的优化，不是 MIPS 越高性能就越高。



## Eight Great Ideas

- Design for Moore’s Law （设计紧跟摩尔定律）
- Use Abstraction to Simplify Design (采用抽象简化设计)
	- 分层设计，各层之间提供接口
- Make the Common Case Fast (加速大概率事件)
	- 优先优化大概率事件对性能的影响更大
- Performance via Parallelism (通过并行提高性能)
- Performance via Pipelining (通过流水线提高性能)
- Performance via Prediction (通过预测提高性能)
- Hierarchy of Memories (存储器层次)
- Dependability via Redundancy (通过冗余提高可靠性)

