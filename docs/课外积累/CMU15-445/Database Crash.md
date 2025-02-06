
# 9. Database Crash

## Crash Recovery

Recovery Algorithm 是确保数据库一致性、原子性和持久性的技术。当一个 Crash 发生时，所有未提交到磁盘内的数据都有丢失的风险，恢复算法旨在崩溃后阻止信息丢失。

所有 Recovery Algorithm 都有两部分：

- <1> 事务正常运行时进行的操作
- <2> 发生崩溃后恢复数据库到能确保一致性、原子性和持久性的状态时进行的操作

!!! quote "Key Primitives 基本操作"
	- **UNDO:** 移除一个不完整或是已中止事务的影响的过程
	- **REDO:** 重新应用已提交事务的效果的过程

## Failure Classification

DBMS 根据底层存储设备不同而被分为不同组件，相应需要处理多种不同类型的故障。其中一些故障可以恢复，一些则不能。

<font style="font-weight: 1000;font-size: 20px" color="orange">Type 1: Transaction Failures</font>

可能导致事务失败的两类错误是逻辑错误和内部状态错误：

- **Logical Errors:** 由于某些内部错误(例如完整性、约束)，事务无法完成
- **Internal State Errors:** 因为某些错误条件(例如 Deadlock)，DBMS 必须中止该事务

<font style="font-weight: 1000;font-size: 20px" color="orange">Type 2: System Failures</font>

系统故障是支撑数据库管理系统的底层软硬件中的意外故障，这些故障必须在 Crash Recovery Protocol 中得到考虑。

- **Software Failure:** DBMS 实现出现问题(例如未捕获的除0异常)，系统必须停止
- **Hardware Failure:** 托管 DBMS 的计算机崩溃(例如电源中断)
	- 我们假设非易失性存储介质的内容不会被系统崩溃损坏，这被称为 *Fail-Stop* 假设，简化了恢复过程

<font style="font-weight: 1000;font-size: 20px" color="orange">Type 3: Storage Media Failure</font>

存储介质故障是指物理存储设备损坏时发生的不可修复故障。

## Buffer Pool Management Policies

DBMS 的 Steal Policy 决定了是否允许未提交的事务覆盖非易失性存储中的已提交值：

- **STEAL:** 允许
- **NO-STEAL:** 不允许

DBMS 的 Force Policy 决定了事务在提交之前，是否需要将事务所做的所有更新反映到非易失性存储中：

- **FORCE:** 必需
- **NO-FORCE:** 非必需

!!! note "强制写入使得恢复更加容易，因为所有更改都得到保留，但会导致更差的性能"

**NO-STEAL + FORCE** 是最容易实现的缓冲池管理策略。在此策略中，DBMS 无需撤销已中止事务的修改，因为这些更改尚未写入磁盘。它也无需重做已提交事务的更改，因为所有更改都保证在提交时写入磁盘。

不过，该策略的一个缺点是所有修改数据必须能放的进内存中，否则事务无法执行，因为 DBMS 不允许在事务提交前将脏页写入磁盘。

## Shadow Paging

Shadow Paging 是对之前方案的改进，DBMS 在写入 Page 时额外复制一份，以维护数据库的两个独立版本：

- **master:** 仅包含已提交事务的更改
- **shadow:** 临时数据库，包含未提交事务的更改

更改仅在 shadow 中进行，当一个事务提交时，shadow 原子性地变成 master，而旧的 master 最终会被 Garbage Collected。

Shadow Paging 的一个缺点是复制整个 Page Table 代价高昂。实际上，我们只需要复制更新过的叶节点路径，而不是整个树。另一个问题是，这只能支持只有一个 Writer 的一组事务或者批量事务。

## Write-Ahead Logging

使用 Write-Ahead Logging，数据库管理系统在将更改写入 Disk Page 前，会将数据库的所有更改记录在一个 Logging File 中。日志中包含足够执行必要撤销以及 REDO 操作的信息，崩溃后将根据日志恢复数据库。

!!! info "Logging File 位于 Stable Storage 中"

WAL 属于 **STEAL + NO-FORCE** 的系统：

![[WALSystemFigure.png]]

!!! success "versus Shadow Paging"
	在 Shadow Paging 中，DBMS 需要对磁盘上随机非连续页面进行写操作。而 Write-Ahead Logging 则将随机写转换成顺序写，具有更好的性能，这也是几乎每个 DBMS 都使用 WAL 的原因。但是 WAL 的恢复时间比 Shadow Paging 慢，因为它必须 Replay Log。

每条日志条目都包括重放单个对象更改所需的信息：

- Transaction ID
- Object ID
- Before Value (for `UNDO`)
- After Value (for `REDO`)

DBMS 必须在向外界表明事务已提交之前，将事务的所有日志条目写入磁盘。系统可以使用分组提交的方式来优化多日志批处理。

在 Log-Structured DBMS 中，事务的 log 会被写入名为 MemTable 的内存缓冲区，当这个缓冲区满后，内容会被刷新到磁盘上。但它仍然需要一个独立的 Write-Ahead Log，这是因为 WAL 的刷新频率通常比 MemTable 高，并且 WAL 可用于崩溃后恢复 MemTable 的内容。

### Logging Schemes

Log 记录的内容根据实现方式也有所不同：

- **Physical Logging:** 
	- 记录数据库特定位置的 Byte-Level Change
- **Logical Logging:**
	- 记录事务执行的 High Level Operations
	- 相比于 Physical Logging，每个日志记录写入数据更少，因为每个记录可以更新多个 Page 中的多个元组。但是这个方案在非确定性并发控制方案中难以实现恢复效果。
- **PhysioLogical Logging:**
	- 混合方法，根据 Slot Number 识别元组而不是其具体位置。因此，在日志写入磁盘后，DBMS 可以重新组织 Page
	- 最常用的实现方式

## ARIES

Algorithms for Recovery and Isolation Exploiting Semantics 是 IBM 为 DB2 系统开发的一种恢复算法，ARIES 协议有以下三个关键概念：

- **Write-Ahead Logging:** 任何更改在写入磁盘前先写入稳定存储的日志中
- **Repeating History During Redo:** 重启时，回溯操作并恢复数据库到崩溃前的状态
- **Logging Changes During Undo:** 记录 undo 操作，以确保重复失败情况下操作不会重复执行

!!! danger "这里也摆了"