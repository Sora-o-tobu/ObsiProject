# 错题本

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