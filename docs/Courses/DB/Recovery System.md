
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

!!! info "这种记录前后值的方式称为物理日志，逻辑日志则记录值的变化，如 `+100`，其颗粒度更小"

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

## ARIES 算法

### Data Structure

我们之前提到的恢复算法其实都是基于 ARIES 算法简化的，它们去除了 Optimization 部分。跟它们相比，ARIES 算法有如下特征：

- <1> 使用 **Log Sequence Number(LSN)** 来标志 Log Records
	- LSNs 以 Page 的形式存储
- <2> Physiological Redo
	- 是介于物理和逻辑 Redo 中的策略，它以物理的方式定位受影响的 Page，以逻辑的方式记录业内的操作
	- 相比于物理 Redo 需要记录整个 Page 的字节变化，Physiological Redo 只需要记录页内编号 + slot 操作，节省大量空间
	- *Warning:* 要求 Page 的写回是原子操作 
- <3> 维护 Dirty Page Table 来避免不必要的 Redo 操作
- <4> 使用 Fuzzy Checkpoint，在检查点只记录脏页的相关信息，不要求脏页写回

!!! info "Log Sequence Number"
	LSN 作为 Log 的标识符，要求顺序递增。为了加快访问速度，通常直接取 Log 在业内的偏移作为 LSN。

我们在 Page 的首部中还需额外维护 **PageLSN**，用来存储最后一条影响该 Page 的日志的 LSN。在恢复阶段，LSN 值不超过 PageLSN 的日志记录不会在该页上执行，从而避免重复的 Redo。

一个普通的 Log Record 的格式如下：

```
+-----+---------+---------+----------+----------+
| LSN | TransID | PrevLSN | RedoInfo | UndoInfo |
+-----+---------+---------+----------+----------+
```

在之前的恢复算法中，Undo 阶段时我们需要为所有被 Undo 的日志写入对应的 Undo Log。在 ARIES 中，该日志称为 **Compensation Log Record(CLR)**，是一个 Read-Only 的日志记录，并且有一个特殊的字段 `UndoNextLSN`，用于指向下一个需要执行 Undo 操作的日志：

```
+-----+---------+-------------+----------+
| LSN | TransID | UndoNextLSN | RedoInfo |
+-----+---------+-------------+----------+
```

!!! tip "在该 CLR 和其 `UndoNextLSN` 之间的所有日志都已经被 Undo 了，从而避免了重复的 Undo 操作"
	![[CLRStructure.png]]

Dirty Page Table 记录仍在 Buffer 中、已经被更新过的 Page 的信息，包括：

- **PageLSN**
- **RecLSN** LSN 小于 RecLSN 的日志都已经生效且写回磁盘中了
	- 当一个 Page 被插入脏页表中时，设置 RecLSN = PageLSN（更新前一刻的）

只要一个 Dirty Page 被写入磁盘，则从脏页表中移除该页。

!!! example "ARIES Data Structure"
	![[ARIESDS.png]]
	
	- `4894.1` 表示 Page 4894 的第一个数据
	- `RecLSN = 7564` 表示从 LSN 7564 开始的 Log 就没有对 Stable Data 中的表生效了（尽管它们已经进入 Stable Log 中）


在 CheckPoint，记录当前 Dirty Page Table 和当前正在活跃的事务。对于每一个活跃事务，记录它们写入的最后一个 Log Record 的 LSN，记为 **LastLSN**。

脏页不必在 CheckPoint 写回，它们在后台持续写回，因此 CheckPoint 的消耗较小，可以相对更频繁的设置。

### Recovery Algorithm

ARIES 的恢复算法分为三个阶段：分析阶段、Redo阶段、Undo阶段。

- **<1> Analysis pass**
    - 从最后一个 CheckPoint 开始
    - 读取 Dirty Page Table
	    - 设置 `RedoLSN = min RecLSN`（脏页表中的最小 RecLSN），如果脏页表为空，则设置为 CheckPoint 的 LSN
	    - 初始化 Undo List 为 CheckPoint 中记录的活跃事务
	    - 读取 Undo List 中每一个事务的最后一条 Log 的 LSN
	- 从 CheckPoint 开始正向扫描
		- 如果发现了不在 Undo List 中的事务则加入 Undo List
		- 如果发现了 Update Log Record，如果对应 Page 不在脏页表中，则将其加入脏页表中（设置其 RecLSN 为该日志的 LSN），==用于 Redo==
		- 如果发现了事务的 End Log，则将其从 Undo List 中移除
		- 扫描过程中记得记录每一个事务的最后一条日志的 LSN，==用于 Undo==
- **<2> Redo pass**
	- 从 RedoLSN 开始正向扫描 ，当发现更新记录的时候
		- 如果对应 Page 不在脏页表中。或者这一条记录的 LSN 小于页面的 RecLSN 就忽略这一条
		- 否则从磁盘中读取这一页，如果磁盘中得到的这一页的 PageLSN 小于该 Log，就 redo，否则就忽略这一条记录
- **<3> Undo pass**
	- 从分析阶段获得的每个事务最后一条日志的 LSN 开始，接下来每一步都选取这之中 LSN 最大的日志开始 Undo
	- 在 undo 一条记录之后
		- 对于普通的记录，将 NextLSN 设置为 PrevLSN
		- 对于CLR记录，将 NextLSN 设置为 UndoNextLSN
			- 这之间的日志将被跳过，不会执行 Undo

!!! question
	![[ARIESRecoveryEx1.png]]