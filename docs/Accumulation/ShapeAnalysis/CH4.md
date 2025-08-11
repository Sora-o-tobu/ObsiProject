
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
	
	举个例子，一个正方体是流形网格，但是：
	
	- 如果把两个立方体通过 **一条边**焊接在一起 → 这个共享边就是 **非流形边**；
	- 如果两个立方体只通过 **一个顶点**焊接 → 这个点就是 **非流形顶点**。


