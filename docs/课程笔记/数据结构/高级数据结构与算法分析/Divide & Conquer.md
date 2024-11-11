
# Divide & Conquer

## Overview

To solve a problem recursively,we can:

- **Divide** the problem into a number of sub-problems
- **Conquer** the sub-problems by solving them recursively
- **Combine** the solutions to the sub-problems into the solution for the original problem

In general, the Time Complexity of recurence is:

$$
T(N) = aT(N / b) +f(N)  
$$

!!! quote "Cases solved by divide and conquer"
	- The maximum subsequence sum - the $O(N\log N)$ solution
	- Tree traversals - $O(N)$
	- Mergesort and quicksort - $O(N\log N)$

## Closest Points Problem

Given $N$ points in a plane. Find the closest pair of points.

If we use simple *Exhaustive Search*, there are $N(N-1) / 2$ pairs of answers to check, which means the time complexity can be $O(N^2)$

Then we consider using *Divde and Conquer* to solve it. It's just similar to the maximum subsequence sum problem(最大子序列和问题).

选取一个平行于 y 轴的直线，将平面分成两个部分，分别看作一个子问题求内部点之间最短路径。在已知两部分平面内部点各自最短路径的情况下，还需要求两部分之间的点互相的最短距离，这部分算法的时间复杂度将决定整个递归算法的时间复杂度：

若 Combine 的时间复杂度为 $O(N)$ :

$$\begin{array}l
T(N) & = 2T(N / 2)+cN \\ & = 2[2T(N / 2^2)+ cN / 2] +cN\\
& = 2^2 T(N / 2^2) +2cN\\
& = ... \\
& =2^kT(N / 2^k) +kcN\\
& =N+cN\log N = O(N\log N)
\end{array}$$

若 Combine 的时间复杂度为 $O(N^2)$

$$
\begin{array}l
T(N) & = 2T(N / 2)+cN^2 \\ & = 2[2T(N / 2^2)+ cN^2 / 2] +cN^2\\
& = 2^2 T(N / 2^2) +\frac{3}{2}cN^2\\
& = ... \\
& =2^kT(N / 2^k) +cN(1+\frac{1}{2}+...+\frac{1}{2^{k-1}})\\
& =O(N^2)
\end{array}
$$

所以，我们必须确定一个算法，使得 Combine 部分的时间复杂度为 $O(N)$。

由于我们已知左右两侧平面点之间的最小值，那么整个平面的距离最小值不会大于这个值。选取已知最小值小的那个 $\delta$ 作为阈值，在分割线左右两侧划分一个范围，且超出这个范围的点都不会在考虑范围之内：

![[cczgfwdddbzklfwn.png]]

按照统计理论，这两条线内部的点的个数一般在 $\sqrt{N}$ 内，因此时间复杂度为 $O(\sqrt{N}^2) = O(N)$ ，但作为计算机算法不能这样分析。

注意到，我们还可以对 y 坐标进行距离过滤，要求 $y_1 -y_2 \lt \delta$ ，此时要比较的两点有如下要求：

- $\Delta y \lt \delta$
- $\Delta x \lt \delta$
- 两点若在同一侧，则该两点之间距离大于 $\delta$

因此，满足条件的最多只有 7 个点，也就是说要比较 7 次（常数次）

那么，对于搜索的点，只需要比较常数次就可以遍历到下一个点了，比较时间复杂度为 $O(N)$

```c
for(i = 0;i < NumPointsInStrip;i++)
	for(j = i + 1;j < NumPointsInStrip;j++)
		if(Dist_y(P_i, P_j) > Min_dist)
			break;
		else if(Dist(P_i, P_j) < Min_dist)
			Min_dist = Dist(P_i, P_j);  
```

## Solution Method

### Mathematical Induction

可以通过猜一个时间复杂度，然后通过数学归纳法证明它。

**【Example】** $T(N)=2T(\lfloor N / 2 \rfloor)+N$

**Guess：** $T(N)=O(N\log N)$

**Proof：** 

$$\begin{array}l
T(N) & = 2T(\lfloor N / 2 \rfloor)+N\\
& \le 2c \lfloor \frac{N}{2}\rfloor \log \lfloor \frac{N}{2}\rfloor +N \\
& \le cN(\log N -\log 2)+N \\
& \le cN\log N, \ \ \text{for c} \ge 1
\end{array}$$

但是，使用数学归纳法一定记住，最后的常数不能变，不然时间复杂度就会在不知不觉中增加，以下是错误的分析：

**Guess：** $T(N)=O(N)$

$$\begin{array}l
T(N) & = 2T(\lfloor N / 2 \rfloor)+N \\
& \le 2c \lfloor \frac{N}{2}\rfloor +N \\
& \le cN +N
\end{array}
$$

这里常数从 $c$ 变成 $c+1$ 了，但是这只是数学归纳法的一步。此题目向下迭代的次数为 $O(\log N)$ 次，每迭代一次，常数就加一，最终复杂度还是会变为 $O(N\log N)$

### Recursion-tree method

感觉不是很重要？总之层数按最深的那个来。

**【Example】** $T(N)=2T(\sqrt{N})+\log N$

**Proof：**

一直递归到最后一层，有：

$$
T(N)=2^k T(N^{1 / 2^k})+(k+1)\log N
$$

令 $N^{1 / 2^k}=2$ ，可得 $k=\log \log N$，则：

$$
T(n)=O(\log N+(k+1)\log N) = O(\log N\log \log N)
$$

### Master method

$$\begin{gather}T(N)=aT(\frac{N }{b})+\Theta(N^k \log^pN),a\ge 1,b\gt 1 ,p\ge 0 \\
T(N)=\begin{cases}O(N^{\log_ba})\  , &\ if\ \log_ba\gt k \\ O(N^k\log ^{p+1}N)\ , &\ if\ \log_ba=k \\  O(N^k \log ^pN)\ , &\ if\ log_ba\lt k
\end{cases}
\end{gather}$$

证明过程见课本 