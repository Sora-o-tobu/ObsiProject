
# Approximation

## Overview

对于一个困难的 NPC 问题，有以下三种处理方法：

- <1> 如果 N 很小，那么指数级的复杂度也是可以接受的
- <2> 通过增加限制条件或只取关键结果从而在多项式时间内解决
- <3> 在多项式时间内找到接近最优解的答案，即 **approximation algorithm**
	- 牺牲精度，加快速度

!!! danger "Greedy算法并不能保证接近最优解，在某些题目中结果可能相差很大"

定义近似比 **Approximation Ratio** ：

$$
\rho(n) \ge \max (\frac{C}{C^*} ,\frac{C^*}{C})
$$

其中 $C$ 和 $C^*$ 分别是给定算法结果和最优解的量化。例如，在求最大最小值的问题中，可以直接定义 $C$ 为最终得到的最值结果。

其中，输入大小 $n$ 可以任意取值。

近似比用来刻画近似解和最优解的接近程度。

一个优化问题的**近似范式(Approximation Scheme)**定义为对于该问题的任意输入实例，它的近似比均小于等于 $1+\varepsilon \  (\varepsilon \gt 0)$ ，即 Approximation Scheme 为 $(1+\varepsilon)-\text{approximation algorithm}$ 。

当近似范式的时间复杂度关于输入大小 $n$ 为多项式级别时，我们称其为**多项式时间近似范式(polynomial-time approximation scheme, PTAS)**，例如 $O(n^{\frac{2}{\varepsilon}}),O(\frac{1}{\varepsilon}^2 n^3)$；而当时间复杂度关于 $1/\varepsilon$ 和 $n$ 都是多项式级别时，称其为**完全多项式时间近似复杂度(Fully polynimial-time approximation scheme, FPTAS)**，如上面的 $O(\frac{1}{\varepsilon}^2 n^3)$。

!!! example "对于旅行商问题，PTAS将生成一个长度最多为$(1+\varepsilon)L$的解，其中L为最短旅行长度"

关于算法的设计，我们考虑这三个维度：

1. **最优性(optimality):** 即能求准确解；
2. **高效性(efficiency):** 即算法是否高效；
3. **普遍性(all instances):** 即算法是否普遍适用于所有的情况；

倘若一个解法：

1. 同时满足最优性和高效性，那么这个算法对特殊情况能高效求准确解；
2. 同时满足最优性和普遍性，那么这个算法对所有情况都能求准确解；
3. 同时满足高效性和普遍性，那么这个算法可能是个近似算法；

存在一个算法能够同时满足上面三个特性的前提条件是，能够证明 $P=NP$。

## 经典案例：近似装箱问题

给出 $N$ 个大小分别为 $S_1, S_2,... S_N$ 的物体，每个箱子的容量为 1，则最少需要多少箱子才能把所有物品装下。

暴力算法需要列举所有排列，所以它的时间复杂度为指数级，则装箱问题属于 $NPH$ 问题。

### Next Fit

NF 策略总是选择当前最后一个 bin，若能够容纳，则将当前 item 放入其中，否则新开一个 bin。

NF 策略总是使用不超过 2M−1 个 bin，其中 M 表示准确解的 bin 数。

**证明：**

首先我们能够确定，相邻两个箱子装的 Size 数和大于 1，即 $S(B_i) +S(B_{i+1}) \gt 1$ 。当 NF 算法的结果为 $2M-1$ 时：

$$
\begin{array}c
\begin{cases} S(B_1) +S(B_2) &\gt 1 \\ S(B_2) +S(B_3) &\gt 1 \\
& ...  \\
S(B_{2M-3}) +S(B_{2M-2}) &\gt 1  \\
S(B_{2M- 1}) &\le 1 \end{cases}
\\ \Rightarrow \sum_{i=1}^{2M-1}\gt \sum_{i=1}^{2M-2} \gt M-1 \\
\Rightarrow \sum_{i=1}^{2M-1} \ge M
\end{array}
$$

因此对于 $2M-1$，最优解起码需要 $M$ 个 Bin 才能装下所有。

而如果算法结果为 $2M$ ：

$$
\sum_{i=1}^{2M} \gt \frac{2M}{2} \Rightarrow \sum_{i=1}^{2M} \ge M+1
$$

即至少需要 $M+1$ 个 Bin 才能装下所有，矛盾。

### First Fit

FF 策略总是选择第一个能放下当前 item 的 bin，若所有 bin 都无法容纳当前 item，则新开一个 bin。

采用合适的数据结构，可以使该算法的时间复杂度为 $O(N\log N)$。

NF 策略总是使用不超过 ⌊1.7M⌋ 个 bin，并且存在一族能对边界取等的输入，其中 M 表示准确解的 bin 数。

### Best Fit

BF 策略总是选择能够容纳当前 item 且剩余空间最小的 bin（即 tightest），若所有 bin 都无法容纳当前 item，则新开一个 bin。

采用合适的数据结构，可以使该算法的时间复杂度为 $O(N\log N)$。

NF 策略也总是使用不超过 ⌊1.7M⌋ 个 bin，并且存在一族能对边界取等的输入，其中 M 表示准确解的 bin 数。

!!! failure "Online"
	上述三个方法都属于**在线算法(online)**，其特点在于对未来的数据不可知，且不能修改上一步所作的决策。在测试中，我们总能构造一组输入使得结果处于最坏的情况上。
	
	事实上，对于 online 算法，理论证明总能构造一组输入，使得算法结果最少为准确解的 $5/3$ 倍。

### First Fit Decreasing

FFD 属于离线算法，它将 item 按照 size 降序排序，而后使用 FF（或 BF，由于单调性，两者等价）。

FFD 策略总是使用不超过 $\frac{11}{9}M+ \frac{2}{3}$ 个 bin，并且存在一族能对边界取等的输入，其中 M 表示准确解的 bin 数。

## 经典案例：背包问题

体积为 $M$ 的背包，去装 $N$ 个物品。每个物品 $i$ 有重量 $w_i$ 和价值 $p_i$ 。

对于 Fractional Version ，一个物品可以选择只装一部分进入背包，则此时物品重量和价值同时乘以百分比。可以直接使用贪心算法，按照性价比从高到低装入即可得到最优解。

我们主要讨论的为 0-1 Version，即物品不能被分割，要么是 0 要么是 1，这是一个经典的 $NPH$ 问题。

### Greedy Algorithm

贪心策略可以选择当前能够装入的物品中，价值最高的或是性价比最高的。两种决策得到的近似比均为 2。

在此给出简单证明：

$$
\begin{array}l
(1)\ p_{max}\le P_{opt}\le P_{frac} \\
(2)\ p_{max}\le P_{greedy} \\
(3)\ P_{opt} \le P_{frac} \le P_{greedy} +p_{max} \\
(4)\ \rho = P_{opt} / P_{greedy} \le 1+ p_{max} / P_{greedy} \le 2
\end{array}
$$

- <1> 选择价值最高的贪心算法总价值小于 0-1 背包问题的最优解，而 fractional 背包问题的最优解大于 0-1 问题的最优解
- <2> $P_{greedy}= \max (p_{max}, p_{density})$ ，贪心算法的结果为两种策略的最大值
- <3> 考虑 $P_{frac} \le p_{density} + p_{max}\le P_{greedy} +p_{max}$
	- fractional 最优解和性价比优先贪心算法的差别就在于背包是否留下空位，而这个空位的价值一定小于 $p_{max}$ 
- <4> 不等式两边除以 $P_{greedy}$ ，得到近似比小于 2

### Dynamic Programming

$W_{i,p}$ 表示从物品 $\{1,...,i\}$ 中装下总价值 $p$ 所需要的最小重量。则动态规划的递归式为：

$$
W_{i,p} =\begin{cases}p==0?0:\infty, & i=0 \\
W_{i-1,p}, & p_i >p \\
\min (W_{i-1,p} , w_i + W_{i-1,p- p_i}), & \text{Otherwise} \end{cases}
$$

$i$ 的取值为 $1,2,...,n$，而 $p$ 的取值为 $1,..., np_{max}$，所以总时间复杂度为 $O(n^2 p_{max})$

```c
// assume n: 物品数量, pmax: 最大物品价值, dp[i][p]: 前 i 个物品中，总价值为 p 的最小重量
int MaxValue = 0;
for(int i = 0; i <= n ; i++ )
    for(int j = 0; j <= n * pmax; j++)
    {
        if(i == 0)
            dp[i][j] = j == 0 ? 0 : INF;
        else if(p[i] <= j)
            dp[i][j] = min(dp[i-1][j], dp[i-1][j - p[i]] + w[i]);
        else    
            dp[i][j] = dp[i-1][j];
        if(j > MaxValue && dp[i][j] <= W)
            MaxValue = j;
    }
```

虽然看上去是多项式时间复杂度，但是由于 $p_{max}$ 的值往往远大于 $n$ ，可能会达到指数级别。这时，我们可以选择牺牲一些精度，将 $profit$ 做一些舍入(向上取整)从而得到更小的 $p_{max}$ ：

![[dproundup.png]]

不过一般 0-1 背包问题采用如下动态规划求解：

```c
// assume n: 物品数量, m:背包容量, dp[i][j]: 前 i 个物品中，容量为 j 的最大价值
for(int i = 0; i <= n ; i++ )
    for(int j = 0; j <= m ; j++ )
    {
        if(i == 0 || j == 0) // initialize
            dp[i][j] = 0;
        else if(j < w[i]) // cannot put i-th item
            dp[i][j] = dp[i-1][j];
        else // can put i-th item
            dp[i][j] = max(dp[i-1][j], dp[i-1][j-w[i]] + p[i]);
    }
int MaxValue = dp[n][m];
```

!!! info "时间复杂度为 $O(nm)$，由于背包容量 m 本质上是指数级别的，所以并不是多项式级别复杂度"
	由于输入 m 的规模可以被手动压下去，所以这其实是一个伪多项式算法。伪多项式算法的优点在于很容易改造成 FPTAS 近似算法。

## 经典案例：K-Center Problem

在平面上有 $n$ 个点，需要在平面上画出 $k$ 个圆，要求点离最近的圆心的最大距离最小化。

直接正向求解可能无从下手，可以使用二分法尝试得到近似的答案。首先需要定义一个检测半径 $r$ 能否包围全部点的函数。

对于一个准确解的半径 $r^*$ ，我们以一个圆内任意一点为圆形，以 $2r^*$ 为半径画一个更大的圆，则这个新圆能够保证所有原来的圆内的点都被包含在内，则我们直接将这个两倍半径作为近似解。

```c
Centers  Greedy-2r ( Sites S[ ], int n, int K )
{   Centers  C[ ] = ;
    Select any s from S and add it to C;
    while ( |C| < K ) {
        Select s from S with maximum dist(s, C);
        Add s it to C;
    } /* end-while */
    return C;
}
```

!!! danger "该函数中，|C|相比准确情况偏小，所以只能保证结果的上界，下界则不准确"

若求出来所需的中心数大于 $K$ ，说明需要更大的最小半径才能包围所有点。而如果所需中心数小于等于 $K$ ，则尝试缩小最小半径，直到恰好不能包围所有点为止。

由于包围的圆采用两倍小圆的半径以包围小圆中的所有点，该近似算法的近似比为 $\rho =2$，即为 2-approximation，最终所求的结果也要乘以 2，即 $2r$。

!!! note "除非 $P=NP$，否则K-Center问题不存在 ratio < 2 的解决方法"

