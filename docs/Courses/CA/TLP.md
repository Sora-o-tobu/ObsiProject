
# Thread-Level Parallelism

## Multi-Processor

单处理器性能优先，现代U基本都为多处理器，它属于 **MIMD** 架构。

多处理器根据 Memory Model 的不同可分为 **UMA** 和 **NUMA** 两种：

![[MultiProcessorMemoryModel.png]]

其最主要的区别在于 Processor 和 Memory 之间的位置关系。对于 UMA，一个处理器访问任何 Memory 都是等延迟的，但是随着系统的增大，其延迟显著增大；对于 NUMA，一个处理器访问离它最近的 Memory 最快，访问别的 Memory 需要经过 Interconnect 中转，这一架构更适用于 *Large Multiprocessor Systems*。

多处理器核与核之间的编程模型可以分为四种：

- **<1> Multiprogramming:** 无交流
- **<2> Shared Address Space:** 通过内存共享
- **<3> Message Passing:** 通过消息传递
- **<4> Data Parallel:** 在不同数据上同时处理，并全局实时交换信息

处理器之间的通信存在一定延迟，在硬件上，可以通过使用**缓存**共享数据，这需要解决缓存一致性的问题。而在软件上，则使用**同步 Synchronization**。

!!! warning "缓存一致性是第五章最重要的内容"

### Cache Coherence

在 Shared Memory 的情况下，对于同一块内存，可能有不同的处理器在进行读写操作。如果每个处理器都有自己的缓存，当一个处理器修改了内存中的数据，其它处理器的缓存中可能仍然保持着旧数据。我们需要一种方法能够“通知”其它处理器这一修改，主要有以下两种策略：

- 监听策略 snooping
- 目录策略 directory-based

<font style="font-weight: 1000;font-size: 20px" color="orange">Snooping 协议</font>

Snooping 协议允许处理器之间进行通信。当 P1 写入 X，需要通知其它处理器，其它处理器监听到这一通知时，如果自己的 Cache 中也有 X 的副本，可选以下两种不同的策略进行处理：

- **Write Invalidate:** 将 X 的副本设置为无效
	- 即设置缓存块的 Valid 位为 0
- **Write Broadcast:** 更新 X 的副本

!!! info "通过 BUS 实现"
	即每块缓存都能接收来自自己的 Processor 的信号以及来自 BUS 的信号。

在 **Simple Write-Invalidate Protocol** 中，每个缓存块设置了三种状态：

- **Invalid:** 无效状态
	- 即 Valid 位为 0
- **Shared:** 共享状态
	- Valid = 1, Dirty = 0
- **Exlusive:** 独占状态
	- Valid = 1, Dirty = 1

=== "From CPU"
	![[MPfromCPU.png]]
=== "From Bus"
	![[MPfromBus.png]]

!!! note
	- Cache 本身采取 Write-Back 策略，对于自身 CPU 的请求，只要没有写请求则一直处于 Shared 状态（和其它处理器共享相同数据），如果要写缓存，则进入独占状态，此时如果别的处理器想要读或写该内存，都需要将其写回
	- Cache 本来在独占状态，如果接收到 Bus 的 Write Miss 请求(其它处理器尝试写入同一内存)，则将脏块写回且进入 Invalid 状态（因为采用 Write-Invalidate Protocol）

![[SnoopingProtocolEx1.png]]

考试常考的则是其拓展协议 **MESI Protocol**，其增加一个状态 *Modified*：

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

<font style="font-weight: 1000;font-size: 20px" color="orange">目录协议</font>

为每一块内存记录 **directory**，目录的内容为：

- 该块的状态
	- Shared
	- Uncached
	- Exclusive
- 哪些处理器有这个块的副本
	- 用 bit vector 表示
- Dirty/Clean

![[DirectoryProtocolOverview.png]]

!!! note "目录协议可以既可以在 Distributed Memory 中实现，也可以在组织成 banks 的 Centralized Memory 中实现"

对一块内存操作，涉及三个处理器的概念：

- **Local Node:** 发起请求的处理器
- **Home Node:** 对应内存地址所在的处理器
	- Home Node 负责维护该内存地址对应的数据项的状态，例如 `X:S, {P1,P2}` 表示数据项 X 为 Shared 状态，Remote Node 为 P1, P2
- **Remote Node:** 拥有这个副本的处理器

> 例子好复杂哦，去看 PPT 或者其它资料吧

### Synchronization