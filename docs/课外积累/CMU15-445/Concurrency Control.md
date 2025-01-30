
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
- **Exclusive Lock (X-LOCK):** 排它锁允许一个事务修改对象，但会组织其它事务获取这个对象上的任何锁

!!! warning "对比 Latch"
	Latches 是从其它线程中底层地保护 DBMS 内部关键数据结构的原语。Latches 仅在执行操作时保持，且不需要回滚更改。锁存器有两种模式：
	
	- **READ:** 允许多个线程同时读取同一项。即便一个线程已经获取了 Read Latch，另一个线程也可以获得它
	- **WRITE:** 只允许一个线程访问该项。如果一个线程已经获取了 Write Latch，那么其它线程不再能够获得它。持有 Write Latch 的线程还会阻止其它线程获得 Read Latch。

