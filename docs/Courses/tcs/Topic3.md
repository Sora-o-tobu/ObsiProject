
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
	- 停机状态中可划分接受状态和拒绝状态
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

| 运算             | 正则语言 | 上下文无关语言 | 递归语言 | 递归可枚举语言 |
| -------------- | ---- | ------- | ---- | ------- |
| 并 ($\cup$)     | ✅    | ✅       | ✅    | ✅       |
| 交 ($\cap$)     | ✅    | ❌       | ✅    | ✅       |
| 连接 ($\circ$)   | ✅    | ✅       | ✅    | ✅       |
| 补 ($\bar{A}$)  | ✅    | ❌       | ✅    | ❌       |
| Kleene 星 ($*$) | ✅    | ✅       | ✅    | ✅       |

!!! note "If $L$ is a recursive language, then it is R.E"

!!! tip
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

## Primitive Recursive Functions

**原始递归函数**是一类基础的、必定停机的函数。它由如下三个基本函数通过任意次的组合(Composition)和递归(Recursive)定义而成：

- **零函数 Zero Function** $Z(x)=0$
	- k 元零函数 $Z_k ( n_1,..., n_k)=0$
- **后继函数 Successor Function** $S(n)=n+1$
- **投影函数 Projection Function** $P_i^k (x_1,..., x_k) = x_i$

常见的原始递归函数有：

- $sgn(x)\; \Rightarrow \; \left \{\begin{array}l sgn(0)=0 \\ sgn(n+1) =1 \end{array}\right.$
	- 其中 $n$ 指的是递归次数，原始递归构造的一般步骤为：
		- **基例**：$f(0)=m$
		- **递归步**：$f(n+1)= h(...,[f(..)])$，其中 $h()$ 也是原始递归函数
- $plus2(n)=n+2= S(S(n))$
- $plus(m,n)= m+n \; \Rightarrow\; \left \{\begin{array}l plus(m,0)=m \\ plus(m,n+1) = S(plus(m,n)) \end{array} \right .$
	- 通常简写为 $m+n$
- $mult(m,n)\;\Rightarrow \; \left \{\begin{array}l mult(m,0)=0 \\ mult(m,n+1) = plus(m,mult(m,n)) \end{array} \right .$
	- 通常简写为 $m\cdot n$
- $exp(m,n) \;\Rightarrow \; \left \{\begin{array}l exp(m,0)=1\\ exp(m,n+1)=mult(m, exp(m,n)) \end{array} \right .$
	- 通常简写为 $m\uparrow n$
- $f(n_1, ...,n_k)=m \; \Rightarrow \; S(...S(Z_k( n_1,..., n_k)))$
	- 即常数函数是原始递归的
- $pred(x) \;\Rightarrow \; \left \{\begin{array}l pred(0) =0 \\ pred(n+1) = n \end{array} \right .$
- $m\sim n = max\{m-n, 0\} \; \Rightarrow \; \left \{\begin{array}l m\sim 0 =m \\ m\sim (n+1) = m\sim n -1 =pred( m\sim n) \end{array} \right .$


**原始递归谓词 Primitive Recursive Predicate** 是只能取值为 0 或 1 的原始递归函数，它们将逻辑判断转化为了更加数学的形式，例如：

- $iszero(x)\;\Rightarrow \;\left \{ \begin{array}l iszero(0)=1 \\ iszero(n+1)=0\end{array} \right.$
	- 同时 $iszero(x) = 1\sim sgn(x) = \neg sgn(x)$
- $positive(x)=sgn(x)$
- $greater-than-or-equal(m,n) = iszero(n\sim m)$
	- 即 $m\ge n = iszero( max\{n-m,0\})$
- $less-than(m,n) = 1\sim greater-than-or-equal(m,n)$

!!! tip "原始递归谓词之间的 negation、disjunction、conjunction 操作也是原始递归谓词"
	- $\neg p(m)= 1\sim p(m)$
	- $p(m,n)\lor q(m,n) = 1\sim iszero(p(m,n)+ q(m,n))$
	- $p(m,n)\land q(m,n) = 1\sim iszero(p(m,n)\cdot q(m,n))$
	- $\exists t\le m,\; p(n_1,..., n_k, t) \;\Leftrightarrow \; \neg iszero(\sum_{t=0}^m p(n_1,..., n_k, t))$
	- $\forall t \le m, \; p(n_1,..., n_k, t) \;\Leftrightarrow \; \neg iszero(\prod_{t=0}^m p(n_1,..., n_k, t))$

有了谓词之后，我们即可声明如下**按情况定义**的函数也是原始递归的：

$$
f(n_1,..., n_k) = \left \{\begin{array}l g(n_1, ..., n_k), & \text{if } p(n_1,..., n_k) \\ h(n_1,..., n_k), & \text{otherwise}  \end{array}\right .
$$

其中 $g(), h()$ 都是原始递归函数，$p()$ 是原始递归谓词，上述函数等价于：

$$
f()= p()\cdot g() + (1\sim p())\cdot h()
$$

给定 k+1 元函数 $g(n_1,..., n_k, m)$，我们关心其中一个参数 $m$ 的取值能否使得 $g()=1$。为此，我们定义一个新的 k 元函数 $f()$：

$$
f(n_1,..., n_k) = \left \{ \begin{array}l
\text{the least }m \text{ such that } g(...,m)=1 \\
0 \text{  if no such } m \text{ exists}
\end{array}\right .
$$

该函数记作：

$$
f = \mu m [g(...,m)=1]
$$

直观上来看，该函数试图找到能够使得 $g()=1$ 的最小 $m$ 值并返回，因此它被称为 $g$ 的 **Minimalization**。不幸的是，函数 $f$ 并不是 Recursive 的，它完成功能如下述伪代码所示，可能找不到成立的 $m$ 值而永不停机：

```c
m := 0;
while g(n_1,...,n_k,m) != 1
	do m := m+1;
output m;
```

!!! note "该伪代码自然也不属于*algorithm*"

如果对于函数 $g()$，上述代码能够正常停机，我们就称其为 **minimalizable** 的，这也意味着其具有如下性质：

$$
\forall n_1,..., n_k \in N,\; \exists m\in N\; \text{such that } g(...,m)=1
$$

**【Theorem】**
可以通过基础函数的 *composition*, *recursive definition* 以及可最小化函数的 *minimalization* 得到的函数，是 **$\mu$-recursive** 的，也称*部分*递归的。

!!! example
	证明 $g(m,n,p)=greater-than-or-equal((m+2)\uparrow p, n+1)$ 是 $\mu$-recursive 的。
	
	实际上直接证明该函数是 minimizable 的即可。$\mu p [greater-than-or-equal((m+1)\uparrow p, n+1)]= \lceil \log_{m+2} (n+1)\rceil$。

**【Theorm】**
一个函数 $f: N^k \rightarrow N$ 是 $\mu$-recursive 的，当且仅当它是 recursive 的，亦即 TM Computable。

!!! bug "Primitive Recursive $\subset$ Recursive $\subset$ $\mu$-Recursive"
	图灵机中所述的 Recursive 属于 General Recursive Functions，要求对于所有输入都停机。

