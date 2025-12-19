
# Undecidability

## Church-Turing Thesis

**Algorithm** 被理解为总能在所有输入上都停机的图灵机。根据 Church-Turing Thesis 假设，凡是能以算法形式执行的 Computation，都可以被图灵机模拟。

!!! note "即当且仅当可以用图灵机实现时称为算法，该论题不能严格证明，但它是计算理论的基础假设"

我们知道，TMs 的个数是可数的，而 Problems 或 Language 是不可数的，因此一定存在不可被图灵机计算的问题，也称一定存在 Nonrecursive Language。

!!! tip "Every Recursive Language is *decided* by a TM"

## Universal Turing Machines

将任意图灵机 $M$ 的状态和符号用有限字符串表示（例如用二进制编码状态和符号），并将整个 $M$ 的五元组表格序列化为字符串。万能图灵机 $U$ 接收输入 $\langle M\rangle,w$，模拟任意图灵机 $M$ 在输入 $w$ 上的行为。

具体设计如：$U$ 取三条磁带，将描述 $\langle M\rangle$ 复制到第二条磁带、将输入 $w$ 写在第一条磁带，第三条磁带记当前位置状态；然后循环模拟 $M$ 的一步：在第二条磁带检索对应四元组以决定新的状态和符号，直到 $M$ 进入停机状态为止。这样，$U$ 在 $\langle M\rangle w$ 上的计算与 $M$ 在 $w$ 上的计算等价。

万能图灵机证明了图灵机“可编程”的能力，即可以用单一机器来模拟所有图灵机；也引出了重要概念：**可判定**与**可枚举**。

## Halting Problem

停机问题是最经典的不可判定问题。

假定我们有一个测试程序，当有程序不递归调用自己时，测试程序就调用它；当有程序递归调用自己时，测试程序就不调用它。那么试想，测试程序是否会调用自己呢？

上述例子说明当算法询问自身行为时会产生矛盾。我们有子程序 `halt(P,X)` 用来判断任意程序 `P` 在输入 `X` 是否会 Halt，是则返回 True。那么对于如下函数，我们调用 `diagonal(diagonal)` 是否会停机呢？

```c
diagonal(X):
	if halt(X, X):
		diagonal(X)
	else:
		halt
```

`diagonal(diagonal)` 停机当且仅当 `halt(diagonal, diagonal)=False`，从而发生矛盾。

形式化上，定义语言 $H=\{\langle M\rangle w: \text{图灵机}~M\text{在输入}~w\text{上停机}\}$。显然 $H$ 是**递归可枚举**（r.e.）的，因为万能图灵机可以枚举所有停机对。但定理指出 $H$ **不可递归**（不可判定），即没有算法能对任意 $(M,w)$ 判定是否停机。

该定理的证明利用类似自指的归谬法：若 $H$ 可判定，则可构造新机判定 $H_1=\{\langle M\rangle: M \text{在输入自己}\langle M\rangle\text{上停机}\}$；进一步推导出矛盾。这同时说明递归语言类严格包含于递归可枚举语言类，且递归可枚举语言不封闭于取补（类 RE 在取补下一般不可再 RE）。

## Undecidable Problems

不能被算法解决的问题被称为 **Undecidable Problems** 或 **Unsolvable Problems**。

**【Definition】**
若存在可计算函数 $\tau:\Sigma^* \to\Sigma^*$，使得对于任意字符串 $x$，如果 $x\in L_1$ 当且仅当 $\tau(x)\in L_2$，则称 $L_1$ 可规约（reduce）到 $L_2$，用 $L_1 \le _P L_2$ 表示。

!!! abstract "规约是计算理论中核心技巧，常用于比较问题难度及证明 NP-完全性 或不可判定性"

若可判定（递归）问题 $L_2$ 可用算法解决，则通过 Recursive Function $\tau$ 可构造算法解决 $L_1$；反之若 $L_1$ 不可判定且 $L_1\leq_P L_2$ 则 $L_2$ 也不可判定。类似地，递归可枚举的性质也可通过规约得出：若 $L_2$ 可枚举，则 $L_1$ 可枚举；若 $L_1$ 不可枚举，则 $L_2$ 也不可枚举。

!!! quote "总结，在 $L_1 \le_P L_2$ 的情况下"
	- 如果 $L_1$ not recursive，则 $L_2$ not recursive
	- 如果 $L_1$ not r.e.，则 $L_2$ not r.e.
	- 如果 $L_2$ recursive，则 $L_1$ recursive
	- 如果 $L_2$ r.e.，则 $L_1$ r.e.

通过规约可证明下列问题是**不可判定**的：

- (a) 给定图灵机 $M$ 和输入串 $w$，判断 $M$ 是否在 $w$ 上停机（即经典停机问题）。
- (b) 给定图灵机 $M$，判断 $M$ 是否在空字符串上停机。
- (c) 给定图灵机 $M$，判断 $M$ 是否存在*某个*输入字符串使其停机（语言是否非空）。
- (d) 给定图灵机 $M$，判断 $M$ 是否对**所有**输入都停机（即语言是否等于 $\Sigma^*$）。
- (e) 给定两台图灵机 $M_1,M_2$，判断它们是否停机于完全相同的输入集。
- (f) 给定图灵机 $M$，判断 $M$ 所识别的语言是否是正则语言？是否是上下文无关？是否是递归语言？
- (g) 存在一台固定的图灵机 $M^*$ 使得：给定任意输入 $w$，判断 $M^*$ 是否在 $w$ 上停机（即停机问题的“固定实例”版本）。

以上各问题均可借助停机问题或 Rice 定理证明其不可判定性。例如，可构造映射规约把 (a) 归约到 (b)：

- $H=\{Mw \;|\; M \text{ halts on } w\}$
- $L=\{M \;|\; M \text{ halts on } e\}$
- $f(Mw)=\text{the encoding of } M_w = \{\text{if input } x\ne e \text{, reject; else run } M \text{ on } w\text{; if } M \text{ Halts, accept}\}$
	- 对于 $Mw \in H$，一定有 $f(Mw)\in L$
	- 对于 $Mw\notin H$，一定有 $f(Mw) \notin L$（因为 $M$ 不在 $w$ 上停机，一定被拒绝）
	- 因此有 $H\le_P L$

因为 $H$ 是不可递归的，因此 $L$ 也不可递归。

??? example "对于其它例子所构造的可计算函数 $f$"
	- **(c)** 从 (b) 规约至 (c)：$f(M)=\text{the encoding of } M_e = \{\text{1. Erase the input } x \text{; 2. run M on } e \text{; 3. if } M\text{ halts, accept}\}$
		- 这样，对于任何不在空串上停机的图灵机 $M$，$f(M)$ 一定会被拒绝
	- **(e)** 从 (d) 规约至 (e)：$f(M_1)=\text{the encoding of } M_1 M_2 = \{\text{1. } M_2 \text{ 是在所有输入上停机的机器 }  \text{; 2. run }  M_1\text{ on } x \text{; 3. if } M_1\text{ halts, accept}\}$
		- 对于某个不在所有输入上停机的机器 $M$，一定有 $f(M)$ 的返回值 $M_1 M_2$ 两台机器在不同输入上不同时停机（即存在某个输入 $w$，使得 $M_1$ 不在 $w$ 上停机，而 $M_2$ 在 $w$ 上停机）


**可判定问题示例**：并非所有看似复杂的问题都是不可判定的。例如：

- **有穷自动机语言包含性:** 给定有限自动机 FA $M_1,M_2$，判断是否 $L(M_1)\subseteq L(M_2)$。这可化为“$L(M_1)\cap (Σ^*-L(M_2))=\emptyset$”的空语言测试问题，通过遍历检查是否存在可达终止状态来解决。因此自动机包含性是可判定的。
- **机器语言计数性:** 语言 $L(M)$ 总是可列（countable），因此判断 $L(M)$ 是否可列或不可列是平凡的可判定问题。其他类似的性质如“是否存在一个输入使得 $M$ 在 $|M|$ 步内停机”等，也可通过枚举有限输入并模拟解决。

例如，令  

$$L_{even}=\{⟨M⟩∣M 的语言 L(M) 中至少包含一个含有偶数个 b 的字符串\}$$

可以证明 $L_{\text{even}}$ 是递归可枚举而不可递归的。其可枚举性通过交错模拟 $M$ 在所有字符串上的行为得到；其不可判定性则通过停机问题归约：构造新机 $M'$ 使得 $L(M')={\varepsilon}$ 当且仅当原机 $M$ 在空串上停机（$\varepsilon$ 含 0 个 $b$，0 为偶数），从而将 $H$ 归约到 $L_{\text{even}}$。

**Rice定理示例**：上述 (f) 项属于 Rice 定理讨论的范畴——判断 $L(M)$ 是否具有某种非平凡性质（如正规、上下文无关、递归）一般都是不可判定的（后节 Rice 定理会详细说明）。例如问题“$L(M) = \{,\texttt{"2024"},\}$？”（某特定语言）即为语言的语义性质，也是不可判定的。

## R.E. Languages

事实上，我们有理论：

$$
L\text{ is recursive iff } L \text{ and } \bar{L} \text{ are both R.E.}
$$

