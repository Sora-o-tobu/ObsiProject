
## 多元函数微分

### 点和点集的关系

- 开集：平面点集E中每个点都是E的内点，即int E = E，则称E为开集
- 闭集：平面点集E的余集 $R^2-E$ 为开集，则称E为闭集
- 若E中任意两点之间都可以用一条完全含于E的有限条折线相连接，则称E具有**连通性**
- 若E既为开集又有连通性，则称E为**开区域**
- 开区域连同其边界构成的点集称为**闭区域**

!!! info "常见的开集、闭集"
	![[常见的开集、闭集.png]]

### 累次极限和二重极限的存在关系

![[累次极限和二重极限的存在关系.png]]

### 验证可微与不可微:

![[验证不可微的方法.png]]

![[验证不可微的方法2(不连续也不一定不可微).png]]

??? example
	![[可微否？可微也.jpg]]

### 连续不一定可微的例子

![[连续不一定可微.png]]

以及$f(x,y)=\sqrt{x^2 +y^2}$ 在(0,0)处连续，但 ==偏导不存在== ，所以也不可微

### 可微但是一阶偏导不连续

$f(x,y)=(x^2 +y^2)sin\frac{1}{( x^2+ y^2)}$ 在(0,0)处可微，但是一阶偏导不连续。

!!! info
	因此，如果一阶偏导连续，那么可以推出可微；反过来则不一定

### 隐函数的偏导数

没啥用，不如一阶微分形式不变性：

$F(x,y,z)=0\ \Rightarrow \ F'_xdx +F'_ydy +F'_zdz =0$，由此可以求得z关于x和y的偏导

### 梯度定义以及方向导数求解步骤

- **梯度**定义
	- grad u = $\frac{\partial u}{\partial x}i+\frac{\partial u}{\partial y}j+\frac{\partial u}{\partial z}k=(\frac{\partial u}{\partial x},\frac{\partial u}{\partial y},\frac{\partial u}{\partial z})$
	- $grad\ (u,v)=ugrad\ v+vgrad\ u$
	- $grad\ f(u)=f'(u)grad\ u$

![[方向导数求解步骤.png]]

!!! danger
	注意此定理使用的先导条件是函数可微

!!! info "如何利用定理计算方向导数？"
	函数$f(x,y)\ =\ \frac{xy}{\sqrt{x^2 +y^2}}\ (x,y)\ne (0,0)$ 在 (0,0) 不可微，但是方向导数存在:
	
	设任意方向 $\overrightarrow{v}=(\cos \alpha ,\cos \beta)$ 
	
	则方向导数为 $\lim _{t\rightarrow 0^+}\frac{f(t\cos \alpha,t\cos \beta)-f(0,0)}{t}=\lim_{t\rightarrow o^+}\frac{t ^2\cos \alpha \cos \beta}{t ^2} =\cos \alpha \cos \beta$


### 偏导数存在和方向导数存在不可以互相推导出

![[方向导数存在的结论.png]]

### 泰勒公式的应用（不能忘记！）

![[泰勒公式真正姿态.jpg]]

![[泰勒公式的应用.jpg]]

---
![[泰勒公式应用3.jpeg]]

!!! info
	$f|_{\partial{D}}=0$ 大概是指函数f在D的边界上恒为0吧 (更新: $\partial D$ 就是 ==D的边界== )

---

!!! failure "居然不是泰勒公式的题..."
	![[居然不是泰勒公式的题.jpg]]
	
	要点在于凑出(0,0)处的值与边界值相等

### 极值的必要条件

![[极值的必要条件.png]]

一般在驻点和边界处寻找题目要求的最值。

### 极值的充分条件

![[极值的充分条件.png]]

### 拉格朗日乘数法求条件极值

在给定条件$G(x,y,z)=0$下，求目标函数$u=f(x,y,z)$的极值。

构造新函数 $L(x,y,z,\lambda)=f(x,y,z)+\lambda G(x,y,z)$

函数L 关于$x、y、z、\lambda$分别求导，得到四元方程组，可以求出驻点，再进行验证得到极值

!!! abstract
	- $\lambda$是辅助参数，如果不用求出$\lambda$就能求出怀疑极值点当然最好，否则就要求出$\lambda$
	- 此外，如果f或G的选取不好计算，可以尝试对其进行变换 (如平方等)