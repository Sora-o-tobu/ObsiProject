
# Shading

## Illumination & Shading

### Blinn-Phong Reflection Model

本课程中，Shading 定义为对一个物体应用不同的材质。

![[simplemodelshading.png]]

- Specular highlights - 高光
- Diffuse reflection - 漫反射
- Ambient lighting - 环境照明

为了计算局部特定点上光照与相机的关系，我们定义如下输入：

![[shadinglocal.png]]

- View direction, $v$
- Surface normal, $n$
	- 在极小范围内，都可以认为平面是无弯曲的
- Light direction, $l$
	- For each of many lights
- Surface parameters
	- color, shininess...

<font style="font-weight: 1000;font-size: 20px" color="red">对于漫反射 Diffuse：</font>


- 光线均匀反射在所有方向
	- 对于任意 View direction, Surface color 都是相同的
- 接受到的光线总量： $\cos \theta = l\cdot n$
	- 由于一般考虑单位面积，所以使用 $\cos \theta$ 描述也是足够的

![[lamberttheorm.png]]

对于点光源，我们知道任意球面光线总能量相同，那么到达离点光源 $r$ 处某一单位面积上的光线能量为：

$$
E = I/r^2
$$

最终得出漫反射的光线为：

$$
L_d = k_d (I / r^2) \max (0, n\cdot l)
$$

其中 $k_d$ 是 diffuse coefficient(color)，反应该材料对不同颜色光的吸收率。

![[kdeffect.png]]

<font style="font-weight: 1000;font-size: 20px" color="red">对于高光 Specular：</font>

- 光线强度取决于 view direction
	- 当 $v$ 角度与镜面反射光路线接近时，能接收到光线

定义半程向量：

$$
h = bisector(v,l) = \frac{v+l}{||v+l||}
$$

![[speclurvector.png]]

那么高光的光线为：

$$
L_s = k_s (I / r^2)\max (0, n\cdot h)^p
$$

向量之间夹角的余弦确实能够体现角度的接近程度，但是对于高光来说容忍度过高，因此需要在角度上作 $p$ 次方。

![[cosinepower.png]]

!!! info "通常，p 取 100 ~ 200"

不同的 $k_s$ 和 $p$ 的效果如下：

![[differentpeffect.png]]

<font style="font-weight: 1000;font-size: 20px" color="red">对于环境光线 Ambient：</font>

对某个点接收到的环境光线计算是极其困难的，我们可以大胆假设来自环境的光强 $I_a$ 是固定的：

$$
L_a = K_a I_a
$$

- 环境光和向量没关系，是一个常数


![[phingmodeleffect.png]]

### Shading Frequencies

着色频率，就是着色要应用在哪些点上。

- Flat shading - shade each triangle
	- 对每一个三角形平面进行着色
	- 不适合光滑平面
- Gouraud shading - shade each vertex
	- 对每一个三角形的顶点进行着色
- Phong shading - shade each pixel
	- 对每一个像素进行着色

![[differenteffectofshadingmodel.png]]

可以看到，当我们的几何划分的较细时，可以使用相对简单的 Flat shading 得到类似的效果。

那么我们如何定义逐顶点的法线和逐像素的法线呢？

对于顶点，其法线可以取其周围三角形的加权平均：

$$
N_v = \frac{\sum N_i}{|\sum N_i|}$$

![[eachvertexn.png]]

对于平面内的像素，由于已知平面顶点的法线，可以利用重心坐标得出平滑过渡的法线

![[eachpixeln.png]]

## Graphic Pipeline

即实时渲染管线：

![[realtimerenderingpipeline.png]]

现在的CPU允许用户自己编程，去解决顶点和像素如何操作，需要我们使用图形学API（如 OpenGL）自己写一个 shader program。

## Texture Mapper

为了在一个物体的不同位置定义不同属性，我们引入了纹理映射。

任何的三维表面都可以展开为二维平面，这个二维平面即可称为纹理（Texture）：

![[texturemapptingexample1.png]]



![[texturemapperexample2.png]]

将三角形的顶点映射到 Texture 上的坐标 $(u,v)\in [0,1]^2$ ：

![[texturemapperexample3.png]]

为了实现插值算法， 我们引入重心坐标（Barycentric Coordinates）。

三角形内部的任意一点坐标，都可以由其顶点坐标的线性组合表示：

$$
(x,y)=\alpha A +\beta B +\gamma C,\ \ \alpha +\beta +\gamma = 1
$$

!!! failure "如果 $\alpha ,\beta ,\gamma$ 中有负数，则该点不在三角形内部"

那么重心坐标即 $(\alpha ,\beta ,\gamma)$。除此之外，重心坐标还可以通过面积关系求得：

![[barycentriccoordinates.png]]

利用向量叉乘等于围成的面积，我们将顶点坐标带入：

![[tbarcentriccoordinates.png]]

完成上述重心坐标的定义后，我么就可以对三角形内任意点进行插值：

$$
V=\alpha V_A +\beta V_B +\gamma V_C
$$

其中 $V$ 可以是 positions,texture,coordinates,color,normal,depth,material attributes...

!!! question "实际应用中，将三维空间的三角形投影到二维时，某一点的重心坐标很可能改变。因此我们默认选取三维重心坐标。"

```c++
for each rasterized screen sample (x,y):
	(u,v) = evaluate texture coordinate at (x,y); 
	// ↑ use barycentric cordinates
	texcolor = texture.sample(u,v);
	sample.color = texcolor; 
	// e.g. k_d of diffuse color
```

在很多时候，纹理较小，并不能和图像的分辨率一一对应。这时候很多坐标映射到纹理中时并不是整数值，需要我们对其进行额外的算法得到该点对应的纹理。

![[TextureMagnificationoverview.png]]

上图例子中，第一张图采用了最近整数值的算法，直接对映射坐标进行四舍五入，不过效果显而易见不是非常好。

**Bilinear** 算法使用了双线性插值来解决这个问题。对于一个我们需要采样的点（red point），我们考虑其周围四个 texture sample locations（black points）。依次对其横坐标和竖坐标进行线性插值，得到参数 $s,t$：

![[bilinearalgorithm.png]]

最后可以综合考虑周围四个 texel 的属性，得出我们需要的 texel。

另一种情况是，纹理非常大，此时相机看向远处一个地方的像素，它会涵盖相当大的一部分纹理，我们不能简单判断该像素究竟选择何种纹理。

![[textureverylargequestion.png]]

解决方法之一可以和先前反走样一样，使用 supersampling 得到一个很好的结果，但是这个算法的问题是超采样耗费太多时间。

另一个思路是不采样，而是去获取该像素对应的 texture 区域的平均值，例如 Mipmap 范围查询算法，其特点是：

- Fast 快
- Approximate 近似估计值
- Square 只能用于正方形

在拿到纹理之前，我们先对纹理进行预处理，得到不同层数的纹理图案：

![[mipmapprepocess.png]]

存储纹理所需的额外空间为 $1/3$ ，几乎不增加性能负担。

!!! info "这也是Mipmap被称为图像金字塔的原因"

例如，要对远处 $8\times 8$ 的像素集合映射上图纹理，那么每颗像素都要映射的 texel 为 $16\times 16$ ，即 Level 3。此时再对每个像素的映射值进行双线性插值，从而得到高质量的图像。

!!! note "遇到要使用 Level 1.8 ???"
	对两层都进行双线性插值，然后使用线性插值得到结果。
	
	![[level18.png]]

还有另一种比 Mipmap 效果更好的算法，叫各向异性过滤（Anisotropic Filtering），其差别在于给出的多层纹理并不是每层都减小一半，而是分别减少长宽一半：

![[anisotropicfilteringtexture.png]]

在我们看向平面时，一个像素对应的纹理区域很有可能对应着不规律的形状：

![[anisotropicfilteringtexture2.png]]

而各向异性过滤允许我们对横竖方向上长条形区域作快速的范围查询，能够解决一部分 Mipmap 解决不了的问题。

!!! warning "Appendix"
	还有环境贴图、凹凸贴图(Assignment3的Bump)、位移贴图(Assignment3的Displacement)等

