
# IPC

> https://ipc-sim.github.io/
> https://zhuanlan.zhihu.com/p/154542103
> https://clv-iclucia.github.io/2024/09/19/ipc-tips/

## Overview

IPC 全称 Incremental Potential Contact。其中 Incremental Potential 是基于**数学优化**的**有限元物理仿真**方法中，在每一个时间步中要最小化的函数。Contact 包括碰撞和摩擦，在仿真中作为额外的约束条件存在。

IPC 将物体的 Contact 巧妙地融合到目标函数 Incremental Potential 中，能够达到精确、稳定、无穿透的效果。

## Contact Model

### Position Update

在不考虑碰撞时，弹性物体的所有采样点 $x$ 在每个时间步可通过最小化 Incremental Potential $E(x)$ 来更新，我们使用的是**隐式欧拉时间积分**：

$$
x^{t+1} =argmin_x E(x)=argmin_x \left( \frac{1}{2} ||x-\hat{x}^t||_M ^2 + \Delta t^2 \Psi (x) \right )
$$

- $\hat{x}^t= x^t + v^t \Delta t + M^{-1} f_{ext} \Delta t^2$
	- $v^t$ 取 $(x^t - x^{t-1}) / \Delta t$
- $f_{ext}$ 指该点所受外力（包括重力）
- $M$ 为采样点的质量矩阵
	- $||x-\hat{x}^t|| _M^2= (x-\hat{x}^t)^TM (x-\hat{x}^t)$
- $\Psi$ 是弹性势能

我们使用牛顿迭代法稳定、精确求解 $E(x)$ 的最小化。

考虑碰撞时，则需要额外加上一个“无碰撞”的约束条件，IPC 认为，如果准确算出物体表面每一个“点-三角对”和“边-边对”的距离，在初始状态无重叠的情况下，保证所有这些距离在模拟的每一个时间步 $\Delta t$ 中的优化每一次迭代轨迹的每一处都不为 0，则物体之间就不会发生穿透。

### Distance Compute

`ipc-toolkit` 的所有 distance 相关函数都是距离的平方，因为开方的运算实际是相当 Costly 的。


### Constraint Set

IPC 的约束主要有 边-边 约束 和 点-三角 约束。只有在我们的 $\hat{d}$ 范围内的约束才是“活跃”的，因此在每次发生变化时都要重新计算约束。

### Barrier Potential

出现在 Section 4.2