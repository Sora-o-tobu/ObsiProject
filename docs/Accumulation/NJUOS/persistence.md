
# 持久化

## 设备驱动程序

用户程序不应该能够直接读取寄存器或 IO 总线中的数据，我们将连接到总线上的设备“虚拟化”为文件描述符，通过系统调用的方式与设备进行数据交互。

虚拟文件举例：

- `/dev/pts/[x]` pseudo terminal，伪终端接口
- `/dev/zero`, `/dev/null`, `/dev/random`，提供特定行为的数据流设备
- `/proc/stat`，提供内核状态的只读虚拟接口，属于虚拟文件系统的一部分

!!! tip
	这些设备文件本质上并不对应磁盘上的实际文件，而是由内核通过挂载虚拟文件系统（如 devfs, procfs）动态生成，并映射到对应设备驱动程序所管理的硬件或内核资源。

但是设备不仅仅是数据，还要包括它们的**配置**，一般有两种实现方法：

- <1> 控制作为数据流的一部分(ANSI Escape Code)
- <2> 提供一种新的接口(request-response)

在 UNIX 系统中，几乎所有设备的配置操作都依赖于系统调用 `ioctl`，即 I/O Control，允许用户通过文件描述符向设备驱动发送定制化的指令。每一个设备拥有自己定义的一组 `request` 编码，这也使得 `ioctl` 成为了一个堆叠的屎山 —— 没有统一规范，极度设备相关，几乎每个驱动都重新造轮子。

```c
/// man 2 io
#include <sys/ioctl.h>
/// control device
int ioctl(int fd, unsigned long request, ...);
```

调用 `ioctl(fd, request, ...)` 会将命令 `request` 和相关参数传递到内核中对应设备驱动的控制分支逻辑中。`request` 通常是通过宏如 `_IO`, `_IOR`, `_IOW`, `_IOWR` 等构造的编码，封装了设备类型、操作编号、参数大小等信息。

!!! note "WSL1 就是因为难以兼容所有设备所以不再使用，如果想完整复刻整套 `ioctl`，不如直接弄成虚拟机"
	`ioctl` 的滥用导致设备行为不再抽象统一，成为严重的可移植性问题。WSL1 基于 syscall 转发机制，难以模拟所有 Linux 驱动行为，尤其是涉及 `ioctl` 的地方。而虚拟机则可以直接运行完整 Linux 内核，自然支持所有内核 API 与驱动逻辑，避免此类兼容性困境。

## 文件系统

在存储器之上，操作系统实现了“目录树”结构的文件系统，是我们能够以一种直观、自然、有序的方式管理操作系统里数以百计的持久化对象。

### 文件系统 API

随着操作系统的文件越来越多，逐渐形成一些约定俗成的目录结构，例如 Linux FHS ([Filesystem Hierarchy Standard](https://refspecs.linuxfoundation.org/FHS_3.0/fhs/index.html))。

文件系统中，**硬链接**允许一个文件被多个目录条目同时引用，可以相当于多个“正式入口”，本质上是多个目录项指向同一个 inode。对任意一个硬链接的修改，都会影响同一个物理文件。使用 `ls -i` 查看硬链接，会发现它们具有相同的 inode 编号：

```bash
$ ln a.txt b.txt
$ ls -i a.txt b.txt
327681 a.txt
327681 b.txt  # inode 相同，说明是硬链接
```

而**软连接**(符号链接)则相当于“快捷方式”，它创建一个新文件并将另一个文件的绝对/相对路径以文本形式存储在文件里，因此可以跨文件系统，也可以用来链接目录：

```bash
$ ln -s /etc/hosts myhosts  # 软链接，不要求目标存在
$ ls -l myhosts
lrwxrwxrwx 1 alice staff 10 Mar 10 12:35 myhosts -> /etc/hosts
```

!!! note "链接指向的位置也未必需要存在，即它可以指向一个不存在的文件"

文件是操作系统的对象，所有文件都有其元数据，或称为**属性**，一般可以通过 `ls -l / ll` 进行查看。元数据按照从左到右：

- **Type:** d(directory), l(link), p(pipe), c(char), b(block)
- **Mode:** rwx，共三组，对应 user, group, other
	- 例如 0o755 = rwx(111) r-x(101) r-x(101)
- **Links:** 引用计数，包括硬链接、目录(`..`)

```bash
$ ls -l example.txt
-rw-r--r-- 1     alice staff 1024 Mar 10 12:34 example.txt
# mode     link  owner group size modified     name
```

一个**块设备**中可能已有目录结构，可以通过 `mount` 命令将其挂载到已有的目录中：

```bash
$ mount /dev/sdb1 /mnt/usb
$ ls /mnt/usb  # 查看挂载后的内容
```

!!! warning "将不支持 FHS 的文件系统(例如 FAT)挂载到 Linux 中，所有文件默认权限为 `777`，即做最保守判断"

!!! danger "存储设备格式化其实也只是把设备中的目录树清空，但原数据仍然在盘中，如果存在镜像则可以恢复"

### 文件系统实现

文件系统为我们实现了树状的目录结构，并提供了丰富的 API 让我们能够增删改查。实际上，文件系统就是在存储系统（Block I/O）之上实现的一个支持修改查询的数据结构。

```c
struct FSObject {
	virtual ~FSObject() = default;
};

struct File : FSObject {
	vector<char> content;
};

struct Directory : FSObject {
	map<string, unique_ptr<FSObject>> children;
};
```

区别在于，底层块设备的访问单位是 **固定大小的块（block）**，也就是所谓的 **Block I/O**，不支持像内存那样的 Random Access。因此，即便我们只想要某个字节的数据，也必须将其所在的整个块读入内存，形成**读放大**（Read Amplification）；写入一个字节，也必须重写整个块，形成**写放大**（Write Amplification）。

!!! quote "现代文件系统架构（Logical Volume Manager, LVM）"
	![[ModernFileSys.png]]
	
	- 支持逻辑卷抽象、快照、分层存储等特性  
	- 在物理设备与文件系统之间引入抽象层

#### FAT

在早期（如软盘）中，存储容量较小，一个磁盘中可能仅有几百个 Sector，这时选用简单的链表结构反而更高效。链表作为数据结构的两种设计方案：

- <1> **隐式链表**：在每个数据块末尾存放指向下一个块的指针
    - ✅ 实现简单，不需额外空间
    - ❌ 块大小不是 $2^k$ 不利于硬件优化；随机访问效率极差
- <2> **显式链表（FAT）**：所有块指针集中存放在一个连续区域中
    - ✅ 结构清晰、便于缓存；支持快速查找和跳转
    - ❌ FAT 区若损坏，整个文件系统难以修复

二者对比，肯定选用方案 2，在内存中缓存一份 **File Allocation Table(FAT)** 集中保存所有指针，从而获得更高的读写性能。

为了获得更高的可靠性，可以将 FAT 多备份几个，存储设备保留首字节写明共有几个 FAT 存档：

![[FAT.png]]

!!! note "我们修改文件结构时，要写回 n 份 FAT；但是我们可以延迟写回，写放大的影响并没有很大"

在古早的 DOS 系统中，文件名长度限制规定为 8+3，即文件名最多 8 个字符，扩展名最多三个字符。为支持更长的 Unicode 文件名，现代 FAT 引入了 **Short Entry + 多个 Long Entry** 的方案：例如，如下是文件 `The quick brown.fox` 文件的元信息：

![[FATFileName.png]]

!!! tip "为了与旧系统兼容，原先的 Short Entry 也保留，但是溢出的部分用波浪线 `~n` 代替"

#### EXT

FAT 文件系统适用于容量小、需求简单的场景，例如相机 SD 卡、U 盘、UEFI 系统分区等。但随着存储需求增长，FAT 显得力不从心，UNIX 系统转向了更强大的结构化文件系统。

UNIX 文件系统是一个接近于图的树状结构，链接的存在允许“虚拟磁盘”存在多份引用，它将实际数据和目录中的文件分开，分别存储在树的 `struct node` 和 `struct edge` 中。

- **inode（index node）**：存储文件的元数据与实际数据块位置
- **目录项（edge）**：将文件名映射到 inode 编号

!!! info "也就是 UNIX 用 inode table 代替了 FAT 的功能"

例如，文件的元数据存储在 index node(inode) 中，无论你用硬链接创建多少个该文件的“入口”，对应 inode 个数不会改变，只有 edge 个数会增加。

![[ext2.png]]

EXT 的可靠性仍然是一个重要问题，因为存储 inode 的数据块损毁是相当严重的。

#### FUSE

**FUSE** 是一种将文件系统逻辑移出内核、放入用户空间的机制。它是一种协议而非格式，允许开发者在用户态以普通程序方式实现自定义文件系统。

- 核心组件：
    - **libfuse**：用户态库，实现挂载与协议处理
    - **内核模块 fuse.ko**：接收系统调用，转发给用户态程序
- 典型应用：`sshfs`（将远程 SSH 目录挂载成本地目录）、`encfs`、`rclone mount`

你可以自己决定如何存储、读取数据（如基于 JSON、数据库、压缩包、甚至网络），只要实现标准的文件系统 API（如 `getattr`, `read`, `write` 等）即可。

例如 Btrfs（B-Tree Filesystem）就基于 COW + B 树结构构建所有逻辑：

![[btrfs.png]]


!!! abstract "Everything is B-Tree: 可以实现 Resize、透明压缩、快照等等"

## 数据库系统

