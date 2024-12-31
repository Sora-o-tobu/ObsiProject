
# Divide & Conquer

## Overview

To solve a problem recursively,we can:

- **Divide** the problem into a number of sub-problems
- **Conquer** the sub-problems by solving them recursively
- **Combine** the solutions to the sub-problems into the solution for the original problem

得到的递归时间复杂度等式的一般形式如下：

$$
T(N) = aT(N / b) +f(N)  
$$

!!! quote "采用分治思想的算法时间复杂度"
	- The maximum subsequence sum 最大子串和 - the $O(N\log N)$ solution
	- Tree traversals 树遍历 - $O(N)$
	- Mergesort & quicksort - $O(N\log N)$

## 经典案例：Closest Points Problem

在一个二维平面上给出 $N$ 个点，找出一对距离最短的点。

如果我们使用*穷举搜索*，一共需要检查 $N(N-1) / 2$ 组点对，即时间复杂度达到 $O(N^2)$。

因此，我们考虑使用和最大子序列问题类似的分治法来解决该问题。

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

所以，我们必须确定一个算法，使得 Combine 部分的时间复杂度为 $O(N)$，才能将整体时间复杂度降低至 $O(N\log N)$

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

??? success "拓展阅读: [最大子序列和问题](https://leetcode.cn/problems/maximum-subarray/description/)"
	对于一个子序列区间 $[l,r]$，我们维护四个量：
	
	- `lSum` 表示以 l 为左端点的最大子序列和
	- `rSum` 表示以 r 为右端点的最大子序列和
	- `mSum` 表示 $[l,r]$ 内的最大子序列和
	- `iSum` 表示 $[l,r]$ 的区间和
	
	```c
	struct Status {
	    int lSum, rSum, mSum, iSum;
	};
	
	struct Status pushUp(struct Status l, struct Status r) {
	    int iSum = l.iSum + r.iSum;
	    int lSum = fmax(l.lSum, l.iSum + r.lSum);
	    int rSum = fmax(r.rSum, r.iSum + l.rSum);
	    int mSum = fmax(fmax(l.mSum, r.mSum), l.rSum + r.lSum);
	    return (struct Status){lSum, rSum, mSum, iSum};
	};
	
	struct Status get(int* a, int l, int r) {
	    if (l == r) {
	        return (struct Status){a[l], a[l], a[l], a[l]};
	    }
	    int m = (l + r) / 2;
	    struct Status lSub = get(a, l, m);
	    struct Status rSub = get(a, m + 1, r);
	    return pushUp(lSub, rSub);
	}
	
	int maxSubArray(int* nums, int numsSize) {
	    return get(nums, 0, numsSize - 1).mSum;
	}
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

**【Example】** $T(N) =\sqrt{N} T(\sqrt{N}) + N\log \sqrt{N}$

**Guess：** 主定理不适用于该题，并且计算递归层数的方法较为复杂难理解，因此我们采用猜测验证。由于这个实际上是把一个 MergeSort 分为 $\sqrt{N}$ 个段分别 sort 再合并，因此我们猜测时间复杂度为 $O(N\log N)$

$$\begin{array}l
T(N) & = \sqrt{N}T(\sqrt{N}) + \frac{1}{2}N\log N
\\ & = \sqrt{N} \times \sqrt{N} \log \sqrt{N} + \frac{1}{2}N\log N
\\ & = \frac{1}{2}N \log N + \frac{1}{2}N \log N 
\\ & = N\log N
\end{array}$$

非常完美的证明！

### Recursion-tree method

感觉不是很重要？总之层数按最深的那个来。

**【Example】** $T(N)=2T(\sqrt{N})+\log N$

**Proof：** 一直递归到最后一层，有：

$$
T(N)=2^k T(N^{1 / 2^k})+(k+1)\log N
$$

令 $N^{1 / 2^k}=2$ ，可得 $k=\log \log N$，则：

$$
T(N)=O(\log N+(k+1)\log N) = O(\log N\log \log N)
$$

**【Example】** $T(N)=T(N^{1 / 3}) + T(N^{2 / 3}) + \log N$

**Proof:** 递归最大层数仍然是 $\log \log N$ 数量级，并且每层都有 $c\log N$ ，那么写题的时候可以简单认为 $T(N)= O(\log N \log \log N)$ 。

- 设 $t(k)= T(2^k)= T(N)$，那么 $t(k) =t( k / 3) + t(2k / 3)+ k =O(k\log k)$
- 将 $k=\log N$ 代回去得到 $T(N)= t(k) =O(\log N \log \log N)$

### Master method

$$\begin{gather}T(N)=aT(\frac{N }{b})+\Theta(N^k \log^pN),a\ge 1,b\gt 1 ,p\ge 0 \\
T(N)=\begin{cases}O(N^{\log_ba})\  , &\ if\ \log_ba\gt k \\ O(N^k\log ^{p+1}N)\ , &\ if\ \log_ba=k \\  O(N^k \log ^pN)\ , &\ if\ log_ba\lt k
\end{cases}
\end{gather}$$

主定理总共有很多版本，但我不想细分差别了，所以只记这个。