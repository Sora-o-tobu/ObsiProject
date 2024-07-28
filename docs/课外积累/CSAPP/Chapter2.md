# Chapter 2. Representing and Manipulating Information

## Part 1. 信息的存储

通常情况下，程序将内存看作一个大数组，数组的元素长度为一个字节，每个元素都有唯一的地址 (address) ，这些所有地址的集合就称为 ==虚拟地址空间== (virtual address space)

字长决定了虚拟空间地址的范围，例如，对于一个 w bit word 的机器，虚拟地址的范围是 $0$ ~ $2^w-1$

!!! tip
	现如今，大部分服务器、PC、手机都完成了从32位字长到64字长的迁移；不过在一些嵌入式的场景中，32位机器仍占有一席之地
	
	在迁移过程中，大部分64位机器做了向后兼容，因此64位机器仍然可以运行由32位机器编译的程序；同时，可以在64机器上通过指令 `gcc hello.c -m32 -o hello32` 编译能够在32位机器上运行的程序

以 C语言 为例子，将整型变量 (占用四字节) `0x01234567` 存储到内存中，根据存储方式的不同，有如下两种内存结构：

![[整型变量存储.png]]

大多数 Intel 兼容机采取小端模式，IBM和Sum公司的机器大多采用大端法

---

二进制是计算机编码、存储和操作信息的核心，围绕着0和1的研究已经演化出丰富的数学知识体系。

关于布尔运算，在离散数学、数字逻辑设计等课程均有详细讲解，此处亦不再赘述

!!! info "Masking Operation"
	掩码运算，例如希望得到 `0x12ABCDEF` 的最低有效字节的值，可以通过 `0x12ABCDEF & 0xFF` 来得到结果

<font style="font-weight: 1000;font-size: 20px" color="orange">移位运算补充</font>：

- 逻辑右移
	- 空位补充 `0`
- 算数右移
	- 空位与最高有效位相同

实际上，几乎所有编译器对有符号数的右移均采用算数右移；而对于无符号数，一定是逻辑右移

## Part 2. 整数的表示

源码、反码、补码... 老生常谈，此处亦不细讲，仅补充几点额外的信息:

- 对于大多数C语言实现，有符号数和无符号数之间转换的规则是，位模式不变，但是解释这些位的方法变了

```c
❯ nano signed.c
 signed.c
 1 #include <stdio.h>
 2
 3 int main(void)
 4 {
 5         int a = -12345;
 6         unsigned short b = (unsigned short) a;
 7         printf("a = %d , b = %u",a,b);
 8 }

❯ gcc signed.c -o signed
❯ ./signed
a = -12345 , b = 53191
/*
* -12345 : 1100 1111 1100 0111
*  54191 : 1100 1111 1100 0111
*/
```

- 如果一个运算数是有符号数，另一个是无符号数，那么C语言会隐式的将有符号数强制转化为无符号数进行运算

```c
❯ nano versus.c
versus.c
 1 #include <stdio.h>
 2 int main(void){
 3         int a = -1;
 4         unsigned int b = 0;
 5
 6         if (a < b)
 7                 printf("-1 < 0");
 8         else
 9                 printf("-1 > 0");
10 }

❯ gcc versus.c -o versus
❯ ./versus
-1 > 0
```

- 当位数更多的数据类型转化为位数更低的数据类型时，多出的位数舍去；当位数低的数据类型转化为位数高的数据类型时，需要根据要转化成的数据是否是有符号数
	- 若是无符号数，新增位全为 `0`
	- 若是有符号数
		- 若是正数，新增位全为 `0`
		- 若是负数，新增位全为 `1`
- 保证符号拓展数值保持不变

## Part 3. 整数运算

你知道我要说什么，这节内容在 《数字逻辑设计》 中亦有记载

- 无符号数加法的溢出

```c
❯ nano addition.c
addition.c
 1 #include <stdio.h>
 2 int main(void)
 3 {
 4         unsigned char a = 255;
 5         unsigned char b = 1;
 6         unsigned char c = a + b;
 7         printf("a+b = %d , c = %d",a+b,c);
 8 }

❯ gcc addition.c -o addition
❯ ./addition
a+b = 256 , c = 0
/* What if (char)127 + (char)1?
char x = 123 , y = 1;
char z = x + y;
printf("z = %d",z);
# Output: z = -128
# x : 0111 1111
# y : 0000 0001
# z : 1000 0000
*/
```

- 对于两个 w 位的无符号数的乘法，得到的结果也应该为 w 位，因此有可能会产生截断
- 无论是无符号乘法还是补码乘法，运算结果的位级表示都是一样的(截断后)

![[整数乘法.png]]

!!! note "回归性原理，乘法就是加法"
	![[乘法的实质.png]]

- 同样的，除法可以看作是右移操作。对于有符号的负数的算数右移，由于需要满足四舍五入的特性，需要对原数加上 $2^k-1$ 大小的 `bias` ，即可得到向零舍入的结果

![[有符号数算数右移.png]]

## Part 4. 浮点数
