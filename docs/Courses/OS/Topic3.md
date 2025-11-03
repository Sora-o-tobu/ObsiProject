
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

两个进程之间的切换延迟称作 **Dispatch Latency**：

![[topic3_10.png]]

!!! quote "系统总是通过 PCB 对进程进行控制"

进程在其生命周期内，由于系统中各个进程的相互制约以及系统运行环境的变化，进程的状态也在不断发生变化。一个进程通常有以下五种状态：

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

需要注意的是，一个进程从 running 变为 waiting 是**主动**的行为；从 waiting 变为 ready 是**被动**的行为，需要其它相关进程的协助。

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
	如果子进程结束时父进程已经终止，此时该子进程被称为**孤儿进程**，那么 UNIX 的托孤机制会将子进程的 `ppid` 设置为 1，即 `init` 进程；不过有些 OS 不允许子进程在父进程退出后仍然运行。
	
	此外，如果子进程已经结束，但是父进程没有调用 `wait()` 接受子进程返回码，则此时该子进程被称为**僵尸进程**。

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

一个进程的创建原语为：

1. 为新进程分配一个唯一的进程标识号，并申请一个空白 PCB。若 PCB 申请失败，则创建失败。
2. 为进程分配其运行所需资源，包括内存、文件、I/O设备和 CPU 时间等。这些资源可以从操作系统或父进程中获得。
3. 初始化 PCB。
4. 若进程就绪队列有空位，则进入，等待被调度运行。

### Process Termination

当一个进程执行执行完后，它会调用 `exit()` 告知 OS 来清理资源，此时父进程可以通过 `wait()` 来获取返回值。

除此之外，父进程还可以使用 `abort()` 主动让子进程终止，常见原因有：

- 子进程 **超出了分配的资源**（比如用太多内存或时间）
- 子进程 **任务不再需要**（比如父进程发现它没必要继续）
- 或者 **父进程自己要退出**，于是顺带让子进程一并结束

!!! quote "实际上，通过 `fork()`, `exec()`, `exit()` 这三个 API 就足以实现大部分进程调用逻辑"

一个进程的终止原语为：

1. 根据被终止进程的标识符，检索出该进程的 PCB，从中读取该进程的状态
2. 若被终止进程处于运行状态，立即终止该进程的执行，将 CPU 资源分配给其它进程
3. 将该进程所拥有的全部资源归还给其父进程或操作系统
4. 删除该 PCB

## Communication

**Interprocess Communication（IPC）** 是让不同进程之间能够交换信息、协同工作的机制，它有两个基本模型：

- **Message Passing（消息传递）模型**
    - 各进程**不共享内存**，而是通过**发送(send)** 和 **接收(receive)** 消息来通信。
    - 这在**分布式系统**或**安全隔离**的环境中特别常见（例如两个独立的应用程序之间）。
    - 常见例子：Socket 通信、管道（pipe）、消息队列、RPC。
- **Shared Memory（共享内存）模型**
    - 进程间通过**共享一块内存区域**来通信。
    - 共享内存比消息传递快，因为不需要拷贝数据，但需要**同步机制**（如信号量）来防止数据冲突。
    - 一个进程内的线程之间是自然共享进程空间的，但是不同进程之间是独立的，需要通过特殊的系统调用实现

![[topic3_6.png]]

除此之外还有共享文件、管道（pipe）、Socket（适用于双方不在同一个计算机）等进程间通信方式。

## Threads

为了减小程序在并发执行时所付出的时空开销，提高操作系统的并发系统，我们引入**线程（Thread）**这一概念。

线程是进程中的一个实体，是被 OS 独立调度和分派的基本单位。一个线程可与同属一个进程中的其它线程共享该进程拥有的全部资源，同一进程内部的多个线程之间可以并发执行。

引入线程后，进程只作为除了 CPU 外的系统资源的分配单元，而线程则作为 CPU 的分配的分配单元。

??? quote "进程和线程的比较"
	- **调度：** 进程调度的上下文切换开销较大，而统一进程内的线程切换代价远低于进程
	- **并发性：** OS 引入线程后，不同进程之间、一个进程的多个线程之间、不同进程的线程之间都能够并发执行，使得操作系统拥有更好的并发性
	- **拥有资源：** 进程是系统中拥有资源的基本单位，而线程本身只有极少能保证独立运行的资源。但是同一进程内的所有线程都具有相同地址空间，即线程可以访问其隶属进程的系统资源
	- **独立性：** 每个进程都拥有独立的地址空间和资源，除了共享全局变量，不允许其它进程访问。
	- **系统开销：** 创建、撤销进程时系统都要为之分配或回收 PCB 及其它资源，并且进程切换设计开销较大的上下文切换；而线程切换只需要保存和设置少量寄存器内容（Lab 2内容）

线程是一个轻量型实体，它不拥有系统资源，但是每个线程都应该有一个唯一标识符和一个线程控制块，记录线程执行的寄存器和栈等状态。换句话讲，每个线程都拥有自己的 thread ID，PC，Register Set 和 Runtime Stack，而一个线程与同一进程下的其它线程共享 code section，data section，heap，open files 以及 signals。

与进程一样，各线程之间也存在共享资源和相互合作的制约关系，因此线程在运行时也具有 running，waiting，ready 三种基本状态。

在实现上，线程可分为 **User-Level Thread（ULT）** 和 **Kernel-Level Thread（KLT）** 两类：

- **User-Level Thread** 用户级线程中，有关线程管理的所有工作都在用户态完成，内核意识不到线程的存在。
	- 用户应用程序可以使用**线程库**设计成多线程程序，对于使用 ULT 的系统，其调度仍然以进程为单位
	- *Advantage 1:* 线程切换无需转换到内核态，节省开销
	- *Advantage 2:* 不同进程可根据自身需要设计不同的调度算法
	- *Advantage 3:* 实现与操作系统无关，线程管理代码属于用户程序的一部分
	- *Disadvantage 1:* 当一个线程执行系统调用时，进程内的所有线程都被阻塞
	- *Disadvantage 2:* 内核每次分配给一个进程只有一个 CPU，因此同一时刻只有一个线程能执行
- **Kernel-Level Thread** 内核级线程中，有关线程管理的所有工作都在内核态完成
	- 操作系统为每个内核级线程设置线程控制块 TCB，其调度以线程为单位
	- *Advantage 1:* 发挥多 CPU 优势，内核能同时调度同一进程中多个线程并行
	- *Advantage 2:* 若进程中某一线程阻塞，内核可以调度该进程中其它线程占用 CPU
	- *Disadvantage 1:* 线程切换需要转换到内核态，开销较大

!!! info "目前主要使用的线程库包括 POSIX Pthreads，Windows API，Java"

许多系统都使用 ULT 和 KLT 组合的方式实现多线程，根据二者连接方式的不同，形成如下三种不同的**多线程模型**。

<font style="font-weight: 1000;font-size: 20px" color="red">Many-to-One Model</font>

将多个用户级线程映射到一个内核级线程。操作系统为每个进程分配一个内核级线程，该进程中的所有用户级线程都连接到该内核级线程中。

线程的调度与管理在用户态完成，仅当 ULT 需要访问内核时，才将其映射到 KLT 上。同一时刻只有一个 ULT 可以映射 KLT，因此一个线程访问内核时发生阻塞，整个进程都会被阻塞，并且多个 ULT 不能同时在多个 CPU 上运行。

本质上，该模型和只实现 ULT 无区别。

<font style="font-weight: 1000;font-size: 20px" color="red">One-to-One Model</font>

将每个用户级线程映射到一个内核级线程。每个进程有与用户级进程数量相同的内核级线程，线程切换由内核完成，需要切换到内核态。

当一个线程阻塞后，允许调度另一个线程运行；但每创建一个用户线程，都需要相应创建一个内核线程，开销较大。

本质上，该模型和只实现 KLT 无区别。

<font style="font-weight: 1000;font-size: 20px" color="red">Many-to-Many Model</font>

将 n 个用户级线程映射到 m 个内核级线程，并且一定有 $n\ge m$。

既克服了多对一模型并发度不高的缺点，又克服了一对一模型一个进程占用太多内核级线程的缺点。


![[topic3_8.png]]


## CPU Scheduling

### Concept

**​多道 (multiprogramming)** 环境下，进程的个数通常大于 CPU 的个数。CPU 调度就是 OS 关于哪个 ready 进程可以运行（使用 CPU）以及运行多久的决定。

​其目标是始终允许某个进程运行以最大化 CPU 利用率，同时保证一定的公平性。这在多道环境下是必要的，关系到系统的整体效率。

> from [4 调度 - 咸鱼暄的代码空间](https://xuan-insr.github.io/%E6%A0%B8%E5%BF%83%E7%9F%A5%E8%AF%86/os/II_process_management/4_sched/)

![[topic3_7.png]]

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

!!! tip "Medium Term Scheduler"
	有些系统存在中期调度器，目的是提高内存利用率和系统吞吐量。为此，将那些暂时不能运行的进程调至外存等待，此时称为进程**挂起**。
	
	当它们已经具备运行条件且内存中有空闲时，中期调度器决定将其重新调入内存，并修改状态为 Ready，加入就绪队列。

CPU 调度可能出现在任意一个进程上，但是需要请求调度的事件发生后才能进行调度。进程调度的方式，可分为抢占式和非抢占式两种：

- **非抢占式，nonpreemptive** 即使有优先级更高的任务进入就绪队列，也不会打断当前 CPU 正在执行的进程，直到该进程运行结束或某些事件发生
	- 调度发生时机为 Running => Terminated 和 Running => Waiting
	- 优点是实现简单，系统开销小
- **抢占式，preemptive** 若有优先级更高的任务进入就绪队列，则允许调度程序根据某种原则去暂停当前正在执行的进程，将 CPU 分配给这个更重要的进程
	- 调度可以发生在任何时候
	- 抢占式调度提高了系统吞吐率和响应效率，但使得调度控制更复杂

### Scheduling Algorithm

不同的调度算法具有不同特性，为了比较性能，我们给出以下几个评价标准：

- Maximize **CPU Utilization** : CPU 使用率，即 CPU 非空闲的时间比例
- Maximize **Throughput** : 吞吐量，每个时间单元内完成的进程
- Minimize **Turnaround Time** : 周转时间，从进程创立到进程完成的时间，是等待进入内存、在 ready queue 中等待、在 CPU 上执行、I/O 执行等时间的总和
- Minimize **Waiting Time** : 等待时间，在 ready queue 中（或在 Ready 状态下）等待所花的时间之和
- Minimize **Response Time** : 响应时间，交互系统从进程创立到第一次产生响应的时间

这些要求有时甚至是冲突的。例如，较多的 context switch 会减少 throughput，因为 context switch 过程中并没有有用的工作；而较少的 context switch 会增加 response time。

基于上述不同目的，多种调度算法被设计出来。

<font style="font-weight: 1000;font-size: 20px" color="orange">First-Come First Serve</font>

FCFS 算法是**非抢占式**的一个最简单调度算法。

它使用 FIFO 队列来实现 Ready Queue，每次从就绪队列中选择最先进入该队列的进程，直到运行完成或某事件发生阻塞时才释放 CPU。

若一个长作业先到达系统，就会使后面的许多短作业等待很长事件，因此它不能作为分时系统和实时系统的主要调度策略。

<font style="font-weight: 1000;font-size: 20px" color="orange">Shortest-Job-First</font>

SJF 是对短进程优先调度的算法，它有抢占式和非抢占式的两种实现。

SJF 的核心思想是从 Ready Queue 中选择**估计下一次运行时间**最短的进程运行。对于非抢占式系统，「估计下一次运行时间」指该进程所需的总运行时间；对于抢占式系统，「估计下一次运行时间」指该进程的剩余运行时间。

- **\<nnonpreemptive\> Shortest-next-CPU-burst** – once CPU given to the process it cannot be preempted until completes its CPU burst
- **\<preemptive\> Shortest-remaining-time-first** – if a new process arrives with CPU burst length less than remaining time of current executing process, preempt.

在给定一个 Processes 集合，SJF 可以确保具有最佳的 Minimum Average Waiting Time。

!!! danger "SJF 算法的缺点"
	- 对长进程不利，产生 *Starvation* 现象
	- 完全未考虑进程的优先级
	- 「估计下一次运行时间」通过预测得到，并不完全准确

<font style="font-weight: 1000;font-size: 20px" color="orange">Round-Robin</font>

RR 算法是适用于分时系统的**抢占式**调度算法。

RR 算法定义了一个较小的固定时间单元**时间片（time slice / quantum）**，保证任何一个进程都不会连续运行超过一个时间片的时间（除非这是 Ready Queue 的唯一进程）。

它将所有就绪进程按照 FCFS 策略排成 Ready Queue，每隔一个时间片便产生一次时钟中断激活调度程序，将 CPU 分配给就绪队列的队首进程。被剥夺的进程回到就绪队列的末尾重新排队，等候再次运行。

相比 SJF 算法而言，RR 算法的平均等待时间更长，但是响应时间更短。

Quantum 的选择对 RR 算法的性能影响很大。如果时间片足够大，以至于所有进程都能在一个时间片内执行完毕，则 RR 算法退化为 FCFS 算法；如果时间片非常小，则 CPU 将在进程间频繁切换，真正用于运行用户进程的时间将减小。

​在实践中，时间片大约 10~100ms，每次 context-switch 约 10μs。即 context-switch 的时间花费是比较小的。

<font style="font-weight: 1000;font-size: 20px" color="orange">Priority Scheduling</font>

为每个进程设定优先级，每次从就绪队列中选择优先级最高的进程，它也实现为抢占式的和非抢占式的。

这里的优先级可以是静态的（如进程类型、对资源的要求、用户要求），也可以是动态的（如优先级随等待时间增加而提高）。

例如 SJF 算法就是以估算 CPU 运行时间为优先级的 Priority Scheduling，我们知道优先级低的进程可能长时间不会被执行，造成 *Starvation* 现象，解决方法就是设置动态优先级，使得优先级初值较低的进程在等待足够长时间后也可以获得 CPU 资源。

<font style="font-weight: 1000;font-size: 20px" color="orange">Multi-Level Queue</font>

多级队列调度算法在系统中设置多个 Ready Queue，将不同类型或性质的进程固定分配到不同的就绪队列，系统可根据不同用户进程的需求为每个队列选择不同调度策略。

例如前台队列可以使用 RR 调度保证 response，后台队列可以使用 FCFS。当然，不同队列之间也应该有调度，例如 <1> Fixed Priority Sheduling，仅优先级高的前台队列为空时才能够执行优先级低的后台队列；或 <2> Time Slice，两个队列都有一个确定的 CPU 时间，当时间到了则转向另一个队列，一般 80% 时间用于前台，20% 时间用于后台。

在多 CPU 系统中，可以很方便地为每个 CPU 设置单独的就绪队列。

<font style="font-weight: 1000;font-size: 20px" color="orange">Multi-Level Feedback Queue</font>

多级反馈队列调度算法融合前几种算法的优点，通过动态调整进程优先级和时间大小，可以兼顾多方面的系统目标。

**【Example】**
有三个 Ready Queue，分别采用 RR，RR，FCFS 算法，优先级依次降低。

一个进程 Ready 时先进入 $Q_0$，该进程后获得 CPU 有 8ms 时间完成任务，否则被打断进入 $Q_1$；只有当队列 $Q_0$ 中所有进程执行完时才会运行 $Q_1$，进程在 $Q_1$ 同理，如果没在 16ms 完成任务则进入 $Q_2$；最后 $Q_2$ 采取 FCFS 算法，只有 $Q_0,Q_1$ 均为空时才会被执行。

![[topic3_9.png]]


