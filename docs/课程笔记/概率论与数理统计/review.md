# 错题本

## Chapter 1

### 独立性

已知 $0\lt P(A)P(B)\lt 1$ ，则以下条件成立时 $A,B$ 一定独立：

$$
P(\bar{A}|B)= P(\bar{A}|\bar{B})
$$

**解答：**

将 $P(\bar{B}) = 1-P(B)$ 带入可得到 $P(\bar{A}B) =P(\bar{A})P(B)$

### 右连续边界的概率

$$
F(x)=\begin{cases}0, & x\lt 1 \\ \frac{x}{4}, & 1\le x\lt 2  \\
 1, & x\le 2
\end{cases}
$$

求 $P(X=2)$

**解答：**

因为是**右连续**，所以：

$$
P(X=2)=F(x)-F(x-0)|_{x=2}= \frac{1}{2}
$$

## Chapter 3

### 离散型和连续型的乘积可以用全概率公式

**题面：**

$X\sim N(0,1), Y$的取值是$\pm 1$，且$P\{Y=1\}=p$，若 $X,Y$ 相互独立，求证 $Z=X\cdot Y \sim N(0,1)$ 

**解答：**

$$\begin{array}c
F_Z(t) = P(XY\le t)=P(Y=1)P(X\le t)+P(Y=-1)P(X\ge -t)\\
 =P(Y=1)F_X(t) +P(Y=-1) (1-F_X(-t)) \\
 =pF_X(t) +(1-p) F_X(t) = F_X(t)
\end{array}
$$




## Chapter 4

### 对于有放回，从单个事件推广到整体

**题面：**

有 n 张各不相同的卡片,采用有放回抽样,每次取一张,共取 n 次,则有些卡片会被取到,甚至被取到很多次,但有些卡片可能不曾被取到.设这 n 张卡片中被取到的共有 X 张,计算 $E(X)$,并计算当 $n \rightarrow +\infty$ 时，$E(X/n)$的极限：

**解答：**

定义指示变量 \( I_i \)，表示第 \( i \) 张卡片是否被抽到过：

\[
I_i = \begin{cases} 
1 & \text{第 \( i \) 张卡片被取到至少一次} \\
0 & \text{否则}
\end{cases}
\]

则被抽到的卡片总数 \( X \) 可以表示为这些指示变量的和：

\[
X = \sum_{i=1}^n I_i
\]

现在计算 \( E(X) \)，即 \( E\left( \sum_{i=1}^n I_i \right) \)。根据期望的线性性，我们有：

\[
E(X) = \sum_{i=1}^n E(I_i)
\]

要计算 \( E(I_i) \)，我们先考虑第 \( i \) 张卡片在 \( n \) 次抽取中都没有被抽到的概率。每次抽到该卡片的概率为 \( \frac{1}{n} \)，因此第 \( i \) 张卡片在某次抽取中不被取到的概率为 \( 1 - \frac{1}{n} \)。因此，第 \( i \) 张卡片在 \( n \) 次抽取中从未被取到的概率为：

\[
P(I_i = 0) = \left(1 - \frac{1}{n}\right)^n
\]

于是，第 \( i \) 张卡片至少被抽到一次的概率为：

\[
P(I_i = 1) = 1 - \left(1 - \frac{1}{n}\right)^n
\]

所以，

\[
E(I_i) = P(I_i = 1) = 1 - \left(1 - \frac{1}{n}\right)^n
\]

代入回 \( E(X) \) 的表达式：

\[
E(X) = \sum_{i=1}^n \left(1 - \left(1 - \frac{1}{n}\right)^n\right) = n \left(1 - \left(1 - \frac{1}{n}\right)^n\right)
\]


则 \( E(X/n) \) 的极限：

\[
\lim_{n \to +\infty} \frac{E(X)}{n} = \lim_{n \to +\infty} \left(1 - e^{-1}\right) = 1 - e^{-1}
\]

因此，当 \( n \to +\infty \) 时，\( E(X/n) \) 的极限为：

\[
\lim_{n \to +\infty} E(X/n) = 1 - e^{-1} \approx 0.632
\]

## Chapter 5

### 随机变量乘积的方差

**题面：**

设随机变量 $X_i$ 的密度函数为：

$$
f_i(X)=\begin{cases} \frac{i|x|^{i-1}}{2}, & |x|\le 1  \\
0, & \text{其他} \end{cases} ,\ \ \ i=1,2,3,...,n
$$

且 $X_1, X_2, ...,X_n$ 相互独立。令 $Y_n = \prod_{i=1}^n X_i$ ，用切比雪夫不等式求使得 $P\{|Y_n| \ge \frac{1}{2}\} \le \frac{1}{9}$ 成立的最小 $n$ 。

**解答：**

对于任意 $i$ ，由于 $xf_i(x)$ 为奇函数，所以 $E(X_i)=0$ ，同时有 $E(Y_n) =0$。

当 $X_i, X_j$ **相互独立时**，有引理：

$$
Var(X_i\cdot X_j)=Var(X_i)Var (X_j) +Var(X_i) E(X_j)^2 +Var(X_j) E(X_i)^2
$$

由于本题中 $E(X_i) =E(X_j) =0$

所以有：

$$
Var(X_i\cdot X_j) =Var(X_i)\cdot Var(X_j)
$$

又：

$$
\begin{array}l Var(X_i)  & =\int _{-1}^1 (x-0)^2f(x)dx 
\\ & = \int_{-1}^1 x^2 \frac{i|x|^{i-1}}{2}dx 
\\ & = \int _0^1 ix^{i+1}dx
\\ & = \frac{i}{i+2}
\end{array}$$

所以：

$$
Var(Y_n)= \prod_{i=1}^n X_i = \frac{1}{1+2} \cdot \frac{2}{2+2} \cdot \frac{3}{3+2}... \frac{n}{n+2} = \frac{2}{(n+1)(n+2)}
$$

带入切比雪夫不等式：

$$
P\left(|Y_n| \ge \frac{1}{2}\right) \le \frac{2\cdot 2^2}{(n+1)(n+2)} \le \frac{1}{9}
$$

得 $n$ 最小值为 7。

### 问有多少把握，肯定是转换成正态看

**问题：**

随机变量 $X$ 满足三角分布：

$$
f(x)=\begin{cases}x, & 0\le x \lt 1 \\ 2-x, & 1\le x\ \lt 2 \\ 0, & \text{其他} \end{cases}
$$

要保证至少有 $95\%$ 把握使事件 $\{\frac{1}{2} \lt X \lt \frac{3}{2}\}$ 出现次数不少于 80 ，则需要至少多少次观察？

**解答：**

$$\begin{array}c
q=P\left(\frac{1}{2} \lt X\lt \frac{3}{2}\right)=\frac{3}{4} \\
\text{随机变量Y为n次试验中满足条件的次数} \\
\text{则 } E(Y)=nq, D(Y)=nq(1-q) \\
Y\sim N(nq, nq(1-q)) \\
95\% \le P(Y\ge 80) =1- \Phi\left(\frac{80-nq}{\sqrt{nq(1-q)}}\right)\\
\text{解得}  n=117
\end{array}$$

