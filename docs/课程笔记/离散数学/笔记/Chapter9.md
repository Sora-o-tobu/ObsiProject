# CH 9 : Relations

## 9.1 Relations and Their Properties

### 定义

A ==binary relation==(二元关系) R from set $A$ to set $B$ is a subset of $A\times B$

!!! note
	
	**A binary relation R is a set**
	
	$R\subseteq A\times B$
	
	$R=\{(a,b)|a\in A ,b\in B ,aRb\}$

!!! note "Functions As Relations"
	The graph of function 是从A到B的一个relation，但是relation不一定是function，因为一个x可能对应多个y

A relation of set A is a relation from A to A.

### 表示方式

- list its all ordered pairs 列举所有pairs
- using a set build notation/specification by predicates 利用条件语句的set
- 2D Table
- Connection Matrix / Zero-one Matrix
- Digraph

#### 2D Table
![[二维表表示relation.png]]

#### Connection Matrices
![[矩阵表示Relation.png]]

因此，binary relations on a set A with n elements 共有$2^{n^2}$个

#### Digraph
![[有向图表示Relation.png]]

[例子](#)
![[有向图表示Relation例子.png]]


### Special Properties of Binary Relations

- **Reflexive** 自反性
- **Irreflexive** 反自反性
- **Symmetric** 对称性
- **Antisymmetric** 反对称性
- **Transitive** 传递性

[Definition](#) A relation R on a set A is ==reflexive== if
$$
\forall x(x\in A\to (x,x)\in R)
$$

对于任意A中元素，(x,x)都在R中

因此，若用矩阵表示，矩阵正对角线应当全部为1；若用有向图表示，每个vertex都应该有个loop指向自己

自反关系个数为$2^{n ^2-n}$

--- 
[Definition](#) A relation R on a set A is ==irreflexive== if
$$
\forall x(x\in A \to (x,x)\notin R)
$$
对于任意A中元素，(x,x)都不在R中

因此，若用矩阵表示，矩阵正对角线应当全部为0；

存在relation既不是reflexive也不是irreflexive:
![[既不自反也不反自反的relation.png]]

反自反关系个数为$2^{ n^2-n}$

因此既不是自反，也不是反自反的relation共有$2^{n ^2} - 2*2^{ n^2-n}$ 个

--- 
[Definition](#) A relation R on a set A is ==symmetric== if
$$
\forall x\forall y((x,y)\in R\to(y,x)\in R)
$$

用矩阵表示，则矩阵关于对角线对称

对称关系的个数为$2^n \cdot 2^{\frac{ n^2-n}{2}} = 2^{\frac{ n^2+n}{2}}$

--- 
[Definition](#) A relation R on a set A is ==antisymmetric== if
$$
\forall x\forall y((x,y)\in R \land (y,x)\in R \to x=y)
$$

注意，只是关于对角线不存在两个都为1的情况，(0,0)是允许的

因此，也可以这样表示关系:
$$
\forall x\forall y((x,y)\in R\land x\ne y\to (y,x)\notin R)
$$

![[Antisymmetric的矩阵表示.png]]

反对称关系的个数为$2^ n\cdot 3^{\frac{ n^2-n}{2}}$

---
[Definition](#) A relation R on a set A is ==transitive== if
$$
\forall x\forall y\forall z((x,y)\in R\land (y,z)\in R\to (x,z)\in R)
$$

用 $m_{ij}$ 表示矩阵第i行第j列的值，则有 $\overline{m_{ij}\land m_{jk}}\lor m_{ik}=1$

!!! note "判别特征的例子"
	![[判别特征的例子1.png]]
	![[判别特征例子2.png]]

!!! note 
	有symmetric和transitve不能推导出reflexive

### Combining Relations

- Set operation
- Composition
- Inverse relation

#### Set operation
![[setoperation1.png]]

#### Composition
![[Composition1.png]]

!!! note "注意，R是前面那个"
	![[compositon例子.png]]

利用矩阵乘法可以求Composition
![[矩阵求Composition.png]]

利用有向图求Composition
![[利用有向图求Compositon.png]]

[Definition](#)
Let R be a relation on the set A. The powers $R^n,n=1,2,3,...$  are defined recursively by
$$
R^1 =R,and\ R^{n+1}= R^n\circ R
$$


[Theorem](#)
The relation R on a set A is ==transitive== if and only if $R^n\subseteq R,for \ n=1,2,3,...$


!!! note "对称关系的n次方还是对称的"
	![[对称关系的n次方也是对称的.png]]

#### Inverse relation
![[关系转置1.png]]

!!! note ""
	![[relation恒等式.png]]



## 9.4 Closures of Relations

- Reflexive Closure
- Symmetric Closure
- Transitive Closure

[Definition](#) ==The closure of relaitons R with respect to property P== is the relation S with respect to property P containing R such that S is a subset of every relation with property P containing R.

关系R关于性质P的闭环，就是一个具有性质P且包含R的关系S。

### Reflexive Closure

[Theorem](#) 
Let R be a relation on A . The ==reflexive closure== of R , denoted by $r(R)$ , is  $R\cup I_A\ \ I_A=\{(x,x)|x\in A\}$

!!! info "证明需要证明Closure的三个性质"
	1. 包含R  2. 是Reflexive的关系 3. 是包含R的最小Reflexive关系
	![[ReflexiveClosure证明.png]]


- 用集合表示的话，就是往R里面加入所有不在R内的$(a,a)\ where\ a\in A$
- 用有向图表示的话，则是给每一个Vertex添加loop
- 用connection matrix表示则在主对角线上全置1

!!! example
	**Question：** $R=\{(a,b)|a<b,a,b\in Z\}$ what is $r(R)$?
	
	$r(R)=R\cup I_A =\{(a,b)|a<b,a,b\in Z\} \cup \{(a,a)|a\in Z\}$
	
	$=\{(a,b)|a\le b,a,b\in Z\}$


### Symmetric Closure

[Theorem](#) 
Let R be a relation on A . The ==symmetric closure== of R , denoted by $s(R)$ , is $R\cup R^{-1}$


!!! info "证明需要证明Closure的三个性质"
	1. 包含R  2. 是Symmetric的关系 3. 是包含R的最小Symmetric关系
	![[SymmetricClosure证明1.png]]
	![[SymmetricClosure证明2.png]]


- 用集合表示的话，如果$(a,b)$在R内且$(b,a)$不在R内，则加上
- 用有向图表示的话，如果存在一个edge从 y 到 x，再加上一个从 x 到 y 的edge
- 用connection matrix表示则  $M_{s(R)} =M_R \lor M_R^T$

### Transitive Closure

先导知识:

- 有向图中一段长度为n的path可以用sequence表示:$(x_0 ,x_1),( x_1, x_2),...,(x_{n-1} ,x_{n})$
	- 也可以简写成 $x_0 ,x_1 ,x_2 ,...,x_{n-1} ,x_{n}$
- 如果 $x_0= x_n\ (n\ge 1)$，则称这条path为==Cycle==或者==Circuit== 
	- $|A|=n\Rightarrow Any\ path\ of\ length\ >n\ must\ contain\ a\ cycle$

[Theorem](#)
Let R be a relation on A . There is a path of length n from a to b if and only if $(a,b)\in R^n$

可以用数学归纳法证明：
$(a,x)\in R\ ,\ (x,b)\in R^{n-1} \Rightarrow (a,b)\in R^{n}$

[Definition](#)
The ==connectivity relation== denoted by $R^*$ , is the set of ordered pairs (a,b) such that there is a path (in R) from a to b : $R^* =\bigcup_{n=1}^{\infty} R^n$ and $t(R)=R^*$

[Corollary](#)
If $|A|=n$ , then $t(R)=R^* =R\cup R^2\cup ...\cup R^n$








