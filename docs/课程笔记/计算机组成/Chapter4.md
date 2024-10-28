
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

