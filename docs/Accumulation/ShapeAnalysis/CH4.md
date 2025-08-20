
# Surface

## Smooth & Discrete Surface

### Theoretical Definition

简单来说，我们可以认为曲面是一个二维平面通过一个可微映射 $f$ 嵌入到三维空间的结果：

![[SA_4_1.png]]

当 $Df$ 不满秩的时候会出现曲面退化的情况，因此我们要求曲面的 Jacobian 矩阵必须满足满秩的条件。

!!! quote "Jacobian Matrix"
	$$\begin{array}c f:\ \mathbb R^m \rightarrow \mathbb R^n \\ (Df)_j^i = \left ( \frac{\partial f^i}{\partial x^j} \right ) \end{array}$$

就和曲线一样，我们同样认为曲面也是一个具有特定属性的点集合，而不仅仅是一个 function。

我们认为曲面本身是一个三维空间中的对象，但是在曲面上的每一点的局部上仍然可以看作是一个二维的平面。这个平面被称为*切空间(tangent space)*。

我们可以从曲线的角度来理解切空间。对于曲面上经过 $\mathbf{p}$ 点的曲线 $\gamma$，我们定义 $\mathbf{p}$ 点的切空间为满足 $\gamma(0)=\mathbf{p}$ 的所有曲线在该处切向量的集合。

而如果从微分的角度来看，切空间可以认为是局部参数化的对象。实际上，这两种定义都是等价的。

![[SA_4_2.png]]



???+ note "在切空间的基础上，我们进一步定义 normal space，它是垂直于切平面法向量的集合"
	![[SA_4_3.png]]

### Discrete Representations

计算机中表示曲面最常用的方法是三角形网格，即使用三角形来近似局部的曲面：

$$
\begin{array}l
V= (v_1, v_2,..., v_n) \subset \mathbb R^3
\\ E= (e_1, e_2, ...,e_k) \subseteq V\times V \\
F=(f_1, f_2,..., f_m) \subset V\times V \times V
\end{array}
$$

!!! warning "实际应用中可能会有非流形网格"
	- **非流形边:** 一条边被三个或更多面共享
	- **非流形顶点:** 顶点周围邻域不能铺成拓扑圆盘
		- 拓扑即顶点之间的连接关系
	
	举个例子，一个正方体是流形网格，但是：
	
	- 如果把两个立方体通过 **一条边**焊接在一起 → 这个共享边就是 **非流形边**；
	- 如果两个立方体只通过 **一个顶点**焊接 → 这个点就是 **非流形顶点**。

我们可以把网格上任意顶点相连的边的数量称为顶点的 **valence** 或者 **degree**，利用这个概念就可以描述网格的一些拓扑性质。

![[SA_4_4.png]]

**Euler Characteristic** $\chi$ 是网格重要的拓扑性质，根据欧拉公式，我们有：

$$
V-E+F=\chi,\ \ \chi = 2-2g
$$

其中 $g$ 称为网格的亏格（genus），它可以理解为曲面上的孔洞数，例如一个咖啡杯的亏格 $g=1$，即它的杯把手处。上式说明对于任意形式的曲面，只要它们具有相同的亏格数，网格的顶点数、边数以及面数一定满足欧拉公式。

注意到没有边界的三角形网格，每条边都对应两个面，每个面对应三条边，我们可以得到面和边数量上的关系：

$$
2E=3F
$$

我们将其代入欧拉公式：

$$
V-E+F=V- \frac{1}{2}F=\chi 
$$

相比于边数，大多数情况下网格的 $\chi$ 都非常小，因此我们可以近似得到 $F \approx 2V$（等价于 $E\approx 3V$）。

!!! note "这样估计下来，三角形网格的 average valence = 6"

??? quote "Other Representation"
	除了三角网格外，具体应用中有非常多的备选方案。像是机器学习中常见的**有向距离场(signed distance field, SDF)**、流体仿真中常用的**隐式曲面(implicit surface)**甚至点云等都可以表示复杂的曲面。

### Data Structure

<font style="font-weight: 1000;font-size: 20px" color="orange">Triangle Soup</font>

最直观的表示方式是直接记录 Mesh 中每个 Face 的顶点坐标，形式如下：

```
x1, y1, z1 / x2, y2, z2 / x3, y3, z3 
x1, y1, z1 / x2, y2, z2 / x3, y3, z3
...
```

这在早期的一些图像库，例如 OpenGL 中有所使用，但是这样又浪费空间又无法轻易从中获得曲面拓扑性质，因此几乎不会在几何处理中使用。

<font style="font-weight: 1000;font-size: 20px" color="orange">Shared Vertex Structure</font>

另一种常见方式分别记录顶点的坐标和每个 Face 的顶点编号，从而避免了顶点坐标的反复记录：

```
v 0.2 1.5 3.2
v 5.2 4.1 8.9
...
f 1 5 3
f 5 1 2
...

```

!!! note "例如 `.obj` 格式就是这样存储的"

遗憾的是这种方式仍然对某些几何处理不友好，例如我们想要对网格平滑滤波，需要用到相邻顶点的坐标，此时想要直接获得相邻顶点的坐标是较为困难的。

<font style="font-weight: 1000;font-size: 20px" color="orange">Halfedge Data Structure</font>

> Complex data structures enable simpler traversal at cost of more bookkeeping.

我们需要设计一种局部性良好的数据结构来方便我们查询。

Halfedge 是网格处理中非常流行的数据结构。我们知道一条边被两个面所占有，因此半边指的是其在对应面的部分，半边的方向即为该面的方向。

在每个顶点上需要记录所有以该顶点为起点的半边，每个面上需要记录所有与该面相邻的半边，而每条半边自身还需要记录它的**反向半边(flip)**、**下一条半边(next)**以及半边自身的起点和所在面的编号。

除此之外，半边数据结构还可以方便转换为对偶形式。假设有一函数 $f\in \mathbb R^{|V|}$ 将每点映射为一个实数：

![[SA_4_5.png]]

那么假如我们希望对函数 $f$ 做积分，我们应该如何定义被积元素？

仿照曲线积分，我们为每一个顶点分配一个面积，而函数的积分等于顶点处函数值乘上 *dual cell* 的面积：

![[ObsiProject/docs/Accumulation/ShapeAnalysis/image/SA_4_6.png]]

实际上，根据 dual cell，我们可以在原始网格的基础上定义一个对偶网格，这样一个曲面实际上对应了两个网格：

![[SA_4_6 1.png]]

!!! tip "对偶网格也通过半边数据结构表示，两者可通过 $\star$ 算子通过旋转半边来转换"

