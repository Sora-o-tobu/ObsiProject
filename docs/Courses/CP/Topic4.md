
# Abstract Syntax

## Semantic Actions

Parser 在识别出某个语法结构（产生式）时，可以立即进行求值、构建树、查符号表等**语义动作**。

产生式 $T\rightarrow T*F$ 对应的语义动作如下：

```c
int a = T();
eat(TIMES);
int b = F();
return a * b;
```

??? question "$T\rightarrow FT'; T'\rightarrow *FT'$ 的语义动作如何设计"
	第二个 rule 要获取到第一个 rule 中 $F$ 的值，可以通过函数参数的形式进行传递，例如：
	
	```c
	int T(void) { switch(tok) ...
		case LPAREN: return T'(F());
	}
	int T'(int a) { switch(tok) ...
		case TIMES: eat(TIMES); return T'(a*F());
	}```

在 Yacc-Generated Parsers 中，语义动作就是 rules 的 `{...}` 中的部分。

## Abstract Parse Tree

Reduce 过程中构建的语法树，每个 Leaf Node 对应输入的一个 token；每个 Internal Node 对应一条 Grammar Rule。这种语法树表示了语言的具体语法，因此被称为**具体语法树**（**Concrete Parse Tree**）。

然而，具体语法树包含过多冗余信息，Parser 为了更好地将信息传递给 Compiler 的下一个阶段，会将 Grammar 及其对应的 Parse Tree 都进行简化，从而得到 **Abstract Parse Tree**（AST）。

![[cp_topic4_1.png]]

对于如上 AST，C 语言的数据结构实现可以为：

```c
typedef struct A_exp_ *A_exp;
struct A_exp_ {
  enum { A_numExp, A_plusExp, A_timesExp } kind;   // 变体标签
  union {
    int num;
    struct { A_exp left; A_exp right; } plus;
    struct { A_exp left; A_exp right; } times;
  } u;
};

// Constructors
A_exp A_NumExp(int num);
A_exp A_PlusExp(A_exp left, A_exp right);
A_exp A_TimesExp(A_exp left, A_exp right);

// Constructor for A_PlusExp for example
A_exp A_PlusExp(A_exp left, A_exp right) {
	A_exp e = checked_malloc(sizeof(*e));
	e->kind = A_plusExp;
	e->u.plus.left = left;
	e->u.plus.right = right;
	return e;
}
```

在 Yacc 中，使用如下 Rules 及其对应的语义动作：

```c
exp : NUM              { $$ = A_NumExp($1); }
    | exp PLUS exp     { $$ = A_PlusExp($1, $3); }
    | exp TIMES exp    { $$ = A_TimesExp($1, $3); }
```

![[cp_topic4_2.png]]

**位置信息（Positions）—— 错误报告的关键**：

- 一遍编译器中，词法分析器还能记住当前token位置。
- 但启用AST后，**解析完才做语义分析**，词法器早已结束。
- 解决方案：在**每个AST节点**中增加pos字段（行号+列号）。
	- 词法器把每个token的起止位置传给解析器。

```c
A_exp A_NumExp(int num, position pos);
A_exp A_PlusExp(A_exp left, A_exp right, position pos);
```

!!! warning "Bison 支持使用 Position Stack，但 Yacc 不支持"