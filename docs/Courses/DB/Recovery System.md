
# Recovery System

!!! quote "Failure Classification"
	- **Transaction Failure**
		- **Logical Errors:** 事务因内部错误无法完成
			- Overflow, bad input, data not found
		- **System Errors:** DBMS 必须终止一个正在活跃的事务
			- Deadlock
	- **System Crash**
		- Power Failure, Hardware Failure, Software Failure
	- **Disk Failure**
		- 磁盘存储损坏

## Log-Based Recovery

**Log** 由一系列 Log Records 组成，维护在稳定存储中:

- 当事务 $T_i$ 开始时，写入记录 `<T_i, start>`
- 在事务 $T_i$ 执行 `WRITE(X)` 前，写入记录 `<T_i, X, V_1, V_2>`
	- 其中 `V_1`, `V_2` 分别是数据项 `X` 写入前后的值
- 当事务 $T_i$ 结束时，写入记录 `<T_i, commit>`

### Deffered Database Modification

Deffered Database Modification 将所有写操作都推迟到 **Partial Commit** 阶段，即事务执行过程中只记录 Log；事务 Commit 后再读 Log 并将需要写入的内容执行。

假定 DB Crash 时，存储在 Stable Storage 中的 Log 分别为如下三种情况：

![[DefferedDatabseModificationEx1.png]]

- **【a】** 不需要做 `REDO` 操作，因为没有任何数据被实际写入
- **【b】** 需要 `REDO(T_0)`，因为 $T_0$ 已经 `COMMIT` 了，但它的数据还没成功写入
- **【c】** 需要 `REDO(T_0), REDO(T_1)`

### Immediate Database Modification

Immediate Modification 允许事务在未提交之前就将更新写入 Buffer 或者 Disk 中，但是仍然需要注意 Log Record 的写入必须在数据项的写入之前。

!!! question "这里我们暂时假设 Log Record 是直接写入 Stable Storage 中的"

由于将更新后的 Block 写回磁盘相对耗时，它们的实际写入时间可能出现在该事务 `COMMIT` 之后，例如：

![[ImmediModiEx1.png]]

在如上背景下，我们将数据库的恢复分为两个操作：

- **【Undo】** 从事务 $T_i$ 的最后一个记录开始，将对应数据项的值都恢复到 old values
	- 如果记录只包含 `<T_i, start>` 不包含 `<T_i, commit>`
	- Undo 时，也要记录对应的日志
- **【Redo】** 从事务 $T_i$ 的第一个记录开始，将对应数据项的值都设置为 new values
	- 如果记录既包含 `<T_i, start>` 又包含 `<T_i, commit>`

!!! warning "由于同一个 operation 在恢复过程中可能执行多次，它们都需保证是 idempotent 的"
	即幂等，无论执行同一操作多少次，最后的效果和只执行一次相同。

![[ImmeModiEx2.png]]

!!! tip "先 REDO 再 UNDO"

在数据库运行过程中，我们会设置 **CheckPoints**，以防止数据库恢复需要读取的 Log 太多。

在执行 Checkpointing 时：

- <1> 将主存中的所有 Log Record 都写入磁盘
- <2> 将所有被更新过了的 Buffer Blocks 都写入磁盘
- <3> 写入一条日志 `<CheckPoint L>` 到磁盘
	- `L` 是执行 CheckPoint 时还在活跃的事务列表

!!! info "Checkpointing 时所有 Updates 都会停止"

在执行恢复时，我们首先找到记录 `<CheckPoint L>`，只有 `L` 以及位于检查点之后开始的事务需要 Redo 和 Undo。

## Recovery With Concurrent Transactions

```python
undo_list = []
redo_list = []
log_ptr = the end of log
while (*log_ptr != <checkpoint L>):
	switch (log_ptr->type):
		case <T_i, commit>:
			redo_list.append(T_i)
		case <T_i, start>:
			if not T_i in redo_list:
				undo_list.append(T_i)
		case <T_i, abort>:
			undo_list.append(T_i)
for txn in L:
	if not T_i in redo_list:
		undo_list.append(T_i)
```

在得到两个列表后，先从后向前 Undo，再从 Checkpoint 向后 Redo。

![[RWCTEx1.png]]

## Buffer Management

主存和 Stable Storage 交互的最小单位时 Block，而一个 Log Record 的大小远小于一个 Block，我们不可能每次都直接将单个 Log 写入磁盘。通常，Log 都缓存在主存中，只有如下情况才会输出到磁盘中：

- <1> Blocks of log records in buffer are full
- <2> Log Force operation is executed
	- checkpoint happens
	- 如果采用 Force Policy 恢复算法，一个事务 `COMMIT` 时也要将其所有 LOG 都强制写回

!!! info "Rules for Log Record Buffering"
	- 只有 Log `<T_i, commit>` 写入磁盘后 $T_i$ 才能进入 `COMMIT` 状态
	- 所有 $T_i$ 相关 Log 都得早于 `<T_i, commit>` 写入磁盘
	- Log 的写入需要早于其对应的数据项
		- 即 Write-Ahead Logging rule, WAL

!!! note "Latch"
	在将一个 Block 写回磁盘的时候，其中的数据项不应该被其它更新修改。因此，我们可以为这个 Block 申请一个 Exlusive Lock。这种短期锁实际被称为 **Latch**。

Database Buffer 可以在 Real Main Memory 或者 Virtual Memory 中执行，它们各有优劣：

=== "Drawbacks of Reserved Main-Memory"
	- 内存事先被划分，灵活性被限制
	- 内存的划分不可改变，因此当 DB 的需求改变时难以更改 Buffer 大小
=== "Drawbacks of Virtual Memory"
	- Page 的写回可能需要从磁盘的 Swap Space 中取出对应块，再写回，即造成额外 IO，这被称为 Dual Paging Problem
		- Swap Space 是磁盘暂存“溢出”的内存的区域
	- 理想情况下，当 OS 打算回收某个 Page 时，它应该先通知 DBMS，如果这个 Page 不是 Dirty 的，那么可以直接释放；如果是 Dirty 的，那么先写入相关日志，再把该数据页写入数据库文件，之后也可以直接释放，不需要放入临时的 Swap Space，从而避开 Dual Paging Problem
		- 可惜一般 OS 不支持这个功能

