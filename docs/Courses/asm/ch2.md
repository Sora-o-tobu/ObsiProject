
# The Microprocessor and its Architecture

## Internal Mircroprocessor Architecture

### Registers

Intel IA-32 的基础执行环境有如下寄存器：

![[ch2_1.png]]

其中，图片下半部分的浮点寄存器和向量寄存器为 x87 新增的。

!!! info "注意到浮点控制 Flag 比 EFLAGS 对了一个 Tag，这是因为浮点计算相比整型更加复杂，需要标识舍入方案等"

演变到如今 64-bit mode，它的执行环境如下：

![[ch2_2.png]]

如下表格更直观的比较了二者的寄存器差异，其中深色部分表示存在不同：

![[ch2_3.png]]

AX, BX, CX, DX, BP, SP, SI, DI 是自 x86 开始即有的 **General-Purpose Registers**，它们在设计之初有各自的作用：

| Register | Name              | Commonly used as                                                                                                                 |
| -------- | ----------------- | -------------------------------------------------------------------------------------------------------------------------------- |
| `A`      | Accumulator       | Return value, especially the sum of arithmetic operations<br>80x86 对于 A 系列寄存器保留了单独的操作码来表示是什么运算，从而减少指令的译码长度；而用其它寄存器，则需要指示操作码以及寄存器 |
| `B`      | Base Index        | Starting point of an array or list structure<br>通常存储一个对象的偏移地址                                                                    |
| `C`      | Counter           | Used by loops<br>常用作循环计数器或一些指令的计数器（如 `REP`）                                                                                      |
| `D`      | Data              | Extended space for accumulator<br>常存储溢出的部分结果数据，例如乘法的高位结果以及除法的除数                                                                  |
| `BP`     | Base Pointer      | Pointer to address of current stack frame<br>尤其是函数调用的时候                                                                          |
| `SP`     | Stack Pointer     | Pointer to address of last bytes PUSHed to memory 指向栈顶的偏移                                                                        |
| `SI`     | Source Index      | Starting point of unbounded stream data, especially a string<br>通常在字符串操作指令中指向 source string 数据的开头，但也用作一般用处                       |
| `DI`     | Destination Index | Ending point of unbounded data, especially in slicing operations<br>通常在字符串操作指令中指向 destination string 数据的开头，但也用作一般用处              |

大部分 x86 寄存器都有多粒度的特性，即可以划分为多个 partial register，对于 A, B, C, D 系列寄存器，其命名规则如下（以 A 为例）：

![[ch2_4.png]]

上图同时也说明了该寄存器的修改规则，即只有修改 32-bit 的 partial register `eax` 时，`rax` 的 bit 32-63 会被置 0；其余修改时余位不变。

特别说明的是，`ah` 和 `al` 这两个高低 8-bit 寄存器只有 A, B, C, D 拥有。

!!! info "BP, SP, SI, DI 其实也有低八位，格式为 BPL 等，但是这只在 64-bit mode 下可用"

Intel，AMD，VIA 的 CPU 都不支持重命名 partial register，因为它会担心这是一个虚假的数据依赖，以防用户要利用 partial 的特性进行编程，以如下为例，假定要连续对两处内存地址进行数据修改，其中右侧的第二次修改的是 16-bit 的数据，如果都用 A 系列的寄存器，右边的 `AX` 就不能被 CPU 的寄存器重命名所修改，从而导致 stall：

![[ch2_5.png]]

**Segment Registers** 共有六个：

- **CS  (code):** 代码段，存储程序的代码
- **DS (data):** 数据段，存储程序大部分数据
- **ES (extra):** 额外数据段，存储一些 destination data
- **SS (stack):** 栈
- **FS & GS:** 80386-Core2 新增的额外段寄存器，可通用

64-bit mode 下，memory 的管理方式从分段管理变为分页管理，`DS`, `ES`, `SS` 被忽视（置 0），即便我们尝试对其修改也可能不起作用：

![[ch2_8.png]]

除了以上的 General-Purpose Registers 和 Segment Registers，我们还有一些 **Special-Purpose Registers**，包括 `RIP`, `RSP`, `RFLAGS` 等：

- **RIP** 指向段中下一条指令的地址，即 Instruction Pointer
- **RSP** 指向栈底，即 Stack Pointer
- **RFLAGS** 表示微处理器的状态，用来控制 operation 的行为
	- 分为状态标志、控制标志以及一些信息

![[ch2_6.png]]

状态标志有：

- **C (bit 0):** Carry Flag 算术运算或逻辑运算产生进位、借位则为 1
	- 两数相加产生进位时 CF 置 1
	- 两数相减产生借位时 CF 置 1
	- 两数相乘乘积超过被乘数宽度时 CF 置 1
	- 移位指令最后移出的那一位保存在 CF 中
- **Z (bit 6):** Zero Flag 算术运算或逻辑运算的结果为 0 则为 1
- **S (bit 7):** Sign Flag 算术运算或逻辑运算的结果的符号位
	- 即运算结果的最高位
- **O (bit 11):** Overflow Flag 运算结果超出机器可表示的范围则为 1
	- 当两正数相加变负数时 OF 置 1
	- 当两负数相加变正数时 OF 置 0
	- 当两数相乘乘积宽度超过被乘数宽度时 OF 置 1（此时 CF 也置 1）
	- 当仅移动一位，且移位前最高位不等于移位后最高位时 OF 置 1
- **P (bit 2):** Parity Flag 运算结果低 8 位中二进制 1 的个数为偶数时为 1
- **A (bit 4):** Auxiliary Flag 保存 BCD 运算中途的 carry 和 borrow
	- 若执行加法指令时第 3 位向第 4 位产生进位则 AF 置 1
	- 若执行减法指令时第 3 位向第 4 位产生借位则 AF 置 1

!!! example "对于如下 BCD 运算，CF 只保存最后的进位，中间的进位由 AF 保存，指示需要 +6 调整"
	![[ch2_7.png]]

控制标志有：

- **D (bit 10):** Direction Flag 控制字符串操作指令的递增/递减方向
- **I (bit 9):** Interrupt Flag 控制是否允许硬件中断
- **T (bit 8):** Trap Flag 设置 CPU 运行模式
	- 当 `TF=1` 时，CPU 进行单步模式
	- 当 `TF=0` 时，CPU 进行常规模式
- **VM (bit 17):** Virtual Mode 在保护模式下，选择 Virtual Mode Operation
- **IOPL (bit 12, 13):** 在保护模式下，选择 I/O 设备的特权阶级
- **NT (bit 14):** Nested Task 在保护模式下，当前 Task 是在另一个 Task 的嵌套中则为 1
- **AC (bit 18):** Alignment Check 如果 AC 置 1，且 CPU 的对齐检查机制开启，则会对访问地址进行对齐检查，失败则触发异常
	- 例如，访问 `int a`(4 Bytes) 时，地址应该为 4 的倍数，例如 `0x1000`，此时访问 `0x1002` 就会抛出异常
- **RF (bit 16):** Resume 控制 CPU 在单步调试或断点后如何继续执行
- **ID (bit 21):** Identification 用于测试 CPU 是否支持 `CPUID` 指令，如果支持，则该位可以被软件读写；如果不行，则写这个位无效
- **VIF (bit 19):** Virtual Interrupt Flag 在 Virtual-8086 Mode 和多任务环境下的中断标志
- **VIP (bit 20):** Virtual Interrupt Pending 表示是否有虚拟中断正在等待处理

!!! note "早期 CPU 可以通过观察 RFLAGS 中哪些位可以被修改来判断 CPU 属于哪一代"

### Operating Mode

!!! quote ""
	![[ch2_9.png]]

Long Mode（在 Intel 中对应 *IA-32e*）是 Legacy Protected Mode 的拓展，它包含了两个子模式 *64-Bit Mode* 和 *Compatibility Mode*：

- **64-Bit Mode** 支持 64-bit 架构的所有特性和寄存器拓展
- **Compatibility Mode** 支持现存 16-bit 和 32-bit 应用的二进制兼容

!!! warning "Long Mode 并不支持 Legacy Real Mode 和 Legacy Virtual-8086 Mode，因此不能简单抛弃 Legacy Mode"

除此之外，还有一个特殊的模式 **System management mode（SMM）**，它最初是用来适配平台的固件以及特定的底层硬件驱动的。在 AMD64 架构下，不同 Operating Mode 的切换如下：

![[ch2_10.png]]

!!! note "`RSM` 表示退出 SMM，从哪里进入 SMM 的就会退回到哪个模式"

### Memory Management

Memory Mangement 通常需要满足 *Relocation*，*Protection*，*Sharing* 三个要求：

|要求|含义|
|---|---|
|**重定位（Relocation）**|程序可以在内存中任何位置装入和执行，需要地址映射机制。|
|**保护（Protection）**|防止一个进程访问或修改另一个进程的内存空间。|
|**共享（Sharing）**|允许多个进程安全地共享相同的代码或数据。|

其中，**Segmentation** 和 **Paging** 是最主流的两种 Memory Management Schemes，这里简要对二者进行一些对比：

|对比项目|**分段（Segmentation）**|**分页（Paging）**|
|---|---|---|
|**大小（Size）**|段的大小**可变**，由用户或编译器决定（例如：代码段、数据段、堆栈段）。|页的大小**固定**（例如 4KB、8KB），由硬件决定。|
|**碎片（Fragmentation）**|容易产生**外部碎片**（因为不同段大小不一，内存分配不连续）。|容易产生**内部碎片**（因为最后一页可能没被完全用完）。|
|**表结构（Tables）**|使用**段表（Segment Table）**，记录每个段的基址和长度，查找速度较快。|使用**页表（Page Table）**，记录页号对应的物理块号。查找相对较慢，但**TLB（Translation Lookaside Buffer）**可加速。|

=== "Legacy-Mode Memory Management"
	![[ch2_11.png]]
=== "Long-Mode Memory Management"
	![[ch2_12.png]]

接下来，我们会使用如下几个 terms 来指代各种不同地址：

- **Effective Addresses** or **Segment Offsets**
	- 段内偏移地址
- **Logical Addresses**
	- 逻辑地址，表示形式类似 `Seg:Offset`
- **Linear Addresses**
	- 线性地址，相当于逻辑地址的转译，是连续的地址
	- 如果启用了 Paging，Linear Addresses 相当于虚拟地址 Virtual Addresses
- **Physical Addresses**
	- 物理地址，在 Memory 中的真实内存布局

#### Real Mode Memory Addressing

Real Mode 的地址空间只有 1MB 大小，即 $2^{20}$ byte。它的 Linear Address 的计算为：

$$
\text{Seg} \times 10h + \text{Offset}
$$

!!! tip "乘上 $10h$，等价于左移 4 位"
	Real Mode 中，段寄存器和偏移寄存器都是 16-bit 的，因此偏移后相加得到的地址是 20-bit 的

1 MB Address **Wrap-Around** 是这个设计很经典的问题。当 Linear Address 计算最高位进位时，物理地址发生 overflow，反而变得很小，这也被称为回环。可以考虑如下左右两个例子：

![[ch2_13.png]]


由于有相当数量的商业软件特意使用了这个特性，后续的 8086/8088/80186 CPU 都保留了这点。

#### Protected Mode Memory Addressing

在保护模式下，段寄存器不再单纯存储段地址，而是存储指向 **Descriptor Table** 中的某一描述符的 **Selector**。

例如对于指令 `MOV [BX], AX`，它的访存流程如下：

![[ch2_17.png]]

**Descriptor** 描述了一个内存段的地址，长度以及访问权限，它的结构随着架构的升级变更如下：

![[ch2_14.png]]


其中 **Base** 部分指定了这个段的起始位置，**Limit** 部分指定了这个段的长度（也可以说是结束部分）。**G** 代表了 Granularity Bit，它决定了该段 Limit 的缩放程度：

- 当 **G=0**，段的长度为 $(\text{limit}+1)\ \text{bytes}$
	- 此时 limit 的范围为 0 - $2^{20}-1$
- 当 **G=1**，段的长度为$(\text{limit}+1)\times 4K\ \text{bytes}$
	- 也可以认为将 limit 替换为 $\text{limit}<<12 + 0xFFF$
	- 此时 limit 的范围为 0FFFH - $2^{32}-1$

**【Example】** 有一个描述符，base address = 1000,0000H, limit = 01FFH，求 G 分别为 0 和 1 时的段起始位置和结束位置：

- 当 **G=0**
	- Starting Location = 1000,0000H
	- Ending Location = 1000,0000H + 01FFH = 1000,01FFH
- 当 **G=1**
	- Starting Location = 1000,0000H
	- Ending Location = 1000,0000H + 01F,FFFFH = 101F,FFFFH

1 Byte 大小的 **Access Rights** 指定了这个段的权限控制：

![[ch2_15.png]]

其中 *DPL* 字段共 2-bit，指定了该段的 Priviledge-Level。特权等级共分 4 级，按照数字大小权限依次降低：

![[ch2_16.png]]

Descriptor Tables 分为 **Global-Descriptor Table（GDT）**、**Local-Descriptor Table（LDT）**、**Interrupt-Descriptor Table（IDT）** 三类，其中 LDT 需要通过访问 GDT 中的 entry 来间接访问，这一点我们会在稍后谈及。

**Selector** 存储在 16-bit 的段寄存器中，它的结构如下：

![[ch2_18.png]]

**TI** 指定了是要访问 GDT 还是 LDT；**RPL** 指定了此次请求的特权阶级。

除此之外，段寄存器 `CS` 中最低两位不为 RPL，为 **Current Privilege-Level（CPL）**，CPL 的值永远为 CPU 当前的特权阶级。综合下来，关于 Descriptor 和 Selector 中的三种特权阶级表示位的关系与区别如下：

![[ch2_19.png]]

在进行数据访问的时候，我们会对权限进行检查：

=== "Accessing Data Segments"
	当访问数据段时，我们要求 CPL 和 RPL 的权限都**高于**该段的 DPL：
	
	![[ch2_20.png]]
=== "Accessing Stack Segments"
	当访问栈段时，我们要求 CPL 和 RPL 的权限都**等于**该段的 DPL：
	
	![[ch2_21.png]]

为了存储 Descriptor Tables 的基地址，处理器中有 4 种用户不可见的寄存器：

- **GDTP** and **IDTR**：存储 GDT 和 IDT 的 Base 和 Limit，在进入保护模式之前就被加载
- **LDTR** and **TR**：指向 GDT 中特殊的描述符
	- 例如 LDTR 指向 GDT 中的指向 LDT 的描述符

!!! note "还有一些不可见的寄存器用作 Descriptor Cache"

当需要发生进程切换时，LDTR 会加载新任务对应的 selector 值，从而利用 LDT 实现扩展内存空间。

![[ch2_22.png]]

!!! info "左下角对 index 乘 8 是因为 Descriptor Table 中一个 entry 是 8 Byte"

??? note "图中 *gdtr or ldtr* 部分的具体实现如图"
	![[ch2_23.png]]

在保护模式下，分段内存模型 Segmented-Memory Models 有两种：

|模型|说明|特点|
|---|---|---|
|**多段模型 (Multi-Segmented Model)**|把内存分成多个独立的段，比如代码段、数据段、堆栈段等。每个段有自己的基址和界限。|✅ 每个段可以独立管理、保护与共享。❌ 地址转换更复杂，软件需要管理多个段寄存器。|
|**平坦模型 (Flat-Memory Model)**|把所有段的基址都设为 0，界限设为整个地址空间长度（例如 4GB）。这样所有内存都连续可寻。|✅ 看起来就像“没有分段”，只有一个连续的线性地址空间。✅ 编程更简单，现代 OS（如 Linux、Windows）大多采用这种方式。|

![[ch2_24.png]]


#### Memory Paging

Paging 将内存划分为统一大小的 Page，通常：

- 在 **long mode**, physical pages 长度可为 4-KByte, 2-MByte, 1-GByte
- 在 **legacy mode**，physical pages 长度可为 4-KByte, 2-MByte, 4-MByte

与计组中学到的分页稍有区别的是，现实 CPU 通常会采用 Multi-level Paging，而不是只通过一个 Page Table 就能转换虚拟地址和物理地址。这是因为 Single-level 往往占用太多内存。

例如，对于 64-bit computer，假定我们设置 4KB page size, 4GB physical memory, 4Byte per page table entry，那么 $\text{Page Offset}= \log_2 4K=12\ bits$，那么 $\text{Virtual Page Number} = 64-12=52\ bits$，一个 entry 有 4B，那么整个 Page Table 占用内存 $2^{52}\times 4= 2^{54}\ bytes$，这已经是物理内存 4GB（$2^{32}$） 的不知道多少倍了。

现在我们使用 Multi-level Paging 技术。通常，我们设置一个 Page Table 的大小等同于一个 Page 的大小，对于该例即为 4KB。此时一个 Page Table 共有 4KB / 4B = 1024 个 entries。

1024 个 entries 意味着我们只需要 10-bit 就能对一个 Page Table 进行索引，那么层数计算则为 $\#\text{ of Level}= \frac{52}{10}\ \text{ceiled is } 6$，即共需要 6 层，其索引结构类似如下：

![[ch2_25.png]]

!!! abstract "Key Points"
	- 所有 Page Table 的 entry size 均需相同
	- Page Table 由内到外分别称为
		- Page Table, PT
		- Page Directory, PD
		- Page Directory Pointer Table, PDPT

|🌟 **类别**|🟢 **优点 (Pros)**|🔴 **缺点 (Cons)**|
|---|---|---|
|**内存占用**|仅为**实际使用的虚拟地址空间**分配页表，节省大量内存|页表结构本身更复杂，需要额外的目录页表，占用少量额外空间|
|**稀疏地址空间支持**|对于**稀疏内存使用的进程**非常高效，不会为空洞地址范围浪费页表项|若进程使用地址空间非常密集，多级页表节省的空间优势不明显|
|**可扩展性**|适用于大型虚拟地址空间（如 32 位、64 位系统）|随层数增加，页表查找过程更长、更慢|
|**访问速度**|——|每增加一级页表，就需要**多一次内存访问**才能完成地址转换|
|**性能优化**|可与 **TLB（Translation Lookaside Buffer）** 配合使用，显著减少查找延迟|若 TLB 命中率低，会出现明显性能下降|

Paging 为 OS 提供了将虚拟地址转换为物理地址（**Relocation**）的抽象层，因此不同应用可以使用各自隔离的虚拟地址空间（**Protection**），多个应用还可以通过 Shared Mapping 共享同一个物理页（**Sharing**）。

与分段模式相同，Paging 也需要做一些权限保护，这些用于维护权限的位处于页表的 Entry 中。例如，Page-Directory Entry 的结构如下：

![[ch2_26.png]]

!!! note "都是 0 表示高权限等级，1 表示低权限等级（这块能 Write，说明这块权限等级低）"

在 Multi-Paging 下，一个物理页的最终权限是导向它的 PDE 和 PTE 权限位的 **AND**：

$$
\text{Page Attribute = PDE Attribute \& PTE Attribute}
$$

Paging 使用 CR0-CR4 几个 Control Registers 来对分页进行控制，它们的结构如下：

![[ch2_27.png]]

- **Control Register 0:** 主要包括分页相关控制位
	- PE(bit 0) 是否启用保护模式。当 PE = 1 时会从 Real Mode 进入 Protected Mode
	- PG(bit 31) 是否启用分页。该位在进出 Long Mode 时被设置
- **Control Register 2:** 保存发生 Page Fault 时的 Linear Address
	- Handler 通过读取 CR2 来确定哪个地址导致了错误，从而执行缺页调入、权限检查、进程终止等操作
- **Control Register 3:** 存储页目录的 Physical Address
	- Page-Directory Base: 页目录基地址
- **Control Register 4:** Pentium 开始新增的控制寄存器，拓展了原有的分页架构控制
	- PSE(bit 4) 是否启用大页拓展。开启后*允许* PDE 直接映射 4MB 的大页
		- 当 `CR4.PSE=1` and `PDE.PS=1` 时，该 PDE 映射到一个 4MB Page
		- 当 `CR4.PSE=1` and `PDE.PS=0` 时，该 PDE 映射到一个 4KB Page
	- PAE(bit 5) 是否启用物理地址拓展。开启后允许物理地址空间超过 4GB
		- 例如 36-bit 物理地址，可寻址 64GB 内存；此时 Linear Address 还是 32-bit
		- PAE 启用的条件下允许映射 2MB Page，此时 PSE 位被忽略，大页达不到 4MB

=== "PSE"
	![[ch2_28.png]]
=== "PAE"
	![[ch2_29.png]]


