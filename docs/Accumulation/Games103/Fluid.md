
# Fluid

## Height Field Model

以二维平面为例，我们构造一个高度场，来模拟水波的起伏：

![[heightfield2D.png]]

!!! info "由于一个 x 只能对应一个 y，因此这实际上叫 1.5D"

因此，我们只需要负责更新高度 $h(x)$ 和速度 $u(x)$ 即可。

$$
\frac{dh(x)}{dt}+ \frac{d(h(x)u(x))}{dx} = 0 \Leftrightarrow \frac{dh(x)}{dt}+ h(x)\frac{du(x)}{dx} = 0
$$

其中 $d(h(x)u(x))$ 表示在 $dx$ 范围内，单位时间水量的变化。利用积分的链式法则拆开，并且忽略掉很小的 $u(x)\frac{dh(x)}{dx}$ 后得到后一个简化版等式。


$$
\frac{du(x)}{dt}=-\frac{1}{\rho} \frac{dP(x)}{dx}
$$

其中 $\rho$ 是密度，$P$ 是压强。此处忽略了外力等因素，我们在 lab4 中使用压强来模拟水的效果。

![[hupdate.png]]

对于二阶导数，我们的求法是进行泰勒展开得到近似值：

$$\begin{array}l
f(t_0 +\Delta t) = f(t_0) + \Delta t \frac{df(t_0)}{dt} + \frac{\Delta t^2 }{2} \frac{d^2f(t_0)}{dt ^2} +... \\
f(t_0 -\Delta t) = f(t_0) - \Delta t \frac{df(t_0)}{dt} + \frac{\Delta t^2 }{2} \frac{d^2f(t_0)}{dt ^2} +...
\end{array}$$

所以：

$$
\frac{d^2h(t)}{d t^2} = \frac{h(t_0+\Delta t) + h(t_0-\Delta t)- 2h(t_0)}{\Delta t^2}
$$

最终得到如下更新方程：

![[heightupdateequation.png]]

仅仅是这样，无法满足总水量是不变的条件，即 $\sum h = V$ ，有两种对应解决方法：

![[VolumnPreservation.png]]

在 lab4 中，我们采用 Solution 2 的形式，将 $\frac{\Delta t^2 Hg}{\Delta x^2 \rho}$ 记为常数 $\alpha$（$P=hg$），并为 $h_i( t_0) - h_i( t_0 -\Delta  t)$ 乘上粘滞常数 Viscosity $\beta$：

$$
h_i (t_0+ \Delta t) = h_i( t_0) +\beta (h_i( t_0) - h_i( t_0 -\Delta t)) + \alpha (h_{i+1} (t_0) + h_{i-1} (t_0) - 2h_i (t_0))
$$

!!! info "Boundary Condtion"
	遇到边界怎么处理？
	
	- <1> 认为边界外 $h$ 为常数
	- <2> 认为边界外的 $h$ 与等于边界上的值，如 $h_{i+1} = h_i$
	
	![[BoundaryCondition.png]]

接下来开始考虑刚体排水导致的高度变化这个问题。我们为排水处设置一个虚拟高度，来模拟排水造成的水波，有公式：

![[VirtualHeight1.png]]

总之，最后解出 $v$，再根据 $v$ 来更新高度，流程如下：

![[CouplingPipeline.png]]

## Eulerian Fluids

