
# Program Control Instructions

## 控制转移指令

根据跳转距离的远近，`jmp` 分为三类：

- **短跳(short jump)** ：跳转距离用一个字节表示，机器码以 `EB` 开头
	- 短跳后面只能接目标偏移地址或标号；而近跳还可以接 16 位寄存器或 16 位变量
	- 短跳机器码的 idata 为一个字节，对应大小为目标地址减去下条指令的偏移地址($+2)
- **近跳(near jump)** ：跳转距离或目标地址用一个字表示，机器码以 `E9` 开头
	- 机器码 `E9FD1E` 对应指令 `1D3E:0100: jmp 2000h` 即跳转到地址 `1D3E:2000`
	- 其中 `FD1E` 含义为 `1EFD = 2000h - 0103h`，即目标地址减去下条指令的偏移地址
	- 偏移地址在机器码中也是小端存储
- **远跳(far jump)** ：目标地址用一个远指针表示(段地址：偏移地址)，机器码以 `EA` 开头
	- 机器码 `EA0000FFFF` 对应指令 `jmp 0FFFFh:0000h` 。但是实际上远跳不能直接接常数地址，下面两种方式可以实现相同效果：

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