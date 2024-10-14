
# Chapter 2. Instruction: Language of Machine

!!! quote "精简指令集"
	前面我们讨论过，计算机的 Performance 受每条指令执行的时间影响，而不同的 CPU 可能使用不同的指令架构集（ISA）。
	
	RSIC-V 是 UC Berkeley 大学设计的第五代RISC芯片，其特点是开源、架构简单、模块化设计，是本课程所要讲解的 ISA。

## 寄存器

RISC-V 一共有 32 个寄存器，每个寄存器宽度均为 64 位，命名从 `x0` 到 `x31` 

!!! tip "不同于8086，RISC-V中 `1 word = 32 bit`，所以一个寄存器有 `doubleword`"

![[riscvreg.png]]

- `x0` 保持常数 `0` ，常用于函数返回时作为无意义地址的接收方： `jalr x0, 0(x1)`
	- `x0` 的读写是无效的，处理器并不会去读写它的值
- `x1` 用来保存返回地址，即 `return address` 。
	- 在调用函数前，使用 `jalr x1, FUNC_ADDR` 保存PC当前指令的下一条指令地址，即 `PC+4` 。
	- 调用的函数返回时，使用 `jalr x0, 0(x1)` 返回调用指令的下一条指令继续执行。
- `x2` 即栈指针，始终指向栈顶元素。栈从高地址向低地址增长。
- `x3` 指向全局变量和静态数据区的起始地址，称为 `global pointer` ，用于访问和管理全局变量。
- `x4` 是线程寄存器，指向当前线程的局部寄存器，即线程私有的数据区域。
- `x5-x7` 以及 `x28-x31` 是临时寄存器，通常用于存储计算的中间结果或临时变量，它们是调用者保存的寄存器。
- `x8-x9` 以及 `x18-x27` 也可以用来存储临时数据，不过它们是被调用者保存的寄存器，即被调用函数在开头和结尾分别需要入栈和出栈以恢复它们的值。
- `x10-x17` 是参数寄存器，函数调用的前八个参数会存储在这些寄存器中，但如果参数超过 8 个就需要放到栈中传递（如果参数为 8 bytes，`fp+8` 是第九个参数，`fp+16` 是第十个参数...）。同时，过程的结果也会放到这些寄存器上，对于C语言这种只有一个返回值的语言，可能只会用到 `x10` 。

## 指令格式

不同于 8086 无定长的机器码，RISC-V 的指令都是 64 bit ，且有固定格式的。

<img src="../image/riscvasm.png" style="max-height:100%"/>

!!! note "只有 `load` 和 `store` 系列可以访问 Memory"

RISC-V 指令格式如下：

![[RISCVIGSG.png]]

- `opcode`: 指 `operation` 的编码，大部分时候需要和 `funct3` 和 `funct7` 一起决定指令的种类。
- `rd`: 即 `Destination Register` ，目标寄存器。
- `funct3`: 3 bit 的 function code，相当于 additional opcode。
- `rs1`: 即 `First Source Register` ，第一个源寄存器。
- `rs2`: 即 `Second Source Register` ，第二个源寄存器。
- `funct7`: 7 bit 的 function code，相当于 additional opcode。
- `i`: 立即数，需要注意是有符号数，例如 `i[11:0]` 的范围是 $-2^{11}$ ~ $2^{11}-1$

可以观察到 I-Type 有两个条目，下面那个条目只对应 `slli`,`srli`,`srai` ，因为立即数移位操作并不可能对一个 64 位寄存器进行大于 63 位的移位操作，因此 immediate 中只有后 6 位能实际用上，因此前六位可以用来当作额外的操作码字段。

??? example "例子：对于 R 型指令 `add`"
	![[add转化机器码.png]]

RISC-V 使用 `Branch` 系列指令进行跳转，举一例演示其应用：

```asm
C CODE:
if(i==j) f = g + h;
else f = g - h;

RISC-V ASM CODE:
assume x19:f, x20:g, x21:h, x22:i, x23:j
	bne x22, x23, else # goto else if i != j
	add x19, x20, x21  # f = g + h
	beq x0, x0, EXIT   # goto EXIT
else:
	sub x19, x20, x21  # f = g - h
EXIT:
	...statement
```

同时，也可以使用无条件跳转指令 `jal` 和 `jalr` (Jump And Link Register) 来达成C语言 switch 语句的效果：

```asm
comment@
模拟 Jump Address Table
@

C CODE:
switch(k){
	case 1: f = i + j; break;
	case 2: f = g + h; break;
	case 3: f = g - h; break;
	case 4: f = i - j; break;
}

RISC-V ASM CODE:
assume x20~x25:f~k, x5:constant 4, x6:JumpTable 的首地址
	blt x25, x0, EXIT
	bge x25, x5, EXIT # if k < 0 or k >= 4, goto EXIT
	slli x7, x25, 3   # x7 = k * 8 ，因为地址表每一项都是 8 Bytes 的
	add x7, x7, x6    # x7 = Address of JumpTable[k]
	ld x7, 0(x7)      # x7 = JumpTable[k]
	jalr x1, 0(x7)    # jump
	...Next Instruction
	...statement
	...
L0:
	add x20, x23, x24 # f = i + j
	jalr x0, 0(x1)    # return Next Instruction
L1:
	add x20, x21, x22 # f = g + h
	jalr x0, 0(x1)
L2:
	sub x20, x21, x22 # f = g - h
	jalr x0, 0(x1)
L3:
	sub x20, x23, x24 # f = i - j
	jalr x0, 0(x1)
```

