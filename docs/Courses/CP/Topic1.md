
# Overview

## Introduction

**编译器 Compiler** 是一个复杂的程序，用于将 Program 从一个 Language 翻译到另一个 Language，即：

$$
\text{Source Program} \Rightarrow_{Compiler} \text{Target Program}
$$

实际上，编译原理串联起了从软件到硬件的完整链条：

![[cp_topic1_1.png]]

## Workflow

传统编译器的工作流分为前中后三个部分，这种模块化设计使得编译器的实现更容易理解与实现，并且允许复用部件。

![[cp_topic1_2.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">词法分析 Lexing</font>

**词法分析**将程序字符流（String）分解为记号序列（Token Sequence），它通过*正则表达式*进行匹配，删除了字符串中不必要的部分。例如：

![[cp_topic1_3.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">语法分析 Parsing</font>

**语法分析**接收传来的 Token 序列，并将其解析为合适的语法结构。通常，我们使用*抽象语法树*（*AST*）来表示语法：

![[cp_topic1_4.png]]

语法分析通常使用*上下文无关语法*进行解析，对于不合法的 Token，它能够检测到可能的错误并报告给用户。

<font style="font-weight: 1000;font-size: 20px" color="orange">语义分析 Semantic Analysis</font>

**语义分析**决定了语法结构的含义。

对于符合语法的程序，我们可以通过词法分析和语法分析从中构建出一个语法数；然而并不是所有合语法的程序都是合法的程序，我们需要检查是否符合*语义规则*。

!!! example "例如如下程序符合语法，但不是合法的程序"
	```c
	int a[5] = {0};
	int b = a;
	```

在这个阶段，我们通常应用符号表（Symbol Table）来进行处理。符号表中记录了程序中变量、函数等对象的各种属性，可由编译器各个步骤使用。

<font style="font-weight: 1000;font-size: 20px" color="orange">中间代码生成 IR</font>

**中间代码**是源语言和目标语言之间的桥梁，是一种过渡性质的代码形式，往往介于语法树和汇编代码之间。

![[cp_topic1_5.png]]

??? question "为什么不直接转为汇编代码？"
	1. 中间代码将编译器自然划分为前端和后端部分，如果需要改变编译器的源语言或目标语言，只需要重写其前端或后端即可。
	2. 中间代码不像语法树那样抽象，也不像汇编代码一样底层，更适合进行分析和优化。

<font style="font-weight: 1000;font-size: 20px" color="orange">优化</font>

基于中间代码，我们可以对其进行分析和变换，目标是*降低执行时间*或*减少资源消耗*。

![[cp_topic1_6.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">目标代码生成</font>

最后，整个后端的功能总的来说就是**目标代码生成**，它通过指令选择、寄存器分配、指令调度三个流程将中间代码翻译到*目标语言*：

![[cp_topic1_7.png]]


一个对阶乘函数转化为 RISC-V 汇编代码编译的完整工作流示例如下：

![[cp_topic1_8.png]]


