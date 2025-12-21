
# Simple Model

## Regular Language & Finite Automata

### Deterministic Finite Automata

有限状态机拥有一定数量的状态，状态之间通过一些特定条件进行转移。用图像表示，State Diagram 的一些表示符号如下：

![[t2_1.png]]


一个 **Deterministic Finite Automata (DFA)** 由五个参数 $(K,\Sigma , \delta, s, F)$ 组成：

- $K$: 有限状态集合
- $\Sigma$: 字母表
- $s\in K$: 初始状态
- $F\subseteq K$: 最终状态集合（set of final states）
- $\delta$: 转移方程
	- $\delta: K\times \Sigma \rightarrow K$

!!! note "Final States"
	DFA 输入被接受的状态，若处理完一串输入后停留在 Final State，则该输入被接受；反之则被拒绝。例如，对于如下状态机，输入 `aabba` 会被 ACCEPT：
	
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

此时一定存在某一 Symbol $a\in \Sigma$ 在配置转移过程中被读取，并且有：

- $w=aw'$
- $\delta (q,a) = q'$

$\vdash_M^*$ 是运算 $\vdash_M$ 关于 **reflexive, transitive** 的 Closure，那么 $(q, w) \;\vdash_M^*\; (q', w')$ 说明配置 $(q,w)$ 可以在有限步转移到 $(q', w')$，并且我们此时称 $(q,w)$ **yields** $(q',w')$。

!!! note "这里的有限步也可以是 0，因此也有 $(q,w)\; \vdash_M^* \; (q,w)$"

此时，我们可以将上面对于 Final States 描述改为：

A string $w\in \Sigma^*$ is said to be *accepted* by M **iff** there is a state $q\in F$ such that $(s,w)\; \vdash_M^*\; (q,e)$。

所有被 DFA $M$ 接受的 string 集合表示为 $L(M)$，它是被 $M$ 接受的 Language。

??? success "DFA 最小化问题"
	- <1> 删除所有不可达状态
	- <2> 合并等价状态
		- <2.1> 初始状态分为*接受组*和*非接受组*，因为接收状态和非接受状态不等价
		- <2.2> 考察接收到某一输入符号后，某组内的不同状态是否会转移到不同等价组
			- 如果不等价，则将它们分开建为新组
		- <2.3> 重复 2.2，直到无法细分
	
	??? example
		![[t2_19.png]]
		
		根据规则：“接受状态与非接受状态不等价”。我们将所有状态分成两组：
		
		- **接受状态组 (Final)**： $\{q_5\}$ (双圈状态)
		- **非接受状态组 (Non-Final)**： $\{q_0, q_1, q_2, q_3, q_4\}$
		
		检查大组 $\{q_0, q_1, q_2, q_3, q_4\}$ 内部是否还能拆分，例如考察输入 **1**：
		
		- **$q_0$** 输入 1 $\to$ $q_2$ (在非接受组)
		- **$q_1$** 输入 1 $\to$ $q_3$ (在非接受组)
		- **$q_2$** 输入 1 $\to$ $q_5$ (**在接受组**)
		- **$q_3$** 输入 1 $\to$ $q_5$ (**在接受组**)
		- **$q_4$** 输入 1 $\to$ $q_5$ (**在接受组**)
		
		$q_0, q_1$ 在输入 1 后仍然留在非接受组，而 $q_2, q_3, q_4$ 在输入 1 后跳到了接受组 ($q_5$)。因此，$\{q_0, q_1\}$ 和 $\{q_2, q_3, q_4\}$ 必须分开。现在的划分变成了三组：$P_1 = \{ \{q_0, q_1\}, \{q_2, q_3, q_4\}, \{q_5\} \}$
		
		后续细分步骤以此类推

### Nondeterministic Finite Automata

NFA 不同于 DFA，它在某些状态下读到一个符号时，可能有多个去向，甚至可以不读入输入就跳转。这种不确定性让它同时尝试多条路径，只要有一条成功就接受输入。

NFA 同样是由五个参数组成，不同的是，它的转换方式由 *function* 变为了 *relation*：

- $K$: finite set of states
- $\Sigma$: alphabet
- $s\in K$: initial state
- $F\subseteq K$: set of final states
- $\Delta$: transition relation
	- $\Delta \subseteq K\times (\Sigma \cup \{e\}) \times K$

??? example "应用 NFA 的三个例子"
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
??? example "More Example"
	=== "NFA"
		![[t2_8.png]]
	=== "DFA"
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

**【More Example】**

已知 $\Sigma=\{0,1,...,9\}$，证明 $L=\{w: w\in \Sigma^*\text{ be the nonnegative integers divisible by 2 and 3}\}$ 是正则语言。

正则语言之间有三种运算，两个正则语言通过这三种运算的结果仍然是正则语言，因此我们可以考虑将 $L$ 分解为可以被 2 整除的 $L_2$ 以及可以被 3 整除的 $L_3$ 分别证明是正则语言。

首先对于 $L_2$，它的特征是个位为偶数，因此表示如下，它也是通过几个正则语言运算得到的表达式：

$$
L_2=\Sigma^* \cap \Sigma^* \{0,2,4,8\}
$$

对于 $L_3$，我们知道它的所有位数字加起来要是 3 的倍数，我们可以为它绘制一个 NFA：

![[t2_14.png]]

因此 $L_2$ 和 $L_3$ 都是正则的，而 $L=L_2 \cap L_3$ 自然也是正则的。

更多判断 Language 是否是正则的例子如下：

- $L=a^*$ 是正则的，因为它被 Regular Expression 定义
- $L=\{a^n : n\ge 0\}$ 是正则的，因为它和上面语言等价
- $L=\{w^R: w\in L_0, \text{ and}\ L_0\text{ is regular}\}$ 是正则的
	- 这里 $^R$ 表示反转
- $L=\{a^n: n\in Even\}$ 是正则的，因为它等价于 $L=(aa)^*$
- $L=\{w\in \{a,b\}^*: w\text{ has an equal number of a's and b's}\}$ 不是正则的
	- 假设 $L$ 是正则的，那么 $L\cap a^* b^*=\{ a^n b^n: n \ge 0\}$ 就是正则的，但是 FA 无法记忆前面有多少个 a 以确保后面出现相同数量的 b，因此矛盾
- $L=\{a^n: n\in Prime\}$ 不是正则的
	- 注意这是质数，不是奇数
- $L=\{ww^R : w\in \{a,b\}^*\}$ 不是正则的
- $L=\{ww: w\in \{a,b\}^*\}$ 不是正则的
- $L=\{wcw: w\in \{a,b\}^*\}$ 不是正则的

对于任意一个非空 alphabet $\Sigma$，它的 String $\Sigma^*$ 是可数无限集（Countable Infinite），而所有 Language 就是 $\Sigma^*$ 的所有子集，其总数满足：

$$
|P(\Sigma^*)| \;= \; 2^{|\Sigma^*|}
$$

这意味着 Language 的总数是不可数的。

但是 Regular Language 是可数的，因为每个正则语言都可以被一个有限自动机描述，而有限自动机的集合是可数的。

### Pumping Theorem

若 $L$ 是一个 regular language，则存在一个常数 pump length $p\in \mathbb{Z}^*$ 使得 $\forall w\in L$ with $|w|\ge p$ 可被分为 3 个部分 $w=xyz$，满足：

- $\forall i \ge 0, xy^iz\in L$
- $|y| > 0$
	- 被 pump 的字段非空
- $|xy| \le p$
	- $y$ 出现在 String 的前 $p$ 个字符内

直观上看，在 DFA 接受一个长度至少为 $p$ 的字符串时，开始读入前 $p$ 个字符必然经过某一个状态两次（鸽巢原理），于是在这两个相同状态之间的读入片段 $y$ 就是一个循环（loop），所以可以重复（或删去一次，相当于 $i=0$），而该片段仍保持被 DFA 接受——这就是“泵”的含义。

!!! note "通常我们取 $p$ 为某个 DFA 的状态数"

泵定理是 Regular Language 的一个必要**不**充分条件，因此我们可以通过证明泵定理不成立来证明该语言不是一个 Regular Language。


1. 假设 $L$ 是正则的
2. 那么存在泵长度 $p$
3. 选择一个特定的字符串 $w\in L$ 且 $|w|\ge p$。你的 $w$ 可以依赖于 $p$（通常取与 $p$ 相关的形式，如 $a^p b^p$）
4. 根据泵引理，我们知道任意对满足 $|xy|\le p, |y|>0$ 的划分 $w=xyz$，都应当满足 $\forall i\ge0,\ xy^iz\in L$
5. 对任意可能的 $y$（满足上面的约束）构造一个 $i$（通常 $i=0$ 或 $i=2$），使得 $xy^iz\notin L$。这就与泵引理结论矛盾，从而 $L$ 非正则。

**注意**：第 5 步要覆盖“任意可能的 $y$”——也就是说你要分析所有满足 $|xy|\le p, |y|>0$ 的情况并在每种情况下给出反例（一般通过观察 $y$ 的组成，例如 $y$ 只能由若干个 `a` 组成，从而改变 `a` 的数量会破坏语言性质）。


**【Example】**

证明 $L=\{a^n b^n \mid n\ge0\}$ 非正则

1. 假设 $L$ 正则，设对应的泵长度为 $p$。
2. 取 $w = a^p b^p \in L$。显然 $|w| = 2p \ge p$。
3. 任意划分 $w=xyz$ 满足 $|xy|\le p, |y|>0$。因为 $|xy|\le p$，$xy$ 完全位于第一个 $a^p$ 中，所以 $y$ 只含 `a`（且至少一个 `a`）。
4. 令 $i=0$。则 $xy^0z = xz$ 则比原来少了一些 `a`（而 `b` 个数不变），所以形状变成 $a^{p-|y|} b^p$，不属于 $L$（因为 `a` 与 `b` 的数量不同）。
5. 与泵引理结论矛盾，故 $L$ 非正则。

!!! example
    常见的 non-regular languages 和简要证明思路 ($p$ 是 pump length):
	
    - $L = \{0^n1^n\}$: 选 $0^p1^p$，则 $xy^0z \notin L$ 
    - $L = \{ww\}$, $L = \{ww^R\}$: 选 $ab^pab^p$ 和 $b^paab^p$，则 $xy^0z \notin L$ 
    - $L = \{0^m1^n\}$ where $m > n$: 选 $0^{p+1}1^p$，则 $xy^0z \notin L$ ($m\ge n$ 也一样)
    - $L = \{0^m1^n\}$ where $m < n$: 选 $0^p1^{p+1}$，则 $xyyz \notin L$ ($m\le n$ 也一样)
        - 根据上面两个例子可以看出，union of 2 non-regular languages 不一定是 non-regular 的，例如 $m > n$ 和 $m \le n$ 的 union 是 $0^*1^*$
    - $L = \{0^m1^n\}$ where $m \neq n$: 假设 regular，则 $(\{0^*1^*\} - L) \cap \{0^*1^*\} = \{0^n1^n\}$ is regular，矛盾
    - $L = \{1^n\}$ where $n$ is prime: 选 $1^k$ where $k > p$ and $k$ is prime，若 $y = 1^s$ where $0 < s \le p$，则 $\forall n \ge 0, k + (n - 1)s$ is prime。但取 $n = k + 1$ 得到 $k + ks = k(1 + s)$ is not prime，矛盾
    - $L \in \{0, 1\}^*$ where numbers of 0's and 1's are equal: 假设 regular，则 $L \cup 0^*1^* = \{0^n1^n\}$ is regular，矛盾

!!! quote "不存在判断任意语言是否为 Regular 的算法"
	- 证明语言**正则**
	    - 该语言有限
	    - 构造一个接受该语言的 FA
	    - 构造一个生成该语言的正则表达式
	    - 正则语言的闭包性质
	- 证明语言**非正则**
	    - Pumping Theorem
	    - 反证
	        - 不存在接受该语言的 FA
	        - 正则语言的闭包性质


## Context-free Languages & Pushdown Automata

### Context-free Language

在形式化定义上，一个 **Context-Free Grammar, CFG** 由四个部分组成 $G=(V, \Sigma, R, S)$：

- $V$：字母表
- $\Sigma \subseteq V$: 终结符集合（The set of terminal symbols）
	- $V-\Sigma$: 非终结符集合（the set of non-terminals）
	- 非终结符一定是非空集合，因为起码包含 $S$
	- 而终结符可以为空集 $\emptyset$
- $S\in V-\Sigma$: 开始符号
- $R$: 规则集合
	- $R\subseteq (V-\Sigma) \times V^*$
	- 是*可数*集合，形式为 $A\rightarrow \alpha$，其中 $A\in V-\Sigma,\; \alpha \in V^*$

CFG 的**上下文无关**体现在 $R$ 中的 rules 左侧只能是单个**非终结符**，即与周围的终结符无关。

!!! tip "终结符的意义在于，在根据定义的规则推导过程中，它们不能被替换；当推导到只剩下终结符时，表示生成完成"

对应的，一个语言 $L$ 是 **Context-Free Language（CFL）** 当且仅当存在一个 CFG $G$ 使得 $L=L(G)$，即可以被 CFG 生成。

??? question "判断：Language $L$ is CFL iff $L=L(G)$"
	这个表述是错的，还要说明 $G$ 是一个 CFG 😂

??? question "判断：Language $L$ is CFL iff it is accepted by a CFG"
	这个表述也是错的，正确表述为：Language $L$ is CFL iff it is **generated** by a CFG

!!! note "Regular Language 是 CFL 的子集，即 All Regular Languages are CFL"

| 主题                       | 正规语言（Regular Language）    | 上下文无关语言（Context-Free Language, CFL） |
| ------------------------ | ------------------------- | ----------------------------------- |
| 生成工具（Generation Device）  | Regular Expression（正规表达式） | Context-Free Grammar（上下文无关文法）       |
| 识别工具（Recognition Device） | Finite Automata（有限自动机，FA） | Pushdown Automata（下推自动机，PDA）        |
| 等价关系（Equivalence）        | FA ⇔ Regular Expression   | PDA ⇔ Context-Free Grammar          |

**【Example】**

$$
L=\{a^n b^n\; |\; n \ge 0\}
$$

- 终结符为 $\{a,b\}$
- 开始符为 $S$
- 产生式集合为 $\{S\rightarrow aSb, S\rightarrow e\}$
	- 或者表示为 $\{(S,aSb),\; (S,e)\}$ 或 $\{S\rightarrow aSb\; |\; e\}$

!!! info "这里的开始符 $S$ 只是生成规则中的“变量”，最终要在推导生成中被替换为结束符"

**【More Example】**

$$
L=\{w\in \{a,b\}^* \;:\; w=w^R\}
$$

即证明回文字符串是上下文无关的，我们构造 CFG：

- $S\rightarrow e,\; S\rightarrow a,\; S\rightarrow b$
- $S\rightarrow aSa,\; S\rightarrow bSb$
	- 可以简写为 $S\rightarrow e \;|\; a \;|\; b \;|\; aSa \;|\; bSb$

同 Regular Language 一样，CFL 也关于操作 Union, Concatenation, Kleene Star 闭包，但对 Intersection 和 Complementation 不闭包：

| 运算             | 正则语言 | 上下文无关语言 |
| -------------- | ---- | ------- |
| 并 ($\cup$)     | ✅    | ✅       |
| 交 ($\cap$)     | ✅    | ❌       |
| 连接 ($\circ$)   | ✅    | ✅       |
| 补 ($\bar{A}$)  | ✅    | ❌       |
| Kleene 星 ($*$) | ✅    | ✅       |

!!! warning "但是 CFL 和正则语言的交运算是封闭的，即 $\text{CFL} \cap \text{Regular}=\text{CFL}$，常用来证明语言不是 CFL"



### Parse Tree

CFG 的推导可以表示为一个 **Parse Tree**，该树的叶节点均为 *terminal symbols*，根节点为 *start symbol*，其余节点为 alphabet $V$ 中的元素。我们将叶节点从左到右拼接成字符串，即可得到该 Parse Tree 对应的产出（yield）。

例如，对于 $a^nb ^n$，可以绘制如下 parse tree 表示 $a^4 b^4 \in a^n b^n$：

![[t2_15.png]]

我们称两个推导满足 $D$ precedes $D'$:

- **前提场景**：$x_{k-1}= x_{k-1}'= uAvBw$
    - 这表示在第 $k-1$ 步时，当前的句型中同时包含两个非终结符 $A$ 和 $B$。
    - $u, v, w$ 是上下文（由终结符或非终结符组成的串）。
    - 注意：$A$ 在 $B$ 的左边。
- **推导 $D$（先左后右）**：
    - **第 $k$ 步 ($x_k = uyvBw$)**：先对左边的 $A$ 应用规则 $A \rightarrow y$。此时 $B$ 保持不变。
    - **第 $k+1$ 步 ($x_{k+1} = uyvzw$)**：再对 $B$ 应用规则 $B \rightarrow z$。
- **推导 $D'$（先右后左）**：
    - **第 $k$ 步 ($x'_k = uAvzw$)**：先对右边的 $B$ 应用规则 $B \rightarrow z$。此时 $A$ 保持不变。
    - **第 $k+1$ 步 ($x'_{k+1} = uyvzw$)**：再对 $A$ 应用规则 $A \rightarrow y$。
- **结论**：
    - $x_{k+1} = x'_{k+1}$：无论先做哪一步，两步之后的结果是一模一样的。
    - $\forall i \neq k, x_i = x'_i$：除了第 $k$ 步的中间状态不同，推导的其他所有步骤完全一致。

在这里，由于推导 $D$ 有限处理了左边的非终结符 $A$，按照标准“最左推导”原则，称 $D$ 先于 $D'$。

!!! info "Precede 记为 ≺"

在上下文无关文法（CFG）中，推导是一个**线性**的时间过程（一步接一步），而分析树是一个**空间**的层级结构。

- **现象**：当一个句型中同时存在两个非终结符（例如 $A$ 和 $B$）时，你可以选择先推导 $A$，也可以选择先推导 $B$。
- **本质**：只要 $A$ 和 $B$ 是“并列”的（即互不为祖先），谁先谁后并不影响最终生成的树形结构。
- **目的**：这段定义就是为了把这种“仅仅是顺序不同，但结构相同”的推导归为一类，称为 **Similar（相似）**。

那么对于两个推导 $(D,D')$，如果它们对运算 precedes 关于自反、对称、传递闭包时，我们称推导 $D,D'$ 是 Similar 的。

假如一个 Grammer $G$ 存在一些可以由不同的 Parse Tree 推导的 String，则我们称 $G$ 是 **ambiguous** 的。

### Pushdown Automata

**下推自动机（Pushdown Automata, PDA）**是一个带栈结构的 NFA，它可以使用栈做一些记忆功能。

在形式化定义上，PDA 由六个部分组成 $P=(K,\Sigma, \Gamma, \Delta, s, F)$：

- $\Gamma$: stack alphabet
	- 栈中能出现的符号集合
- $\Delta$: transition relation
	- $\Delta \subseteq \left(K\times (\Sigma \cup \{e\})\times \Gamma^* \right)\times \left(K\times \Gamma^* \right)$
	- 相对于 NFA 的 $\left(K\times (\Sigma \cup \{e\}) \right)\times K$，在转移前后都增加了栈相关字符串
	- 前一个 $\Gamma^*$ 是栈顶字符串，匹配后 pop 出来
	- 后一个 $\Gamma^*$ 是转移后要 push 进去的字符串

最后判断能否接受一个 String $w$ 要求不仅状态位于 Final State，还要 Stack 为空。用 $\vdash$ 表示，则记为：

$$
(s,w,e)  \vdash_M^* (p,e,e),\; \text{for some state }p\in F
$$

相应的，能被 PDA $M$ 接受的语言记为：

$$
L(M)=\{w\;|\; (s,w,e)  \vdash_M^* (p,e,e),\; \text{for some }p\in F\}
$$


在表示上，我们在状态转移的箭头上新增一个项，表示我们从栈中 pop/push 了什么符号，例如 $\left((p,u,\beta), (q,\gamma)\right)\in \Delta$：

![[t2_16.png]]

我们一般所说的 PDA 都是**非确定性**的，允许读取一个符号后（再根据栈的状态）做不同转移，也允许做 e-transition；而确定性的 PDA 并不与 CFL 等价，此处不再证明，请直接记住这个结论。

例如，对于 $L(M)=\{ww^R\; :\; w\in \{a,b\}^*\}$，它的 diagram 如下：

![[t2_17.png]]


!!! example "更多例子：拥有相同数量 $a$ 和 $b$ 的 Language！"
	![[t2_18.png]]
	
	??? question "Is c necessary?"
		$c$ 被用作栈底标志，可以简化 PDA 的设计，但并不是必要的。

??? example "More Example"
	![[t2_20.png]]


事实上，对于 $L(G)=\{wcw^R\; : \; w\in \{a,b\}^*\}$ 这种拥有形如 $R=\{S\rightarrow aSa\;|\; bSb\;|\;c\}$ 的规则的 CFG，可以分如下三步构造 PDA 的转移方程：

$$\begin{array}l
I: & ((p,e,e), (q,S)) & \#初始压栈
\\ II: & ((q,e,S),(q,aSa)), ((q,e,S),(q,bSb)), ((q,e,S),(q,c)) & \#书写规则
\\ III: & ((q,a,a),(q,e)), ((q,b,b),(q,e)), ((q,c,c),(q,e)) & \#读取输入
\end{array}
$$


??? example "对字符串 abbcbba 按照如上转移方程进行推导"
	$$\begin{array}l(𝑝, 𝑎𝑏𝑏𝑐𝑏𝑏𝑎, 𝑒) \vdash (𝑞, 𝑎𝑏𝑏𝑐𝑏𝑏𝑎, 𝑆) \vdash (𝑞, 𝑎𝑏𝑏𝑐𝑏𝑏𝑎, 𝑎𝑆𝑎) \vdash (𝑞, 𝑏𝑏𝑐𝑏𝑏𝑎, 𝑆𝑎) \\ \vdash (𝑞, 𝑏𝑏𝑐𝑏𝑏𝑎, 𝑏𝑆𝑏𝑎) \vdash (𝑞, 𝑏𝑏𝑐𝑏𝑏𝑎, 𝑏𝑆𝑏𝑎) \vdash (𝑞, 𝑏𝑐𝑏𝑏𝑎, 𝑆𝑏𝑎) \vdash (𝑞, 𝑏𝑐𝑏𝑏𝑎, 𝑏𝑆𝑏𝑏𝑎) \\ \vdash (𝑞, 𝑐𝑏𝑏𝑎, 𝑆𝑏𝑏𝑎) \vdash (𝑞, 𝑐𝑏𝑏𝑎, 𝑐𝑏𝑏𝑎) \vdash^∗ (𝑞, 𝑒, 𝑒)\end{array}$$


### Pumping Theorem for CFL

若 $L$ 是一个 Context-Free Language，则存在一个常数 pump length $p\in \mathbb{Z}^*$ 使得 $\forall w\in L$ with $|w|\ge p$ 可被分为 5 个部分 $w=uvxyz$，满足：

- $\forall i \ge 0,\; uv^i xy^iz\in L$
	- 无论我们将 $v$ 和 $y$ 重复多少次（包括0次，即删除），生成的字符串仍然属于 $L$。
- $|v|+|y| > 0$ 或 $|vy|\ge 1$
	- 被 pump 的字段非空
	- $v$ 和 $y$ 不能同时为空
- $|vxy| \le p$
	- $y$ 出现在 String 的前 $p$ 个字符内

泵定理是 Context-Free Language 的一个必要**不**充分条件，因此我们可以通过证明泵定理不成立来证明该语言不是一个 Context-Free Language。

**【Example】**

证明 $L=\{a^i b^j c^i d^j\}$ 不是 CFL：

- 对于任意 $p\ge 1$，取 $w=a^p b^p c^p d^p$，则 $|w|=4p\ge p$
- 任意划分 $w = uvxyz, |vxy| \le p, |vy| \ge 1$，则 $vxy$ 最多包含两种字符
- 此时取 $i=0$，则有 $u v^0 x y^0 z = uxz \notin L$

产生矛盾，因此 $L$ 不是上下文无关语言。

!!! quote "不存在判断任意语言是否为 CFL 的算法"
	- 证明语言**上下文无关**
		- 有限
		- 构造 CFG 或 PDA
		- 运算性质
	- 证明语言**非上下文无关**
		- Pumping Theorem
		- 运算性质
