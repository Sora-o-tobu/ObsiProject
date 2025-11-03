
# Data Movement Instructions

> This chapter concentrates on the data movement instructions.
> The data movement instructions include `MOV`, `MOVSX`, `MOVZX`, `PUSH`, `POP`, `BSWAP`, `XCHG`, `XLAT`, `IN`, `OUT`, `LEA`, `LDS`, `LES`, `LFS`, `LGS`, `LSS`, `LAHF`, `SAHF`.
> String instructions: `MOVS`, `LODS`, `STOS`, `INS`, and `OUTS`.
> Data movement instructions **do not** affect flags.


## 编码格式

!!! danger "cm 老师说往年都没有考，今年要考（2025-2026 秋冬）"

在 code segment descriptor 中，存在两个位指示了指令的长度模式：

- L=0 and D/B =0 for 16-bit instruction mode
- L=0 and D/B =1 for 32-bit instruction mode
- L=1 for 64-bit instruction mode

![[ch4_1.png]]

在不同运行模式下，同一指令的编码可能不一样。下面我们对比如下两种 format：

![[ch4_2.png]]

例如，对于如下指令，它在 80286（16-bit）和 80386（32-bit）下编码就有所不同：

```asm
in .286
MOV WORD PTR [BX], 5 ;       C7 07 05 00

in .386
MOV WORD PTR [BX], 5 ; 67 66 C7 07 05 00
					 ; 因为 16-bit 不是 80386 的默认长度
```

x86 中，指令长度范围为 1 Byte - 15 Bytes。我们尝试分析指令编码中的几个关键字段：

- **Opcode**
	- 如果 Direction Bit = 1，数据流从 R/M field 流向 REG field，即 `MOV REG, R/M` （Register or Memory）
	- 如果 Direction Bit = 0，数据流从 REG field 流向 R/M field，即 `MOV R/M, REG`

!!! warning "为什么不像 RISC-V 一样指定 rs 和 rd 呢？"
	这是因为 x86 中寻址不仅仅是寄存器寻址，还有内存寻址。所以 x86 选择固定寻址的两个域以及数据流的方向。



![[ch4_3.png]]

`8A5501H`，`MOV DL, [DI+1]`，MOD 变了，加了 disp



32-bit 中的 Scaled-Index Byte 格式如下：

```
    7   6   5   4   3   2   1   0
+---+---+---+---+---+---+---+---+
| S   S |   Index   |   Base    |
+---+---+---+---+---+---+---+---+
```

当 `R/M` 等于 `100` 时，Scaled-Index Byte 被使用：

![[ch4_4.png]]



64-bit 下，... TODO

## Instruction Set

本节主要讲解相关数据操作指令。

### LOAD EFFECTIVE ADDRESS

Load-effective address instruction 被设计用来支持 high-level language，其类型分为两种：

- `LEA`: 加载 near point，即 offset
- `LDS`, `LES`, `LFS`, `LGS`, `LSS`: 加载 far point，即 segment selector & offset

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">LEA</font>
	
	- **指令格式：** lea dest, src
	- **指令效果：** 取变量的偏移地址
	- **注意：** 等效于 `mov dx, offset abc` 但是可以用来作加法
		- `lea dx, ds:[bx+si+3]` 正确
		- `mov dx, bx+si+3` 错误
		- `lea eax, [eax+eax*4]` 利用 `lea` 做常数乘法
		- 指示符 `OFFSET` 会在编译时替换为立即数，因此效率更高；但需要动态取址是用不了


!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">LES</font>
	
	- **指令格式：** les dest, src
	- **指令效果：** 将src指向地址的高十六位写入es，低十六位写入dest
	- **注意：** 由于采取小端存储法，所以是后面的存入es，例如：
		- 从 `ds:[bx]` 连续存放四个字节: `9A,78,56,34`
		- `les di, ds:[bx]`
		- `ES=3456h, DI=789Ah`

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">LDS</font>
	
	- **指令格式：** lds dest, src
	- **指令效果：** 将src指向地址的高十六位写入ds，低十六位写入dest
	- **注意：** 注意事项同上

???+ example
	设有一内存字X，X的偏移地址与段地址按顺序存放在从地址 1000:10F0 起的内存单元中，请写出汇编指令把X的值赋值给寄存器AX。(要求使用LDS指令)
	
	```asm
	mov ax, 1000h
	mov ds, ax
	lds bx, dword ptr ds:[10F0h]
	mov ax, [bx]
	```
	
	设有一内存字节Y，Y的偏移地址与段地址按顺序存放在从地址1000:10F0起的内存单元中，请写出汇编指令把Y的值加1。(要求使用LES指令)
	
	```asm
	mov ax, 1000h
	mov ds, ax
	les di, dword ptr ds:[10F0h]
	inc byte ptr es:[di]
	```

最常使用的取远地址指令是 `LSS`，它可以用来更方便地切换栈：

```asm
CLI

# SAVE OLD SP, SS
MOV AX, SP
MOV WORD PTR SADDR, AX
MOV AX, SS
MOV WORD PTR SADDR+2, AX

# LOAD NEW SP, SS
MOV AX, DS
MOV SS, AX
MOV AX, OFFSET STOP
MOV SP, AX
STI

# DO STH
# DO STH
# DO STH

# RESTORE OLD STACK
LSS SP, SADDR
```

### STRING DATA TRANSFER

!!! quote "x86 指令只有两个操作数，因此不得不隐式使用各种默认寄存器"

字符串操作指令包括：

- `movs` 字符串复制 move string 
- `cmps` 字符串比较 compare string
- `scas` 字符串搜索 scan string
- `stos` 写入字符串 store string
- `lods` 读取字符串 load string
- `ins` 从 io 接口读取字符串 in string
- `outs` 向 io 接口写入字符串 out string

与字符串操作指令相关的指令前缀包括：

- `rep` 重复 repeat
- `repe` 若相等则重复 repeat if equal
- `repz` 若结果为零则重复 repeat if zero ，与 `repe` 等效
- `repne` 若不相等则重复 repeat if not equal
- `repnz` 若不为零则重复 repeat if not zero , 与 `repne` 等效

字符串操作指令可以与指令前缀结合使用，也可以单独使用。单独使用表示仅执行一次字符串操作，而加了指令前缀则可以最多重复执行 **CX** 次。

指令后缀 `B` 则表示操作数的大小。

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">REP MOVSB</font>
	
	- **指令格式：** rep movsb
	- **指令效果：** 以字节为单位从 `DS:[SI]` 复制字符串到 `ES:[DI]`
		- byte ptr ES:[DI] = byte ptr DS:[SI];
		- if(DF == 0) {SI++; DI++} 即正向复制
		- if(DF == 1) {SI--; DI--} 即反向复制
		- CX--; if(CX == 0) goto DONE;

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">REPE CMPSB</font>
	
	- **指令格式：** repe cmpsb
	- **指令效果：** 以字节为单位比较 `DS:[SI]` 与 `ES:[DI]` 的字符串，并将 SI 和 DI 停留在第一次不相等的地方
		- cmp byte ptr ES:[DI], byte ptr DS:[SI];
		- if(DF == 0) {SI++; DI++} 即正向比较
		- if(DF == 1) {SI--; DI--} 即反向比较
		- CX--; if(CX == 0 || ZF != 0) goto DONE;
	- **注意:** `CMPS` 指令不影响标志位，当退出该指令时，标志寄存器 FL 恢复;但是仍然可以用 `je` 等指令实现判断跳转

可以自己试试下面的程序的执行效果：

```asm
comment @
将字符串s复制到到t中，并比较s和t是否相等
@

data segment
s db "I hate Monsterhunter:wild", 0Dh, 0Ah    ; , '$'
slen =  $ - offset s
equal_msg db "Equal", 0Dh, 0Ah, '$'
unequal_msg db "Not Equal", 0Dh, 0Ah, '$'
data ends

extra segment
t db slen dup(0)
extra ends

code segment
assume cs:code, ds:data, es:extra
compare:
	mov si, offset s           ; 为什么这里需要重新对 si 和 di 赋值？
							   ; 因为 movsb 指令已经对其进行了++操作
	mov di, offset t
	mov cx, slen
	repe cmpsb                 ; 比较字符串
	je is_equal
not_equal:
	mov dx, offset unequal_msg
	jmp output
is_equal:
	mov dx, offset equal_msg
output:
	mov ah, 9                  ; 输出 ds:[dx] 的字符串，直到遇到 '$'
	int 21h
	ret

main:
	mov ax, data
	mov ds, ax
	mov ax, extra
	mov es, ax
	mov si, offset s
	mov di, offset t
	mov cx, slen
	cld                        ; DF = 0
	rep movsb                  ; 字符串复制
	mov cx, slen
	mov bx, 0
L1:
	mov dl, byte ptr es:[bx]   ; 取数据，不能是 t:[bx]；byte ptr 不加也行
	mov ah, 2                  ; 如果字符串以 '$' 结尾，则可以直接使用ah = 9输出dx中字符串
	int 21h
	inc bx
	loopnz L1                  ; CX--,若cx不等于零，则循环
	call compare
exit:
	mov ah, 1h
	int 21h
	mov ah, 4ch
	int 21h
code ends
end main
```

!!! note "$"
	这里数据段中有一句 `slen = $ - offset s`，作用是计算数组 s 的长度。但是 `slen` 本身不会出现在数据段内，而是类似于宏的存在。

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">REPE SCASB</font>
	
	- **指令格式：** repe scasb
	- **指令效果：** 在 `ES:[DI]` 指向的目标字符串中搜索 AL|AX|EAX 的值
	- **注意:** 基本原理基本同上，一般只利用 CX 前后的变化求字符串长度

用 `repne scasb` 求字符串长度：

```asm
data segment
s db "Sakura no toki", 0
slen dw 0
data ends

code segment
assume cs:code, ds:data
main:
	mov ax, data
	mov ds, ax
	mov es, ax
	mov di, offset s
	mov cx, 0FFFFh     ; 最大搜索次数
	xor al, al         ; AL = 0h
	cld                ; DF = 0
	repne scasb
	inc cx             ; 00h 不算在字符串内
	not cx             ; CX = 0FFFFh - CX = 搜索过的字符个数
	mov [slen], cx
	mov ah, 4Ch
	int 21h
code ends
end main
```

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">REP STOSB</font>
	
	- **指令格式：** rep stosb
	- **指令效果：** 把 AL|AX|EAX 的值写入 `ES:[DI]` 指向的目标字符串
	- **注意:** 基本原理基本同上

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">REP LODSB</font>
	
	- **指令格式：** rep lodsb
	- **指令效果：** 从 `ES:[DI]` 中读取 x 个字节保存在 AL|AX|EAX 中
	- **注意:** 基本原理基本同上，x 为对应的字节长度

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">INS / OUTS</font>
	
	- **指令格式：**  
		- INSB / INSW / INSD
		- OUTSB / OUTSW / OUTSD
	- **指令效果：**  
		这两条都是**串操作指令**，用于在端口与内存之间进行数据块传输，配合 `REP`、`REPE`、`REPNE` 等前缀可实现批量输入输出。  
		- `INS`：从 I/O 端口读入数据 → 写入到内存地址 `ES:[DI]`
		- `OUTS`：从内存地址 `DS:[SI]` 读数据 → 写入到 I/O 端口
		- 若方向标志 `DF = 0`：`SI`、`DI` **递增**（正向传输）  
		- 若方向标志 `DF = 1`：`SI`、`DI` **递减**（反向传输）  
		- 执行完一次后，若带有 `REP` 前缀，则会自动 `CX--` 并循环直到 `CX=0`
	- **注意事项：**  
		- 1. `INS` 默认目的段为 **ES**，而源为 **I/O 端口**，因此不能更改目的段寄存器。  
		- 2. `OUTS` 默认源段为 **DS**，而目的为 **I/O 端口**，同样不能修改目标段。  
		- 3. 两者都使用 **DX** 指定端口号，**必须保证端口已被正确映射或允许访问**。  
		- 4. 一般在系统级编程、设备驱动或操作系统中使用，**用户态程序通常不可直接使用**。  
		- 5. 当使用 `REP INS` 或 `REP OUTS` 时，CPU 会自动根据 `CX` 的值进行批量 I/O 操作。


### MISCELLANEOUS DATA TRNASFER

最后是一些不好分类的数据传输指令，包括 `XCHG`, `LAHF`, `SAHF`, `XLAT`, `IN`, `OUT`, `BSWAP`, `MOVSX`, `MOVZX`, `CMOV` 等。

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">XCHG</font>
	
	- **指令格式：** xchg op1, op2
	- **指令效果：** 交换 op1 和 op2
	- **注意：** `xchg` 不影响任何标志位；且不能对段寄存器操作

`XHCG` 可以用来在进程同步之间实现自旋锁，例如：

```asm
# lock variable 1: locked, 0: unlocked
locked dd 0

spin_lock:
	# set EAX = 1
	mov eax, 1

	lock xchg eax, [locked]

	test eax, eax

	# if eax = 0, we obtain the lock
	# otherwise, repeatedly request
	jnz spin_lock
	ret
```

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">LAHF</font>
	
	- **指令格式：** lahf
	- **指令效果：** 将 EFLAGS 的低八位放入 AH
	- **注意：** AH := (SF:ZF:0:AF:0:PF:1:CF)

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">SAHF</font>
	
	- **指令格式：** sahf
	- **指令效果：** 从 AH 读入 EFLAGS 的低八位

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">XLAT</font>
	
	- **指令格式：** xlat
	- **指令效果：** al = byte ptr ds:[bx+al]
	- **注意：** 相当于 bx 是基地址，al 是偏移地址，用来查表操作

利用 xlat 指令和转换表将十进制整数转换成十六进制并输出：

```asm
.386
data segment use16
t db "0123456789ABCDEF"
x dd 2147483647
data ends

code segment use16
assume cs:code, ds:data
main:
  mov ax, data      ;\
  mov ds, ax        ; | ds:bx->t[0]
  mov bx, offset t  ;/
  mov cx, 8
  mov eax, [x]
next:
  rol eax, 4
  push eax
  and eax, 0Fh
  xlat
  mov ah, 2
  mov dl, al
  int 21h
  pop eax
  loop next
  mov ah, 4Ch
  int 21h
code ends
end main
```

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">MOVSX</font>
	
	- **指令格式：** movsx dest, src
	- **指令效果：** 把值 src 符号扩充至 dest 中
	- **注意：** dest 的位宽需大于 src
	- **例子：**
		- `movsx bx, al ; al = 80h` => `bx = 0FF80h`

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">MOVZX</font>
	
	- **指令格式：** movzx dest, src
	- **指令效果：** 把值 src 零扩充至 dest 中
	- **注意：** dest 的位宽需大于 src
	- **例子：**
		- `movzx bx, al ; al = 80h` => `bx = 0080h`

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">BSWAP</font>
	
	- **指令格式：** bswap reg
	- **指令效果：** 逆向指定寄存器的 byte order，相当于大端小端格式转换
	- **注意：** 其 opcode 格式较特殊 `[REX.W, 0F, C8 + rd]`

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">CMOVcc</font>
	
	- **指令格式：** cmovcc dest, src
	- **指令效果：** 如果满足条件 `cc`，则执行 mov
	- **注意：** cc 可以是 `AE`, `BE`, `Z`, `G`, `E` 等等，与控制转移指令类似

???+ example "使用 `cmovcc` 代替分支，增大基本块和调度范围"
	![[ch4_5.png]]

