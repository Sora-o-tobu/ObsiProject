
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

### 均方误差

设 $\hat{\theta} = \hat{\theta} (X_1, X_2,..., X_n)$ 是总体参数的估计量，称 $E[(\hat{\theta} - \theta)^2]$ 是估计量 $\hat{\theta}$ 的**均方误差**，记为 $Mse(\hat{\theta})$ 。

设 $\hat{\theta}_1$ 和 $\hat{\theta}_2$ 都是 $\theta$ 的估计量，若对于任意 $\theta \in \Theta$，$Mse(\hat{\theta}_1) \le Mse(\hat{\theta}_2)$ ，且存在某个 $\theta$ 使不等号成立，则称在均方误差准则下 $\hat{\theta}_1$ 优于 $\hat{\theta}_2$ 。

!!! info "无偏估计量的均方误差"
	对于无偏估计量，$Mse(\hat{\theta})= Var(\hat{\theta})$。因此，对于两个无偏估计量来说，均方误差准则等价于有效性准则

### 相合性

设 $\hat{\theta} = \hat{\theta} (X_1, X_2,..., X_n)$ 是总体参数的估计量，若对任意 $\varepsilon \gt 0$，有：

$$
\lim _{n \to +\infty} P\{ |\hat{\theta}_n -\theta|\lt \varepsilon\} =1
$$

即 $\hat{\theta}_n$ 依概率收敛于 $\theta$ ，则称 $\hat{\theta}_n$ 是 $\theta$ 的**相合估计量**，并记为 $\hat{\theta}_n \xrightarrow P \theta,\ n\to +\infty$

!!! note "一般矩法求得的估计量都满足相合性；对于极大似然估计, 在总体分布满足一定的条件下, 求得的估计量也是待估参数的相合估计量"

<font style="font-weight: 1000;font-size: 20px" color="orange">例 1：</font>

$$
f(x:\theta) =\begin{cases} \frac{1}{\theta}e^{- \frac{x}{\theta}} , & x\gt 0 \\ 0, & \text{其他}\end{cases}
$$

- （1）在形如 $c\sum_{i=1}^n X_i$ 的估计中求c，使其在均方误差准则下最优。
- （2）判断上面得到的估计量是否为 $\theta$ 的相合估计量。

$$\begin{array}l
c\sum_{i=1}^n X_i & = cn\hat{\theta} \\
Mse(cn\hat{\theta}) & = E[(cn\hat{\theta} - \theta)^2] \\
 & = E^2 (cn\hat{\theta} -\theta) +D(cn\hat{\theta} -\theta) \\
 & = [cnE(\hat{\theta}) -\theta]^2 +c^2 n^2 D(\hat{\theta}) \\
 & = c^2 n^2 E^2(\hat{\theta}) - 2cn\theta E(\hat{\theta}) + c^2 n^2 \frac{1}{n} D(X) \\
 & = \theta^2 [(n^2+n) c^2 -2cn +1]
\end{array}$$

$$
\text{所以 } c=\frac{1}{n+1} \text{时} Mse \text{取得最小值}$$

对于相合估计量的判断，应用切比雪夫不等式：

$$
P\{(\frac{n}{n+1}\hat{\theta} - \theta)\lt \varepsilon\} \ge 1- \frac{Var(\frac{n}{n+1}\hat{\theta})}{\varepsilon^2} = 1- \frac{n\theta^2}{(n+1)^2 \varepsilon^2}\to 1
$$




## 区间估计

!!! quote ""
	![[概率论与数理统计 (黄炜, 张帼奋, 张奕, 张彩伢) (Z-Library).png]]

### 定义

人们常常根据点估计对总体参数作出判断，但这种判断的把握有多大？可信度有多高？点估计无法回答这些问题。统计学家为了弥补此种不足，提出了区间估计。

对于待估参数 $\theta \in \Theta$ ，其有两个统计量 $\hat{\theta}_1 =\hat{\theta}_1 (X_1, X_2, ..., X_n) \lt \hat{\theta}_2 = \hat{\theta}_2 (X_1, X_2,..., X_n)$，且对于给定的 $\alpha \in (0,1)$ 和任意 $\theta \in \Theta$，有：

$$
P\{\hat{\theta}_1 \lt \theta \lt \hat{\theta}_2\} \ge 1- \alpha
$$

则称随机区间 $(\hat{\theta}_1 , \hat{\theta}_2)$ 为参数 $\theta$ 的**置信水平**为 $1-\alpha$ 的**置信区间**。

统计量 $\hat{\theta}_1$ 和 $\hat{\theta}_2$ 分别称为双侧置信下限和双侧置信上限；与之相对的，置信水平为 $1-\alpha$ 的单侧置信下限（上限）为：

$$\begin{array}l
P\{\hat{\theta}_1 \lt \theta\} \ge 1-\alpha, &\theta \in \Theta \\
P\{\theta \lt \hat{\theta}_2\} \ge 1-\alpha, &\theta \in \Theta \\
\end{array}
$$

**Lemma** 设统计量 $\hat{\theta}_1$ 和 $\hat{\theta}_2$ 分别为参数 $\theta$ 的置信水平为 $1-\alpha_1$ 和 $1-\alpha_2$ 的单侧置信上下限，那么 $(\hat{\theta}_1 , \hat{\theta}_2)$ 为参数 $\theta$ 的置信水平为 $1-\alpha_1 -\alpha_2$ 的置信区间。

!!! info "奈曼原则"
	称置信区间的平均长度 $E(\hat{\theta}_2 -\hat{\theta}_1)$ 为置信区间的**精确度**，由定义可知，当样本容量 $n$ 给定时，置信水平越高，精确度越低。因此我们在确定一定的置信水平前提下，尽可能提高精确度。如若总体 $X$ 是连续型随机变量，则我们尝试使 $P\{\hat{\theta}_1 \lt \theta \lt \hat{\theta}_2\}$ 等号成立。

### 枢轴量法

枢轴量（Pivot）为分布完全已知、且形式上不依赖于其它未知参数的函数，通常形式为 $G(X_1, X_2, ...,X_n;\theta)$ 。

通过枢轴量法求置信区间的步骤通常分为三步：

（1）构造一个已知的枢轴量 $G(X_1, X_2, ...,X_n;\theta)$；

（2）选取适当常数 $a,b$ ，使得 $b -a$ 最小，且满足：

$$
P_\theta \{a\lt G(X_1, X_2, ...,X_n;\theta) \lt b\} = 1-\alpha
$$

（3）将参数 $\theta$ 从函数中分离出来，等价转换为：

$$
P\{\hat{\theta}_1 \lt \theta \lt \hat{\theta}_2\} = 1- \alpha
$$

!!! note "习惯上，我们常取 $a,b$ 为"
	$$P_\theta\{G(X_1, X_2, ...,X_n;\theta) \le a\}=P_\theta\{G(X_1, X_2, ...,X_n;\theta) \ge b\}= \frac{\alpha}{2}$$


### 正态总体参数的区间估计

#### 单个正态总体

<font style="font-weight: 1000;font-size: 20px" color="red">均值 $\mu$ 的置信区间</font>

若总体方差 $\sigma^2$ 已知，则可以构造函数 $\frac{\bar{X}-\mu}{\sigma / \sqrt{n}} \sim N(0,1)$ ，式中仅有一个未知参数 $\mu$，且总体分布已知为标准正态分布，那么：

$$\begin{array}c
P\{a\lt \frac{\bar{X}-\mu}{\sigma / \sqrt{n}} \lt b\} =1 -\alpha \\
\Rightarrow P\{\bar{X}-b \frac{\sigma}{\sqrt{n}} \lt \mu \lt \bar{X}+a \frac{\sigma}{\sqrt{n}}\}
\end{array}$$

此时，置信区间的平均长度为 $L=(b-a) \frac{\sigma}{\sqrt{n}}$ ，根据正态分布的对称性可知取 $-a =b= z_{\alpha / 2}$ 时，$L$ 最短，则置信区间为：

$$
(\bar{X}- \frac{\sigma}{\sqrt{n}}z_{\alpha / 2},\ \bar{X} + \frac{\sigma}{\sqrt{n}} z_{\alpha / 2})
$$

常写作 $(\bar{X} \pm \frac{\sigma}{\sqrt{n}}z_{\alpha / 2})$

---

若总体方差 $\sigma^2$ 未知，此时函数 $\frac{\bar{X}-\mu}{\sigma / \sqrt{n}}$ 含有两个未知量，不能再使用。考虑用 $\sigma^2$ 的无偏估计量 $S^2$ 代替： $\frac{\bar{X}-\mu}{S / \sqrt{n}} \sim t(n-1)$

那么同理可得置信区间为：

$$\begin{array}c
\left(\bar{X} -\frac{S}{\sqrt{n}} t_{\alpha / 2} (n-1),\ \bar{X} +\frac{S}{\sqrt{n}} t_{\alpha / 2} (n-1)\right)\\
Or\ \ \ (\bar{X} \pm \frac{S}{\sqrt{n}} t_{\alpha / 2} (n-1))
\end{array}$$

!!! success "实际应用中，$\sigma^2$ 未知的情况更多，因此该区间的价值更大"

<font style="font-weight: 1000;font-size: 20px" color="red">方差 $\sigma^2$ 的置信区间</font>

样本方差的函数 $\frac{(n-1)S^2}{ \sigma^2} \sim \chi^2(n-1)$ 不依赖任何未知参数，那么：

$$
P\left\{\chi^2_{1- \alpha / 2 }(n-1)\lt \frac{(n-1)S^2}{ \sigma^2} \lt \chi^2_{\alpha / 2 }\right\} = 1-\alpha
$$

转换得到方差 $\sigma^2$ 的置信水平为 $1-\alpha$ 的置信区间为：

$$\left ( \frac{(n-1)S^2}{\chi^2_{\alpha / 2}(n-1)}, \frac{(n-1)S^2}{\chi^2_{1-\alpha / 2}(n-1)}\right)$$

#### 两个正态总体

##### 均值差 $\mu_1- \mu_2$ 的区间估计

分三种情况讨论。

<font style="font-weight: 1000;font-size: 20px" color="orange">两总体的方差已知</font>

$$
\bar{X} -\bar{Y} \sim N\left(\mu_1 - \mu_2, \frac{\sigma_1^2}{ n_1} + \frac{\sigma^2_2}{ n_2}\right)
$$

则类似单个正态总体的推导，得到置信区间为：

$$
\left(\bar{X} -\bar{Y} \pm z_{\alpha / 2}\sqrt{ \frac{\sigma_1^2}{ n_1} + \frac{\sigma^2_2}{ n_2}} \right)
$$


<font style="font-weight: 1000;font-size: 20px" color="orange">两总体的方差未知，但相同</font>

此时可取 $\sigma^2_1 = \sigma^2_2= \sigma^2$ 的无偏估计量为：

$$
S_{\omega}^2 =\frac{(n_1-1) S_1^2 + (n_2-1) S_2^2}{ n_1+ n_2 -2}
$$

且由定理可知：

$$
\frac{\bar{X}-\bar{Y} - (\mu_1 -\mu_2)}{ S_\omega\sqrt{ \frac{1}{n_1} +\frac{1}{n_2}}} \sim t(n_1 +n_2 -2)
$$

同样仿照单个正态总体的推导，得到置信区间为：

$$
\left(\bar{X} -\bar{Y} \pm t_{\alpha / 2} (n_1 +n_2 -2) S_\omega \sqrt{\frac{1}{n_1} +\frac{1}{n_2}} \right)
$$

<font style="font-weight: 1000;font-size: 20px" color="orange">两总体的方差未知，且不相同</font>

当样本容量 $n_1, n_2$ 足够大时（大于50），可认为：

$$
\frac{\bar{X}-\bar{Y} - (\mu_1- \mu_2)}{ \sqrt{\frac{S_1^2}{ n_1} +\frac{S_2^2}{ n_2}}} \sim N(0,1)
$$

则其置信区间为：

$$
\left(\bar{X}-\bar{Y} \pm z_{\alpha / 2} \sqrt{\frac{S_1^2}{ n_1} +\frac{S_2^2}{ n_2}} \right)
$$

##### 方差比 $\sigma_1^2 / \sigma_2^2$ 的区间估计

$$
\frac{S_1^2 / S_2^2}{ \sigma_1^2 / \sigma_2^2} \sim F(n_1 -1, n_2-1)  
$$

可得置信区间结果：

$$
\left( \frac{S_1 ^2 / S_{2}^2}{ F_{\alpha / 2} (n_1 -1,n_2 -1)}  , \frac{S_1 ^2 / S_{2}^2}{ F_{1-\alpha / 2} (n_1 -1,n_2 -1)} \right)
$$

!!! warning "单个正态总体的方差区间估计和此处的方差比估计都不具备最优性，因为 $\chi^2$ 和 $F$ 分布都不具备对称性"

