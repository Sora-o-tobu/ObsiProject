
# Transformation

!!! quote "Graphics' Dependencies"
	- **Basic Mathematics**
		- Linear algebra, calculus, statistics(统计学)
	- **Basic Physics**
		- Optics, Mechanics
	- **Misc**
		- Signal Processing 信号处理
		- Numerical analysis 数值分析
	- **A bit of aesthetics**

图形学中，给出一个向量，默认是列向量。

## 2D Transformations

### Scale 缩放

将向量 x 轴方向缩放 $s_x$ 倍，y 轴方向缩放 $s_y$ 倍：

$$
\left[\begin{matrix} x' \\ y' \end{matrix} \right] =\left[\begin{matrix}s_x & 0\\ 0 & s_y\end{matrix}\right] \left[\begin{matrix} x \\ y \end{matrix} \right]  = \left[\begin{matrix} s_xx \\ s_yy \end{matrix} \right] 
$$

### Shear 切变

![[shearexample.png]]

竖直方向位移为 0，水平方向位移与 y 坐标呈 $ay$ 关系：


$$
\left[\begin{matrix} x' \\ y' \end{matrix} \right] =\left[\begin{matrix}1 & a\\ 0 & 1\end{matrix}\right] \left[\begin{matrix} x \\ y \end{matrix} \right]  = \left[\begin{matrix} x+ay \\ y \end{matrix} \right]
$$

### Rotate 旋转

![[rotateexample.png]]

$$
\left[\begin{matrix} x' \\ y' \end{matrix} \right] =\left[\begin{matrix}\cos \theta & -\sin \theta\\ \sin \theta & \cos \theta \end{matrix}\right] \left[\begin{matrix} x \\ y \end{matrix} \right]  = \left[\begin{matrix} x\cos \theta -y\sin \theta \\ x\sin \theta +y\cos \theta \end{matrix} \right]
$$

### Inverse 逆变换

![[inverseexample.png]]


!!! info "旋转矩阵的逆等于旋转矩阵的转置，即正交矩阵"
	$$\begin{array}c R_\theta = \left[\begin{matrix}\cos \theta & -\sin \theta\\ \sin \theta & \cos \theta \end{matrix}\right] \\ R_{-\theta} = \left[\begin{matrix}\cos \theta & \sin \theta\\ -\sin \theta & \cos \theta \end{matrix}\right] = R_\theta^T \\ 
	\text{By Definition:  } R_\theta^{-1} = R_{-\theta} = R_\theta ^T\end{array}$$


## Homogeneous coordinates

### Translation 平移

对于平移操作，我们不能简单使用矩阵乘法表示该变换，这也是为什么我们需要引入齐次坐标变换。

![[translationexample.png]]


$$
\left[\begin{matrix} x' \\ y' \end{matrix} \right] =\left[\begin{matrix}a & b\\ c & d \end{matrix}\right] \left[\begin{matrix} x \\ y \end{matrix} \right] + \left[\begin{matrix} t_x \\ t_y\end{matrix} \right]
$$


齐次坐标通过增加一维坐标来表示平移变换。

- 2D point ：$(x,y,1)^T$
- 2D vector：$(x,y,0)^T$

在齐次坐标系中，$(x,y,\omega)^T$ 表示一个 2D point $(\frac{x}{\omega}, \frac{y}{\omega},1)^T$ ，其中 $\omega \ne 0$

!!! success "正确性"
	- vector + vector = vector
	- point  -  point = vector
	- point  - vector = point

那么平移可表示为：

$$
\left[\begin{matrix} x' \\ y'  \\\omega'\end{matrix} \right] =\left[\begin{matrix}1 & 0& t_x \\ 0 & 1 & t_y  \\ 0 & 0 & 1\end{matrix}\right] \left[\begin{matrix} x \\ y \\ 1 \end{matrix} \right]  = \left[\begin{matrix} x+t_x \\ y+t_y \\ 1\end{matrix} \right]
$$

### Affine Transformations 仿射变换

重新看待下列二维变换+平移：

$$
\left[\begin{matrix} x' \\ y' \end{matrix} \right] =\left[\begin{matrix}a & b\\ c & d \end{matrix}\right] \left[\begin{matrix} x \\ y \end{matrix} \right] + \left[\begin{matrix} t_x \\ t_y\end{matrix} \right]
$$

在齐次坐标下，上式表示为：

$$
\left[\begin{matrix} x' \\ y'  \\\omega'\end{matrix} \right] =\left[\begin{matrix}a & b& t_x \\ c & d & t_y  \\ 0 & 0 & 1\end{matrix}\right] \left[\begin{matrix} x \\ y \\ 1 \end{matrix} \right] 
$$

!!! warning "先线性变换，再平移！！！"

相应的，用齐次坐标分别表示缩放、旋转的矩阵：

**Scale：**

$$
S(s_x, s_y)=\left[\begin{matrix}s_x & 0 & 0 \\ 0 & s_y & 0 \\ 0 & 0 & 1\end{matrix}\right]
$$

**Rotate：**

$$
R(\alpha) = \left[\begin{matrix}\cos \alpha & -\sin \alpha & 0 \\ \sin \alpha & \cos \alpha & 0 \\ 0 & 0 & 1\end{matrix}\right]
$$

**Translation：**

$$
T(t_x, t_y) =\left[\begin{matrix}1 & 0& t_x \\ 0 & 1 & t_y  \\ 0 & 0 & 1\end{matrix}\right]
$$

## Composing Transforms

!!! example "矩阵乘法不满足交换律"
	![[composingtransformationsexample.png]]


有时候旋转操作很难把握，可以先把点移动回原点，进行旋转后再移动回去：

![[decomposingcomplextransforms.png]]

## 3D Transformations

Use **homogeneous coordinates** again:

- 3D point  = $(x,y,z,1)^T$
- 3D vector = $(x,y,z,0)^T$

同样，在 $\omega \ne 0$ 的情况下，$(x,y,z,\omega)^T$ 表示一个三维点 $(\frac{x}{\omega},\frac{y}{\omega},\frac{z}{\omega},1)^T$ 

那么，三维空间的仿射变换如下：

$$
\left[\begin{matrix}x' \\ y'  \\ z' \\ 1\end{matrix}\right] = \left[\begin{matrix}a & b& c & t_x \\ d & e & f & t_y  \\ g & h & i & t_x \\ 0 & 0 & 0 & 1\end{matrix}\right] = \left[\begin{matrix}x \\ y \\ z \\ 1\end{matrix}\right] 
$$

**Scale：**

$$
S(s_x, s_y, s_z)=\left[\begin{matrix}s_x & 0 & 0 & 0\\ 0 & s_y & 0  & 0\\ 0 & 0 & s_z & 0 \\ 0 & 0 & 0 & 1\end{matrix}\right]
$$

**Translation：**

$$
T(t_x, t_y, t_z) =\left[\begin{matrix}1 & 0 & 0 & t_x \\ 0 & 1 & 0 & t_y  \\ 0 & 0 & 1 & t_z \\ 0 & 0 & 0 & 1\end{matrix}\right]
$$

**Rotate：**

$$\begin{array}c
R_x(\alpha) = \left[\begin{matrix}1 & 0 & 0 & 0\\0 & \cos \alpha & -\sin \alpha & 0 \\ 0 & \sin \alpha & \cos \alpha & 0 \\ 0 & 0 & 0 & 1\end{matrix}\right] \\
R_y(\alpha) = \left[\begin{matrix}\cos \alpha & 0 & \sin \alpha & 0\\0 & 1 & 0 & 0 \\ -\sin \alpha & 0 & \cos \alpha & 0 \\ 0 & 0 & 0 & 1\end{matrix}\right] \\
R_z(\alpha) = \left[\begin{matrix}\cos \alpha & -\sin \alpha & 0 & 0\\ \sin \alpha & \cos \alpha & 0 & 0\\0 & 0 & 1 & 0 \\0 & 0 & 0 & 1\end{matrix}\right]
\end{array}$$

!!! warning "需要注意绕y轴的旋转矩阵 $\sin$ 的正负性不一样"

任意三维旋转可表示为组合矩阵：

$$
R_{xyz}(\alpha, \beta, \gamma) = R_x(\alpha) R_y(\beta) R_z(\gamma)
$$

另有 Rodrigues' Rotate Formula，可用来计算绕任意轴 $n$ 旋转角度 $\alpha$：

$$
R(n,\alpha) = \cos (\alpha) I + (1-\cos(\alpha))nn^T + \sin(\alpha) \left[\begin{matrix}0 & -n_z & n_y \\ n_z & 0 & -n_x \\ -n_y & n_x & 0\end{matrix}\right]
$$

使用时通常先把轴平移至原点，旋转完后再平移回去。

## Viewing Transformation

### View/Camera Transformation

把三维空间的物体，变成二维空间的图。

定义一个摄像机，需要三个向量：

- Position $\overrightarrow{e}$
- Look-at/gaze direction $\hat{g}$
- Up direction $\hat{t}$

为了简化操作，我们可以定义相机放置于原点 $\overrightarrow{e}=(0,0,0)^T$ ，向上方向沿 y 轴 $\hat{t}=(0,1,0)^T$，视角方向沿 -Z 轴看 $\hat{g}=(0,0,-1)^T$

那么，将相机移动至指定位置的矩阵记为 $M_{view}$：

$$\begin{array}c
M_{view} = R_{view} T_{view} \\
T_{view} = \left[\begin{matrix}1 & 0 & 0 & -x_e \\ 0 & 1 & 0 & -y_e  \\ 0 & 0 & 1 & -z_e \\ 0 & 0 & 0 & 1\end{matrix}\right]
\\ R_{view} = \left[\begin{matrix}x_{\hat{g}\times\hat{t}} & y_{\hat{g}\times\hat{t}} & z_{\hat{g}\times\hat{t}} & 0 \\ x_t & y_t & z_t & 0  \\ x_{-g} & y_{-g} & z_{-g} & 0 \\ 0 & 0 & 0 & 1\end{matrix}\right]
\end{array}$$

!!! note "Object 也要和 Camera 一起旋转，保证相对位置不变"

### Projection Transformation

投影分为两种：

- Orthographic projection 正交投影
- Perspective projection 透视投影

![[porjectiontransformationtouying.png]]

差别在于是否有近大远小的性质。

![[PTExample2.png]]

#### Orthographic Projection

- <1> Center cuboid by translating
- <2> Scale into "canonical" cube ($[-1,1]^3$)

![[Orthographicprojectione1.png]]


!!! info "右手系"
	课程用的是右手系，相机沿 -Z 方向观察，所以 z 值越大就越近；不过一些API（例如 OpenGL）用的是左手系，正好相反。

#### Perspective Projection

- Most common in Computer Graphics, art, visual system
- Further objects are smaller
- Parallel lines not parallel; converge to single point

透视投影可以看作先把原平面按比例挤压成近平面的大小，再对该区域进行正交投影。

![[nsppex1.png]]

我们需要考虑远平面和近平面之间的点 $(x,y,z,1)^T$ 的压缩。

假设远平面距离相机的 z 轴距离为 $f$ ，近平面距离相机的 z 轴距离为 $n$ ，根据相似三角形，可以得到 $x,y$ 的压缩比例为 $\frac{n}{z}$ ，那么压缩矩阵实现的效果为：

$$
\left[\begin{matrix}x \\ y \\ z\\ 1\end{matrix}\right] \Rightarrow
\left[\begin{matrix} nx/z \\ ny/z \\ ?\\ 1\end{matrix}\right]==\left[\begin{matrix}nx \\ ny \\ z\cdot ?\\ z\end{matrix}\right]
$$

因此我们已经可以推导出压缩矩阵的第一、二、四行，表示为：

$$
M_{persp->ortho}=\left[\begin{matrix}n & 0 & 0 & 0 \\ 0& n & 0 & 0 \\ ? & ? &? & ? \\ 0 & 0 & 1 & 0\end{matrix}\right]
$$

关于第三行的推导，可代入两个不会被压缩改变的特殊点：

- 位于近平面的点 $(x,y,n,1)^T$
- 位于远平面中心的点 $(0,0,f,1)^T$

并且由于 $n^2$ 与内部点的 $x,y$ 坐标没有关系，可以判断出第三行前两位均为 0，那么：

$$\begin{array}c \left[\begin{matrix}x \\ y \\ n \\ 1\end{matrix}\right] \Rightarrow\left[\begin{matrix}nx \\ ny \\ n^2 \\ n\end{matrix}\right]\\
\Rightarrow \left[\begin{matrix}0 & 0 & A & B\end{matrix}\right]\left[\begin{matrix}x \\ y \\ n \\ 1\end{matrix}\right] = n^2 \\
\Rightarrow \ \ \ \ \ \ \ \ \ n\cdot A +B = n^2 \\
\text{Samely: } f\cdot A+B = f^2 \\
\Rightarrow \left\{\begin{array}l A=n+f \\ B=-nf\end{array}\right .
\end{array}
$$

即：

$$\begin{array}c
M_{persp->ortho}=\left[\begin{matrix}n & 0 & 0 & 0 \\ 0& n & 0 & 0 \\ 0 & 0 & n+f & -nf \\ 0 & 0 & 1 & 0\end{matrix}\right] \\
M_{persp} = M_{orhto} M_{persp->ortho}
\end{array}
$$

对于内部的点：

$$
z' = n+f - \frac{nf}{z} >z
$$

因此内部的点会被推向远平面。