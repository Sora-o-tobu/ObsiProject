
# Parsing

## Overview

语法分析的输入是词法分析的输出结果，即 token 序列；输出是抽象语法树，用于后续语义分析。

**语法**（**Syntax**）是如何将 words 组装成 phrases, clauses, sentences 的方式；而**语法分析**（**Syntax Analysis**）则基于一定文法（grammar）规则， 对一个程序的 phrase structure 进行 *parsing*。

![[cp_topic3_1.png]]

??? note "词法分析并不会在意不同 token 之间的关系，因此我们需要语法分析"
	![[cp_topic3_2.png]]


## CFG

**上下文无关语言**（**CFG**）用于判断一个 token 序列（or String）是否合法。

!!! info "Context-Free Grammer"
	相比于词法分析使用的正则语言，CFG 拥有一定的记忆功能，从而能够完成一些更高级的任务（例如递归括号匹配），我们称上下文无关语言的*表达能力更强大*。

正如我们之前学习过的一样，我们称能够用多个不同的 parse trees 构造出同一个 string 的文法是 **ambiguous** 的，这一特性会对编译器在语义上的理解带来歧义，因此需要尽可能避免。

例如对于式子 `2*3+4`，我们希望其满足两个性质：

- *Precedence*
	- `*` > `+`
- *Left-Association*
	- `1-2-3` = `(1-2)-3`, instead of `1-(2-3)`

为此，我们使用额外的非终结符来确保高优先级的 `*` 两侧的符号不会出现 `+`，除非使用小括号进行递归：

![[cp_topic3_3.png]]

上述修改强制让 $T$ 相关的产生式晚于 $E$ 出现，从而确保了在语法分析树中不会出现二义性；并且，该修改还满足左递归（产生式右侧的第一个符号与左侧的符号相同），从而实现运算规则的左结合。

??? warning "Ambiguous Parse Tree"
	![[cp_topic3_4.png]]

## Parser

### Top-Down Parsing

对于一个 Compiler，要分析所有 CFG 的代价是非常高昂的。通常，编写者只专注于其中的一部分子类，能够高效地满足对应的 Programming Language 的需求即可。

**Recursive Descent Parsing** 是一种自顶向下、Predictive、Simple 的分析方法，它也被称为 **LL(k) Grammar**：

- **L（Left-to-right）**：从左到右扫描输入串
- **L（Leftmost derivation）**：做**最左推导**
- **k（k symbol lookahead）**：只看 **k 个 token** 就决定该走哪个表达式

!!! example
	![[cp_topic3_5.png]]

```c
enum token {IF, THEN, ELSE, BEGIN, END, PRINT, SEMI, NUM, EQ};
extern enum token getToken(void); // from lexer
enum token tok; 

void advance() { tok = getToken();} 
void eat(enum token t) {if (tok==t) advance(); else error();}

void S(void) {
  switch(tok) {
    case IF: eat(IF); E(); eat(THEN); S(); eat(ELSE); S(); break; 
    case BEGIN: eat(BEGIN); S(); L(); break; 
    case PRINT: eat(PRINT); E(); break; 
    default: error();
  }
}

void L(void) {
  switch(tok) {
    case END: eat(END); break;
    case SEMI: eat(SEMI); S(); L(); break;
    default: error();
  }
}

void E(void) { eat(NUM); eat(EQ); eat(NUM); }
```

*Predictive Parsing* 有三个相关概念：

- **Nullable**
	- 一个非终结符 $A$ 是可空的，当且仅当 $A\Rightarrow ^{*} \varepsilon$
	- 如果有 $A\rightarrow BC$，且 $B,C$ 均为 Nullable 的，则 $A$ 也是 Nullable 的
- **First Set**
	- 从 $A$ 推导出的字符串中，可能出现在最前面的终结符集合
	- $FIRST(A) = \{a | A\Rightarrow ^{*} a \alpha\}$
	- $\varepsilon$ 也可以在 $FIRST(A)$ 中
- **Follow Set**
	- 某些推导中，可以紧跟在 $A$ 后面的终结符集合
	- $FOLLOW(A)= \{a| S\Rightarrow ^{*} \alpha A a \beta\}$
	- 对于产生式 $A\rightarrow \alpha B \beta$：
		- $FOLLOW(B) = FOLLOW(B) \cup (FIRST(\beta) -\{\varepsilon\})$
		- 如果 $\beta \Rightarrow ^{*} \varepsilon$，则 $FOLLOW (B) = FOLLOW (B) \cup FOLLOW (A)$

![[cp_topic3_6.png]]

**LL(1) Grammar** 要求构造的 Predictive Parsing Tables 中的每个表项中至多有一个 entry，但并不要求表项一定非空。表项为空代表一个 *syntax error*，则进行相应的 exception 处理并退出即可。

!!! tip "`1` 体现在列名中的终结符长度为 1，如果是 LL(2)，则列名可能为 `ab`, `ac` 等"

在匹配过程中，由于要做最左推导，我们将产生式按照从右到左的顺序压入栈，这样子左侧的非终结符会更优先被推出栈。每当输入符号与栈顶符号相同时，我们就认为该终结符匹配成功，进行消去；最终输入和栈均为空时，即认为接受该输入。

!!! example "对左右括号进行匹配的例子"
	![[cp_topic3_7.png]]

假如产生式中同时存在 $E\rightarrow E+T$ 和 $E\rightarrow T$（左递归），那么构建的 Tables 中必定存在重复项，不满足 LL(1) 要求：

对于每个 $t\in FIRST(T)$，都一定存在 $t\in FIRST(E+T)$，此时 $E\rightarrow E+T$ 和 $E\rightarrow T$ 都会被放置在 row $E$, col $t$ 这一栏内。

为此，我们可以对存在重复项的式子进行改写。一种通用的改写方法如下：

$$
\begin{array}l
A\rightarrow A\alpha \\
A\rightarrow \beta
\end{array} \Rightarrow
\begin{array}l
A\rightarrow \beta A' \\
A' \rightarrow \alpha A' \\
A' \rightarrow 
\end{array}
$$
这种方法被称为 **Right-Recursion**，一个应用示例如下：

$$
\begin{array}l
E\rightarrow E+T \\
E\rightarrow T
\end{array} \Rightarrow
\begin{array}l
E\rightarrow TE' \\
E' \rightarrow +TE' \\
E' \rightarrow 
\end{array}
$$

!!! warning "若同一个非终结符推导出来的产生式中，前几个符号完全相同，此时使用 Left Factoring"
	$$\begin{array}l S\rightarrow \text{if } E \text{ then } S \text{ else } S \\ S\rightarrow \text{if } E \text{ then } S\end{array} \Rightarrow \begin{array}l S\rightarrow \text{if } E \text{ then } S\ X \\  X \rightarrow \text{else } S \\ X\rightarrow \end{array}$$




### Bottom-Up Parsing

LL(k) Grammar 虽然很容易实现成一个 C 程序，但是它的表达能力有限，需要对某些表达式进行合适的处理，以消除歧义。

**LR(k) Parsing** 是一种表达能力更强的文法类，它将 LL(k) 中间的 L 替换为 R，对应于 *Rightmost Derivation*，即最右推导。该方法的核心操作为*移入*和*规约*，因此也被称为 **Shift-Reduce Parsing**。

??? info "简单来讲，LR(k) 的思想就是从输入串开始，根据产生式逐步将其规约至起始符号"
	![[cp_topic3_8.png]]

我们定义一个表达方式 $\alpha . \beta$，点号 $.$ 左侧的 $\alpha$ 子串中既可能包含终结符，又可能包含非终结符；右侧的 $\beta$ 子串中只可能包含终结符，代表我们还未对其进行规约处理。所谓的 *Shift* 操作，就是对这个点号进行右移处理，每右移一位即读入一个 token。

我们从最简单的 LR(0) Parsing 开始理解，它不超前查看输入的任何 token，而只根据状态栈顶对应的 state 进行处理。

!!! failure "当然，LR(0) 相当于符号栈没有作用了，退化为正则语言（或有限状态机 FA）"

在 LR(0) 中，表达式（*LR(0) item*） $A\rightarrow \alpha . \beta$ 代表了编译器已经处理了 $\alpha$，并且期望之后能够得到 $\beta$。

我们使用增广文法，引入新的开始符号 $S'$，并添加一条新的式子 $S'\rightarrow S$。当编译器试图用该式进行规约时，代表整个输入串已经完成解析。

基于此，我们构造一个 DFA：

![[cp_topic3_9.png]]

!!! question "如何理解上图的 NFA 中空转移的含义？"
	当点号停留在一个非终结符 $S$ 前时，表示编译器接下来期望读取到一个能规约成 $S$ 的字符串。因此，它立刻开始尝试匹配 $S$ 的各种可能性，因此会用空转移指向所有 $S\rightarrow xxx$ 的状态。

接下来给出一个更加复杂些的文法，并且对每个状态进行标号处理：

![[cp_topic3_10.png]]

我们维护 *Stack* 和 *Symbol* 两个栈，每次根据状态栈顶、符号栈顶、next token 这三项决定接下来的 *Action*：

![[cp_topic3_11.png]]

为此，我们构造一个更符合计算机直觉的转移表，每个表项代表了一种 Action：

![[cp_topic3_12.png]]

LR(0) Grammar 自然存在局限性，有时会出现转移表中一个格子中存在多个项，此时我们需要根据 LHS 的 $FOLLOW$ 来判断其能否 reduce：

![[cp_topic3_13.png]]

!!! note "将这种判断方式应用到全表，则该文法称为 Simple LR Parsing（SLR）"

上述方法一定程度上了解决了冲突，但我们也很容易想到，如果符号 $+$ 也出现在 $FOLLOW(E)$ 中，导致 s4, r2 又出现在同一个格子中了呢？

**LR(1) Parsing** 在每条产生式的基础上，额外超前查看了一个终结符来决定是否进行 reduce。一个 LR(1) item 新增了一个 lookahead symbol，形式为 $A\rightarrow \alpha . \beta, x$，它意味着接下来的输入需要能够被 $\beta x$ 推导得到。

!!! tip "对于 $A\rightarrow \alpha . \beta, x$，下一个 input token 一定属于 $FIRST(\beta x)$，注意 $\beta$ 可能是 nullable 的"

对比 LR(0) 的闭包计算算法，它会把所有 $w \in FIRST(\beta z)$ 都作为单独的一个 item 加入闭包：

![[cp_topic3_14.png]]

此时，只有下一个输入在当前状态的 lookahead symbol 中时，我们才允许它进行 reduce。

**【Example】**

![[cp_topic3_15.png]]

![[cp_topic3_16.png]]

当今的编译器大多都采用 **LALR Grammar**，它将 LR(1) 的很多状态进行了合并，是的其更为精简、高效。

!!! quote
	![[cp_topic3_17.png]]

## Automatic Parser Generation

通过学习我们可以看出无论时 LL Parsing 还是 LR Parsing，都是一种表驱动的算法。既然如此，我们自然可以通过更加一般化、更加鲁棒的 Parser Generator 来自动生成语法分析器。

!!! quote "我们并不保证 Parser Generator 一定比 hand-written parsers 的效率更高"

**Yacc**（**Yet another compiler-compiler**）是一个源编译器工具，我们实验所用的 Bison 就是其高级版本。它的 Input 为 `.y` 文件，输出为 `.c` 文件，实现方式采用 LALR Parsing。

Yacc 的基本格式如下：

```c
{definitions}
%%
{rules}
%%
{auxiliary routines}
```

Yacc 中，关于二义性消除，我们按照优先级从低到高定义如下规则：

```c
%nonassoc EQ NEQ // 不允许嵌套，例如 a != b != c
%left PLUS MINUS // 左结合
%left TIMES DIV
%right EXP       // 右结合
```

一条 rule 的优先级由其 RHS 的最后一个终结符优先级决定；根据下一个 token 和该条 rule 的优先级，Yacc 进行如下判断：

- 如果 rule 的优先级更高，则进行 Reducing
- 如果 rule 和 token 的优先级相同，则根据 token 类型：
	- %left 则 Reducing
	- %right 则 shifting
	- %noassoc 则报错

!!! example
	![[cp_topic3_18.png]]

另外，我们还可以直接使用 `%prec` 来指定 rule 的优先级。例如，我们希望 `-6*8` 应该视为 `(-6)*8` 而不是 `-(6*8)`，那么可以：

```c
%token INT PLUS MINUS TIMES UMINUS
%start exp
%left PLUS MINUS
%left TIMES
%left UMINUS
%%
exp : INT
| exp PLUS exp
| exp MINUS exp
| exp TIMES exp
| MINUS exp %prec UMINUS
```

## Error Recovery

作为开发者，我们希望在语法分析阶段就将所有错误都报出，而不仅在第一个错误就停下。错误恢复机制包括 **Local Error Recovery** 和 **Global Error Repair**。

### Local Error Recovery

当遇到某个 Token 报错时，尝试在该 Token 局部进行增删改等调整来恢复。

Yacc 通过一个特殊的 Token `error` 来控制恢复，用来通配掉某一步推导过程中所有的语法错误。

![[cp_topic3_19.png]]

对于 LR Parser，其遇到语法错误时会采取如下操作：

1. **Pop the stack**（必要时）：不断弹出栈顶状态，直到找到一个状态，在该状态下对 **error** 这个特殊终结符有 **shift** 动作。
2. **Shift the error token**：把 error 符号移入栈中（相当于“接受了错误”）。
3. **Discard input symbols**（必要时）：丢弃后续输入符号，直到找到一个能让当前 lookahead 有**非错误动作**（shift 或 reduce）的符号。
4. **Resume normal parsing**：恢复正常解析。

!!! warning
	- 由于要 POP 操作，可能会把之前已经完成的部分语法结构（例如已经归约的表达式、语句）丢掉。
	- 如果语义动作（Semantic Actions）有**副作用**（side effects），如打印信息、生成中间代码、修改符号表等，弹出后就可能导致“不可能”的语义动作执行。
	- **典型例子**：解析到 f( a, b, c 时发现错误（缺少右括号），如果之前左括号 ( 已经触发了语义动作，弹出栈时就可能造成语义不一致。
	
	![[cp_topic3_20.png]]
	
	解决方法可以是使用 side-effect-free semantic actions。（之后的内容）

### Global Error Repair

我们用恢复后能 Pass 的 Token 数量来评价一个恢复状态的好坏。

**Burke-Fisher Error Repair** 是一种全局错误恢复方法，主要用于 LR（或 LALR）语法分析器中。

|对比维度|Local Error Recovery（局部）|Burke-Fisher（全局）|
|---|---|---|
|恢复范围|错误点附近，少量弹出栈 + 跳过输入|回退 k 个 token，全局尝试修复|
|修复质量|较简单，常产生次优修复|质量高，接近“最小距离”修复|
|计算代价|低|较高（但可控）|
|典型实现|Error 产生式 + 栈弹出|回退 + 穷举单 token 操作|

当解析器在第 **n** 个 token 处检测到语法错误时：

1. **回退（Backup）**：回到错误发生前 **k** 个 token 的位置（k 通常取 3~5，常用 4）。
2. **穷举尝试修复**：在从 (n-k) 到 n 的这段区间内，尝试所有可能的**单 token 级修改**：
    - **删除（Deletion）**：删除某个 token
    - **插入（Insertion）**：在某个位置插入一个可能的 token
    - **替换（Substitution）**：把某个 token 替换成另一个 token
	    - 使用伪指令 `%change` 给予启发式的替换
    - **合并（Merging）**：把相邻两个 token 合并成一个（例如 := 被拆成 : 和 =）
    - **拼写纠正**（可选扩展）：如 BEGNI → BEGIN
3. **评估修复**：对每种可能的修复，**重新解析** 从修复点往后的输入，看看能成功解析多远。
4. **选择最佳修复**：挑选能让解析器继续最远（解析最多 token）的那个修复方案。
5. **报告 + 继续**：向用户报告做了什么修复，然后用修复后的输入继续正常解析。

