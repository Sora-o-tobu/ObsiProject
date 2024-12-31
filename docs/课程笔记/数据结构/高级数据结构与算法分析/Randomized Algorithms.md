
# Randomized Algorithms

!!! quote "一般的最优算法可以看作是特殊的随机算法：Yield the correct answer with high probability"

## 经典案例：The Hiring Problem

HeadHunter 帮经理推荐 $N$ 个 Applicants，经理每天可以面试一个人，但是只录取 $M$ 个人。

其中，面试一个人的消耗 $C_i$ 远小于雇用一个人的消耗 $C_h$（录取后再解雇这个人也要交钱）。

!!! info "下面我们将 M 置 1 考虑"

### A Naive Solution

按照面试的顺序，如果当前面试的人是已面试过的人质量最好的，则雇用他（同时解雇前面雇佣的人）。

```c
int Hiring ( EventType C[ ], int N )
{   /* candidate 0 is a least-qualified dummy candidate */
    int Best = 0;
    int BestQ = the quality of candidate 0;
    for ( i=1; i<=N; i++ ) {
        Qi = interview( i ); /* Ci */
        if ( Qi > BestQ ) {
            BestQ = Qi;
            Best = i;
            hire( i );  /* Ch */
        }
    }
    return Best;
}
```

其中，若 candidate 的质量呈递增分布，则该算法所得的 cost 将是最坏情况；若 candidate 随机分布，则 candidate i 在前 i 个人中最优的概率为 $\frac{1}{i}$ ，分析计算平均 cost：

$$
\begin{array}c
X_i =\begin{cases}1, & \text{if hired}  \\
0, & \text{if NOT hired}\end{cases} \\
\Rightarrow X=\sum X_i, \ E[X_i] =Pr[\text{candidate i is hired}]=\frac{1}{i} \\
\Rightarrow E[X] =E\left[\sum X_i\right]= \sum E[X_i] =\sum_{i=1}^N \frac{1}{i} = \ln N +O(1)
\end{array}
$$

所以最终的 Cost 大致为 $O(C_h\ln N +NC_i)$。

### Randomized Solution

```c
int RandomizedHiring ( EventType C[ ], int N )
{   /* candidate 0 is a least-qualified dummy candidate */
    int Best = 0;
    int BestQ = the quality of candidate 0;

    randomly permute the list of candidates; // How to do it???

    for ( i=1; i<=N; i++ ) {
        Qi = interview( i ); /* Ci */
        if ( Qi > BestQ ) {
            BestQ = Qi;
            Best = i;
            hire( i );  /* Ch */
        }
    }
}
```

这里的随机意义在于输入序列不再依赖于 HeadHunter 给出的 input，防止 HeadHunter 故意给出最坏序列的情况发生。

```c
// A feasible function to make randomized input
void PermuteBySorting ( ElemType A[ ], int N )
{
    for ( i=1; i<=N; i++ )
        A[i].P = 1 + rand()%(N3); 
        /* makes it more likely that all priorities are unique */
    Sort A, using P as the sort keys;
}
```

### Online Hiring Problem

但是有时我们并不能在一开始就得到输入序列，我们需要一个 Online 的算法：

```c
int OnlineHiring ( EventType C[ ], int N, int k)
{
    int Best = N;
    int BestQ = -  ;
    for ( i=1; i<=k; i++ ) {
        Qi = interview( i );
        if ( Qi > BestQ )   BestQ = Qi;
    }
    for ( i=k+1; i<=N; i++ ) {
        Qi = interview( i );
        if ( Qi > BestQ ) {
            Best = i;
            break;
        }
    }
    return Best;
}
```

前面 $k$ 个人面试后只记录下其中最大值，然后面试后面的人的时候，遇见更好的人，立即雇用并结束算法。

那么参数 $k$ 的选择将决定该算法的优劣：

- $S_i$ := 第 i 个候选人最优
- $S_i = \{A:=\text{the best one is at position i} \} \cap \{B:= \text{no one at positions k+1 to i-1 are hired}\}$

$$\begin{array}c
Pr[S_i] =Pr[A\cap B] =Pr[A] \cdot Pr[B] = \frac{1}{N} \cdot \frac{k}{i-1} \\
Pr[S] =\sum_{i=k+1}^N Pr[S_i] = \sum_{i=k+1}^N \frac{k}{N(i-1)} = \frac{k}{N} \sum_{i=k}^{N-1} \frac{1}{i}
\end{array}$$

!!! warning "Why $\frac{k}{i-1}$ ???"
	相当于，前 $i-1$ 个候选人，最优者出现在前 $k$ 个人中


!!! note "Lemma"
	$$\int_k^N \frac{1}{x} dx = \sum_{i=k}^{N-1} \int_i^{i+1} \frac{1}{x}dx \le \sum_{i=k}^{N-1} \frac{1}{i} \le \sum_{i=k}^{N-1} \int _{i-1}^{i} \frac{1}{x}dx = \int _{k-1}^{N-1}\frac{1}{x}dx$$

$$
\frac{k}{N}\ln \left(\frac{N}{k}\right) \le Pr[S]\le \frac{k}{N} \ln \left(\frac{N-1}{k-1}\right)
$$

经过求导，可以求得 $Pr[S]$ 在 $k= \frac{N}{e}$ 处取得最大概率。


## 经典案例：Quicksort

快速排序的 Pivot 选择，两边至少都有 $n/4$ 元素，以使得时间复杂度仍为 $O(N\log N)$（比理想的 $n/2$ 拥有更大的常数），这样的 Pivot 称为 **Central Splitter**。

我们之前学过的快排是在首中尾三个数中选中位数，但这样并不能保证 Pivot 的选择是 Central Splitter。

而随机化的快速排序在每次递归都选择 Central Splitter 作为 Pivot，其思想为重复随机选择，直到选出 Central Splitter，其期望选择次数为 2 次，每次选择的代价为 $O(N)$。

!!! info "Why 2???"
	$$1* \frac{1}{2} +2* \frac{1}{4}+ 3*\frac{1}{8}+...$$

![[randomquicksort.png]]

!!! failure "Worst Case 从原来的 $O(N^2)$ 、Best Case 从原来的 $O(N)$ 都变成了 $O(N\log N)$"

