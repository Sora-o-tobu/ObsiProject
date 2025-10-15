
# Simple Model

## Regular Language & Finite Automata

### Deterministic Finite Automata

有限状态机拥有一定数量的状态，状态之间通过一些特定条件进行转移。用图像表示，State Diagram 的一些表示符号如下：

![[t2_1.png]]


一个 **Deterministic Finite Automata（DFA）** 由五个参数组成：

- $K$: finite set of states
- $\Sigma$: alphabet
- $s\in K$: initial state
- $F\subseteq K$: set of final states
- $\delta$: transition function
	- $\delta: K\times \Sigma \rightarrow K$

!!! note "Final States"
	DFA 输入被接受的状态，若处理完一串输入后停留在 Final State，则该输入被接受；反之则被拒绝。例如，对于如下状态机，输入 `aabba` 会被 *accepted*：
	
	![[t2_2.png]]

!!! example "设计一个 DFA，要求实现 Language $L_1 =\{w\in \{a,b\}^* : w\text{ 没有三个连续的 b}\}$"
	![[t2_3.png]]
	
	??? question "如果我们想要 $L_2=\{w\in\{a,b\}^* : w\text{ 包含三个连续的 b}\}$ 怎么办?"
		由于 $L_2 = \Sigma^* - L_1$，我们只需要将 Final States $F=\{q_0, q_1, q_2\}$ 变为 $F=\{q_3\}$ 即可。

**Configuration** 是 $K\times \Sigma^*$ 的子集，代表了一个 DFA 当前的状态，表示为 $(q,w)$，其中：

- $q\in K$ 为当前状态（current state）
- $w\in \Sigma^*$ 为尚未读取的剩余输入串

我们使用符号 $\vdash_M$ 来表示 DFA 在配置之间的转移：

$$(q, w) \;\vdash_M\; (q', w')$$

此时一定有条件：

- 存在某一 Symbol $a\in \Sigma$
- $w=aw'$
- $\delta (q,a) = q'$

$\vdash_M^*$ 是运算 $\vdash_M$ 的 **reflexive, transitive** 的 Closure，那么 $(q, w) \;\vdash_M^*\; (q', w')$ 说明配置 $(q,w)$ 可以在有限步转移到 $(q', w')$，并且我们此时称 $(q,w)$ **yields** $(q',w')$。

!!! note "这里的有限步也可以是 0，因此也有 $(q,w)\; \vdash_M^* \; (q,w)$"

此时，我们可以将上面对于 Final States 描述改为：

A string $w\in \Sigma^*$ is said to be *accepted* by M **iff** there is a state $q\in F$ such that $(s,w)\; \vdash_M^*\; (q,\upepsilon)$。

所有被 DFA $M$ 接受的 string 集合表示为 $L(M)$，它是被 $M$ 接受的 Language。

### Nondeterministic Finite Automata

NFA 不同于 DFA，它在某些状态下读到一个符号时，可能有多个去向，甚至可以不读入输入就跳转。这种不确定性让它同时尝试多条路径，只要有一条成功就接受输入。

NFA 同样是由五个参数组成，不同的是，它的转换方式由 function 变为了 relation：

- $K$: finite set of states
- $\Sigma$: alphabet
- $s\in K$: initial state
- $F\subseteq K$: set of final states
- $\Delta$: transition relation
	- $\Delta \subseteq K\times (\Sigma \cup \{e\}) \times K$

以下为应用 NFA 的三个例子：

![[t2_4.png]]

两个自动机 $M_1$,$M_2$ 相等，当且仅当 $L(M_1)=L (M_2)$。我们希望证明 DFAs 和 NFAs 在能力上等价时，必须证明以下两件事：

- For each DFA, produce an NFA that accepts the same language
- For each NFA, produce an DFA that accepts the same language

其中第一点是显而易见的，对于第二点要求，给定一个 NFA，可以构造一个等价 DFA，这意味着我们需要“填补” DFA 和 NFA 之间的差异。

- **Difference 1:** Transition Relation
	- Transition relation of DFA 一定是一个 function: $K\times \Sigma \times K$
	- Transition relation of NFA 不一定是 function: $K\times (\Sigma \cup \{e\}) \times K$
- **Difference 2:** Domain
	- The domain of transition relation of DFA is $K\times \Sigma$
	- The domain of transition relation of NFA is $K\times (\Sigma \cup \{e\})$
		- 即 NFA 容忍无字符输入下的状态转移

**【Example】**

下面我们以一个 NFA 为例：

$$\begin{array}l
K=\{q0, q1,q2\}, \Sigma =\{a,b\}, s=q0,\ F=\{q2\}, \\
\Delta = \{(q0, a,q1), (q1,b,q0), (q1,b,q2), (q2,a,q0)\}
\end{array}
$$

绘制成 State Diagram 的结果如下：

![[t2_5.png]]

为了解决以上两点 Difference，我们可以从初始状态开始思考，计算转换后的 DFA 有什么状态。我自己思考总结得到的计算方法如下：

![[t2_7.png]]

!!! danger "zq 老师的 PPT 上的方法我没太理解，且绿色部分 PPT 上没有"
	按照定义，一个 DFA 要求**每个状态对于每个输入符号都必须有且只有一个转移**，否则意味着自动机在这个输入上是不确定的。

现在我们回顾一下 Language 中的闭包 EClose $E(q)$：

$$
E(q)=\{p\in K: (q,e)\; \vdash_M^*\; (p,e)\}
$$

在状态机模型下，$E(q)$ 即表示状态机 $M$ 从状态 $q$ 开始，在无输入的情况下能达到的所有状态的集合。

除此之外，还有一个结论是从 NFA 转换到 DFA，状态的个数可以达到指数级别的增长，例如：

$$
|K| = 5\;\Rightarrow\; |K'|=2^5 = 32
$$

这一点在我们上面画的树中也有所体现。但是实际上大部分状态都与原先的状态机无关，因此指数级只是一个上限。

**【More Example】**

把如下 NFA 转换成 DFA：

![[t2_8.png]]

转换步骤仍然如上，注意最后的 Dead State 也要画出来：

![[t2_9.png]]

### Regular Language

> 以下内容大部分来自 [语言、自动机与正则表达式 - 鹤翔万里的笔记本](https://note.tonycrane.cc/cs/tcs/toc/topic1/#_5)

一个 Language 被称为 **regular**，当且仅当它被一个自动机 $M$ 接收。我们有定理：

- 如果 $A$ 和 $B$ 是正则语言，则 $A\cup B$, $A\circ B$, $A^*$ 都是正则语言。

我们一般使用 $R$ 表示**正则表达式**，使用 $L(R)$ 表示该正则表达式对应的语言。一个正则表达式由如下规则定义：

- **Atomic**：对于 $\emptyset$ 对应语言 $L(\emptyset)=\emptyset$，对于 $a\in \Sigma$ 有 $L(a)=\{a\}$
- **Composite**：
    - $R_1​ \cup R_2$​ 对应语言 $L(R_1 \cup R_2​)= L(R_1​) \cup L(R_2​)$
    - $R_1 R_2$​ 对应语言 $L(R_1 ​R_2​)= L(R_1​)\circ L(R_2​)$
    - $R_1^∗$​ 对应语言 $L(R_1^∗​)= L(R_1​)^∗$
    - 优先级：$^*\gt \circ\gt\cup$
        - Ex. $ab^∗ \cup b^∗a= (a(b^∗)) \cup ((b^∗)a)$

!!! example "$R$ 和 $L(R)$ 的对应关系"
	- $\emptyset ^*$ => $\{e\}$
	- $a(a\cup b)^* b$ => $\{w\in \{a,b\}^*\; :\; w \text{ starts with } a \text{ and ends with } b\}$
	- $(a\cup b)^* a (a\cup b)^* a (a \cup b)^*$ => $\{w\in \{a,b\}^*\; : \; w \text{ contains at least two a's}\}$

事实上，自动机和正则表达式之间是等价的，可以相互转换。要将一个 FA 转换为正则表达式，我们首先需要掌握如何通过归纳来折叠一个状态，如下是我们所使用的基本操作：

![[t2_10.png]]

给定一个 NFA $M$，要化简为正则表达式 $R$，我们考虑为其手动新增一个初始状态和接受状态，用 e-transition 连接到原来的初始状态和接受状态，再逐一删除中间状态：

![[t2_11.png]]

**【Example】** 假定有自动机 $M$ 如下：

![[t2_12.png]]

按照上面的逻辑，转换步骤如下：

![[t2_13.png]]

那么最终 $R=a^* b \left( a\cup b a^* b a^* b \right)^*$

## Context-free Languages & Pushdown Automata

