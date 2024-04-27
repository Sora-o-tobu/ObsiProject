# Chapter 4 Sequential Circuits
## Part 1 Storage Elements and Sequential Circuit Analysis

### Introduction of Sequential Circuits 时序电路的介绍
一个时序电路包含:

- Storage Elements:
	- **Latches** or **Flip-Flops** 锁存器或触发器
- Combinational Logic 

![[时序电路基本构造.png]]

时序电路的种类:

- Synchronous 同步时序电路
	- 根据时钟信号的变化来更新输出的值 更加可控
- Asynchronous 异步时序电路
	- 每当输入的状态改变，立刻更新输出的值 变化更快


#### 锁存器

Latch 的条件

1. 拥有两个稳定的状态，0和1
2. 能够长时间维持稳定态
3. 在特定的情况下能够改变状态

!!! note ""
	
	=== "S'R'锁存器"
		![[S'R'锁存器.png]]
	
	=== "SR锁存器"
		![[SR锁存器.png]]
	二者的共同点是当S置1的时候，Q会变成1(注意S'R'锁存器 S置1的时候实际上输入了S'=0)

通过加上时钟信号作为使能，可以组成Synchronous的门控锁存器 **Clocked S-R Latch**
![[门控时序电路.png]]

以上两种锁存器都存在一个问题，就是对于S/R某种输入存在不确定状态，并有可能产生介稳态(metastable state)

??? info "metastable state"
	![[metastablestate的产生.png]]

为了消除这种不确定，一种解决方法是限定锁存器的输入状态，由此构成了**D-Latch**

![[D锁存器原理图.png]]


| C   | D   | Q(t+1)     |
| --- | --- | ---------- |
| 0   | X   | No change  |
| 1   | 0   | 0: Clear Q |
| 1   | 1   | 1: Set Q   |

只有当C为1时，D锁存器才能写入数据；而当C为0时，D锁存器的数据不会变化

!!! danger
	在实际使用过程中，可能会遇到一个storage element连接到另一个storage element，然后另一个storage element又重新连接回原来的storage element，即在combinational logic中一个storage element的输入输出可能会相连。
	
	但是如果$\overline{Q}$与D相连，在clock时钟信号有效的时间内，Q的值可能会多次变化，导致我们无法确定C置0时Q的输出是什么。这就是我们常说的**空翻现象**

#### 触发器

为解决上述空翻问题，我们引入触发器

- Master-Slave flip-flops 主从触发器
- Edge-triggered flip-flops  边缘触发触发器

!!! example "SR主从触发器"
	![[SR主从触发器原理图.png]]

这样子当C置0时，Slave从Master中读取数据，而Master的数据是固定不变的，不会产生变化

但是其实主从触发器仍然存在问题，当S和R均置0时，如果出现0-1-0glitch(S或R输入短暂跳为1)，会导致Slave写入异常数据，这被称为1s-catching。

为了避免1s-catching，可以使用边缘触发触发器，它只保存了电位变化瞬间的那个数据，大大减小了危害。

!!! example ""

	=== "Negative-Edge-Triggered D Flip-Flop"
		![[边缘触发器.png]]
		![[负边缘触发器器件图示.png]]
	
	=== "Positive-Edge-Triggered D Flip-Flop"
		![[正边缘触发器原理图.png]]
		![[正边缘触发器器件图示.png]]


![[标准符号.png]]