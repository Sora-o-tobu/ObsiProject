
# Geometry

## Introduction

隐式几何： $f(x,y,z)=0$

!!! note "使用隐式表示，可能难以想想图形长什么样；但是很方便判断某一点是否在图形上"

显式几何：直接给出或通过参数映射给出各个点（或小三角形）的坐标

对于同样一个几何形体(y和z调换一下)：

![[samegeometry.png]]

=== "Implicit"
	$$f(x,y,z)=(2-\sqrt{x^2 +y^2}) ^2 +z^2 -1$$
=== "Explicit"
	$$f(u,v)=((2+\cos u)\cos v, (2+\cos u) \sin v, \sin u)$$


### Implicit

#### Constructive Solid Geometry


![[CSGEx1.png]]

#### Distance Functions

对每个点维护它们离几何图形最近的距离，当两个图形需要 Blend 时，将 Signed Distance Function 相加以更新：

![[maybesdfeffect.png]]

当我们对两个几何形体进行 Blend 时，面上某个点的距离函数更新为其对分别两个图形的距离函数的相加，则融合后的几何形体的边界为新的距离函数等于0的点。

![[whatissdf.png]]


#### Level Set Methods

与距离函数类似，但是是每个格子维护一个相对高度。

![[levelsetmehtodex1.png]]

!!! info "即等高线"

#### Fractals

即分形，可以类比算法中的递归，用来描述自相似的几何图形，但是较难控制形状。

比如雪花。

### Explicit

#### Point Cloud

用一组点表示几何图形，点云密度足够大才能表示一个较为复杂的模型。

在实际应用中通常需要转换为三角形面。

#### Polygon Mesh

多边形面表示是图形学中使用最广泛的显式几何，非常方便进行处理、仿真、采样等操作。

在 Wavefront Object File (`.obj`) 中，多边形面的格式如下：

```
v  0.23639098 17.838633 1.3139851    # 顶点坐标

vt 0.499264 0.29569197 0             # 纹理坐标

vn 0.41076726 0.49810898 -0.76364768 # 法线

f  12/11/15 12/11/15 12/11/15        # 连接关系，face
   # 例如 12/11/15 指第12个v，第11个vt，第15个vn为一组
   # 在Unity中相当于Mesh类的indices数组
```

## Curves

贝塞尔曲线使用一系列控制点来定义一段曲线。

先考虑只有三个点的贝塞尔曲线（quadratic Bezier）：

![[quadraticbezier.png]]

其中 $\frac{b_0 b_0^1}{ b_0 b_1} = \frac{b_1 b_1^1}{b _1 b_2} = \frac{b_0^1 b_0^2 }{b_0^1 b_1^1} = t$

数学上，易证明得到（上标表示层数，不是平方）：

$$\begin{array}c
b_0^1(t) =(1-t)b_0 +tb_1 \\
b_1^1(t) =(1-t)b_1 +tb_2 \\
b_0^2(t) =(1-t)b_0^1 +tb_1^1 \\
\Rightarrow b_0^2 = (1-t)^2b_0 +2t(1-t) b_1 +t^2b_2
\end{array}$$

同样适用于更多点数：

![[fourpointbeziercurve.png]]

那么有 Bernstein Form of order n：

$$\begin{array}c
b^n(t) = b_0^n(t) = \sum_{j=0}^n b_j B_j^n(t) \\
B_j^n(t) = C_n ^i t^i (1-t)^{n-i}
\end{array}
$$

其中 $B_j^n(t)$ 称为 Bernstein polynomial。

贝塞尔曲线有如下性质：

- 起点和终点满足：$b(0)= b_0, b(1)=b_n$
- 起点和终点的切线满足：$b'(0)=n(b_1 -b_0), b'(1)= n(b_n -b_{n-1})$
- 对控制点先做仿射变换再画贝塞尔曲线得到的结果与对控制点先画贝塞尔曲线再对曲线仿射变换一样
- 贝塞尔曲线一定位于控制点形成的凸包内

!!! info "Convex Hull"
	即能包围给定点的最小凸多边形

通常使用 Piecewise cubic Bezier curve （分段，每四个点，贝塞尔曲线）作为曲线描述。例如 PS 中给出的钢笔工具就是采用四个点作为控制点来画出贝塞尔曲线。

![[gangbigongju.png]]

!!! tip "连续性"
	若只是方向相同，定义为该处 C0 Continuity；若方向和距离都相同，定义为该处 C1 Continuity

## Surfaces

![[beziersuface1.png]]

For bi-cubic Bezier surface patch,

- Input: 4 * 4 Control Points
- Output: 2D surface parameterized by $(u,v)$ in $[0,1]^2$

具体做法是对每四个点都画出对应的贝塞尔曲线，再对每四个贝塞尔曲线画出画出一个 moving curve。 TOBEDONW

