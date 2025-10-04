
# Processes

## Concept

在 Batch System 中，我们称运行的程序为 *job*；在 Time-Sharing System 中，我们称运行的程序为 *task*。在教科书中，上面的两个概念几乎与**进程（Process）**等价。

进程是一段 Program 在操作系统中的执行实体，每个进程都有自己独特的内存空间，包括**用户部分**和**内核部分**。

对于用户部分，text 和 data 部分所需要的空间在一开始就被确定，而 heap 和 stack 都可以动态的扩展和收缩：

- **Text section:** 代码区
- **Data section:** 数据区
- **Heap:** 堆
- **Stack:** 栈

我们使用 **Process Control Block(PCB)** 来保存一个进程的内核部分，这些控制信息包括：

- **Process State:** 进程状态
- **Program Counter:** 程序计数器
- **CPU Registers:** 寄存器信息
- **CPU Scheduling Information:** 调度信息
- **Memory-management Information:** 内存管理信息
- **Accounting Information:** 动态数据的统计与记录信息
- **I/O Status Information:** I/O 状态信息

当 CPU 需要在不同进程之间切换时，它需要保存上一个进程的 PCB，并加载下一个进程的 PCB 信息：

![[topic3_2.png]]


一个进程的状态图如下：

![[topic3_1.png]]

- **new:** 进程正在被创造
	- 创建过程会分配初始资源，包括 PCB 等
- **running:** 指令正在被执行
	- 即正在使用 CPU 资源
	- CPU 有几个 core，就最多有几个进程处于 running 状态
- **waiting:** 进程正在等待某些事件，或正因为某些事件被阻塞
	- waiting 状态下，即便有空余 CPU 资源，该进程也不会继续
- **ready:** 进程正在等待被分配给处理器
	- 一旦有空余 CPU 资源，就会进入 running 状态
- **terminated:** 进程结束执行

## Scheduling

为了进行进程的调度，OS 维护了三条相关的 Scheduling Queues：

- **Job queue:** set of all processes in the system
- **Ready queue:** set of all processes in main memory, ready and waiting to execute
- **Device queues:** set of processes waiting for an I/O device
	- 也被称为 wait queues，等待队列

进程在 ready 状态时需要等待 CPU 资源的派发；在 waiting 状态时需要等待对应 I/O 完成或事件完成。我们通常使用链表作为数据结构对 Ready Queue 和 Wait Queue 进行维护：

![[topic3_3.png]]

实际上，不同进程会因为不同的时间而等待，因此 Wait Queues 实际上存在很多条；在结束 Waiting 后，进程就会进入 Ready Queue，因此实际情况可能如下：

![[topic3_4.png]]


Scheduler 是一个决定哪个程序该执行的程序，它在多进程系统中非常关键，可以分为 Long-term Scheduler 和 Short-term Scheduler。

- **长期调度器 (Long-term scheduler / Job scheduler)**
    - 决定**哪些作业/进程可以进入内存**，放进就绪队列 (ready queue)。
    - 长期调度器控制系统中同时有多少个进程在竞争 CPU，如果太多则会造成频繁的切换，导致效率下降；如果太少则 CPU 可能空闲，浪费资源
    - 长期调度器通常会让 I/O-bound Process 和 CPU-bound Process 混合进入系统，防止 CPU 或 I/O 子系统长期空闲
- **短期调度器 (Short-term scheduler / CPU scheduler)**
    - 决定**下一个由 CPU 执行的进程**，负责从就绪队列中挑选
    - 发生频率很高（毫秒级），需要效率极高。

!!! tip "有些系统还有 Medium Term Scheduler"


## Operations on Processes

### Process Creation

当一个进程创建了另一个进程时：

- 创建者叫 **父进程 (parent process)**
- 被创建的叫 **子进程 (child process)**

子进程本身也可以再创建新的进程，于是整个系统里的进程就形成了一个 **树状结构（process tree）**，例如：

![[topic3_5.png]]

我们使用 `pid` 来标识进程，使用 `ppid` 来标识它的父进程。

!!! tip "使用命令 `pstree` 可以查看进程树"

当父进程创建子进程时，操作系统需要决定资源是否共享，常见有三种策略：

|模式|说明|
|---|---|
|① 父与子共享所有资源|比如打开的文件、内存空间等都共享。|
|② 子只共享父进程资源的子集|例如，只继承打开的标准输入输出。|
|③ 父与子完全不共享资源|子进程有自己的独立资源，常用于隔离。|

不同系统或创建方式（如 Linux 的 `fork()` 或 `exec()`）会采用不同策略。

例如进程管理 API `fork()` 会完整复制一份状态机，包括内存、寄存器等。在执行 `fork()` 时，父进程中会返回子进程的 `pid`，子进程中则返回 0。

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    printf("A process starts!\n");

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        printf("Fork failed!\n");
    } else if (pid == 0) {
        // sleep(1);
        printf("pid is zero, so it's child process!\n");
    } else {
        // wait(NULL);
        // sleep(1);
        printf("pid is nonzero thus it's parent process!\n");
    }
}
```

上面的 C 代码中，父进程在 `fork()` 创建子进程后，两个进程将**并发执行（Execute Concurrently）**，互不阻塞；如果将 Line 18 的注释取消，那么父进程将等到子进程返回信号 `SIGCHILD` 才会继续运行。

!!! note "父子进程的终止关系"
	如果子进程结束时父进程已经终止，那么 UNIX 的托孤机制会将子进程的 `ppid` 设置为 1，即 `init` 进程；不过有些 OS 不允许子进程在父进程退出后仍然运行。

而 `exec()` 会将当前进程重置成一个可执行文件描述状态机的初始状态，新程序将会覆盖原有的内存空间。

一个典型的使用该 API 的流程如下，它先 `fork` 一个子进程，让子进程执行别的程序，而父进程只要等待子进程结束即可：

```c
pid_t pid = fork();      // 创建一个子进程
if (pid == 0) {
    // 子进程执行这里
    execlp("/bin/ls", "ls", NULL);   // 用 ls 程序替换当前子进程
} else {
    // 父进程执行这里
    wait(NULL);              // 等待子进程结束
}

```

!!! info "所谓的重置，其实只复位了用户可见的状态，OS 内部维护的进程号、目录、打开的文件等不变"

### Process Termination

当一个进程执行执行完后，它会调用 `exit()` 告知 OS 来清理资源，此时父进程可以通过 `wait()` 来获取返回值。

除此之外，父进程还可以使用 `abort()` 主动让子进程终止，常见原因有：

- 子进程 **超出了分配的资源**（比如用太多内存或时间）
- 子进程 **任务不再需要**（比如父进程发现它没必要继续）
- 或者 **父进程自己要退出**，于是顺带让子进程一并结束

!!! quote "实际上，通过 `fork()`, `exec()`, `exit()` 这三个 API 就足以实现大部分进程调用逻辑"

## Communication

### Interprocess

**Interprocess Communication（IPC）** 是让不同进程之间能够交换信息、协同工作的机制，它有两个基本模型：

- **Message Passing（消息传递）模型**
    - 各进程**不共享内存**，而是通过**发送(send)** 和 **接收(receive)** 消息来通信。
    - 这在**分布式系统**或**安全隔离**的环境中特别常见（例如两个独立的应用程序之间）。
    - 常见例子：Socket 通信、管道（pipe）、消息队列、RPC。
- **Shared Memory（共享内存）模型**
    - 进程间通过**共享一块内存区域**来通信。
    - 共享内存比消息传递快，因为不需要拷贝数据，但需要**同步机制**（如信号量）来防止数据冲突。

![[topic3_6.png]]



### Client-Server Systems

- Sockets
- Remote Procedure Calls
- Remote Method Invocation

