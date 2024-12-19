
# 大数定律和中心极限定理

!!! quote "这章的题目其实是最简单的，只需要记住几个结论即可，写题都是转换成正态分布求解的"

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

### 马尔可夫不等式 *

若随机变量 $Y$ 的 $k$ 阶(原点)矩阵存在，则对任意 $\varepsilon \gt 0$ ，有：

$$
P\{|Y|\ge \varepsilon\} \le \frac{E(|Y|^k)}{\varepsilon ^k}
$$

亦可写作：

$$
P\{|Y|\lt \varepsilon\} \ge 1-\frac{E(|Y|^k)}{\varepsilon ^k}
$$

!!! warning "不在考察范围内"

### 切比雪夫不等式

设随机变量 $X$ 的数学期望和方差存在 , 分别记为 $\mu, \sigma ^2$ , 则对任意的 $\varepsilon \gt 0$, 有：

$$
P\{|X-\mu| \ge \varepsilon\} \le \frac{\sigma^2 }{\varepsilon^2}
$$

亦可写作：

$$
P\{|X-\mu| \lt \varepsilon\} \ge 1- \frac{\sigma^2 }{\varepsilon^2}
$$

!!! success "对样本均值使用切比雪夫不等式，由于 $D(\bar{X})= \frac{\sigma^2}{n}$，则 $P\{|\bar{X}-\mu| \ge \varepsilon\} \le \frac{\sigma^2 }{n\varepsilon^2}$"

<font style="font-weight: 1000;font-size: 20px" color="orange">例 1：</font>

**题面：** 总体服从指数分布 $E(0.1)$，抽取一个容量为 5 的样本，根据切比雪夫不等式来估计：

$$
P\{ |\bar{X} -10| \ge 10\} \le \frac{1}{n\varepsilon^2 \lambda^2} = \frac{1}{5}
$$

### 三个大数定律

设 $\{X_i, i\ge 1\}$ 为一随机变量序列，若存在常数序列 $\{c_n, n\ge 1\}$ 使得对于任意 $\varepsilon \gt 0$ 有以下成立：

$$
\lim _{n\rightarrow +\infty} P\{|\frac{1}{n}\sum_{i=1} ^n X_i -c_n|\ge \varepsilon \} = 0
$$

即 $n\rightarrow +\infty$ 时，有 $\frac{1}{n}\sum_{i=1}^n X_i -c_n \xrightarrow p 0$ ，则称随机变量序列 $\{X_i, i\ge 1\}$ 服从**弱大数定律**，简称大数定律。

实践中 , 人们发现大量测量值的算术平均具有一定的稳定性, 这一稳定性其实就是大数定律的客观背景. 最早的大数定律是著名的伯努利大数定律。

[定义] **伯努利大数定律：**

设 $n_A$ 为 $n$ 重伯努利试验中事件 $A$ 的发生次数，$p$ 为其每次试验中发生的概率，则对任意 $\varepsilon \gt 0$ 有：

$$
\lim _{n\rightarrow +\infty} P\{|\frac{n_A}{n}-p| \ge \varepsilon\} =0
$$

!!! note "$n_A/n$ 即事件A的频率"

[定义] **切比雪夫大数定律：**

设 $X_1, X_2, ...,X_n$ 相互独立，具有相同数学期望 $\mu$ 和方差 $\sigma^2$ 。则当 $n\rightarrow \infty$ 时：

$$
\frac{1}{n}\sum_{k=1}^n X_k \xrightarrow P \mu
$$

[定义] **辛钦大数定律：**

设 $X_1, X_2, ...,X_n$ 独立同分布，且 $E(X_i) =\mu$ ，则当 $n\rightarrow \infty$ 时：

$$
\frac{1}{n}\sum_{k=1}^n X_k \xrightarrow P \mu
$$

!!! danger "不是简单的切比雪夫大数定律的特例，因为方差可能不存在"

<font style="font-weight: 1000;font-size: 20px" color="orange">例 1：</font>

设 $X_1, X_2, ...,X_n$ 独立同分布，且 $X_i\sim U(0,1)$，则 $\sqrt[n]{X_1 X_2 ...X_n}$ 依概率收敛吗?

**解答：**

令 $Y_n = \sqrt[n]{X_1 X_2 ...X_n}, \ Z_n =\ln Y_n$ ，则 $Z_n= \frac{1}{n}(\ln X_1 +.. +\ln X_n)$

$$
E(\ln X_n)= \int ^1_0 \ln xdx= -1
$$

根据辛钦大数定律可得 $Z_n \xrightarrow p -1$ ，由依概率收敛的性质得到 $Y_n \xrightarrow p e^{-1}$

## 中心极限定理

[定义] **独立同分布的中心极限定理：**

设 $X_1, X_2, ...,X_n$ 独立同分布，$E(X_i)=\mu, D(X_i)=\sigma^2$，则对任意实数 $x$ :

$$
\lim _{n\rightarrow \infty} P(\frac{\sum_{i=1}^n X_i-n\mu }{\sqrt{n} \sigma}\le x)=\int ^x_{-\infty} \frac{1}{\sqrt{2\pi}}e^{-\frac{t^2}{2}}dt = \Phi(x)
$$

因此，当 $n$ **充分大**时:

$$\begin{array}c
\sum _{i=1}^n X_i \sim N(n\mu, n\sigma ^2)
\\ \frac{1}{n}\sum _{i=1}^n X_i \sim N(\mu, \sigma ^2/n)
\end{array}$$

[推论] **隶莫弗-拉普拉斯中心极限定理：**

设 $n_A$ 为在 n 重伯努利试验中事件 $A$ 发生的次数，$p$ 为事件 $A$ 在每次试验中发生的概率，即 $P(A)=p$，则对任意 $x\in R$ ：

$$
\lim_{n\rightarrow \infty} P(\frac{n_A -np}{\sqrt{np(1-p)}}\le x)=\int ^x_{-\infty} \frac{1}{\sqrt{2\pi}}e^{-\frac{t^2}{2}}dt = \Phi(x)
$$

即，当 $n$ 足够大时：

$$
B(n,p)\sim N(np, np(1-p))
$$

