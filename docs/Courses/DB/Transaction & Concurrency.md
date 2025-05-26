
# Transaction & Concurrency

## Transaction Concept

事务是在一个共享数据库上执行的一系列不可拆分的操作，它们是 DBMS 的基本更改单位，即事务是原子操作(atomic)。

通常，事务由一系列 SQL 语句组成，并以 `COMMIT` 或 `ROLLBACK` 结尾，例如：

```sql
BEGIN
UPDATE account SET balance = balance - 50
	WHERE account_number = A;
UPDATE account SET balance = balance + 50
	WHERE account_number = B;
COMMIT;
```

上述操作，要么完成所有步骤，要么都不做。在事务执行过程中，数据库可能是 inconsistent 的，但是我们保证当事务 commit 后，数据库能够保持一致性。

为了确保数据的完整性，我们维护数据库 **ACID** 性质：

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">Atomicity</font>
	
	- **Concept：** 确保单个事务是原子操作，有两种方法可以做到这点
	- **Approach：**
		- <1> **Logging** DBMS 记录所有操作，如果事务结果是 `ABORT` 则可以根据记录撤销操作。几乎所有现代 DBMS 都使用 Logging 方法
		- <2> **Shadow Paging** DBMS 创建需要修改的 Page 的副本给事务修改
			- 事务 `COMMIT` 时将结果 Flush 回磁盘，并将 `db_pointer` 指向该拷贝
			- 事务 `ABORT` 时直接将拷贝删除

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">Consistency</font>
	
	- **Concept：** 数据库初始是一致的，且每个事务都是一致的，则要保证结束时数据库也是一致的。一致性有两个概念：
		- **Database Consistency:** 数据库准确表示它建模的真实世界实体，并遵循 Integrity 约束（如，一个人的年龄不会是负数）
		- **Transaction Consistency:** 数据库在事务前是一致的，则事务后也是一致的。确保事务一致性是应用程序的责任。

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">Isolation</font>
	
	- **Concept：** 单个事务的执行要与其它事务不相互影响。DBMS 必须在保持隔离错觉的同时，交错执行并发事务。
	- **Approach：**
		- <1> **Pessimistic:** DBMS 假设事务总会发生冲突，因此它首要目标是不让冲突问题发生
		- <2> **Optimistic:** DBMS 假设事务冲突很少发生，因此它选择在事务提交后发生冲突时处理

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">Durability</font>
	
	- **Concept：** 如果事务结果是 `COMMIT`，它对数据库的影响需要持续存在。
	- **Approach：** DBMS 可以使用 *Logging* 或者 *Shadow Paging* 来确保更改是 Durable 的。

一个事务共有五个状态：

- **Active:** 初始状态，事务执行中一直处于 Active
- **Partially Commited:** 所有语句都执行完毕，但还没有 Commit，此时要输出的数据可能还在 Buffer 中
- **Failed:** 无法正常执行
- **Aborted:** Rollback，并将数据库恢复至该事务执行前的状态。Abort 后有两种可选方案
	- <1> Restart Transaction
		- 只有无内部逻辑错误发生时可选
	- <2> Kill Transaction
- **Committed:** 顺利结束并提交

![[TransactionState.png]]

## Concurrent Executions & Schedules

!!! quote "并发执行的优点"
	- 增加处理器和磁盘的利用率
		- 一个事务读写磁盘的同时，另一个事务可以使用 CPU
	- 减少平均响应时间
		- 较短事务不用等待较长事务结束

并发执行会破坏数据库的一致性，因此我们需要对正在执行的一系列事务进行调度。**Schedules** 指并行事务所有指令的执行顺序。

一种最简单的调度为 Serial Schedule，它一定可以保证数据库的一致性，但是效率低下：

![[SerialScheduleEx1.png]]

!!! info "N 个并行事务有 N! 种可选的串行调度"

与之相对的是并行调度，它不能保证一致性，因此我们从 **Serializability** 和 **Recoverability** 两个方向来评价它。

### Serializability

如果一个 Schedule 等价于 Serial Schedule，我们称其为 **Serializable** 的。它可以细分为冲突可串行化以及视图可串行化，本课程只要求 Conflict Serializability。

对于两个事务 $T_i, T_j$ 中的两条对同一个数据项操作的指令 $I_i, I_j$，当且仅当它们均为读操作，即 `READ(Q)` 时，认为它们之间不会发生 Conflict。

!!! info "对不同数据项操作的指令都不会有冲突"

如果两个操作是有冲突的，则二者执行次序不能交换；若无冲突，则可以交换。那么如果一个 Schedule $S$ 可以通过交换一系列无冲突指令变为 $S'$，则称 $S$ 和 $S'$ 为 **Conflict Equivalent** 的。

根据以上定义，我们如果可以声明一个并行调度 $S$ 与串行调度 **Conflict Equivalent**，则该调度是 **Conflict Serializable** 的。

![[ConflictSerialEx1.png]]


### Recoverability

Recoverability 用于评价一个调度对于并行事务失败的可恢复性。一个 **Recoverable Schedule** 要求：

对于一个要读取数据项 $A$ 的事务 $T_j$，如果 $A$ 在读取之前被事务 $T_i$ 修改过，则事务 $T_i$ 的 `COMMIT` 操作要早于 $T_j$ 的 `COMMIT` 操作。

如下是一个简单的 **NOT Recoverable** 的例子，如果事务 $T_8$ Abort 了的话，那么 `WRITE(A)` 的操作也被回滚，则已经提交了的事务 $T_9$ 的 `READ(A)` 读取到的值是错误的，数据库完整性受损：

![[RecoverableScheduleEx1.png]]

那么如果一个事务 Abort 了的话，其它相应的事务也要一起终止，这被称为 **Cascading Rollback**：

![[RecoverableScheduleEx2.png]]

!!! note "Cascading Rollbacks 可能导致巨大的开销，这也是我们不期望的"

如果一个 Schedule 中，对于任意一组对同一个数据项先写后读的事务 $T_i, T_j$，如果 $T_i$ 的 `COMMIT` 操作在 $T_j$ 的读操作之前，即 $T_j$ 不需要跟着 $T_i$ 一起 `ROLLBACK`，则称该调度为 **Cascadeless Schedules**。

## Lock-Based Protocols

**Serializable Schedule** 是并行控制的基础，DBMS 使用 Locks 为事务动态生成可串行化的调度，这些锁在并发访问下保护数据对象：

- **Shared Lock（S）:** 只允许该对象被其它事务读取。
	- 如果一个事务持有 Shared Lock，另一个事务仍然可以获取相同的 Shared Lock
- **Exclusive Lock（X）:** 排它锁只允许一个事务修改对象，并且会阻止其它事务获取这个对象上的任何锁

那么一般来说，如果我们需要写一个数据，则申请一个 X 锁；如果只需要读一个数据，则申请一个 S 锁。如果申请被拒绝了，则我们需要一直等待到该对象先前的锁被释放，自己成功获得该锁时才能继续执行。

!!! note "DeadLock"
	**Deadlock（死锁）** 为当一组事务互相等待对方持有的资源（锁）释放，从而都无法继续执行的状态。
	
	死锁是并发控制中无法完全避免的现象，只能通过预防、避免、检测与恢复等手段来管理。

Two-Phase Locking 属于 **Pessimistic** 并发控制协议，并且该协议不需要提前知道事务将执行的所有查询操作。

- **Phase 1 - Growing:** 在 Growing 阶段，每个事务都从 DBMS 的 Lock Manager 请求它需要的锁，Lock Manager 授权/拒绝这些请求
	- 允许将 S 锁升级为 X 锁
- **Phase 2- Shrinking:** 事务在释放它们的第一个 Lock 时进入 Shrinking 阶段，在此阶段只允许事务释放 Lock，并且不能获取新的锁
	- 允许将 X 锁降级为 S 锁

该控制协议能够确保得到一个 **Conflict Serializable Schedule**，我们可以根据两个阶段的分界线 Lock Point 的时间顺序来得到可串行化的执行顺序。

!!! info "如果 Precedence Graph 中存在边 $T_i \rightarrow T_j$，则这两个事务的 lock point 一定满足 $\alpha_i < \alpha_j$"

!!! warning
	- Two-Phase Locking 不保证没有死锁的发生
	- Two-Phase Locking 不保证没有 Cascading Rollback 的发生，但是可以通过加强版协议 Strict Two-Phase Locking 来解决
		- 即一个事务的排他锁只有 `COMMIT` 或 `ABORT` 时才能释放
	- Two-Phase Locking 是得到 Conflict Serializable Schedule 的充分条件，而不是必要条件，这也说明存在不能由该协议得到的 Conflict Serializable Schedule

=== "`READ(D)`"
	```python
	if T_i has a lock on D:
		read(D)
	else:
		if wait until no other txn has a lock-X on D:
			lock-S(D)
			read(D)
	```
=== "`WRITE(D)`"
	```python
	if T_i has a lock-X on D:
		write(D)
	else:
		if wait until no other txt has any lock on D:
			if T_i has lock-S on D:
				upgrade lock-S to lock-X
			else:
				lock-X(D)
			write(D)
	```

DBMS 中，通过一个单独的线程 Lock Manager 来处理事务的锁请求。在管理器内部维护了一个 Lock-Table 表，里面存储了哪些事务持有锁以及哪些事务正在等待锁的信息。

!!! info "通常，Lock-Table 被实现为内存中根据数据项的名称来哈希的哈希表"

### Graph-Based Protocols

Graph-Based Protocols 是 2PL 的一个替代，它用 $d_i\rightarrow d_j$ 表示数据项之间的先后关系，且任意同时访问了 $d_i, d_j$ 的事务都必须满足访问 $d_i$ 早于 $d_j$。

通过这种方式绘制的 Directed Acyclic Graph 称为 **Database Graph**。我们以一种简单的 Graph-Based 协议 **Tree Protocol** 来简单讲解其基本逻辑。

![[TreeProtocolEx1.png]]

- <1> 只允许使用排他锁 Exclusive Lock
- <2> 对于任意事务 $T_i$，其加锁的第一个事务可以是 Tree 中任意一个节点，但是后来加锁的数据项必须是*当前*已拥有锁的节点的子节点
- <3> 数据项可以随时解锁，但是已经解锁的数据项后面不能再次上锁

![[TreeProtocolEx2.png]]

!!! quote ""
	=== "Advantage"
		- Tree Protocol 保证 **Conflict Serialzability** 和 **Freedom From Deadlock**
			- Deadlock Free 也意味着不需要 Rollback
		- 相比于 2PL，可以更早对数据项解锁
			- 意味着更短的等待时间，以及并行性的增加
	=== "Disadvantage"
		- 不保证 **Recoverability** 和 **Freedom of Cascade**
		- 事务可能需要对一些它们不访问的数据项上锁，才能上锁下层的子节点
			- 增加了锁开销和等待时间，可能会降低并发度

## Multiple Granularity

为了更好的便利性，我们允许根据需要以不同大小来上锁数据项，即 **Multiple Granularity**。我们根据 Data Granularities 划分出层次，将其表示为 Tree 的形式：

![[HierarchyOfDataGranEx1.png]]

当一个事务显式地对一个节点上锁时，它还会隐式地对其所有后代都加上同样的锁。

!!! info "Granularity of Locking"
	- **Fine Granularity(细粒度，lower in the tree)**: high concurrency, high locking overhead
		- 为什么细粒度带来更高锁开销？系统中存在大量锁对象，也需要更频繁的加锁/解锁操作，因此需要更多的锁管理、调度等资源
	- **Coarse Granularity(粗粒度，higher in the tree):** low locking overhead, low concurrency

如果事务 $T_1$ 对 $r_{a1}$ 施加排他锁后，事务 $T_2$ 想要对整个 DB 施加共享锁，那么它就需要扫描整棵树来确定节点 'DB' 的后代是否存在排他锁，这自然是我们不能接受的开销。因此我们提出意向锁 **Intention Lock** 作为一个状态。

!!! note "在节点被显式上锁前，先对该节点的所有祖先都施加 Intention Locks"

- **Intention-Shared(IS):** 表明其后代中存在 S 锁
- **Intention-Exclusive(IX):** 表明其后代中存在 X 锁
- **Shared and Intention-Exclusive(SIX):** 当前节点正处于共享锁状态，不过事务正在对其后代某个节点施加排他锁
	- `S + IX` 相当于告诉其它事务，我的确在这个节点只读，但我马上会去下层做排他操作，请别与我冲突
	- 一个事务想要读取整张表的一部分内容，同时它还打算对这张表中的某些行做更新（因此要对那些行获取 X 锁），这个时候就需要在“表级别”拿一个 SIX 锁

|已有锁 \ 申请锁|IS|IX|S|SIX|X|
|---|---|---|---|---|---|
|**IS**|✅|✅|✅|✅|❌|
|**IX**|✅|✅|❌|❌|❌|
|**S**|✅|❌|✅|❌|❌|
|**SIX**|✅|❌|❌|❌|❌|
|**X**|❌|❌|❌|❌|❌|

!!! warning
	两个事务之间的 SIX 锁是不兼容的，从而保证事务可以安全读取并写入，其它事务要么在更低层级申请共享锁，要么等待前一个事务完成操作再加锁。

在这种多粒度的层次下，Locking Scheme 可以概括如下：

- <1> 必须满足锁兼容性表
- <2> 必须先对一棵树的根节点添加锁，可以用任意锁模式(S,X,IS,IX,SIX)先行锁定
	- 可以确保事务先占领整个锁层次结构的入口，然后逐层向下拓展锁
- <3> 如果想对某个节点 $Q$ 施加 *S* 或 *IS* 锁，则该节点的父节点必须处于 *IS* 或 *IX* 状态
- <4> 如果想对某个节点 $Q$ 施加 *X* 或 *IX* 或 *SIX* 锁，则该节点的父节点必须处于 *IX* 或 *SIX* 状态
- <5> 满足 2PL 协议，如果事务先前对某个节点解锁，则该事务后续不能够再施加锁
- <6> 只有当节点 $Q$ 的所有子节点都没有被 $T_i$ 持有锁时，事务 $T_i$ 才能解锁 $Q$
	- 即加锁自顶向下，解锁自下而上

!!! danger "TBD: Deadlock Handling & Insert, Delete Operations"

