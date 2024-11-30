
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

