
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

