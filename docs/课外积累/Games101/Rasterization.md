
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

如果该像素值为 1，那么就将其着色为该三角形的颜色；反之，不进行着色。最终可以得到类似下图的结果：

![[samplingresultwithoutantialiasing.png]]

## Antialiasing

上面得到的结果根本不像三角形！图片中的 Jaggies(锯齿) 在图形学中也被称为 Aliasing(走样)。

!!! info "Artifacts"
	表示图形学中一切 Errors,Mistakes,Inaccuracies 等不合理的结果。

- **Sampling Artifacts in Computer Graphics**
	- Jaggies - Sampling in space
	- Moire - Undesampling images
	- Wagon wheel effect - Sampling in time
	- \[Many more\]...


### Blurring/ Pre-Filtering

反走样一个可行操作是在采样之前进行一个模糊化处理。

![[blurringexample1.png]]

任何一个函数，都可以通过傅里叶展开变为三角函数的集合。因此，傅里叶变换可以将任意函数转换成带有不同频率的三角函数的和，并且能够通过逆变换将其转换回来。

![[fouriertransform.png]]

对变换出的三角函数采样，对于低频率的三角函数，我们容易通过采样点恢复出其原始信号；对于高频率的三角函数，我们不容易将其恢复：

![[samplingtrianglefunction.png]]

!!! success "What cause aliases???"
	对于下图蓝色高频率函数，我们将其采样点恢复成原始信号，得到黑色曲线，二者频率完全不同，但是通过同一个采样方法得到的采样结果却是完全相同的，这就导致了走样。
	
	![[whatcausealiases.png]]

因此，从频率角度上看，**Filtering**实现为去除一些频率段的内容：

=== "Orginal Image"
	![[originalimage.png]]
=== "High-pass Filter"
	![[highpassfilter.png]]
=== "Low-pass Filter"
	![[Lowpassfilter.png]]

对于一个图片，该处亮度变化越明显，对应的函数频率越高，亦即一个边界。因此，模糊操作可以通过卷积或平均实现。

![[ObsiProject/docs/课外积累/Games101/image/Convolutionexample1.png]]

卷积定理：在时域上进行卷积效果上等同于在频域上乘积再逆傅里叶变换变回去。

![[convolutiontheromexample1.png]]

再次在频域角度上看待采样，可以看作是对频谱的不断复制：

![[reviewsamplinginfrequencydomain.png]]

!!! danger "时域上采样率越高，频域上间隔越大（二者相反）"

那么，Aliasing也可以看作是复制的频谱之间出现重合导致的：

![[mixedfrequencycontent.png]]

我们不希望频域之间出现重叠，因此我们可以在采样之前对频域进行模糊（低通滤波）：

![[limitthenrepeat.png]]

### Practical solution

对于任何被三角形覆盖的像素，对其被覆盖的面积求平均，然后分配颜色：

![[practicalsolutionforantialiasing.png]]

不过实际上我们并不能去计算每个单位像素的被覆盖率，因此我们采取 Supersampling 措施（**MSAA**）

![[supersamplingpractice1.png]]

!!! warning "Supersampling并不是提高采样率，仅用作模糊操作"

以 $2\times2$ Supersampling 为例，第一步先确定所有采样点是否在三角形内部，第二部根据 Box 内在内部的采样点个数对其进行上色:

=== "Step1"
	![[supersamplingstep1.png]]
=== "Step2"
	![[supersamplingstep2.png]]

可以看出，如果使用 $2\times2$ 的 BOX，MSAA 相当于增大了 4 倍的计算量


## Visibility/ Occlusion

将三维物体映射到二维上，需要解决深度冲突的问题，最终需要显示的是最前方的颜色。

解决的思想为 Z-Buffer，该算法使用一个深度缓冲区来存储对应屏幕上像素的颜色值。当栅格化遍历到某一点时，先判断其是否位于三角形内，如果在，则再将其插值深度值（interpolated depth value）与深度缓冲区（depth buffer）中的相应值比较，如果前点更靠近相机，则设置像素颜色并更新深度缓冲区。

```c++
std::fill(depth_buf.begin(), depth_buf.end(), 
	std::numeric_limits<float>::infinity());
std::fill(frame_buf.begin(), frame_buf.end(), 
	Eigen::Vector3f{0, 0, 0});

for (each triangle T)
	for (each sample (x,y,z) in T)
		if (z < depth_buf[x,y]) {     // closest sample so far
			frame_buf[x,y] = rgb;     // update color
			depth_buf[x,y] = z;       // update depth
		} else; // do nothing, this sample is occluded
```

![[zbufferexample1.png]]

