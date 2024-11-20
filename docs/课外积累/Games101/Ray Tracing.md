# Ray Tracing

!!! quote "Why Ray Tracing"
	相比于只使用光栅化，光线追踪质量较高，但是运行非常缓慢。因此，对于实时画面我们采用光栅化，对于离线画面我们使用光线追踪。

## Shadow Mapping

在点光源条件下，判断某一点不在阴影中的根据为该点同时在相机和光源的视野内。

- **Step1** 从点光源位置进行光栅化但不进行着色及之后的操作，将得到的深度结果记录；
- **Step2** 将相机视角中的某一点投影回光源的深度图像中，比较该点到光源的距离与该点在深度图中记录的深度，如果深度一致，说明该点不在阴影中；否则，在阴影中。

![[shadowmappingwithdepthimage.png]]

如果光源有一定大小，就会形成软阴影，阴影颜色越淡，照到上面的光越多。

![[softhardshadows.png]]

## Whitted-Style Ray Tracing

在图形学中，我们定义光线有以下属性：

- <1> 光线沿直线传播
- <2> 光线之间不发生碰撞
- <3> 光线从光源传播到相机

根据光路的可逆性，我们可以假设光从我们的相机出发，经过不断投射到达光源。

相机发出的光可以经过多次反射或折射，在到达的每一个点都判断一次与光源的距离和角度关系，分别计算着色值，最后相加：

![[whittedraytracing1.png]]

### Ray-object Intersection

光线由起点 $o$ 和传播方向 $d$ 定义：

$$
r(t)=o+td,\ 0\le t\lt \infty
$$

一个球面由球心 $c$ 和半径 $R$ 定义：

$$
p:\ (p-c)^2 -R^2 =0
$$

那么光线和球的交点必须同时满足以上两个方程：

$$\begin{array}l
(o+td-c)^2 -R^2=0 \\
at^2 +bt+c=0, where \\
a=d\cdot d \\
b= 2(o-c)\cdot d \\
c=(o-c)\cdot (o-c)-R^2
\end{array}$$

将其推广到一般**隐式表面** $f(p)=0$ ：

$$
f(o+td)=0
$$

!!! info "得到的有效解要求为实数、大于 0"

对于**显式表面**，如 Triangle Mesh，可以将每个三角形都与光线方程作判断，但是可想而知计算量是非常大的。

为简化计算，可以先求出光线和三角形平面的交点，再判断该交点是否处于三角形内。

一个平面由其上一点 $p'$ 和其法线 $N$ 定义：

$$
p:\ (p-p')\cdot N=0\ \Rightarrow \ ax+by+cz+d=0
$$

那么将光线方程带入求交点：

$$
(o+td-p')\cdot N=0 \Rightarrow t=\frac{(p'-o)\cdot N}{d\cdot N}
$$

!!! info "注意表达式中都是向量点乘"

而判断交点是否处于三角形内在光栅化章节中已学过，此处不再展示。（分别于三角形三条边叉乘）

Moller Trumbore Algorithm 可以通过重心坐标一步到位判断三角形与光线的交点：

$$
\begin{array}c
O+tD=(1-b_1 -b_2)P_0 +b_1 P_1 +b_2 P_2 \\
\left [ \begin{matrix}  t \\ b_1 \\ b_2\end{matrix}\right ] = \frac{1}{S_1\cdot E_1} \left [ \begin{matrix} S_2 \cdot E_2 \\ S_1 \cdot S \\ S_2 \cdot D \end{matrix}\right ]
\end{array}
$$

其中：

$$\begin{array}c
\overrightarrow{E_1}=P_1 -P_0 \\
\overrightarrow{E_2}=P_2 -P_0 \\
\overrightarrow{S} =O-P_0 \\
\overrightarrow{S_1} =D\times E_2 \\
\overrightarrow{S_2} = S\times E_1
\end{array}$$

### Bouding Volume

为了进一步提升光线和三角形表面求交的速度，我们引入包围盒的概念（or 包围体积）：我们将一个复杂图形包围在包围盒内，如果光线不能命中包围盒，那光线也一定不能命中盒内的表面。

在三维空间中，我们通常使用轴对齐包围盒作为 BOX，它通常是由三对平面形成的交集，且每对平面都与 $XY$ 或 $YZ$ 或 $XZ$ 平面平行。

![[axisalignedboundingbox.png]]

我们以二维的 Axis-Aligned Bounding Box 为例：

![[2DAABB.png]]

对每一对平面计算光线进入和出去的时间，则最后光线在 BOX 内的时间为时间的交集。

- **Key Ideas**
	- The ray enters the box only when it enters all pairs of slabs
	- The ray exits the bos only when it exists any pair of slabs

$$
t_{enter}=\max \{t_{min}\}, t_{exit}=\min \{t_{max}\}
$$

不过由于光线并不是一条直线，而是射线，我们通过对进入和离开时间的正负值判断即可得到是否有交点：

- If $t_{exit}\lt 0$
	- 盒子在光线后面 - no intersection
- If $t_{exit} \ge 0,t_{enter} \lt 0$
	- 光线起点在盒子内部 - have intersection

即有交点的条件当且仅当 $t_{enter} \lt t_{exit}\ \&\&\ t_{exit} \ge 0$ 

#### Uniform Grids

在实践中，我们将包围盒划分成一个个小格子，并对其进行预处理，判断某一格子内是否有物体表面存在。

![[howtoaccewithaabbs.png]]

在光线射入的过程中，如果射入的该格子内不存在表面，则可以直接略过；如果存在，则需要进行表面和光线的交点判断。

!!! info "基本理论在于光线与盒子求交速度远大于光线与三角形面求交"

同时，我们需要对划分的格子数也做一个平衡。在实际算法中：

$$
\begin{array}c
\#cell =C* \#objs \\
C = 27\ \text{in 3D}
\end{array}
$$

#### Spatial Partitions

另一种方法是采用空间划分

![[spatialpartition1.png]]

- **Oct Tree** 八叉树
	- 在空间中，将 Bounding Box 划分成等大八块，如果小块内有物体，则继续切成八块，不断递归；如果盒内为空，则不再划分。
- **KD Tree**
	- 交替进行 $X,Y,Z$ 方向上的划分
- **BSP Tree**
	- 与 KD Tree 略有不同，其划分方向上没有要求

所以给定一个场景，需要使用 KD Tree 预处理。

![[KDpreprocessing.png]]

!!! note "实际物体列表只存储在 KD Tree 的叶节点上"

#### Object Partitions & BVH

Bounding Volume Hierarchy(**BVH**) 是非常广泛引用的加速结构，它解决了上述两个方案的缺点。

![[bvhe1.png]]

其思想为将物体划分为多个部分，对每部分求出它们的包围盒，这样一个物体只会出现在一个包围盒内，缺点是 Bounding Box 可能会存在重合部分。

- How to subdivide a node ?
	- Choose a dimension to split
	- **Heuristic 1** Always choose the longest axis in node
	- **Heuristic 2** Split node at location of median object
- Internal nodes store
	- Bounding Box
	- Pointers to child
- Leaf nodes store
	- Bounding Box
	- List of objects

!!! quote "Accelerate Pros & Cons"
	![[accprocons.png]]


## Basic Radiometry

尽管 Whitted-Style 光线追踪能够得到较好的结果，但是这个模型对于物理世界的模拟存在很多谬论，也就不可能得到真实世界的光线情况。

在辐射度量学中，光具有以下四个空间属性：

- **Radiant Flux** 辐射通量
- **Intensity** 光照强度
- **Irradiance** 辐射照度
- **Radiance** 辐射亮度


![[morepropertyoflight.png]]

???+ tip "Radiant Flux"
	Radiant Energy 是电磁辐射的能量，但是在图形学中并不常用。而 **Radiant Flux** 是单位时间内 emitted,reflected,transmitted, or received 的能量，即 energy per unit：
	
	$$\begin{array}cQ[J=Joule] \\ \Phi = \frac{dQ}{dt}[W=Watt][lm=lumen] \end{array}$$

???+ tip "Intensity"
	定义 **Radiant Intensity** 为 power per unit solid angle emitted by a point light source.
	
	$$I(\omega) = \frac{d\Phi}{d\omega}[\frac{W}{sr}][\frac{lm}{sr}=cd=candla]$$
	
	其中立体角 Solid Angle 为面积除以半径平方，那么整个球的立体角为 $4\pi$
	
	$$d\omega = \frac{dA}{r^2}=\sin \theta d\theta d\phi $$
	
	![[solidanglewithweifen.png]]

???+ tip "Irradiance"
	定义 **Irradiance** 为 power per unit area incident on a surface point.
	
	$$E(x)= \frac{d\Phi (x)}{dA}[\frac{W}{m^2}][ \frac{lm}{m^2}=lux]$$
	
	此时再回过头去看着色模型中的 Lambert's Cosine Law：
	
	![[radiometrylambertcosine.png]]

???+ tip "Radiance"
	定义 **Radiance** 为 power per unit solid angle, per projected unit area，即一个微小的面，向一个微小的方向射出的能量：
	
	$$L(p,\omega)= \frac{d^2 \Phi (p,\omega)}{d\omega dA \cos \theta}[nit]$$
	
	Radiance 用来描述光线，而光线追踪就是计算 radiance。
	
	![[radiencefigure.png]]

我们可以将 Radiance 和前面两个基本属性联系起来：

- **Radiance：** Irradiance pre solid angle
	- $L(p,\omega) = \frac{dE(p)}{d\omega \cos \theta}$
- **Radiance：** Intensity per projected unit area
	- $L(p,\omega)=\frac{dI(p,\omega)}{dA\cos \theta}$

其中，**Irradiance** 和 **Radiance** 的关系尤为重要。

Irradiance 是单位面积 $dA$ 接受到的能量总和，而 Radiance 是从单位立体角射到 $dA$ 的能量：

![[irradianceversusradiance.png]]

## Bidirectional Reflectance Distribution Function

双向反射分布函数(BRDF)用来解决已知入射光能量和角度，求射到物体表面向各个方向辐射出去的能量和角度。

我们假设物体表面并不是直接反射光线，而是将光线能量吸收，再重新发射出去，则对于一单位面积，其用来决定入射方向为 $\omega_i$ 的光线能量和出射方向为 $\omega_r$ 的光线关系的BRDF函数为：

$$
f_r (\omega _i \rightarrow \omega _r) =\frac{dL_r(\omega _r)}{dE_i( \omega _i)} = \frac{dL_r(\omega _r)}{L_i(\omega _i)\cos \theta_i d\omega_i}
$$

![[therelectionfequation.png]]

那么，对入射方向进行一次积分，可到到该出射方向的光线：

$$
\Rightarrow L_r (p,\omega_r)=\int _{H^2} f_r (p,\omega _r) L_i (p,\omega _i) \cos \theta _i d\omega _i 
$$

!!! info "$L_e(p,\omega_0)$ 是物体本身的光。不发光则为 0"

再考虑到物体本身可能存在的发光，最终可以得到更一般化的渲染方程：

$$
\Rightarrow L_o (p,\omega_o)=L_e(p,\omega_o) + \int _{\Omega ^+} f_r (p, \omega_i, \omega _o) L_i (p,\omega _i) (n\cdot \omega_i) d\omega _i
$$

但是实际渲染中，光线从多个方向射入，使得上述积分难以计算。在此，我们不深入证明的给出全局光照的方程：

$$
L=E+KE +K^2E +K^3E+...
$$

$E$ 就代表直接从光源发出的光，$KE$ 表示直接光照，即光照射到表面后反射一次的光照，相应的 $K^nE$ 表示光弹射 n 次后的光照。

## Probability Review

