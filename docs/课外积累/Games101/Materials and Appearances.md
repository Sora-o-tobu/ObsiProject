
# Materials and Appearances

## Matrials Overview

在图形学中，材质 == BRDF，即渲染方程中的 $f_r$ 。

对于漫反射面(Diffuse)，其 BRDF 和 $L_i(\omega _i)$ 都可视为常数，则其 BRDF 取值为 $[0, \frac{1}{\pi}]$：

![[diffusematerials1.png]]

对于每个颜色，都可设置其对应的 BRDF，将其看作多维向量看待即可。

某些表面整体呈光滑的镜面，但是局部又有点凹凸不平，这种材质称为 Glossy，例如古代技术不那么发达的铜镜等。

=== "反射"
	![[perfectspecularreflection.png]]
=== "折射"
	![[perfectrefraction1.png]]


## Microfacet Material

距离足够远时，表面上很多微小的细节都看不到，即微表面模型：宏观上看外观，微观上看几何

![[microfacetBRDF.png]]

则微表面模型的 BRDF 定义如下：

$$
f(i,o)=\frac{F(i,h)G(i,o,h)D(h)}{4(n, i)(n,o)}
$$

- $F(i,h)$ Fresnel Term, 菲涅尔方程，描述了物体表面在不同入射光角度下反射光线所占比率
- $G(i,o,h)$ Geometry Function, 几何函数，描述了微表面自遮挡的属性。当一个平面相对粗糙时，平面表面上的微平面可能挡住其它微平面从而减少反射的光线
- $D(h)$ Normal Distribution Function, 法线分布函数，其代表微观角度下所有微小镜面法线分布情况。粗糙表面法线分布均匀，光滑表面法线分布集中。

> 请看这篇 [基于物理的渲染：微平面理论(Cook-Torrance BRDF推导)](https://zhuanlan.zhihu.com/p/152226698)

## 各向同性、各向异性

具有各向同性的材质，其微平面上法线分布较为规律。

![[gexiangtongixnghegexiangyixing.png]]

从 BRDF 上考虑，如果分布函数在不同方位角上不同，则为各向异性：

$$
f_r(\theta _i, \phi_i; \theta_o, \phi _o) \ne f_r(\theta _i, \phi_i -\phi; \theta_o, \phi _o -\phi) 
$$

!!! info "理论上，$\phi_o = (\phi_i - \pi)\% 2\pi$"

## Properties of BRDFs

- **Non-negativity** 非负性
- **Linearity** 线性性，可以分别计算相加
- **Reciprocity principle** 可逆性，交换入射方向和出射方向，得到的 BRDF 完全相同
- **Energy conservation** 出射的能量小于等于入射的能量
- **Isotropic, Anisotropic** 
	- 如果各向同性，可以把 BRDF 从四维降至三维 $f_r(\theta _i, \phi_i; \theta_o, \phi _o)= f_r (\theta_i ,\theta_o, \phi_o -\phi_i)$
	- 