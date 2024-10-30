
# Chapter 4. The Processor

## 单周期处理器

单周期处理器在一个时钟周期执行一个指令，即 CPI = 1，而时钟周期取决于最长指令的执行时长。

在处理器中，一条指令的执行通常分为四到五个阶段，分别是取指、译码、执行、访存、写回。其中，对于任何指令，前两个步骤都是完全相同的，而后几步则取决于该指令的类别。

- **取指令IF**：根据程序计数器PC中的指令地址，从存储器中取出一条指令；同时，PC根据指令字长度自动递增产生下一条指令所需要的指令地址（PC+4），但遇到*地址转移*指令时，则控制器把转移地址送入PC
- **指令译码ID**：对取指令操作中得到的指令进行分析并译码，确定指令要完成的操作并产生相应的控制信号
- **指令执行EXE**：根据操作控制信号执行指令动作，然后转移到结果写回状态
- **存储器访问MEM**：所有需要访问存储器的操作都在这一步骤执行，包括 `load` 和 `store`
	- `beq` 是为什么？
- **结果写回WB**：将执行的结果或访问存储器得到的数据写回相应寄存器中

!!! info "不同指令的不同阶段"
	![[不同指令的不同阶段.png]]

一个较完整的 Datapath with Control 示意如下：

![[datapathwithcontrol.png]]

我们一共定义 7 个控制信号，用于控制 Datapath 进行正确的指令操作：


| Signal Name              | Effect when =0                | Effect when =1                        |
| ------------------------ | ----------------------------- | ------------------------------------- |
| **RegWrite**             | 无                             | 将 Write Data 写入 Write Register        |
| **ALUSrc**               | ALU 第二个输入选择寄存器堆第二个输出          | ALU 第二个输入选择指令中经过 sign-extention 后的立即数 |
| **Branch**               | PC = PC + 4                   | PC = PC + imm(左移一位后的)                 |
| **Jump**                 | 无                             | PC = PC + jump address(左移一位后的)        |
| **MemRead**              | 无                             | 读取存储器对应地址上的数据                         |
| **MemWrite**             | 无                             | 写入存储器对应地址上的数据                         |
| **MemtoReg<br>(2 bits)** | 00：寄存器堆的 Write data 为 ALU 的输出 | 01：寄存器堆的 Write data 为 Memory 的输出      |
|                          | 10：寄存器堆的 Write data 为 PC + 4  | 无                                     |

!!! note "控制信号只有 `MemtoReg` 是两位的，这是因为 Jump 指令要把 PC+4 存入寄存器"

注意到上面示意中控制单元的输出还有一个 2bit 的 `ALUop` ，这其实是控制单元的一个中间输出，其经过两层译码后才能得到对应的 `ALU operation` 信号，以控制 ALU 进行何种计算。

![[2leveldecoder.png]]

对于一般的指令，有表格如下：

![[ybzlbg.png]]

其中 ALUop 不需要死记硬背，只要知道其最终对应的 ALU Function 是什么就够了，如：

- `ld` 对应 `add` ，因为需要将寄存器与立即数相加
- `sd` 对应 `add` ，同 `ld` 相同，需要寄存器与立即数相加
- `beq` 对应 `sub` ，因为需要将两个寄存器相减以判断是否相等
- 对于 `R-type` 指令，在第二层译码器根据 `ALUop`,`Fun3`,`Fun7` 来判断 ALU 执行的操作

实际的数据传输例子，请看课件或是 [咸鱼暄的笔记](https://xuan-insr.github.io/computer_organization/4_processor/)

## 流水线

### Overview

假定 CPU 对于下列操作消耗时间如下：

![[piplinedl.png]]

那么，对于各种不同的指令，它们各自执行完所需要的总时间为:

![[gzbtzlzxwsxydsj.png]]

!!! note "Latancy: 每一条指令执行需要的时间"

对于单周期处理器，最长的时钟延迟决定了处理器的时钟频率，但是最耗时指令不一定是最常用的指令，这违背了八大思想的 *Making common case fast* 。为了解决这个问题，我们引入 pipelining 以提升处理器的性能。

1. **IF**: Instruction fetch from memory
2. **ID**: Instruction decode & register read
3. **EX**: Execute operation or calculate address
4. **MEM**: Access memory operand
5. **WB**: Write result back to register

处理器执行一条指令共有五个阶段，在流水线中，每个阶段对应一个状态，为了更高效利用处理器，在任一时钟周期我们期望每个阶段都有指令正在执行，形成理想流水线如下：

![[pipelineoverview.png]]

而时钟周期由最长时钟延迟的阶段决定。

!!! note "CPI 从 1 变成了 5 !"

仍然使用上述各种操作耗费时间，下图对比单周期处理器和流水线处理器流程图：

![[singlecyclec2pipeline.png]]

可见，各个阶段耗时越平衡，流水线的性能提升越大。

RISC-V 指令集较方便进行流水线设计：

- All instruction are 32-bits
	- 更方便 fetch 和 decode 的运作
- 指令格式更少，也更规律
	- 更方便 decode 和 read register
- 只在 load 或 store 指令中操作 data memory 而不会将存取的结果做进一步运算，这样就可以将 `MEM` 放在比较后面的位置；如果还能对结果做运算则还需要一个额外的阶段，此时流水线的变长并没有什么正面效果

!!! note "对于流水线，指令的 Latancy 并没有提升"

### Hazards

Hazards(冒险)是阻止指令进入下一阶段的情况，可分为：

- **Structure Hazards** 结构冒险
	- A required resource is busy
- **Data Hazards** 数据冒险
	- Need to wait for previous instruction to complete its data read/write
- **Control Hazards** 控制冒险
	- Deciding on control action depends on previous instruction

#### Structure Hazards

由于不同指令的各个阶段很可能在处理器中同时执行，会遇到处理器同时读写寄存器或Memory!?这显然是不被允许的，为了解决这个矛盾，在实际的流水线实现中我们需要不同阶段之间加上 pipeline register 来保存下一个时钟周期要写入该阶段的数据。

!!! info "流水线寄存器和 PC 一样，都在每个时钟周期下降沿写入数据"


#### Data Hazards
当两个指令所用到的数据有先后要求时，会发生数据冒险。

为了解决这个矛盾，后来的指令要先等到上一条指令的运算结果写回到寄存器或是Memory中，才可以继续译码。那么，后来的指令的 **IF** 阶段需要 Stall ，等待期间处理器流水线会插入 Bubble 来代替空指令。

例如，对于下面两条连续指令，它们一个需要写回 x19，一个需要读取 x19：

```asm
add x19, x0, x1
sub x2, x19, x3
```

![[datahazedexample.png]]

当然，如果 Datapath 有额外的数据通路，处理器也允许中间数据进入 **WB** 阶段之前，直接传输到下一条指令的 **EX** 阶段：

![[datahazardsexample2.png]]

!!! example "从 Memory 读取的数据直接传入下一指令 EX 阶段"
	![[congmemroyduqudeshuju.png]]

#### Control Hazards

对于跳转指令，下一条指令地址究竟是 PC+4 还是 PC+address 起码要等到 EX 阶段后🤔