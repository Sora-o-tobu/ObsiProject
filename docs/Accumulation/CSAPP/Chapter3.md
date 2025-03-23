# Chapter 3. Machine-level Representation of Programs

## Historic Perspective
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

```c
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

## 机器指令

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

### 数据传送指令

- `mov a, b`
- `push a`
- `pop a`
- `xchg op1, op2`
	- 交换`op1`和`op2`

```c
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

### 算术与逻辑运算指令

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

### 指令与条件码

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

#### 条件码的应用

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

### 跳转指令和循环指令

跳转指令`jl .L4`功能为，当`x>y`时，顺序执行；当`x<y`时，跳转至`.L4`处继续执行

![[跳转指令.png]]

!!! info "与`set`指令类似，其它各个跳转指令功能如下"
	![[各个跳转指令功能.png]]

现代处理器会提前预测分支的结果，但是如果预测错误会清空流水线大大降低效率


!!! quote "循环语句是通过条件测试和跳转的结合来实现的"


通过 `do-while` 循环求阶乘：
![[do-while阶乘.png]]

除了 `do-while` 外，采用 `while` 和 `for` 生成的汇编代码也基本类似

C语言还提供了 `switch` 语句，根据索引值进行多重分支。例如，一个 `switch(n)` 语句对`case 0;`,`case 1`,...,一直到`case 6`的情况做了分支，那么其汇编代码关键语句有：

```c
switch_eg:
	cmpq $6, %rsi # compare n:6
	ja .L8
	leaq .L4(%rip), %rcx
.L4:
# 对于Case 0-6 的情况，通过跳转表来访问不同分支
	.long: .L3-.L4 # case 0
	.long: .L8-.L4 # case 1
	.long: .L5-.L4 # case 2
	.long: .L6-.L4 # case 3
	.long: .L7-.L4 # case 4
	.long: .L8-.L4 # case 5
	.long: .L7-.L4 # case 6
# 重复情况，如case 4和6，跳转至统一位置
```

!!! quote "为什么`switch`比`if-else`快？"
	程序使用跳转表来处理多重分支。就算`swtich`情况很多，程序也只需要一次跳转就能处理复杂的分支情况(时间复杂度为`O(1)`)

## 过程 (函数调用)

过程提供了一种封装代码的方式，它可以隐藏某个行为的具体实现，同时提供清晰的接口定义。在不同的编程语言中，过程的具体实现可以是不一样的，如C语言的函数、Java的方法等

当函数执行所需要的存储空间超出寄存器能够存放的大小时，就会借助栈上的存储空间，我们把这部分存储空间称为函数的栈帧。

### 返回地址

当一个函数P调用函数Q时，会将一个返回地址压入栈中，该地址指明了函数Q执行结束返回时要从函数P的哪个位置继续执行，这个返回地址的压栈操作并不是由指令`push`来执行，而是由函数调用指令`call`来实现的。

以下例函数调用为例，在`main`函数中调用`multstore`函数，使用`gcc`生成机器码并用`objdump`反汇编查看：

![[函数call返回地址.png]]

观察相关汇编部分可知，`call`指令调用`multstore`时，会向程序栈中压入地址`0x700`，即`main`函数中调用函数完毕后的下一条命令的地址。

当函数`multstore`执行完毕，指令`ret`从栈中弹出返回地址，写入到程序指令寄存器`%rip`中

![[函数对应汇编代码.png]]

### 参数传递

!!! quote "如果一个函数的参数大于6，超出的部分就要通过栈来传递"

假设函数P有n个整型参数，参数1到参数6的传递可以使用对应的寄存器，当n的值大于6时，参数7及其之后的参数需要用到栈来传递。

![[参数传递.png]]

!!! note ""
	- 通过栈来传递参数的时候，所有的数据大小都是向8的倍数看齐
	- 寄存器的使用取决于传递参数的大小
	- `%rsp`通常指向栈顶

**More Example:**

```c
long caller()
{
	long arg1 = 534;
	long arg2 = 1057;
	long sum = swap(&arg1,&arg2);
	//交换两个指，并返回它们的和
	long diff = arg1 - arg2;
	return sum * diff;
}

long swap(long *xp, long *yp)
{
	long x = *xp;
	long y = *yp;
	*xp = y;
	*yp = x;
	return x+y;
}
```

对应生成汇编代码如下：

![[callerswapexample.png]]

- `subq $16, %rsp` 将栈顶指针`%rsp`向下移动16字节，表示在栈上分配16个字节的空间
- 接下来两条`movq`指令将`arg1`和`arg2`存储在函数`caller`的栈帧上
- 参数准备完毕后，执行`call`指令调用`swap`函数
- 最后函数`caller`返回之前，通过栈顶指针加上16的操作来释放栈帧

## 数组的分配与访问

对于不同的指针，`char *p`以及`int *q`，若其对应的地址都为`0x100`

- `p=p+1` → `0x101`
- `q=q+1` → `0x104`

!!! tip "对于数组`int E[6]`，其第二个元素为`E[1]`或`*(E+1)`"

在`ISO C99`标准中，引入了变长数组的概念:

```c
long var_ele(long n, int A[n][n], long i, long k){
	return A[i][j];
}
```

当变长数组作为函数参数时，参数n必须在数组A之前。变长数组元素的地址计算与定长数组类似，不同点在于新增了参数`n`，需要使用乘法指令`imulq`来计算`n*i`

## 结构体与联合体

首先我们来看一下一个结构体的声明：

```c
struct rec {
	int i;
	int j;
	int a[2];
	int *p;
};

//其各个字段相对于结构体起始地址处的字节偏移如下
0    4    8    12   16
+----+----+----+----+----+
| i  | j  |a[0]|a[1]| p  |
+----+----+----+----+----+
```

无论是单个变量还是数组元素，都是通过起始地址加偏移量的访问方式来访问

![[结构体数据对齐.png]]

- 对于图中的结构体，它包含两个`int`类型的变量以及一个`char`类型的变量，我们有可能下意识移位它占九个字节的存储空间
- 实际上，使用`sizeof()`函数对其大小求值，可以得到它的实际存储空间为12字节
- 原因是为了提高内存系统的性能，系统对于数据存储的合法地址做出了一些限制
	- 对于不同的数据类型，地址对齐的原则是任何K字节的基本对象的地址必须是K的倍数
	- 例如变量`j`是`int`类型，占四个字节，那么它的起始地址必须是四的倍数，因此编译器会在变量`c`和`j`之间插一个三字节的间隙
	- 除此之外，结构体的末尾还可能需要填充间隙 ==(对于结构体数组而言)==

!!! example "More Example!"
	![[结构体更多例子.png]]

---

C语言中还有一种数据类型—— **联合体**

![[联合体定义.png]]

与结构体不同，联合体所有变量共用同一个内存区域，因此联合体的大小取决于它最大字段的大小

如上图联合体，变量`v`和数组`i[2]`的大小都是八个字节，因此，这个联合体占用8个字节的存储空间

联合体的一种应用情况是，我们事先知道两个不同字段的使用是互斥的，那么我们可以将这两个字段声明为一个联合体

例如，我们需要一个二叉树，其中只有叶节点含有两个`double`类型的数据，那么使用`struct`定义其节点需要32个字节(上)

![[联合体二叉树.png]]

由于其特殊性，我们可以使用联合体`union`来定义其节点(下)，是的每个节点只需要16个字节的存储空间，相对于结构体的定义方式，可以节省一半空间

!!! warning "这种编码方式，没有办法确定一个节点到底是叶节点还是内部节点"
	通常的解决方法是引入一个枚举类型，然后创建一个结构体，它包含一个标签和一个联合体
	```c
	typedef enum {N_LEAF,N_INTERNAL} nodetype_t;
	
	struct node_t{
		nodetype_t type;
		union {
			struct {
				struct node_t *left;
				struct node_t *right;
			} internal;
			double data[2];
		} info;
	};
	```



除此之外，联合体还可以用来访问不同数据类型的位模式，当我们使用简单的强制类型转换，将`double`类型转换成`unsigned long`类型的时候 (`unsigned long u = (unsigned long) d`)，由于二者的二进制位差别很大，这时我们可以将这两者类型的变量声明为一个联合体，这样就可以以一种类型来存储，以另外一种类型来访问，变量`u`和`d`就具有相同的位表示：

```c
unsigned long double2bits (double d) {
	union {
		double d;
		unsigned long u;
	} temp;
	temp.d = d;
	return temp.u;
};
```

## 缓冲区溢出

栈帧中保存着程序执行所需要的重要信息，如返回地址、寄存器的值等。但是在C语言中，对数组的引用不会进行任何的边界检查，那么对越界的数组进行写操作，就会破坏存储在栈中的状态信息

!!! failure "大名鼎鼎的 Stack Overflow"

栈溢出指的是程序向栈中某个变量中写入的字节数超过了这个变量本身所申请的字节数，因而导致与其相邻的栈中的变量的值被改变。这种问题是一种特定的缓冲区溢出漏洞，类似的还有堆溢出，bss 段溢出等溢出方式。栈溢出漏洞轻则可以使程序崩溃，重则可以使攻击者控制程序执行流程。此外，我们也不难发现，发生栈溢出的基本前提是：

- 程序必须向栈上写入数据。
- 写入的数据大小没有被良好地控制。

==详情可自行了解 CTF 比赛中的 **pwn** 题目==

---

编译器和操作系统为了对抗缓冲区溢出攻击，使用了：

- **Stack Randomization** (栈随机化)
	- 栈的位置在每次运行时都有变化
	- 在 Linux 系统中，栈随机化已经成为了标准行为，它属于地址空间布局随机化的一种，简称 **ASLR**
- **Stack Corruption Detection** (栈破坏检测)
	- 编译器在产生的汇编代码中加入一种栈保护机制来检测缓冲区越界，即在缓冲区与栈保存的状态值中存储一个特殊值，金丝雀值 (canary) 
	- canary 是每次程序运行时随机产生的，在函数返回之前，会检测金丝雀值是否被修改来判断是否遭受攻击
- **Limiting Executable Code Regions** (限制可执行代码区域)
	- 消除攻击者向系统中插入可执行代码的能力
	- 其中一种方法是限制哪些内存区域能够存放可执行代码 (CTF题目一般会给出可读可写可执行地址)

