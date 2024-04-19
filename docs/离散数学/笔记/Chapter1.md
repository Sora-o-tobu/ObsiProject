# CH 1 : The Foundations: Logic and Proofs
## 1.1 Propositional Logic （命题逻辑）

### **Definition**
- A proposition is a declarative sentence that is either true or false, but not both.
也就是说必须要有意义，比如：
- `Where am I?`  不是
- `Cheer up!` 不是
- `X+1=2` 不是
- `1+1=2` 是
- `Hangzhou is the capital of China.` 是
- `The teacher exclaimed, “Don’t come into class late again!”` 是（说话这个行为是，内容无所谓）
### 组成
- Propositional variables:  small letters such as  p, q, r, s …
- Truth value: T (true proposition), F (false proposition)
- Logical operators (Connectives): be used to form compound propositions from existing propositions
	- Negation operator  $\neg(NOT)$
	- Conjunction operator  $\land(AND)$
	- Disjunction operator  $\lor(OR)$
	- Exclusive or operator  $\oplus(XOR)$
	- Conditional operator  $\to(IF-THEN)$ 一般来讲是$p\to q 指If\ p，then\ q或是p\ implies\ q，但是p\ only\ if\ q也是这种表达，请注意哦$
	- Biconditional operator  $\leftrightarrow(IF AND ONLY IF)$

**Precedence:**
$\neg$ >$\land$=$\lor$>$\to$=$\leftrightarrow$
这也说明：
	p $\lor$ q $\to$ r  means  (p $\lor$ q) $\to$ r

## 1.2 Application of Propositional Logic

好像没什么

## 1.3 Propositional Equivalences

### Classification
- **Tautology:** compound proposition that is ==always true==
- **Contradiction:** compound proposition  that is ==always false==
- **Contingency:** compound proposition that is neither a tautology nor a contradiction.

此处仅列出较为难记忆的定理：
**Absorption laws:**
- $p\lor (p\land q)\equiv p$
- $p\land (p\lor q)\equiv p$
**Logical equivalence involving biconditional statements:**
- $p\leftrightarrow q\equiv (p\land q)\lor(\neg p\land \neg q)$
- $\neg(p\leftrightarrow q)\equiv p\leftrightarrow \neg q$

## 1.4 Predicates and Quantifiers
### Predicates 谓词
#### Definition
A predicate (propositional function) is a statement that contains variables. Once the values of the variables are specified, the function has a truth value.

Examples:
- `P(X)="x>3"`
- `Q(x,y)="x is the best player in the team y"`
- `R(x,y,z)="x+y=z"`

### Quantifiers 量词
分为Universal quantification $\forall$ 和 Existential quantification $\exists$

需要注意，量词的优先级大于所有的逻辑运算符号，所以:
$\forall x P(x)\land Q(x) = (\forall x P(x))\land Q(x)$
因此，一定要记得加括号哦~

#### Binding Variables 
A variable is bound if it is quantified 
A variable is free if it is neither quantified nor specified with a value

Example:
$\exists x\ (x+y)=1$ 中，x是bound，y是free

#### Logical Equivalences

- $\forall x(A(x)\land B(x))\equiv \forall A(x) \land \forall x B(x)$
- $\exists x(A(x)\lor B(x))\equiv \exists A(x) \lor \exists x B(x)$
- $\forall x(A(x)\lor B(x))\ne \forall A(x) \lor \forall x B(x)$
- $\exists x(A(x)\land B(x))\ne \exists A(x) \land \exists x B(x)$

## 1.5 Nested Quantifiers

同种类型的量词嵌套可以互换，但是不同就不行
For example:
- $\exists x\exists y\ P(x,y)\equiv \exists y\exists x\ P(x,y)$
- $\exists x\forall y\ P(x,y) \ne \forall y\exists x\ P(x,y)$

## 1.6 Rules of Inference

> [!NOTE] TABLE
> ![[rules of inference.jpg]]
> 



其中尤其需要记住最后一条`Resolution`

## 1.7 Introduction to Proofs
### Some Terminology（术语）

- theorem 定理，定律
- Axioms 公理，自明之理 = postulates 假设，前提        即我们假定是对的statements
- lemma 引理
- corollary 推论
- conjecture 推测 如果conjecture被证明为True，它就变成了theorem

### Proof by Contraposition 反证法

$P\to Q \equiv \neg Q\to \neg P$

### Vacuous Proof
If we know P is false then  $P\to Q$ is vacuously true.

### Trivial Proof
If we know Q is true, then $P\to Q$ is true

### Proof  p by Contradiction
步骤：
- 假设p是False
- 推导出一对矛盾的结论
- 说明p是True


> [!NOTE] Example
> ![[没有最大质数的证明.png]]


## 1.8 Proof Methods and Strategy

举例法和穷举法

## Appendix : NormalForms

**[Definition]** A set of logical operators is called ==functionally complete== if every compound proposition is logically equivalent to a compound proposition involving only this set of logical operators. 即可以用这组逻辑运算表示所有的逻辑运算,如 $\neg \ and\ \land(\lor)$

**[Definition]** ==Disjunctive clauses== are disjunctions with one or more literals ($p\ or\ \neg p$) as disjuncts.
> **Example**
> $q\lor r$ Disjunctive clause
> $\neg q\land p$ Conjunctive clause
> $p\land q\lor r$ Not a clause


**[Definition]** A conjunction with one or more disjunctive clauses as its conjuncts is said to be in ==conjunctive normal form==. (Conjunction 是中间都是$\land$)
> **Example**
> $p\land (q\lor r)$ √
> $\neg q\land p$ √
> $p\land ((p\land q)\lor r)$ ×


相应的，==Full Conjunctive Normal Form== 就是在conjunctive normal form的基础上要求所有字母都在clauses中出现，如:
$$
(p\lor q\lor r)\land(p\lor q\lor \neg r)\land(\neg p\lor \neg q\lor r)
$$
另一种表达方式是Full Conjunctive Normal Form是a conjunction of ==maxterms==

**[Definition]** 简单来讲，就是将所有Quantifiers放到前面的形式
> **Example**
> $\forall xP(x)\ \lor \ \forall Q(x)$  不是
> $\forall x\forall y \ \neg(P(x)\lor Q(x))$  是
> $\neg \forall xR(x)$ 不是

