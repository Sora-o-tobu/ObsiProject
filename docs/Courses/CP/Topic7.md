
# Intermediate Code

## Overview

编译器中端是连接“前端”（语义分析）和“后端”（代码生成、优化）的桥梁，核心内容是将抽象语法树 AST 翻译成一个机器无关的中间表示 IR。

为什么需要 IR？如果直接从源代码翻译到机器码，那么对于 $N$ 种语言，$M$ 种机器，则需要 $N\times M$ 种组合；而通过 IR，我们只需要编写 $N+M$ 种翻译器：

![[cp_topic7_1.png]]

常见的编译器中使用的 IR 有：

- **Three-Address Code，TAC**
- Static Single-Assignment，SSA
- Control Flow Graph，CFG
- Abstract Syntax Tree，AST
- **Expression Trees（IR Tree）**

## Three-Address Code

**三地址代码**的原则是，每条指令最多使用 3 个地址（操作数），其基本形式为 `x = y op z`。

例如，如下 AST 对应的三地址代码为：

![[cp_topic7_9.png]]

![[cp_topic7_2.png]]

!!! example
	![[cp_topic7_3.png]]

在实现上，由于它最多只使用三个地址，我们可以再添加一个 operation 字段，将一条三地址代码构成*四元组*(*quadruple*)表示并存储：

```c
t1 = x > 0              =>      (gt, x, 0, t1)
if_false t1 goto L1     =>      (if_f, t1, L1, _)
fact = 1                =>      (asn, 1, fact, _)
lable L2                =>      (lab, L2, _, _)
```

!!! success "另一种实现方式为*三元组*，它可以使用 `(n)` 的形式来指代第 n 行代码的结果"
	![[cp_topic7_10.png]]

## Intermediate Representation Tree

Tiger 编译器使用树状结构的 IR Tree，它有两类节点：

- **Expression 节点** `T_exp`
	- `CONST(i)`：整数常量
	- `NAME(n)`：汇编标签（符号地址）
	- `TEMP(t)`：临时寄存器（类似虚拟寄存器）
	- `BINOP(op, e1, e2)`：二元运算（+ - * /、位运算、移位等）
	- `MEM(e)`：内存读（fetch）；若作为 MOVE 左孩子则是内存写（store）
	- `CALL(f, args)`：函数调用（左到右求值）
	- `ESEQ(s, e)`：先执行语句 s，再求值 e
- **Statement 节点** `T_stm`
	- `MOVE(TEMP t, e) / MOVE(MEM(e1), e2)`：赋值
	- `EXP(e)`：求值 e 但丢弃结果
	- `JUMP(e, labs)`：无条件跳转
	- `CJUMP(op, e1, e2, trueLabel, falseLabel)`：条件跳转
	- `SEQ(s1, s2)`：顺序执行
	- `LABEL(n)`：标签定义

根据前端传递下来的 AST Expression 的特性，翻译时内部将其分为三类：

- *Expression with return values*
	- Ex，用 `T_exp` 表示
- *Expression with return no value*
	- Nx，用 `T_stm` 表示
	- 例如过程调用、while expression 等
- *Expression with boolean values*
	- Cx，Conditional Jump，用 `T_stm` 的组合表示
	- 例如 `a > b`

![[cp_topic7_4.png]]

!!! note "上述 `NULL` 部分留空，等到已知后再填入，实现方式为 Patch Lists"

对于 `flag := (a>b|c<d)`，赋值语句右侧应为 Ex，我们使用转换函数 `toEx(...)` 来将条件 Cx 转换为 0/1，例如：

![[cp_topic7_5.png]]


!!! success "`toEx(Cx)` 的具体实现"
	![[cp_topic7_6.png]]

函数栈帧中的一个简单变量 `v` 可以用它的 Memory Location 来声明：

![[cp_topic7_7.png]]

!!! info "Tiger Compiler 中，所有变量的大小都是 1 word"

上述 IR Tree 也可以等价表示为：

![[cp_topic7_8.png]]

!!! warning "Tiger 中的数组、Record（结构体）都是指针形式的"

