
# Overview

!!! quote "这节的内容只是为了留个印象"

## Introduction

操作系统是一个特殊的程序（Program），起到 User 和 Computer Hardware 之间中介的作用。概括来讲，操作系统的目标是执行用户程序并让解决用户问题变得更简单，以及高效利用硬件资源。

一个计算机系统可以被分为四个部分：

- **Hardware:** 提供基本计算资源
	- CPU, Memory. I/O Devices...
- **Operating System:** 在众多用户和应用中控制并协调硬件资源
- **System/Application Programs:** 决定系统资源的使用方式，用来解决用户的计算问题
	- Compilers, Web Browsers, Database Systems, Video Games...
- **Users**
	- People, Machines, Other Computers...

事实上，操作系统并没有一个统一的定义，大家都认可的部分是**内核（Kernel）**，它是始终在运行的部分，其余的则包括 System Program 和 Application Program，所以 OS 的范围差异很大。

在计算机刚启动时，**bootstrap program** 被加载，它通常存储在 ROM 或 EPROM 中，也被称为 firmware。

bootstrap program 会初始化硬件，加载操作系统内核并跳转到内核的入口。

!!! note "现代 PC 把 bootstrap 程序称作 BIOS 或 UEFI 固件"

在计算机中，I/O 设备可以和 CPU 并行执行，每一个 Device Controller 都负责管理一个特定的 Device，CPU 并不会直接操控设备，而是通过 Controller 下令。

更详细来讲，每个 Device Controller 都有一个 local buffer，CPU 通过 Main Memory 来和这些 local buffer 进行数据交互。当控制器完成一项 I/O 操作时，会通过 System Bus 发送一个**中断信号**给 CPU，CPU 接收到信号后就会暂停当前任务，去执行对应的中断处理程序。

操作系统其实就是**中断驱动**的，中断会让 CPU 的控制权转移到 **Interrupt Service Route(ISR)**，为了让中断结束后能够恢复执行，CPU 必须保存被打断的上下文，这样 ISR 处理完毕后，CPU 就能回到原来被打断的程序继续执行，就好像什么都没发生过。

!!! info "上下文包括寄存器的值以及 PC"

在处理中断时，通常会暂时屏蔽新的中断，否则可能会造成丢失中断的问题。有些系统还允许有*中断优先级*，高优先级的 ISR 可以打断低优先级的 ISR。

我们用 **trap** 描述一种控制转移的过程，而这个过程是由 interrupt 或 exception 引起的。通常为了方便起见，我们约定 trap 为 interrupt 和 exception 的总称。

!!! danger "不同体系结构中定义可能不同，有的只把 exception 称作 Trap（例如 sld 的 PPT）"

> [RISC-V Unprivileged Spec](https://github.com/riscv/riscv-isa-manual/releases/download/20240411/unpriv-isa-asciidoc.pdf) 1.6 节中对于 trap、interrupt 与 exception 的描述:
> We use the term _exception_ to refer to an unusual condition occurring at run time **associated with an instruction** in the current RISC-V hart. We use the term _interrupt_ to refer to an **external asynchronous event** that may cause a RISC-V hart to experience an unexpected transfer of control. We use the term _trap_ to refer to **the transfer of control to a trap handler** caused by either an exception or an interrupt.

|Interrupt|Exception|
|---|---|
|Hardware generate|Software generate|
|These are **asynchronous external requests** for service (like keyboard or printer needs service).|These are **synchronous internal requests** for service based upon abnormal events (think of illegal instructions, illegal address, overflow etc).|
|These are **normal events** and shouldn’t interfere with the normal running of a computer.|These are **abnormal events** and often result in the termination of a program|

??? info "Synchronous I/O Methods"
	早期 I/O 其实同步、阻塞式的，程序发起 I/O 后，必须等到 I/O 完成才能继续执行。CPU 在等待期间，要么会执行一条特殊的 `WAIT` 指令，什么也不干直到收到设备的中断；要么轮询检查设备状态，但这种方式不仅浪费 CPU 时间，还有可能和正常程序抢占内存带宽。
	
	![[topic1_1.png]]

除了中断驱动 I/O 外，还有一种高效的 I/O 传输机制为 **Direct Memory Access(DMA)**。

在早期的程序控制 I/O 或中断驱动 I/O 中，如果要传输一个大文件，每次传输一个字节 CPU 都会被中断，其大部分时间都浪费在搬运数据上。

而对于 DMA，设备控制器直接负责 local buffer 和 Main Memory 之间的数据传输，完全绕过 CPU 控制。CPU 只需要负责给 DMA 下达指令（例如源地址、目标地址、数据大小等），DMA 控制器在整个数据块传完后才会给 CPU 发送一个中断信号。

因此，DMA 通常用于高速 I/O 设备上，例如硬盘、网卡等。

## Operating-System Structures

### Process Management

!!! quote "对进程的调度就是对 CPU 资源的调度"

一个操作系统中要运行很多程序，Kernel 需要负责其它程序的运行。

- <1> **Batch Processing Systems:** (单道)批处理系统
	- Jobs 存储在内存/外存中，而内存中始终有一个 Job 在运行
	- 操作系统负责在内存中 Job 结束时加载下一个 Job 到内存中运行
	- 我们将加载到内存中运行的程序称为**进程（Process）**，这是一个动态的概念
- <2> **Multiprogramming Batch Systems:** 多道批处理系统
	- 在批处理系统基础上，当正在运行的 Job 发生 I/O 操作时，操作系统负责让 CPU 转而运行另一个 Job
	- 多道批处理系统通过这种方式避免了批处理系统等待用户输入等 I/O 事件的 CPU 时间浪费
- <3> **Time Sharing Systems:** 分时系统
	- 将时间划分为很小的时间片，操作系统负责安排各个 Job 轮流运行
	- 由于时间频率很高，远高于人类的反应速度，因此操作系统可以快速在多个进程之间切换运行，让用户误以为这些进程是同时运行的
	- 分时系统仍然是多道的（Multiprogramming），但已经不是批处理（Batch）的了；它相比多道批处理系统拥有了更好的*交互性*

!!! abstract "除此之外还有实时操作系统、网络操作系统、分布式计算机系统等"
	![[topic1_2.png]]

程序本身是存放在磁盘上的静态代码，想要变成进程运行，必须占用 CPU、内存、I/O 设备、文件等资源。

对于单线程进程，一个进程只有一个程序计数器 PC，一次只执行一条指令。而对于多线程进程，一个进程中可以有多个执行流，每个线程有自己的 PC，可以并发执行。

同时，系统中通常有很多进程，**并发（Concurrency）**是通过 CPU 在多个进程/线程之间不断切换实现的。

操作系统是“进程的管理者”，负责进程的生命周期控制、协调、通信和避免冲突，具体包括：

- **创建和删除进程**
    - 操作系统负责启动新的进程（用户程序、系统服务等），并在进程完成或被终止时清理、回收资源。
- **挂起和恢复进程**
    - 有时候进程需要暂停（比如等待 I/O），OS 可以把它挂起；
    - 当条件满足时，OS 又能让它恢复执行。
- **进程同步（Process Synchronization）**
    - 多个进程可能需要协调合作，比如访问共享内存、修改同一文件。
    - OS 提供同步机制（锁、信号量、管程等），避免数据冲突或竞争条件。
- **进程通信（Process Communication, IPC）**
    - 进程之间需要交换数据（例如浏览器和插件之间）。
    - OS 提供通信方式：管道、消息队列、共享内存、socket 等。
- **死锁处理（Deadlock Handling）**
    - 多个进程可能因为互相等待对方的资源而卡死（死锁）。
    - OS 要么 **检测并解除死锁**，要么 **避免/预防死锁发生**。

### Memory Management

计算机的存储分层模型在很多课都讲过，此处不再重复赘述。

程序运行前的数据必须被加载到内存中，程序处理后的结果也要存在内存中。所以操作系统要决定什么时候、哪些内容放在内存中。

内存管理的目标是为了优化 CPU 利用率，并提高响应速度，其内容包括：

- **记录（Tracking）**：跟踪当前哪些内存区域被占用，属于哪个进程。
- **决定调度（Deciding）**：选择哪些进程/数据要装入内存，哪些要换出。
- **分配与回收（Allocating/Deallocating）**：当进程需要内存时分配，用完后回收。

### Storage Management

操作系统把各种复杂的物理存储设备抽象成一个统一的逻辑概念**文件（File）**，用户不需要关心设备的物理细节，只需要对这些“文件”操作即可。

文件通常被组织成 directories 的形式，并通过 Access Control 进行权限管理。File-System Management 的核心工作包括：

- **文件和目录管理**：创建、删除文件和目录，支持目录层次结构。
- **文件操作接口**：提供操作文件的基本原语（open、read、write、close 等）。
- **文件映射**：把文件逻辑结构映射到物理存储设备（如磁盘块）。
- **访问控制**：决定谁能访问文件、能做什么（读/写/执行权限）。
- **备份与恢复**：把文件存到更稳定的介质（如磁带、云存储），防止数据丢失。

磁盘通常用来存放放不进内存中的大量数据以及需要长期保存的数据，磁盘子系统性能好坏将直接决定整个计算机的运行速度（尤其是 I/O 密集型任务），所以高效的磁盘管理算法对操作系统来说十分重要。

操作系统在磁盘管理中的主要工作包括：

- **空闲空间管理（Free-space management）**：记录哪些磁盘块是空的，能分配给新文件。
- **存储分配（Storage allocation）**：决定文件数据放在哪些磁盘块（连续分配、链式分配、索引分配等）。
- **磁盘调度（Disk scheduling）**：当多个进程发出 I/O 请求时，选择合理的访问顺序（比如 FCFS、SSTF、SCAN、C-SCAN）以减少磁盘寻道时间，提高性能。

!!! note "一些存储介质不需要很快，如 Optical Storage 和 Magnetic Tape，但仍然需要被管理"

