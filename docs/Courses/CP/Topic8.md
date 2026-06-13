
# Basic Blocks and Traces

## Motivation

语义分析阶段生成的 IR Tree 最终要被翻译成汇编/机器语言。虽然 Tree 语言的算符是精心挑选的、与大多数机器的能力相匹配，但仍存在一些**不匹配（mismatch）**：有些方面与机器语言不能精确对应，有些方面则会妨碍编译期的优化分析。

具体来说，Tree 与机器语言程序之间的不匹配有：

- **CJUMP 有两个跳转目标**：`CJUMP(op, e1, e2, t, f)` 可以跳向 t 或 f 两个标签；而真实机器的条件跳转指令只有一个目标——条件为假时 *fall through* 到下一条指令
- **表达式内部的 ESEQ 节点很不方便**：ESEQ 带有副作用，*兄弟子树不同的求值顺序会产生不同的结果*。而我们希望能以任意顺序求值子表达式（方便优化）
- **表达式内部的 CALL 节点有同样的问题**：
	- CALL 也有副作用，求值顺序影响结果
	- 此外，当 CALL 出现在另一个 CALL 的参数表达式中时，如 `CALL(f, [e1, CALL(g, [e2, ...])])`，内层调用会破坏存放参数的固定寄存器（formal-parameter registers），引发冲突

**解决方案**：对 IR Tree 进行三个阶段的变换：

**Stage 1**：把树重写为一组**规范树**（Canonical Trees）的线性列表，其中不含 SEQ 和 ESEQ 节点：

![[cp_topic8_1.png]]

**Stage 2**：把这个列表划分为一组**基本块**（Basic Blocks），每个基本块内部不含跳转和标签：

![[cp_topic8_2.png]]

**Stage 3**：把基本块排序成一组**轨迹**（Traces），使得每个 CJUMP 后面紧跟它的 false label：

![[cp_topic8_3.png]]

## Canonical Trees

**规范树**满足以下两条性质：

1. **不含 SEQ 或 ESEQ 节点**
2. **每个 CALL 节点的父节点只能是 `EXP(...)` 或 `MOVE(TEMP t, ...)`**

- 由性质 1 可知，每棵规范树**只含一个 statement node，即根节点**，其余节点全部是 expression node（SEQ/ESEQ 是仅有的"语句嵌入表达式/语句串联"结构，去掉后语句无法再嵌套）。
- 结合性质 2，CALL 的父节点必须是规范树的根，且只能是 `EXP(..)` 或 `MOVE(TEMP t, ..)`；又由于这两种形式都只能容纳一个 CALL，所以**每棵规范树中至多只有一个 CALL 节点**。

要完成 Stage 1 的变换，需要做三件事：

1. 消除 ESEQ（eliminate ESEQs）
2. 把 CALL 提升到顶层（move CALLs to top level）
3. 消除 SEQ（eliminate SEQs）

### Transformations on ESEQ

`ESEQ(s, e)` 表示先执行语句 s（产生副作用），再求值 e 作为结果。

消除 ESEQ 的核心思想是：**把 ESEQ 节点在树中不断上提（lift higher and higher），直到它的父节点是语句节点，此时它就可以变成一个 SEQ 节点**。

基本的提升规则——给定 `ESEQ(s, e)`，将 s 抽出，根据父节点的类型用 ESEQ（父为表达式）或 SEQ（父为语句）重写：

![[cp_topic8_4.png]]

$$
\begin{aligned}
\text{ESEQ}(s_1, \text{ESEQ}(s_2, e)) &= \text{ESEQ}(\text{SEQ}(s_1,s_2), e)\\
\text{BINOP}(op, \text{ESEQ}(s, e_1), e_2) &= \text{ESEQ}(s, \text{BINOP}(op, e_1, e_2))\\
\text{MEM}(\text{ESEQ}(s, e_1)) &= \text{ESEQ}(s, \text{MEM}(e_1))\\
\text{JUMP}(\text{ESEQ}(s, e_1)) &= \text{SEQ}(s, \text{JUMP}(e_1))\\
\text{CJUMP}(op, \text{ESEQ}(s, e_1), e_2, l_1, l_2) &= \text{SEQ}(s, \text{CJUMP}(op, e_1, e_2, l_1, l_2))
\end{aligned}
$$

!!! note "上述规则中 ESEQ 都出现在**第一个**子表达式的位置，s 的副作用本来就先于其他部分执行，所以可以直接上提，不改变语义"

**【Question】** 如果 ESEQ 出现在 BINOP/CJUMP 的**第二个**操作数位置，如 `BINOP(op, e1, ESEQ(s, e2))`，能否直接上提？

![[cp_topic8_5.png]]

**不能**。原树中求值顺序为 `e1 → s → e2`；若直接把 s 提到最外层，顺序变成 `s → e1 → e2`。当 s 修改了 e1 依赖的状态时（例如 `s = MOVE(MEM(x), y)`，`e1 = MEM(x)`），结果就会改变。

为保持求值顺序，必须**用一个新的临时变量 t 把 e1 的值先取出来**，再上提 s：

![[cp_topic8_6.png]]

$$
\begin{aligned}
\text{BINOP}(op, e_1, \text{ESEQ}(s, e_2)) &= \text{ESEQ}(\text{MOVE}(\text{TEMP }t, e_1),\ \text{ESEQ}(s, \text{BINOP}(op, \text{TEMP }t, e_2)))\\
\text{CJUMP}(op, e_1, \text{ESEQ}(s, e_2), l_1, l_2) &= \text{SEQ}(\text{MOVE}(\text{TEMP }t, e_1),\ \text{SEQ}(s, \text{CJUMP}(op, \text{TEMP }t, e_2, l_1, l_2)))
\end{aligned}
$$

但如果 **s 与 e1 可交换（commute）**，就不需要引入临时变量，可以直接上提：

![[cp_topic8_7.png]]

$$
\begin{aligned}
\text{BINOP}(op, e_1, \text{ESEQ}(s, e_2)) &= \text{ESEQ}(s, \text{BINOP}(op, e_1, e_2))\\
\text{CJUMP}(op, e_1, \text{ESEQ}(s, e_2), l_1, l_2) &= \text{SEQ}(s, \text{CJUMP}(op, e_1, e_2, l_1, l_2))
\end{aligned}
$$

!!! info "**Commute 的定义**：若 s 赋值的临时变量和内存位置都不被 e1 引用（且 s 和 e1 不同时执行外部 I/O），则 s 与 e1 可交换"

是否可交换在编译期**不总是可判定的**。例如 `s = MOVE(MEM(x), y)`，`e = MEM(z)`：是否交换取决于运行时 `x == z` 是否成立。因此我们做**保守近似**（conservative approximation）：

- `commute(s, e) = True`：当 s 和 e **一定**可交换时
- `commute(s, e) = False`：其他所有情况

一个（非常朴素的）`commute` 估计函数：

- **常量**（`CONST(i)`、`NAME(n)`）与任何语句可交换
- **空语句**（如 `EXP(CONST(i))`）与任何表达式可交换
- 其他情况一律视为**不可交换**

!!! quote "保守近似是编译器分析的常见策略：宁可错失优化机会（引入多余的 MOVE），也不能改变程序语义"

#### General Rewriting Rules

对每一种 Tree 语句/表达式，都可以制定类似的规则把其中的 ESEQ 抽出来。一般化的做法是：先识别出该语句/表达式的**子表达式列表**，然后做 Rewrite——抽出子表达式中的"语句"部分，并更新子表达式。

以子表达式列表 `[e1, e2, ESEQ(s, e3)]` 为例：

- 若 s 与 e1、e2 都可交换：

```c
(s; [e1, e2, e3])
```

- 若 s 与 e2 不可交换：

```c
(SEQ(MOVE(t1, e1), SEQ(MOVE(t2, e2), s)); [TEMP(t1), TEMP(t2), e3])
```

- 若 s 与 e2 可交换、但与 e1 不可交换：

```c
(SEQ(MOVE(t1, e1), s); [TEMP(t1), e2, e3])
```

完整的重写算法分两步：

1. **Subexpression-extraction**：为每种 Tree 语句/表达式编写"子表达式抽取"方法——抽出子表达式中的语句，并把每个子表达式转换成无 ESEQ 的版本
2. **Subexpression-insertion**：编写"子表达式插入"方法——用各子表达式的无 ESEQ 版本重建该语句/表达式

对一个 Tiger 程序（一个 `T_stm`），递归地执行该变换，所有 ESEQ 就会被逐层上提，最终全部变成 SEQ 节点。

!!! example "处理 `CJUMP(<, CONST 343, MEM(ESEQ(sx, TEMP a)), t, f)`"
	1. 处理左子表达式 `CONST(343)`：抽出空语句 `s1 = EXP(CONST(0))`，`e1' = CONST(343)`
	2. 处理右子表达式 `MEM(ESEQ(sx, TEMP a))`：递归上提 ESEQ 得 `ESEQ(sx, MEM(TEMP a))`，故 `s2 = sx`，`e2' = MEM(TEMP a)`
	3. 更新子表达式：left → `CONST(343)`，right → `MEM(TEMP a)`
	4. 抽出语句 `SEQ(s1, s2) = SEQ(EXP(CONST(0)), sx)`，去掉空语句后即 `sx`；由于 `commute(s2, e1') = True`（e1' 是常量），无需添加 MOVE
	5. 返回 `SEQ(sx, CJUMP(<, CONST 343, MEM(TEMP a), t, f))`

### Move CALLs to Top Level

Tree 语言允许 CALL 作为子表达式使用，但每个函数都把返回值放进**同一个专用寄存器** `TEMP(RV)` 中。于是对于：

```c
BINOP(PLUS, CALL(...), CALL(...))
```

第二个 CALL 会在 PLUS 执行之前**覆盖 RV 寄存器**，第一个调用的结果就丢失了。

解决思路：**把每个返回值立即存入一个新的临时寄存器**：

```c
CALL(fun, args)
// 👇👇👇
ESEQ(MOVE(TEMP t, CALL(fun, args)), TEMP t)
```

这样改写后，ESEQ 消除器自然会把其中的 MOVE 上提（percolate up）到包含它的 BINOP 等表达式之外——CALL 也就被提升到了顶层，其父节点恰好是 `MOVE(TEMP t, ...)`，满足规范树的性质 2。

### Eliminate SEQs（Linearize）

整个函数体 s0 经过上述处理后得到 s0′，所有 SEQ 节点都集中在树的**顶部附近**，形如 `SEQ(SEQ(SEQ(..., sx), sy), sz)`。

`linearize` 函数反复应用结合律规则：

```c
SEQ(SEQ(a, b), c) = SEQ(a, SEQ(b, c))
```

把 s0′ 整理为右倾的 `SEQ(s1, SEQ(s2, ..., SEQ(sn-1, sn)...))`。由于此时 SEQ 不再提供任何结构信息，可以直接丢弃 SEQ，把结果视为一个简单的**语句列表**：

$$s_1,\ s_2,\ \dots,\ s_{n-1},\ s_n$$

其中任何 $s_i$ 都不含 SEQ 或 ESEQ 节点，即一组规范树。Stage 1 完成。

## Basic Blocks

确定程序中跳转的去向，就是在分析程序的**控制流（Control Flow）**。控制流是程序指令的执行顺序，分析时：

- 忽略寄存器和内存中的数据值
- 忽略算术计算

也就是说，对控制流分析而言，任何非跳转指令的行为都是"无趣的"——因此可以**把连续的非跳转指令打包成一个基本块**，只分析基本块之间的控制流。

**基本块**是一个语句序列，控制流总是**从块首进入、从块尾离开**，即：

- 第一条语句是 `LABEL`
- 最后一条语句是 `JUMP` 或 `CJUMP`
- 中间没有其他的 `LABEL`、`JUMP`、`CJUMP`

把长语句序列划分为基本块的算法：

1. 从头到尾扫描语句序列
2. 遇到 `LABEL`：**开始**一个新块（同时结束上一个块）
3. 遇到 `JUMP` / `CJUMP`：**结束**当前块（下一条语句开始新块）
4. 收尾修补：
	- 若某块结尾不是 `JUMP`/`CJUMP`，则在块尾**补一条**跳到下一块标签的 `JUMP`
	- 若某块开头没有 `LABEL`，则**发明一个新标签**放在块首

**【Question】** 为什么要显式补上 JUMP 和 LABEL？

因为补全后每个基本块都是**自包含**的：进入点和退出目标都显式可见，不依赖于块在序列中的物理位置。这样后续才能**以任意顺序重新排列基本块**（Stage 3 的 traces 正需要这一点），而程序语义不变。

!!! example "对 TAC 划分基本块"
	```c
	(1)  x := input
	(2)  y := x - 1
	(3)  z := x * y
	(4)  if z < x goto (7)
	(5)  p := x / y
	(6)  q := p + y
	(7)  a := q
	(8)  b := x + a
	(9)  c := a - b
	(10) if p == q goto (12)
	(11) goto (3)
	(12) return
	```
	- (3)、(7)、(12) 是跳转目标（LABEL），各自开始新块
	- (4)、(10)、(11) 是跳转，各自结束所在块，(5)、(11)、(12) 开始新块
	- 划分结果：`{1,2}`、`{3,4}`、`{5,6}`、`{7,8,9,10}`、`{11}`、`{12}`
	- 对 TAC 可忽略"补 LABEL/JUMP"这一步，但对 Tree 语言必须执行

## Traces

基本块可以**以任意顺序排列**，程序的执行结果不变（这正是补全 JUMP/LABEL 带来的好处）。基于这一点，我们可以选择一种满足以下条件的排列：

- **每个 CJUMP 后面紧跟它的 false label**——这样 CJUMP 就能直接翻译成真实机器的"条件跳转 + fall through"
- 同时尽量让无条件 JUMP 紧跟其目标标签——这样的 JUMP 可以**直接删除**，程序更快：

```c
JUMP(NAME next)
LABEL(next)      // JUMP 可删
```

**Trace**（**轨迹**）是程序执行期间**可能被连续执行**的一个语句序列（可以包含条件分支）。一个程序可以有许多不同的、相互重叠的 traces，但为了安排 CJUMP 和 false label，我们想要一组**恰好覆盖**整个程序的 traces：

- 每个基本块恰好属于一个 trace
- 覆盖集中的 traces **越少越好**——trace 之间的切换需要 JUMP，traces 越少，JUMP 越少

如何找覆盖 traces？思路是：**从某个块出发（trace 的开头），沿着一条可能的执行路径走下去（trace 的剩余部分）**：

- 若 b1 以 JUMP 跳到 b4，b4 又 JUMP 到 b6，则可形成 trace `b1, b4, b6`
- 若 b6 以 `CJUMP(cond, b7, b3)` 结尾，则把 **false 分支 b3** 接到当前 trace 后继续（保证 CJUMP 紧跟 false label），b7 留给其他 trace

![[cp_topic8_8.png]]

这本质上是对**控制流图（CFG）做深度优先搜索**。Algorithm 8.3（生成覆盖程序的 traces 集合，且每块只属于一个 trace）：

1. 从某个块开始，沿着 JUMP 链一路走下去
2. 给路过的每个块**打标记**，并追加到当前 trace
3. 直到来到一个**所有后继都已被标记**的块，结束当前 trace
4. 另选一个未标记的块，开始下一条 trace；直到所有块被标记

```c
把程序中的所有块放入列表 Q
while Q 非空:
    开始一条新的 (空) trace T
    从 Q 中取出队首块 b
    while b 未被标记:
        标记 b; 把 b 追加到 T
        b = b 的某个未标记的后继 (优先 false 分支)
    结束 trace T
```

!!! warning "Algorithm 8.3 生成的 traces 中，CJUMP 有可能不紧跟其 false label"

### Finishing Up

把排好序的 traces 重新展平成一个长语句列表，然后做少量调整，使**每个** CJUMP 都紧跟 false label：

- CJUMP 后紧跟 **false label**：保持不动（大多数情况）
- CJUMP 后紧跟 **true label**：交换 true/false 标签，并把条件取反（如 `<` 变 `>=`）
- CJUMP 后**两个标签都不跟**：发明一个新的 false label `lf′`，把一条 CJUMP 改写成三条：

```c
CJUMP(cond, a, b, lt, lf′)
LABEL lf′
JUMP(NAME lf)
```

### Optimal Traces

不同的 trace 覆盖有优劣之分。**频繁执行的指令序列（如循环体）应该独占一条 trace**：

- 这能最小化无条件跳转的数量
- 也有利于其他优化（寄存器分配、指令调度等）

考虑 while 循环的三种布局：

![[cp_topic8_9.png]]

- **(a)** 测试在顶部：每次迭代需要 1 条 CJUMP（不跳）+ 1 条回到 test 的 JUMP，共 2 条跳转指令
- **(b)** CJUMP 紧跟 false label（done），但循环体在 epilogue 之后：每次迭代 CJUMP **被采纳**（taken）跳到 body，再 JUMP 回 test，依然 2 条跳转，且条件跳转每次都跳转，对分支预测/流水线不友好
- **(c)** 测试在**底部**：入口处一条 JUMP 跳到 test；此后每次迭代只需 1 条 CJUMP（为真跳回 body，为假 fall through 到 done）——**每次迭代只有 1 条跳转指令，最优**

!!! success "结论：(c) 最好。把循环测试放到循环体底部、让整个循环体占据一条 trace，是编译器的经典做法（loop rotation / bottom-test loop）"