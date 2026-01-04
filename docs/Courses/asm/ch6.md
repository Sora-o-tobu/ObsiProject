
# Program Control Instructions

## 控制转移指令

根据跳转距离的远近，`jmp` 分为三类：

- **短跳(short jump)** ：跳转距离用一个字节（1 Byte）表示，机器码以 `EB` 开头
	- jump range 范围为 -128 to +127
    - 短跳后面只能接目标偏移地址或标号；而近跳还可以接 16 位寄存器或 16 位变量
    - 短跳机器码的 idata 为一个字节，对应大小为目标地址减去*下条指令*的偏移地址($+2)
- **近跳(near jump)** ：跳转距离或目标地址用一个字（2 Byte）表示，机器码以 `E9` 开头
    - 指令 `1D3E:0100: jmp 2000h` 对应机器码 `E9FD1E`，即跳转到地址 `1D3E:2000`
    - 其中 `FD1E` 含义为 `1EFD = 2000h - 0103h`，即目标地址减去下条指令的偏移地址
    - 偏移地址在机器码中也是小端存储
    - 也被称为段内跳转
- **远跳(far jump)** ：目标地址用一个远指针表示(段地址：偏移地址)，机器码以 `EA` 开头
    - 机器码 `EA0000FFFF` 对应指令 `jmp 0FFFFh:0000h` 。但是实际上远跳不能直接接常数地址。
    - 如果要跳转到不同段，一定是使用远跳；例如不同模式切换时
- **任务切换(Task Switch)** ：跳转到不同 task 中的指令处，只存在于 protection mode

```asm
data segment
addr dw 0000h, 0FFFFh
data ends

code segment
assume cs:code, ds:data
main:
	mov ax, data
	mov ds, ax

	; jmp 0FFFFh:0000h

	; jmp dword ptr 32位变量
	jmp dword ptr [addr]

	; 机器码方式
	db 0EAh
	dw 0
	dw 0FFFFh

	; 假定 extra 段已正确定义
	mov word ptr es:[di], 0000h
	mov word ptr es:[di+4], 0FFFFh
	jmp dword ptr es:[di]
code ends
end main
```

!!! info "主要通过指令机器码长度区分这三个跳"

不过我们编写代码的时候直接使用 `jcc` 指令即可，区分类型的事交给机器。

| Jcc指令     | 含义        | 跳转条件            | 解释                              |
| --------- | --------- | --------------- | ------------------------------- |
| **jc**    | 有进位则跳     | `CF==1`         | Jump if carry 有进位或借位            |
| **jnc**   | 无进位则跳     | `CF==0`         | Jump if no carry 无进位或借位         |
| **jz**    | 为零则跳      | `ZF==1`         | Jump if zero 运算结果为 0            |
| **jnz**   | 不为零则跳     | `ZF==0`         | Jump if not zero 运算结果不为 0       |
| **js**    | 有符号位则跳    | `SF==1`         | Jump if sign 符号数运算结果为负          |
| **jns**   | 无符号位则跳    | `SF==0`         | Jump if no sign 符号数运算结果为正       |
| **jo**    | 有溢出则跳     | `OF==1`         | Jump if overflow 符号数运算结果有错      |
| **jno**   | 无溢出则跳     | `OF==0`         | Jump if not overflow 符号数运算结果正确  |
| **jp**    | 有奇偶校验标志则跳 | `PF==1`         | Jump if parity 运算结果低八位1的个数为偶    |
| **jnp**   | 无奇偶校验标志则跳 | `PF==0`         | Jump if no parity 运算结果低八位1的个数为奇 |
| **jcxz**  | CX为零则跳    | `CX==0`         | Jump if CX is zero CX的值为 0      |
| **jecxz** | ECX为零则跳   | `ECX==0`        | Jump if ECX is zero ECX的值为 0    |
| **ja**    | 无符号大于则跳   | `CF==0 & ZF==0` | 与 `jnbe` 完全等价                   |
| **jae**   | 无符号大于等于则跳 |                 |                                 |
| **jb**    | 无符号小于则跳   |                 | 与 `jc`, `jnae` 完全等价             |
| **jbe**   | 无符号小于等于则跳 |                 |                                 |

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

另外，在书写汇编代码时我们可以通过伪指令 `.IF`, `.ELSE`, `.ELSEIF`, `.ENDIF`, `.WHILE`, `.ENDW`, `.REPEAT`, `.UNTIL` 等实现更简单的控制流。

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

=== "Near Call"
	![[ch6_5.png]]
	
	??? note
		Near Call 前后段寄存器 CS 均为 1000，因此 push 的返回地址只需要压入偏移 0003H，即过程调用返回为 10003H 
=== "Far Call"
	![[ch6_6.png]]
	
	??? note
		Far Call 前后段寄存器 CS 的值从 1000 变为 1100，因此 push 的返回地址需要压入 CS 和 IP 两个，过程调用返回地址为 1000:0005 = 10005H

如果函数有返回值，一般根据位数使用 `al,ax,eax` 来存储返回值。

!!! question "How to return 64 bit?"
	使用 `edx:eax` 分别存储高位和低位

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
main PROC
	; some STATEMENTS
	RET
main ENDP
```



## 中断与异常

中断的调用其实相当于内置函数的调用，中断函数的地址存储中断向量表中，在 8086 中位于内存 `0000:0000` 后，每四个字节表示一条中断指令。例如，对于如下所示的一段内存：

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

`int 21h` 的常用中断：

| AH  | 功能           | 补充解释                          |
| --- | ------------ | ----------------------------- |
| 01  | 有回显地读取一个标准输入 | AL=输入字符, 实际使用中还可以用作末尾敲任意键结束程序 |
| 02  | 输出字符到标准输出    | DL=输出字符                       |
| 09  | 输出字符串到标准输出   | DS:DX=字符串地址，一直到 '$' 停止输出      |
| 4C  | 带返回码终止       | AL=返回码                        |


## MISCELLANEOUS

HLT

NOP
最高只有 9B NOP，要补足 15B 可以多加 0x66

LOCK Prefix

BOUND: MEM 指示上下界

ENTER LEAVE：创建栈帧、释放栈帧从三条指令变为两条指令