
# Operating-System Structures

## OS Services

从用户角度来看，OS 提供了一系列有用的服务：

- **User Interface:** 包括 Command-Line(CLI), Graphics User Interface(GUI) 和 Batch 等
- **Program Execution:** 提供将程序加载进内存执行的能力
- **I/O Operations:** 一个正在运行的程序可能需要 I/O，涉及 File 和 I/O Device 的管理
- **File System Manipulation:** 文件的增删改查，以及权限管理等
- **Communications:** 进程之间有时需要交换信息，这一交换也有可能是通过网络的不同主机之间的
	- 交流可以通过 Shared Memory 或 Message Passing 等方式实现
- **Error Detection:** OS 需要时刻注意潜在的错误
	- 对于每一种错误，OS 都需要采取合适的对策保证正确性与计算的连贯性

而从 System 的角度来看，OS 提供了以下服务：

- **Resource Allocation:** 资源需要在多用户、多进程之间分配
- **Accounting:** 跟踪哪个用户使用多少资源和哪种资源
- **Protection and Security:** 
	- Protection 包括确保所有系统资源的访问都是受控的
	- Security 包括对外来者进行用户认证、对外部 I/O 设备进行检查

## System Calls

System Calls 是 OS 提供的编程接口服务，大部分用户都通过打包后的高层 Application Program Interface（API） 来间接调用。

实现上，CPU 的运行模式被分为用户态（**User Mode**）和内核态（**Kernel Mode**）。当 CPU 处于用户态时，CPU 只能执行非特权指令。通常，我们的应用程序运行在用户态，而操作系统内核运行在内核态。

虽然我们可以在用户态调用相关的 Syscall 指令，但是执行系统调用时会发生特权级的切换，因此它属于**特权相关指令（privilege-sensitive）**。

!!! info "三个最普遍的 API"
	- Win32 API for Windows
	- POSIX API for POSIX-based Systems
		- 几乎所有的 UNIX, 以及 Linux 和 MacOS
	- Java API for JVM

凡是与资源相关的操作，都必须通过系统调用向操作系统提出服务请求。按照功能，System Calls 可以分为以下几类：

- Process Control
- File Management
- Device Management
- Information Maintenance (e.g. time, date)
- Communications
- Protection

!!! abstract "一些举例"
	![[topic2_2.png]]

每一个系统调用都有唯一编号，当用户程序发起系统调用时，通过一个 table 查找到对应的内核服务。

因此对于调用者来说，应用程序只需要按照 API 的规范调用函数，而不必知道系统调用在内核里的具体实现。

??? example "例如 `printf()` API 中就包含了 `write()` 这一系统调用"
	![[topic2_1.png]]

很多系统调用还需要一些参数，通常有三种传递方法：

- <1> **Pass the parameters in registers**
	- 但是有时候需要的参数大于可用的寄存器数量
- <2> **Parameters stored in a block(or table) in memory, and address of block passed as a paramter in register**
	- 这也是 Linux 和 Solaris 使用的方法
- <3> **Parameters pushed onto the stack by user program and popped off by the OS**

System Programs 本身并不是 kernel 的一部分，但是它们依赖系统调用来完成任务，这些程序为用户提供了一个更方便的工作环境，它们简化了编程、运行、文件管理等操作。系统程序可分类为：

- **File Manipulation** 文件操作
	- 对 files 和 directories 进行 Create, Delete, Copy, Rename, Print, Dump, List 等操作
	- `cp`, `rm`, `dir`, `ls`...
- **Status Information** 状态信息
	- 显式系统运行状态、资源使用情况
	- `top`, `ps`, `df`...
- **File Modification** 文件修改
	- 文本编辑器
	- `vim`, `nano`...
- **Programming Language Support** 编程语言支持
	- Compilers, Assemblers, Debuggers, Interpreters
	- `gcc`, `python`, `gdb`...
- **Program Loading & Execution** 程序加载与执行
	- Loaders
	- `ld`, `exec`...
- **Communications** 通信
	- 为进程之间、用户之间、不同系统之间建立 virtual connections
	- `ssh`, `ftp`...

## OS Structure

随着操作系统功能的不断增多和代码规模的不断扩大，提供合理的结构，对降低操作系统复杂度、提升操作系统安全与可靠性来说变得尤为重要。

<font style="font-weight: 1000;font-size: 20px" color="orange">分层法</font>

**分层法**将操作系统被划分为多个层次，每个层次都**只能使用**它的下一层提供的 functions 和 services，这是一个单向依赖。

!!! note "最底层（layer 0）为硬件，最高层（layer N）为 User Interface"

最原始的 UNIX OS 只包含 System Programs 和 Kernel 两个层次，它在一定程度上分层，属于 **单体结构 Monolithic Structure**：

![[topic2_3.png]]

分层法简化了系统的设计与调试，易于扩充维护，但是会导致依赖关系固定后显得不灵活，并且执行每个功能都需要自上而下穿越多层，效率较差。

<font style="font-weight: 1000;font-size: 20px" color="orange">Microkernel</font>

按照操作系统的内核架构来划分，可以分为宏内核和微内核两种。

**Microkernel** 的思想希望把本来放在内核里的功能尽量移动到用户态（User Space）中运行，不同模块之间通过 Message Passing 进行交流。这一思想的优点包括：

- **可扩展性 (Easier to extend)**：新功能可以作为用户态服务加进来，不必改内核。
- **可移植性 (Easier to port)**：只要把小小的微内核移植到新硬件，其余用户态服务几乎不用大改。
- **可靠性 (More reliable)**：出 bug 的代码大部分跑在用户态，崩溃不会导致整个系统崩溃，只需要重启对应模块即可。
- **安全性 (More secure)**：内核态代码少，攻击面更小。

为了提供应用程序与同样运行在用户空间中的各种服务的通信，微内核需要频繁地在用户态和内核态进行切换，操作系统的执行开销较大，从而导致了明显的性能问题。

一个经典的微内核架构如下：

![[topic2_4.png]]

??? tip "MacOS 属于混合型架构"
	![[topic2_5.png]]
	
	!!! quote "目前的主流操作系统虽然都是宏内核架构，但也吸收很多微内核架构的优点"

<font style="font-weight: 1000;font-size: 20px" color="orange">模块化</font>

大多数现代 OS 都采用了**内核模块化设计（Kernel Modules）**，它采用面向对象设计，将各个核心功能都做成独立模块。不同模块之间不使用 Message Passing，而是直接调用彼此的**接口函数**，因此效率更高。

上面提到的分层法就是实现系统模块化的一种方式。而**可加载内核模块 loadable kernel modules, LKM**在实现上更加优秀，相比于分层法，它允许任何模块都能直接调用其它模块的接口。在启动时，内核的核心模块会被加载；而运行时需要用到其它模块功能时才会加载对应模块。

例如，Solaris 的 Modular Approach 设计如下：

![[topic2_6.png]]

## Virtual Machines

虚拟机将硬件+内核整体看作“硬件层”，在它上面再做了一层抽象。VM 对运行在之上的软件提供和真实物理硬件一样的接口，从而让操作系统或应用感觉自己直接跑在真实机器上。

![[topic2_7.png]]

## System Boot

每台具体机器的硬件环境并不相同，操作系统使用程序 `SYSGEN` 收集硬件配置信息，从而针对该机器生成适配版本。

计算机启动时，需要把 Kernel 装进内存中并开始执行，这个过程称为 Booting。

CPU 被硬编码为通电后立即跳转到内存地址空间中的一个特定位置，即 **Bootstrap Program**。它是存于 ROM 里的一小段代码，负责定位操作系统内核，把它加载到内存中，并启动运行。

!!! warning "Bootstrap & Bootloader"
	- **Bootstrap Program (引导程序/固件)**
	    - **位置**：它是**硬件**的一部分。存放在主板的 ROM/Flash 芯片中（即 BIOS 或 UEFI）。
	    - **本质**：它是**固件 (Firmware)**。即使你的电脑硬盘被拆掉了，里面没有安装任何操作系统，这段代码依然存在且可以运行（比如你能进入 BIOS 设置界面）。
	    - **职责**：负责“叫醒”电脑硬件（自检），它的最后一项工作是**找到**硬盘上的 Bootloader。
	    - Bootstrap Program 不属于操作系统
	- **Bootloader (引导加载器)**
	    - **位置**：它是**软件**。存放在你的硬盘（MBR、ESP分区）或 USB 启动盘上。
	    - **本质**：它是**以文件形式存在的程序**。如果你格式化硬盘，它就会消失。
	    - **职责**：负责“搬运”。它的核心工作是把操作系统内核（Kernel）从硬盘搬到内存里，然后把接力棒交给内核。
	    - Bootloader 通常也不属于操作系统，但是它通常随操作系统配套安装
