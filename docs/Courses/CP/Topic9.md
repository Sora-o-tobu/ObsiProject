
# Instruction Selection

## Overview

我们的最终目标是：源语言程序 → 目标语言程序。前面的章节已经完成了：源语言程序 → 一组规范树（canonical trees）。本章解决**指令选择（Instruction Selection）**：规范树 → 汇编。

为什么需要这一步？IR Tree 语言中**每个树节点只表达一种操作**（fetch、store、加减、条件跳转……），而**一条真实的机器指令往往能同时完成多个基本操作**。例如：

```c
LOAD r1 ← M[r2 + c]    // 同时完成了 加法 + 取内存
```

指令选择阶段的任务就是：**为给定的 IR Tree 找到合适的机器指令组合来实现它**。

## Tree Patterns and Tiling

每条机器指令都可以表示为 IR 树的一个片段，称为**树模式（Tree Pattern）**。例如 `LOAD ri ← M[rj + c]` 对应的树模式：

![[cp_topic9_1.png]]

于是指令选择问题被形式化为**铺瓦/平铺（Tiling）**：

- **Tile**：与合法机器指令对应的树模式集合
- **Tiling**：用**互不重叠**的树模式**完全覆盖** IR 树
- **指令选择**：用**尽可能少**的树模式铺满整棵树

为了演示，教材发明了一个指令集——**Jouette 体系结构**（Jouette 在法语中意为"玩具"）：

![[cp_topic9_2.png]]

关于 Jouette 的几点说明：

- `BINOP(PLUS, x, y)` 简写为 `+(x, y)`
- 寄存器 `r0` 恒为 0
- ADDI/LOAD/STORE 中的常数 c 可以为 0
- **一条指令可以对应多个树模式**（例如 ADDI 既匹配 `+(e, CONST)` 也匹配 `+(CONST, e)`，LOAD 匹配 `MEM(+(e, CONST))`、`MEM(+(CONST, e))`、`MEM(CONST)`、`MEM(e)` 四种）

??? example "`a[i] := x` 的 IR 树（a、x 都是栈帧内偏移为常量的变量，元素大小为 4）"
	![[cp_topic9_3.png]]
	
	该树有多种铺法，下图给出两种：
	
	![[cp_topic9_4.png]]
	
	注意图 (a) 中编号 1、3、7 的"瓦片"只是寄存器（TEMP），不对应任何机器指令。

**总是可以**用"微型瓦片"（每片只盖一个节点）铺满整棵树（除了 MOVE 必须与 MEM 一起），例如 `a[i] := x` 的微型铺法需要 10 条指令——可行，但显然不优。

## Optimal vs Optimum Tilings

**Best tiling**（最佳铺法）指代价最小的指令序列：指令条数最少；或当各指令执行时间不同时，总执行时间最短。严格区分两个概念：

- **Optimum Tiling（最优铺法）**：所有瓦片的代价之和达到**全局最小**（global）
- **Optimal Tiling（局部最优铺法）**：任何相邻的两块瓦片都不能合并成一块代价更低的瓦片（local）

!!! info "每个 optimum tiling 一定是 optimal 的，反之不成立"

!!! note "如果某个树模式可以被拆成若干总代价更低的小瓦片，那么这个模式根本就不该留在瓦片目录里——所以 optimal 的定义只需考虑“相邻合并”"

!!! example "设每条指令代价为 1，MOVEM（内存到内存搬运）代价为 m"
	![[cp_topic9_5.png]]

	左边铺法 6 units，右边（用 MOVEM）5+m units。**两种都是 optimal**（无法局部合并改进），但哪个是 optimum 取决于 m：m < 1 时右边更优，m > 1 时左边更优，m = 1 时一样。

## Algorithms for Instruction Selection

求 optimal 的算法比求 optimum 的简单：

- **CISC**（Complex Instruction Set Computer）：单条指令能完成多个操作，瓦片大小、代价差异大，optimum 与 optimal 的差距明显
- **RISC**（Reduced Instruction Set Computer）：瓦片小且代价均匀，optimum 和 optimal 通常**没有区别**，简单的算法（Maximal Munch）就够用

### Maximal Munch

**Maximal Munch** 是求 **optimal** tiling 的**贪心**算法，**自顶向下**工作：

1. 从树根开始，找出能匹配（fit）根节点的**最大瓦片**（覆盖根节点及附近的若干节点），盖上去
2. 盖上后剩下若干子树，对每棵子树**递归**执行同样的算法

其中"最大瓦片"= **节点数最多**的瓦片；若两个相同大小的瓦片都能在根上匹配，**任选其一**即可。

!!! warning "Maximal Munch 按自顶向下的顺序选瓦片，但指令是在递归**返回**时生成的，因此生成顺序是**逆序**的（先生成子树的指令）"

实现：两个递归函数，`munchStm` 处理语句、`munchExp` 处理表达式。匹配时**按瓦片大小从大到小**排列 case（biggest tiles first）：

![[cp_topic9_6.png]]

Program 9.4 给出了 `munchExp` 各模式的匹配优先级（指令的操作数寄存器留待之后确定）：

![[cp_topic9_7.png]]

### Dynamic Programming

Maximal Munch 总能找到 optimal，但不一定 optimum。**动态规划**算法能基于子问题的最优解求出 **optimum**，**自底向上**工作。

为树中**每个节点**分配一个代价：该节点的代价 = 能铺满**以该节点为根的子树**的最佳指令序列的总代价。

具体过程——对根节点为 n 的 IR 树：

1. **递归**求出 n 的所有孩子（孙子……）节点的代价
2. 把每种树模式（瓦片）与节点 n 匹配。每个瓦片有零或多个**叶位**（leaves，可挂接子树的位置）。对代价为 $c_t$ 的瓦片 t，其在 n 处的匹配代价为：

$$c_t + \sum_{\text{all leaves } i \text{ of } t} c_i$$

3. 选择总代价**最小**的树模式，记录 `(最小代价, 模式编号)`

!!! example "对 `MEM(+(CONST 1, CONST 2))` 自底向上计算（记号 `(a, b)`：a 为最小代价，b 为对应模式号）"
	- 两个 **CONST** 节点：只有模式 (8) ADDI `CONST` 匹配，代价 1 + 0 = 1 → `(1, 8)`
	- **+** 节点：模式 (2) ADD `+(e1,e2)` 代价 1+2=3；模式 (6) ADDI `+(CONST,e1)` 代价 1+1=2；模式 (7) ADDI `+(e1,CONST)` 代价 1+1=2 → `(2, 6/7)`
	- **MEM** 节点：模式 (13) LOAD `MEM(e1)` 代价 1+2=3；模式 (10) `MEM(+(e1,CONST))` 代价 1+1=2；模式 (11) `MEM(+(CONST,e1))` 代价 1+1=2 → `(2, 10/11)`

![[cp_topic9_8.png]]

求出根节点（即整棵树）的代价后，进入**指令发射（Instruction Emission）**阶段：

```c
Emission(node n):
    for n 处所选瓦片的每个叶位 li:
        Emission(li)        // 先发射叶子子树的指令
    发射 n 处匹配的指令
```

![[cp_topic9_9.png]]

上例最终发射：`ADDI r1 ← r0 + 1`，然后 `LOAD r1 ← M[r1 + 2]`，共 2 条指令。

### Tree Grammars

对于指令集复杂、有**多类寄存器和寻址模式**的机器，DP 算法有一个有用的推广。考虑"脑残版"Jouette：**a 寄存器**用于寻址，**d 寄存器**用于数据，每块瓦片的根和叶都必须标上 a 或 d：

![[cp_topic9_10.png]]

此时可以用一个**上下文无关文法**来描述瓦片，非终结符为：

- `s`：语句
- `a`：计算到 a 寄存器中的表达式
- `d`：计算到 d 寄存器中的表达式

例如 LOAD、MOVEA、MOVED 的文法规则：

```c
d → MEM(+(a, CONST))
d → MEM(+(CONST, a))
d → MEM(CONST)
d → MEM(a)
d → a        // MOVEA
a → d        // MOVED
```

这种文法**高度歧义**：同一个表达式有许多不同的指令序列实现，第 3 章的 parsing 技术在这里没什么用。但 DP 算法的推广运作良好：**对每个节点、对文法的每个非终结符，分别计算最小代价匹配**（即每个节点记录"作为 a 寄存器算出的最小代价"和"作为 d 寄存器算出的最小代价"）。

### Fast Matching

Maximal Munch 和 DP 都要检查**所有**与某节点匹配的瓦片。一个瓦片匹配，当且仅当瓦片的每个非叶节点的算符（MEM、BINOP、CONST……）与树中对应节点相同。

加速方法：用节点 n 的标号做 `switch`（即决策树），目标是 IR 树中**每个节点不被查看两次**：

```c
match(n) {
  switch (label(n)) {
    case MEM: ...
    case BINOP: ...
    case CONST: ...
  }
}
```

### Efficiency of Tiling Algorithms

设：

| 符号 | 含义 |
| --- | --- |
| T | 不同瓦片的数量 |
| K | 匹配瓦片平均含 K 个非叶节点 |
| K′ | 判断某子树处哪些瓦片匹配时，最多需要检查的节点数 |
| T′ | 每个树节点处平均匹配的模式数 |
| N | 输入树的节点数 |

- Maximal Munch：时间正比于 $(K' + T')\,N / K$
- DP：时间正比于 $(K' + T')\,N$

由于 K、K′、T′ 都是与 N 无关的常数，**两种算法的运行时间都是线性的**。

## CISC Machines

RISC 与 CISC 的典型对比：

| RISC | CISC |
| --- | --- |
| 32 个寄存器 | 寄存器很少（16/8/6 个） |
| 只有一类整数/指针寄存器 | 寄存器分类，某些操作只能用特定寄存器 |
| 算术运算只在寄存器间进行 | 算术运算可通过“寻址模式”访问寄存器或内存 |
| 三地址指令 `r1 ← r2 ⊕ r3` | 二地址指令 `r1 ← r1 ⊕ r2` |
| load/store 只有 `M[reg+const]` 一种寻址模式 | 多种寻址模式 |
| 每条指令定长 32 位 | 变长指令（变长 opcode + 变长寻址模式） |
| 每条指令只有一个结果/效果 | 指令可带副作用（如“自增”寻址模式） |

在 CISC 上做指令选择会遇到一系列问题，对应的解决方案：

**1. 寄存器太少**

- 解决：放开手脚地生成 TEMP 节点，假设寄存器分配器会处理好

**2. 寄存器分类**

- 例：Pentium 上乘法的左操作数必须在 `eax`，结果高位放入 `edx`（对 Tiger 无用）、低位在 `eax`
- 解决：**显式地搬运操作数和结果**。实现 `t1 ← t2 × t3`：

```c
mov eax, t2     // eax ← t2
mul t3          // eax ← eax × t3;  edx ← 垃圾
mov t1, eax     // t1 ← eax
```

**3. 二地址指令**：目标寄存器必须与第一个源寄存器相同

- 解决：**额外加一条 move**。实现 `t1 ← t2 + t3`：

```c
mov t1, t2      // t1 ← t2
add t1, t3      // t1 ← t1 + t3
```

之后寄希望于寄存器分配器把 t1、t2 分到同一个寄存器，从而把 move 删掉。

**4. 算术运算可寻址内存**

- 指令选择阶段把每个 TEMP 都当作"寄存器"，但其中很多最终其实是内存位置
- 解决：**运算前把操作数全部取入寄存器，运算后再存回内存**。两种写法：

```c
mov eax, [ebp - 8]
add eax, ecx              // 等价于   add [ebp - 8], ecx
mov [ebp - 8], eax
```

右边更简洁，但两者**一样快**；左边的明显缺点是**毁掉了 eax 中原有的值**。

**5. 多种寻址模式**

- 一个完成 6 件事的寻址模式通常也要 6 步才能执行完——**并不更快**
- 它的两个好处：“糟蹋”更少的寄存器、指令编码更短
- 结论：树匹配指令选择经过一些工作也能选出 CISC 指令，但只用简单的 RISC 风格指令，程序同样快

**6. 带副作用的指令**

- 例：某些机器有"自增"内存取指令，一条指令产生**两个结果**：`r2 ← M[r1]; r1 ← r1 + 4`，树模式难以表达
- 三种解决方案：
	1. **无视**这些指令，希望它们消失（眼不见为净）
	2. 在树模式匹配代码生成器中，用 ad hoc 的方式匹配特殊惯用法
	3. 换用完全不同的指令选择算法，基于 **DAG 模式**（参见龙书）而非树模式

## Instruction Selection for the Tiger Compiler

被瓦片铺满的树中，**每块瓦片的根对应一个保存在寄存器中的中间结果**。用哪个寄存器？这是**寄存器分配**的任务。寄存器分配的许多方面与具体目标指令集无关，它可以放在指令选择之前或之后：

- 放在**之前**：此时还不知道哪些树节点需要寄存器保存结果（只有瓦片的根需要显式寄存器，瓦片内部的节点不需要），无法很准确
- 因此我们选择在指令选择**之后**做寄存器分配

### Abstract Assembly Language Instructions

指令选择的输出是**抽象汇编指令** `AS_instr`——"还没分配寄存器的汇编语言"：

```c
typedef struct {Temp_labelList labels;} *AS_targets;
typedef struct {
  enum {I_OPER, I_LABEL, I_MOVE} kind;
  union {
    struct {string assem; Temp_tempList dst, src; AS_targets jumps;} OPER;
    struct {string assem; Temp_label label;} LABEL;
    struct {string assem; Temp_tempList dst, src;} MOVE;
  } u;
} *AS_instr;
```

- **OPER**：普通操作
	- `assem`：汇编指令字符串
	- `src`：操作数寄存器列表（可为空）
	- `dst`：结果寄存器列表（可为空）
	- `jumps`：可能跳转到的目标标签列表；总是顺序执行（fall through）的指令 `jumps = NULL`
- **LABEL**：程序中跳转可达的一个点。`assem` 是该标签在汇编中的样子，`label` 记录用的是哪个标签符号
- **MOVE**：与 OPER 类似，但**只做数据传送**（这一区分方便寄存器分配器之后做 move 合并/删除）

`AS_print(f, i, m)` 把指令 i 格式化输出到文件 f，其中 m 是 **Temp_map**——告诉每个 temp 被分配到哪个寄存器的映射表：

```c
/* temp.h */
typedef struct Temp_map_ *Temp_map;
Temp_map Temp_empty(void);                              // 新建空映射
Temp_map Temp_layerMap(Temp_map over, Temp_map under);  // 分层叠加
void Temp_enter(Temp_map m, Temp_temp t, string s);
string Temp_look(Temp_map m, Temp_temp t);
```

Temp_map 就是一张 key 为 `Temp_temp`、值为字符串的表；映射可以**分层**：若 `σ3 = layerMap(σ1, σ2)`，则 `look(σ3, t)` 先查 σ1，失败再查 σ2。主要使用者是寄存器分配器。

### Machine-Independence

`AS_instr` 类型**独立于目标机汇编语言**：它不知道寄存器的最终分配，在 assem 字符串中用 `` `s0 ``（第 0 个源寄存器）、`` `d0 ``（第 0 个目标寄存器）这样的占位符指代：

```c
AS_Oper("LOAD `d0 <- M[`s0+8]",
        Temp_TempList(Temp_newtemp(), NULL),     // dst: 新 temp
        Temp_TempList(T_Temp(F_FP()), NULL),     // src: FP
        NULL)                                    // jumps
```

寄存器分配后，真正的汇编可能是 `LOAD r1 <- M[r27+8]`。

!!! example "另一个例子（t908、t909、t910 是指令选择器新选的 temporaries）"
	| assem | dst | src |
	| --- | --- | --- |
	| ``ADDI `d0 <- `s0 + 3`` | t908 | t87 |
	| ``LOAD `d0 <- M[`s0 + 0]`` | t909 | t92 |
	| ``MUL `d0 <- `s0 * `s1`` | t910 | t908, t909 |
	
	寄存器分配后可能变成：
	
	```c
	ADDI r1 <- r12+3
	LOAD r2 <- M[r13+0]
	MUL  r1 <- r1 * r2
	```

对于**二地址指令**（一个操作数既是源又是目标），如 `add t1, t2`（效果 `t1 ← t1 + t2`），表示为：

| assem | dst | src |
| --- | --- | --- |
| ``add `d0, `s1`` | t1 | t1, t2 |

其中 `` `s0 ``（= t1）在 assem 字符串中**隐式**出现（`` `d0 `` 与它是同一个寄存器），没有被显式写出。

### Producing Assembly Instructions

如何把 Tree 表达式 "munch" 成 Assem 指令？`munchStm` 与 `munchExp` **自底向上**地以**副作用**方式发射指令；`munchExp` 返回保存结果的 temporary：

```c
static Temp_temp munchExp(T_exp e);
switch (e) {  // 按瓦片匹配
  case MEM(BINOP(PLUS, e1, CONST(i))): {
    Temp_temp r = Temp_newtemp();           // 新寄存器名
    emit(AS_Oper("LOAD `d0 <- M[`s0+" + i + "]\n",
         L(r, NULL), L(munchExp(e1), NULL), NULL));
    return r;
  }
  ...
}

static void munchStm(T_stm s);
case MOVE(MEM(BINOP(PLUS, e1, CONST(i))), e2):
    // 与 munchExp 几乎一样，但不创建新寄存器（语句无返回值）
    emit(AS_Oper("STORE M[`s0+" + i + "] <- `s1\n",
         NULL, L(munchExp(e1), L(munchExp(e2), NULL)), NULL));
```

`emit` 只是把指令累积到一个链表里，最后整体返回：

```c
/* codegen.c */
static AS_instrList iList = NULL, last = NULL;
static void emit(AS_instr inst) {
  if (last != NULL)
    last = last->tail = AS_InstrList(inst, NULL);
  else
    last = iList = AS_InstrList(inst, NULL);
}
```

### Procedure Calls

过程调用 `EXP(CALL(f, args))` 与函数调用 `MOVE(TEMP t, CALL(f, args))` 由如下瓦片匹配：

```c
case EXP(CALL(e, args)): {
  Temp_temp r = munchExp(e);
  Temp_tempList l = munchArgs(0, args);
  emit(AS_Oper("CALL `s0\n", calldefs, L(r, l), NULL));
}
```

- `munchArgs(i, args)`：生成把所有实参移动到正确位置（寄存器或内存）的代码；参数 i 表示当前处理第 i 个实参，递归处理第 i+1 个……
- CALL 会"糟蹋"（trash）某些寄存器：**caller-save 寄存器、返回地址寄存器、返回值寄存器**。这份列表 `calldefs` 要作为 CALL 的 **destinations（dst）** 列出，让寄存器分配器知道这些寄存器的旧值会被破坏

!!! note "一般地，任何带有“写其他寄存器”副作用的指令都需要这样处理（把被写的寄存器列入 dst）"

### If There's No Frame Pointer

使用帧指针时，每次过程调用要：把 SP 复制到 FP，再把 SP 加上新帧大小。其实可以不用真实的 FP，改用**虚拟帧指针（Virtual Frame Pointer）**：

- **省时间**（少一条复制指令）、**省空间**（多出一个寄存器可作他用）
- 做法：`codegen` 函数把所有对 `FP + k` 的引用替换为 `SP + k + fs`，其中 fs 是帧大小（编译期对每个函数已知）