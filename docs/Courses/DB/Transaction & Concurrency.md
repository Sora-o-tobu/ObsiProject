
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
	
	- **Concept：** 数据库初始是一致的，且每个事务都是一致的，则要保证结束时数据库也是一致的。
	- **Concept：** 一致性有两个概念
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

如果两个操作是有冲突的，则二者执行次序不能交换；若无冲突，则可以交换。那么如果一个 Schedule $S$ 可以通过交换一系列无冲突指令变为 $S'$，则称 $S$ 和 $S'$ 为 **Conflict Equivalent** 的。

根据以上定义，我们可以声明如果一个并行调度 $S$ 与串行调度 **Conflict Equivalent**，则该调度是 **Conflict Serializable** 的。

![[ConflictSerialEx1.png]]


### Recoverability

