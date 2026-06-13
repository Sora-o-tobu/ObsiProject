
# Register Allocation

## Motivation

上一章活跃性分析（Liveness Analysis）为我们产出了**干涉图（Interference Graph）**。本章要解决的，正是干涉图最重要的用途——**寄存器分配（Register Allocation）**：把 IR 中**无限多**的临时变量（temporaries）映射到机器上**有限个**寄存器。

![[cp_topic11_01.png]]

IR（中间表示）允许使用任意多的 temporary（t1、t2、…、t9、…），但真实机器只有固定数量的寄存器（如 RISC-V 的 x0–x31，其中可自由分配的只有十几个）。寄存器分配器的工作就是：

- **核心任务**：把众多 temporary 分配到少量机器寄存器上
- **附带优化**：尽量让一条 `MOVE` 指令的**源和目的**落在**同一个寄存器**里，这样该 `MOVE` 就可以**直接删除**

!!! info "如何做：先求干涉图，再给图着色"
	- **求干涉图**：通过活跃性分析得到（上一章内容）。
	- **着色（coloring）**：给干涉图的每个节点染一种颜色，使得**任何一条干涉边相连的两个节点颜色不同**；**颜色 ↔ 寄存器**。颜色数 = 可用寄存器数 K。

换句话说：寄存器分配 = 用 **K 种颜色给干涉图着色**。被干涉边连接的两个 temporary 同时活跃，必须放入不同寄存器，因此必须染不同颜色。

## Coloring by Simplification

寄存器分配本质上是 **NP-完全（NP-complete）** 问题，图着色同样是 NP-完全的。

![[cp_topic11_02.png]]

虽然找到"用恰好 K 色着色"的精确解在最坏情况下不可行，但存在一个效果很好的**线性时间近似算法（linear-time approximation algorithm）**。它由四个主要阶段组成：

!!! note "四个主要阶段：**Build → Simplify → Spill → Select**"

### Build

通过活跃性分析构造**干涉图**：

- 每个**节点**代表一个 temporary；
- 边 $(t_1, t_2)$ 表示这两个 temporary **不能**分配到同一个寄存器。

产生干涉边最常见的原因，就是 $t_1$ 与 $t_2$ **在某一时刻同时活跃**（live at the same time）。

### Simplify

用一个简单的启发式规则给图着色。设可用寄存器数为 **K**：

!!! success "若节点 n 的度数 < K，把 n 从图 G 中删去得到 G'。**若 G' 能用 K 色着色，则 G 也能。**"
	**为什么？** n 的邻居不超过 K−1 个，所以它们最多用掉 K−1 种颜色；无论它们怎么染，**总会剩下至少一种颜色**留给 n。

这自然导出一个**基于栈 stack-based** 的着色算法：

> 反复地把**度数小于 K** 的节点从图中移除，并**压入栈**。每移除一个节点，都会降低其邻居的度数，从而为后续移除创造更多机会。

### Spill

简化过程中，可能到达某一步：图 G 中**剩下的节点度数全部 ≥ K**（称为**显著度数 / significant degree** 的节点）。此时简化无法继续。

![[cp_topic11_03.png]]

这时需要选一个节点，决定在程序执行时把它放进**内存**而非寄存器，这称为**溢出（spill）**。但是——

!!! quote "我们真的必须立刻溢出它吗？**不一定。**"
	采用一个**乐观近似（optimistic approximation）**：先假设"被溢出的节点**不会**与图中剩余的任何节点干涉"，于是仍然可以把它**移除并压栈**，让简化继续下去。

被选中的节点称为**潜在溢出**（**potential spill**）节点。把它压栈后，简化照常进行。

### Select

当所有节点都压入栈后，**反向弹栈**并逐个赋予颜色：

![[cp_topic11_04.png]]

- 把一个由 **Simplify** 压栈的节点加回图中时，它当初度数 < K，所以**一定有颜色可用**；
- 但一个由 **Spill** 启发式压栈的**潜在溢出**节点 n 被弹出时，**不保证**可着色：
	- 若 n 的邻居恰好已用满 **K 种不同颜色** → 这是一次**真实溢出（actual spill）**：先不给它分配颜色，继续 Select 找出其他真实溢出；
	- 若 n 的邻居用了**少于 K 种**颜色 → 仍可给 n 着色，它**不会**变成真实溢出。

!!! info "这种「先乐观压栈、弹栈时再看运气」的技巧称为**乐观着色（optimistic coloring）**。"

### Start Over

如果 Select 阶段确实有节点找不到颜色（真实溢出），就必须**改写程序**：

- 在每次**使用（use）前**插入 `LOAD`，从内存取回；
- 在每次**定义（def）后**插入 `STORE`，写回内存。

于是一个被溢出的 temporary 会变成若干个**活跃区间极短**的新 temporary。这些新 temporary 可能与图中其它 temporary 干涉，因此要在**改写后的程序上重新运行整个算法**，如此反复，直到 Simplify 成功、不再产生溢出为止。

!!! note "实践中，**一到两次迭代几乎总是足够**。"

### Example

下面用一个完整例子演示 Build/Simplify/Spill/Select（取 **K = 4**）。虚线表示 `MOVE` 指令（不是干涉边）：

![[cp_topic11_05.png]]

Simplify 阶段的初始工作集可以从 **g、h、c、f** 开始——因为它们的邻居都少于 4 个。

一种可行的删除顺序：

1. 移除 **g、h**（及其所有边）→ 此后 **k** 的度数降到 < 4，成为可移除的候选，加入工作表；
2. 依次移除 **k → d → j → e → f → b → c → m**，把它们全部压栈（栈底到栈顶大致为 g, h, k, d, j, e, f, b, c, m）。

随后 **Select** 反向弹栈逐个着色，得到一种合法的 4 着色（节点右上角数字即颜色/寄存器编号）：

![[cp_topic11_06.png]]

整个过程**没有触发溢出**，因为这张图本身就是 4-可着色的。

## Coalescing

回忆 `MOVE` 优化的目标：让 `MOVE` 的源和目的共用一个寄存器，从而删除该 `MOVE`。

!!! success "合并规则"
	若干涉图中 `MOVE` 指令的**源与目的之间没有干涉边**，则可以删除该 `MOVE`，并把源、目的两个节点合并（coalesce）成一个新节点；新节点的边是原两节点边的并集。

但合并并非总是安全的：

![[cp_topic11_07.png]]

引入的新节点比原来两个节点**更受约束**（它继承了双方所有的边）。因此：

!!! warning "一张原本 K-可着色的图，在鲁莽合并（reckless coalescing）之后可能不再 K-可着色。"

所以我们只在安全时才合并——"安全"指合并**不会**把可着色的图变成不可着色。如何判定安全？以下两种保守策略都是安全的：**Briggs** 与 **George**。

- **Briggs 准则**：若合并后得到的节点 ab 的**显著度数（度数 ≥ K）邻居**数量**少于 K**，则 a 和 b 可以合并。
	- **为什么安全？** Simplify 阶段会把所有**非显著度数**（度数 < K）的邻居移除；合并节点最终只与那些**显著度数**邻居相邻。既然显著度数邻居少于 K，Simplify 就能把这个合并节点也移除——因此不会破坏可着色性。
- **George 准则**：若 a 的**每一个**邻居 t 都满足以下之一——t 已经与 b 干涉，或 t 是非显著度数（度数 < K）——则 a 和 b 可以合并。
	- **为什么安全？** 逐个分析 a 的邻居 t：
		- 若 t 已与 b 干涉：合并后 $(a,t)$ 和 $(b,t)$ 并成同一条 $(ab,t)$，**不增加** ab 的度数；
		- 若 t 是非显著度数：t 会被 Simplify 移除，同样**不会**导致度数上升。

### Coloring with Coalescing

把合并融入着色流程后，算法的相图（phase diagram）如下（教材 **Figure 11.4**）：

![[cp_topic11_08.png]]

> **Coalesce、Simplify、Spill 要交替进行，直到图为空。**

完整的六个阶段：

1. **Build**：构造干涉图；并把每个节点标记为 **move-related** 或 **non-move-related**。move-related 节点指它是某条 `MOVE` 指令的源或目的。
2. **Simplify**：每次移除一个**度数低（< K）且 non-move-related** 的节点。
3. **Coalesce**：在简化后的图上做**保守合并**（Briggs / George）。合并产生的新节点不再是 move-related，可供下一轮简化使用。Simplify 与 Coalesce 反复交替，直到只剩**显著度数节点或 move-related 节点**。
4. **Freeze**：若 Simplify 和 Coalesce 都无法进行，就找一个**度数低的 move-related 节点**，冻结（freeze）与它相关的 `MOVE`——即**放弃合并这些 MOVE**，把这些节点当作 non-move-related，然后恢复 Simplify 和 Coalesce。
5. **Spill**：若没有低度数节点，选一个显著度数节点作为**潜在溢出**压栈。
6. **Select**：弹出整个栈并赋色；若出现**真实溢出**，则重建图（rewrite 后重来）。

### Example

仍用前面的图（K = 4），其中 **b、c、d、j** 是仅有的 move-related 节点。Simplify 的初始工作表只能含 non-move-related 节点。先移除 **g、h、k**，得到下图左侧的简化结果。

此时调用一轮 Coalesce，发现 **c 和 d 可以合并**（Briggs 和 George 两个准则都满足）：

![[cp_topic11_09.png]]

接着还可以合并 **b 和 j**；之后**没有更多 move-related 节点**，合并结束：

![[cp_topic11_18.png]]

最后再调用一次 Simplify 移除剩余节点，并在 Select 中赋色，得到一种可行分配（注意 `j&b`、`c&d` 各自共用一个颜色，对应的两条 `MOVE` 都被删除）：

![[cp_topic11_19.png]]

## Precolored Nodes

有些寄存器具有**特殊用途**：参数寄存器（argument registers）、帧指针（frame pointer, FP）、返回值寄存器（return-value register）等。对每个这样的寄存器，我们使用一个**永久绑定到它**的特殊 temporary（例如代表 FP 的 temporary）。这类节点称为**预着色节点（precolored nodes）**。

!!! note "预着色节点的性质"
	- 每种颜色**只有一个**预着色节点；
	- 所有预着色节点**两两相互干涉**（它们是不同的物理寄存器）；
	- 一个普通 temporary**可以**与某个预着色寄存器同色，**只要二者不干涉**——这正是"标准调用约定寄存器可以在过程内部被当作临时变量复用"的依据；
	- **不能 simplify 预着色节点**（机器寄存器数目固定，谈不上"K 色是否够用"）；
	- **不能把预着色节点 spill 到内存**（机器寄存器本来就是寄存器）。

### Temporary Copies of Machine Registers

着色算法不断调用 simplify / coalesce / spill，**直到只剩下预着色节点**。由于预着色节点不能溢出，前端必须**让它们的活跃区间尽量短**——办法是生成 `MOVE` 指令，把值搬入/搬出预着色节点。

例如设 `r7` 是一个 **callee-save**（被调用者保存）寄存器，在函数入口处定义、出口处使用。前端在入口把它复制到一个新 temporary `t231`，在出口再复制回来：

```text
  原始：                 改写为：
Enter: def(r7)         Enter: def(r7)
                              t231 ← r7
         ⁞                      ⁞
                              r7 ← t231
Exit:  use(r7)         Exit:  use(r7)
```

!!! quote "若函数内寄存器压力很大，`t231` 会被溢出；否则 `t231` 会与 `r7` 合并、两条 `MOVE` 都被删除"

### Caller-Save and Callee-Save Registers

- **不跨任何过程调用**活跃的局部变量/临时变量，通常应分配到 **caller-save**（调用者保存）寄存器；
- **跨越多个过程调用**仍活跃的变量，应保存在 **callee-save**（被调用者保存）寄存器中。

考虑下面的代码：`t` 不跨调用，`s` 跨越 `f()`、`g()` 两次调用。

```c
foo() {
    t = ...
    ... = ... t ...
    s = ...
    f()
    g()
    ... = ... s ...
}
```

若变量 x **跨过程调用活跃**，则：

- 它与**所有 caller-save（预着色）寄存器**干涉；
- 它也与**为 callee-save 寄存器新建的所有 temporary**（如 `t231`）干涉；

于是**必然发生一次溢出**。按常用启发式（溢出"高度数但很少被使用"的节点），到底先溢出谁？是 **x** 还是 **t231**？——若溢出 t231，等价于把 callee-save 寄存器的内容存一次、取一次（一对 load/store）；这往往比反复在每个调用点保存 x 更划算。这正是 callee-save 机制省下开销的方式。

### Example with Precolored Nodes (K = 3)

下面是一个含预着色节点的完整例子。源程序与其汇编（机器有 **3** 个寄存器：`r1`、`r2` 为 caller-save，`r3` 为 callee-save，K = 3）：

```c
int f(int a, int b) {
    int d = 0;
    int e = a;
    do { d = d + b;
         e = e - 1;
    } while (e > 0);
    return d;
}
```

```asm
enter: c ← r3            // 把 callee-save r3 存入 temporary c
       a ← r1
       b ← r2
       d ← 0
       e ← a
loop:  d ← d + b
       e ← e - 1
       if (e > 0) goto loop
       r1 ← d            // 返回值
       r3 ← c            // 恢复 callee-save
       return            // (r1, r3 live out)
```

其干涉图如下（实线为干涉边，虚线为 `MOVE`；教材中漏画了 `c–r1` 这条边）：

![[cp_topic11_20.png]]

**起手没有任何机会**做 simplify、coalesce 或 freeze：

- **Simplify**：所有非预着色节点度数都 ≥ K；
- **Coalesce**：任何合并都会得到一个与 ≥ K 个显著度数节点相邻的节点（不保守）；
- **Freeze**：即使把 a、b、c、d、e 视为 non-move-related，它们度数仍 ≥ K。

所以**必须先溢出某个节点**。选谁？——选**度数高但很少被使用**的节点。用如下**溢出代价（spill cost）启发式**：

$$
\text{spill priority} = \frac{(\text{循环外 uses+defs}) + 10\times(\text{循环内 uses+defs})}{\text{degree}}
$$

> 循环内的访问乘以系数 10（粗略地估计循环会执行约 10 次），优先级**越低越该被溢出**。

| Node | 循环外 uses+defs | 循环内 uses+defs | degree | spill priority |
|:---:|:---:|:---:|:---:|:---:|
| a | 2 | 0  | 4 | (2 + 10×0)/4 = **0.50** |
| b | 1 | 1  | 4 | (1 + 10×1)/4 = **2.75** |
| **c** | 2 | 0  | 6 | (2 + 10×0)/6 = **0.33** ← 最小 |
| d | 2 | 2  | 4 | (2 + 10×2)/4 = **5.50** |
| e | 1 | 3  | 3 | (1 + 10×3)/3 = **10.33** |

**c** 的优先级最低（0.33，度数高达 6 却完全不在循环内使用），因此**首先溢出 c**。把 c 作为潜在溢出压栈后，继续做合并：

![[cp_topic11_21.png]]

- **2. Coalesce a & e**（满足保守准则）；
- **3. Coalesce b & r2**（普通节点与预着色寄存器同色）；

接着尝试合并 `r1ae` 和 `d`：

![[cp_topic11_22.png]]

!!! warning "**`r1ae` 与 `d` 干涉**，因此 `r1ae` 与 `d` 之间的 `MOVE` 是**受约束的（constrained）**。"
	我们把它从后续考虑中移除（**freeze**）；`d` 不再被当作 move-related，于是可以被 simplify。

**5. Simplify d** 之后图中只剩预着色节点。进入 **Select**，反向弹栈赋色：

![[cp_topic11_23.png]]

- a、b、e 已在合并时获得颜色；
- 弹出 **d**，赋色 **r3**；
- 弹出 **c**：c 变成**真实溢出（actual spill）**。

!!! info "**为什么 c 是真实溢出？** c 与 r1、r2、r3 以及若干普通节点干涉，它的邻居已经用满了全部 3 种颜色（K = 3），无色可用，故只能真实溢出。"

**7. Rewrite**：为真实溢出的 c 插入访存指令——def 后 `STORE`、use 前 `LOAD`，c 拆成两个短命 temporary `c1`、`c2`：

```text
enter: c1 ← r3
       M[cloc] ← c1     // store after def
       a ← r1
       b ← r2
       d ← 0
       e ← a
loop:  d ← d + b
       e ← e - 1
       if (e > 0) goto loop
       r1 ← d
       c2 ← M[cloc]     // load before use
       r3 ← c2
       return
```

**8–11. Build 新图并重新合并**：在改写后的程序上重建干涉图，c 的活跃区间已变得极短，可以顺利合并：先合并 `c1&r3` 再 `c2&r3`，然后 `a&e`、`b&r2`，最后 `ae&r1` 并 simplify d。

![[cp_topic11_16.png]]

**12. Select**：弹栈时只需给 d 选 **r3**，其余节点都已通过合并或预着色确定颜色：

![[cp_topic11_17.png]]

最终着色：

| Node | Color |
|:---:|:---:|
| a | r1 |
| b | r2 |
| c1 & c2 | r3 |
| d | r3 |
| e | r1 |

**13. Rewrite + 14. 删除自反 MOVE**：用上面的寄存器分配改写程序后，删除所有"源 = 目的"的 `MOVE`（如 `r1 ← r1`、`r3 ← r3`），得到最终代码：

```text
enter: M[cloc] ← r3
       r3 ← 0
loop:  r3 ← r3 + r2
       r1 ← r1 - 1
       if (r1 > 0) goto loop
       r1 ← r3
       r3 ← M[cloc]
       return
```

只用 3 个寄存器就完成了分配，且仅引入了一对 store/load（callee-save 寄存器 `r3` 在入口存、出口取）。

!!! note "George 准则与预着色节点的一个微妙之处"
	在本例中，`r3` 的所有显著度数邻居似乎都已与 `c` 干涉，那为何不能按 George 准则把 `r3` 和 `c` 合并掉？
	**约定**：当 George 准则应用于"含一个预着色节点"的节点对时，**总是在那个非预着色的节点上**测试规则（这里只在 `c` 上测 George）。
	**原因**：如果允许在预着色节点 `r3` 上测 George 并据此合并 `r3` 与 `c`，就等于把一个本应无法着色的节点 `c`（它有多条干涉关系）**强行**染成 `r3`，从而**丧失了将它溢出的机会**，反而可能得到错误或更差的分配。

