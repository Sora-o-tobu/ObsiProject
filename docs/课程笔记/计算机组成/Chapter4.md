
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


| Signal Name              | Effect when =0                                    | Effect when =1                             |
| ------------------------ | ------------------------------------------------- | ------------------------------------------ |
| **RegWrite**             | 无                                                 | 将 Write Data 写入 Write Register             |
| **ALUSrc**               | ALU 第二个输入选择寄存器堆第二个输出                              | ALU 第二个输入选择指令中经过 sign-extention 后的立即数      |
| **Branch**               | PC = PC + 4                                       | PC = PC + imm(左移一位后的)                      |
| **Jump**                 | 由 `Branch` 决定 PC                                  | PC = PC + jump address(左移一位后的)             |
| **MemRead**              | 无                                                 | 读取存储器对应地址上的数据                              |
| **MemWrite**             | 无                                                 | 写入存储器对应地址上的数据                              |
| **MemtoReg<br>(2 bits)** | 00：寄存器堆的 Write data 为 ALU 的输出                     | 01：寄存器堆的 Write data 为 Memory 的输出           |
|                          | 10：寄存器堆的 Write data 为 PC + 4(Only for `jal,jalr`) | 11：寄存器堆的 Write data 为 imme（Only for `lui`） |

!!! note
	其实控制信号 `Jump` 也是 2bit 的，因为指令 `jalr` 的跳转目标地址为 `rs1+imm`


注意到上面示意中控制单元的输出还有一个 2bit 的 `ALUop` ，这其实是控制单元的一个中间输出，第一层对 `opcode` 进行译码，可以得到该指令对应的 `type` 及大部分控制信号；第二层再联合 `Funct3,Funct7,ALUop`，译码得出对应的 `ALU operation` 信号，以控制 ALU 进行何种计算。

![[2leveldecoder.png]]

不过对于 `ALU_Operation` 最终译码出来的结果，课本和课件略有差异，具体选哪个我也不知道：

| ALU Control | Function      |
| ----------- | ------------- |
| 0000        | AND           |
| 0001        | OR            |
| 0010        | Add           |
| 0110        | Subtract      |
| 0111        | Set Less Than |
| 1100        | NOR           |


| ALU Control | Function      |
| ----------- | ------------- |
| 000         | AND           |
| 001         | OR            |
| 010         | Add           |
| 110         | Subtract      |
| 111         | Set Less Than |
| 100         | NOR           |
| 101         | SRL           |
| 011         | XOR           |


对于一般的指令，有表格如下：

![[ybzlbg.png]]

??? failure "某lab中我自己设定的控制信号表，但是和理论课略有不同，没有太大参考价值"
	![[myselfcontrolsignaltable.png]]

其中 ALUop 不需要死记硬背，只要知道其最终对应的 ALU Function 是什么就够了，如：

- `ld` 对应 `add` ，因为需要将寄存器与立即数相加
- `sd` 对应 `add` ，同 `ld` 相同，需要寄存器与立即数相加
- `beq` 对应 `sub` ，因为需要将两个寄存器相减以判断是否相等
- 对于 `R-type` 指令，在第二层译码器根据 `ALUop`,`Fun3`,`Fun7` 来判断 ALU 执行的操作

实际的数据传输例子，请看课件或是 [咸鱼暄的笔记](https://xuan-insr.github.io/computer_organization/4_processor/)

!!! example "课件上的数据传输例子（有点丑说是）"
	=== "R-type"
		![[rtypedatepath.png]]
	=== "load"
		![[loaddatapath.png]]
	=== "store"
		![[storedatapath.png]]
	=== "bea"
		![[beqdatapath.png]]
	=== "jal"
		![[jaldatapath.png]]

!!! example "考试可能的题型"
	=== "S & R"
		![[midtermquestion.png]]
	=== "Only S-type"
		![[midtermonlywithS.png]]

## 流水线

### Overview

假定 CPU 对于下列操作消耗时间如下：

![[piplinedl.png]]

那么，对于各种不同的指令，它们各自执行完所需要的总时间为:

![[gzbtzlzxwsxydsj.png]]

!!! note "Latancy: 每一条指令执行需要的时间"

对于单周期处理器，最长的时钟延迟决定了处理器的时钟频率，但是最耗时指令不一定是最常用的指令，这违背了八大思想的 *Making common case fast* 。为了解决这个问题，我们引入 pipeline 以提升处理器的性能。

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
	- 硬件不支持多条指令在同一时钟周期执行
- **Data Hazards** 数据冒险
	- Need to wait for previous instruction to complete its data read/write
	- 当前指令的执行需要等待前面指令的数据结果
- **Control Hazards** 控制冒险
	- Deciding on control action depends on previous instruction
	- 指令非顺序执行而导致下一条执行的指令不是真实期望的

#### Structure Hazards

由于不同指令的各个阶段很可能在处理器中同时执行，会遇到处理器同时读写寄存器或Memory!?这显然是不被允许的，为了解决这个矛盾，在实际的流水线实现中我们需要不同阶段之间加上 **pipeline register** 来保存上一个时钟周期的数据。

!!! info "流水线寄存器和 PC 一样，都在每个时钟周期下降沿写入数据"

![[pipelineregister.png]]

对于 `load` 指令，它的 single-clock-cycle 的数据通路图如下图所示：

=== "IF"
	![[loadif.png]]
=== "ID"
	![[loadid.png]]
=== "EX"
	![[loadex.png]]
=== "MEM"
	![[loadmem.png]]
=== "WB"
	![[loadwb.png]]
	Tips: 这里对 Datapath 修正在于 `WriteRegister` 要从 `MEM/WB` 流水线寄存器中读取 


最终得到的简化的 Pipeline with control 的 Datapath 如图：

![[pipelinewithcontrolsimplified.png]]

当处理器需要插入 Bubble 时，将所有流水线寄存器的控制信号全部置 0，并阻止 PC 和 `IF/ID` 流水线寄存器的更新。

!!! success "硬件资源冲突"
	流水线执行期间，也可能发生两条及以上的指令同时对Memory发起资源请求，解决方法是将指令和数据分离存储，这样取指阶段只使用 IMEM ，访存阶段只使用 DMEM。


#### Data Hazards
当两个指令所用到的数据有先后要求时，会发生数据冒险。

为了解决这个矛盾，后来的指令要先等到上一条指令的运算结果写回到寄存器或是Memory中，才可以继续译码。那么，后来的指令的 **IF** 阶段需要 Stall ，等待期间处理器流水线会插入 Bubble 来代替空指令。

例如，对于下面两条连续指令，它们一个需要写回 x19，一个需要读取 x19，没有优化的处理器中间需要插入两轮 Bubble ，直到上一条指令的 **WB** 结束，下一条指令才能进入 **EXE** 阶段：

```asm
add x19, x0, x1
sub x2, x19, x3
```

![[datahazedexample.png]]

当然，如果 Datapath 有额外的数据通路，处理器也允许中间数据进入 **WB** 阶段之前，直接传输到下一条指令的 **EX** 阶段，这种优化名为 **Forwarding** (or **Bypassing**)：

![[datahazardsexample2.png]]

!!! example "从 Memory 读取的数据直接传入下一指令 EX 阶段(Load-use Hazard)"
	![[congmemroyduqudeshuju.png]]
	load 型指令即便使用 Forwarding 也要插入一个 Bubble。
	
	幸亏的是，下一条指令在 Stall 前并没有对寄存器堆或 MEM 作出任何操作，因此可以直接将 NOP 指令取代它后面的阶段。

对于使用 Forwarding 优化的处理器，执行下列指令过程中会发生两个数据冒险，导致总运行时间会增加两个时钟周期：

```asm
ld x1, 0(x0)
ld x2, 8(x0)
add x3, x1, x2 ; stall happened
sd x3, 24(x0)
ld x4, 16(x0)
add x5, x1, x4 ; stall happened
sd x5, 32(x0)

;Need 7+4+2 = 13 cycles if using forwarding
```

我们在写汇编代码时，将两个 `ld` 指令提前，能够使硬件有更快的运行速度：

```asm
ld x1, 0(x0)
ld x2, 8(x0)
ld x4, 16(x0)
add x3, x1, x2 ; stall happened
sd x3, 24(x0)
add x5, x1, x4 ; stall happened
sd x5, 32(x0)
```

!!! info "我们使用编译器时，汇编器一般会自动优化排列"

那么我们有个问题，在硬件层面上，处理器如何判断需要进行 Forward 操作？

我们可以把需要 Forward 的情况分为三种：

- EX Hazard
	- 下一条指令需要用到当前指令的运算结果寄存器
	- 此时 ALU 的一个输入从 `EX/MEM` 中转接
- MEM Hazard
	- 下下条指令需要用到当前指令的运算结果寄存器
	- 此时 ALU 的一个输入从 `MEM/WB` 中转接
- Load-use Hazard
	- 当前指令为 `load` ，且下一条指令需要用到其 `load` 读取的数据

**EX Hazard:**

```c
if(EX/MEM.RegWrite
  and (EX/MEM.RegisterRd != 0)
  and (EX/MEM.RegisiterRd == ID/EX.RegisterRs1))
  ForwardA = 10;
if(EX/MEM.RegWrite
  and (EX/MEM.RegisterRd != 0)
  and (EX/MEM.RegisiterRd == ID/EX.RegisterRs2))
  ForwardB = 10;
```

**MEM Hazard:**

```c
if(MEM/WB.RegWrite
  and (MEM/WB.RegisterRd != 0)
  and not(EX/MEM.RegWrite and EX/MEM.RegisterRd != 0)
  and (EX/MEM.RegisterRd == ID/EX.RegisterRs1)
  and (MEM/WB.RegisterRd == ID/EX.RegisterRs1))
  ForwardA = 01;
if(MEM/WB.RegWrite
  and (MEM/WB.RegisterRd != 0)
  and not(EX/MEM.RegWrite and EX/MEM.RegisterRd != 0)
  and (EX/MEM.RegisterRd == ID/EX.RegisterRs2)
  and (MEM/WB.RegisterRd == ID/EX.RegisterRs2))
  ForwardB = 01;
```

对于不同的控制信号 `Forward(A/B)` ，ALU 的操作数选择如下表：

| Mux Control  | Source | Explanation                                                      |
| ------------ | ------ | ---------------------------------------------------------------- |
| Forward = 00 | ID/EX  | This ALU operand comes from the register file                    |
| Forward = 10 | EX/MEM | This ALU operand comes from the prior ALU result                 |
| Forward = 01 | MEM/WB | This ALU operand comes from data memory or an earlier ALU result |

同时存在 EX Hazard 和 MEM Hazard 时只执行 EX Hazard（选择 Most Recent 的冒险处理）。例如：

```asm
add x1, x1, x2 ; Instruction 1
add x1, x1, x3 ; Instruction 2
add x1, x1, x4 ; Instruction 3
```

其中，指令 1,3 之间发生 MEM Hazard；指令 2,3 之间发生 EX Hazard，那么处理器只需要处理 EX Hazard 即可，这也是为什么上面 MEM Hazard 的判断条件这么写。

除此之外，还有特殊的 **Load-Use Hazard** ，它的判断标准为：

```c
ID/EX.MemRead and 
((ID/EX.RegisterRd == IF/ID.RegisterRs1) or 
 (ID/EX.RegisterRd == IF/ID.RegisterRs2))
```

#### Control Hazards

对于跳转指令，下一条指令地址究竟是 PC+4 还是 PC+address 起码要等到 EX 阶段后，需要插入三个Bubble🤔

一种解决方法是在 `branch` 指令的ID阶段就提前对两个源寄存器进行比较（需要硬件支持），从而只需要一个Bubble就可以完成下一条指令地址的选择。

```asm
36: sub x10, x4, x8
40: beq x1, x3, 32 ; PC-relative branch
   #Bubble         ; to 40 + 16*2 = 72
44: and x1, x2, x5 ; 不执行
48: orr x1, x2, x6 ; 不执行
    ...            ; 不执行
72: ld x4, 50(x7)
```

但是对于更长的流水线来说，这种处理方式可能不能平稳运行，是 unacceptable 的。

更多时候，我们使用预测来解决控制冒险：

- Static Branch Prediction 
	- 为特定指令设置默认预测命中
	- 例如，对于 loop 和 if-statement
		- 向回跳转默认命中
		- 向下跳转默认不命中
- Dynamic Branch Prediction
	- 基于运行历史来预测是否命中

即便预测没有命中，最早的指令也才进行三个阶段到 EXE，并没有对整体状态进行更新（写回寄存器或写入内存等），恢复状态是较为方便的。

!!! example "2 bit Predictor"
	![[2bitpredictor.png]]

## Exception & Interrupt

- 中断来自于 CPU 外部的 I/O controller，通常用于信息的输入与输出
- 异常来自于 CPU 内部的意外控制流，是与CPU当前执行指令相关的同步事件

!!! note "区分中断和异常"
	**一种角度**：异常一般是同步的，而中断一般是异步的
	
	**另一种角度**：异常是由程序引起的，而中断一般是由外部设备引起的

在计组实验中，异常以非法指令、`ecall` 等形式出现，而中断以外设中断等形式出现。

CPU 遇到异常时，需要进行异常处理，机器模式下的一般步骤如下：

- **Save the Context:** 保存当前 CPU 状态。包括PC，寄存器等
- **Trap Handler:** 跳转到异常处理程序。这个程序一般是预先定义好的，其 PC 地址放在 `mtvec` 寄存器中
	- 同时更新机器模式异常原因寄存器 `mcause`，更新机器模式异常PC寄存器 `mepc`，更新机器模式异常值寄存器 `mtval`，更新机器模式状态寄存器 `mstatus`
- **Return:** 处理完异常后，恢复 CPU 状态，继续执行

有一些系统级别的控制指令，其 `opcode` 均为 `1110011`。这些指令包括：

- `CSR` 指令
- `ecall` 和 `ebreak` ，用于异常处理
- `mret` 和 `sret` ，用于从异常处理中返回

`mtvec` 寄存器格式：

```
31                     1    0
+----------------------+----+
|         BASE         |MODE|
+----------------------+----+
```

- `MODE=0`，异常相应时，跳转到 `BASE`
- `MODE=1`，异常相应时，跳转到 `BASE`
- `MODE=1`，中断响应时，跳转到 `BASE+4*Cause`
	- 其中 `CASUE` 为中断对应的异常编号

## Instruction-Level Parallelism

### Static Multiple Issue

静态多发射，由编译器完成发射相关判断。编译器会将同时执行不会冲突的指令分成不同 **issue packets**，一起发射，从而增加流水线的效率。

!!! info "Speculation 猜测"
	不管是静态多发射还是动态多发射，编译器或处理器都会“猜测指令”的行为，以尽早消除掉该指令与其它指令之间的依赖关系。

在经典双发射例子中，ALU/Branch 类型指令可以和 Load/Store 同时执行：

![[twoissuepackect.png]]

!!! example "Example 1"
	![[dualissueex1.png]]
	该例中编译器对指令顺序进行了重排，从而提高了Instruction Per Cycle。但是并非无限制打包，同一个包内指令数据不能有依赖关系。

!!! example "Example 2"
	![[dualissueex2.png]]
	配合循环展开，减少数据竞争，得到IPC=1.75，已经很接近理想数值 2 了。


### Dynamic Multiple Issue

动态多发射在动态执行过程中由硬件完成发射相关判断，允许CPU乱序执行指令。

```asm
ld x31, 20(x21)
add x1, x31, x2
sub x23, x23, x3
andi x5, x23, 20
```

当 `add` 在等待 `x31` 数据时，CPU可以先执行 `sub` 的前几个阶段。

为什么不只用编译器优化的静态多发射呢？

- <1> 不是所有的 stalls 是可预测的
	- e.g., cache misses
- <2> 跳转指令的结果是动态决定的
- <3> 对于一个 ISA，其不同执行方法有着不同的 Latency 和 Hazard

