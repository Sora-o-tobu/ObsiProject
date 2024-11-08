
# Rasterization

!!! quote
	在做完 Model,View,Projection 的 Transformation 后，所有物体都会停留在 $[-1,1]^3$ 内，而要将这个正方体画在屏幕内，就需要进行光栅化。


## Canonical Cube to Screen

定义一个屏幕空间：

- 屏幕空间左下角定义为坐标原点
- 像素点索引定义为 $(x,y)$ 整数点对，且范围为 $(0,0)\ to\ (width -1,height-1)$
- 像素中心点位于 $(x+0.5,y+0.5)$
- 屏幕的覆盖范围为 $(0,0)\ to\ (width,height)$

关于 $x,y$ 坐标，直接将其从 $[-1,1]^2$ 拉成 $[0,width]\times[0,height]$，并把中心移动至 $(\frac{width}{2},\frac{height}{2})$， 使用的矩阵为：

$$
M_{viewport}=\left[\begin{matrix}\frac{width}{2} & 0 & 0 & \frac{width}{2} \\  0 & \frac{height}{2} & 0 & \frac{height}{2} \\ 0 & 0 & 1 & 0 \\ 0 & 0 & 0 & 1\end{matrix}\right]
$$

## A Simple Approach: Sampling

!!! info "采样是图形学中一个重要的概念"

给定一个三角形，如果一个像素的中心在该三角形内，将其定义为 1，否则定义为 0：

```c++
for (int x = 0;x < xmax; ++x)
	for (int y = 0;y < ymax; ++y)
		image[x][y] = inside(tri, x+0.5, y+0.5);
```

![[insidefunction.png]]

将点 $Q$ 与三条边分别作叉乘，每次叉乘根据正负性能够判断该点位于该边的左侧还是右侧。经过三次叉乘，即可判断该点是否位于三角形内部。

