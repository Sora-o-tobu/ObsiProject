# Chapter 5 Digital Hardware Implementation
# Part 2 Programmable Implementation Technologies

## Programming technologies

分为三种types：
- Control connections

- Control transistor switching

- Build lookup tables

### Control connections 控制实体电路通断
- Mask programming 掩模编程（较昂贵）
- Fuse  熔丝，可以控制是否要断掉（一次性）
- Antifuse  与熔丝相反，可通过控制电压来控制是否接通（体积小，成本高）

### Control transistor switching 控制晶体管控制极

当控制极输入1时，通；输入0时，断。
特点：
- Erasable（可重编程）
- Electrically erasable 
- Flash （Flash memory，可用于u盘）

### Build Lookup Tables（LUT） 查表法
采用多路选择器，当需要改变逻辑关系的时候不需要改变器件


## Programmable Logic Device

- Read Only memory(ROM) ==Fixed AND & Programmable OR==
- Programmable Array Logic(PAL) ==Programmable AND & Fixed OR==
- Programmable Logic Array ==Programmable AND & Programmable OR==
- Complex Programmable Logic Device (CPLD) /FieldProgrammable Gate Array (FPGA)
其中，前三种是通过Control connections来控制，最后一种使用LUT控制

![[可重编程逻辑器件1.png]]
[解读]：
对于ROM：将所有最小项译码出来，但是最终可以只让其中两项或出来
对于PAL：OR的数量是死的，但是可以在AND中加项或是减项
对于PLA：OR和AND都可以发生增减，灵活但有局限（缺陷）

### Logical Symbols
![[LogicalSymbols.png]]

![[LogicalSymbol例子.png]]

![[LogicalSymbol例子2.png]]

## Read Only Memory
Read Only Memory(**ROM**) or Programmable Read Only Memory(**PROM**)

> [!NOTE] General Structure with decoder
> ![[ROM结构.png]]


**[Example]** An $2^3\times 4$ ROM (N=3 input lines , M=4 output lines) 
![[ROM.png]]

> [!NOTE] 实际使用用例：3bit输入的平方计算 $B[5:0]=A[2:0]\times A[2:0]$
> 1. 首先列出真值表
> ![[Truthtablerom.png]]
> 2. 将能直接化简掉的直接化简，如上图$B_1=0,B_0=A_0$
> 3. 然后运用minterms直接得出ROM的结构图：
> ![[ROM2.png]]


## Programmable Array Logic
The PAL is the opposite of the ROM , having ==programmable set of ANDs== combined with ==fixed ORs==.
> [!NOTE] General Structure
> ![[PAL基础结构.png]]


由于与门的 fan-in 有限，所以不可能将所有minterm表示，这就导致有些function可能做不到，这个缺点可以由回手掏解决：
![[PAL.png]]

## Programmable Logic Array
**优点:** 
灵活，能完成对ROM和PAL来说impractical的函数，
**缺点:**
与门的数量限制了应用
需要多余的外部连接来进行多层次的电路设计

![[PLA优化.png]]因此通常会在后面加一个异或门（决定是否要对结果取非）来增加与门表示的product的种类

## Field-Programmable Gate Arrays （FPGA）
### Lookup Table（LUT）
通过组合多路选择器来选择要输出的真值表的值：
![[Lookuptable.png]]

> [!NOTE] Internal Structure
> ![[FPGA结构.png]]


## Configurable Logic Block
FPGA基本的逻辑单元

![[CLB.png]]
由于LUT中的Storage Cells是不稳定的，我们需要在里面使用PROM来永久保存data.

> [!NOTE] Example
> ![[FPGAexample.png]]


**[Example]** 将格雷码转换成二进制码，用四种可重编程元器件实现：
1. ROM
![[ROM实现格雷码转换.png]]
2. PAL
![[PAL实现格雷码转换.png]]
3. PLA
![[PLA实现格雷码转换.png]]
4. FPGA
![[FPGA实现格雷码转换.png]]