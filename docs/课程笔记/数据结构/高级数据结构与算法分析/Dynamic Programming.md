
# Dynamic Programming

- <1> 由优化子问题结构寻找递推关系
- <2> 由递推关系求解问题，但不采用递归结构，而是自底向上
	- 循环过程中将中间值存储下来，下次需要用到时直接取值即可，不用再计算

## 经典案例：Fibonacci

常规使用递归解决斐波那契数列求和:

```c
int Fib(int n) {
	if (N <= 1)
		return 1;
	else
		return Fib(n-1) + Fib(n-2);
}
```

观察时间复杂度：

$$
T(N)\ge T(N-1)+T(N-2)\ \Rightarrow T(N)\ge F(N)$$

很明显呈指数级增长。

我们画出该算法的递归树：

![[fibonaccidiguishu.png]]

能看出问题在于非常多重复的函数调用冗余。为了避免重复调用计算子问题，我们应想到将中间过程答案记录下来，后面再需要用到时调用记录下来的数值即可：

```c
int Fib(int N) {
	int i, Last, NextToLast, Answer;
	if(N <= 1) return 1;
	Last = NextToLast = 1; // F(0) = F(1) = 1
	for (i = 2;i <= N;i++) {
		Answer = Last + NextToLast; //F(i)=F(i-1)+F(i-2)
		NextToLast = Last; //Updata F(i-2)
		Last = Answer; //Updata F(i-1)
	}return Answer;
}
```

时间复杂度从指数级降到 $O(N)$ ！

## 经典案例：矩阵相乘

对于两个矩阵 $A_{n\times p}\times B_{p\times r}$ ，需要的计算次数为 $T=n\times p\times r$ 。

假定我们要对如下四个矩阵进行矩阵乘法：

$$
M_{1[10\times 20]} * M_{2[20\times 50]} * M_{3[50\times 1]} * M_{4[1\times 100]}
$$

**In order 1:**

$$
M_{1[10\times 20]} * (M_{2[20\times 50]} * (M_{3[50\times 1]} * M_{4[1\times 100]}))
$$

计算时间为：

$$
50\times 1\times 100+ 20\times 50\times 100+10 \times 20\times 100 = 125,000
$$

**In order 2:**

$$
(M_{1[10\times 20]}* (M_{2[20\times 50]} * M_{3[50\times 1]})) * M_{4[1\times 100]}
$$

计算时间为：

$$
20\times 50\times 1+10\times 20\times 1 + 10\times 1\times 100 = 2,200
$$

可以看到两种顺序计算时间差距巨大，而我们需要找出计算时间花费最小的顺序。

令 $b_n$ 为计算 $M_1\cdot M_2\cdot ...\cdot M_n$ 的总顺序个数，那么其初值为：

$$
b_2 = 1, b_3 = 2, b_4 = 5 ...
$$

假定 $M_{ij}= M_i\cdot M_{i+1}\cdot ...\cdot M_j$，那么 $M_{1n}=M_1 \cdot ...\cdot M_n= M_{1i}\cdot M_{i+1\ n}$  

$$
b_n =\sum ^{n-1}_{i=1}b_i b_{i-1} \ \Rightarrow \ b_n =O(\frac{4^n}{n\sqrt{n}})
$$

为指数级复杂度，因此暴力求解不可取。

!!! info "DP 的关键在于优化子结构以及记录重复值"

为了达到优化子结构，我们要求子问题 $M_{1i}$ 和 $M_{i+1\ n}$ 的计算都是最优解，否则总答案不可能为最优解。

$$
m_{ij} = \left \{ \begin{array}l 
0 ,& \text{if }\ i=j \\
\min_{i\le l\lt j}\{ m_{il} + m_{l+1\ j} + r_{i-1} r_l r_j\}, & \text{if }\ i\lt j
\end{array} \right .
$$

因此我们需要把长度为 $1,2,3,...,n-1$ 的子问题都解决，然后将需要重复利用的值记录，即可降低复杂度解决：

```c
/* r contains number of columns for each of the N matrices */ 
/* r[ 0 ] is the number of rows in matrix 1 */ 
/* Minimum number of multiplications is left in M[ 1 ][ N ] */ 
void OptMatrix( const long r[ ], int N, TwoDimArray M ) 
{   int  i, j, k, L; 
    long  ThisM;
    for(i = 1;i <= N;i++)   M[ i ][ i ] = 0; // Initialize
    for(k = 1;k < N;k++) // k = j - i 子问题的长度
        for(i = 1;i <= N - k;i++) { // For each position 
	        j = i + k;
	        M[i][j] = Infinity; 
	        for(L = i;L < j;L++) { 
		        ThisM = M[i][L] + M[L+1][j] + r[i-1] * r[L] * r[j]; 
		        if (ThisM < M[i][j])  // Update min 
			        M[i][j] = ThisM; 
		    }  // end for-L
        }  // end for-Left
}
```

!!! info "最外层循环一定是子问题长度"

得到时间复杂度为 $T(N)=O(N^3)$


## 经典案例：最优二叉搜索树

有 $N$ 个关键字 $w_1 \lt w_2\lt ... \lt w_N$ ，每个关键字被检索的概率为 $p_i$ ，离树根越近，检索时间越少，那么对该二叉搜索树检索的平均时间为：

$$
T(N)=\sum _{i=1}^N p_i(1+ d_i)
$$

给定下列检索表：

| word            | break | case | char | do   | return | switch | void |
| --------------- | ----- | ---- | ---- | ---- | ------ | ------ | ---- |
| **probability** | 0.22  | 0.18 | 0.20 | 0.05 | 0.25   | 0.02   | 0.08 |

$$\begin{array}l
T_{ij} ::=OBST\ for \ w_i, ..., w_j \\
c_{ij}::= cost\ of\ T_{ij} \\
r_{ij}::=root\ of\ T_{ij} \\
w_{ij}::=weight\ of\ T_{ij}=\sum_{k=i}^j p_k
\end{array}
$$

![[obsttest.png]]

$$\begin{array}l
c_{ij} &=p_k +cost(L)+cost(R)+weight(L)+weight(R)
\\ & = p_k +c_{i, k-1} +c_{k+1, j} +w_{i,k-1} +w_{k+1,j} \\
& = w_{ij} + c_{i,k-1} +c_{k+1, j}
\end{array}$$

所以要使 $T_{ij}$ 为最优二叉搜索树，$c_{ij}$ 要取 $\min_{i\le l\lt j}\{ w_{ij} + c_{i,k-1} +c_{k+1, j}\}$

所以我们从单个节点的最优二叉搜索树开始逐个向上搭建，其中 $w_{ij}$ 固定已知，为 $i-j$ 范围内所有节点的概率和，我们只需要取到最小的 $c_{i,k-1} +c_{k+1,j}$ 即可：

![[obstsolution.png]]

!!! info "格子结构"
	以第三排第一个格子为例，`break..char` 表示以 `break`,`case`,`char` 为节点构造的最优二叉搜索树，$1.02$ 表示该最优二叉搜索树的 $cost$ ，`case` 表示该最优二叉搜索树以节点 `case` 为根。

最终可以得到 `break..void` 的 $cost$ 为 $2.15$ ，其以 `char` 为根节点；据此，还可以进行回溯，将该二叉搜索树构建出来。

!!! danger "少个Product Assembly问题，我摆了"