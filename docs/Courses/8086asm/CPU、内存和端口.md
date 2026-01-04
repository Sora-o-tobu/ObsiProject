
## 数据的表示方式

汇编语言使用关键词 **db** (Define Byte) 来定义字节类型的变量，展开来讲即：

| 关键词    | 位数  | 符号             | 变量                                                            |
| ------ | --- | -------------- | ------------------------------------------------------------- |
| **db** | 8   | byte           | `char`                                                        |
| **dw** | 16  | word           | `short int`                                                   |
| **dd** | 32  | double word    | `long int` 或 `float`                                          |
| **dq** | 64  | quadruple word | `__int64` , `long long` (十进制 `%lld` , 十六进制 `%llx`) 或 `double` |
| **dt** | 80  | ten bytes      | `long double` （`%Lf`）                                         |

!!! info "1 word 的位数表示一个系统单步能处理的最大位数"

那么变量定义的格式如下：

```asm
变量名 db|dw|dd|dq|dt 初始值
```

当我们需要多个相同的初始值时，可以使用 DUP 运算符：

```asm
xyz dw 1000h dup(55AAh) ; 共1000h个元素，每个元素值都初始化为55AAh
x db 3 dup(1,2) ; 共6个元素，分别为 1，2，1，2，1，2
y db 2 dup('A',3 dup('B'), 'C') ; y = "ABBBCABBBC"
```

## 内存

内存以字节为单位分配地址，DOS系统运行在 CPU 的实模式下，可访问的地址范围为 `[00000h,0FFFFFh]` ，即最多只能访问 **1MB** 内存空间

我们将形如 `0FFFFFh` 的单个数值表示的地址称作物理地址。由于 8086 每个寄存器都均为 16 位宽度，没有一个寄存器能容纳一个 20 位的物理地址，为此 Intel 公司设计 8086 时定义了 4 个段地址寄存器和 4 个偏移地址寄存器，以便用逻辑地址形式即 `Seg:Offset` 组合来间接访问物理地址。

段地址和偏移地址两个概念都和段（segment）有关：

- 偏移地址指段内某个变量或标号与段首之间的距离
- 段地址是指 20 位段首地址的高 16 位

!!! note "段 Segment"
	段是符合以下两个条件的一块内存空间：
	
	- 内存块的长度位 10000h 字节，即 64KB
	- 内存块的 20 位首地址的低 4 位必须等于 0
	
	例如，对于段 `[12340h,2233Fh]` ，其段地址为 `1234h`

### 逻辑地址

在 8086 中，逻辑地址由 `16bit Seg:16bit Offset` 构成，可以根据如下公式将其转换成物理地址：

$$
\text{phy addr} = \text{Seg} \times 10h + \text{Offset}
$$

在汇编语言程序中，如果要访问逻辑地址 `1234h:5678h` (物理地址 `179B8h`) 指向的字节并赋值给寄存器 AL，可以这样写：

```asm
mov ax, 1234h
mov ds, ax ; 以 s 结尾的寄存器不能接受常数给它赋值
mov al, ds:[5678h] ; 段地址只能用段寄存器表示，1234h:[5678h] 是错误的写法
```

- 用 `offset 变量名或标号名` 引用变量或标号的偏移地址
- 用 `seg 变量名或标号名或段名` 引用段地址

在引用变量或数组元素时，若用常数表示它们的偏移地址，称这种寻址方式为直接寻址，如 `ds:[5678h]` ；若它们的偏移地址含有寄存器，称这种寻址方式为间接寻址，如 `ds:[bx+2]`

在源程序中，设某个变量或数组名字为 `var` ，则直接寻址的一般形式为：`段寄存器:var[常数]` 或 `段寄存器:[var+常数]` 。经过编译后变量名或数组名会替换成它们的偏移地址。例如，假设 `offset s=8` ，那么 `ds:[s+1]` , `ds:s[-2]` 分别编译成 `ds:[9]` 和 `ds:[6]`

8086 规定的间接寻址一般形式为： `段寄存器:[寄存器1+寄存器2+常数]` ，其中两个寄存器可以同时存在也可以缺少一个。仅有 `BX`,`BP`,`SI`,`DI` 这四个寄存器允许出现在方括号中，且 `BX` 和 `BP` 不能同时出现；`SI` 和 `DI` 不能同时出现。

!!! danger "缺省段址"
	若逻辑地址中省略段地址，则使用默认缺省值：
	
	- 直接寻址方式缺省段址为 DS
	- 间接寻址依据偏移地址是否含有寄存器 `BP` 区分
		- 若有，则缺省段址为 SS
		- 若无，则缺省段址为 DS

### 小端规则

Intel 用的是小端存储法(little-endian)：

```asm
a dd 12345678h ; 假设其地址在 1000
		小端		大端
+1000	0x78	0x12
+1001	0x56	0x34
+1002	0x34	0x56
+1003	0x12	0x78

---

t dw 89ABh, 0CDEFh ; t是一个数组
;t+0 AB
;t+1 89
;t+2 EF
;t+3 CD

mov ax, t[2] ; 也可以写成 mov ax, [t+2] 取到 CDEF
mov ax, t[1] ; 取的就是 EF89 所以数组取的方法不能跟C语言搞混 
```

### 内存空间划分和显卡地址映射

16 位 CPU 只能访问 `0000:0000` 至 `F000:FFFF` 之间的 1M 内存空间，其划分如下：

| 地址范围                    | 用途        |
| ----------------------- | --------- |
| `[0000:0000,9000:FFFF]` | 操作系统和用户程序 |
| `[A000:0000,A000:FFFF]` | 映射显卡内存    |
| `[B000:0000,B000:7FFF]` | 映射显卡内存    |
| `[B800:0000,B800:7FFF]` | 映射显卡内存    |
| `[C000:0000,F000:FFFF]` | 映射 ROM    |

所以操作系统和用户程序总共可用的内存空间为 `[0,9FFFF]` 共 640KB

## 寄存器

8086 一共有 14 个 16 位寄存器：

- 通用寄存器：包括 `AX`,`BX`,`CX`,`DX` ，用于算术、逻辑、移位运算
- 段地址寄存器：包括 `CS`,`DS`,`ES`,`SS` ，用来表示段地址
- 偏移地址寄存器：包括 `IP`,`SP`,`BP`,`SI`,`DI` ，用来表示偏移地址
- 标志寄存器：包括 `FL` ，用来存储标志位

!!! info "80386中，除了段寄存器外所有寄存器均扩充到 32 位宽度，前面加上 `E`"

### 通用寄存器

通用寄存器包括 `AX`,`BX`,`CX`,`DX` ，作用是做算术、逻辑、移位运算。其中 `AX` 的高八位和低八位成为 `AH`,`AL` ，其它寄存器同理。

在 80386 中，32 位寄存器 `EAX` 低 16 位就是 `AX` ，而高 16 位没有名字。

!!! note "其实命名不是按照字母表前四个字母来的，不过我们写程序的时候随意用也没关系"
	- `AX`: Accumulator 累加器
	- `BX`: Base 基地址寄存器
	- `CX`: Counter 计数器
	- `DX`: Data 数据寄存器

### 段地址寄存器

段地址寄存器包括 `CS`,`DS`,`ES`,`SS` 

- `CS` ：代码段寄存器
- `DS` ：数据段寄存器
- `ES` ：附加段寄存器
- `SS` ：堆栈段寄存器

其中 `CS` 不能用 `mov` 指令赋值；`DS`,`ES`,`SS` 可以用 `mov` 赋值，但源操作数不能是常数，且如果是寄存器的话仅限 `AX`,`BX`,`CX`,`DX`,`SP`,`BP`,`SI`,`DI` (除 `IP` 外)，位宽必须是 `word ptr` 宽度。

!!! question "如何把 `DS` 赋值成 `CS` ？"
	```asm
	push cs
	pop ds
	```

DOS 把某个 EXE 载入到内存后，在将控制权交给程序之前，会对以下寄存器赋初值：

- `CS` = 代码段的段地址
- `IP` = 首条指令的偏移地址
- `SS` = 堆栈段的段地址
- `SP` = 堆栈段的长度
- `DS` = PSP 段址
- `ES` = PSP 段址

`CS:IP` 即指向当前将要执行的指令；`SS:SP` 指向堆栈顶端；**PSP** 是程序段前缀（program segment prefix），它是一个由 DOS 分配给当前 EXE 的，位于首段之前的，长度为 100h 字节的内存块，PSP 中存储了与当前 EXE 进程相关的一些信息。

由于 `DS` 的初始值并非数据段的段地址，因此在程序刚开始运行时不能用 `DS` 的当前值作为段地址去引用或访问数据段内任何一个变量，而是应该先把 `DS` 赋值为 seg data

```asm
	mov ax, data
	mov ds, ax
```

### 偏移地址寄存器

偏移地址寄存器包括 `IP`,`SP`,`BP`,`SI`,`DI` 

- `IP` 需要跟 `CS` 搭配构成 `CS:IP` 指向当前将要执行的指令，所以该寄存器的名字不能在任何指令中引用
- `SP` 需要跟 `SS` 搭配构成 `SS:SP` 指向堆栈顶端，该寄存器不能置于 \[\] 内用于间接寻址
- 剩下的三个寄存器可以用来间接寻址，但总共有四个段寄存器，所以 Intel 安排通用寄存器 `BX` 也可以用来间接寻址
	- `BX`,`BP`,`SI`,`DI` 四个寄存器除了用来间接寻址外，都可以参与算术、逻辑、移位运算
	- 寄存器 `BP` 默认跟栈搭配，因此指令 `mov si, [bp+6]` 等效于 `mov si, word ptr ss:[bp+6]`

!!! danger "寄存器相对寻址时，方括号内必须是`BX,BP,SI,DI`，不然会报错"

### 标志寄存器

`FL` 是标志寄存器，它里面有些位用来反应当前指令的执行状态，称为**状态标志**；有些位用于控制 CPU，称为**控制标志**；剩余 7 位是保留位，除了第一位恒为 `1` 外，其它保留位恒为 `0`。

- 状态标志共 6 个，包括：`CF`.`ZF`,`SF`,`OF`,`PF`,`AF`
- 控制标志共 3 个，包括：`DF`,`IF`,`TF`

```
 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|  |  |  |  |OF|DF|IF|TF|SF|ZF|  |AF|  |PF|  |CF|
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
```

#### 状态标志

<font style="font-weight: 1000;font-size: 24px;" color="red">进位标志 CF</font>

- `add`,`sub`,`mul`,`imul` 以及移位指令都会影响 CF（Carry Flag）
	- 两数相加产生进位时 CF 置 1
	- 两数相减产生借位时 CF 置 1
	- 两数相乘乘积超过被乘数宽度时 CF 置 1
	- 移位指令最后移出的那一位保存在 CF 中

如，可以利用带进位加法指令 `adc` 以及逻辑左移指令 `shl` 实现进制转换：

```asm
comment @---------------------------------------------------------
该汇编程序实现 16 位整数转换成 2 进制输出
------------------------------------------------------------------@
data segment
abc dw 00AF0h
data ends

code segment
assume cs:code, ds:data
main:
	mov ax, data
	mov ds, ax
	mov ax, [abc]
	mov cx, 16 ; 16 次循环
again:
	shl ax, 1  ;逻辑左移一位，影响 CF
	mov dl, '0'
	adc dl, 0  ;DL = DL + 0 + CF
	push ax    ;把AX压入栈(保存ax状态，因为下一步ah被修改了)
	mov ah, 2
	int 21h    ;输出DL中的字符
	pop ax     ;恢复AX的值(不想用栈的话直接用bx存储数据即可)
	sub cx, 1
	jnz again  ;若CX不等于0，则跳回 again
	mov ah, 1
	int 21h ; 敲任意键继续
	mov ah, 4Ch
	int 21h    ;return 0
code ends
end main
```


<font style="font-weight: 1000;font-size: 24px" color="red">零标志 ZF</font>

- 算术运算指令、逻辑运算指令、移位运算指令均会影响 ZF（Zero Flag）
	- 当运算结果为 0 时 ZF 置 1
	- 当运算结果非 0 时 ZF 置 0
- **注意：** `jz`(jump if zero) 和 `je`(jump if equal) 跳转依据均为 `ZF==1`，故二者等价；同理，`jnz` 和 `jne` 也等价


<font style="font-weight: 1000;font-size: 24px" color="red">符号标志 SF</font>

- 算术运算指令、逻辑运算指令、移位运算指令均会影响 SF（Sign Flag）
	- 当运算结果为正时 SF 置 0
	- 当运算结果为负时 SF 置 1
- **注意：** 相当于运算结果的最高位；符号标志相关的跳转指令可以用来判定正负
	- `js`(jump if sign) 依据为 `SF==1`
	- `jns`(jump if no sign) 依据为 `SF==0`


<font style="font-weight: 1000;font-size: 24px" color="red">溢出标志 OF</font>

- `add`,`sub`,`mul`,`imul`、移位运算指令均会影响 OF（Overflow Flag）
	- 当两正数相加变负数时 OF 置 1
	- 当两负数相加变正数时 OF 置 1
	- 当两数相乘乘积宽度超过被乘数宽度时 OF 置 1（此时 CF 也置 1）
	- 当仅移动一位，且移位前最高位不等于移位后最高位时 OF 置 1


<font style="font-weight: 1000;font-size: 24px" color="red">奇偶校验位 PF</font>

- 当运算结果低 8 位中二进制 1 的个数为偶数时 PF 置 1；否则置 0


<font style="font-weight: 1000;font-size: 24px" color="red">辅助进位标志 AF</font>

- 若执行加法指令时第 3 位向第 4 位产生进位则 AF 置 1
- 若执行减法指令时第 3 位向第 4 位产生借位则 AF 置 1
- **注意：** AF（Auxiliary Flag） 并没有相关跳转指令，它跟 BCD 码调整指令有关，如 `AAA`,`AAS`,`DAA`,`DAS`

??? example "遇到这种题好好算算二进制减法吧"
	若以下指令序列的操作数均为符号数，请写出每个指令序列执行后标志位CF、ZF、SF、OF的值，并指出CMP指令的前后操作数的大小关系。
	
	(1) mov  al, 99h
	
	 cmp  al, 34h    ; CF=0, ZF=0, SF=0, OF=1, 99h < 34h
	
	(2) mov  ah, 81h
	
	cmp  ah, 0FFh   ; CF=1, ZF=0, SF=1, OF=0, 81h < 0FFh
	
	(3) mov  bx, 1234h
	
	cmp  bx, 8086h  ; CF=1, ZF=0, SF=1, OF=1, 1234h > 8086h
	
	(4) mov  cx, 0FFFFh
	
	cmp  cx, 0FFFEh ; CF=0, ZF=0, SF=0, OF=0, 0FFFFh > 0FFFEh
	
	(5) mov  dx, 3F7Dh
	
	cmp  dx, 1000h  ; CF=0, ZF=0, SF=0, OF=0, 3F7Dh > 1000h

#### 控制标志

<font style="font-weight: 1000;font-size: 24px" color="red">方向标志 DF</font>

- DF（Direction Flag）用来控制字符串操作指令如 `rep`,`movsb` 的运行方向
	- 当 `DF=0` 时，字符串操作指令按正方向运行（先操作低地址再操作高地址）
	- 当 `DF=1` 时，字符串操作指令按反方向运行（先操作高地址再操作低地址）
- **注意：** `cld`(clear direction) 指令设置 `DF=0`；`std`(set direction) 指令设置 `DF=1`
- 通常，若源首地址小于目标首地址，则置 `DF` 为 1；相应的，若源首地址大于目标首地址，则置 `DF` 为 0

例如，我们希望将位于 `1000` 处的字符串 `ABCDE` 复制到地址 `1002` 处，如果我们设置 `DF=0` 的话：

```
Before copy:
	Source		Destination
	 +---+		+---+
 1000|'A'|	1002|	|
	 +---+		+---+
 1001|'B'|	1003|	|
	 +---+		+---+
 1002|'C'|  1004|	|
	 +---+		+---+
 1003|'D'|	1005|	|
	 +---+		+---+
 1004|'E'|	1006|	|
	 +---+		+---+
-------------------------------------
After copy:
	Source		Destination
	 +---+		+---+
 1000|'A'|	1002|'A'|
	 +---+		+---+
 1001|'B'|	1003|'B'|
	 +---+		+---+
 1002|'A'|  1004|'A'|
	 +---+		+---+
 1003|'B'|	1005|'B'|
	 +---+		+---+
 1004|'A'|	1006|'A'|
	 +---+		+---+
-------------------------------------
Well, the right answer is when DF = 1:
	Source		Destination
	 +---+		+---+
 1000|'A'|	1002|'A'|
	 +---+		+---+
 1001|'B'|	1003|'B'|
	 +---+		+---+
 1002|'A'|  1004|'C'|
	 +---+		+---+
 1003|'B'|	1005|'D'|
	 +---+		+---+
 1004|'C'|	1006|'E'|
	 +---+		+---+
```

<font style="font-weight: 1000;font-size: 24px" color="red">中断标志 IF</font>

- IF（Interrupt Flag）用于禁止、允许硬件中断
	- 当 `IF=0` 时，禁止硬件中断
	- 当 `IF=1` 时，允许硬件中断
- **注意：** `cli`(clear interrupt) 指令设置 `IF=0`；`sti`(set interrupt) 指令设置 `IF=1`
- 我们通常使用的 `int 21h` 属于软件中断，是显式(Explicit)的
- 硬件中断是指由硬件某个事件触发，并由CPU自动插入并调用一个隐式(implicit)的 `int n` 指令来调用中断服务子函数

`int n` 函数的指针保存在 `0:n*4` 处，例如 `int 1h` 函数的首地址为 `1234h:5678h` ：

```
0:4 78h
0:5 56h
0:6 34h
0:7 12h
```

我们可以手动将其地址修改，将 `int` 中断的指针指向我们自己定义的函数，实现侵入。

<font style="font-weight: 1000;font-size: 24px" color="red">陷阱标志 TF</font>

- TF（Trap Flag）位于 `FL` 寄存器第八位，用于设置 CPU 的运行模式
	- 当 `TF=1` 时，CPU 进行单步模式
	- 当 `TF=0` 时，CPU 进行常规模式
- **注意：** 当 CPU 进入单步模式后，每执行一条指令后都会跟随执行一条 `int 01h` 中断指令

`TF` 并不能通过单独指令修改，但可以通过如下操作置 1：

```asm
pushf      ; push FL
pop ax     ; AX = FL
or ax, 100h; TF位于FL第八位，通过or指令置一
push ax    ; 
popf       ; pop FL 即 FL=AX,TF=1
```

## 端口

端口地址独立于内存地址，并且不像内存那样既有段地址又有偏移地址，端口地址仅有 16 位偏移地址，范围是 `[0000h,0FFFFh]` 。

读写端口地址的指令是 `in`,`out` ，例如 `in al, 61h` 表示从 61h 号端口读取一个字节的信号并保存到 `AL` 中。

以下程序利用端口实现输出当前时间信息：

```asm
.386
data segment use16
;
current_time db "00:00:00", 0Dh, 0Ah, '$'
data ends

code segment use16
assume cs:code, ds:data
main:
	mov ax, data
	mov ds, ax
	mov al, 4
	out 70h, al  ; 向 70h 端口发送信号 4，表示接下来要读或写CMOS的四号内存单元
	in al, 71h	 ; 从 71h 端口读取CMOS的四号单元之值(BCD码格式的小时值)
				 ; e.g. AL = 19h => 7pm
	call convert ; 调用函数 convert 转换 AL 中的小时值，如把 19h 转换成 AL='1',AH='9'
	mov word ptr current_time[0], ax
	mov al, 2
	out 70h, al  ; 同理，表示接下来要读或写CMOS的二号内存单元
	in al, 71h   ; 读取BCD码格式的分钟值
	call convert
	mov word ptr current_time[3], ax
	mov al, 0
	out 70h, al
	in al, 71h
	call convert
	mov word ptr current_time[6], ax
	mov ah, 9
	mov dx, offset current_time
	int 21h		 ; 输出 ds:dx 指向的字符串
	mov ah, 1
	int 21h      ; 敲任意键继续
	mov ah, 4Ch
	int 21h 	 ; return 0
;---------Convert---------
convert:
	mov ah, al
	and ah, 0Fh
	shr al, 4
	add ah, '0'
	add al, '0'
	ret
;-------End of Convert-----
code ends
end main
```

对于从键盘读取输入：

<1> 高 dos中断调用

```asm
mov ah, 1
int 21h
```

相当于 `al=getchar()` 但不能读取方向键

<2> 中 bios中断调用(basic input/output system)

```asm
mov ah, 0
int 16h
```

AX = 键盘编码，可以读取方向键、功能键、PgUp等键，但不能读取单独的ctrl键

<3> 低 端口操作

```asm
in al, 60h
```

`60h` 是键盘输入的端口，可以读取各个键的编码

## 文本模式的显卡地址映射

```asm
code segment
assume cs:code
main:
	mov ax, 03h    ; 80x25文本模式
	int 10h
	mov ax, 0B800h ; 文本模式下的显存地址,那么最左上角处的字符和颜色分别由 0B800:0000 和 0B800:0001 决定
	mov es, ax     ; 计算机启动时会实现 B800:0000 与跟显卡相应地址的映射
	mov di, 0      ; xor di, di ; xor 的运行速度更快，适合赋初值0
	mov al, 'A'
	mov ah, 71h    ; 前景色为浅灰色，7；后景色为蓝色，1
	mov cx, 2000
zz:
	mov al, 'A'
	mov ah, 71h
again:
	mov es:[di], ax ; 坐标(x,y)对应的显卡偏移地址为 (y*80+x)*2，一个字母显示占据两个字节，第一个字节是显示内容，第二个字节是颜色
	add di, 2       ; 转到下一个偏移地址
	add al, 1
	sub ah, 1       ; 顺手更改颜色
	cmp al, 'Z'+1   ; 重复显示字符 A-Z
	je  zz
	loop again      ; 循环 2000 次
	mov ah, 1
	int 21h         ; 敲任意键继续
	mov ah, 4Ch     ; return al
	int 21h
code ends
end main

; 0 黑    1 蓝    2 绿    3 青    4 红    5 洋红
; 6 棕    7 白    8 灰    9 亮蓝  A 亮绿  B 亮青
; C 亮红  D 紫    E 黄    F 亮白
```

可以得到类似如下图所示的效果：

![[gputextwrite.png]]