
# Turing Machine

!!! danger "图灵机部分是考试的重点"

## (Deterministic) Turing Machine

PDA 的局限性在于我们只可对栈顶的元素进行操作，而图灵机则进一步将 Stack 替换为 Tape，允许一个读写头在纸带上任意移动并读写。

图灵机是一种理想化的计算模型，可以看作是一台拥有无限长磁带和读写头的简单计算机。磁带被划分为一系列格子，每格可以存放一个符号；机器有一个读写头，每次可读写当前格符号并左右移动一步。我们将其中一端设为左端标志（$\rhd$），符号表 $\Sigma$ 中包含一个特殊的空白符（$\sqcup$）和左端标志，但不包含“向左”“向右”这样的移动符号。图灵机通过有限的状态控制其行为，任何时刻处于某个状态并读写磁带上的符号，从而不断进行计算。

在形式定义上，一个图灵机由五个部分组成 $(K,\Sigma,\delta,s,H)$：

- $K$：finite set of states
- $\Sigma$：alphabet
	- 特殊符号 $\rhd$（left end symbol）表示纸带的最左侧，无法覆盖
	- 特殊符号 $\sqcup$（blank symbol）表示纸带这个位置是一个空格子
	- Alphabet 中不包括特殊符号 $\rightarrow, \leftarrow$
- $s\in K$：initial states
- $H\subseteq K$：set of halting states
- $\delta: (K-H)\times \Sigma \rightarrow K\times ((\Sigma - \{\rhd\})\cup \{\leftarrow ,\rightarrow \})$：transition function
	- $(K-H)$：非停机状态，停机状态无法转移
	- $\Sigma$：读写头读入的符号
	- $(\Sigma - \{\rhd\})$：读写头写入的符号
	- $\{\leftarrow, \rightarrow\}$：读写头向左/右移动
	- 读写头从 Left End 离开后，就不会再回到该位置，即需要满足
		- $\forall q\in K-H\; \text{if} \; \delta(q,\rhd)=(p,b),\; \text{then} \; b=\rightarrow$
		- $\forall q\in K-H\; \text{and}\; a\in \Sigma  \; \text{if} \; \delta(q,a)=(p,b),\; \text{then} \; b \ne \rhd$

图灵机是 Deterministic 的，不允许在同一个状态读入同一个符号而产生不同的输出。完整的图灵机定义还包括**接受状态**和**拒绝状态**两个部分，一个图灵机接受 String Input 当且仅当 TM 最后停机，并且处于接受状态：

| 最终状态     | 是否停机 | 结果          |
| -------- | ---- | ----------- |
| 接受状态     | 停机   | ✔ 接受        |
| 拒绝状态     | 停机   | ✘ 不接受       |
| 非接受/拒绝状态 | 不停机  | ✘ 不接受（loop） |

图灵机上定义的 Configuration 为三元组：$(q,u,v) \in K\times \rhd (\Sigma - \{\rhd\})^* \times (\{e\}\cup (\Sigma -\{\rhd\})^* (\Sigma -\{\rhd , \sqcup\}))$

- $u$ 称为左侧字符串
	- $\rhd (\Sigma - \{\rhd\})^*$：纸带从 Left End 到读写头为止的位置
- $v$ 称为右侧字符串
	- $\{e\}$：读写头右侧全为 Blank 则为 $e$
	- $(\Sigma -\{\rhd\})^* (\Sigma -\{\rhd , \sqcup\})$：读写头右侧的非空格子

若状态 $q \in K$ 属于停机状态，即 $q\in H$，则称该配置为停机配置。图灵机从初始配置 $(s, \rhd, w)$ 开始运行，按照 $\delta$ 规则不断转移配置，其中 $w$ 即为输入字符串。

!!! example "$(q,\rhd \sqcup ab, a)$ 可以等价写为 $(q, \rhd \sqcup a\underline ba)$，下划线表示读写头正指向的位置"

为了简化设计，人们引入了各种图灵机的扩展模型，如多带、多头、二维磁带、非确定性等，但这些扩展并不会本质增加图灵机的计算能力，即任何拓展图灵机都可以由普通单带确定性图灵机模拟。

多带图灵机拥有 $k$ 个磁带和 $k$ 个读写头。形式上，它的转移函数 $\delta$ 作用在所有 $k$ 个读写头当前读取的符号上，并同时更新所有头的位置和写入符号。初始时，输入字符串放在第一条带，其他各带均为空。结束时第一条带上的内容为输出，其他带忽略。

多带图灵机在实际构造中非常方便，例如可以用两带图灵机轻易实现字符串复制：第一条带读取原字符串，第二条带同步写入每个字符，然后再将第二条带上的内容拷贝回第一条带；也可并行地实现任意多比特的二进制加法（在两带上分别存放两个数，然后按位相加）。

## Recursive Language

!!! quote
	![[t3_1.png]]

我们规定图灵机有两个特殊的停机状态：接受态和拒绝态。如果在输入字符串 $w$ 下机器运行最终进入接受态并停机，就称该机**接受** $w$；如果进入拒绝态并停机，则称**拒绝** $w$。

若机器对于任意输入都必停机（进入接受或拒绝态），则称它**决定**（decide）一个语言 $L\subseteq\Sigma^*$：当且仅当 $w\in L$ 时机器接受 $w$，否则拒绝。语言 $L$ 如果存在如此的图灵机，就称 $L$ 为 Recursive Language。

如果一台图灵机对输入 $w\in L$ 时必停机接受，但对 $w\notin L$ 时则可能永不停机（既不接受也不明确拒绝），那么称该机**半判定**（semidecide）语言 $L$。能被图灵机半判定的语言称为**可识别语言**或**递归可枚举语言（Recursive Enumerable, RE）**。

!!! note "If $L$ is a recursive language, the it is R.E"

!!! note
	直观地说，图灵机可以“枚举”出语言中的所有字符串，但对于不在语言中的输入，它可能陷入无限循环。图灵可识别语言也称为类型-0语言，与任意**文法**生成的语言等价。

## Nondeterministic Turing Machine

**非确定性图灵机（Nondeterministic Turing Machine, NTM）**是一种允许在每一步有多种可能转移的模型。

形式上，一个 NTM 的转移关系 $\Delta$ 是一个从 $(K-H)\times\Sigma$ 到若干可能动作的多值映射。换句话说，在给定状态和符号下，它可能有多条规则可以同时适用。这时，机器的运行可以被看作是一棵**计算树**：机器从起始配置开始，每当遇到多个可选操作就沿不同分支继续计算。

只要有一条“猜测路径”使机器接受，即读取输入 $w$ 并最终进入接受态停机，就称该 NTM 接受 $w$。

- 如果 NTM 在所有输入上所有分支都保证停机，则它可以**决定**一个语言：当且仅当 $w$ 属于该语言时至少有一个分支接受，否则没有分支接受。
	- 比起确定性图灵机，这里多了一种“存在接受路径”的要求。
- 若只要求存在接受分支而其他分支可能不终止，则称语言被 NTM **半判定**。

!!! tip "任何 NTM 能识别或半判定的语言，都可以被某台确定性图灵机识别或半判定，也就是说 NTM 和图灵机计算能力相同"

**【Example】** 构造 NTM 来半判定语言 $C=\{\text{二进制表示的合数（非质数）}\}$。

我们利用 NTM 的非确定性直接猜测因子 $p *q$，由于 $p$ 和 $q$ 均小于输入 $w$，所以猜测是有限步的。我们将所有分支的 $p$ 和 $q$ 相乘，如果结果等于 $w$，则该分支能猜对并接受；否则，没有任何分支会接受（但可以无限循环，即不停机）。

此时最简单的方式是使用一个 3-带图灵机进行模拟，第一条磁带读取二进制输入 $w$，第二条磁带和第三条磁带分别用于写被猜测的 $p,q$。.

## Grammer

Grammer 是最一般的文法，与 Context-free Grammer 相比，Grammer 允许 rules 左侧包含任意数量的符号，但至少含有一个非终结符。

!!! abstract "回忆一下：CFG 要求 rules 左侧只能是单个*非终结符, Nonterminal*"

文法 $G$ 的推导记号用 $\Rightarrow_G$ 表示：若有 $uAw\to uBw$ 是 $G$ 的一条规则，则有 $uAw\Rightarrow_G uBw$。文法生成的语言 $L(G)=\{w\in\Sigma^*: S\Rightarrow_G^* w\}$ 是所有可从开始符号推导得出终结符串的集合。

**【Example】** 构造 $\{a^n b^n c^n: n \ge 1\}$ 对应的 Grammer：

$$\begin{aligned}
S &\to A B C S \mid T_c,\\ BA &\to AB,\quad CA\to AC,\quad CB\to BC,\\ C T_c &\to T_c c,\quad C T_c \to T_b c,\quad B T_b \to T_b b,\quad B T_b \to T_a b,\quad A T_a \to T_a a,\quad T_a \to e
\end{aligned}$$

直观上，**第一排的规则**会先从开始符号生成 $(ABC)^NT_c$，然后通过**第二排的重排规则**将所有的 $A$ 移动到最左边，所有的 $C$ 移动到最右边（$T_c$ 左边），从而重排为 $A^n B^n C^n T_c$，最终通过**第三排的替换规则**进行转换。

Grammer 和图灵机的等价关系表现为，一个 Language 可以被某个 Grammer 生成当且仅当它是一个 R.E Language，即可以被图灵机半判定。

