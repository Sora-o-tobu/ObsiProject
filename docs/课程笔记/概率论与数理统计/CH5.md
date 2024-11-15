
# 大数定律和中心极限定理

大数定律及中心极限定理这两类是极限定理中的基本理论。

- 大数定律主要探讨随机变量序列的平均在一定条件下的稳定性规律
- 中心极限定理的主要研究内容是大量的随机变量之和的分布在一定条件下可以用正态分布去逼近


## 大数定律

### 概率意义下的极限定义

设 $\{Y_n, n\ge 1\}$ 为一随机变量序列，$c$ 为一常数。若对任意的 $\varepsilon \gt 0$ ，都有：

$$
\lim _{n\rightarrow +\infty} P\{|Y_n  -c|\ge \varepsilon\} = 0
$$

则称 $\{Y_n, n\ge 1\}$ **依概率收敛**于 $c$，记为 $Y_n \xrightarrow P c, n\rightarrow +\infty$ 。

显然，上式亦可表示为：

$$
\lim _{n\rightarrow +\infty} P\{|Y_n  -c|\lt \varepsilon\} = 1
$$

对于多元变量，依概率收敛有如下性质：

$$\begin{array}c
\text{若} X_n\xrightarrow p a, Y_n\xrightarrow p b, n\rightarrow +\infty, \text{且} g(x,y) \text{在} (a,b) \text{处连续，则有：} \\
g(X_n, Y_n)\xrightarrow p g(a,b)
\end{array}
$$

### 马尔可夫不等式

若随机变量 $Y$ 的 $k$ 阶(原点)矩阵存在，则对任意 $\varepsilon \gt 0$ ，有：

$$
P\{|Y|\ge \varepsilon\} \le \frac{E(|Y|^k)}{\varepsilon ^k}
$$

亦可写作：

$$
P\{|Y|\lt \varepsilon\} \ge 1-\frac{E(|Y|^k)}{\varepsilon ^k}
$$

### 切比雪夫不等式

设随机变量 $X$ 的数学期望和方差存在 , 分别记为 $\mu, \sigma ^2$ , 则对任意的 $\varepsilon \gt 0$, 有：

$$
P\{|X-\mu| \ge \varepsilon\} \le \frac{\sigma^2 }{\varepsilon^2}
$$

亦可写作：

$$
P\{|X-\mu| \lt \varepsilon\} \ge 1- \frac{\sigma^2 }{\varepsilon^2}
$$

### 两个大数定律

设 $\{X_i, i\ge 1\}$ 为一随机变量序列，若存在常数序列 $\{c_n, n\ge 1\}$ 使得对于任意 $\varepsilon \gt 0$ 有以下成立：

$$
\lim _{n\rightarrow +\infty} P\{|\frac{1}{n}\sum_{i=1} ^n X_i -c_n|\ge \varepsilon \} = 0
$$

即 $n\rightarrow +\infty$ 时，有 $\frac{1}{n}\sum_{i=1}^n X_i -c_n \xrightarrow p 0$ ，则称随机变量序列 $\{X_i, i\ge 1\}$ 服从**弱大数定律**，简称大数定律。

实践中 , 人们发现大量测量值的算术平均具有一定的稳定性, 这一稳定性其实就是大数定律的客观背景. 最早的大数定律是著名的伯努利大数定律。

设 $n_A$ 为 $n$ 重伯努利试验中事件 $A$ 的发生次数，$p$ 为其每次试验中发生的概率，则对任意 $\varepsilon \gt 0$ 有：

$$
\lim _{n\rightarrow +\infty} P\{|\frac{n_A}{n}-p| \ge \varepsilon\} =0
$$

