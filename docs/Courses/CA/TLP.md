
# Thread-Level Parallelism

## Multi-Processor

单处理器性能有限，现代U基本都为多处理器，它属于 **MIMD** 架构，每个处理器 Fetch 自己的指令，对自己的数据流进行操作。

多处理器根据 Memory Model 的不同可分为 **UMA** 和 **NUMA** 两种：

![[MultiProcessorMemoryModel.png]]

其最主要的区别在于 Processor 和 Memory 之间的位置关系。对于 UMA，一个处理器访问任何 Memory 都是等延迟的，但是随着系统的增大，其延迟显著增大；对于 NUMA，一个处理器访问离它最近的 Memory 最快，访问别的 Memory 需要经过 Interconnect 中转，这一架构更适用于 *Large Multiprocessor Systems*，支持更多的 Cores。

!!! danger "注意是到 Memory 的 Latency，不是到 Cache。因此上述两种模型对应下面两种多核架构"
	=== "Symmetric Shared-Memory Multiprocessor, **SMP**"
		![[SMPCA.png]]
	=== "Distributed Shared-Memory Multiprocessor, **DSP**"
		![[DMPCA.png]]

多处理器核与核之间的编程模型可以分为四种：

- **<1> Multiprogramming:** 无交流
- **<2> Shared Address Space:** 通过内存共享
- **<3> Message Passing:** 通过消息传递
- **<4> Data Parallel:** 在不同数据上同时处理，并全局实时交换信息

处理器之间的通信存在一定延迟，在硬件上，可以通过使用**缓存**共享数据，这需要解决缓存一致性的问题。而在软件上，则使用**同步 Synchronization**。

!!! example "High Communication Cost"
	=== "题面"
		How much faster if no communication vs if 0.2\% remote ref?
		
		- App running on a 32-processor MP;
		- 100 ns for reference to a remote mem;
		- clock rate 4.0 GHz;
		- base CPI 0.5;
	=== "解答"
		$$\begin{array}l CPI &  = Base\ CPI + Remote\ Rate \times Remote\ Cost \\ & =  0.5 + 0.2\% \times \frac{100ns}{0.25ns} = 1.3 \\ Speedup & = \frac{1.3}{0.5} = 2.6 \end{array}$$

### Cache Coherence

!!! warning "缓存一致性是第五章最重要的内容"

在 Shared Memory 的情况下，对于同一块内存，可能有不同的处理器在进行读写操作。如果每个处理器都有自己的缓存，当一个处理器修改了内存中的数据，其它处理器的缓存中可能仍然保持着旧数据。我们需要一种方法能够“通知”其它处理器这一修改，主要有以下两种策略：

- 监听策略 snooping，**适用于 SMP**
- 目录策略 directory-based，**适用于 DSP，但也可以用在组织成 Bank 的 SMP 中**

<font style="font-weight: 1000;font-size: 20px" color="orange">Snooping 协议</font>

Snooping 协议允许处理器之间进行通信。当 P1 写入 X，需要通知其它处理器，其它处理器监听到这一通知时，如果自己的 Cache 中也有 X 的副本，可选以下两种不同的策略进行处理：

- **Write Invalidate:** 将 X 的副本设置为无效
	- 即设置缓存块的 Valid 位为 0
- **Write Broadcast:** 更新 X 的副本

!!! info "通过 BUS 实现"
	即每块缓存都能接收来自自己的 Processor 的信号以及来自 BUS 的信号。

在 **Simple Write-Invalidate Protocol(MSI Protocol)** 中，每个缓存块设置了三种状态：

- **Invalid:** 无效状态
	- 即 Valid 位为 0
- **Shared:** 共享状态
	- Valid = 1, Dirty = 0
- **Modified(Exlusive):** 独占状态
	- Valid = 1, Dirty = 1

=== "From CPU"
	![[MPfromCPU.png]]
=== "From Bus"
	![[MPfromBus.png]]

!!! note
	- Cache 本身采取 Write-Back 策略，对于自身 CPU 的请求，只要没有写请求则一直处于 Shared 状态（和其它处理器共享相同数据），如果要写缓存，则进入独占状态，此时如果别的处理器想要读或写该内存，都需要将其写回
	- Cache 本来在独占状态，如果接收到 Bus 的 Write Miss 请求(其它处理器尝试写入同一内存)，则将脏块写回且进入 Invalid 状态（因为采用 Write-Invalidate Protocol）

![[SnoopingProtocolEx1.png]]

考试常考的则是其拓展协议 **MESI Protocol**，其增加一个状态 *Exclusive*：

- **Invalid:** 无效状态
	- Valid = 0
- **Shared:** 共享状态
	- Valid = 1, Dirty = 0，且该缓存行被大于等于 2 个处理器缓存
	- 共享状态下处理器想要写该缓存行，则其它缓存副本必须进入 Invalid 状态
- **Exclusive:** 独占状态
	- Valid = 1, Dirty = 0，且该缓存行只被该处理器缓存
	- 独占状态下处理器可以直接修改该缓存行而不需要通知其它处理器
- **Modified:** 修改状态
	- Valid = 1, Dirty = 1

![[MESIProtocolOverview.png]]

Coherent Miss 中，根据是否源于处理器之间的数据共享而划分为两种：

- **True Sharing Miss** 不同处理器对同一个 Cache Block 的*同一个变量*访问造成的 miss
	- Case 1：对一个 Shared Cache Block 进行初次写操作，此时其它缓存中该块被 Invalidate，算作一次 coherence miss
	- Case 2：尝试读取另一个处理器的 Modified Word，由于该处理器本身 Cache 中该块肯定是 Invalid 的，因此也是 coherence miss 
- **False Sharing Miss** 没有共享数据，但被缓存协议误判为共享
	- 不同处理器访问了同一个 Cache Block 中的不同变量，但是这些变量共享了缓存块，导致写入一个变量使得整个 Block 被 Invalidate，从而干扰其它处理器访问其它变量，造成 miss

<font style="font-weight: 1000;font-size: 20px" color="orange">目录协议</font>

![[DirectoryProtocolOverview2.png]]

每个处理器都有自己的 Cache，为了保证数据一致性，我们通过一个分布式目录结构 Directory 记录每个内存块当前的缓存状态以及被哪些处理器缓存了。

!!! info "因此不需要像 Snooping 协议一样广播数据变更，从而节省带宽，更适合大规模系统"
	不是广播，而是目录与请求节点、与一个或多个远程节点之间选择性地进行通信。

为每一块内存记录 **directory**，目录的内容为：

- 该块的状态
	- Shared：有节点缓存了该块，且值为最新
	- Uncached：没有节点拥有该块的 Copy
	- Modified(Exclusive)：有且只有一个节点拥有该块的 Copy，且已被 Write
- 哪些处理器有这个块的副本
	- 用 bit vector 表示
- Dirty/Clean

!!! note "注意 Directory 是对内存而不是对缓存的哦"

![[DirectoryProtocolOverview.png]]

对一块内存操作，涉及三个处理器的概念：

- **Local Node:** 发起请求的处理器
- **Home Node:** 对应内存地址所在的处理器
	- Home Node 负责维护该内存地址对应的数据项的状态，例如 `X:S, {P1,P2}` 表示数据项 X 为 Shared 状态，Remote Node 为 P1, P2
- **Remote Node:** 拥有这个副本的处理器

=== "From CPU"
	![[DirectoryFromCPU.png]]
=== "From Cache"
	![[DirectoryFromDirectory.png]]
	
	!!! note
		- 某 Cache 独占一个地址块，但是当这个块因为 Cache 空间不够或者其它原因被 victim，需要 Write-Back 时，会向该块所对应的目录位置发送“数据写回”消息，并转为 Uncached 状态

### Synchronization

!!! bug "没有啦"

!!! quote "对于大型多处理器系统，同步可能是性能的瓶颈"

- **Hardware Primitive:** 提供一个可以原子性地获取并更改内存值的 Instruction Sequences
	- 在 User Level Synchronization 上使用
- **Spin Lock:** 处理器使用一个循环持续尝试获取锁


```asm
# atomic exchange
		DADDUI R2, R0, 1   # locked value
lockit: EXCH   R2, 0(R1)   # exchange
		BNEZ   R2, lockit  # if success, R2 = 0

# test-and-set
lockit: T&S   R2, 0(R1)
		BNEZ  R2, lockit

# spin lock using LL & SC
lockit: LL    x2, 0(x1)     # load linked
		BNEZ  x2, lockit    # no free => spin
		ADDUI x2, x0, 1     # locked value
		SC    x2, 0(x1)     # atomic exchange
		BNQZ  x2, lockit    # already lock?
```

如果已经实现了缓存一致性，则 Lock Value 可能会在 Cache 中；否则它一定存储在 Memory 中。

!!! danger "`ll` 和 `sc` 之间只能由 Register-Register Instruction，不能有访存指令"
	与 `EXCH` 等相比，它们将读取和写入独立，解决了不恰当竞争，例如对 unavailable lock 进行不必要的 write 请求。

![[synchronizationex.png]]