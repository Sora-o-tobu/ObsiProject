
# Operating-System Structures

## OS Services

从用户角度来看，OS 提供了一系列有用的服务：

- **User Interface:** 包括 Command-Line(CLI), Graphics User Interface(GUI) 和 Batch 等
- **Program Execution:** 提供将程序加载进内存执行的能力
- **I/O Operations:** 一个正在运行的程序可能需要 I/O，设计 File 和 I/O Device 的管理
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

!!! info "三个最普遍的 API"
	- Win32 API for Windows
	- POSIX API for POSIX-based Systems
		- 几乎所有的 UNIX, 以及 Linux 和 MacOS
	- Java API for JVM

按照功能，System Calls 可以分为以下几类：

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

操作系统被划分为多个层次，每个层次都只使用它的下层提供的 functions 和 services。

!!! note "最底层（layer 0）为硬件，最高层（layer N）为 User Interface"

最原始的 UNIX OS 只包含 System Programs 和 Kernel 两个部分，属于 **Monolithic Structure**：

![[Topic2_3.png]]

**Microkernel** 的思想希望把本来放在内核里的功能尽量移动到用户态（User Space）中运行，不同模块之间通过 Message Passing 进行交流。这一思想的优点包括：

- **可扩展性 (Easier to extend)**：新功能可以作为用户态服务加进来，不必改内核。
- **可移植性 (Easier to port)**：只要把小小的微内核移植到新硬件，其余用户态服务几乎不用大改。
- **可靠性 (More reliable)**：出 bug 的代码大部分跑在用户态，崩溃不会导致整个系统崩溃。
- **安全性 (More secure)**：内核态代码少，攻击面更小。

缺点是 User Space 和 Kernel Space 之间存在频繁通信，带来一定开销。

一个经典的微内核架构如下：

![[topic2_4.png]]

??? tip "MacOS 属于混合型架构"
	![[topic2_5.png]]

而大多数现代 OS 都采用**内核模块化设计（Kernel Modules）**，它采用面向对象设计，将各个核心功能都做成独立模块。不同模块之间不使用 Message Passing，而是直接调用彼此的接口函数，因此效率更高。

例如，Solaris 的 Modular Approach 设计如下：

![[topic2_6.png]]

## Virtual Machines

虚拟机将硬件+内核整体看作“硬件层”，在它上面再做了一层抽象。VM 对运行在之上的软件提供和真实物理硬件一样的接口，从而让操作系统或应用感觉自己直接跑在真实机器上。

![[topic2_7.png]]

## System Boot

每台具体机器的硬件环境并不相同，操作系统使用程序 `SYSGEN` 收集硬件配置信息，从而针对该机器生成适配版本。

计算机启动时，需要把 Kernel 装进内存中并开始执行，这个过程称为 Booting。**Bootstrap Program** 是存于 ROM 里的一小段代码，它负责定位操作系统内核，把它加载到内存中，并启动运行。