
# Cloth

## Mass-Spring System

一个理想的弹簧满足胡可定理，即它的力与拉伸的形变长度成正比。对于多个弹簧组成的系统，系统的能量以及中心点所受的力表示为：

![[multiplespringeffect.png]]

对于一段布料，我们构造水平方向、对角线方向以及弯曲的弹簧，构成了一个 Network：

![[structurespringnetwork.png]]

!!! info "弯曲弹簧阻止布料向某一方向自由弯曲"

但是实际使用时，布料的 Mesh可能不是很规则、很结构化的方块。对于三角形网格，一种简单的思路就是认为其每条边都是一个弹簧，并未某一边相对的两个顶点都构造一个弯曲弹簧：

![[unstructurespringnetwork.png]]

有了这些弹簧后，我们就可以用显式或隐式的方法搞出 Mass-Spring System 来模拟布料。

### Explicit Integration

首先决定一个粒子状态的更新，将弹簧系统插入到力的计算过程中：

![[particlesystemupdate.png]]

!!! warning "在 $\Delta t$ 或者弹簧系数 $k$ 过大的情况下，显式积分做法很可能会产生 overshooting"

### Implicit Integration

另一种模拟方式称为隐式积分，认为速度的更新依赖于下一状态的力，位置的更新依赖于下一状态的速度。经过调整，可以将问题转换为求解关于变量 $x^{[1]}$ 的方程：

![[implicitintegratione0.png]]

问题在于，力的方程 $f(x)$ 可能不是关于 $x$ 的线性方程。所幸的是，由于 $x$ 是一个 $3\times N$ 的矩阵，里面包含了 $N$ 个顶点的位置信息，因此我们可以采用线性优化的方式简化求解：

![[optimizeproblemofsimplicit.png]]

!!! danger "具体原理听不懂啦"

## Bending and Locking Issues


