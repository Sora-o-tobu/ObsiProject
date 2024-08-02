# Chapter 3. Machine-level Representation of Programs
## Part 1. 程序的机器级表示
### Historic Perspective
!!! quote "Intel处理器的发展历史"
	![[Intel处理器的发展历史.png]]

!!! danger "`mov`的方向问题"
	汇编有`intel`,`att`两种格式，本篇相关的汇编代码均采用`intel`格式，即`mov a,b`等效于`b=a`

```shell
❯ nano mstore.c
 1 long mult2(long,long);
 2
 3 void mulstore(long x,long y,long *dest) {
 4         long t = mult2(x,y);
 5         *dest = t;
 6 }

❯ gcc -O2 -S mstore.c
# 使用 O2 选项优化，减少生成的汇编代码的复杂度
```


仅截取部分汇编代码作为示例，其中以 `.` 开头的行都是指导汇编器和链接器工作的伪指令，分析汇编代码的时候可以完全忽略它们，下面的示例中将其全部删去：

```shell
mulstore:
        pushq   %rbx 
        # 将寄存器%rbx的值压入程序栈保存，在该函数返回前pop恢复其值
        movq    %rdx, %rbx
        # 将%rdx内容复制到%rbx
        call    mult2@PLT
        movq    %rax, (%rbx)
        popq    %rbx
        ret
```

!!! note "寄存器的分类"
	寄存器分为调用者保存寄存器和被调用者保存寄存器，其区别在于恢复某一在调用(`call`)函数过程中被修改值的寄存器时，其恢复方式是在 调用者(Caller) 中入栈出栈保存值还是在 被调用者(Callee) 中入栈出栈恢复。如，上面的`%rbx`就是一个 ==callee-saved register==
	![[寄存器分类.png]]

??? info "C语言数据类型对应的汇编后缀"
	![[C语言数据类型对应的汇编后缀.png]]

```shell
❯ gcc -O2 -c mstore.c
# 生成对应的机器代码文件 mstore.o 
❯ ls
mstore.c  mstore.o  mstore.s
❯ objdump -d mstore.o
# 反汇编器将机器代码翻译成汇编代码

mstore.o：     文件格式 elf64-x86-64

Disassembly of section .text:

0000000000000000 <mulstore>:
   0:   f3 0f 1e fa             endbr64
   4:   53                      push   %rbx
   5:   48 89 d3                mov    %rdx,%rbx
   8:   e8 00 00 00 00          call   d <mulstore+0xd>
   d:   48 89 03                mov    %rax,(%rbx)
  10:   5b                      pop    %rbx
  11:   c3                      ret
```

### 机器指令

![[寄存器的发展.png]]

从16位寄存器到32位寄存器到64位寄存器

大多数指令由操作码(Operation code,如`movq`,`xorq`等)和操作数(Operands,如`%rbx`,`$8`等)组成。

- 操作码
	- Operation code
	- 如`movq`,`xorq`等
- 操作数
	- Operands
	- 分为立即数、寄存器、内存引用三类
		- 立即数以`$`开头，后面跟一个整数，如`$8`
		- 寄存器外接一个小括号表示的是内存引用

#### 数据传送指令

- `mov a, b`
- `push a`
- `pop a`
- `xchg op1, op2`
	- 交换`op1`和`op2`

```shell
movabsq $0x0011223344556677 %rax
# %rax: 00 11 22 33 44 55 66 77
movb $-1 %al
# %rax: 00 11 22 33 44 55 66 FF
movw $-1 %ax
# %rax: 00 11 22 33 44 55 FF FF
movl $-1 %eax
# %rax: 00 00 00 00 FF FF FF FF
# 任何位寄存器生成32位值的指令都会把该寄存器的高位置0
# 即，当movl的目的操作数为寄存器是，高四字节置0
```

!!! warning "当源操作数数位小于目的操作数时，需要对其进行零拓展或符号位拓展"

![[程序栈.png]]

对于程序栈，栈底对应地址高位，栈顶对应地址低位，一般以栈顶在下作为程序栈的表示方法。

指针`%rsp`保存栈顶的地址，因此，对于入栈指令 `pushq %rax`，其等效于：

1. `subq $8, %rsp`
2. `movq %rax, (%rsp)`

#### 算术与逻辑运算指令

- `leaq src, dest`
	- Load Effective Address
	- 将src的有效地址写入dest

??? note "利用`lea`实现算数运算"
	![[利用lea实现算数运算.png]]

**一些一元操作：**

| Instruction | Effect               | Description   |
| ----------- | -------------------- | ------------- |
| `INC D`     | $D\leftarrow D+1$    | Increment 自增  |
| `DEC D`     | $D\leftarrow D-1$    | Decrement 自减  |
| `NEG D`     | $D\leftarrow -D$     | Negate 取负     |
| `NOT D`     | $D\leftarrow \sim D$ | Complement 取补 |

**一些二元操作：**


| Instruction | Effect                 | Description                 |
| ----------- | ---------------------- | --------------------------- |
| `ADD S,D`   | $D\leftarrow D+S$      | ADD 加                       |
| `SUB S,D`   | $D\leftarrow D-S$      | Substract 减                 |
| `IMUL S,D`  | $D\leftarrow D*S$      | Multiply 乘                  |
| `XOR S,D`   | $D\leftarrow D \^ S$   | Exclusive-or 异或             |
| `OR S,D`    | $D\leftarrow D \mid S$ | Or 或                        |
| `AND S,D`   | $D\leftarrow D \& S$   | And 与                       |
| `SAL k,D`   | $D\leftarrow D<<k$     | Left shift 左移               |
| `SHL k,D`   | $D\leftarrow D<<k$     | Left shift 等同于`SAL`         |
| `SAR k,D`   | $D\leftarrow D>>_Ak$   | Arithmetic right shift 算数右移 |
| `SHR k,D`   | $D\leftarrow D>>_L k$  | Logical right shift 逻辑右移    |
!!! note "对于移位指令，只允许以特定寄存器`cl`作为操作数"

#### 指令与条件码

![[条件码寄存器.png]]

ALU除了执行算数和逻辑运算之外，还会根据该运算的结果去设置 ==条件码寄存器==

条件码寄存器由CPU来维护，长度是单个比特位，它描述了最近执行操作的属性。假设ALU执行两个连续的算数指令：

```shell
t1: addq %rax, %rbx
t2: subq %rcx, %rbx
```

t1时刻，条件码寄存器保存的是指令1的执行结果的属性；t2时刻，条件码寄存器中的内容将被下一条指令结果属性覆盖。

常用条件码：

- `CF` 进位标志 ==(Carry FLag)==
	- 当CPU最近执行的一条指令最高位产生进位的时候，`CF` 置1，用来检查无符号数操作的溢出
- `CF` 零标志 ==(Zero Flag)==
	- 当最近操作的结果等于零时，`ZF` 置1
- `SF` 符号标志 ==(Sign Flag)==
	- 当最近操作的结果小于零时，`SF` 置1
- `OF` 溢出标志 ==(Overflow Flag)==
	- 针对有符号数，最近的操作导致正溢出或者负溢出时 `OF` 置1

!!! tip "`cmp`和`test`两个指令用以设置条件寄存器的值(不改变其它寄存器)"

##### 条件码的应用

- 应用一：利用符号标志判断 `a==b`
![[条件码应用1.png]]

- 应用二：判断 `a<b` ，与上述类似，但需要用 `SF^OF` 判断结果
	- 设 `t=a-b`
	- 当没有发生溢出，即 `OF=0`
		- Case 1：`a<b,t<0,SF=1,SF^OF=1`
		- Case 2：`a>b,t>0,SF=0,SF^OF=0`
	- 当有发生溢出，即 `OF=1`
		- Case 3：例如`a=-2,b=127`时，`a<b,t=127>0,SF=0,SF^OF=1`
		- Case 4：例如`a=1,b=-128`时，`a>b,t=-127<0,SF=1,SF^OF=0`

???+ note "其它判断情况"
	=== "有符号数"
		![[条件码其它判断情况.png]]
	=== "无符号数"
		![[条件码其它判断情况无符号数.png]]



