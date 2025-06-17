
# Instruction-Level Parallelism

## Overview

1985 年以后的处理器几乎都使用了流水线来使指令能重叠执行。指令之间的可能的重叠称为指令级并行 **Instruction-Level Parallelism**。

有关提供指令间并行度来扩展基本流水线概念的技术包括如下，以及它们分别降低了 CPI 的哪一部分：

![[ILPoverview.png]]

一个流水线处理器的 CPI 值等于基础 CPI 和因为各种 Stall 而耗费的周期之和：

Pipeline CPI = Ideal Pipeline CPI + Structural Stalls + Data Hazard Stalls + Control Stalls

*Ideal Pipeline CPI* 是对能够实现的最佳性能的度量，通过缩短上式的右边三项 Stalls，我们可以降低总流水线 CPI。

!!! note "对于一段程序，我们根据跳转指令将其分隔为一系列 Basic Block，在基本块中的指令 ILP 的机会较少"

## Data Dependences and Hazards

在硬件资源足够情况下，没有依赖的指令之间可以完全并行，不会导致任何停顿。

只要指令之间存在依赖，而且它们执行顺序接近，使得对应依赖中操作数的访问顺序改变，就会存在 Hazards。

根据指令中读访问和写访问的顺序，可以将 Data Hazards 分为三类：

- **RAW，Read After Write**
	- $j$ 试图在 $i$ 写入源位置之前读取该位置，使 $j$ 获得一个错误的旧值
	- 是最常见的冒险，对应 Data Dependence
- **WAW，Write After Write**
	- $j$ 试图在 $i$ 写入源位置之前写入该位置，使写操作执行顺序错误
	- 属于 Name Dependence 的 output dependence
	- 只有在允许多个流水级进行写操作的流水线中，或者在前一个指令 Stall 时还允许后一条指令继续执行的流水线中出现
- **WAR，Write After Read**
	- $j$ 试图在 $i$ 读取源位置之前写入该位置，使 $i$ 错误地获取新值
	- 属于 Name Dependence 的 antidependence
	- 如果某些指令在流水线早期写结果，而其它指令在流水线后期读取源位置，或者对指令重排序，就会发生 WAW 冒险

!!! example "这里假设有两条顺序的指令 $i,j$"

### Data Dependence

!!! info "也被称为 True Data Dependence"

如果以下任意一个条件成立，则称指令 $j$ 数据依赖于 $i$：

- 指令 $i$ 生成的结果可能会被指令 $j$ 用到
- 指令 $j$ 数据依赖于指令 $k$，指令 $k$ 又数据依赖于 $i$
	- 即存在依赖链

!!! note "单条指令内部的依赖如 `add x1, x1, x1` 不视为依赖"

如果两条指令数据依赖，则它们的顺序不能调换，且不能同时执行。

当通过寄存器传送数据时，我们很容易判断检测一个数据依赖关系，但当存在一些分支干扰或者编译器、硬件优化时，判断会变得略微复杂。

当通过存储地址传送数据时，两个看起来不同的地址可能指向了同一个地方，例如 `100(x4)` 和 `20(x6)`，两个看起来一样的地址也可能指向不同地方，例如 `20(x4)` 和 `20(x4)` ，这使得依赖关系的检测变得更为复杂。

### Name Dependence

当两条指令使用相同的寄存器或存储地址(作为**Name**)，但之间没有数据依赖时，就会发生名称依赖。**Name Dependence** 有两种类型：

- **<1> Antidependence**
	- 指令 $j$ 尝试对指令 $i$ 要读取的寄存器或地址进行写操作，即 WAR 冲突
	- 为了确保指令 $i$ 能够读到正确的值，必须保持指令原来顺序
- **<2> Output Dependence**
	- 指令 $i$ 和 $j$ 尝试对同一个寄存器或地址进行写操作，我们希望最后的值依赖于后一条指令 $j$，即 WAW 冲突

```asm
; Antidependence
fsd  f4, 0(x1)   ; i
addi x1, x1, -8  ; j

; Output Dependence
add t1, t2, t3   ; i
add t1, t4, t5   ; j
```

与数据依赖不同，名称依赖可以通过改变冲突的寄存器号或存储地址来使这些指令不再冲突，那么涉及的指令就可以重新排序或者同时执行。

对于寄存器重名所造成的依赖，更容易通过寄存器重命名打破依赖。可以通过编译器静态或者硬件动态处理。

### Control Dependence

控制依赖决定了一个指令相对于 Branch Instruction 的顺序，一个最简单的示例是，以下代码 `S1` 控制依赖于 `p1`，`S2` 控制依赖于 `p2` 但不依赖 `p1`：

```c
if p1
	S1;
if p2
	S2;
```

控制依赖并不是一个必须保持的关键特性，在不影响程序正确性的情况下，我们有可能希望执行一些还不应该被执行的指令以减少 Stalls，从而破坏控制依赖。

对程序正确性至关重要的两个特性是 **Exception Behavior** 和 **Data Flow**，这也是由数据依赖和控制依赖共同维护的。

- 保留**异常行为**，即改动指令执行顺序，但不能改变程序如何产生异常的。
    - 另外，对指令执行的重新排序不得导致程序中出现新的异常。
- 分支让**数据流**变得动态，因为不同基本块的数据源不一定相同。通过保留控制依赖，就能够阻止对数据流的非法变化。


## Techniques

这里先对我们计组中学过的 Pipeline 进行一个简单拓展。

在计组中，我们认为流水线各个阶段的耗时均是相同的，都为一个时钟周期。而实际上，在引入浮点计算操作后，EX 阶段的各个操作的消耗可能相差巨大，我们有必要将其细分，进一步流水线化：

![[4LevelPipeline.png]]

在 Lab5 中，我们可以看到流水线被划分为了 IF, ID, FU, WB 四级。由于 FU 阶段操作时间不同，我们引入两个概念：

- **Latency：**【上一条指令产生结果】 与【下一条指令使用结果】之间的时间间隔
	- 通常 latency = (FU Time - 1) cc
- **Initiation Interval：**两条指令发射进入同一个 FU，它们之间必须至少间隔多少个时钟周期
	- 如果 FU 完全流水化(Fully Pipelined)，那么 Initiation Interval = 1
	- 如果 FU 非流水化(Non Pipelined)，那么 Initiaion Interval = latency + 1

![[LatencyAndInterval.png]]

!!! danger ""
	=== "Integer ALU"
		 ![[IntegerALULatency.png]]
	=== "Data Memory"
		![[MemoryLatency.png]]

这种流水线存在以下潜在问题：

- 需要添加新的流水线寄存器
- 由于除法器是非流水化的，有可能发生结构冲突
	- 也因此，一个除法器内只能同时有一条指令在计算
	- 为 FU 设置 busy 位，开始执行置 1，结束时置 0
- 由于各个指令执行时间不同：
	- 可能出现一个周期内有多条指令要同时写回寄存器
		- 设计一个移位寄存器，如果当前开始执行的指令在 N 个时钟周期后要写回，则预约寄存器第 N 个位置；每个时钟周期都向左移一位，从而判断什么时候有指令要写回
	- 可能出现写寄存器顺序错乱问题，即 WAW 冲突
		- 记录每个 FU 要写回什么位置，同时利用上述移位寄存器判断当前 FU 在多久后写回。如果有 WAW，则后一条指令需等待前一条指令写入
- RAW 冲突更频繁，且造成的停顿更长
- 乱序完成，难以中断

!!! info "该流水线不会发生 WAR 冲突，前一条指令的读操作一定在后一条指令的写操作之前"

## 硬件方法

- 动态调度
	- ScoreBored
	- Tomasulo
- 动态分支预测
	- 预测器
	- 精确中断和 ROB
- 多发射
	- 超标量

### Dynamic Scheduling

#### Scoreboard 算法

**Scoreboard 算法**允许原本被 Stall 阻塞的指令继续往前运行，并且仍然维持数据流的正确性。其动态调度的【动态】体现在：

- 能解决编译时无法确定的一些数据依赖
- 简化编译器的实现，不同的架构可以对应同一种汇编的实现
	- 即写汇编代码的人不需要知道 scoreboard 的具体设计

!!! info "Scoreboard 流水线的五个阶段"
	- IF：取指令
	- IS：Issue，负责解码指令以及监视结构冲突
	- RO：Read Operands，监视数据冲突，读取操作数
	- EX：送到不同 FU 执行，访存也在这一阶段
	- WB：写回结果

我们为 Scoreboard 维护三张表：

- 指令状态表 **Instruction Status Table**
	- 记录指令的状态，在哪个阶段
- 功能单元状态表 **Functional Unit Status Table**
	- `busy`: FU 是否空闲
	- `op`: FU 正在执行什么操作
	- `Fi`, `Fj`, `Fk`: FU 的操作数对应哪个寄存器
		- `Fi` 是目的寄存器
	- `Qj`, `Qk`: FU 的操作数如果没准备好，应该从哪个 FU 中读取
	- `Rj`, `Rk`: FU 的操作数是否准备好，当所有操作数准备好时，FU 操作进入 EX 状态，记得将 `Rj` 和 `Rk` 都改为 NO
- 寄存器状态表 **Register Status Table**
	- 如果某个寄存器的值正在被某个 FU 操作生成，则填入这个 FU 的编号（空则置0）
	- 生成好之后，将实际值填入

具体例子，可以去网站、PPT、历年卷中查看。需要注意几个要点在于：

- **【issue】** 什么时候一条指令允许发射？
	- 需要使用的 FU 是空闲的，避免结构冲突
		- 查功能单元状态表
	- 该指令的 Rd 和其它正在执行的指令的 Rd 没有重复，避免 WAW 冲突
		- 查寄存器状态表
- **【Read Operand】** 什么时候开始读数（读数完下一个时钟周期即为 EX）
	- 当且仅当两个源寄存器都准备好了才开始读数，即 `Rj`, `Rk` 均为 Yes
- **【Write Back】**
	- 写回时需要注意该目的寄存器是否在某个 FU 的 Ready List 中为 Yes
	- 如果是 Yes，则等该指令读完再写回，避免 WAR 冲突

!!! bug "Scoreboard 的缺陷"
	- 没有分支预测，即发射窗口只能覆盖一个 Basic Block
	- FU 的数量、种类和延迟是瓶颈
	- WAR 和 WAW 冲突需要解决，花费大量时间
		- 可以通过显式寄存器重命名解决，减少时间
	- 乱序完成，不能精确中断
		- 即异常发生时处理器无法确保异常点之前的所有指令均已完成，且异常点之后的指令也存在着部分执行的情况

!!! tip "通过显式寄存器重命名解决 WAR 和 WAW 冲突"
	ISA 可能没有提供足够的逻辑寄存器，不过我们可以设置更多物理寄存器，对每一条需要写寄存器的指令，都为其分配一个新的物理寄存器。指令完成之后，物理寄存器中的值会被写会逻辑寄存器。
	
	对于此方法，我们需要维护一个 free list 表示是否有 free 的物理寄存器；还要维护一个物理寄存器到逻辑寄存器的映射表。

> [Scoreboard 算法模拟网站](https://jasonren0403.github.io/scoreboard/)

#### Tomasulo 算法

Tamosulo 算法能够解决一些 Scoreboard 不能很好处理的问题，例如：

```asm
I1: LOAD R1, 0(R2)
I2: ADD  R3, R1, R4
I3: MUL  R1, R5, R6
```

Scoreboard 中，指令 I3 会因为其和 I1 的 WAW 冲突而不能发射，而 Tomasulo 算法能够通过保留站允许其先发射，并正确区分哪个写是有效的。

!!! note "Scoreboard 将控制权给三张表；Tomasulo 将控制权交给 FU 自己，它们自行决定是否接收指令"
	二者一个重要区别在于 IS 阶段，Scoreboard 读的是寄存器的名字，而 Tomasulo 读的是其中的数据，名字允许通过寄存器重命名更改。

- 一个 FU 可以有多个 buffer，称为**保留站 reserved station**
	- `busy`: FU 是否空闲
	- `op`: FU 正在执行什么操作
	- `Vj`, `Vk`: 两个源寄存器对应的值（注意是值）
		- 进入 EXE 阶段时即可将该项清空
	- `Qj`, `Qk`: 如果源寄存器没准备好，则要从哪个 FU 读取
		- CDB 广播时，如果是自己需要的，则获取
	- `Address`: Load/Store 读写的地址
		- Load/Store指令可能会访问相同的地址，此时对于该内存地址来讲，有可能出现数据冒险，因此 `Address` 用来检测冲突
		- 一般来讲，Load/Store 的 EX 第一个时钟周期用来计算 `Address`
	- 距离 FU 完成该条指令的剩余周期数
- 内存也有自己的 reserved station，称为 **Load/Store Buffer**
	- `busy`: 这个位置是否有内存读写请求
	- `address`: 读写的地址
- 表 Register Status Table，同 Scoreboard
- 总线 Common Data Bus(CDB)，负责将结果广播到所有保留站和寄存器
	- 也可以用于实现 Bypassing 功能

![[TomasuloOverview.png]]

- IF：取指令
- IS：Issue，负责解码指令、监视结构冲突、读操作数、重命名寄存器
- EX：送到不同 FU 执行，访存也在这一阶段
- WB：写回结果（写回到 CDB，广播到所有保留站和寄存器）

!!! info "相比于 Scoreboard，将 RO 阶段并入 issue 中"

另外，对于 WAW 冲突，Tomasulo 算法中就算寄存器状态表中 `rd` 对应项不为空，仍然允许该条指令发射，并将自己的 `rd` 覆盖掉寄存器状态表对应项。指令写回时，通过 CDB 进行广播，只有当寄存器状态表或者保留站中存在对应为自己的 FU 时才允许写入，否则直接丢弃这个运算结果。

!!! tip "每个时钟周期寄存器状态表中的结果都会被写回到寄存器堆中，但如上所述并不是所有指令结果都会被写回"

- **Advantage**
	- 分布式冲突检测，不同 FU 有自己的保留站，提高了并行度
	- 通过隐式的寄存器重命名解决了 WAR 和 WAW 冲突
		- 指令发射时即将准备好的数据读入保留站，以此消除 WAR 冲突
- **Disadvantage**
	- 需要解决同一时刻发射多个指令、同一时刻多个值写入 CDB 的问题(主要对于硬件)
	- 和 scoreboard 一样为乱序执行，无法实现精确中断

!!! quote "寄存器重命名"
	- WAR：重命名后一个（W）
	- WAW：重命名前一个
	
	=== "Before Rename"
		![[beforerename.png]]
	=== "After Rename"
		![[afterrename.png]]

<!--
> [我自己写的一个 Tomasulo 模拟小网站](https://tomasulo.nimisora.top/) （其实是 AI 写的）
-->

> [【计算机体系结构】Tomasulo算法 - 知乎](https://zhuanlan.zhihu.com/p/499978902) 可以看一下这篇文章的案例讲解部分

??? tip "有耐心的同学可以读一下这个"
	![[tomasuloalgorithm.png]]
	
	- 观察 Load/Store Step，它的 `RS[r].A` 在发射时初始化为指令的 `imm`；EX 阶段的第一个时钟周期将常数与寄存器值相加以计算实际访问地址 `RS[r].Vj + RS[r].A`；此时 Load 和 Store 指令存在执行差异：
		- 对于 Load 指令，它还需要在 EX 阶段进行内存访问读取
		- 对于 Store 指令，它需要在 WB 阶段额外判断要写回的数据 `RS[r].Vk` 是否准备好（`RS[r].Qk`）
			- 即 Store 指令在进入 EX 阶段时不要求 `Vk` 准备好，它只在 EX 阶段计算地址，但是进入 WB 阶段要求
			- 例如对于 `LD R1, 8(R2)`，这里 `Vj` 指 `R2`，`Vk` 指 `R1`，`A` 初始化为 8

### Advanced Branch Prediction

如果不做分支预测，那么上述动态调度算法都只能在一个基本块中进行分析。

常见的**静态预测**策略有总是不跳转和总是跳转。统计数据显示，大多数分支都是跳转的，所以总是预测跳转的策略理论上更好。但是，总是跳转的实现相较于总是不跳转的实现更为复杂，因此在实际中我们更多的是选择总是不跳转。

**动态预测**基于硬件实现：

- **1 bit 预测器**
	- 对于*每个*跳转指令，只用一个位表示是否跳转，上次跳转了则置 1，上次没跳转则置 0；下次又遇到该跳转指令时，为 1 则跳转，为 0 则不跳转
	- 通过维护分支历史表 Branch History Table 和 Branch Target Buffer，用 PC 的低位作为索引，高位作为 Tag，来维护各个跳转指令的 1 bit 预测器
	- 优点是硬件实现简单

![[1bitpredictor.png]]

- **2 bit 预测器**
	- 用两位表示分支是否跳转，分为四个状态
		- 00: strongly not taken
		- 01: weakly not taken
		- 10: weakly taken
		- 11: strongly taken
	- 每个跳转指令从 00 开始，跳转一次就加一，不跳转一次就减一。
	- 相比 1 bit 预测器，为决策增加了*滞后性*

![[2bitpredictorsg.png]]

- **Correlating Predictor** 相关预测器（2-level Predictor 的一种实现）
	- 许多分支指令的跳转与否依赖于别的分支指令的跳转结果，这时候仅靠自己的跳转历史来预测不够准确
	- 二级预测器包含两个层次的结构：
		- *Level 1* 全局历史寄存器，是一个 $m$ bit 的移位寄存器，记录过去 $m$ 条指令是否跳转，例如 `GHR = 110` 指最近三次是“跳，跳，不跳”
		- *Level 2* Pattern History Table 模式表，使用 Level 1 作为索引，表项通常是一个 2-bit Predictor(或者称 Counter)
	- 注意，“上一条分支”也有可能指自己
	- 对于 (m,n) predictor，其参考最近 $m$ 条指令的跳转结果，每种情况都是一个 $n$ bit predictor
		- 那么一条指令的分支预测缓冲区大小为 $n*2^m$ bit，求 History Table 的大小还要乘上 PC 中用于 index 的低位位数 $2^b$ （entries 个数）

![[CorrPredictor.png]]

??? question "(23-24 Final) How many bits are needed to impl (2,2) predictor with 4K entries?"
	$$2*2^2*4K=32K\ bit$$

- **Gshare Predictor** 属于 Correlating Predictor
	- 将最近 m 条分支指令的历史和 PC 低 m 位进行异或运算来从分支历史表中选择对应预测器，其特点是结合全局历史和本地历史

![[GsharePredictor.png]]

- **Tournament Predictor** 饱和预测器，属于 Hybrid Predictor
	- 前面的预测器中，两条不同跳转指令的 PC 地址的前几位可能相同，导致相互干扰。饱和预测器使用 global 和 local 两个预测器，并根据**跳转地址**来选择使用哪个 predictor
	- 选择器也是一个 2 bit counter，当发生两次错误预测时，它会更改首选预测器
	- global 是全局共有的，local 是自己的

![[TourPredictor.png]]

??? warning "注意是使用跳转地址 Branch Address 作为 Index"
	![[BranchAddressIndex.png]]

剩下两个小透明简单有个印象：

- **Branch Target Buffer, BTB** 分支目标缓冲
	- 其实不是预测器，但是也是一种技术
	- 在跳转指令 IF 阶段时，就预先把目标地址存进表中；如果 branch taken，则直接从表中取地址，不用去访存 Instruction Mem 
	- 由于提前对指令进行预取，对 Memory 的带宽要求更高
- **Return Address Predictor**
	- `call`, `return` 也属于一种跳转，只要记住这个预测器是针对函数调用设计的就好了

!!! quote "三种不同预测器的 Miss Rate 比较"
	![[AdvancePredictorComp.png]]

#### Speculation

我们称基于硬件的猜测执行为 **Speculation**。如果分支结果还没确定，我们需要猜测执行以拓展动态调度的范围，因此我们需要处理预测失误的情况，也即需要支持 **rollback** 操作。

同数据库类似，我们将指令分为【指令执行】+【指令提交】两个阶段，只允许将正确的指令结果 commit 写回到寄存器和内存中。这意味着我们需要一个*提交缓冲区*，在 Tomasulo 算法中称为 **ReOrder Buffer(ROB)**。

ROB 作为循环 FIFO 队列来实现顺序提交，同时也可以替代 Store Buffer(指令 store 的保留站) 作为存储缓冲区，其缓存内容大致如下：

![[ROBEx1.png]]

在指令的 WB 阶段后添加一个 Commit 阶段，并且只有 ROB 的队首指令允许提交。

指令发射的时候，同时进入保留站和 ROB；当指令需要读取操作数时，也可以请求 ROB 中还未提交的临时结果。

!!! tip "如果保留站或者 ROB 满了，则停止发射指令直到有空位"

需要注意的是，如果队首是 branch 或其它跳转指令，但是它预测错误，那么我们可以清空 ROB（Flush）以消除预测失败的影响，这是因为直到指令提交前其结果都不会真正写入寄存器堆和内存中。

与此同时，我们也为 Register Status 添加一个 Field，用来表示当前寄存器在 ROB 的哪个 Entry：

![[FPRSSpecu.png]]

**【Example】** ROB 只允许队首提交，因此即便第二条指令很早就进入写回阶段，也要等待 `div` 提交。

![[ROBtomasuloEx1.png]]


??? info "带有 ROB 的 Tomasulo Hardware"
	![[ROBTomasuloE.png]]

通过这种方式，实现了乱序执行、乱序完成，但是**顺序发射、顺序提交**，从而能够实现精确中断。异常相关的信息会被记录到 ROB 中对应项中，直到该条指令要被提交时触发。

!!! question "带 Speculation 的 Tomasulo，在 \_\_ 有空余的情况下可以 Issue，发射后从 \_\_ 读取 operand 值到 \_\_。"
	- 第一空 C，发射时要同时写入保留站和 ROB
		- A. Reservation Station
		- B. ROB
		- **C. Reservation Station & ROB**
		- D. Reservation Station | ROB
	- 第二空 D，也允许从 ROB 读数
		- A. ROB
		- B. Register
		- C. ROB & Register
		- **D. ROB | Register**
	- 第三空 A，先读到保留站，所有操作数都 ready 后再进入 FU
		- **A. Reservation Station**
		- B. Function Unit

!!! quote "Conlusion"
	![[DynamicSchedulingCon.png]]

??? tip "有耐心的同学可以读一下这个"
	![[tomasuloROBalgorithm.png]]

### Multi Issue: Superscalar

之前我们所说的方法都是为了使实际 CPI 能够接近 Ideal CPI，但是不使用多发射的话 Ideal CPI 只能为 1。

而多发射技术允许一条周期发射多条指令，使得 ideal CPI < 1。在硬件层面，多发射主要通过 **Superscalar** 超标量实现。

- 静态调度 Superscalar
	- 编译器技术优化
	- 顺序执行
- 动态调度 Superscalar
	- 基于 Tomasulo 算法
	- 乱序执行

!!! abstract "能使 CPI < 1 的技术"
	![[CPIlt1.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">Static Superscaler</font>

静态调度的 Superscaler 就是计组课程中简单提到的多发射技术。处理器会将同时执行不会冲突的指令分成不同的 **issue packets** 一起发射，从而增加流水线效率。

但是需要注意的是，处理器在 issue 阶段一个时钟周期，需要完成 **issue check** 的操作。一般来讲，对于 N 发射来说，比较的次数为 $O(N^2-N)$。如果一个 Cycle 不能完成 issue check，那么我们就要考虑增大 clock cycle time 了。

!!! warning "更高的发射率也会导致更高的分支预测损失，因此多发射也需要提高分支预测准确率"

外加硬件层面的静态多发射还需要相应增加寄存器读写口、result buses、forwarding paths等，因此实际上很少有厂家采用这种方式多发射了。

<font style="font-weight: 1000;font-size: 20px" color="orange">Dynamic Superscaler</font>

动态调度的 Superscalar 通过如下两种技术来保证一个时钟周期可以完成一次 issue check：

- **pipeline：**将issue检查流水化，例如一条指令的检查在半个时钟周期完成，两条指令的检查就可以在一个时钟周期完成
- **widen issue logic：**将issue检查逻辑宽度增大，使得一次能检查多个指令

假定我们现在有 1 mem unit，1 integer unit，1 FP unit，要执行以下循环：

```asm
loop:
	L.D.    F0, 0(R1)
	ADD.D   F4, F0, F2
	S.D     F4, 0(R1)
	SUBU    R1, R1, #8
	BNEZ    R1, loop
```

采取双发射，注意 branch 指令只能单独发射：

![[DynSuperscalarEx1.png]]

- 此处指令 1 和 2、4 和 5、6 和 7 之间有数据依赖，因此需要等待前一条指令写回 CDB
- 指令 3 和 4、6 和 8、8 和 9 之间发生结构冲突，因此，因此不能不能同时进入 FU 执行

注意到可以将地址计算单独从 integer unit 中分离出来，即不走整数加法，这样可以解决上面的结构冲突：

![[DynSuperscalarEx2.png]]

!!! note "看到有些指令同时写回了，如果硬件上不支持两条 write CDB 的话，可以将写回时间错开"

接下来我们假设 ALU、Load/Store、Branch 分别使用独立的 integer unit，根据以下例子来观察带有 Speculation 的多发射怎么处理：

```asm
loop:
	LD     R2, 0(R1)
	ADDI   R2, R2, #1
	SD     R2. 0(R1)
	ADDI   R1, R1, #4
	BNE    R2, R3, loop
```

在没有使用 Speculation 时，branch 后面的指令虽然可以先发射，但是只有当分支确定后才能执行，运行情况如下：

![[DynSuperscalarEx3.png]]

那么接下来我们假设预测不跳转，后面的指令可以提前执行：

![[DynSuperscalarEx4.png]]

!!! tip "既然乱序执行了，记得要顺序提交"

## 软件方法

- 编译器优化
	- 循环展开
	- 软件流水线
	- 全局代码调度
- 静态分支预测
- 静态多发射：VLIW

### Loop Unrolling

此处我们给出两条存在数据依赖的浮点相关指令之间所需要插入的 Stall (或 Bubble)：

| Pre       | Post      | Latency (cycle) |
| --------- | --------- | --------------- |
| FP ALU op | FP ALU op | 3               |
| FP ALU op | Store     | 2               |
| Load      | FP ALU op | 1               |
| Load      | Store     | 0               |

!!! note "由于采用了 Bypassing 策略，`MEM` 阶段的输出可以直接作为 `MEM` 阶段的输入，所以 `fld` 和 `fsd` 之间无延迟"

假定我们有一代码段：

```c
for (i = 999; i >= 0; i--)
	x[i] = x[i] + s;
```

其作为 RISC-V 汇编语言的表示如下：

```asm
loop:
	fld    f0, 0(x1)   # f0 = x[i]
	fadd.d f4, f0, f2  # f2 = s, f4 = x[i] + s
	fsd    f4, 0(x1)   # x[i] = f4
	addi   x1, x1, -8  # size(double) = 8 Bytes
	bne    x1, x2, loop
```

在不进行 Pipeline Scheduling 的情况下，一轮循环需要 Stall 3 次；如果我们将与其它指令无数据依赖的 `addi x1, x1, -8` 移动到第一个 Stall 处，则整轮循环变为花费 7 个时钟周期：

![[PipelineSchedulingEx1.png]]

!!! info "同时下面这条 `fsd` 指令对应更改为 `fsd f4, 8(x1)`"

在上面这个例子中，每 7 个时钟周期完成一轮循环迭代。但对数组元素进行实际操作的指令只占了 3 个时钟周期，其余 4 个时钟周期包括两次 Stall 和循环开销（`addi` 和 `bne`)。我们希望消除这 4 个时钟周期。

一个简单方案是循环展开 **Loop Unrolling**，它将循环体复制多次，同时调整循环终止的代码。循环展开不仅消除了分支，还将流水线调度的调控范围从一次循环体增加到了来自多个不同迭代的指令。

对上面这个例子进行循环展开，删除冗余的 `addi` 和 `bne` 计算，并利用**“寄存器重命名”**来进行有效的调度：

![[LoopUnrollingEx2.png]]

循环展开在每次循环的迭代都是独立时最有效，即没有 loop-carried dependency。它通常在实际编译前就已完成，从而消除冗余的计算。

### Static Branch Prediction

编译器编译时，可以根据条件跳转的方向来预测是否 taken：

- 例如对于 `loop` 类型，预测跳转
- 对于 `if` 类型，预测不跳转

还可以根据前几次运行结果的 profile.out 来预测。

### Multi Issue: VLIW

!!! danger "VLIW 选择题会考，注意这也是多发射的一种，即可以将 CPI 降至 1 以下"

Very Long Instruction Word 通过编译器将几个指令打包成一个长指令，它确保一个长指令中各个指令之间都是独立的，从而可以并行执行。

![[VLIWEx1.png]]

- 技术问题
	- 如果找不到足够多的独立指令，则很多功能单元闲置
	- 某个功能单元被 Stall 了，则影响整个长指令的延迟
- 兼容性问题
	- 需要对不同硬件或不同指令集调整

```c
for ( ... )
	x[i] += s;
```

VLIW 包含多个槽位 slot，每个槽对应一个功能单元。假定我们有一个具有 2 个访存运算、 2 个浮点运算、 1 个整数运算的 VLIW 处理器，尝试对如上简单循环进行展开并进行多发射：

![[VLIWprocessor.png]]

### 高级编译器优化技术

#### Software Pipeline

```asm
loop:
1:    LD     F0, 0(R1)
2:    ADD    F4, F0, F2
3:    SD     0(R1), F4
4:    SUBI   R1, R1, 4
5:    BNEZ   R1, loop
```

上述汇编代码中，指令 2,3,4 之间存在数据依赖，但是如果进行循环展开后下面的指令 3,5,7 之间无数据依赖，它们之间也不需要任何 Stall：

![[SoftwarePipelineEx1.png]]

这三个指令重新和控制循环的 `SUBI` 和 `BNEZ` 组装后，可以构成一个只有 Start 和 End 时存在 Stall，中间不会停顿的流水线：

![[SoftwarePipelineEx2.png]]

#### 全局代码调度

全局代码调度能够跨 branch 移动指令，一种常见的技术是 Trace Scheduling，它分为两步：

- **<1> Trace Selection:** 寻找一条 Basic Blocks 的串联执行路径
- **<2> Trace Compaction:** 将 Trace 打包成几个 VLIW 指令

!!! info "Trace Scheduling 是基于编译器的 Speculation"

![[TraceShedulingEx1.png]]

