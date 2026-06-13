
# Liveness Analysis

## Motivation

- **核心问题**：IR 可以使用**无限多**的临时变量（temporaries），而真实机器只有**有限个**寄存器。
- **解决思路**：如果两个临时变量 a、b *从不在同一时刻"被使用"*，它们就可以放进**同一个寄存器**。例如 `a = b + 1; return a;` 中，b 用完之后 a 才开始有值，二者可共用。当临时变量多到寄存器装不下时，多出来的才放进内存。

那么如何判断两个变量是否"同时被使用"？这就是**活跃性分析（Liveness Analysis）**：确定每个变量的活跃区间，以便共享寄存器。

!!! info "Live（活跃）：一个变量是活跃的，当且仅当它持有的值**在将来可能还会被用到**"

如何分析"变量 x 在语句 n 之后还会被用到吗"？分两步：

1. 哪些语句可能在 n 之后执行？→ 构造**控制流图（Control Flow Graph，CFG）**
2. 这些语句会使用 x 吗？→ 分析语句，**从未来往过去**反推（**Backward Analysis**）

## A Running Example

构造 CFG：**节点**是语句，**边**是控制流——若语句 m 后可能紧接着执行语句 n，则有边 m → n：

![[cp_topic10_1.png]]

逐个变量分析活跃性（某变量在一条**边**上活跃，指从该边出发存在一条**不经过任何 def** 就到达某个 **use** 的有向路径）：

![[cp_topic10_2.png]]

- **b**：在 2 中被定义，在 4 中被使用 → b 在边 2→3、3→4 上活跃。注意 4 之后 b 不再活跃（4 重新定义 a 前 b 已用完）
- **a**：在 1、4 中被定义，在 2、5 中被使用 → a 在 1→2、4→5、5→2 上活跃（但在 2→3→4 上**不**活跃，因为之后对 a 的"使用"前会先经过 4 的重新定义）
- **c**：在 3 中既被使用又被定义，在 6 中被使用 → c 在**整个图上**都活跃（包括入口边——说明 c 可能是函数参数；若 c 是局部变量，则它在入口处活跃意味着**使用了未初始化的变量**，编译器可借此发出警告）

由于 **a 和 b 从不同时活跃**，整个程序其实只需要 **2 个寄存器**：一个给 c，一个给 a 和 b 共用。

## Terminology

变量的活跃性沿着 CFG 的边"**流动**"，活跃性分析是**数据流问题**（dataflow problem）。

流图术语——节点 n 拥有：

- **out-edges**：指向**后继**（successor）节点的边；`succ[n]` 为后继集合
- **in-edges**：来自**前驱**（predecessor）节点的边；`pred[n]` 为前驱集合

例如上图中：节点 5 的 out-edges 为 {5→6, 5→2}，`succ[5] = {2, 6}`；节点 2 的 in-edges 为 {1→2, 5→2}，`pred[2] = {1, 5}`。

**Def 与 Use**：

- 对变量赋值即**定义**（**define**）该变量；变量出现在赋值号右侧（或其他表达式中）即**使用**（**use**）该变量
- `def(v)`：定义变量 v 的节点集合；`def(n)`：节点 n 定义的变量集合；use 同理

例如上图：`def(3) = {c}`，`def(a) = {1, 4}`，`use(3) = {b, c}`，`use(a) = {2, 5}`。

**Live-in 与 Live-out**：

- **Live-in**：变量在节点 n 的**任一条 in-edge** 上活跃，则它在 n 处 live-in；`in[n]` 为 n 的 live-in 集合
- **Live-out**：变量在节点 n 的**任一条 out-edge** 上活跃，则它在 n 处 live-out；`out[n]` 为 n 的 live-out 集合

## Calculation of Liveness

通过 use 和 def 计算 in/out，三条规则：

- <1> 若对某个 $m \in succ[n]$ 有 $a \in in[m]$，则 $a \in out[n]$
	- 后继入口活跃 → 本节点出口活跃
- <2> 若 $a \in use[n]$，则 $a \in in[n]$
	- 节点 n 使用 a → a 在 n 入口处活跃
- <3> 若 $a \in out[n]$ **且** $a \notin def[n]$，则 $a \in in[n]$
	- 出口活跃且本节点未重新定义 → 入口也活跃

![[cp_topic10_3.png]]

上图示例：设 `in[m1] = {d}`、`in[m2] = {e}`，n 为 `b < g`、p 为 `b := c+1`，则 `out[n] = {d, e}`，`in[n] = {b, g, d, e}`（b、g 被使用，d、e 透传），`out[p] = {b, g, d, e}`，`in[p] = {c, g, d, e}`（b 被 p 定义，故去掉 b 加入 c）。

把三条规则整理为**数据流方程**：

$$
in[n] = use[n] \cup (out[n] - def[n])\qquad
out[n] = \bigcup_{s \in succ[n]} in[s]
$$

用**迭代法**求解：

![[cp_topic10_4.png]]

```c
for each n:
    in[n] ← {}; out[n] ← {}
repeat
    for each n:
        in'[n] ← in[n]; out'[n] ← out[n]
        in[n] ← use[n] ∪ (out[n] − def[n])
        out[n] ← ⋃ (s∈succ[n]) in[s]
until in'[n] = in[n] and out'[n] = out[n] for all n   // 不动点
```

!!! quote "有时，按照 CFG 从后往前，先计算 `out` 再计算 `in` 的迭代效率会更高"
	![[cp_topic10_8.png]]

### Variants of the Calculation

- **基本块**：只有一个前驱、一个后继的节点没什么意思，可以把它们与前驱/后继**合并**，得到节点更少的图——每个节点是一个基本块，能显著减少迭代开销
- **一次一个变量**：不必同时算所有变量，在需要某个变量的信息时单独为它做数据流计算。这也很实用，因为很多 temporary 的活跃范围**非常短**（搜索很快终止）

### Representations of Sets

`in[n]`、`out[n]` 的两种实现方式（设程序有 N 个变量，机器字长 K 位）：

- **位数组（Bit Arrays）**：适合**稠密**集合。每个集合 N 位；并集相当于逐字按位或，一次并集操作 N/K 次机器操作
- **有序链表（Sorted Lists）**：适合**稀疏**集合。按某个全序键（如变量名）排序；并集相当于归并链表。当集合平均元素数远小于 N/K 时，有序链表更快

### Time Complexity

设程序规模为 N：至多 N 个节点、N 个变量。

- 一次集合并操作 O(N)
- 内层 for 循环：每个节点常数次集合操作 → 一轮 O(N²)
- repeat 循环：所有 in/out 集合的总大小至多 2N²，且集合**单调增长**、不能无限增长 → 至多迭代 2N² 轮... 实际上界为 O(N²) 轮

**最坏情况 O(N⁴)**。实践中（配合恰当的计算顺序）通常在 **O(N) 到 O(N²)** 之间。

## Least Fixed Points

数据流方程的解**不唯一**。下表中 X 和 Y 都是方程的解（假设程序中还有一个本片段没用到的变量 d）：

![[cp_topic10_5.png]]

- **X** 是"自然"的解；**Y** 在很多集合里多塞了一个 d——它仍满足方程，只是**不精确**
- **Z** 不是解（它漏掉了某些必须活跃的变量）

方程的任何解都是一个**保守近似**：若某次执行中 a 在节点 n 之后真的被用到，那么**任何解**中都保证 $a \in out[n]$；但**反之不然**——$a \in out[n]$ 不代表它的值真的会被用。

这样的保守性是否可接受？对活跃性分析：

- 若两个变量被（可能错误地）认为同时活跃，我们只是把它们放进**不同**寄存器——结果是**可能多用几个寄存器，但程序计算结果一定正确**
- 即：保守近似可能**错误地认为变量活跃**，但**绝不会错误地认为变量死亡**
- 反之，若用不满足方程的 Z：根据 Z，b 和 c 从不同时活跃，于是把它们分到同一个寄存器——编译出的程序就**算错了**

!!! quote "用于编译器优化的数据流方程都应如此设计：任何解都向优化器提供**保守**的信息。不精确的信息只会导致次优的程序，而**绝不会导致错误的程序**"

**最小不动点（Least Fixed Point）**：若 X 是方程的解，且所有解都**包含** X，则称 X 为最小解/最小不动点。

!!! info "**定理**：Equation 10.3 存在最小不动点，且上述迭代算法计算出的**正是**最小不动点（证明见习题 10.2）"

## Static and Dynamic Liveness

考虑下面的程序：

![[cp_topic10_6.png]]

人眼能看出：节点 1 后 `a = b*b ≥ 0`，节点 2 后 `c = a+b ≥ b`，所以节点 3 的 `c ≥ b` **恒为真**，节点 4（`return a`）**永远不会执行**——即 a 在节点 2 之后其实不再需要。但数据流方程**不知道条件分支会走哪边**，仍认为 a 在 3→4 方向活跃。"更聪明"的方程本可让 a 和 c 共用一个寄存器。

那能不能造一个完全看透控制流的编译器？**不能**，可用停机问题证明：

!!! info "**定理（Halting）**：不存在程序 H(P, X)，对任何程序 P 与输入 X，当 P(X) 停机时返回 true、否则返回 false"

**推论**：不存在程序 H′(P, L) 能对任何程序 P 和其中的标签 L 判断"执行 P 时 L 是否会被到达"。（证明：若 H′ 存在，把 P 中所有 halt 改成 `goto L`、L 放在程序末尾，则 H′ 可实现 H，矛盾。）

这个定理并不是说"我们永远无法判断某个标签是否可达"，而是说**不存在对所有程序都有效的通用算法**。我们可以用特例算法改进活跃性分析，但没有编译器能真正判断一个变量的值是否**真的**被需要。因此只能采用保守近似：**假设条件分支两边都可能走**。

由此区分两种活跃性：

- **动态活跃（dynamic liveness）**：a 在节点 n 动态活跃，若程序的**某次实际执行**从 n 出发、不经过 a 的任何定义就到达 a 的某个使用（动态条件）
- **静态活跃（static liveness）**：a 在节点 n 静态活跃，若**存在**一条从 n 到 a 的某个使用的控制流边路径，不经过 a 的任何定义（静态近似）

!!! note "若 a 动态活跃，则它一定静态活跃（dynamic ⊆ static）。编译器使用静态活跃性"

## Interference Graphs

活跃性分析最重要的应用之一是**寄存器分配**：有临时变量 a, b, c, … 和寄存器 r1, …, rk，阻止 a 和 b 被分到同一个寄存器的条件称为**干涉（interference）**。两类干涉：

1. **活跃范围重叠**（overlapping live ranges）：a、b 同时活跃
2. **指令限制**：a 必须由某条**不能寻址寄存器 r1** 的指令产生，则 a 与 r1 干涉

干涉关系的两种表达方式——**矩阵**（用 x 标记干涉对）或**无向图**（节点 = 变量，边 = 相互干涉的变量对）：

![[cp_topic10_7.png]]

!!! success "寄存器分配 = 给干涉图**着色**：相邻节点不同色，颜色数 ≤ 寄存器数（详见下一章）"

### Special Treatment of MOVE Instructions

MOVE 指令需要特殊处理——**不要**在 MOVE 的源和目的之间制造人为的干涉。考虑：

```c
t := s          // copy
...
x := ... s ...  // use of s
...
y := ... t ...  // use of t
```

按一般规则，t 定义时 s 活跃，应加干涉边 (s, t)。但 s 和 t **装的是同一个值**，根本不需要两个寄存器——所以这种情况**不加边** (s, t)（这正是后续 move 合并优化的基础）。而如果之后 s 仍活跃时出现了对 t 的**非 move 重定义**（`t := ...`），那次定义自然会加上干涉边 (t, s)，不会出错。

因此，**为每个新定义添加干涉边的规则**是：

1. 对任何定义变量 a 的**非 move** 指令 n，设 $out[n] = \{b_1, \dots, b_j\}$，添加干涉边 $(a, b_1), \dots, (a, b_j)$
2. 对 **move** 指令 `a := c`，设 live-out 集合为 $\{b_1, \dots, b_j\}$，对每个**不等于 c** 的 $b_i$ 添加干涉边 $(a, b_i)$

## Liveness in the Tiger Compiler

Tiger 编译器的流分析分**两个阶段**：

1. 分析 Assem 程序的控制流 → 生成**控制流图**
2. 在控制流图上分析变量活跃性 → 生成**干涉图**

### Graphs 模块

算法中图的每个节点要代表某个"东西"（如一条指令）。两种节点-信息关联方式：

```c
G_enter(mytable, n, x)   // 用 G_table（哈希表）把信息 x 关联到节点 n
n = G_Node(g, x)         // 或者直接把 x 放进节点；G_nodeInfo(n) 取回 x
```

### FlowGraph 模块

管理控制流图：节点 = 一条指令（或一个基本块）；边 (m, n) = 指令 m 之后可能执行 n（跳转或顺序落下）。每个节点的属性：

```c
/* flowgraph.h */
Temp_tempList FG_def(G_node n);   // 该节点定义的 temporaries（目的寄存器）
Temp_tempList FG_use(G_node n);   // 该节点使用的 temporaries（源寄存器）
bool FG_isMove(G_node n);         // 是否 MOVE 指令（若 def=use 可删除）
G_graph FG_AssemFlowGraph(AS_instrList il);
```

!!! note "不同的分析需要在节点上关联不同的信息（use/def、live-in/live-out……）。不要为每种新分析去改图这个被广泛使用的接口——**图就是图**，流图 = 图 + **单独打包**的辅助信息（表或者节点到信息的映射函数）"

### Liveness 模块

- **输入**：流图
- **输出**：**干涉图** + 一张节点对列表（代表 MOVE 指令，**希望被分配到同一寄存器**的 src/dst 对，供寄存器分配器做 move 合并）

```c
/* liveness.h */
typedef struct Live_moveList_ *Live_moveList;
struct Live_moveList_ {G_node src, dst; Live_moveList tail;};
Live_moveList Live_MoveList(G_node src, G_node dst, Live_moveList tail);

struct Live_graph { G_graph graph; Live_moveList moves; };
Temp_temp Live_gtemp(G_node n);     // 节点 n 代表哪个 temporary

struct Live_graph Live_liveness(G_graph flow);
```

实现时维护一个记录每个流图节点出口活跃集合的 **liveMap**：

```c
static void enterLiveMap(G_table t, G_node flowNode, Temp_tempList temps) {
    G_enter(t, flowNode, temps);
}
static Temp_tempList lookupLiveMap(G_table t, G_node flownode) {
    return (Temp_tempList)G_look(t, flownode);
}
```

有了完整的 liveMap，就能按前述规则构造干涉图。

**【Question】** 如果一个新定义的 temporary 在定义后**立即就不活跃**（定义了但从未使用）怎么办？

- 它确实不需要寄存器来"保存"值；但若定义它的指令**因为其他副作用仍要执行**，它就会写入某个寄存器
- 这个寄存器**绝不能**装着其他活跃变量——因此**零长度的活跃范围也会与所有覆盖它的活跃范围干涉**（仍要按规则给它加干涉边）
