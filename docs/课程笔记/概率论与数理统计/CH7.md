
# 参数估计

## 点估计

设总体的分布函数为 $F(x;\theta)$ ，点估计问题通过构造一个适当的统计量 $\hat{\theta} (X_1, X_2,..., X_n)$ 来估计总体的参数 $\theta$ 。我们称  $\hat{\theta} (X_1, X_2,..., X_n)$ 为 $\theta$ 的点估计量，若以具体值带入，则称 $\hat{\theta} (x_1, x_2,..., x_n)$ 为 $\theta$ 的一个估计值。

### 矩法

依据大数定律，即当样本容量 $n\rightarrow +\infty$ 时，样本矩依概率收敛于相应的总体矩，即：

$$
A_k \xrightarrow p \mu_k, \ B_k \xrightarrow p v_k
$$

其中 $A_k, B_k$ 分别为样本的k阶原点矩和k阶中心矩，$\mu_k, v_k$ 分别为总体的k阶原点矩和k阶中心矩。

利用矩法计算参数估计量的基本步骤为：

<1> 求矩关于参数的函数：

$$
\mu_i = E(X^i) =h_i( \theta_1,..., \theta_k),\ \  i=1,...,k
$$

<2> 求各参数关于k阶矩的反函数：

$$
\theta_i= g_i( \mu_1, ...,\mu_k),\ i=1,...,k
$$

<3> 以样本各阶矩 $A_1,... ,A_k$ 代替总体各阶矩 $\mu_1, ..., \mu_k$ 计算各参数的矩估计：

$$
\hat{\theta} = g_i( A_1, ..., A_k)
$$

!!! warning "上面只是用了原点矩，写题中还经常用到中心矩计算，原理和步骤相同"

<font style="font-weight: 1000;font-size: 20px" color="orange">例 1：</font>

设总体 $X\sim U(a,b)$ ，a,b 为未知参数，样本 $X_1,..., X_n$ ，求 a 和 b 的矩估计。

**解答：**

<1> 求矩关于参数的函数：

$$
\mu_1 =E(X)= \frac{a+b}{2},\ \ v_2=Var(X) = \frac{(a-b)^2}{12}
$$

<2> 求各参数关于k阶矩的反函数：

$$
a=\mu_1 -\sqrt{3v_2}, \ \ b=\mu_1 +\sqrt{3v_2}
$$

<3> 以样本原点矩 $A_1= \bar{X}$ 代替总体各阶矩 $\mu_1$，样本中心矩 $B_2 = \frac{1}{n}\sum_{i=1}^n (X-\bar{X}) ^2$ 代替总体中心矩 $v_2$ ，得到参数 a,b 的矩估计：

$$
\hat{a} = \bar{X} -\sqrt{3B_2},\ \ \hat{b} = \bar{X} + \sqrt{3B_2}
$$

### 最大似然法

极大似然法的基本思想: 设某事件 A 发生的概率依赖于待估参数 $\theta$ , 如果观察到 A 已经发生, 那么就取使得事件 A 发生的概率达到最大的 $\theta$ 的值作为 $\theta$ 的估计。

当 $X$ 为连续型总体时，设其密度函数为 $f(x;\theta)$ ，$\theta \in \Theta$ 是未知的待估参数，$X_1,..., X_n$ 是来自总体的随机样本，并设 $x_1,..., x_n$ 是已得到的样本值，则似然函数定义为：

$$
L(\theta) = L(\theta ; x_1, x_2,..., x_n) =\prod_{i=1} ^n f(x_i;\theta)
$$

形式与样本的联合密度函数相同 $f(x_1, x_2,..., x_n; \theta)$ 相同。参数的最大似然估计值为似然函数取最大值的情况：

$$
L(\hat{\theta}) =L(\hat{\theta} ; x_1,..., x_n) = \max_{\theta\in \Theta} L(\theta; x_1,..., x_n)
$$

称相应的统计量 $\hat{\theta}(X_1, ...,X_n)$ 为 $\theta$ 的最大似然估计量，简称 **MLE**。

!!! info "对于离散型总体"
	$$L(\theta) = L(\theta ; x_1, x_2,..., x_n) =\prod_{i=1} ^n p (x_i;\theta)$$


寻求最大似然估计时常用微分法，有**似然方程**：

$$
\frac{dL(\theta)}{d\theta}|_{\theta = \hat{\theta}} =0
$$

为了计算方便，往往对似然方程取对数，记为**对数似然函数**：

$$
l(\theta) =\ln L(\theta)
$$

则上式等价于：

$$
\frac{dl(\theta)}{d\theta} |_{\theta = \hat{\theta}} = 0
$$

!!! note "若有多个待估参数，则需要对每个参数分别求偏导"

!!! danger "有时候遇见求导无零点，则求最大值对应的点即可，但是要注意 $\theta$ 的取值范围"
	$$\begin{array}c X\sim U(0,\theta) \\ L(\theta) = \begin{cases} \frac{1}{\theta ^n}, & 0\le x_1,...,x_n \le \theta \\ 0, & \text{其他}\end{cases} \\ \text{参数越小，似然方程越大，但是有 } \theta \ge \max (x_1, ...,x_n) \\ \Rightarrow \hat{\theta} = \max (x_1,..., x_n)\end{array}$$

<font style="font-weight: 1000;font-size: 20px" color="orange">例 1：</font>

设总体服从泊松分布 $X\sim P(\lambda)$ ，其中 $\lambda$ 是未知参数，若 $X_1, X_2, ...,X_n$ 是来自总体的样本，求参数 $\lambda$ 的极大似然估计量。

**解答：**

$$\begin{array}c
L(\lambda) =\prod _{i=1}^n p(x_i; \lambda) =\prod_{i=1}^n \frac{\lambda ^{x_i}}{ x_i!}e^{-\lambda} = \frac{\lambda ^{\sum_{i=1} ^n x_i}}{ \prod_{i=1}^n x_i!} e^{-n\lambda} \\
\Rightarrow l(\lambda) = \left(\sum_{i=1}^n x_i \right)\ln \lambda -\sum_{i=1}^n x_i! -n\lambda \\
\frac{dl(\lambda)}{d\lambda}|_{\lambda=\hat{\lambda}} = 0,\ \ \text{此方程有唯一解：} \\
\hat{\lambda} = \frac{1}{n} \sum_{i=1}^n X_i = \bar{X}
\end{array}$$

<font style="font-weight: 1000;font-size: 20px" color="orange">例 2：</font>

设总体服从正态分布 $X\sim N(\mu, \sigma^2)$ ，求未知参数 $\mu, \sigma^2$ 的极大似然估计量:

![[正态分布似然估计计算.png]]

!!! warning "实际上还要对似然方程求二阶导看是否小于0，以确定这是极大值"

[极大似然估计的不变原理](#) 已经估计出参数，则对其统计量的极大似然估计可以直接带入这些参数。

例如，均分分布 $E(X)$ 的极大似然估计为 $\frac{\hat{a} +\hat{b}}{2}$。

## 估计量的评价准则

### 无偏性

估计量本身是统计量，其取值随着样本值改变而改变，因此我们不能根据某次抽样的结果来衡量估计量的好坏。一个自然评价标准是要求估计量无系统偏差，即要求在大量重复抽样时，所有估计值的平均值应与待估参数的真值相同，这就是**无偏性准则**。

设 $\theta \in \Theta$ 是总体的待估参数，$X_1, X_2,..., X_n$ 是来自总体的样本，若估计量 $\hat{\theta}$ 的数学期望存在，且满足：

$$
E(\hat{\theta}) = \theta , \ \ \  \forall \theta \in \Theta 
$$

称 $\hat{\theta}$ 为 $\theta$ 的无偏估计。

如果 $\hat{\theta}$ 是一个有偏估计，且有 $E(\hat{\theta})=a\theta +b$ ，则我们可以对其进行无偏纠正得到 $\theta$ 的无偏估计：

$$
\frac{1}{a}(\hat{\theta} - b)
$$

!!! info 
	- 若 $E(\hat{\theta}) \ne \theta$ ，则称 $E(\hat{\theta}) - \theta$ 为估计量 $\hat{\theta}$ 的**偏差**
	- 若 $E(\hat{\theta}) \ne \theta$ ，但 $\lim _{n\rightarrow +\infty}E(\hat{\theta}) = \theta$ ，则称 $\hat{\theta}$ 是 $\theta$ 的**渐近无偏估计**

- 样本均值 $\bar{X}$ 和样本方差 $S^2$ 分别是 $\mu$ 和 $\sigma^2$ 的**无偏估计**
- 样本二阶中心距 $B_2$ 是 $\sigma^2$ 的**渐近无偏估计**

### 有效性

有些情况下，同一总体参数可能有多个无偏估计量，为比较无偏估计量的好坏，我们需进一步考察估计量取值的方差。估计量方差越小，说明该估计量的取值越集中在参数真值附近。

设 $\hat{\theta_1} =\hat{\theta_1} (X_1, X_2, ...,X_n)$ 与 $\hat{\theta_2} =\hat{\theta_2} (X_1, X_2, ...,X_n)$ 都是参数 $\theta$ 的无偏估计，若 $\forall \theta \in \Theta$，$Var_\theta( \hat{\theta_1}) \le Var_\theta (\hat{\theta_2})$，且至少有一个 $\theta \in \Theta$ 使不等号成立，则称 $\hat{\theta_1}$ 比 $\hat{\theta_2}$ **有效**。



还是对均匀分布 $U(0,\theta)$ 的参数 $\theta$ 进行分析：

- **矩估计：** $\hat{\theta} = 2\bar{X}$
- **最大似然估计：** $\hat{\theta} = \max (X_1, ...,X_n)$

**无偏性：**

- **矩估计：** $E(\hat{\theta}) = 2 E(\bar{X}) = \theta$ 
	- 所以是参数 $\theta$ 的无偏估计，不需要纠正
- **最大似然估计：** $E(\hat{\theta}) = \frac{n}{n+1}\theta$
	- 所以是参数 $\theta$ 的有偏估计，需要纠正为 $\frac{n+1}{n}\hat{\theta}$

**有效性：**

- **矩估计：** $D(\hat{\theta})= 4\cdot \frac{\sigma^2}{n} = \frac{\theta^2}{3n}$
- **最大似然估计：** $D(\hat{\theta}) = \frac{\theta^2}{n(n+2)}$

所以最大似然估计的有效性更好。

## 区间估计

人们常常根据点估计对总体参数作出判断，但这种判断的把握有多大？可信度有多高？点估计无法回答这些问题。统计学家为了弥补此种不足，提出了区间估计。

