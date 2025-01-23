
# 3. Database Storage

## Overview

正如我们在其它课程中学到的，计算机的存储结构体系可划分为：

![[storagehierarchy.png]]

而数据库一般存储于非易失性半导体中，在这种存储下，顺序访问通常比随机访问快得多。

!!! note "这也要求我们在算法上最大化顺序访问的比例以获得更好的性能表现"

因为系统无法直接在磁盘上操作数据，我们的 DBMS 架构要负责弄清楚如何在非易失性磁盘和易失性 Memory 中移动数据。

在面向磁盘的 DBMS 中，数据库全部位于磁盘上，文件中的数据被组织成多个 Page，其中第一个 Page 为 index。

![[DISKORIENTEDDBMSoverview.png]]

要对数据进行操作，DBMS 需要将数据从磁盘转入内存中，它通过一个缓冲池来管理磁盘和内存之间数据移动。上层的 Execution Engine 会根据具体操作向缓冲池请求特定 Page，缓冲池负责将该 Page 放入内存，并提供指向内存中该 Page 的一个指针。

!!! info "常见 DBMS 中，除了 SQLite 是文件，其它均为多文件"

## Memory Mapped I/O

为了实现更高性能地数据库操作，我们希望为其设计一个更符合其操作特性的额外系统。在数据库系统内部，总是能够作出比直接使用操作系统更好的策略。

我们只希望操作系统提供足够的 Memory 并且保证进程不会被中断，所有的操作都应该在数据库系统内部完成。

DBMS 的上层设计目标是支持超出可用内存量的数据库。这自然而然让我们想起了 OS 中虚拟内存的应用，在 DBMS 中实现这一类似策略的方法是使用 mmap 将文件的内容映射到进程的地址空间内。

但实际出于正确性和性能原因，一般不建议在 DBMS 中使用 mmap。

## File Storage & Page

数据库存储在磁盘上的基本形式是 File，它们一般以特定形式进行编码，操作系统对这些文件的内容一无所知，只有 DBMS 知道如何解码内容。

DBMS 的存储管理器负责管理数据库的文件，它将文件表示为 Page 的集合，并会对已读取和写入的 Page 进行跟踪。

Page 中可以包含不同类型的数据（索引、元组...），但大多数系统不会在一个 Page 中混用类型。也有某些系统要求 Page 是 Self-contained，即读取每一个 Page 所需的信息都在 Page 本身中。

每个 Page 都应具有独特的标识符。大多数 DBMS 都具有一个间接层，用于将 Page ID 映射为 file path and offset。

!!! note "如果数据库是一个单文件，那么可以直接令偏移量为 Page ID，不需要 file path"

!!! success "DBMS locate a page on disk by using linked list or page directory"
	- **Linked List:** Header Page holds pointers to a list of free pages and a list of data pages.
		- 但是当我们想找一个特定的 Page 时，必须进行线性扫描
	- **Page Directory:** DBMS 维护一个特殊的 Page，用于跟踪 Date Pages 的位置以及其剩余可用空间。

支持可变大小 Page 所需开销较大，因此 DBMS 一般都使用固定大小的 Page。Page 的三个相关概念：

- <1> Hardware Page (usually 4KB)
- <2> OS Page (4KB)
- <3> Database Page (1-16KB)

存储设备保证能够支持 Hardware Page 大小的 atomic write。但是如果 Database Page 大小大于 Hardware Page，当系统发生错误崩溃时，程序很有可能会在数据库 Page 写入磁盘的途中，我们需要采取额外的措施来保证数据安全、完整的写入。

!!! info "原子写 Atomic Write"
	一个不可分割的 IO 写操作，要么一起结束，要么一起回退。

### Page 结构

Page 的开头通常包含一个 Header，用来存储关于内容的元数据：

- Page Size
- Checksum
- DBMS version
- Transaction visibility
- Self-containment (某些系统，如 Oracle 需要这个)

Page 中数据的布局有两种主要方式，分别为 slotted-pages 和 log-structured。

**Slotted Pages: Page maps slots to offsets**

- 是 DBMS 中最常用的做法
- 在 Header 中，会跟踪已经使用的 slot 数量、上一次使用的 slot 的偏移量，以及一个 slot 数组
	- Slot 是等长的，存储着对应元组的偏移量和长度等数据；而元组可以是不定长的。
- slot 数组从开头增长到结尾，元组从末尾增长到开头，当它们相遇时，视为该 Page 已满


![[slottedpagesfigure.png]]


### Tuple 结构

元组本质上是一个字节序列，DBMS 将这些字节解释为属性类型和 Value。

- **Tuple Header:** Contain meta-data about tuple
	- DBMS's concurrency control protocol 的可见信息
	- Bit Map for `NULL` values
	- 注意 DBMS 不在这里存储有关数据库架构的元数据
- **Tuple Data:** Actual data for attributes
	- Attribute 通常按照创建表时指定的顺序存储
- **Unique Identifier:**
	- 每个元组都分配有一个独特标识
	- 最常见： `page_id + (offset or slot)`

!!! tip "Denormalized Tuple Data 非规范元组数据"
	如果两个表是相关的，那么 DBMS 可以预先连接它们，即将它们表现在同一个 Page 中进行处理。这将使得读取速度非常快，但也会带来更高的更新成本。