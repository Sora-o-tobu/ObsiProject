# CH 2 : Basic Structure
## 2.1 Sets
A set is an unordered collection of objects.
通常用大写字母表示集合，小写字母表示集合里的元素

### Cardinality 集合的势
Let S be a set. If there are exactly n distinct elements in S where n is a nonnegative integer, we say that S is a finite set and that n is the cardinality of S.
用| S | 表示 the cardinality of S

### Power set
Power set of S is the set of all subsets of the set S , 用 P(S) 表示

$P(S)=\{x|x\subseteq S\}$

Note:

- $|S|=n\ implies\ |P(S)|=2^n$
- $S\ is\ finite\ and\ so\ is\ P(S)$

> [!NOTE] Extreme example
> (1)$S=\{\emptyset\}$
> 
>    $P(S)=\{\emptyset ,\{\emptyset\}\}\ \ \ \ |P(S)|=2$ 
>    
> (2)$S=\{\emptyset ,\{\emptyset\}\}$ 
> 
> $P(S)=\{\emptyset , \{\emptyset \} , \{\{\emptyset\}\} , \{\emptyset ,\{\emptyset\}\}\}\ \ \ \ |P(S)|=4$

例题：
一. Show that $P(A)\in P(B)\ implies\ A\in B$
证明:
$$
P(A)\in P(B) \Rightarrow P(A)\subseteq B \newline 
$$

$$
\begin{cases}P(A)\subseteq B \\A\in P(A) \end{cases} \Rightarrow A\in B 
$$

二.Show that $A\subseteq B \Rightarrow P(A)\subseteq P(B)$

$$
\forall x \in P(A),\begin{cases}x\subseteq A \\ A\subseteq B\end{cases} \Rightarrow x\subseteq B\Rightarrow x\in P(B) \Rightarrow P(A)\subseteq P(B)
$$

### Cartesian Product 笛卡尔积

也就是叉乘

$A\times B=\{(a,b)|a\in A\land b\in B\}$

!!! note
	$A\times \emptyset =\emptyset \times A$


## 2.2 Set Operations

- **Union** 
	- $A\cup B=\{x|x\in A\lor x\in B\}$
- **Intersection**
	- $A\cap B=\{x|x\in A \land x\in B\}$
- **Difference**
	- $A-B=\{x|x\in A \land x\notin B\}=A\cap\overline{B}$

由此得到:
$|A\cup B|=|A|+|B|-|A\cap B|$


> [!NOTE] TABLE
> ![[集合的inference表.png]]


## 2.3 Functions

Let A and B be nonempty sets. A ==function (mapping or transformations)== f  from A to B:
$$
f:A\to B
$$

$$
Also\ \forall a(a\in A\to \exists !b(b\in B\land f(a)=b))
$$
A is called ==domain== ,B is called ==codomain==

!!! info "f(a)=b"
	
	- b is called the ==image== of a under f;
	
	- a is called a ==preimage== of b;

> [!NOTE] NOTATION!
> 
> $f(\emptyset)=\emptyset$
> 
> $f(\{a\})=\{f(a)\}$
> 
> $f(A\cup B)=f(A)\cup f(B)$
> 
> $f(A\cap B)\subseteq f(A)\cap f(B)$

**The graphs of functions:**
$\{(a,b)|a\in A \land f(a)=b\}$

### 几种函数类型
- **One-to-one** also injection 单射
	- $\forall a\forall b(f(a)=f(b)\to a=b)$
- **Onto** also surjection 满射
	- $\forall b\in B\  \exists a \in A(f(a)=b)$
- **One-to-one and Onto** or **one-to-one correspondence** also bijection 双射、一一对应

!!! note
	如果存在函数 f 使得A to B有个双射，则他们具有相同的势（cardinality）

如何证明函数 f 是单射或是满射？
![[单射满射的证明方法.png]]






**Monotonic Functions 单调函数**

- monotonically (strictly) increasing
	- $\forall x \forall y(x<y\to f(x)<f(y))$
- monotonically (strictly) decreasing
	- $\forall x \forall y(x<y\to f(x)>f(y))$

**Inverse Functions 逆函数**
Function f is invertible iff f is a ==bijection==.
$f^{-1}(y)=x\ iff\ f(x)=y$

### Some important functions
- **Floor function**
	- $\lfloor x\rfloor$
- **Ceiling function**
	- $\lceil x\rceil$

!!! info
	$\lfloor -x\rfloor =-\lceil x\rceil$
	
	$\lceil -x\rceil=-\lfloor x\rfloor$



## 2.4 Sequence and Summations 数列与求和

### Definition
A ==sequence== is a function from a subset of the set of integers (usually either the set {0, 1, 2, …} or the set {1, 2, 3, …}) to a set S. We use the notation $a_n$ to denote the image of the integer n. We call $a_n$ a term of the sequence. ($\{a_n\}$)

> [!NOTE] Some Useful Summation Formulae
> ![[数列和.png]]

## Cardinality of Sets
**[Definition 1](#)** The cardinality of a set A is equal to the cardinality of a set B, denoted | A | = | B |, iff there exists a bijection from A to B.

从A到B有双射，则A和B等势

**[Definition 2](#)** If there is an injection from A to B, the cardinality of A is less than or the same as the cardinality of B and we write  |A| ≤ |B|. 

从A到B有单射，则A的势小于等于B

!!! example "Prove that |(a,b)| = |(0,1)| in R"
	$$\begin{gather}\frac{x-a}{b-a}=\frac{y-0}{1-0}\newline Let\ f\ be\ function\ from\ A\ to\ B\ that\ y=f(x)=\frac{x-a}{b-a}\\ Then\ y\ is\ a\ bijection\ from\ (a,b)\ to\ (0,1) \end{gather} $$

**[Definition](#)** A set that is either finite or has the same cardinality as the set of positive integers called ==countable==.

任何有限的集合或者与整数集等势的集合都称作==countable==

When an infinite set S is countable, we denote the cardinality of S by $\aleph _0$ ( aleph null ).

If $|A|=|Z^+|$ , the set A is countable infinite.

[Example:](#)
	Show the set of positive rational numbers $|Q^+|=|Z^+|$

$$
\begin{gather}
\forall x \in Q^{+},x=\frac{p}{q},\ p,q\in Z^+
\\Let S=\{(p,q)|p,q\in Z^{+}\}=Z^{+}\times Z^{+}\\
\begin{cases} |Q^+|\le |S|\\|S|=|Z^+|\\|Z^+|\le |Q^+|
\end{cases}\Rightarrow |Q^+|=|Z^+|
\end{gather}
$$

所以，有理数集Q是可数的。

**[Theorem](#)** The union of a countable number of countable sets is countable.（可数个可数集的合集是可数的）

[Example:](#)
	Prove that the set of real number between 0 and 1 is uncoutable.

$$
\begin{gather}
A=\{x|x\in (0,1)\land x\in R\}\\
(1)|Z^+|\le|A|\\
B=\{\frac{1}{n+1}|n\in Z^+\}\\
\therefore |B|=|Z^+|\\
\because B\subseteq A\\
\therefore |Z^+|<=|A|\\
(2)|Z^+|\ne |A|\\
\end{gather}
$$


![[补充2.png]]

$$
\begin{cases} |Z^+|\le|A|\\|Z^+|\ne |A|
\end{cases}\Rightarrow |Z^+|<|A|
$$


### **[有趣的结论（其实是重要的结论）](#)** 有理数集是可数的，但是实数集不可数

!!! note
	$|R| =|R\times R|$

[Example:](#)
	Show that $|[0,1]|=|(0,1)|(Both\ Uncountable)$
	
$$
\begin{gather}
(1)B\subseteq A \Rightarrow |B|\le|A|\\
(2)Let\ g(x)=\frac{1}{2}x+\frac{1}{4},x\in [0,1]\\
for\ g(x)\ is\ a\ bijection\ from\ [0,1]\ to\ [\frac{1}{4} , \frac{3}{4}].\\
\therefore |A|\le|B|\\
Therefore\ |A|=|B|
\end{gather}
$$


[Theorem](#) |R|=|(0,1)|
	构造函数即可证明，如$f(x)=\frac{2}{\pi} \tan(x)$

[Theorem](#) The cardinality of the power set of an arbitrary set has a greater cardinality than the original arbitrary set.
即 $|P(S)|\ge |S|$
