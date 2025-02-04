
# 8. Concurrency Control

## ACID

!!! quote "Transactions"
	事务是在一个共享数据库上执行的一系列不可拆分的操作，它们是 DBMS 的基本更改单位，即事务是原子操作(atomic)。
	
	!!! info "Atomic"
		示例：将 100 美元从 Andy 的银行账户转入我的账户
		
		1. 检查 Andy 是否有 100 美元
		2. 从他的账户扣除 100 美元
		3. 向我的账户添加 100 美元
		
		要么完成所有步骤，要么所有步骤都不做

在 SQL 中，事务以 `BEGIN` 命令开始，若事务的结果是 `COMMIT` ，将其更改保存在数据库中；若事务的结果是 `ABORT` ，撤销事务的所有更改。用于确保数据库正确性的标准由首字母缩写 **ACID** 给出：


- <font style="font-weight: 1000;font-size: 17px" color="grey">Atomicity:</font>  确保单个事务是原子操作，有两种方法可以做到这点
	- **Approach 1: Logging** DBMS 记录所有操作，如果事务结果是 `ABORT` 则可以根据记录撤销操作。几乎所有现代 DBMS 都使用 Logging 方法
	- **Approach 2: Shadow Paging** DBMS 创建需要修改的 Page 的副本给事务修改，仅当事务结果为 `COMMIT` 时应用。
- <font style="font-weight: 1000;font-size: 17px" color="grey">Consistency:</font> 数据库初始是一致的，且每个事务都是一致的，则要保证结束时数据库也是一致的。一致性有两个概念：
	- **Database Consistency:** 数据库准确表示它建模的真实世界实体，并遵循 Integrity 约束（如，一个人的年龄不会是负数）
	- **Transaction Consistency:** 数据库在事务前是一致的，则事务后也是一致的。确保事务一致性是应用程序的责任。
- <font style="font-weight: 1000;font-size: 17px" color="grey">Isolation:</font> 单个事务的执行要与其它事务不相互影响。DBMS 必须在保持隔离错觉的同时，交错执行并发事务
	- 并发控制协议分为两大类:
		- **Pessimistic:** DBMS 假设事务总会发生冲突，因此它首要目标是不让冲突问题发生
		- **Optimistic:** DBMS 假设事务冲突很少发生，因此它选择在事务提交后发生冲突时处理
	- 如果两个事务中的两个操作要对同一个对象执行，并且至少有一个操作是写入操作，则会发生冲突，它有三种变体：
		- **Read-Write Conflicts (Unrepeatable Reads):** 一个事务对一个对象进行多次读取，无法获得相同的值
		- **Write-Read Conflicts (Dirty Reads):** 在其它事务的写入操作 `COMMIT` 之前，一个事务就已经读取到更改后的值
		- **Write-Write Conflicts (Lost Update):** 一个事务覆盖另一个并发事务的未提交数据
- <font style="font-weight: 1000;font-size: 17px" color="grey">Durability:</font> 如果事务结果是 `COMMIT`，它对数据库的影响需要持续存在
	- DBMS 可以使用 Logging 或者 Shadow Paging 来确保更改是 Durable 的。

## Two-Phase Locking

DBMS 使用 Locks 为事务动态生成可序列化的 Execution Schedule，并且不需要提前知道每个事务的读写设置，这些 Locks 在并发访问下保护数据库对象。

Locks 有两种基本类型：

- **Shared Lock (S-LOCK):** 允许多个事务同时读取同一个对象。如果一个事务持有 Shared Lock，另一个事务也可以获取相同的 Shared Lock
- **Exclusive Lock (X-LOCK):** 排它锁只允许一个事务修改对象，并且会阻止其它事务获取这个对象上的任何锁

!!! warning "对比 Latch"
	Latches 是从其它线程中底层地保护 DBMS 内部关键数据结构的原语。Latches 仅在执行操作时保持，且不需要回滚更改。锁存器有两种模式：
	
	- **READ:** 允许多个线程同时读取同一项。即便一个线程已经获取了 Read Latch，另一个线程也可以获得它
	- **WRITE:** 只允许一个线程访问该项。如果一个线程已经获取了 Write Latch，那么其它线程不再能够获得它。持有 Write Latch 的线程还会阻止其它线程获得 Read Latch。

事务向 Lock Manager 请求 Lock，而 Lock Manager 根据其它事务当前持有的锁来决定同意或阻止请求。Lock Manager 内部维护一个 Lock-Table，存放哪些事务持有锁以及哪些事务在等待获取锁。

!!! tip "Lock-Table 不需要是 Durable 的，因为当 DBMS 崩溃时，所有仍在运行的事务都会自动中止"

Two-Phase Locking 属于 **Pessimistic** 并发控制协议，并且该协议不需要提前知道事务将执行的所有查询操作。

- **Phase 1 - Growing:** 在 Growing 阶段，每个事务都从 DBMS 的 Lock Manager 请求它需要的锁，Lock Manager 授权/拒绝这些请求
- **Phase 2- Shrinking:** 事务在释放它们的第一个 Lock 时进入 Shrinking 阶段，在此阶段只允许事务释放 Lock，并且不能获取新的锁

!!! info "Two-Phase Locking 缩写为 2PL"

2PL 仍然可能有 Dirty Reads、Deadlock 等问题，具体解决方法请看 [Lecture 16](https://15445.courses.cs.cmu.edu/fall2022/schedule.html)

## Timestamp Ordering Concurrency Control

时间戳排序属于 **Optimistic** 并发控制协议。相比事务在被允许读写前需要获取锁，时间戳排序使用时间戳来确定事务的可序列化顺序。

每个事务 $T_i$ 都被分配了一个独特的固定时间戳 $TS(T_i)$，不同的具体方案分配时间戳的方式不同，一些高级方案甚至为每个事务分配多个时间戳。

如果 $TS(T_i) <TS(T_j)$，那么 DBMS 必须保证 Execution Schedule 中 $T_i$ 在序列化顺序早于 $T_j$。

!!! info "时间戳分配策略"
	DBMS 可以使用系统时钟作为时间戳，但是这在夏令等极端时段会出现问题；另一种选择是使用 Logical Counter，但是这存在溢出和在分布式系统中难以维护的问题。一些混合方法结合使用这两种方案。


Basic Timestamp Ordering Protocol(BASIC T/O) 允许在不适用 Lock 的情况下对数据库对象进行读写操作。作为替代，对于任意数据库对象 $X$，它都会被标记上最后成功执行读操作的时间戳 $R-TS(X)$ 或写操作的时间戳 $W-TS(X)$。

对于任意操作，数据库会检查其作用对象的时间戳，如果事务试图以违反时间戳排序的方式访问对象，则该事务会被中止并重新启动。由于时间戳排序假设违反规则的情况很少出现，因此这些重启也很少发生。

## Multi-Version Concurrency Control

MVCC 不仅是一个并发控制协议，它涉及到数据库管理系统设计和实现的各个方面。

!!! info "MVCC 是 DBMS 中最广泛使用的方案"

在 MVCC 下，DBMS 为每个逻辑对象维护多个物理版本，当事务写入对象时，DBMS 创建一个该对象的新版本；当事务读取对象时，它读取对象的最新版本。

MVCC 的一个基本优点是 Writer 不会阻塞 Writer，Reader 不会阻塞 Reader。这也意味着一个事务修改对象的同时，其它事务可以读取该对象的旧版本。

此外，多版本数据库还可以轻松支持 time-travel queries，例如对 3 小时前的数据库进行查询。

MVCC 有四个重要设计决策：

1. Concurrency Control Protocol
2. Version Storage
3. Garbage Collection
4. Index Management

其中 Concurrency Control Protocol 可以选用前几节的 Two-Phase Locking、Timestamp-Ordering 等。

### Version Storage

Version Storage 有关数据库管理系统如何存储逻辑对象的物理版本以及事务如何找到对它们可见的最新版本。

DBMS 在元组中添加一个指针字段，为每个逻辑元组创建一个版本链，其本质是一个按照时间戳排序的版本链表。索引始终为链表的“头部”，即取决于设计，要么指向最新版本，要么指向最旧版本，线程遍历链表知道找到正确的版本。

- **方案一: Append-Only Storage**
	- 逻辑元组的所有物理版本都存储在同一表空间中，每次更新只需要将新版本追加到表中并更新版本链
	- 如果版本链按最新到最老(N2O)排序，则每次添加新版本都要更新索引
- **方案二: Time-Travel Storage**
	- DBMS 维护一个独立的 time-travel table，该表存储元组的旧版本。每次更新时，DBMS 将元组的旧版本复制到 time-travel table 中，并用新数据覆盖主表中的元组。主表中的元组指针指向 time-travel table 中的过去版本
- **方案三: Delta Storage**
	- 与方案二类似，但是 DBMS 只存储 Deltas。所谓 Deltas 即新旧版本之间元组的变化，事务通过遍历 Deltas 来重新创建旧版本
	- 比 Time-Travel Storage 写入更快，但是读取更慢

### Garbage Collection

数据库系统需要随着时间推移从数据库中移除可回收的物理版本。如果一个版本不可被任何活动事务访问，或者它是由一个被中止的事务创建的，那么这个版本就是可回收的。

<font style="font-weight: 1000;font-size: 20px" color="orange">Approach 1: Tuple-level GC</font>

在元组级别的垃圾回收，DBMS 通过直接检查元组来找到旧版本，实现这一目标有两种策略：

- **Background Vacuuming:** 有一单独线程定期扫描表并查找可回收的版本。
	- 适用于任意版本存储方案，一个简单优化是维护一个 Dirty Page Bitmap，跟踪自上次扫描以来哪些页面被修改
- **Cooperative Cleaning:** 工作线程在遍历版本链时识别可回收版本
	- 这只适用于 O2N 链

<font style="font-weight: 1000;font-size: 20px" color="orange">Approach 2: Transaction-level GC</font>

在事务级别的垃圾回收中，每个事务负责跟踪自己的旧版本，DBMS 不必扫描元组。当一个事务完成，Garbage Collector 使用事务维护的读写集来识别哪些元组可以回收。

### Index Management

所有 Primary Key(pkey) indexes 都指向版本链头部。DBMS 更新 pkey 的频率取决于元组更新时是否有新版本被创建。

!!! info "事务更新 pkey，这一操作被视为 `DELETE` => `INSERT`"

管理 Secondary Indexes 情况更加复杂，通常有如下两种办法处理：

- **Logical Pointers**
	- 数据库管理系统为每个元组设置一个固定标识符，使用一个额外的间接层来讲逻辑 ID 转换到元组的物理位置，而元组的更新只需要更改间接层中的映射
- **Physical Pointers**
	- DBMS 使用版本链头部的物理地址。但是这种策略要求头部更新时每个 index 都要相应更改

