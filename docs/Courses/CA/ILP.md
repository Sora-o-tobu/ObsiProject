
# Instruction-Level Parallelism

## Overview

1985 年以后的处理器几乎都使用了流水线来使指令能重叠执行。指令之间的可能的重叠称为指令级并行 **Instruction-Level Parallelism**。

有关提供指令间并行度来扩展基本流水线概念的技术包括如下，以及它们分别降低了 CPI 的哪一部分：

![[ILPoverview.png]]

一个流水线处理器的 CPI 值等于基础 CPI 和因为各种 Stall 而耗费的周期之和：

Pipeline CPI = Ideal Pipeline CPI + Structural Stalls + Data Hazard Stalls + Control Stalls

*Ideal Pipeline CPI* 是对能够实现的最佳性能的度量，通过缩短上式的右边三项 Stalls，我们可以降低总流水线 CPI。

在实现方法上，ILP 大体可分为两类：

- **<1> Hardware discover and exploit dynamically**
- **<2> Software find statically**

!!! quote
	本章将介绍这两种方法和相关概念，还将讨论 ILP 的局限性。这是这些局限性直接导致了向多核的演变。

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

!!! info "这里假设有两条顺序的指令 $i,j$"

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
	- 指令 $j$ 尝试对指令 $i$ 要读取的寄存器或地址进行写操作，相当于数据依赖反过来
	- 为了确保指令 $i$ 能够读到正确的值，必须保持指令原来顺序
- **<2> Output Dependence**
	- 指令 $i$ 和 $j$ 尝试对同一个寄存器或地址进行写操作，我们希望最后的值依赖于后一条指令 $j$

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

## Techniques

### Pipeline Scheduling & Loop Unrolling

本章我们基于一个标准的五级流水线来讲解。由于浮点操作实现更为复杂，其 `EX` 阶段往往比 Integer ALU 需要更多时钟周期。

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
	fld    f0, 0(x1)  # f0 = x[i]
	fadd.d f4, f0, f2 # f2 = s, f4 = x[i] + s
	fsd    f4, 0(x1)  # x[i] = f4
	addi   x1, x1, -8 # size(double) = 8 Bytes
	bne    x1, x2, loop
```

在不进行 Pipeline Scheduling 的情况下，一轮循环需要 Stall 3 次；如果我们将与其它指令无数据依赖的 `addi x1, x1, -8` 移动到第一个 Stall 处，则整轮循环变为花费 7 个时钟周期：

![[PipelineSchedulingEx1.png]]


在上面这个例子中，每 7 个时钟周期完成一轮循环迭代。但对数组元素进行实际操作的指令只占了 3 个时钟周期，其余 4 个时钟周期包括两次 Stall 和循环开销（`addi` 和 `bne`)。我们希望消除这 4 个时钟周期。

一个简单方案是循环展开 **Loop Unrolling**，它将循环体复制多次，同时调整循环终止的代码。循环展开不仅消除了分支，还将流水线调度的调控范围从一次循环体增加到了来自多个不同迭代的指令。

对上面这个例子进行循环展开，删除冗余的 `addi` 和 `bne` 计算，并利用“寄存器重命名”来进行有效的调度：

![[LoopUnrollingEx2.png]]



### Advanced Branch Prediction

1 bit 预测器

2 bit 预测器

Two Level 预测器

