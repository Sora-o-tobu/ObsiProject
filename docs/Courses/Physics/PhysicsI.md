# 零碎知识点

!!! warning
	- 让你求具体数值的题目，如果填空后面没有给单位，要自己加上!!!
	- 给出波形图让你求波函数，注意该波形图是否是 t=0s 时的波形图，且要关注波形图中x,y的单位是否是 cm
	- 求电场强度,电场力等，最好顺带写上方向，如 $F=\frac{\lambda ^2}{2\pi \varepsilon _0 r_0}\overrightarrow{i}$


## 常用变换

#### 如果要求速度与距离而不是时间的关系

$$
\frac{dv}{dt} = v \frac{dv}{dx}
$$

## 曲率半径:
$$
\rho =\frac{1}{k}=\frac{[1+{f'}^{2} (x)]^\frac{3}{2}} {|f''(x)|}
$$

虽然但是，平常如果遇到看似需要用曲率半径求法向加速的情况，请想象能不能先求出总加速度，有

$$
{a_总}^2 ={a_\tau}^2 + {a_n}^2\ \ \ 其中a_\tau一般指切向加速度
$$

## 常用转动惯量

![[常见转动惯量.png]]

### 以直径为轴的细圆环及其推导

![[以直径为轴的细圆环.png]]

### 实心球转动惯量的推导

![[实心球转动惯量的推导.png]]

!!! note "在计算实心球的时候不要用dθ来作为微元，但是薄球壳可以"


### 平行轴定理

$$
J=J_C+mh^2
$$
其中:

- $J$是任一转轴的转动惯量
- $J_C$是通过质心转轴的转动惯量
- $m$是刚体的质量
- $h$是两轴间的距离

### 垂直轴定理

在刚体为薄板，且位于xy平面内的条件下:

$$
J_z=J_x+J_y
$$

其中，$J_x、J_y、J_z$分别为对x、y、z轴的转动惯量


!!! danger "与地面无滑动也有摩擦力"
	![[与地面无滑动也有摩擦力.jpg]]


## 两个物体只有保守力的情况下角动量守恒

**保守力**:
沿任意闭合回路做功为零的力，例如弹簧简谐运动的弹力、重力、“有心力”(如万有引力)

!!! example "如此题的第一小问，可以用角动量守恒解答: $mv_0D =mvd$"
	![[角动量守恒.jpg]]


**保守力判断依据**:

$$\begin{gather}
(1)\oint F_保dr=0 \\
(2)\frac{\partial F_x}{\partial y} = \frac{\partial F_y}{\partial x}\ (对于二维)
\end{gather}
$$


!!! note "常见非保守力:摩擦力、爆炸力、涡旋电场力、磁力、非弹性冲击力"

只有内力是保守力，才能引入势能；系统在任意位置的势能等于从该位置沿任意路径变到势能零点过程中，保守内力做的功。

**【错题】：** 
人造地球卫星的椭圆轨道离地心最近距离是3R，最远距离是6R，取地球表面加速度为g，则求此卫星的最小速率和最大速率。

![[椭圆轨道的速率.jpg]]

!!! note "由于势能离越远就越大，因此系统的机械能应为$\frac{1}{2}mv^2 +(- \frac{{GMm}}{r})$"

!!! example "这也是角动量守恒"
	![[这也是角动量守恒.jpg]]

## 洛伦兹变换

$$
x'=\frac{x-ut}{\sqrt{1-\frac{u^2}{c^2}}},\ t'=\frac{t-\frac{ux}{c^2}}{\sqrt{1-\frac{u^2}{c^2}}}
$$

### 长度收缩

$$
l'=l_0\sqrt{1- \frac{u^2} {c^2}}
$$

所以观察运动中的系（相对观测者的系而言）中杆子的长度比静止长度短

!!! note
	如果观察运动中的系中方块的体积，则不一定所有边长都变短，可能只有相对于观察者运动的那一个 $l$ 变短了

### 时间延缓

$$
t=\frac{t'}{\sqrt{1-\frac{u^2} {c^2}}}
$$

!!! example "看运动系中的物体有时间延缓效应"
	假定在地球上观察到一颗脉冲星的脉冲周期为 0.5s ，且这颗星正沿观察方向以运行速度 0.8c 离我们而去，那么这颗星的固有脉冲周期应该是 ==0.3s==

### 运动物体的质量

$$
m=\frac{m_0} { \sqrt {1-\frac{u^2} {c^2}}}
$$

其中m为物体以速度u运动时的质量，m<sub>0</sub>为物体在相对静止参考系中的质量

因此，运动中物体质量较大

### 爱因斯坦速度变换公式

$$
v_x'=\frac{v_x-u}{1-\frac{v_xu}{c^2}},v_y'=\frac{v_y\sqrt{1-\frac{u^2}{c^2}}}{1-\frac{v_xu}{c^2}}
$$

>例如，取实验室为S系，观察者为S‘系，一个粒子在S系中以0.6c运动，观察者与粒子同向以0.8c运动，则在S'系中粒子速度为:

$$
v_x'=\frac{0.6c-0.8c}{1-0.6*0.8}=\frac{-5}{13}c
$$

### 能量\\动能\\动量

- 静止物体的能量: $E_0 =m_0c^2$
- 运动物体的能量: $E=mc^2 =\frac{m_0 c^2} { \sqrt {1-\frac{u^2} {c^2}}}$
- 运动物体的动能: $E_k =mc^2 -m_0c^2$
- 运动物体的动量: $p=mv$



## 机械振动

$$
x=A\cos (\omega t+ \varphi)
$$

$$
v= \frac{{dx}}{{dt}} =-\omega A\sin (\omega t+\varphi) 
$$

$$
a= \frac{d^2x}{dt^2} =-\omega ^2 A\cos (\omega t +\varphi) =-\omega ^2 x\ \ \Rightarrow\ \  \omega=\sqrt{ \frac{k}{m} }
$$


弹性势能$E_p= \frac{1}{2} kx^2= \frac{1}{2}kA^2 \cos ^2 (\omega t+\varphi)$

动能$E_k= \frac{1}{2}mv^2 = \frac{1}{2}m\omega ^2 A^2 \sin ^2 (\omega t+\varphi)= \frac{1}{2}kA^2 \sin ^2 (\omega t+\varphi)$

因此系统的总机械能为 $E= \frac{1}{2}kx^2 + \frac{1}{2}mv^2 = \frac{1}{2}kA^2$

### 常见的简谐运动

-  单摆

单摆所受的重力的切向分力: $mg\sin \theta$

单摆小球的切向加速度$a_\tau =l(d^2\theta / dt^2)$

由牛顿第二定律得到 $mg\sin \theta =m l(\frac{d^2\theta}{dt^2})$ ，当$\theta$很小的时候 $\sin \theta \approx  \theta$

因此有$- \frac{g}{l}\theta =\frac{d^2\theta}{dt^2}=- \omega ^2 \theta$  ==(角加速度为 $-\frac{g}{l}\theta$)==

即:

$$
\theta =\theta _m\cos (\omega t+\varphi)，其中\omega =\sqrt{\frac{g}{l}}
$$

-  复摆（物理摆）

![[物理摆.png]]

![[物理摆2.png]]

-  扭摆

$$
M=-k\theta\ ; \ M=J \frac{d^2\theta}{ dt^2} =-J\omega ^2\theta\ \ \Rightarrow \omega =\sqrt{\frac{k}{J}}
$$

!!! example
	=== "常规解法"
	![[机械振动例题.png]]
	=== "功能关系"
	由于简谐振动总机械能不变，有$\frac{1}{2}kx^2 + \frac{1}{2}mv^2$ ，分析题目可知：
	
	物体动能： $\frac{1}{2}mv^2$
	
	滑块动能： $\frac{1}{2}J\omega ^2 = \frac{1}{4}Mv^2$
	
	弹性势能： $\frac{1}{2}kx^2$ (此处x指相对平衡位置的位移，不一定是弹簧原长处)
	
	因此系统机械能 $E=\frac{1}{2}(m+ \frac{1}{2}M)v^2 + \frac{1}{2}kx^2$
	
	==等式关于时间t求导==： $0=(m+ \frac{1}{2}M)v \frac{{dv}}{{dt}}+kx \frac{{dx}}{{dt}}$
	
	约掉 $v= \frac{{dx}}{{dt}}$，且有 $\frac{{dv}}{{dt}}= \frac{d^2x}{dt ^2}$得到
	$(m+ \frac{1}{2}M)\frac{d^2x}{dt ^2} +kx=0$
	
	因此直接得到： $T= \frac{2\pi}{\omega} =2\pi \sqrt{\frac{M/2+m}{k}}$

- **实际上，利用总机械能关于时间 t 求导为 0 这一性质，可以用功能分析代替受力分析来求简谐运动周期**

!!! example
	![[机械能关于时间不变.png]]

### 振动的合成

#### 同方向同频率的简谐振动的合成

![[振动的合成.png]]

#### 相互垂直的简谐振动的合成

![[垂直简谐振动的合成.png]]

??? info "证明"
	![[垂直简谐振动合成证明1.png]]
	
	![[垂直简谐振动合成证明2.png]]


![[垂直合成的图形.png]]


### 机械波

波动方程最基本的形式 $y(x,t)=A\cos [\omega (t- \frac{x}{u})+ \varphi]$

- **波速u**：振动传播的速度
- **波长$\lambda$**：相位差为 $2\pi$ (运动完全相同) 的两个质点的距离
- 此方程的波是正向传播的，若是负向传播，将 x 改成 -x (可以看作 u 变成 -u)
- 带入`x`后，波动方程成为该处质点的**振动方程**
- 带入`t`后，波动方程成为该时刻的**波形**

#### 波的能量


任取体元$\Delta V$,质量为$\Delta m =\rho \Delta V$ ,波传播到此体元时，体元具有动能 $W_k$ 和势能 $W_p$ ，由于简谐波表达式为 $y(x,t)=A\cos \omega (t - \frac{x}{u})$ ，可以证明 $W_k =W_p = \frac{1}{2}\rho A^2 \omega ^2 (\Delta V)\sin ^2 \omega (t- \frac{x}{u})$

即，体元的总机械能为 $\rho A^2 \omega ^2 (\Delta V)\sin ^2 \omega (t- \frac{x}{u})$

!!! question "证明过程"
	对于体元的动能，$W_k =\frac{1}{2}(\Delta m)v^2 =\frac{1}{2} \rho (\Delta V)v^2$ ,
	
	又$v= \frac{\partial y}{\partial t} = -A\omega \sin \omega (t-\frac{x}{u})$
	
	得到$W_k = \frac{1}{2}\rho A^2 \omega ^2 (\Delta V)\sin ^2 \omega (t- \frac{x}{u})$
	
	对于势能，根据胡可定律，这一体元所受弹性力为$f=\frac{YS}{{\Delta x}} \Delta y$
	
	因此弹性势能$W_p=\frac{1}{2}k(\Delta y) ^2$
	$=\frac{1}{2} \frac{YS}{{\Delta x}} (\Delta y) ^2$
	$=\frac{1}{2}YS\Delta x (\frac{\partial y}{\partial x})^2$
	
	又$S\Delta x=\Delta V\ ,\ Y=\rho u^2$
	$\frac{\partial y}{\partial x} =A \frac{\omega }{u}\sin \omega (t- \frac{x}{u})$
	
	最后可得$W_p = \frac{1}{2}\rho A^2 \omega ^2 (\Delta V)\sin ^2 \omega (t- \frac{x}{u})$


- 与简谐振子完全不同，机械波动能达最大的时候势能也最大。
- 总机械能随时间变化，说明对于任意体元，都在不断接受和放出能量，说明波动传递能量


!!! note 
	其实也可以想象成这根绳子在该点动能最大时，形变量也最大，因此势能也最大



**【波的能量密度】：**
介质中波的单位体积的能量称为能量密度

$w= \frac{W}{{\Delta V}} = \rho A^2 \omega ^2 \sin ^2 \omega (t- \frac{x}{u})$

**【平均能量密度】：**
能量密度在一个周期内的平均值

$\overline{w} =\frac{1}{T} \int _0 ^T wdt = \frac{1}{2}\rho A^2 \omega ^2$

机械波的能量与振幅、频率的平方成正比，这对弹性介质上传播的各种简谐波均成立

**【能流密度】：**
单位时间内通过介质中某面积的能量称为通过该面积的 ==能流== 。介质中取面积S垂直于波速u，则单位时间内通过S面的能量等于体积uS中的能量wuS 。由于能量是周期性变化的，通常取一个周期内的平均值为 ==平均能流==

$\overline {P} = \overline{w} uS\ ,\ \overline{w} = \frac{1}{2}\rho A^2 \omega ^2$

通过与波传播方向垂直的单位面积的平均能流，称为 ==平均能流密度== 或 ==波的强度== :

$I = \frac{\overline{P}}{S} =\overline {w} u$，其中$z=\rho  u$称为介质的 ==特性阻抗== 。

!!! example ""

	=== "对于平面波"
		
		![[平面波振幅不变.png]]
		
		$\overline {P} _1 =\frac{1}{2}\rho A_1^2 \omega ^2 uS$ 
		
		$\overline {P} _2 =\frac{1}{2}\rho A_2^2 \omega ^2 uS$
		
		对无吸收介质，通过两平面的平均能流相等，则$A_1 =A_2$，平面波在无吸收介质中满足振幅不变条件
	
	=== "对于球面波"
		
		![[球面波振幅与距离成反比.png]]
		
		将$S_1=4\pi r_1 ^2$ $S_2=4\pi r_2 ^2$代入，
		
		由于总能流相等，可以得到 $A_1 r_1 = A_2 r_2$
		
		故球面波的表达式为$y(x,t)= \frac{A_o}{r}\cos \omega (t- \frac{x}{u})$


!!! info "球面波补充"
	![[球面波补充.png]]



#### 波的合成与干涉

**波的干涉**

- **条件**：两个**频率相同、振动方向相同、相位差恒定**的波源
- **现象**：在各点引起的振动振幅虽然各不相同，但不随时间变化

!!! abstract
	![[波干涉方程.png]]

#### 驻波

- **定义**：两列**振幅相等**、沿**相反方向**传播的**相干波**叠加形成的波（也可以是入射波和反射波）
- **特点**：各点的振幅各不相同，从而存在波腹（振幅最大）和波节（振幅为 0），
	- 相邻波节距离λ / 2 , 相邻两波节间的各点同时到达最大位移（或最小位移）， 
	- 波节两侧的相位相反（相差 π）
- **结论**：驻波不是振动状态的传播，也没有能量的传播

!!! info "半波损失"
	![[半波损失例子.png]]
	从波疏介质到波密介质(固定端)，会产生半波损失，从而产生每半个周期出现一次的波节
	
	![[zyh偷来的图.png]]

#### 多普勒效应

![[多普勒效应方程.png]]

!!! example "声速全程不变"
	![[多普勒效应例子1.png]]

---

!!! example "合理分析观察者和波源"
	![[警车鸣笛例子.jpg]]
	本题中，观察者和波源都是自己，但是声音是通过墙壁反射回来的，因此观察者和波源都是在向对方移动，则频率为 $\frac{340+30}{340-30}300$ Hz


## 气体分子动理论

热力学系统可分为三类：

- **孤立系统**：与外界无物质、能量交换
- **封闭系统**：无物质交换，但有能量交换
- **开放系统**：有物质交换，也有能量交换

当系统处于 *热平衡*（温度均衡），*力学平衡*（压强均衡），*化学平衡*（化学成分均衡），这种状态称为 ==平衡态== 。 平衡态指宏观状态的平衡，而大量分子仍作无规则运动。

### 参数


![[微观参数.png]]

!!! info "热力学温度与摄氏温度 $T=t+273.15$"

!!! example "用 PV = vRT 变种公式可以求单位体积分子数"
	$p=nkT$ n为单位体积内的分子数
	![[热力学重要例子.png]]

---

不同的分子具有不同的结构，理想气体分子除平动外，还有转动与振动，因此在研究分子无序运动的平均能量时，对微观模型应做适当的修正。

**自由度**：决定一个物体空间位置所需要的独立坐标数，如平面质点自由度为2，空间质点自由度为3

!!! tip "刚体自由度"
	![[刚体自由度.png]]

![[微观分子自由度.png]]

!!! info "因为平动自由度永远等于 3 ，分子平均平动动能为 $\overline{\varepsilon_t}=\frac{3}{2}kT$"

### 麦克斯韦速率分布律
==理想气体== 在某一平衡态下的速率分布函数，即麦克斯韦速率分布函数

$$
\frac{dN}{N}=f(v)=4\pi (\frac{m}{2\pi kT})^{ \frac{3}{2}} e^{- \frac{mv^2}{2kT}} v^2
$$

式中， `m` 为单个分子质量，`T` 为温度，`k` 为玻尔兹曼常数

- 有 $\int_0^\infty f(v)dv=1$
- 若求速率区间分子比例，为 $\int_{v _1} ^{v_2}f(v)dv$
- 麦克斯韦速率分布率公式不用记  ==(不是我说的哈，真考了别怪我)==

!!! question "由麦克斯韦速率分布律可以求出三种统计速率"
	=== "最概然速率"
		气体分子的最概然速率 $v_p$ (Most Probable Speed) 是速率分布函数 $f(v)$ 取极大值时的速率，即 $f'(v_p)=0$ ，可以求出 $v_p = \sqrt{\frac{2kT}{m}} =\sqrt{\frac{2RT}{\mu}}$
	=== "平均速率"
		大量分子速率的算数平均值，称为平均速率
		其中$\overline{v}=\int ^{\infty} _0 vf(v)dv = \sqrt{\frac{8kT}{\pi m}}= \sqrt{\frac{8RT}{\pi \mu}}$
	=== "方均根速率"
		分子速率平方的平均值开平方，称为方均根速率。
		$\sqrt{\overline{v^2}} =\sqrt{\int ^{\infty} _0 v^2f(v)dv}$
		解得 $\sqrt{\overline{v^2}} =\sqrt{\frac{3kT}{m}} =\sqrt{\frac{3RT}{\mu}}$

**三种速率的大小关系**：

$$
v_p: \overline{v} :\sqrt{\overline{v^2}} =\sqrt{2} :\sqrt{\frac{8}{\pi}} : \sqrt {3}
$$

!!! note "看不懂字母?"
	- 此处 m 为单个分子质量 , 与上文平均动能中的 $\mu$ 同意义
		- 注意区分 $\mu$ 意义的差别
	- $R=kN_A$
	- $\mu$ 为摩尔分子质量 , 记住 $O_2$ 为 32g/mol ，计算的时候要化成千克!!!
		- 若M为气体总质量，则有 $v=\frac{M}{m\times N_A}=\frac{M}{\mu}$

!!! example "善于变换变量"
	![[方均根速率例子1.jpg]]

### 气体分子的平均自由程

- **平均碰撞频率**
	- 一个分子 ==单位时间== 内与其它分子的平均碰撞次数，用 $\overline{Z}$ 表示 
- **平均自由程**
	- 在热平衡状态下，一个气体分子在任意连续两次碰撞之间所经过的直线路程，称为自由程。
	- 一个气体分子在连续两次碰撞之间可能通过的各段自由程的平均值，称为平均自由程,用 $\overline{\lambda} = \frac{\overline{v}}{\overline{Z}}$ 表示

!!! success "平均碰撞频率公式推导"
	分子A相对其它分子以相对平均速率 $\overline{v_r} =\sqrt{2} \overline{v}$ 运动，运动方向上，以d为半径的圆柱体内的分子都将与分子A碰撞，截面面积为 $\pi d^2$ ,且 ==单位体积内有n个分子==（$p=nkT$），则 $\overline{Z}=\sqrt{2}\pi d^2 \overline{v} n$

$$
\overline{Z}=\sqrt{2}\pi d^2 \overline{v} n\ ;\ \overline{\lambda} =\frac{\overline{v}}{\overline{Z}} =\frac{1}{\sqrt{2}\pi d^2n}=\frac{kT}{\sqrt{2}\pi d^2p}
$$

!!! danger
	恒压情况下加热理想气体，气体分子的平均碰撞频率将 ==减少==（T增大对n减少的影响大于对v增大的影响）

## 热力学基础

**【热力学第一定律】：**
dQ = dE +dW，E表示系统的内能

- 其中
	- 系统对外界做功，W>0；外界对系统做功，W<0。
	- 系统从外界吸热，Q>0；系统向外界放热，Q<0。

一个系统升温 dT 时，如果它吸收的热量为 dQ ，则系统的热容量定义为：$C=dQ/dT$

- **比热容**：$c=C/M$
- **摩尔热容**: $C_m=C/v$

### 等体过程

dV = 0，因此等体过程 dW = 0，有第一定律得 $Q=\Delta E =v \frac{i}{2} R\Delta T$ ，即等体过程系统吸收的热量全部转换为内能的增量

==等体摩尔热容== 为1mol的物质在体积保持恒定的状态下，温度升高1K所吸收的热量

$$
C_v= \frac{dQ_v}{vdT} = \frac{i}{2}R
$$

!!! note
	理想气体的等体摩尔热容仅与 ==分子自由度== 有关

### 等压过程

$$
pV=vRT\ \Rightarrow dW = pdV=vRdT;
$$

$$
dQ =vC_p dT
$$

$$
dE=dQ-dW=vC_pdT - vRdT
$$

!!! info
	$C_p =C_v +R$ 这说明与等体过程相比，1mol理想气体温度同样升高1K，等压过程将比等体过程多吸收8.31J热量。

!!! example "公式要记熟"
	![[公式要记得熟练.png]]

### 等温过程

dT = 0，因此等温过程内能不变，dE = 0，由第一定律得 $Q_T=W=\int _{V _1} ^{V_2}pdV =\int _{V _1} ^{V_2}\frac{vRT}{V}dV =vRTln\frac{V_2}{V _1} =vRTln\frac{p_1}{p _2}$ ，即等温过程气体从外界吸收的能量全部用来对外做功。

!!! warning "**等温过程的摩尔热容为无限大**"

### 绝热过程

??? info "绝热过程等式证明过程，补天不需要看"
	dQ = 0，系统与外界无热交换，气体系统以减少内能为代价对外做功。$dE+dW=0\ \Rightarrow \ vC_vdT+pdV=0$ 
	
	对理想气体状态方程求导得：$vRdT=pdV+Vdp$，联立上述两式，得: $(C_V+R)pdV+ C_VVdp=0= C_ppdV+C_VVdp$



定义 $\gamma =\frac{C_p}{C_V}$ ，有$\frac{dp}{p}+\gamma \frac{dV}{V}=0$

对于理想气体，$\gamma$为 ==常数== ，将上式积分得 $pV^\gamma =c_1, c_1为一常数$

利用理想气体方程，还可以将此式表示为用(V,T)或(p,T)作为变量的形式：

- $TV^{\gamma -1}=c_2$
- $p^{\gamma-1}T ^{-\gamma} =c_3$

绝热过程功和内能变化:

$$
A=\Delta E=vC_v\Delta T=vC_v\frac{\Delta (pV)}{vR}=\frac{\Delta(pV)}{\gamma -1}
$$


!!! note "绝热过程dQ = 0，因此摩尔热容为0。"

!!! question "绝热线比等温线更陡峭"
	- 等温方程： $pV=c\ \Rightarrow \ \frac{dp}{dV}=-\frac{p}{V}$
	- 绝热方程： $pV^\gamma =c\ \Rightarrow \ \frac{dp}{dV}=-\gamma \frac{p}{V}\ \ , \gamma \gt 1$ 
	
	![[等温线和绝热线的比较.png]]

!!! example "内能是态函数，而功、热量是过程量，因此这题选择 B"
	![[内能是态函数.png]]


!!! example "综合题"
	![[热力学过程综合题1.png]]
	> Ps : 氧气的化学式量为 32 ，气体标准状态下的摩尔体积为 $22.4\ m^3/mol$

### 循环过程

- 沿**顺时针**方向进行的循环称为 ==正循环== 或 ==热循环== ，系统膨胀对外做功大于系统压缩外界对系统做的功
- 沿**逆时针**方向进行的循环称为 ==逆循环== 或 ==制冷循环== ，系统膨胀对外做功小于系统压缩外界对系统做的功

#### 热机的效率

对于正循环，一定质量的工质在一次循环过程中要从高温热源吸热$Q_1$，对外作净功$|W|$，又向低温热源放出热量$Q _2$，而工质回到初态，内能不变，则 ==热机效率== $\eta$ 表示为：

$$\begin{gather}
|W|=Q_1 -Q_2; \\
\eta =\frac{ |W|}{Q_1} =\frac{Q_1 -Q_2}{Q _1} .
\end{gather}
$$

!!! example "热机效率例题(要求掌握各种过程的热量求法!)"
	![[热机效率例题.jpg]]



#### 制冷机和制冷系数

利用工作物质的逆循环，不断从低温热源吸收热量，转递给高温热源的机器

对于逆循环，工质经一循环，外界必须对系统作功，系统从低温热源吸热$Q_2$，向高温热源放热$Q _1$，使低温热源温度更低，而工质回到初态，内能不变，则 ==制冷系数== $\omega$ 表示为：

$$\omega =\frac{Q_2}{|W|} =\frac{Q_2} {Q_1 -Q_2}
$$

!!! warning "循环过程热机效率解题步骤"
	- 判断是正循环还是逆循环，决定求热机效率还是制冷系数
	- 以常见的正循环题目为例，判断循环每一过程是放热还是吸热，这将决定计算的步骤
		- ![[热机效率求解步骤.jpg]]{ width=200px }
		- 该图中，bc过程放热，ca过程吸热，所以 $\eta =\frac{ |W|}{Q_{ca}} =\frac{Q_{ca} -Q_{bc}}{Q _{ca}}$ 

#### 卡诺循环

工作在两个热源（高温热源 T1 ，低温热源 T2 ），由两个等温过程、两个绝热过程组成的循环

!!! note "T-S 图"
	<center>![[卡诺循环TS图.png]]</center>

- 卡诺 ==热机效率== $\eta _c=\frac{T_1 -T_2}{T_1}$
- 卡诺 ==制冷系数== $e_c =\frac{T_2}{T_1 -T_2}$

??? example "卡诺机例题"
	![[卡诺机例1.png]]
	![[卡诺机例2.png]]
	![[卡诺机例3.png]]

### 熵变
对于微小可逆过程，熵增量写成 $ds =\frac{dQ}{T}$ 单位为 $J/K$

- 绝热可逆过程： $dQ=0\ \ \Delta S=\int ^b_a \frac{dQ}{T}=0$
- ==任何可逆过程：== $\Delta S=vC_v \ln\frac{T_2}{T _1} +vR\ln\frac{V_2}{V _1}$
	- 等体过程熵变： $\Delta S=vC_{V,m}\ln \frac{T_2} {T_1}$
	- 等压过程熵变： $\Delta S=vC_{p,m}\ln \frac{T_2} {T_1}\ \ （pV=vRT）$
	- 理想气体等温过程熵变： $\Delta S=vR\ln\frac{V_2}{V _1} =vR\ln\frac{p_1}{p _2}$


#### 熵增加原理
对于一个绝热系统，$dQ=0$，则有 $\Delta S\ge 0$ 

- 当绝热过程不可逆则取不等号
- 当绝热过程可逆则取等号
- 由此可知孤立系统熵只增不减

!!! example "熵变例题"
	![[熵增加原理Example.png]]


## 电磁学
### 库伦定律

$$
\overrightarrow{F_{12}}=\frac{1}{4\pi \varepsilon_0}\frac{q _1 q_2}{r _{12}^2} \frac{\overrightarrow{r _{12}}}{r _{12}}  \ \ 其中\varepsilon_0=\frac{1}{4\pi k}=8.85\times10^{-12} C^2/(N\cdot m^2)
$$

### 静电场
相对于观察者静止的电荷周围所存在的场称为静电场（该电荷称为场源电荷）

- 电场强度： $\overrightarrow{E}=\frac{\overrightarrow{F}}{q_0}$
- 电通量： $\Phi=\oint_S \overrightarrow{E}d\overrightarrow{S}$    ==第二类曲面积分==

!!! info "高斯定理"
	过任意闭合曲面的电通量等于该曲面所包围的所有电荷量的代数和除以$ε_0$
	
	$\Phi=\oint_S \overrightarrow{E}d\overrightarrow{S} =\frac{1}{\varepsilon_0} \sum q_i$
	
	- 通过闭合曲面的电通量只取决于闭合曲面内包围的净电荷，而与电荷的空间分布无关，但电荷的空间分布会影响闭合面上各点处的场强大小和方向
	- 闭合面外的电荷及其分布对闭合面上的电通量没有贡献，但将影响闭合面上各点的场强大小和方向，如下图两种情形： $Φ_e$ 相同，但E不同:
	![[电通量相同但电场强度不同.png]]


=== "球对称分布"
	包括均匀带电的球面、球体、多层同心球壳等
	
	以半径为R的均匀带电球面为例，高斯面取半径为r的同心球面($r\ge R$)，根据定理 $\Phi=\frac{q}{\varepsilon_0}$ ，则带电球面外任意一点的场强为 $\overrightarrow{E}=\frac{\Phi}{4\pi r^2} \frac{\overrightarrow{r}}{r}$
=== "轴对称分布"
	包括 ==无限长== 均匀带电的直线，圆柱面，圆柱体等
	
	以半径为R的均匀带电圆柱面为例，高斯面取同轴封闭圆柱面，过P点作长为l，半径为r($r\ge R$)的圆柱形高斯面，则电通量为 $\Phi_e=\oint _S \overrightarrow{E}d\overrightarrow{S}=\int _{侧}\overrightarrow{E}d\overrightarrow{S} =E2\pi rl$
	
	若闭合面内的总电荷为 $\lambda l$ ，由高斯定理得： $E\cdot 2\pi rl =\frac{1}{\varepsilon_0}\lambda l\ \ \Rightarrow \ \ E=\frac{\lambda}{2\pi \varepsilon_0 r}$
=== "无限大平面电荷"
	包括无限大的均匀带电平面，平板等
	
	以无限大均匀带电平面为例，高斯面取轴线垂直于带电平面、两底面对称的封闭柱面。设无限大均匀带电平面的面电荷密度为 $\sigma$ ，则通过图示高斯面的总电通量为 $\Phi_e =\oint_S\overrightarrow{E}d\overrightarrow{S}= \int_{左底面} \overrightarrow{E}d\overrightarrow{S}= \int_{右底面} \overrightarrow{E}d\overrightarrow{S}=2ES$
	
	依据高斯定理有： $2ES=\frac{1}{\varepsilon_0}\sigma S \ \Rightarrow \ E=\frac{\sigma}{2\varepsilon_0}$

![[高斯定理应用.png]]


!!! example "课本例题1"
	![[电磁学课本例题1.png]]
	
	- 补充 : 对于求 $E_2$ 的大小，要用到二重积分
		- $dE=\frac{dq}{4\pi\varepsilon_0 (x^2 +r^2)}\times \frac{x}{\sqrt{x^2 +r^2}}= \frac{\sigma xrd\theta dr}{4\pi \varepsilon_0 (x^2 +r^2) ^{3/2}}$
		- 再进行积分即可求得答案

