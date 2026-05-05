
# Semantic Analysis

!!! quote "*语义分析*是 Compiler 前端部分的最后一个阶段"
	- **连接定义与使用**（名字绑定）：把变量、函数的声明和使用关联起来。
	- **类型检查**（Type Checking）：确保表达式、运算符的操作数类型正确，消除语义错误。
	- **为后续阶段准备**：把AST翻译成更简单的中间表示（同时进行类型标注）。

## Symbol Tables

**符号表**用来将 Identifiers 映射到它们的 *types* 和 *locations*，因此也被称为编译器的 **environment**。

符号表中，一个映射关系（或者称为 binding）使用符号 $\mapsto$ 来表示，请看如下例子：

```c
// Language Tiger
function f(a:int, b:int, c:int) =
  (print_int(a+c);
   let var j := a+b
       var a := "hello"     // 遮蔽（shadow）外层的a
    in print(a); print_int(j)
   end;
   print_int(b) // 此时查找的符号表为 σ1
  )
```

**环境变化过程**：

- 进入 `function f`（Line 2）：$\sigma_{1} =\sigma_{0}+ \{a\mapsto \text{int}, b\mapsto \text{int}\}$
- 进入 `let`（Line 4）：$\sigma_{2}= \sigma_{1}+ \{j\mapsto \text{int}\}$
- 再声明 $a$（Line 5）：$\sigma_{3} =\sigma_{2}+ \{a\mapsto \text{string}\}$
	- 右边的绑定覆盖左边
- 退出 `let` 内块：丢弃 $\sigma_{3}$，恢复 $\sigma_{1}$
- 退出函数：丢弃 $\sigma_{1}$，恢复 $\sigma_{0}$

符号表有两种实现风格：

| 风格             | 特点                 | 恢复方式             | 适用场景          |
| -------------- | ------------------ | ---------------- | ------------- |
| **Functional** | 不修改旧表，创建新表（持久化）    | 直接使用旧表指针         | 易回溯、并发        |
| **Imperative** | 直接修改全局表，恢复时需要 Undo | 用Undo栈记录修改，pop恢复 | 内存高效（Tiger采用） |

??? addition "Multiple Symbol Tables"
	一些编程语言可以拥有多个同时正在使用的符号表。例如 Java 中每个 Class 都有自己的符号表，它允许符号表进行嵌套，例如使用 $\sigma_{2}=\{E\mapsto \sigma_{1}\}$ 来表示类 $E$ 使用符号表 $\sigma_{1}$：
	
	![[cp_topic5_1.png]]
	
	其它类似的还有 ML 等语言。

由于课本的 Tiger 语言采用的是命令式的（Imperative），我们此处就按照虎书中的进行讲解。

为了保持高效的查找速度，并且支持简单的删除操作，数据结构上采用 *Hash Table + External Chaining* 的方式来维护符号表。例如 `hash(a) -> <a,int> -> <a,string>`。

- 插入时，我们将新的 Binding 放在链表最前面，即*头插法*
	- 例如原本 $\sigma=\{a\mapsto \tau_{1}\}$，进行 $\sigma +\{a\mapsto \tau_{2}\}$ 操作
	- `hash(a) -> <a,τ2> -> <a,τ1>`
- 弹出时，我们进行 Pop 操作，将头部弹出，实现栈式撤销

![[cp_topic5_2.png]]

!!! info "对于 Functional 风格的符号表，我们只需要创建一个新的 Hash Table，并将原有的格子进行共享即可"
	![[cp_topic5_3.png]]

在查找、插入时，都需要计算该字符串的哈希值，其计算代码类似如下：

```c
unsigned int hash(char *s0) {
  unsigned int h=0; char *s;
  for(s=s0; *s; s++)
    h=h*65599 + *s; 
  return h; 
}
```

注意到计算时间与字符串的长度成正比，这对一些长字符串不友好。为了改进这一步骤，我们将所有变量名都映射到一个 **Symbol** 上，进行符号化，后续的比较都通过符号的指针来进行。

Tiger 的符号相关接口有如下这些，需要了解注释中的内容，而不用记住具体实现：

```c
typedef struct S_symbol_ *S_symbol;
S_symbol S_symbol (string);     // string -> symbol
string S_name(S_symbol);     // symbol -> string

typedef struct TAB_table_ *S_table;
S_table S_empty(void);     // create an empty symbol table
void S_enter(S_table t, S_symbol sym, void *value);  // enter binding
void *S_look(S_table t, S_symbol sym);  // look up symbol
void S_beginScope(S_table t);  // remember current table state
void S_endScope(S_table t);  // restore to most recent beginScope 
                             // that is not closed yet
```

`beginScope` 和 `endScope` 函数并非直接复制当前的状态，而是向当前 Table 中插入特殊标记符号 `<mark>`。当需要恢复时，则重复进行 POP 操作，直到遇到 `<mark>` 标记：

```c
static struct S_symbol_ marksym = { "<mark>", 0 };
void S_beginScope (S_table t) { 
  S_enter(t, &marksym, NULL); 
}
void S_endScope(S_table t) {
  S_symbol s;
  do 
    s= TAB_pop(t); 
  while (s != &marksym);
}
```

!!! note "因此需要一个辅助栈来存储各个 Symbols 进入符号表的顺序，从辅助栈弹出时将对应 Binding 从表中删除"

对于 Tiger Compiler，它实际维护两个符号表命名空间：

- **Type Environment**
	- Tiger 允许自定义类型，因此维护 Type Name 和实际 Type 的映射关系
	- `let type a = int`
- **Value Environment**
	- 供 Functions 和 Variables 使用

## Type Checking

通过遍历语法树，我们进行类型检查。

```c
Struct expty transVar (S_table venv, S_table tenv, A_var v); // 对变量 v 检查
Struct expty transExp (S_table venv, S_table tenv, A_exp a); // 对表达式 a 检查
Void transDec (S_table venv, S_table tenv, A_dec d); // 对声明 d 检查
Ty_ty transTY (S_table tenv, A_ty a); // 对类型变量 a 检查，会将 A_ty 翻译为 Ty_ty
```

其中第二个函数 `transExp` 本身是一个递归遍历的函数，它接受 value environment、type environment 和 expression 作为输入，输出是一个自定义结构体，包含 translated expression 和 Tiger-language type：

```c
struct expty {Tr_exp exp; Ty_ty ty;};
```

```c
// transExp
struct expty transExp(S_table venv, S_table tenv, A_exp a) {
switch(a->kind) {
   ...
   case A_opExp: { // operation
     A_oper oper = a->u.op.oper;
     struct expty left =transExp(venv,tenv,a->u.op.left);
     struct expty right=transExp(venv,tenv,a->u.op.right); 
     if (oper==A_plusOp) { // 对于加法，期望左右子树都是整型
       if (left.ty->kind!=Ty_int)
         EM_error(a->u.op.left->pos, "integer required");
       if (right.ty->kind!=Ty_int)
         EM_error(a->u.op.right->pos,"integer required");
       return expTy(NULL,Ty_Int()); 
     }
    case A_letExp: { // Tiger 中，声明只能由 let 开头
      struct expty exp; 
      A_decList d; 
      S_beginScope(venv); S_beginScope(tenv);
      for (d = a->u.let.decs; d; d=d->tail) // 遍历 declaration
        transDec(venv,tenv,d->head);
      exp = transExp(venv,tenv,a->u.let.body); 
      S_endScope(tenv); S_endScope(venv); 
      return exp;
    }
    ...
   }
 } 
 assert(0); /* should have returned from some clause of the switch */
}

// transDec
void transDec(S_table venv, S_table tenv, A_dec d) {
  switch(d->kind) { 
    case A_varDec: { 
      // var x := exp
      struct expty e = transExp(venv,tenv,d->u.var.init); // 获取 exp 的 type
      S_enter(venv, d->u.var.var, E_VarEntry(e.ty));      // 将 exp.type 复制给 d
  }
  ...
}
...

```

!!! failure "完整代码请看课件 PPT"

