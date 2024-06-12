# Chapter 1 & 2 Part 1 Digital System & Boolean Equation

!!! danger
	Chapter 1 及 Chapter 2 Part 1 内容较为杂乱，因此仅将要点放于此，推荐自行观看课件

## 常见的四种编码方式
![[常用的四种编码方式.png]]

### BCD 码
8,4,2,1又称 BCD码 (Binary Code Decimal)

!!! note "注意区别编码和二进制 (重要)"
	<center>(185)~10~ = (0001 1000 0101)~BCD~ = (10111001)~2~</center>
	
	<center>请注意编码不仅是对每位数字单独编码，还要补满四位</center>
由于BCD编码是用10中排列来表示二进制，其加法运算自然也和二进制加法不同：

- Step 1: 正常加法，得到SUM:
	- ![[BCD加法1.png]]
- Step 2: 如果当前的SUM大于9(Invalid Code)，则给当前位的下一个高位进一个Carry，并给自己Add 6(0110)，得到结果:
	- ![[BCD加法2.png]]

!!! example "1897 + 2905 = 4802"
	![[BCD加法3.png]]

### 余3码 & 8,4,-2,-1
二者的共同点在于都是Complement Code

### 格雷码
格雷码的优势在于相邻的两个数只有一位是不同的，这非常便于我们进行检验校对。(以及后面要学的卡诺图优化)

#### 如何得到格雷码?

先列出对应的BCD编码，再按照 格雷码当前位的数字 = BCD码对应位的数字 & BCD码对应位的下一个高位的数字 计算。 ($Grey_i\ =\ BCD_i\  \&\  BCD_{i+1}$)

## 奇偶校验位

- Even Parity 指使1的数量为偶数
	- Eg: 0111_0101 -> 0111_0101_1
	- 事实上，校验码既可以加到前面也可以加到后面，要看题目要求
	- 对于异或函数 $F= X\oplus Y=X\overline{Y}+\overline{X}Y$ ，可以用作奇函数
		- Odd Function有奇数个1则输出1，可用来生成 ==偶校验码== 
- Odd Parity 则相反
	- 可以用偶函数来生成 ==奇校验码==
		- 注意，偶函数是奇函数非一下哦，不是用异或非连起来

[关于异或，还有如下要点](#)

-  $X\oplus 1=\overline{X}$
	- $(\overline{X\oplus Y})=(X\oplus Y)\oplus 1 =\overline{X}\oplus Y=X\oplus \overline{Y}$  ==交换定律==


![[常用逻辑门.png]]

## Universal Gate 通用门

- A gate type that can implement all possible Boolean functions
	- 目前只指 NOR 以及 NAND

## Duality Rule & Complement 对偶规则 与 互补

- 原函数 $F=\overline{A}{B}+C\overline{D}$
- 对偶函数 $F=(\overline{A}+B)(C+\overline{D})$
- 互补函数 $\overline{F}=(A+\overline{B})(\overline{C}+D)$

## Boolean Algebra 布尔代数
![[常用化简理论.png]]

!!! info
	其中尤其需要记住 Simplification 和 Consensus



