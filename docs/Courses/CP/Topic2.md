
# Lexical Analysis

## Lexical Token

**Lexical Token** 是编程语言的基本语法单元，通常被划分为各种 Types，例如 `ID`(Identifier), `NUM`, `REAL`, `COMMA`(`,`), `NOTEQ`, `LPAREN`, `IF` 等等。

除此之外，以下几类都属于 non-tokens，通常会在进行正式分析前被剔除：

- `comment`
	- 例如 `/* */`
- `preprocessor directive`
	- 例如 `#include <stdio.h>`
- `macro`
	- 例如 `#define NUMS 5,6`
- `blanks`, `tabs`, `new-lines`

因此，我们需要定义合适的 Lexical Rules 来描述编程语言的 Token。一个 Simple, General Purpose, Readable 的分析器使用**正则表达式**进行实现。

## Regular Expressions & FA

![[cp_topic2_1.png]]

为了便于讲解，此处暂且定义如下几个 token 的正则表达式：

$$
\begin{array}l
\text{if} & \{\text{return IF;}\} \\
[\text{a-z}][\text{a-z0-9}]^{*}& \{\text{return ID;}\} \\
[\text{0-9}]^{+}& \{\text{return NUM;}\} \\
([\text{0-9}]^+\text{"."}[\text{0-9}]^*)|([\text{0-9}]^*\text{"."}[\text{0-9}]^+) &\{\text{return REAL;}\} \\
(\text{"--"}[\text{a-z}]^{*}\text{"\\n"}) | (\text{" "} | \text{"\\n"} | \text{"\\t"}) ^{+} & \{\text{/* do nothing */}\} \\
. & \{\text{error();}\}
\end{array}
$$

其中 $.$ 表示除了以上所有规则之外的 String。根据*最长匹配原则*（*Longest Match*）和*首位优先原则*（*Rule Priority*），拥有最短规则并且写在最后的 $.$ 会成为所有字符串最后匹配的对象。

定义完合适的正则表达式后，编译器会自动生成对应的自动机。对于如上六条规则，它们各自的 DFA 为：

![[cp_topic2_2.png]]

但作为 Lexical Analyzer，我们还需要将所有的 FA 组装成一个大的自动机。此处省略细节直接给出最终的 *Combined Finite Automation*：

![[cp_topic2_3.png]]

相应的，我们也可以将一个 DFA 图转化为更符合编程习惯的转移矩阵表。它的纵坐标表示状态，横坐标表示输入；并且，我们将 *state 0* 作为 dead state 来使用：

![[cp_topic2_4.png]]

在对 DFA 根据*最长匹配原则*进行匹配时，Lexer 需要跟踪并维护两个变量：`Last-Final` 和 `Input-Position-at-Last-Final`。它们分别表示上一个 Final State 和对应的输入指针位置。

!!! note "这两个变量会在每次进入任意 Final State 时被更新"

例如，对于输入 `if --not-a-com`，它的匹配过程如下：

![[cp_topic2_5.png]]

!!! info "由于我们要求注释必须要以 `\n` 结尾，且中间不能再出现 `-`，该输入的后半部分会进入 error 的最终状态"

当然，实际上我们更可能是先将正则表达式转换为 NFA，再将其转换为 DFA 及其对应的转换矩阵，这样的构造顺序通常更高效。

相比于课程《计算理论》中学到的 NFA，本课程还将一个单独的 NFA 绘制成如下形状的图示，其中左侧的 *tail* 代表了NFA 的 start edge，右侧的 *head* 代表了 NFA 的 ending state：

![[cp_topic2_6.png]]

那么，一个大的 NFA 就可以由多个小的 NFA 组装而成，例如：

![[cp_topic2_7.png]]

基于此，我们对 token `IF`, `ID`, `NUM`, `error` 的 NFA 绘制如下：

![[cp_topic2_8.png]]

??? abstract "上述 NFA 转为 DFA 后的结果如图"
	![[cp_topic2_9.png]]

