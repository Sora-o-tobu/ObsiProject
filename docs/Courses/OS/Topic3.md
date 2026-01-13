
# Processes

## Concept

在 Batch System 中，我们称运行的程序为 *job*；在 Time-Sharing System 中，我们称运行的程序为 *task*。在教科书中，上面的两个概念几乎与**进程（Process）**等价。

进程是一段 Program 在操作系统中的执行实体，每个进程都有自己独特的内存空间，包括**用户部分**和**内核部分**。

对于*用户部分*，text 和 data 部分所需要的空间在一开始就被确定，而 heap 和 stack 都可以动态的扩展和收缩：

- **Text Section:** 代码区
- **Data Section:** 数据区
- **Heap Section:** 堆
- **Stack Section:** 栈

我们使用 **Process Control Block(PCB)** 来保存一个进程的*内核部分*，这些控制信息包括：

- **Process State:** 进程状态
- **Program Counter:** 程序计数器
- **CPU Registers:** 寄存器信息
- **CPU Scheduling Information:** 调度信息
- **Memory-management Information:** 内存管理信息
- **Accounting Information:** 动态数据的统计与记录信息
- **I/O Status Information:** I/O 状态信息

当 CPU 需要在不同进程之间切换时，它需要保存上一个进程的 PCB，并加载下一个进程的 PCB 信息：

![[topic3_2.png]]

两个进程之间的切换由分派器 Dispatcher 完成，切换延迟称作 **Dispatch Latency**：

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

上面的 C 代码中，父进程在 `fork()` 创建子进程后，两个进程将**并发执行（Execute Concurrently）**，互不阻塞，同时我们也无法确定哪一个进程先获得 CPU；但如果将 Line 18 的注释取消，那么父进程将等到子进程返回信号 `SIGCHILD` 才会继续运行。

!!! note "父子进程的终止关系"
	- **Orphan** 孤儿进程
		- 如果父进程先于子进程结束，那么此时还在运行的子进程就是孤儿进程。
		- 传统 UNIX 的托孤机制会将孤儿进程的 `ppid` 修改为 1（`init`），即保证子进程结束后有人负责回收资源
	- **Zombie** 僵尸进程
		- 如果子进程先于父进程结束，且父进程不调用 `wait` 获取子进程的状态信息，此时已经运行结束的子进程就是僵尸进程
		- 子进程退出时大部分资源都被回收，但是它在进程表中的 slot 保留（包括 `pid`, 退出状态码等信息），只有父进程调用 `wait` 获取该信息后才会删除
		- 很多父进程不会调用 `wait`，但 `init` 会周期性调用 `wait`，在父进程死后，托孤机制会将僵尸子进程移动到 `init` 下，然后通过 `wait` 收尸
	
	??? example "代码示例"
		```c
		// 孤儿进程演示
		if (fork() == 0) {
		    // 子进程
		    sleep(10); // 睡一会，确保父进程先死
		    // 此时醒来，我的 ppid 已经是 1 了
		} else {
		    // 父进程
		    exit(0); // 我先润了
		}
		
		// 僵尸进程演示
		if (fork() == 0) {
		    // 子进程
		    exit(0); // 我死了，变成 Z 状态
		} else {
		    // 父进程
		    while(1) {
		        // 我在忙别的，既不 wait() 也不退出
		        // 那个死去的子进程现在就是僵尸
		        sleep(1); 
		    }
		}
		```


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

!!! abstract "管道 pipe"
	- 管道是一个特殊的共享文件
	- 只要管道不满，写进程就可以写；只要管道非空，读进程就可以读
	- 管道只支持单向传输，即只能 A 写 B 读
		- 如果要实现双向，则需要两个管道

## Threads

为了减小程序在并发执行时所付出的时空开销，提高操作系统的并发系统，我们引入**线程（Thread）**这一概念。

线程是进程中的一个实体，是被 OS 独立调度和分派的基本单位。一个线程可与同属一个进程中的其它线程共享该进程拥有的全部资源，同一进程内部的多个线程之间可以并发执行。

引入线程后，进程只作为除了 CPU 外的系统资源的分配单元，而线程则作为 CPU 利用的分配单元。

??? quote "进程和线程的比较"
	- **调度：** 进程调度的上下文切换开销较大，而同一进程内的线程切换代价远低于进程
	- **并发性：** OS 引入线程后，不同进程之间、一个进程的多个线程之间、不同进程的线程之间都能够并发执行，使得操作系统拥有更好的并发性
	- **拥有资源：** 进程是系统中拥有资源的基本单位，而线程本身只有极少能保证独立运行的资源。但是同一进程内的所有线程都具有相同地址空间，即线程可以访问其隶属进程的系统资源
	- **独立性：** 每个进程都拥有独立的地址空间和资源，除了共享全局变量，不允许其它进程访问。
	- **系统开销：** 创建、撤销进程时系统都要为之分配或回收 PCB 及其它资源，并且进程切换涉及开销较大的上下文切换；而线程切换只需要保存和设置少量寄存器内容（Lab 2内容）

线程是一个轻量型实体，它不拥有系统资源，但是每个线程都应该有一个唯一标识符和一个线程控制块（TCB），记录线程执行的寄存器和栈等状态。

换句话讲，每个线程都拥有自己的 thread ID，PC，Register Set 和 Runtime Stack，而一个线程与同一进程下的其它线程共享 code section，data section，heap，open files 以及 signals。

![[topic3_14.png]]

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

!!! failure "本质上，该模型和只实现 ULT 无区别"

<font style="font-weight: 1000;font-size: 20px" color="red">One-to-One Model</font>

将每个用户级线程映射到一个内核级线程。每个进程有与用户级进程数量相同的内核级线程，线程切换由内核完成，需要切换到内核态。

当一个线程阻塞后，允许调度另一个线程运行；但每创建一个用户线程，都需要相应创建一个内核线程，开销较大。

!!! failure "本质上，该模型和只实现 KLT 无区别"

<font style="font-weight: 1000;font-size: 20px" color="red">Many-to-Many Model</font>

将 n 个用户级线程映射到 m 个内核级线程，并且一定有 $\text n\ge \text m$。

既克服了多对一模型并发度不高的缺点，又克服了一对一模型一个进程占用太多内核级线程的缺点。

!!! info "难以实现，且随着 CPU 核心数量增加，现代 OS 基本都采用 One-to-One Model"


![[topic3_8.png]]


!!! success "多线程模型中的 `fork()` 和 `exec()`"
	- 在一个多线程进程中，如果一个线程调用 `fork()`，是会复制所有线程还是只复制调用者呢？一些 UNIX 系统为两者都提供了对应版本的 `fork()`，具体使用哪种视开发者的目的而选择
	- 一个线程使用 `exec()` 系统调用，则整个进程都会被替换

## CPU Scheduling

### Concept

**​多道 (multiprogramming)** 环境下，进程的个数通常大于 CPU 的个数。CPU 调度就是 OS 关于哪个 ready 进程可以运行（使用 CPU）以及运行多久的决定。

​其目标是始终允许某个进程运行以最大化 CPU 利用率，同时保证一定的公平性。这在多道环境下是必要的，关系到系统的整体效率。

> from [4 调度 - 咸鱼暄的代码空间](https://xuan-insr.github.io/%E6%A0%B8%E5%BF%83%E7%9F%A5%E8%AF%86/os/II_process_management/4_sched/)

![[topic3_1.png]]

为了进行进程的调度，OS 维护了三条相关的 Scheduling Queues：

- **Job queue:** set of all processes in the system
- **Ready queue:** set of all processes in main memory, ready and waiting to execute
- **Device queues:** set of processes waiting for an I/O device
	- 也被称为 wait queues，等待队列

进程在 ready 状态时需要等待 CPU 资源的派发；在 waiting 状态时需要等待对应 I/O 完成或事件完成。我们通常使用链表作为数据结构对 Ready Queue 和 Wait Queue 进行维护：

![[topic3_3.png]]

实际上，不同进程会因为不同的事件而等待，因此 Wait Queues 实际上存在很多条；在结束 Waiting 后，进程就会进入 Ready Queue，因此实际情况可能如下：

![[topic3_4.png]]


**Scheduler** 是一个决定哪个程序该执行的程序，它在多进程系统中非常关键，可以分为 Long-term Scheduler 和 Short-term Scheduler。

- **长期调度器 (Long-term scheduler / Job scheduler)**
    - 决定**哪些作业/进程可以进入内存**，放进就绪队列 (ready queue)
    - 长期调度器控制系统中同时有多少个进程在竞争 CPU，如果太多则会造成频繁的切换，导致效率下降；如果太少则 CPU 可能空闲，浪费资源
    - 长期调度器通常会让 I/O-bound Process 和 CPU-bound Process 混合进入系统，防止 CPU 或 I/O 子系统长期空闲
- **短期调度器 (Short-term scheduler / CPU scheduler)**
    - 决定**下一个由 CPU 执行的进程**，负责从就绪队列中挑选
    - 发生频率很高（毫秒级），需要效率极高

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
	- 等于 CPU 工作时间 / 总时间
- Maximize **Throughput** : 吞吐量，每个时间单元内完成的进程数量
- Minimize **Turnaround Time** : 周转时间，从进程创立到进程完成的时间，是等待进入内存、在 ready queue 中等待、在 CPU 上执行、I/O 执行等时间的总和
	- 等于进程的完成时间减去到达时间
- Minimize **Waiting Time** : 等待时间，在 ready queue 中（或在 Ready 状态下）等待所花的时间之和
	- 等于进程的周转时间减去 CPU Burst Time 和 I/O Burst Time
- Minimize **Response Time** : 响应时间，交互系统从进程创立到第一次产生响应的时间
	- 等于进程第一次获得 CPU 的时间减去到达时间

基于上述不同目的，多种调度算法被设计出来。

<font style="font-weight: 1000;font-size: 20px" color="orange">First-Come First-Serve</font>

FCFS 算法是**非抢占式**的一个最简单调度算法。

它使用 FIFO 队列来实现 Ready Queue，每次从就绪队列中选择最先进入该队列的进程，直到运行完成或某事件发生阻塞时才释放 CPU。

若一个长作业先到达系统，就会使后面的许多短作业等待很长事件，因此它不能作为分时系统和实时系统的主要调度策略。

<font style="font-weight: 1000;font-size: 20px" color="orange">Shortest-Job-First</font>

SJF 是对短进程优先调度的算法，它有抢占式和非抢占式的两种实现。

SJF 的核心思想是从 Ready Queue 中选择**估计下一次运行时间**最短的进程运行。对于非抢占式系统，「估计下一次运行时间」指该进程所需的总运行时间；对于抢占式系统，「估计下一次运行时间」指该进程的剩余运行时间。

- **<nonpreemptive\> Shortest-next-CPU-burst** – once CPU given to the process it cannot be preempted until completes its CPU burst
- **<preemptive\> Shortest-remaining-time-first** – if a new process arrives with CPU burst length less than remaining time of current executing process, preempt.

在给定一个 Processes 集合，SJF 可以确保具有最佳的 Minimum Average Waiting Time。

!!! danger "SJF 算法的缺点"
	- 对长进程不利，产生 *Starvation* 现象
	- 完全未考虑进程的优先级
	- 「估计下一次运行时间」通过预测得到，并不完全准确

<font style="font-weight: 1000;font-size: 20px" color="orange">Round-Robin</font>

RR 算法是适用于分时系统的**抢占式**调度算法。

RR 算法定义了一个较小的固定时间单元**时间片（time slice / quantum）**，保证任何一个进程都不会连续运行超过一个时间片的时间（除非这是 Ready Queue 的唯一进程）。

!!! note "如果一个进程在时间片未用完前就结束，系统也会立即进行进程调度"

它将所有就绪进程按照 FCFS 策略排成 Ready Queue，每隔一个时间片便产生一次时钟中断激活调度程序，将 CPU 分配给就绪队列的队首进程。被剥夺的进程回到就绪队列的末尾重新排队，等候再次运行。

如果进程的剩余运行时间小于一个时间片，那么进程也会主动释放 CPU，随后调度程序将选择就绪队列的下一个进程。

相比 SJF 算法而言，RR 算法的平均等待时间更长，但是响应时间更短。

Quantum 的选择对 RR 算法的性能影响很大。如果时间片足够大，以至于所有进程都能在一个时间片内执行完毕，则 RR 算法退化为 FCFS 算法；如果时间片非常小，则 CPU 将在进程间频繁切换，真正用于运行用户进程的时间将减小。

​在实践中，时间片大约 10~100ms，每次 context-switch 约 10μs。即 context-switch 的时间花费是比较小的。

!!! question "频繁的上下文切换会导致 CPU 利用率低"
	![[topic3_13.png]]

!!! warning "如果一个进程时间片用完的同时有新进程到达，怎么处理 Ready Queue 顺序？"
	先将同一时刻到达的新进程依次加入队尾，再将时间片用完的进程放入 Ready Queue 的队尾（新进程优先）
	
	实际写题过程中也有可能反过来（旧进程优先），具体看题目要求。

<font style="font-weight: 1000;font-size: 20px" color="orange">Priority Scheduling</font>

为每个进程设定优先级，每次从就绪队列中选择优先级最高的进程，它也实现为抢占式的和非抢占式的。

这里的优先级可以是静态的（如进程类型、对资源的要求、用户要求），也可以是动态的（如优先级随等待时间增加而提高）。

例如 SJF 算法就是以估算 CPU 运行时间为优先级的 Priority Scheduling，我们知道优先级低的进程可能长时间不会被执行，造成 *Starvation* 现象，解决方法就是设置动态优先级，使得优先级初值较低的进程在等待足够长时间后也可以获得 CPU 资源。

<font style="font-weight: 1000;font-size: 20px" color="orange">Multi-Level Queue</font>

多级队列调度算法在系统中设置多个 Ready Queue，将不同类型或性质的进程固定分配到不同的就绪队列，系统可根据不同用户进程的需求为每个队列选择不同调度策略。

例如对于一个 B/S 程序，前台队列可以使用 RR 调度保证 response，后台队列可以使用 FCFS。当然，不同队列之间也应该有调度，例如：

- **方法一:** Fixed Priority Sheduling，仅优先级高的前台队列为空时才能够执行优先级低的后台队列
- **方法二:** Time Slice，两个队列都有一个确定的 CPU 时间，当时间到了则转向另一个队列，一般 80% 时间用于前台，20% 时间用于后台。

在多 CPU 系统中，可以很方便地为每个 CPU 设置单独的就绪队列。

<font style="font-weight: 1000;font-size: 20px" color="orange">Multi-Level Feedback Queue</font>

在多级队列调度算法中，一个进程被永久地分配到某个就绪队列中，这种设置缺乏灵活性。

多级反馈队列调度算法融合前几种算法的优点，通过动态调整进程优先级和时间大小，可以兼顾多方面的系统目标。

**【Example】**
有三个 Ready Queue，分别采用 RR，RR，FCFS 算法，优先级依次降低。

一个进程 Ready 时先进入 $Q_0$，该进程后获得 CPU 有 8ms 时间完成任务，否则被打断进入 $Q_1$；只有当队列 $Q_0$ 中所有进程执行完时才会运行 $Q_1$，进程在 $Q_1$ 同理，如果没在 16ms 完成任务则进入 $Q_2$；最后 $Q_2$ 采取 FCFS 算法，只有 $Q_0,Q_1$ 均为空时才会被执行。

![[topic3_9.png]]

!!! note
	通常，若某进程占用 CPU 时间较长，则更容易被移动至优先级更低的队列中；相对的，I/O Bound 或交互式的进程则更容易留在优先级更高的队列中


## Process Synchronization

### Critical Section

虽然多个进程可以共享系统中的各种资源，但其中许多资源一次只能为一个进程所用，我们将一次仅允许一个进程使用的资源成为 **Critical Resource**，包括打印机等物理设备，以及可被若干进程共享的变量、数据等。

对 Critical Resource 的访问必须互斥地进行，在每个进程中，访问 Critical Resource 的那段代码称为 **临界区 Critical Section**。一个进程在执行临界区代码时，其它进程都不能进入临界区。

我们将临界资源的访问过程分为四个部分：

```c
while (true) {
	entry section;      // 进入区 检查进程是否可以进入临界区
	critical section;   // 临界区 访问临界资源
	exit section;       // 退出区 清除正在访问临界区的标志
	remainder section;  // 剩余区 其它代码
}
```

### Mutual Exclusion (Mutex)

互斥也被称为间接制约关系，当一个进程进入 Critical Section 使用临界资源时，另一个进程必须等待。

实现互斥的基本方法包括软硬件方法、互斥锁、信号量等。同步与互斥应尽可能遵循以下原则：

- <1> **空闲让进 Progress:** 临界区空闲，则允许请求进入的进程立即进入
- <2> **忙则等待 Mutex:** 已有进程进入临界区时，其它试图进入的进程必须等待
- <3> **有限等待 Bounded Waiting:** 保证等待时间有限
- <4> **让权等待:** 当进程不能进入临界区时，应立即释放处理器，防止忙等待

!!! warning "其中前三个要求是必须实现的，第四个要求是尽可能实现"

<font style="font-weight: 1000;font-size: 20px" color="orange">软件实现方法</font>

```c
/// P0                       P1
while (true)                 while (true)
{                            {
	while (turn!=0);             while (turn!=1);    // 进入区
	critical section;            critical section;   // 临界区
	turn = 1;                    turn = 0;           // 退出区
	remainder section;           remainder section;  // 剩余区
}                            }
```

*单标志法*设置了一个公用整型变量 `turn`，表示允许进入临界区的进程编号。当 `turn=0` 时，表示允许进程 $P_0$ 进入临界区；当 `turn=1` 时，表示允许进程 $P_1$ 进入临界区。进程退出临界区时会将使用权赋予另一个进程。

但是该算法要求两个进程必须交替使用临界资源，如果进程 $P_0$ 离开外层循环，那么变量 `turn` 永远不会被设置为 1，此时 $P_1$ 无法再次进入临界区，违背了“空闲让进”的原则。

作为改进，我们有如下*双标志先检查法*：

```c
/// P0                       P1
while (true)                 while (true)
{                            {
	while (flag[1]);             while (flag[0]);    // 进入区
	flag[0] = true;              flag[1] = true;
	critical section;            critical section;   // 临界区
	flag[0] = false;             flag[1] = false;    // 退出区
	remainder section;           remainder section;  // 剩余区
}                            }
```

这样既不会发生上述的情况，也允许进程连续使用临界资源。但是因为对 `flag` 的检查和设置不是原子性的，可能存在以下情况：

进程 $P_0$ 和 $P_1$ 同时想要访问临界资源，在 entry section 对 `flag` 检查都通过，导致双方同时进入临界区，违背“忙则等待”原则。

此时若将*检查*放在*设置*后面（即交换代码 4,5 行），虽然不会出现双方同时进入的情况，但是会出现双方同时对自己的 `flag` 进行了设置，导致检查都不通过的**饥饿**现象。

**Peterson** 算法结合了以上算法的思想，利用 `flag[]` 解决互斥访问问题，利用 `turn` 解决饥饿问题，其核心思想是，若双方都想要进入临界区，则将进入的机会“谦让”给对方：

```c
/// P0                       P1
while (true)                 while (true)
{                            {
	flag[0] = true;              flag[1] = true;     // 进入区
	turn = 1;                    turn = 0;
	while (flag[1]&&turn==1);    while (flag[0]&&turn==0);
	critical section;            critical section;   // 临界区
	flag[0] = false;             flag[1] = false;    // 退出区
	remainder section;           remainder section;  // 剩余区
}                            }
```

!!! abstract "Peterson 算法很好地遵循了空闲让进、忙则等待、有限等待三个原则，但仍未实现让权等待"

<font style="font-weight: 1000;font-size: 20px" color="orange">硬件实现方法 1：中断屏蔽</font>

CPU 只会在发生中断时切换进程，因此屏蔽中断能够保证当前运行的进程让临界区的代码顺利执行完，其典型模式为：

```text
...
关中断
临界区
开中断
...
```

该方法的缺点也显而易见：

- <1> 限制了 CPU 交替执行程序的能力，系统效率明显降低
- <2> 将开关中断的权限交给用户不明智
- <3> 不适用于多处理器系统

<font style="font-weight: 1000;font-size: 20px" color="orange">硬件实现方法 2：TestAndSet</font>

TS 指令是一个*原子操作*，其功能是读出指定标志并将其设置为 `true`：

```c
bool TestAndSet(bool *lock) {
	bool old = *lock; // 存放 lock 旧值
	*lock = true;     // 无论之前是否加锁，都将 lock 设置为 true
	return old;       // 返回旧值
}
```

我们用 `true` 表示资源正被占用（加锁），用 `false` 表示空闲（未加锁）。进程在进入临界区之前，先用 TS 指令检查 lock 值：

- 若为 `false`，表示没有进程在临界区，可以进入，并将 lock 设置为 `true`
	- 此时临界资源被加锁，其它进程不能进入
- 若为 `true`，表示有进程在临界区，进入循环等待，直到当前访问临界区的进程退出时解锁

```c
while TestAndSet(&lock); // 加锁并检查
critical section;
lock = false;            // 解锁
remainder section;
```

!!! tip "TS 指令就是保证了检查和设置这两个操作是原子性的"

暂时无法进入临界区的进程会占用 CPU 循环执行 TS 指令，因此没有实现“让权等待”原则。

<font style="font-weight: 1000;font-size: 20px" color="orange">硬件实现方法 3：Swap</font>

Swap 指令也是一个*原子操作*，它的功能是交换两个字节的内容：

```c
void Swap(bool *a, bool *b) {
	bool temp = *a;
	*a = *b;
	*b = temp;
}
```

此时每个临界资源都有一个共享布尔变量 lock，初值为 false；每个进程都有一个局部布尔变量 key，初值为 true。

其原理和 TS 指令实现互斥区别不大，过程描述如下：

```c
bool key = true;
while (key != false) Swap(&lock, &key);
critical section;
lock = false;
remainder section;
```

!!! quote "硬件指令方法实现互斥的优缺点"
	- **Advantage**
		- 简单、容易验证正确性
		- 适用于任意数量的进程，支持多处理器
		- 支持系统中有多个临界区
	- **Disadvantage**
		- 等待进入临界区的进程会占用 CPU 执行 while 循环，没能实现“让权等待”
		- 从等待进程中随机选择一个进程进入临界区，有的进程可能一直选不上，从而导致饥饿现象（有限等待原则）

<font style="font-weight: 1000;font-size: 20px" color="orange">互斥锁 Mutex Lock</font>

互斥锁是解决临界区最简单的软件工具。一个进程在进入临界区时要调用 `acquire()` 来获得锁；在退出临界区时要调用 `release()` 来释放锁。

每个互斥锁有一个布尔变量 `available` 表示锁是否可用。只有锁可用才能成功调用 `acquire()`；当一个进程试图获取不可用的锁时会被阻塞，直到该锁被释放。其过程描述如下：

```c
acquire() {
	while (!available);  // 忙等待
	available = false;   // 获得锁
}
release() {
	available = true;    // 释放锁
}
```

!!! note "`acquire()` 和 `release()` 两个操作自然也是原子操作，因此通常用硬件机制实现"

上面描述的互斥锁也称**自旋锁 Spin Lock**，它的缺点是需要忙等待，当有一个进程在临界区时，任何其它进程在进入临界区前都需要连续循环调用 `acquire()`，这种连续循环显然浪费了 CPU 周期。因此，互斥锁通常用于多处理器系统，一个线程在一个处理器上旋转，而不影响其它线程的执行。

自旋锁的优点是进程在等待锁期间没有发生上下文切换，并且等待的代价不高。

!!! success "不需要忙等待的互斥锁称为 Sleep Lock"

<font style="font-weight: 1000;font-size: 20px" color="orange">信号量 Semaphore</font>

信号量是用来解决互斥和同步问题的功能更强的机制，它只能被两个标准原语 `wait()` 和 `signal()` 访问，也可简写为 `P()` P 操作和 `V()` V 操作。

**整型信号量**被定义为一个用于表示资源数量的整型量 `S`，相比于普通整型变量，整型信号量只有三种操作：初始化、P 操作、V 操作：

```c
wait(S) {            // 相当于进入区
	while (S <= 0);  // 若资源数不够，忙等待
	S = S - 1;       // 若资源数够，则占用一个资源
}
signal(S) {          // 相当于退出区
	S = S + 1;       // 释放一个资源
}
```

整型信号量的使用方式基本与互斥锁相似，因此它也有忙等待的缺点。而**记录型信号量**是一种不存在忙等现象的进程同步机制，它除了需要一个用于表示资源数量的整型变量 `value` 外，还增加了一个进程链表 `L`，用于链接所有等待该资源的进程：

```c
typedef struct {
	int value;
	struct process *L;
} semaphore;
```

此时对应的 P 操作和 V 操作描述如下：

```c
void wait(semaphore S) {
	S.value--;
	if (S.value < 0) {
		add this process to S.L;
		block(S.L);   // running - > waiting
	}
}

void signal(semaphore S) {
	S.value++;
	if (S.value <= 0) {
		// 仍有进程在等待该资源
		remove a process P from S.L;
		wakeup(P);   // waiting -> ready
	}
}
```

此时，为了使多个进程能够互斥地访问某个临界资源，需要为该资源设置一个互斥信号量 `S`，其初值为 1，对应可用资源数为 1，然后将各个进程访问该资源的临界区过程置于 `P(S)` 和 `V(S)` 之间：

```c
semaphore S = 1;
P1() {
	...
	P(S);
	critical section;
	V(S);
	...
}
```

### Classical Synchronization Problems

<font style="font-weight: 1000;font-size: 20px" color="orange">Bounded-Buffer Problem</font>

也称为*生产者-消费者问题*。有一个生产者，一个消费者，共享 Buffer 大小为 $N$；生产者每生产一个产品并放入缓冲区，消费者每次都缓冲区中取出一个产品并消费，因此缓冲区即为临界资源。

我们对该问题设置三个信号量：

- `mutex`: 初始化为 1，用于控制互斥访问 Buffer
- `full`: 初始化为 0，用于记录当前 Buffer “满”缓冲区数
- `empty`: 初始化为 $N$，用于记录当前 Buffer “空”缓冲区数

```c
semaphore mutex = 1;  // 临界区互斥信号量
semaphore empty = n;  // 空闲缓冲区
semaphore full  = 0;  // 缓冲区初始化为空

producer() {
	while (1) {
		produce a item;
		wait(empty);   // 获取空缓冲区单元
		wait(mutex);   // 进入临界区
		add the item to the buffer;
		signal(mutex); // 退出临界区
		signal(full);  // 满缓冲区数 +1
	}
}

consumer() {
	while (1) {
		wait(full);    // 获取满缓冲区单元
		wait(mutex);   // 进入临界区
		remove an item from buffer;
		signal(mutex); // 退出临界区
		signal(empty); // 空缓冲区数 +1
		consume the removed item;
	}
}
```


**【Exercise】** 三个进程 P1，P2，P3 互斥使用一个包含 $N$ 个单元的缓冲区，请设计信号量机制：

- P1 每次用 `produce()` 生成一个正整数并用 `put()` 送入缓冲区某个空单元中
- P2 每次用 `getodd()` 从该缓冲区中取出一个奇数并用 `countodd()` 统计奇数个数
- P3 每次用 `geteven()` 从该缓冲区中取出一个偶数并用 `counteven()` 统计偶数个数

??? success "Answer"
	```c
	semaphore mutex = 1;  // 临界区互斥信号量
	semaphore empty = n;  // 空闲缓冲区
	semaphore odd   = 0;  // 缓冲区初始化为空
	semaphore even  = 0;
	// Process 1
	P1() {
		while (1) {
			produce a item;
			wait(empty);   // 获取空缓冲区单元
			wait(mutex);   // 进入临界区
			add the item to the buffer;
			signal(mutex); // 退出临界区
			if the item is odd:
				signal(odd);
			else:
				signal(even);
		}
	}
	// Process 2
	P2() {
		while (1) {
			wait(odd);    // 获取满缓冲区单元
			wait(mutex);   // 进入临界区
			remove an item from buffer;
			signal(mutex); // 退出临界区
			signal(empty); // 空缓冲区数 +1
			consume the removed item;
		}
	}
	// Process 3
	P3() {
		while (1) {
			wait(even);    // 获取满缓冲区单元
			wait(mutex);   // 进入临界区
			remove an item from buffer;
			signal(mutex); // 退出临界区
			signal(empty); // 空缓冲区数 +1
			consume the removed item;
		}
	}
	```


<font style="font-weight: 1000;font-size: 20px" color="orange">Readers & Writer Problem</font>

有读者和写者两组并发进程，共享一个文件，当两个或以上的读进程同时访问数据时不会产生副作用，但若某个写进程和其它进程同时访问共享数据时则可能导致数据一致性问题，因此要求：

- 允许多个 Reader 同时对文件执行读操作
- 只允许一个 Writer 往文件中写信息
- 任意一个 Writer 在完成写操作之前不允许其它 Reader 或 Writer 工作
- Writer 执行写操作前应让已有的进程全部退出

简单的一个 P 操作、V 操作无法解决该问题，需要额外用到一个计数器来判断当前是否有 Reader 在读取文件。同时，不同 Reader 对计数器的访问也应该是互斥的，因此信号量设置为：

- `count`: 初始化为 0，用于记录当前 Reader 数量
- `mutex`: 初始化为 1，用于保护更新 count 变量时的互斥
- `rw`: 初始化为 1，用于保证 Reader 和 Writer 的互斥访问

```c
int count = 0;   // 整型信号量
semaphore mutex = 1;
semaphore rw    = 1;

writer() {
	while (1) {
		P(rw);          // 互斥访问共享文件
		write sth...
		V(rw);          // 释放共享文件
	}
}

reader() {
	while (1) {
		P(mutex);       // 互斥访问 count 变量
		if (count == 0) // 当第一个 Reader 读共享文件时
			P(rw);      // 阻止 Writer 访问共享文件
		count++;        
		V(mutex);       // 释放 count
		read sth...
		P(mutex)        // 互斥访问 count 变量
		count--;
		if (count == 0) // 当最后一个 Reader 离开
			V(rw);      // 允许写进程写
		V(mutex);       // 释放 count
	}
}
```

!!! abstract "该算法中 Reader 优先级大于 Writer，也存在 Writer 优先级更高的实现方式"

<font style="font-weight: 1000;font-size: 20px" color="orange">Dining-Philosophers Problem</font>

一张圆桌围坐了 5 名哲学家，每名哲学家前有一碗饭，每两名哲学家之间有一根筷子。当哲学家饥饿时，会尝试拿起左、右两根筷子（允许一根一根拿起），若筷子已在他人手上，则需要等待。

很显然，这里的五根筷子属于临界资源，我们为其定义信号量数组 `chostick[5] = {1,1,1,1,1}`。并且，我们对哲学家 $i$ 左边的筷子编号为 $i$，右边的筷子编号为 $(i+1)\%5$。

```c
semaphore chopstick[5] = {1,1,1,1,1};

// Process i
Pi() {
	do {
		P(chopstick[i]);       // 取左边筷子
		P(chopstick[(i+1)%5]); // 取右边筷子
		Dining...
		V(chopstick[i]);       // 放回左边筷子
		V(chopstick[(i+1)%5]); // 放回右边筷子
	} while(1);
}
```

上述贪心算法虽然浅显易懂，但是当所有哲学家同时想要进食，并且同时取到左边筷子时，此时想要取右边筷子就会进程阻塞，发生*死锁*。

!!! success "为防止死锁发生，可制定的一些限制条件"
	- <1> 至多允许 4 名哲学家同时进食，以保证至少有一名哲学家能拿到左右两边的筷子
	- <2> 要求奇数号哲学家先拿左边筷子，偶数号哲学家先拿右边筷子，以保证相邻哲学家都想进食时，只有一名哲学家可以取到筷子，而另一名阻塞等待
	- <3> 仅当哲学家两边筷子都可用时，才允许他拿起筷子

这里我们采用方案 3，仅需定义一个访问 `chopstick[]` 资源的互斥信号量 `mutex` 即可：

```c
semaphore chopstick[5] = {1,1,1,1,1};
semaphore mutex = 1;           // 临界区互斥信号量

// Process i
Pi() {
	do {
		P(mutex);
		P(chopstick[i]);       // 取左边筷子
		P(chopstick[(i+1)%5]); // 取右边筷子
		V(mutex);
		Dining...
		V(chopstick[i]);       // 放回左边筷子
		V(chopstick[(i+1)%5]); // 放回右边筷子
	} while(1);
}
```


## Deadlock

**死锁**指多个进程因竞争资源而造成的一种僵局，各个进程互相等待对方手里的资源而阻塞。

死锁和饥饿都是进程无法顺利向前推进的现象，但是他们有如下差别：

- <1> 发生饥饿的进程可以只有一个；而死锁是因循环等待对方手里的资源导致的，因此发生死锁的进程必然大于等于两个
- <2> 发生饥饿的进程可能处于 Ready 状态（长期得不到 CPU），也可能处于 Waiting 状态（长期得不到 I/O 设备）；而发生死锁的进程只能处于 Waiting 状态

产生死锁必须同时满足以下四个条件：

- **<1> 互斥条件 Mutual Exclusion:** 进程要求对所分配的资源进行排他性使用，即一段时间内某资源只能为一个进程占用，此时其它请求该资源的进程只能等待
- **<2> 不可剥夺条件 No Preemption:** 进程所获得资源使用完前，不能被其它进程夺走，只能主动释放
	- 只有对不可剥夺资源（如打印机）的竞争才会产生死锁，对可剥夺资源（如 CPU 和 Memory）竞争不产生死锁
- **<3> 请求并保持条件 Hold And Wait:** 进程已经保持了至少一个资源，但又提出了新的资源请求，而该资源已被其它进程占用，此时请求进程被阻塞，但自己已获得的资源保持不变
- **<4> 循环等待条件 Circular Wait:** 存在资源循环等待链，链中每个进程已获得的资源同时被链中下一个进程所请求
	- 即存在一个处于等待态的进程集合 $\{P_1, P_2, ..., P_n\}$，其中 $P_i$ 等待的资源被 $P_{i+1}$ 所占有，$P_n$ 等待的资源被 $P_0$ 占有

![[topic3_11.png]]

对于右图，假设系统中共有两个该类资源，并且其中一个资源被 $P_K$ 占用，而另一个资源处于循环等待圈中（例如被 $P_0$）占有。那么虽然满足了上述所说的条件四，但是如果此时 $P_K$ 将资源释放，$P_n$ 就能获得该资源，循环等待就会被打破。

因此，我们说循环等待只是死锁的**必要条件**；但若系统中每类资源都只有一个资源，则资源分配图中含圈就变成了系统出现死锁的**充分必要条件**。

为使系统不发生死锁，必须设法破坏产生死锁的四个条件之一；或者允许死锁的发生，但当死锁发生时能够检测到，并且有能力恢复。

|          | 实现方案                              | 资源分配策略             | 主要优点            | 主要缺点                               |
| -------- | --------------------------------- | ------------------ | --------------- | ---------------------------------- |
| **死锁预防** | 设置限制条件，破环产生死锁的 4 个必要条件            | 保守，宁可资源闲置          | 适用于突发式处理，不必进行剥夺 | 效率低，初始化时间长；<br>剥夺次数多；<br>不便灵活申请新资源 |
| **死锁避免** | 资源动态分配过程中，寻找可能的安全允许顺序，防止系统进入不安全状态 | 预防和检测的折中，运行时判断是否死锁 | 不必进行剥夺          | 必须知道将来的资源需求；<br>进程不能被长时间阻塞         |
| **死锁检测** | 允许死锁，定期检查死锁是否已经发生，通过剥夺等措施解除死锁     | 宽松，只要允许就分配资源       | 不延长进程初始化时间      | 通过剥夺解除死锁，造成损失                      |

### Deadlock Prevention

死锁预防需要破坏死锁产生的 4 个必要条件之一。

- <1> **破坏互斥条件**
	- 若将只能互斥使用的资源改造为允许共享使用，则系统不会进入死锁状态
	- 但很多临界资源只能互斥使用，因此该方法不太可行
- <2> **破坏不可剥夺条件**
	- 当一个已经保持了某些不可剥夺资源的进程，请求新的资源而得不到满足时，它必须释放已经保持的所有资源
	- 这意味着，进程已占有的资源会被暂时释放，或者称被剥夺了
	- 该方法实现复杂，且可能会导致前一阶段工作失效
- <3> **破坏请求并保持条件**
	- 要求进程在请求资源时不能持有不可剥夺资源，具体有以下两种实现方式
		- a. 要求进程在能够获取所需的所有资源前不能运行。进程运行期间不会再申请资源，从而破坏“请求”；在等待获取期间，进程不占有任何资源，从而破坏“保持”
		- b. 允许进程获得运行所需的部分资源后就运行，运行过程中逐步释放，只有全部释放后才能请求新的资源
	- 方法一实现简单，但会造成资源浪费，也会导致饥饿现象；相对来讲，方法二更加优秀
- <4> **破坏循环等待条件**
	- 采用*顺序资源分配法*，给系统中各类资源编号，只允许进程按照编号递增的顺序请求资源
	- 一个进程只有在已经保持小编号资源时，才能申请更大编号资源，因此已持有大编号资源的进程不能再逆向申请小编号资源，因此不会产生循环等待条件
	- 缺点在于，编号时只能考虑大多数资源使用这些资源的顺序，实际使用顺序可能与编号不一致，从而导致编程困难和资源浪费


### Deadlock Avoidance

死锁避免同样属于事先预防策略。对于每次资源申请操作，只有在不产生死锁的情况下，系统才会为其分配资源。

当系统能够按照某种顺序为每个进程分配其所需的资源，并且不会导致死锁，此时该顺序就被称为*安全序列*（可能有多个）。若系统能找到这么一个安全序列，则此时为 Safe State；若不能，则称系统处于 Unsafe State。

只有当系统处于 Unsafe State 时，才有可能出现死锁，因此我们需要保证系统不会进入 Unsafe State。

**银行家算法**是最著名的死锁避免算法。该算法将操作系统视为银行家，管理的资源视为资金，进程请求资源相当于向银行家贷款。进程运行前需要声明对各种资源的最大需求量，其数量不超过系统的资源总量。

假定我们有 $n$ 个进程，$m$ 类资源，我们定于如下数据结构：

- 可利用向量资源 `Available[m]`: 含有 $m$ 个元素的数组，`Available[j]=K` 表示此时系统中有 $K$ 个 $R_j$ 类资源可用
- 最大需求矩阵 `Max[n][m]`: 定义系统中 $n$ 个进程中每个进程对 $m$ 类资源的最大需求，`Max[i][j]=K` 表示进程 $P_i$ 需要 $R_j$ 类资源的最大数量为 $K$
- 分配矩阵 `Allocation[n][m]`: 定义系统中每类资源当前已分配的资源数，`Allocation[i][j]=K` 表示进程 $P_i$ 已获得 $R_j$ 类资源的数量为 $K$
- 需求矩阵 `Need[n][m]`: 表示每个进程接下来最多还需要多少个资源，`Need[i][j]=K` 表示进程 $P_i$ 还需要 $R_j$ 类资源的数量为 $K$

!!! tip "通常矩阵 `Need` 不是已知的，而是在解题过程中计算得到: `Need[i,j]=Max[i,j]-Allocation[i,j]`"

假定我们将所有进程的请求向量存储在矩阵 `Request[n][m]` 中，那么银行家算法的伪代码可以展示为：

```c
// 当进程 P_i 发出资源请求 Request[i]：
if Request[i] > Need[i]:
	error;               // 所需资源超出宣布的最大值
if Request[i] > Available:
	P_i Wait;            // 尚无足够资源，等待

// 全部条件满足，试探性分配资源并更新数据结构
Available        -= Request[i];
Allocation[i]    += Request[i];
Need[i]          -= Request[i];

// 执行 Safety Algorithm，若通过才正式分配资源给 P_i
if Safety():
	// 分配后为 Safe State，正式分配
	grant request;
else:
	// 回滚
    Available     += Request[i]
    Allocation[i] -= Request[i]
    Need[i]       += Request[i]
    P_i Wait;
```

其中使用的安全性算法伪代码可以理解为：

```c
bool Safety() {
	Work = Available;
	Finish[n] = {0};   // 初始化为 0
	while exists p such that
			Finish[p] == 0           // 该进程还未运行结束
			&& Need[p] <= Work:      // 该进程所需资源小于可用资源
		Work = Work + Allocation[p]; // 假定该进程释放资源
		Finish[p] = 1;               // 假定该进程运行结束
	
	if all(Finish == 1):
		return true;   // Safe State
	else:
		return false;  // Unsafe State
}
```

实际上，各个进程进入向量 `Finish` 的顺序即为一个可行的**安全序列**，若运行结束后安全序列中包含了所有进程（即所有均为 1），此时系统就处于安全状态；否则为不安全状态。

!!! warning
	- 银行家算法虽然理论上很完美，但现实操作系统几乎不采用（包括 Windows、Linux、Unix）
		- a. 它要求进程运行前必须声明所需资源最大数量，这在实际中很难预测
		- b. 算法开销较大
		- c. 资源的种类和数量在现代系统中是动态变化的
	- 因此，现代操作系统通常采用死锁忽略（即假设死锁不会发生）或简单的死锁预防，极少采用复杂的死锁避免机制

### Deadlock Detection

在*资源分配图*中，我们用圆圈表示一个进程，用框表示一类资源，框内每个圆表示该类资源中的一个资源。从进程到资源的有向边称为*请求边*，表示该进程申请一个单位的该类资源；从资源到进程的有向边称为*分配边*，表示该类资源已有一个资源分配给了该进程。

![[topic3_12.png]]

例如上图(a)中，进程 $P_1$ 已分得两个资源 $R_1$，并正在请求一个资源 $R_2$；进程 $P_2$ 已分得一个资源 $R_1$ 和一个资源 $R_2$，并正在请求一个资源 $R_1$。

我们可以通过“简化”资源分配图来检测当前系统状态 $S$ 是否为死锁状态，$S$ 为死锁的条件是当且仅当资源分配图是不可完全简化的。简化的具体方法如下：

- <1> **找到所有既不阻塞也不孤立的进程释放，消去该进程的所有有向边**
	- 即找到一个存在有向边与它相连的进程，且该有向边对应的资源的*申请数量*小于该资源的*空闲数量*；若所有连接该进程的边都满足该条件，则说明这个进程可以运行完成，消去它的所有请求边和分配边
	- 空闲数量即该资源的数量减去它在资源分配图中的出度，例如上图资源 $R_1$ 空闲数量为 $3-3=0$，而资源 $R_2$ 的空闲数量为 $2-1=1$
- <2> **第一步中释放的资源可能可以唤醒某些因等待这些资源而被阻塞的进程，重复步骤一进行简化**
	- 如果能消去图中所有的边，则称该图是可完全简化的

一旦检测出死锁，就可以通过某些措施来解除死锁。此处简略讲述几种措施：

- **<1> 资源剥夺法:** 挂起某些死锁进程，并抢占它的资源，将这些资源分配给其它死锁进程
	- 简化后的资源分配图中，还有边相连的那些进程就是死锁进程
- **<2> 撤销进程法:** 强制撤销部分甚至全部死锁进程，并剥夺这些进程的资源
	- 该方法实现简单，但可能会付出较大的代价
- **<3> 进程回退法:** 让一个或多个死锁进程回到足以回避死锁的状态，进程回退时资源释放资源
	- 该方法需要系统保持进程的历史信息，并设置还原点
	- 我们在数据库大作业中就使用了该方法

