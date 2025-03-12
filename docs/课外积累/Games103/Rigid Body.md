
## Rigid Body Dynamic

刚体模拟是非常重要、基础的，因此一般游戏引擎都会默认提供。本课程旨在学习它是如何运作的，而不是如何使用它。

!!! info "刚体模拟本质上是去更新描述物体状态的变量"
	![[RigidBodySimulation.png]]

对于刚体，只允许它做平移和旋转两种不影响物体形变的变化。

### Translational Motion

$$
\begin{cases}v(t^{[1]}) = v(t^{[0]}) + M^{-1} \int_{ t^{[0]}} ^{t ^{[1]}} f(x(t), v(t), t)dt  \\
x(t^{[1]})  =x (t ^{[0]}) + \int_{ t^{[0]}} ^{t ^{[1]}} v(t)dt \end{cases}
$$

其中 $M$ 代表质量，$f(x(t), v(t), t)$ 代表 t 时刻刚体所受的力，那么速度的后半段就表示加速度关于时间的积分。

可以看到，上述二元方程有两个变量 $x(t)$ 和 $v(t)$，它们的更新之间需要互相依赖。为了使其可解，则必须舍弃一些精度：

$$
\begin{cases}v^{[1]} = v^{[0]} + \Delta t M^{-1} f^{[0]}  & \text{Explicit}\\
x^{[1]} = x^{[0]} + \Delta t v^{[1]} & \text{Implicit}  \end{cases}
$$

!!! note "Air Drag Force"
	空气阻力大小通常与速度成正比，不过在一些简单模拟中，我们可以单纯认为空气阻力的作用仅为给速度乘上了一个系数，即 $v^{[1]} = \alpha v^{[0]}$，其中 $\alpha$ 为 decay coefficients(衰减系数)。

### Rotation

旋转可以用矩阵表示，但是在刚体模拟中，旋转矩阵是非常冗余、不直观、难以定义的。

在设计与机械控制中，常采用 Euler Angles 的方式表示旋转，在 Unity 中，分别为 *rotation-by-Z*，*rotation-by-X*，*rotation-by-Y*。

但欧拉角同样存在问题，例如 Gimbal Lock，在某些特定情形下，物体的旋转自由度会减少(多条旋转轴重合在一起)：

![[GimbalLock.png]]


因此我们引入四元数 Quaternion，其特点是类似于虚数，还可以进行除法操作，下面一张表定义了其乘法规则：

![[quaternionquote.png]]

对于四元数 $q=[s, v]$，部分运算规则如下：

$$\begin{array}l
aq = [as, av] \\
q_1 + q_2 = [s_1 + s_2, v_1 + v_2]\\
q_1 \times q_2 =[ s_1 \times s_2 - v_1 \times v_2, s_1 v_2 + s_2 v_1 + v_1 \times v_2] \\
||q|| = \sqrt{s^2 + v\cdot v}
\end{array}$$

!!! info "其中 s 指实数部分，v 指虚数部分（包括i j k）"

用四元数来表示旋转，虚数部分表示旋转轴，实数部分表示旋转角度：

$$
\begin{cases}q=[\cos \frac{\theta}{2}, v] \\
||q||=1 \end{cases}
$$

在 Unity 中，虽然界面上采用欧拉角的方式表示旋转，但其内部逻辑仍然是 Quaternion。

仿照 Translation，转动可以用角速度、力矩以及转动惯量来表示：

$$
\begin{cases}\omega^{[1]} = \omega^{[0]} + \Delta t (I^{[0]}) ^{-1} \uptau^{[0]} \\
q^{[1]} = q^{[0]} + \left[0, \frac{\Delta t}{2} \omega^{[1]} \right]\times q^{[0]} \end{cases}
$$

其中下半部分 $q$ 表示 Quaternion，在 Unity 中已经定义了四元数之间的乘法，但是没有定义加法，需要自己写。

根据以上思想，可以得出其模拟函数大致为：

![[rigidbodysimulationtandr.png]]

## Rigid Collision Detection and Response

### Penalty Method

可以使用距离函数对物体做碰撞检测。当距离为负时，认为发生了碰撞；为正时，则没有碰撞。

碰撞时产生的力，我们可以将其认为一种类弹簧的结构，距离（负数）越大，力越大。但是这样子判断的缺点是只有距离为负数时才会产生力，一种处理方式是为表面增加一个 Buffer，在 Buffer 范围内时就开始产生反冲力：

![[penaltymethod.png]]

这样子，k 的取值是一个重要的议题，一种思考方式是越接近表面，物体所受的力就越大，甚至无穷大。这种 Penalty Method 称为 Log-Barrier：

![[logbarrierpenaltymethod.png]]

!!! note "在Log-Barrier中，如果距离为负数，此时力也是向内的，物体会越来越深。因此需要假定穿透是不发生的"
	为了保证不穿透，则步长 $\Delta t$ 要尽可能小，同时带来了更高的成本

### Impulse Method

在 Penalty Method 中，碰撞后施加的力需要在下一个 $\Delta t$ 中才能起作用，Impulse Method 则想办法在碰撞发生时立即更新速度和位置。其思想是当距离变成负数时，立即将其更新为 0（即表面），而速度的更新则由更符合物理学的方式，分别乘以反弹系数和摩擦系数：

=== "Velocity"
	![[impulsevelocity.png]]
=== "Position"
	![[impulseposition.png]]

### Rigid Body

上述两小节所讲的“物体”都是针对一个小球的，而一个真正的模型是由很多 Vertex 组成的。对于单个 Vertex 的位置和速度状态：

$$
\begin{cases}x_i = x+ Rr_i & :Position\\
v_i = v + \omega \times Rr_i & : Velocity\end{cases}
$$

!!! info "其中 $x,v$ 分别为质心的位置、线速度，$R$ 是一个 Rotation 矩阵"

我们需要更新的状态为 $v,\omega$，在 $\Delta t$ 中碰撞产生的 impulse 冲量 $j$，施加在顶点 $i$ 上产生如下效果：

![[impulsestateupdate.png]]

!!! info "在实际应用中，我们认为 $Rr_i$ 是一个四元矩阵，类似于四元数的形式"

为了简化这一式子，我们讲 cross product 转换成矩阵形式，定义：

$$
r\times q= \left [\begin{matrix} r_y q_z - r_z q_y \\ r_z q_x - r_x q_z \\ r_x q_y -r_y q_x\end{matrix}\right ] = \left [\begin{matrix} 0 & - r_z  & r_y\\ r_z & 0 & -r_x \\ -r_y  & r_x & q_x\end{matrix}\right ] \left [\begin{matrix} q_x \\ q_y \\ q_z\end{matrix}\right ] :=r^*q
$$

由于对于该顶点 $i$，$v_i ^{new}$ 是已知的，因此我们先根据上式来计算出 impulse $j$，再使用 $j$ 更新质心的 $v,\omega$。

![[impulseeffect.png]]

!!! note "如果有多个 Vertex 同时发生碰撞，一般取平均值"

## Shape Matching

Shape Matching 换了种思路，先让每个 Vertex 都根据它们自己的 Position 和 Velocity 运动、碰撞，$\Delta t$ 时间后将这些点恢复成该刚体的形状：

![[shapematchingbasicidea.png]]

那么问题就在于如何将 Vertex 恢复成原来的形状。假设碰撞后某点的位置为 $y_i$，调整后最终的位置为 $x_i =c+Rr_i$，其中只有 $y_i$ 和 $r_i$ 是我们的已知量。我们希望下列式子的值能够最小：

$$
\sum_i \frac{1}{2} ||c+Rr_i-y_i ||^2
$$

分别对 $c$ 和 $R$ 求导，取一阶导数等于 0 的点：

$$
\begin{cases} c = \frac{1}{N}\sum_i y_i \\
R = \left(\sum_i (y_i-c) r_i^T\right) \left(\sum_i r_i r_i^T \right)^{-1} \end{cases}
$$

但是对于旋转矩阵 $R$，我们只需要左半旋转部分，抛弃右半形变部分。

![[shapematchingpipeline.png]]