
# 假设检验

## 基本思想

统计假设简称假设，通常用字母 $H$ 表示。一般我们同时提出两个完全相反的假设，习惯上把其中一个称为**原假设**或**零假设**，用 $H_0$ 表示；把另一个假设称为**备择假设**或**对立假设**，用 $H_1$ 表示。

- <1> $H_0: \theta \ge \theta_0,\ H_1: \theta \lt \theta_0$
	- 左侧检验
- <2> $H_0: \theta \le \theta_0, H_1: \theta \gt \theta_0$
	- 右侧检验
- <3> $H_0: \theta= \theta_0, \ H_1:\theta \ne \theta_0$
	- 双侧检验

在检验假设问题中，若寻找到某个统计量，其取值大小和原假设 $H_0$ 是否成立有密切联系，我们称之为该假设检验问题的**检验统计量**。对应于拒绝原假设的样本值范围称为**拒绝域** $W$，拒绝域的补集 $\bar{W}$ 称为接受域。

由于样本的随机性，任一检验规则在应用时都有可能发生错误判断：

|           | 原假设为真 | 原假设不真  |
| --------- | ----- | ------ |
| 根据样本拒绝原假设 | 第I类错误 | 正确     |
| 根据样本接受原假设 | 正确    | 第II类错误 |

- **第I类错误：** 拒绝真实的原假设（弃真）
	- $\alpha =P\{\text{第一类错误}\}=P\{\text{拒绝}H_0| H_0\text{为真}\}= P_{ H_0}\{\text{拒绝}H_0\}$
- **第II类错误：** 接受错误的原假设（存伪）
	- $\beta =P\{\text{第二类错误}\}=P\{\text{接受}H_0| H_0\text{为伪}\}= P_{ H_1} \{\text{接受} H_0\}$

!!! example "注意 $\alpha$ 与原假设的逻辑关系"
	- 若在显著性水平 $\alpha =0.01$ 情况下拒绝原假设 $H_0$，那么在 $\alpha=0.05$ 情况下一定拒绝 $H_0$
	- 若在显著性水平 $\alpha =0.05$ 情况下接受原假设 $H_0$，那么在 $\alpha=0.01$ 情况下一定接受 $H_0$

!!! info "奈曼-皮尔逊原则"
	首先控制犯第I类错误的概率不超过某一常数 $\alpha \in (0,1)$ ，再寻找检验，使犯第II类错误的概率最小。其中常数 $\alpha$ 称为**显著水平**，常取 0.01，0.05，0.1
	
	因此，假设 $H_0, H_1$ 并不对等，通常只有假设 $H_0$ 有显著差异时才会将其推翻。

下面，我们以课本例子 8.1.1 来演示如何计算错误判断的概率:

**题面** 随机抽取 9 位试验者服用减肥药，记录服用前后的体重差值为 $X$ (服药前-服药后)：

$$\begin{gather}
1.5 , &0.6, & -0.3, &  1.1, & -0.8, & 0, & 2.2, & -1.0, & 1.4
\end{gather}$$

假设 $X\sim N(\mu, 0.36)$，$\mu$ 未知。根据目前的样本资料，能否认为该减肥药有效？

我们对参数 $\mu$ 提出假设：

$$
H_0: \mu =0, \ \ H_1: \mu \gt 0
$$

!!! info "$\mu=0$ 即减肥药无效果，大于零则有效果"

由于样本均值是参数 $\mu$ 的无偏估计，则当 $\mu$ 越小，样本均值 $\bar{X}$ 也应该偏小才正确。我们根据此理论来指定规则：

$$\begin{array}l
\text{当} \bar{X}\ge C\text{时，拒绝原假设} H_0 \\
\text{当} \bar{X}\lt C\text{时，接受原假设} H_0 \\
\end{array}$$

对临界值 $C$ 的判断可由奈曼-皮尔逊原则计算，即使得第I类错误的概率为给定的显著水平：

$$
\alpha (C)=P\{\text{拒绝}H_0| H_0\text{为真}\} = P\{\bar{X}\ge C|\mu =0\} = P\left\{\frac{\bar{X}}{\sigma / \sqrt{n} } \ge   \frac{C}{\sigma / \sqrt{n}} |\mu=0\right\}
$$

由于假设原假设 $H_0$ 为真，可以将 $\mu=0$ 代入，即 $\frac{\bar{X}}{\sigma / \sqrt{n}} \sim N(0,1)$，则：

$$\begin{array}c
\alpha(C) =1- \Phi \left(\frac{C}{\sigma / \sqrt{n}}\right) \le 0.05
\\ C\ge \frac{z_{0.05} \sigma}{\sqrt{n}}= 0.329
\end{array}
$$

!!! note "这里我们取显著性水平为 0.05"

因此，该假设的拒绝域为 $W=\{\bar{X}\ge 0.329\}$，但题目中给出的样本均值为 $\bar{x}=0.522\gt 0.329$，即样本落入拒绝域。因此，我们有 $95\%$ 的把握拒绝原假设 $H_0$，即认为该减肥药有效果。

!!! quote
	![[正态假设检验总结.png]]

## 单个正态总体参数的假设检验

### 参数 $\mu$ 的假设检验

<font style="font-weight: 1000;font-size: 20px" color="red">若 $\sigma^2$ 已知</font>

先考虑双边假设问题:

$$
H_0: \mu =\mu_0 ,\ H_1: \mu \ne \mu_0
$$

其中 $\mu_0$ 是已知的常量，此时可取假设统计量为：

$$
Z=\frac{\bar{X}- \mu_0}{ \sigma / \sqrt{n}}
$$

若原假设成立，即 $\mu_0= \mu$ ，则 $Z\sim N(0,1)$ ，则在给定的显著水平 $\alpha$ 下，检验的拒绝域为：

$$
W=\left\{ |Z| = \left|\frac{\bar{X}- \mu_0}{ \sigma / \sqrt{n}}\right| \ge  z_{\alpha / 2}\right\}
$$

因此，若将样本均值 $\bar{x}$ 代入得到结果 $|z_0| =\left|\frac{\bar{x}- \mu_0}{ \sigma / \sqrt{n}}\right|\ge z_{ \alpha / 2}$ 时，我们有 $100(1-\alpha)\%$ 的把握拒绝原假设，即 $\mu\ne \mu_0$ 。

另一种判断方法是根据 P-值 进行判断:

$$
P值=P_{ H_0}\{|Z| \ge |z_0|\}= 2(1- \Phi (|z_0|)) \le \alpha
$$

即 P-值 小于等于 $\alpha$ 时拒绝原假设，否则不能拒绝原假设。

!!! tip "P-值可以记作接受原假设的概率，P-值越小，越拒绝原假设"

而对于左侧、右侧检验，其拒绝域和 P-值 分别为：

- 左侧检验
	- $W=\left\{Z=\frac{\bar{X}- \mu_0}{\sigma / \sqrt{n}} \le -z_{\alpha}\right\}$
	- $P值=P_{ H_0}\{Z\le z_0\}= \Phi( z_0)$
- 右侧检验
	- $W=\left\{Z=\frac{\bar{X}- \mu_0}{\sigma / \sqrt{n}} \ge z_{\alpha}\right\}$
	- $P值=P_{ H_0}\{Z\ge z_0\}= 1-\Phi( z_0)$

<font style="font-weight: 1000;font-size: 20px" color="red">若 $\sigma^2$ 未知</font>

实际应用中，总体方差常常未知，此时我们不能采用 Z 检验，需要用样本方差代替 $\sigma^2$ ：

$$
T=\frac{\bar{X}-\mu_0} {S  / \sqrt{n}}
$$

若原假设成立，即 $\mu_0= \mu$ ，则 $T\sim t(n-1)$ ，则在给定的显著水平 $\alpha$ 下，检验的拒绝域为：

$$
W=\left\{|T|=\left|\frac{\bar{X}-\mu_0} {S  / \sqrt{n}}\right|\ge t_{\alpha / 2} (n-1)\right\}
$$

对应的 P-值 为：

$$
P值=2 P\{t(n-1) \ge |t_0|\} \le \alpha
$$

- 左侧检验
	- $W=\left\{Z=\frac{\bar{X}- \mu_0}{S / \sqrt{n}} \le -t_{\alpha}(n-1)\right\}$
	- $P值=P\{t(n-1)\le t_0\}$
- 右侧检验
	- $W=\left\{Z=\frac{\bar{X}- \mu_0}{S / \sqrt{n}} \ge t_{\alpha}(n-1)\right\}$
	- $P值=P\{t(n-1)\ge t_0\}$

### 参数 $\sigma^2$ 的假设检验

我们不妨认为参数 $\mu$ 未知，其假设问题包括：

- **双边假设：** $H_0: \sigma^2 =\sigma_0^2, \ H_1: \sigma^2 \ne \sigma_0^2$
- **左侧检验：** $H_0: \sigma^2 \ge\sigma_0^2, \ H_1: \sigma^2 \lt \sigma_0^2$
- **右侧检验：** $H_0: \sigma^2 \le \sigma_0^2, \ H_1: \sigma^2 \gt \sigma_0^2$

其中 $\sigma_0^2$ 是已知的常量，若有 $\sigma_0^2 =\sigma^2$，则检验统计量 $\chi^2$ 服从 $\chi^2 (n-1)$ 分布：

$$
\chi^2 =\frac{(n-1)S^2} {\sigma_0^2 } \sim \chi^2(n-1)
$$

在给定显著水平 $\alpha$ ，以及值 $p_0= P\{\chi^2(n-1) \le \chi_0^2\}$

- **双边检验：**
	- $W=\{\chi^2 \ge \chi^2 _{\alpha / 2}(n-1) \text{ or }\chi^2 \le  \chi^2\le \chi^2_{1- \alpha / 2} (n-1)\}$
	- $P值=2 \min (p_0, 1-p_0)$
- **左侧检验：**
	- $W=\{\chi^2\le \chi^2_{1- \alpha} (n-1)\}$
	- $P值= p_0$
- **右侧检验：**
	- $W=\{\chi^2 \ge \chi^2 _{\alpha}(n-1)\}$
	- $P值=1- p_0$

## 两个正态总体参数的假设检验

### 比较两个正态总体均值的假设检验

实际上和一个正态总体的情况相同：

$$
\bar{X}-\bar{Y} \sim N(\mu_1 -\mu_2, \frac{\sigma_1^2}{n}+ \frac{\sigma_2^2}{n})
$$

我们考虑双侧假设问题：

$$
H_0: \mu_1 =\mu_2,\ H_1: \mu_1 \ne \mu_2
$$

<font style="font-weight: 1000;font-size: 20px" color="red">若 $\sigma_1^2, \sigma_2^2$ 已知</font>

当假设 $H_0$ 成立时，$\frac{\bar{X}-\bar{Y}}{\sqrt{\frac{\sigma_1^2}{ n_1} + \frac{\sigma_2^2}{ n_2}}} \sim N(0,1)$ ，采用 Z 检验可得拒绝域为：

$$
W=\left\{\frac{|\bar{X}-\bar{Y}|}{\sqrt{\frac{\sigma_1^2}{ n_1} + \frac{\sigma_2^2}{ n_2}}} \ge z_{\alpha / 2}\right\}
$$

P-值仍为：

$$
P值=2\min (p_0, 1-p_0)=2 (1-\Phi(|z_0|))
$$

<font style="font-weight: 1000;font-size: 20px" color="red">若 $\sigma_1^2= \sigma_2^2$ 但未知</font>

首先取参数 $\sigma_1^2 =\sigma_2^2= \sigma^2$ 的无偏估计量：

$$
S_{\omega}^2 = \frac{(n_1 -1) S_1^2 + (n_2-1) S_2^2}{ n_1 +n_2 -2}
$$

取检验统计量为：

$$
T=\frac{\bar{X} -\bar{Y}}{ S_\omega \sqrt{\frac{1}{n_1} + \frac{1}{n_2}}}
$$

当原假设 $H_0$ 成立时，$T\sim t(n_1 +n_2 -2)$，则检验的拒绝域为：

$$
W=\{|T| \ge t_{\alpha / 2}(n_1+ n_2 -2)\}
$$

P-值为：

$$
P值= 2P\{t(n_1 +n_2 -2) \ge  |t_0|\}
$$

### 比较两个正态总体方差的检验

有时候，在检验两正态总体均值前，需要先对正态总体方差是否相等进行检验，我们考虑下面的假设问题：

$$
H_0: \sigma_1^2 =\sigma_2 ^2, \ H_1: \sigma_1 ^2 \ne \sigma_2^2
$$

取检验统计量为：

$$
F=\frac{S_1^2}{ S_2^2} 
$$

当原假设 $H_0$ 成立时，$F\sim F(n_1 -1, n_2-1)$，检验的拒绝域为：

$$
W=\{F\ge F_{\alpha / 2} (n_1 -1, n_2 -1)\ \text{or}\ F\le F_{1- \alpha / 2} (n_1 -1, n_2-1 )\}
$$

设 $p_0 = P\{F(n_1-1, n_2-1) \le f_0\}$，则：

- **双边检验：** $P值=2\min \{p_0 , 1-p_0\}$
- **左侧检验：** $P值= p_0$
- **右侧检验：** $P值= 1- p_0$

