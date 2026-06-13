
# Intermediate Code

## Overview

编译器中端是连接“前端”（语义分析）和“后端”（代码生成、优化）的桥梁，核心内容是将抽象语法树 AST 翻译成一个机器无关的中间表示 IR。

**IR** 是一种抽象机器语言：它能表达目标机器的操作，但不依赖过多机器相关细节；同时也独立于源语言的细节。

为什么需要 IR？如果直接从源代码翻译到机器码，那么对于 $N$ 种语言，$M$ 种机器，则需要 $N\times M$ 种组合；而通过 IR，我们只需要编写 $N+M$ 种翻译器：

![[cp_topic7_1.png]]

一个好的 IR 应满足：

- 便于语义分析阶段**生成**
- 便于**翻译**到所有目标机器语言
- 每种构造都有**清晰、简单的含义**，使各种优化变换易于描述和实现

!!! note "AST 和机器语言都可能含有效果复杂的指令（如数组下标、过程调用 vs 向量运算），但二者并不对应。因此 IR 应足够简单，能将 AST 的复杂指令**拆分**成简单成分，再**组合**出真实机器指令（fetch、store、add、move、jump 等）"

常见的编译器中使用的 IR 有：

- **Three-Address Code，TAC**
- Static Single-Assignment，SSA
- Control Flow Graph，CFG
- Abstract Syntax Tree，AST
- **Expression Trees（IR Tree，Tiger 编译器使用）**

## Three-Address Code

**三地址代码**的原则是，每条指令最多使用 3 个地址（操作数），其基本形式为 `x = y op z`，其中 `y`、`z` 也可以是临时操作数（temporary）。

例如，如下 AST 对应的三地址代码为：

![[cp_topic7_9.png]]

![[cp_topic7_2.png]]

针对语言中的不同构造，三地址代码需要变化形式（如一元运算 `t2 = -t1`）。三地址代码**没有统一的标准形式**——原因之一是新语言的特殊特性总需要发明新的指令形式。

!!! example
	![[cp_topic7_3.png]]

!!! quote "TAC 的优点：容易从 AST 直接构造（不需要上下文信息）；并且便于后续优化"

在实现上，整个三地址代码序列用数组或链表存储。由于每条指令最多三个地址，最常见的实现是再添加一个 operation 字段，构成*四元组*(*quadruple*)；地址不足三个的指令将空余字段置空：

```c
t1 = x > 0              =>      (gt, x, 0, t1)
if_false t1 goto L1     =>      (if_f, t1, L1, _)
fact = 1                =>      (asn, 1, fact, _)
label L2                =>      (lab, L2, _, _)
```

!!! success "另一种实现方式为*三元组*，它可以使用 `(n)` 的形式来指代第 n 行代码的结果"
	![[cp_topic7_10.png]]

## Intermediate Representation Tree

Tiger 编译器使用树状结构的 IR Tree，它有两类节点：

- **Expression 节点** `T_exp`：**有返回值**，可能产生副作用
	- `CONST(i)`：整数常量
	- `NAME(n)`：汇编标签（符号地址）
	- `TEMP(t)`：临时寄存器（类似真实机器中的寄存器）
	- `BINOP(op, e1, e2)`：二元运算（`PLUS MINUS MUL DIV`、位运算 `AND OR XOR`、移位 `LSHIFT RSHIFT ARSHIFT`）
	- `MEM(e)`：从地址 e 开始的 wordSize 字节内存。作为 MOVE 左孩子时是写（store），其余位置是读（fetch）
	- `CALL(f, args)`：函数调用（参数从左到右求值）
	- `ESEQ(s, e)`：先执行语句 s（产生副作用），再求值 e 作为结果
- **Statement 节点** `T_stm`：**无返回值**，执行副作用和控制流
	- `MOVE(TEMP t, e)`：求值 e 并存入临时变量 t
	- `MOVE(MEM(e1), e2)`：先求值 e1 得地址 a，再求值 e2 存入以 a 开始的内存
	- `EXP(e)`：求值 e 但丢弃结果（只要副作用）
	- `JUMP(e, labs)`：无条件跳转到地址 e；labs 列出 e 所有可能的目标（用于数据流分析）
	- `CJUMP(op, e1, e2, t, f)`：按序求值 e1、e2 并用关系运算符 op 比较，真跳 t，假跳 f
	- `SEQ(s1, s2)`：顺序执行
	- `LABEL(n)`：定义标签 n 为当前地址

!!! info "IR Tree 的定义独立于 Tiger 或任何其他源语言"

## Translation into IR Trees

### Kinds of Expressions

根据前端传递下来的 AST Expression 的特性，翻译时内部将其分为三类：

- *Expression with return values*
	- Ex，用 `T_exp` 表示
- *Expression with no return value*
	- Nx，用 `T_stm` 表示
	- 例如过程调用、while expression 等
- *Expression with boolean values*
	- Cx，Conditional Jump，用 `T_stm` 的组合 + 一对目标 label 表示
	- 例如 `a > b`
	- `Tr_Cx(patchList trues, patchList falses, T_stm stm)`

![[cp_topic7_4.png]]

!!! note "翻译时还不知道 true/false 的跳转目标，上述 `NULL` 部分留空，并用两个 **Patch Lists** 分别记录所有“待填 t”与“待填 f”的位置，等目标已知后再回填"

有时需要在三类表示之间转换（`toEx`、`toNx`、`toCx`）。例如 `flag := (a>b|c<d)`，赋值语句右侧应为 Ex，我们使用转换函数 `toEx(...)` 来将条件 Cx 转换为 0/1：

![[cp_topic7_5.png]]

!!! success "`toEx(Cx)` 的具体实现：借助一个临时寄存器 r，先 `MOVE(TEMP r, 1)`，若条件为假则改写为 0，最后返回 `TEMP(r)`"
	![[cp_topic7_6.png]]

### Simple Variables

声明在当前过程栈帧中的简单变量 `v`，翻译为它的 Memory Location：

![[cp_topic7_7.png]]

其中 k 是 v 在帧内的偏移，`TEMP fp` 是 Frame Pointer 寄存器。这些是机器相关定义，因此放在 Frame 模块中：

```c
/* frame.h */
Temp_temp F_FP(void);               // 帧指针寄存器
extern const int F_wordSize;        // 机器字长
T_exp F_Exp(F_access acc, T_exp framePtr);
// access 为 InFrame(k)  => MEM(BINOP(PLUS, TEMP FP, CONST(k)))
// access 为 InReg(t832) => TEMP t832
```

!!! info "Tiger Compiler 中，所有变量的大小都是 1 word（整数/指针）"

我们将 `BINOP(PLUS, e1, e2)` 简写为 `+(e1, e2)`，于是上述 IR Tree 也可以等价表示为：

![[cp_topic7_8.png]]

### Array Variables

不同语言对数组变量的处理不同：

- **Pascal**：数组变量代表数组的**内容**，`b := a` 会复制全部元素
- **C**：数组像“指针常量”，`int a[12], b[12]; b=a;` 非法；但 `int a[12], *b; b=a;` 合法（b 指向 a 的开头）
- **Tiger**（同 Java、ML）：数组变量是**指针**。没有 C 那样的数组常量，新数组由 `ta[n] of i` 创建（类型 ta，n 个元素，初值 i），赋值 `b := a` 只是指针赋值

!!! warning "Tiger 中的 Record 同样是指针，record 赋值也是指针赋值，不会复制字段（与 C 的 struct 赋值复制所有字段相反）"

### Structured L-Values

- **L-value**：可出现在赋值号**左侧**的表达式结果，如 `x`、`p.y`、`a[i+2]`，表示一个可被赋值的位置（也可出现在右侧）
- **R-value**：只能出现在赋值号**右侧**的结果，如 `a+3`、`f(x)`，不表示可赋值位置

整数或指针是“标量”（scalar），只占 1 个字。Tiger 中所有变量和 L-value 都是标量（数组/record 实为指针）。而 C 的 `struct`、Pascal 的数组和 record 是**结构化 L-value**，非标量；要支持它们需给 MEM 增加 size 参数：

```c
T_exp T_Mem(T_exp, int size);   // MEM(+(TEMP fp, CONST kn), S)
```

### Subscripting and Field Selection

`a[i]` 的地址为：

$$(i - l) \times s + a$$

其中 l 是下标下界，s 是每个元素的大小（字节），a 是数组元素的基址。若 a 是全局变量且地址编译期已知，则 $a - l\times s$ 可在编译期算出。类似地，record 字段 f 的地址为 $\text{offset}(f) + a$。

数组变量 a 是 L-value，下标表达式 `a[i]` 也是。为了能对 a 的地址做算术运算，**L-value 应翻译为表示其地址的 Tree 表达式**（不带顶层 MEM）：

- 下标/取字段：在地址上做 `+` 运算，得到更小的 L-value
- 用作 R-value（如 `b := a`）：对地址套上 `MEM` 强制转换为 R-value（fetch）
- 被赋值：向该地址 store

在 Tiger 中，数组值本身就是指针，“基址”是指针变量的内容，所以要先取一次 MEM。设 l = 0、s 为字长，`a[i]` 的 IR Tree 为：

```
MEM(+(MEM(e), BINOP(MUL, i, CONST s)))
```

!!! note "MEM 作为 MOVE 左子树表示 store，其余位置表示 fetch"

### Arithmetic

每个整数算术运算符都对应一个 Tree 运算符，但 Tree 语言**没有一元运算符**：

- **取负**：用 0 做减法实现，`-n => 0 - n`
- **按位取反**：与全 1 做 XOR

!!! warning "浮点取负**不能**用 0 减实现：很多浮点表示有负零（−0 的取负是 +0，反之亦然），Tree 语言对一元取负支持不佳"

### Conditionals

比较运算的结果是 Cx 表达式：一个会跳向 true/false 目标的 `T_stm`。Cx 表示的意义就在于条件可以方便地用 `&`、`|` 组合。例如 `x < 5` 翻译为：

```c
stm    = CJUMP(LT, x, CONST(5), NULLt, NULLf)
trues  = {t}
falses = {f}
```

对于 **if-expression** `if e1 then e2 else e3`，最直接的翻译：

1. 对 e1 应用 `toCx`，对 e2、e3 应用 `toEx`
2. 创建标签 t、f，分配临时变量 r
3. label t 之后将 e2 存入 r；label f 之后将 e3 存入 r
4. 两个分支最后都跳到新建的 join 标签，整体结果为 `TEMP r`

```
toCx(e1)
LABEL t
r = toEx(e2)
JUMP join
LABEL f
r = toEx(e3)
JUMP join
LABEL join
TEMP r
```

这种做法正确但不高效，某些情况应特殊处理：

- e2、e3 都是 Nx（无返回值）：`toEx` 也能工作，但最好特殊识别
- e2 或 e3 是 **Cx**：直接 `toEx` 会产生一堆混乱的跳转和标签。例如 `if x < 5 then a > b else 0`，应识别为类似 `if x < 5 & a > b then 1 else 0` 的形式，将 `x<5` 的 Cx（$s_1$）与 `a>b` 的 Cx（$s_2$）直接拼接：`SEQ(s1(z,f), SEQ(LABEL z, s2(t,f)))`

![[cp_topic7_11.png]]

### While Loops

While 循环的一般布局：

```c
test:
    if not(condition) goto done
    body
    goto test
done:
```

循环体内的 `break`（未嵌套在更内层循环中）翻译为 `JUMP done`。为了在嵌套循环中找到正确的 done 标签，翻译函数需要增加一个形参 `break`，其值为**最近一层外围循环**的 done 标签。

!!! warning "这是为了防止循环嵌套时，内层的 `break` 直接跳转到外层的 `done`"

### For Loops

直接的做法是把 for 重写为 let/while 的抽象语法：

```c
for i := lo to hi do body
// 👇👇👇
let var i := lo
    var limit := hi
in  while i <= limit
    do (body; i := i+1)
end
```

**【Question】** 如果 `limit = maxint`，`i+1` 会溢出，从而导致死循环。

解决方法是把判断放到循环体**之后**，先比较再自增：

```c
    if lo > hi goto done
    i := lo
    limit := hi
test:
    body
    if i >= limit goto done
    i := i+1
    goto test
done:
```

### Function Call

函数调用 `f(a1, ..., an)` 的翻译很简单，只需把 *Static Link* 作为隐式的额外参数加入：

```
CALL(NAME lf, [sl, e1, e2, ..., en])
```

其中 `lf` 是 f 的标签，`sl` 是静态链。

!!! note "此处的“隐式”指不被写入 IR 指令中"

## Translation of Declarations

- 对于函数体内的每个**变量声明**，都在栈帧中保留额外空间
- 对于每个**函数声明**，为函数体保留一个新的 Tree 代码 “fragment”

### Variable Declarations

`transDec` 为 let 表达式体更新值环境和类型环境，并把变量的**初始化**翻译为赋值表达式，放在 let body 之前执行。若 `transDec` 作用于函数和类型声明，结果是一个 no-op 表达式，如 `Ex(CONST(0))`。

### Function Declarations

一个函数翻译为 **prologue（入口处理代码）+ body + epilogue（出口处理代码）**。

Prologue 包含：

1. 标记函数开始的伪指令（特定汇编语言需要）
2. 函数名的标签定义
3. 调整栈指针的指令（分配新栈帧）
4. 将逃逸参数（含静态链）存入帧、将非逃逸参数移入新临时寄存器的指令
5. 保存函数内用到的 callee-save 寄存器（含返回地址寄存器）的 store 指令

Body 即翻译后的函数体表达式（Tiger 的函数体是一个表达式）。

Epilogue 包含：

1. 将返回值移入返回值寄存器的指令
2. 恢复 callee-save 寄存器的 load 指令
3. 重置栈指针的指令（释放栈帧）
4. return 指令（跳转到返回地址）
5. 标记函数结束的伪指令

### Fragment

Translate 阶段为每个函数生成一个 fragment，包含：

- **frame**：帧描述符，含局部变量和参数的机器相关信息
- **body**：`procEntryExit1` 返回的结果

```c
/* frame.h */
typedef struct F_frag_ *F_frag;
struct F_frag_ {
  enum {F_stringFrag, F_procFrag} kind;
  union {
    struct {Temp_label label; string str;} stringg;
    struct {T_stm body; F_frame frame;} proc;
  } u;
};
F_frag F_StringFrag(Temp_label label, string str);
F_frag F_ProcFrag(T_stm body, F_frame frame);

typedef struct F_fragList_ *F_fragList;
struct F_fragList_ {F_frag head; F_fragList tail;};
F_fragList F_FragList(F_frag head, F_fragList tail);

/* translate.h */
void Tr_procEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals);
F_fragList Tr_getResult(void);
```
