
# Arithmetic & Logic Instructions

> 在原先 8086 汇编语言笔记的基础上新增部分指令

## 算术指令

### 加法指令

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">ADD</font>
	
	- **指令格式：** add dest, src
	- **指令效果：** dest += src

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">ADC</font>
	
	- **指令格式：** adc dest, src
	- **指令效果：** dest += src + CF
	- **注意：** 即带进位加法，add with carry

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">INC</font>
	
	- **指令格式：** inc op
	- **指令效果：** op++
	- **注意：** inc 指令不影响 CF

!!! example "习题"
	求 12345678h+4243BCBCh 的和，要求结果存放在寄存器DX:AX中：
	
	```asm
	mov ax, 5678h
	mov dx, 1234h
	add ax, 0BCBCh
	adc dx, 4243h
	```

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">XADD</font>
	
	- **指令格式：** xadd dest, src
	- **指令效果：** src = dest; dest += src
		- 例如 xadd ah(1000h), bh(2000h)，执行完后 ah = 3000h, bh = 1000h
	- **注意：** exchange and add

### 减法指令

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">SUB</font>
	
	- **指令格式：** sub dest, src
	- **指令效果：** dest -= src

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">SBB</font>
	
	- **指令格式：** sbb dest, src
	- **指令效果：** dest = dest - src - CF
	- **注意：** 即带借位减法，subtract with borrow

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">DEC</font>
	
	- **指令格式：** dec op
	- **指令效果：** op--
	- **注意：** dec 指令不影响 CF

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">NEG</font>
	
	- **指令格式：** neg op
	- **指令效果：** op = -op
	- **注意：** 相当于取反加一(求补码, op = not op + 1)，非零数求补后 CF = 1，0 求补后 CF = 0

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">CMP</font>
	
	- **指令格式：** cmp op1, op2
	- **指令效果：** compare op1 - op2
	- **注意：** cmp 指令并不计算差值，但会像 sub op1, op2 影响状态标志，后面通常接跳转指令

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">CMPXCHG</font>
	
	- **指令格式：** cmpxhg dest, src
	- **指令效果：**
		- 如果 dest == accu，那么 dest = src，ZF = 1
		- 如果 dest <> accu，那么 accu = dest，ZF = 0
	- **注意：** cmpxhg 指令隐式使用了 A 系列寄存器作为操作数，并且会影响状态标志 ZF

!!! example "试写出对存放在DX:AX中的32位数进行求补(neg)的汇编指令序列"
	```asm
	neg ax    ; 若 ax 非零，则产生进位 CF=1
	pushf     ; 保存 CF 状态
	neg dx
	popf      ; 恢复 CF 状态
	sbb dx, 0 ; 减去 neg ax 产生的进位
	```

### 乘法指令

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">MUL</font>
	
	- **指令格式：** mul src
	- **指令效果：** 非符号数乘法
		- src 为 8 位时，ax = al * src
		- src 为 16 位时，dx:ax = ax * src
		- src 为 32 位时，edx:eax = eax * src
	- **Example:** DX:AX=`123400h` ➡ DX=`0012h`, AX=`3400h` 

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">IMUL</font>
	
	- **指令格式：** imul src
	- **指令效果：** 符号数乘法
		- src 为 8 位时，ax = al * src
		- src 为 16 位时，dx:ax = ax * src
		- src 为 32 位时，edx:eax = eax * src

!!! info "IMUL 的第二类用法"
	Intel 第二代CPU为`IMUL`新增了用法，支持两个或三个操作数。以下四组指令都实现对`eax`乘10，不同的是，`lea`和`imul`指令并不能处理乘法的溢出：
	
	```asm
	mov ebx, 10
	mul ebx               ; edx:eax = eax * ebx
	
	mov ebx, 10
	imul eax, ebx         ; eax = eax * ebx
	
	imul eax, eax, 10     ; eax = eax * Imm
	
	add eax, eax
	lea eax, [eax+eax*4]  ; eax = eax*2 + eax*8
	```

### 除法指令

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">DIV</font>
	
	- **指令格式：** div src
	- **指令效果：** 非符号数除法
		- src 为 8 位时，al = ax / src, ah = ax % src
		- src 为 16 位时，ax = dx:ax / src, dx = dx:ax % src
		- src 为 32 位时，eax = edx:eax / src, edx = edx:eax % src
	- **注意：** 若除数为 0 或者保存商的寄存器无法容纳商时会发生除法溢出，此时 CPU 会在除法指令上方插入并执行一条 int 00h 指令(除法的异常处理)


!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">IDIV</font>
	
	- **指令格式：** idiv src
	- **指令效果：** 符号数除法
		- src 为 8 位时，al = ax / src, ah = ax % src
		- src 为 16 位时，ax = dx:ax / src, dx = dx:ax % src
		- src 为 32 位时，eax = edx:eax / src, edx = edx:eax % src
	- **注意：** 若除数为 0 或者保存商的寄存器无法容纳商时会发生除法溢出，此时 CPU 会在除法指令上方插入并执行一条 int 00h 指令


## 逻辑运算和移位指令

### 逻辑运算指令

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">AND</font>
	
	- **指令格式：** and dest, src
	- **指令效果：** dest = dest & src

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">OR</font>
	
	- **指令格式：** or dest, src
	- **指令效果：** dest = dest | src

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">XOR</font>
	
	- **指令格式：** xor dest, src
	- **指令效果：** dest = dest \^ src

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">NOT</font>
	
	- **指令格式：** not op
	- **指令效果：** op = ^op
	- **注意：** 所有逻辑运算中，只有 NOT 不影响任何状态标志

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">TEST</font>
	
	- **指令格式：** test dest, op
	- **指令效果：** test dest & src
	- **注意：** test 指令并不保存结果值，但会像 and dest, src 一样影响状态标志

!!! abstract "80386 - Pentium 4 有四种测试单个比特的 test 指令"
	![[ch5_1.png]]
	
	- `BT AX, 4` 测试 AX 的第四位，CF = AX[4]
	- `BTS CX, 9` 设置 CX[9] = 1
	- `BTR CX, 9` 设置 CX[9] = 0
	- `BTC CX, 9` 设置 CX[9] = ~CX[9]

### 移位指令

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">SHL</font>
	
	- **指令格式：** shl dest, count
	- **指令效果：** dest <<= count & 1Fh
	- **注意：** 右侧补0，左侧溢出位落入 CF；count 只能是 idata 或 cl
		- 若为 .386，则 idata 可为任意大小的 8 位常数
		- 若为 8086，则 idata 只能为 1
		- 其它移位指令和 shl 指令规定相同，不再做阐述

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">SHR</font>
	
	- **指令格式：** shr dest, count
	- **指令效果：** dest >>= count & 1Fh
	- **注意：** 左侧补0，右侧溢出位落入 CF；count 只能是 idata 或 cl

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">SAL</font>
	
	- **指令格式：** sal dest, count
	- **指令效果：** dest <<= count & 1Fh
	- **注意：** 算术左移指令，与 SHL 完全相同

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">SAR</font>
	
	- **指令格式：** sar dest, count
	- **指令效果：** dest >>= count & 1Fh
	- **注意：** 左侧不变，右侧溢出位落入 CF；count 只能是 idata 或 cl
		- 使用算术右移做除法，得到的结果与 IDIV 并不一致，IDIV 将结果向 0 取整，SAR 将结果向 -inf 取整

??? question "通过对被除数进行补偿，达到 SAR 和 IDIV 等价的效果，具体是如果被除数为负则加上 $2^k-1$"
	=== "C function"
		```c
		int idiv(int x) { return x/8; }
		```
	=== "assembly"
		```asm
		; x86-64 clang 13.0.0 -O3
		lea eax, [rdi+7]  ; eax = rdi + 7
		test edi, edi     ; if edi >= 0
		cmovns eax, edi   ;   eax = edi
		sar eax, 3        ; return eax >> 3
		; 实际上这一套操作比直接 idiv 的运行速度更快
		```

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">ROL</font>
	
	- **指令格式：** rol dest, count
	- **指令效果：** 对 dest 循环左移 count & 1Fh 位，最高位回到最低位同时移到 CF 中
	- **注意：** rotate left；count 只能是 idata 或 cl

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">ROR</font>
	
	- **指令格式：** ror dest, count
	- **指令效果：** 对 dest 循环右移 count & 1Fh 位，最低位回到最高位同时移到 CF 中
	- **注意：** rotate right；count 只能是 idata 或 cl

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">RCL</font>
	
	- **指令格式：** rcl dest, count
	- **指令效果：** 带进位循环左移，即 CF 加在 dest 左侧一起循环左移
	- **注意：** count 只能是 idata 或 cl

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">RCR</font>
	
	- **指令格式：** rcr dest, count
	- **指令效果：** 带进位循环右移，即 CF 加在 dest 右侧一起循环右移
	- **注意：** count 只能是 idata 或 cl

!!! example "试写出把32位数 3F7E59ACh 逻辑左移2位的汇编指令序列，要求结果存放在DX:AX中"
	```asm
		mov ax, 59ACh
		mov dx, 3F7Eh
		mov cx, 2
	again:
		shl ax, 1
		rcl dx, 1
		loop again
	```



## 十进制调整指令

**Binary Coded Decimal** 是指用二进制编码的十进制数。

- **压缩BCD码** 以4位二进制表示1位十进制
	- $37_{Decimal}\Rightarrow 37h$
- **非压缩BCD码** 以8位二进制表示1位十进制
	- $37_{Decimal}\Rightarrow 0307h$
	- 高位没有意义，可以为任意值，如 `06h`,`36h` 都表示十进制数 6

<font style="font-weight: 1000;font-size: 22px;" color="orange"># 压缩BCD码调整指令</font>


!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">DAA</font>
	
	- **指令格式：** daa
	- **指令效果：** 压缩BCD码加法的十进制调整，将AL调整回BCD码
		- if (af == 1 || (al&0Fh) > 9) al += 6, af = 1; else af = 0
		- if (cf == 1 || al > 9Fh) al += 60h, cf = 1; else cf = 0

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">DAS</font>
	
	- **指令格式：** das
	- **指令效果：** 压缩BCD码减法的十进制调整，将AL调整回BCD码
		- if (af == 1 || (al&0Fh) > 9) al -= 6, af = 1; else af = 0
		- if (cf == 1 || al > 9Fh) al -= 60h, cf = 1; else cf = 0


<font style="font-weight: 1000;font-size: 22px;" color="orange"># 非压缩BCD码调整指令</font>

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">AAA</font>
	
	- **指令格式：** aaa
	- **指令效果：** 加法的 ASCII 调整，在 al 被做加法后调整 ax 为非压缩BCD码(ah也被修改)
		- if (af == 1 || (al&0Fh) > 9) al = (al+6)&0Fh, ah += 1, af = 1, cf = 1
		- else af = 0, cf = 0

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">AAS</font>
	
	- **指令格式：** aas
	- **指令效果：** 减法的 ASCII 调整，在 al 被做减法后调整 ax 为非压缩BCD码(ah也被修改)
		- if (af == 1 || (af&0Fh) > 9) al = (al-6)&0Fh, ah -= 1, af = 1, cf = 1
		- else af = 0, cf = 0

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">AAM</font>
	
	- **指令格式：** aam
	- **指令效果：** 乘法的 ASCII 调整，在 al 被做乘法后调整 ax 为非压缩BCD码(ah也被修改)
		- ah = al / 10, al = al % 10

!!! quote ""
	<font style="font-weight: 1000;font-size: 24px">AAD</font>
	
	- **指令格式：** aam
	- **指令效果：** 除法的 ASCII 调整，在 al 被做除法后调整 ax 为非压缩BCD码(ah也被修改)
		- al = ah * 10 + al, ah = 0
