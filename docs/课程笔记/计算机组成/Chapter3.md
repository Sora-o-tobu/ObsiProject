
# Chapter 3. Arithmetic for Computer

## Basic ALU

基本与数逻中 ALU 差不多，请看学长的笔记！（或者看我的数逻笔记QAQ）

[3 Arithmetic - 咸鱼暄的代码空间](https://xuan-insr.github.io/computer_organization/3_arithmetic/)

相较于数字逻辑课程中的 ALU ，本课程所介绍的 ALU 按照 Operation 要实现的功能如下：


| ALU Control Lines | Function         |
| ----------------- | ---------------- |
| 000               | And              |
| 001               | Or               |
| 010               | Add              |
| 110               | Sub              |
| 111               | Set on less than |
| 100               | nor              |
| 101               | srl              |
| 011               | xor              |

### 1 bit 与,或,加减法 ALU

![[1bit与或加减法器.png]]

上图展示了一个实现了与，或以及加减法功能的 1bit ALU。当需要进行减法时，将 Binvert 以及 CarryIn 均置 1 ，实现补码的运算。

!!! note "实际上将 Binvert 和 CaryyIn 连接到同一个输入上就可以了"

### 64 bit ALU

通过 CarryIn 和 CarryOut 的前后连接，即可组装成一个 64 bit ALU

![[64bitALU.png]]

!!! danger "在数逻中已学过，Overflow 等于最后一个 ALU 的 CarryIn 和 CarryOut 异或"

由于硬件需要判断最终的结果是否为 0，最简便的方法就是将所有 1bit Result 连接到或非门上，作为标志 Zero 输出。最终得到 ALU 及其符号如下：

![[64bitALUPlus.png]]


### 超前进位加法器

由于上述加法器后一个 ALU 的 CarryIn 依赖于上一个 ALU 的 CarryOut ，当它的规模增大，时间延迟就会变得很明显。为了减少这个影响，我们将简单介绍 Carry look-ahead adder(CLA) ，即超前进位加法器

![[ObsiProject/docs/课程笔记/计算机组成/image/全加器.png]]

取单个全加器来看，可以看出，要使 CarryOut 等于 1，那么 A,B,CarryIn 三者起码有两个等于 1。

我们定义 $A_i B_i$ 为 generate,$G_i$ ；定义 $A_i\oplus B_i$ 为 propagate,$P_i$ ，那么可以得到关键递推式：

$$
C_{i+1} = G_i +P_i C_i
$$

有了递推关系，我们就可以层层带入，将 $C_i$ 替换成初始已给出的输入：

$$\begin{array}l
C_1 = G_0 +P_0 C_0 \\
C_2 = G_1 +P_1 C_1 =G_1 +P_1 G_0 +P_1 P_0 C_0 \\
C_3 = G_2 +P_2 C_2  =G_2 +P_2 (G_1 +P_1 G_0 +P_1 P_0 C_0 ) \\
\ \ \ \ \ = G_2 +P_2 G_1 +P_2 P_1 G_0 +P_2 P_1 P_0 C_0 \\
C_4 = G_3 +P_3 C_3 = G_3 +P_3 G_2 +P_3 P_2 G_1 \\
\ \ \ \ \ \ \ \ \ \ + P_3 P_2 P_1 G_0 +P_3 P_2 P_1 P_0 C_0
\end{array}$$

按照上述逻辑关系，我们可以构建出 4bit CLA 连接如下：

![[4bitCLA.png]]

可以看出，随着位数的增加，计算一位 Carry 所需的与门越来越多，开销越来越大；同时与门的输入个数也有上限。

为了解决这个问题，我们通常以 4bit 为一组，每组之间行波连接，得到最终的超前进位加法器：

![[FinalCLA.png]]

## Multiplication

乘法本质上就是加法

### V1

![[Mv1.png]]

其中，1000 称为 Multiplicand 被乘数； 1001 称为 Multiplier 乘数

核心思想即为当前 Multiplier 位数为 1，则加上 Multiplicand ；若为 0，则加上 0

![[Mv1G.png]]

两个 64bit 的数相乘，结果最多为 128bit；并且 Multiplicand 在运算过程中需要不断左移，所以该乘法器总共需要一个 64bit 的器件以及三个 128bit 的器件

### V2

为了减少开销，我们可以选择不左移 Multiplicand 而是右移 Product，这样 Multicand 所在的寄存器即可减少至 64bit

!!! example
	![[Mv2E.png]]

![[Mv2G.png]]

改善得到的乘法器如图所示。但是图中 Product 实际上为 129bit，因为乘法分解的加法计算集中在 Product 的左半部分执行，这可能会导致最高位暂时发生进位。

### V3

由于 Product 初始的右半部分并没有使用，且 Multiplier 的位数使用过后即可丢弃，所以我们可以在 Product 的右半部分存储 Multiplier 来为乘法器节省一个 64bit 的寄存器

!!! example
	![[Mv3E.png]]

依照该思想，得到的最终版本乘法器如图所示：

![[Mv3G.png]]

!!! warning "这里的 Product 也是 129bit"

### Booth 算法

在硬件实现方面，往往移位运算比加减法运算有着更小的开销，所以可通过减少 Multiplier 的 1 的数量来实现更快的速度

!!! example
	![[boothex1.png]]

Booth 算法基于此给出了一定规则：

| Bits | Action     |
| ---- | ---------- |
| 10   | 左部分减去被乘数   |
| 11   | 不做算术运算，仅移位 |
| 01   | 左部分加上被乘数   |
| 00   | 不做算数运算，仅移位 |

!!! tip "这里用到的移位为算数移位，遇1补1"

在上述 V3 版本的乘法器基础上，我们还要保留上一个最低位用来 Booth 算法的判断。并且该乘法器对于负数乘法同样适用，接下来以 2*(-3) 为例子演示其操作：

![[Boothex2.png]]

!!! example "More Example"
	![[boothex3.png]]

## Division

TO BE DONE