
# Program Control Instructions

## 控制转移指令

关于跳转指令 `JMP` 的相关内容，请参见第一章节。此处给出 Conditional Jumps 相关知识点。

- 8086-80286 中，`jcc` 只能是 short jump（-128 ~ +127）
- 80386 开始，`jcc` 可以是 short jump 或 near jump（± 32K）
	- 80386 - Pentium 4 中，如果处于 Protected Mode，near jump 可长 5B，跳转范围可达 -2GB ~ +2GB

| Jcc指令     | 含义        | 跳转条件             | 解释                              |
| --------- | --------- | ---------------- | ------------------------------- |
| **jc**    | 有进位则跳     | `CF==1`          | Jump if carry 有进位或借位            |
| **jnc**   | 无进位则跳     | `CF==0`          | Jump if no carry 无进位或借位         |
| **jz**    | 为零则跳      | `ZF==1`          | Jump if zero 运算结果为 0            |
| **jnz**   | 不为零则跳     | `ZF==0`          | Jump if not zero 运算结果不为 0       |
| **js**    | 有符号位则跳    | `SF==1`          | Jump if sign 符号数运算结果为负          |
| **jns**   | 无符号位则跳    | `SF==0`          | Jump if no sign 符号数运算结果为正       |
| **jo**    | 有溢出则跳     | `OF==1`          | Jump if overflow 符号数运算结果有错      |
| **jno**   | 无溢出则跳     | `OF==0`          | Jump if not overflow 符号数运算结果正确  |
| **jp**    | 有奇偶校验标志则跳 | `PF==1`          | Jump if parity 运算结果低八位1的个数为偶    |
| **jnp**   | 无奇偶校验标志则跳 | `PF==0`          | Jump if no parity 运算结果低八位1的个数为奇 |
| **jcxz**  | CX为零则跳    | `CX==0`          | Jump if CX is zero CX的值为 0      |
| **jecxz** | ECX为零则跳   | `ECX==0`         | Jump if ECX is zero ECX的值为 0    |
| **ja**    | 无符号大于则跳   | `CF==0 & ZF==0`  | 与 `jnbe` 完全等价                   |
| **jae**   | 无符号大于等于则跳 | `CF==0`          |                                 |
| **jb**    | 无符号小于则跳   | `CF==1`          | 与 `jc`, `jnae` 完全等价             |
| **jbe**   | 无符号小于等于则跳 | `CF==1 \| ZF==1` |                                 |
| **jg**    | 有符号大于则跳   | `SF==0 & ZF==0`  |                                 |
| **jl**    | 有符号小于则跳   | `SF!=OF`         |                                 |

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">LOOP</font>
	
	- **指令格式：** loop dest
	- **指令效果：** 循环跳转 CX 次
		- if(--CX != 0) IP = dest;
	- **注意：** 先作减法，再判断是否为0，因此将CX赋值为0可循环的次数最多(1+FFFF)；如果不希望CX为0时进入循环，应该在进入循环前使用 `jcxz` 指令跳转到循环出口

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">LOOPZ</font>
	
	- **指令格式：** loopz dest
	- **指令效果：** 等于零则循环，最多循环 CX 次
		- if(ZF == 1 && --CX != 0) IP = dest;
	- **注意：** 不影响标志位

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">LOOPNZ</font>
	
	- **指令格式：** loopnz dest
	- **指令效果：** 不等于零则循环，最多循环 CX 次
		- if(ZF == 0 && --CX != 0) IP = dest;
	- **注意：** 不影响标志位

!!! example "三种循环的汇编"
	=== "Do-While"
		![[ch6_2.png]]
	=== "While"
		![[ch6_3.png]]
	=== "For"
		![[ch6_4.png]]

另外，在书写汇编代码时我们可以通过伪指令实现更简单的控制流。

```asm
.WHILE
.ENDW

.BREAK
.CONTINUE

.REPEAT
.UNTIL
.UNTILCXZ

.IF
.ENDIF
.ELSE
.ELSEIF

CARRY? (carry test)
PARITY? (parity test)
SIGN? (sign test)
ZERO? (zero test)
OVERFLOW? (overflow test)
```

例如，如下代码将存于 `AL` 中的 ASCII 字符 '0'-'9', 'A'-'F' 转换为对应的十六进制数：

```asm
.IF AL >= 'A' && AL <= 'F'
	SUB AL, 7  ; 'A' = 65, '9' = 57
.ENDIF
SUB AL, 30H    ; -48
```

除了以上控制流相关的伪指令外，还有这些伪指令：

```asm
BYTE PTR, WORD PTR ...

DB, DW, DD, DQ, DT(10 B)

DUP

STRUCT, RECORD

name PROC
	; SOME STATEMENTS
	RET
name ENDP

MACRO
	; SOME STATEMENTS
ENDM

SEGMENT, ENDS, ASSUME

ALIGN, ORG

EQU

INCLUDE

.386, .486, .586

.CODE, .DATA, .STACK, .MODEL, .EXIT
```

## 条件设置指令

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">SETcc</font>
	
	- **指令格式：** setcc op
	- **指令效果：** 如果 flags 中对应状态位满足条件 `cc`，则将 op 置 1；反之置 0
		- 例如 setc eax 对 CF 进行检测，效果等价 eax = CF

!!! quote "Conditional Set Instructions"
	![[ch6_1.png]]

## 过程调用

通过堆栈传递参数，在 `call` 指令前将参数按照从右到左的顺序压入栈(对应的栈从上到下)，然后先在函数开头执行 `push bp`, `mov bp, sp` 获取当前状态栈顶地址存入 `bp` 。由于 `call` 指令执行过程中会将返回地址也压入堆栈，因此实际调用参数要从 `[bp+4]` 开始(also `ss:[bp+4]`)：

```asm
code segment
assume cs:code
f:  ; f(int a, int b) return a+b
	push bp
	mov bp, sp
	mov ax, [bp+4]  ; a
	add ax, [bp+6]  ; b
	pop bp          ; 恢复 bp
	ret     ; return 指令同样会 pop 栈找出返回地址

main:
	mov ax, 3
	push ax
	mov ax, 2
	push ax
	call f  ; call 指令会将下一条指令的地址压入栈
back:
	add sp, 4  ; 清空栈
code ends
end main
```

与跳转指令不同的是，`CALL` 指令会将返回地址（下一条指令的地址）也压入栈中；当过程调用返回，调用 `RET` 时，会自动从栈中取出返回地址。

而与跳转指令类似的是，`CALL` 指令也分近跳（3B）和远跳（5B）两种：

=== "Near Call"
	![[ch6_5.png]]
	
	??? note
		Near Call 前后段寄存器 CS 均为 1000，因此 push 的返回地址只需要压入偏移 0003H，即过程调用返回为 10003H 
=== "Far Call"
	![[ch6_6.png]]
	
	??? note
		Far Call 前后段寄存器 CS 的值从 1000 变为 1100，因此 push 的返回地址需要压入 CS 和 IP 两个，过程调用返回地址为 1000:0005 = 10005H

如果过程调用有返回值，一般根据位数使用 `al,ax,eax` 来存储返回值，如果需要返回单个 64-bit 结果，则使用 `edx:eax` 分别存储高位和低位。

在函数调用前后，依照不同的规则需要保证一些寄存器的前后值不变，因此我们需要在函数开头和结尾保存它们的状态并恢复。

对于 `CALL` 和 `RET` 指令，它们也和 `JMP` 一样具有绝对偏移和相对偏移两种形式，区分方式为绝对偏移使用寄存器值作为偏移量，使用段寄存器作为基准量；而相对偏移使用 Label 作为偏移量，Instruction Pointer 作为基准量，在编译时被替换为 signed displacement。

我们通过 `CALL` 进行过程调用时既可以用绝对偏移，又可以用相对偏移，但是通过 `RET` 进行过程返回的时候只能使用**绝对偏移**，因为一个过程可以被多个不同地址的代码调用，因此返回的目的地址不是固定值。

有三种方法可以在 `CALL` 中变更特权阶级：

- <1> 定义符合规范的代码段
- <2> 通过特殊描述符 Gate 中转
- <3> 利用系统调用指令
	- (`SYSCALL/SYSRET` or `SYSENTER /SYSEXIT`) to access ring 0 from ring 3

!!! tip "跨权限的 `CALL` 一定是 Far Call"

`PROC` 和 `ENDP` 是用来表示一个过程的开始与结束的伪指令，它的使用方法如下：

```asm
.code
main PROC USE AX BX CX DX
	; some STATEMENTS
	RET
main ENDP
```



## 中断与异常

中断的调用其实相当于内置函数的调用，中断函数的地址存储在*中断向量表*或*中断描述符表*中。

对于 8086 实模式，中断向量表位于内存 `0000:0000` 后，每四个字节对应一个 ISP 的地址。例如，对于如下所示的一段内存：

```
0000:0054:  40h
0000:0055:  02h
0000:0056:  58h
0000:0057:  02h
0000:0058:  2Dh   <- int 16h 中断向量起始地址
0000:0059:  04h
0000:005A:  70h
0000:005B:  00h
```

对于指令 `int 16h` ，其中断向量位于 `0000:16h*4` ，即起始地址为 `0000:0058` ，并且按照小端存储规则，当我们调用中断指令 `int 16h` 时，下一条指令的地址应为 `0070:042D`，这时 `CS=0070h, IP=042Dh`。

发生中断时，处理器执行如下操作：

1. 将 `FLAGS` 压入栈
2. 设置 `FLAGS.IF = 0`
3. 将 `CS` 和 `IP` 先后压入栈
4. (可选) 将 error code 压入栈
5. 在中断向量表中找到对应的 ISP 地址
6. 将控制权交给 ISP

`IRET` 指令用于从中断处理程序返回，它的执行效果为：

1. 从栈中弹出保存的 `CS` 和 `IP`
2. 从栈中弹出保存的 `FLAGS`
3. 从 `CS:IP` 处开始执行

!!! note "`IRET` 可以等价于一个 `far RET` + `POPF`；保护模式则使用 `IRETD`（`far RET` + `POPFD`）"

保护模式使用中断描述符表，该表每个 entry 是一个 8B 的中断描述符。


## MISCELLANEOUS

`HLT` 用于停止指令执行，并将处理器设置为 HALT 状态。从该状态退出有以下三种方式：

- an enabled interrupt
	- 中断是停止状态最常见的退出方式，计算机必须能对外部世界做出反应
	- 例如 maskable interrupt（可屏蔽中断）, NMI（不可屏蔽中断）
- a debug exception
	- 调试器要求能够接管处理器，即便处理器处于停止状态
- a hardware reset
	- 最底层的手段

多字节 `NOP` 指令常用对齐边界的填充。Intel Optimization Reference Manual 中推荐的多字节 `NOP` 指令的机器码为：

| Length | Assembly                                     | Byte Sequence                 |
| ------ | -------------------------------------------- | ----------------------------- |
| 2B     | `66 NOP`                                     | `66 90H`                      |
| 3B     | `NOP DWORD ptr [EAX]`                        | `0F 1F 00H`                   |
| 4B     | `NOP DWORD ptr [EAX + 00H]`                  | `0F 1F 40 00H`                |
| 5B     | `NOP DWORD ptr [EAX + EAX*1 + 00H]`          | `0F 1F 44 00 00H`             |
| 6B     | `66 NOP DWORD ptr [EAX + EAX*1 + 00H]`       | `66 0F 1F 44 00 00H`          |
| 7B     | `NOP DWORD ptr [EAX + 00000000H]`            | `0F 1F 80 00 00 00 00H`       |
| 8B     | `NOP DWORD ptr [EAX + EAX*1 + 00000000H]`    | `0F 1F 84 00 00 00 00 00H`    |
| 9B     | `66 NOP DWORD ptr [EAX + EAX*1 + 00000000H]` | `66 0F 1F 84 00 00 00 00 00H` |

虽然最高只有 9B `NOP`，要补足 15B 可以在前面多加 `0x66`

???+ example "Multi-byte NOPs Example"
	![[ch6_7.png]]

`BOUND` 指令用于检测某个 array index 是否在 bounds operand 表示范围内，语法格式为 `BOUND REG, MEM`。其中 `MEM` 是一个内存地址，其指向的内容即为我们指示的上下界，通常是 2 个 words 或 1 个 doubleword 的内存位置。

如果操作数 `REG` 不位于 `MEM` 指示的上下界内，则会发出 BOUND range exceeded exception；如果在，则继续执行下一条指令。

```asm
.data
	start DW 0, 10  ; define the boundary

.code
...
	mov ax, 5
	bound ax, start  ; 0 < 5 < 10, continue
	mov ax, 11
	bound ax, start  ; 11 > 10, raises exception
...
```

`ENTER` 和 `LEAVE` 指针用来为被调用的过程建立/释放栈帧。栈帧由以下部分组成：

- **调用者提供**
	- argument parameters
	- return address
- **被调用者提供**
	- 前一个栈帧的指针
	- local variables
	- 被调用者修改的寄存器的拷贝

在我们书写过程时，可以很方便的利用这两个指针进行栈相关的维护：

![[ch6_8.png]]

其中 `ENTER` 指令的语法格式为 `ENTER stack space, nesting levels`。第二个操作数嵌套层数指定了从上一个栈帧中复制过来的栈帧指针个数，被调用的函数可以据此访问它的父函数。

