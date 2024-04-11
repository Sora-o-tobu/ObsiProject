# Part 1 ：Implementation Technology and Logic Design
## Definition Of Two Types Of Logic Circuits

- **Combinational Circuit** 组合电路
A combinational circuit consists of logic gates whose output is a function of only the present input.

- **Sequential Logic Circuit** 时序逻辑电路
Sequential logic is a type of logic circuit whose output depends not only on the present value of its input signals but on the sequence of past inputs (state or memory).

## Combinational circuits

组合电路不包含任何memory devices , feedback loops , one-way transmission of input signal(输入信号的单向传输)
因此，一个组合电路包含：
- m个布尔值输入
- n个布尔值输出
- n个只与当前输入有关系的变换函数

### 设计过程中的技术参数 Technology Parameters

#### Fan-in and Fan-out
- Fan-in 是一个逻辑门能够接受的最多输入，如果超过了，输出会变为undefined 或者 incorrect
- Fan-out 是一个逻辑门的输出所能连接到最多的输入（或称负载 load），且不影响电路正常性能的个数
![[faninandfanout.png]]

**Fan-in的原因，以与门为例**
![[Fanin的原因.png]]

#### 传输延迟 Propagation Delay
传输延迟是输入改变对应的变化到输出改变所需的时间

相对而言，t<sub>HL</sub> 与 t<sub>LH</sub> 都是指输出的变化时间；
而t<sub>PHL</sub> 与 t<sub>PLH</sub> 则计算L和H的中点时间差
![[传输延迟.png]]

- **细品差别**
![[细品差别.png]]

##### 传输延迟的模型

在仿真中，我们常用Delay models有
- Transport delay
- Inertial delay （惯性延迟、固有延迟）

- **Transport delay**：
A change in the output in response to a change on the inputs occurs after ==a fixed specified delay.==
![[Transport delay.png]]

- **Inertial delay**：
A pulse of duration less than the inertial delay (rejection time) does not contain enough energy to cause the device to switch.
![[Inertial delay.png]]


Example：
![[延迟的例子.png]]

如果考虑到了电路延迟的话，就有可能出现意想不到的结果哦~

### 考虑这些因素，我们在logic synthesis中有必要精选Technology mapping
![[工艺映射.png]]

其中映射的一个重要步骤是recognize logic equivalence between 原始逻辑与目标技术
为了做到这个步骤，我们需要：
#### Mapping to NAND gates
1. 将所有AND OR都用NAND 与 Inverter代替
2. 将inverter push 到 fan-out 中
3. 如果inverter可以配对，则消去

Example：
![[NANDmapping.png]]



# Part 2
## Rudimentary Logic Functions 基础函数功能
- Value - Fixing $F = 0$ or $F = 1$ 
- Transferring $F = X$ 
- Inverting $F = \overline{x}$
- Enabling $F = X \cdot EN or F = X + \overline{EN}$

![[Chapter 3 Combinational Logic Design 2024-04-04 15.38.39.excalidraw]]

## Functional Blocks

### 1. Decoder 译码器
Decoding - the conversion of an n-bit input code to an m-bit output code with ==n <= m <= 2n== such that each valid code word produces a unique output code
例如实验lab5的3-8译码器以及lab6的BCD to 7-segment decoder

一个高级的Decoder的设计可以采用hierarchical design设计思想

例如，一个3-8译码器可以由一个2-4译码器和一个1-2译码器组合而成：
![[3-8译码器1.png]]

另外，Decoder一般会有使能信号EN来控制。
![[3-8译码器使能信号.png]]
在这种情况下，我们也可把译码器当作demultiplexer（信号分离器、多路解调器）看待。
b和a的值控制使能信号EN的流向，当EN =0 时，无论b、a为多少，流出去的只有0。

也可以利用使能信号组合更大的Decoder：
![[3-8译码器2.png]]

Decoder的另一个应用是，可以用Decoder加上OR门组成minterms
![[Decoder组成minterms.png]]


### 2. Encoder 编码器

Decimal-to-BCD encoder:
- Inputs:10 bits corresponding to decimal digits 0 through 9 (D<sub>0</sub>, …, D<sub>9</sub>) ——one-hot code
	- one-hot code: 只有一位是1
- Outputs:4 bits with BCD codes
真值表我们很容易就能得到：
![[Decimal-to-BCDencoder.png]]

Priority Encoder(优先级编码器):
优先级高的输入如果有效，则不用管优先级低的输入
真值表（V检测是否有效）：
![[优先级编码器真值表.png]]
优先级编码器可以用K-map狠狠化简，请慢慢化简哦~

### 3. Multiplexer 多路选择器
多路选择器可通过Decoder、AND-OR Gates、3-state buffers来实现

2-to-1-Line Multiplexer:
![[2-to-1多路选择器.png]]

In general , an 2<sup>n</sup>-to-1-line multiplexer is combined:
- n-to-2<sup>n</sup>-line decoder
- $2^n\times 2 AND-OR$ 

例如：
4-to-1-line 多路选择器：
![[4-to-1多路选择器.png]]

如果采用三态门构建的话可以省去最后的或门，并拥有更小的Gate input cost：
![[三态门多路选择器1.png]]

另一种构建方法：
![[三态门多路选择器2.png]]


多路选择器可以应用于不同编码之间的转换
如，Gray to Binary Code：
![[格雷码转换二进制编码.png]]

此外，此转换还可以进行化简：
![[转换的化简.png]]
化简的规则为，观察真值表，当两行中A、B各自相同时，若输出分别为0、1，则可置C或~C；若输出均为0或1，则置常值0或1。

# Part 3 : Arithmetic Functions

