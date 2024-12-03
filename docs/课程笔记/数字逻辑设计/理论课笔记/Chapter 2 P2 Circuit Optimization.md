# Chapter 2 P2 Circuit Optimization

## Canonical Forms 规范型
**Two common Canonical Forms:**

- Sum of Minterms (SOM)
- Product of Maxterms (POM)

==不化简哦~==

[Example](#):

- $F =\overline{X}\overline{Y} +X\overline{Y} = \sum_{m}(0,2)$     SOM
- $F =(X+\overline{Y})(\overline{X}+\overline{Y}) = \prod_{m}(1,3)$    POM

![[转换成SOM的方法.png]]![[转换成POM的方法.png]]
==特别重要公式：x+yz = (x+y)(x+z)==

## Standard Forms 标准型
将规范型化至最简便是标准型

- Standard Sum-of-Products (SOP) form: 
	- $ABC+\overline{A}\overline{B}C+B$
- Standard Product-of-Sums (POS) form:
	- $(A+B)(A+\overline{B}+\overline{C})C$



!!! note "注意！"
	$(AB+C)(A+C)、AB\overline{C}+AC(A+B)$ 都不是标准型！！


!!! danger "思考"
	Relation between SOM and POM？ SOM and SOP?

## Cost Criteria
- Literal cost（L）
- Gate input cost（G）
- Gate input cost with NOTs（GN）==数非门的时候同一个字母不用数两次,例如下图第三个例子,$\overline{B}$ 只数了一次，所以GN=G+3==

![[门消耗数数案例.png]]

## K-map

![[四维卡诺图.png]]

<center>四维卡诺图</center>

!!! info "Don't Care"
	不定项，在卡诺图中用X表示，在最小项之和中用 $\sum d(...)$ 表示，可以随便定义它的值

=== "例题"
	Four variables odd function has ___ “1” squares in its corresponding K-Map.
=== "答案"
	8 , 四个变量，说明共有16种取值，有一半被1填上(错位填入K-Map)

??? info "三维卡诺图"
	![[三维卡诺图实例1.png]]
	![[三维卡诺图示例2.png]]

### 蕴含项、主蕴含项与基本主蕴含项

- 在卡诺图中，一个蕴含项 ==(Implicant)== 就是任意一个包含 $2^n$ 个 1 的单元；
- 而主蕴含项 ==(Prime Implicant)== 则是在卡诺图中的 **极大** 蕴含项；
    - 对于存在 Dont'cares 的 K-map，如果我们关注的是 1，那我们积极地将他们当作 1 来处理，反之亦然；
    - 但是单独的 X 不会被认为是主蕴含项；
- 基本主蕴含项 ==(Essential Prime Implicant)== 是包含只被它覆盖的 1 的主蕴含项；

!!! info
	对于任意函数，主蕴含项一定存在，但是基本主蕴含项不一定存在。

![[蕴含项.png]]

> 上图中，共有三个主蕴含项，其中两个为基本主蕴含项，一个为冗余(Redundant)主蕴含项

## BUFFER 缓冲器
功能函数： F = X

An electronic amplifier used to improve circuit voltage levels and speed.

当一个输入需要同时接入多个逻辑门的时候使用：

![[buffer.png]]

### 3-state Buffer （三态门）

![[三态门.png]]

当使能信号EN为0时，输出高阻Z。

**Truth Table:**

| EN  | IN  | OUT  |
| --- | --- | ---- |
| 0   | x   | Hi-Z |
| 1   | 0   | 0    |
| 1   | 1   | 1    |

## Derived use

- A - AND
- O - OR
- I - Inverter

eg:

- `2 - 2 AOI` -> $F=\overline{WX+YZ}$
- `2 - 2 - 1 AO` -> $F= WX+YZ+V$
- `2 - 2 AI` -> $F=\overline{(W+X)(Y+Z)}$
