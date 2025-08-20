
# Curvature

## Second Fundamental Form

为了衡量曲面偏离平面的程度，我们引入了曲率的概念。曲率不仅用来区分不同的曲面，还用来描述如何嵌入到空间中。

回忆之前定义的曲面法向，我们记**曲面（流形）**为 $\mathcal M$，曲面上 $\mathbf p$ 点的**单位法向量**为 $\mathbf n$，**切空间**为 $T_\mathbf p \mathcal M$：

![[SA_5_1.png]]

仿照曲线的 Gauss Map，我们可类似地定义曲面上的高斯映射，它将曲面的点映射到单位球上：

![[SA_5_2.png]]

同时为了方便后面的推导，我们引入映射微分的定义。

对于流形 $\mathcal{M}$ 到流形 $\mathcal{N}$ 的映射 $\varphi: \mathcal M \rightarrow \mathcal N$，在点 $\mathbf{p}\in \mathcal M$ 取一个切向量 $\mathbf v \in T_\mathbf p \mathcal M$，并且用穿过 $\mathbf p$ 的曲线 $\gamma(t)$ 来表示该向量：$\gamma(0)=\mathbf p,\ \gamma '(0)=v$。

则 $\varphi$ 在 $\mathbf p$ 处的微分 $d \varphi_\mathbf{p} : T_\mathbf{p} \mathcal{M} \rightarrow T_{\varphi(\mathbf{p})} \mathcal{N}$ 可以表示为：

$$d \varphi_\mathbf{p} (\mathbf v) = (\varphi \circ \gamma)' (0)$$

!!! note "这不依赖于 $\gamma(t)$ 的选取，而只与切向量 $\mathbf v$ 本身有关"

对于高斯映射 $\mathbf n: \mathcal M \rightarrow \mathbb S^2$，我们代入微分公式可以得到：

$$d \mathbf{n}_\mathbf{p} (\mathbf{v}) = (\mathbf{n} \circ \gamma)' (0)$$

由于 Gauss 映射 $\mathbf{n}$ 的像总在单位球面上，且 $\|\mathbf n \|^2 =1$，我们沿 $\gamma$ 求导可以得到：

$$
\frac{d}{dt}\left ( \mathbf n(\gamma (t)) \cdot ( \mathbf n(\gamma (t))  \right) |_{t=0} =2 \mathbf n(\mathbf p) \cdot 
(\mathbf{n} \circ \gamma)'(0) =0
$$

即 $(\mathbf{n} \circ \gamma)'(0) \perp \mathbf{n}_\mathbf{p}$，这表示 $d \mathbf{n}_\mathbf{p} (\mathbf{v})$ 一定位于切平面 $T_\mathbf{p} \mathcal{M}$ 上，即

$$d \mathbf{n}_\mathbf{p} (\mathbf{v}) \in T_\mathbf{p} \mathcal{M}$$

实际上 $d\mathbf{n}_\mathbf{p}$ 也称为 **shape operator**。

与 Shape Operator 对应的对称双线性型称为**第二基本型（Second Fundamental）**：

$$\mathbf{II} (\mathbf{v}, \mathbf{w}) = -\mathbf{v} \cdot d \mathbf{n}_\mathbf{p} (\mathbf{w})$$

如果我们将切平面上两个相同的向量 $\gamma'(0) = \mathbf{T} \in T_\mathbf{p} \mathcal{M}$ 送入第二基本形式 $\mathbf{II}$，可以发现第二基本形式连接了曲线与曲面的法向。首先利用切平面和曲面法向的定义有：

$$\gamma'(s) \in T_\mathbf{p} \mathcal{M} \Leftrightarrow \gamma'(s) \cdot \mathbf{n} (\gamma(s)) = 0$$

对两边同时微分可以获得：

$$\kappa \mathbf{N}(s) \cdot \mathbf{n} (\gamma(s)) + \mathbf{T}(s) \cdot d \mathbf{n}_{\gamma(s)} (\gamma'(s)) = 0$$

式中，$\mathbf{T}(s)$ 和 $\mathbf{N}(s)$ 分别为曲线 $\gamma(s)$ 的切向和法向，$\kappa$ 则是曲线的曲率。注意到左边第二项的形式可以改写为曲面的第二基本形式：

$$\mathbf{T}(s) \cdot d \mathbf{n}_{\gamma(s)} (\gamma'(s)) = \mathbf{T}(s) \cdot d \mathbf{n}_{\gamma(s)} (\mathbf{T}(s)) = -\mathbf{II} (\mathbf{T}, \mathbf{T})$$

因此有

$$\mathbf{II} (\mathbf{T}, \mathbf{T}) = \kappa \mathbf{N} \cdot \mathbf{n}$$

