
# Backtracing

回溯算法的思想就是从一个大的候选答案中剪去已经可以证明是不可能正确的分支，即 **Prunning**。

## Eight Queens

在 $8\times 8$ 的棋盘上放置八个 Queens，要求所有 Queen 都不能攻击到彼此，亦即横竖以及对角线上都不能同时有两个 Queen。

一个可能的填法如图所示：

![[EightQueen.png]]

按照常规的思想，由于棋盘横竖都只有一个 Queen，我们定义 $Q_i$ 为第 i 行的 Queen，$x_i$ 为其对应的列，那么 $x_i\in \{1,2,3,4,5,6,7,8\}$ ，且没有重复元素。那么，一共有 $n!$ 种情况可供选择。

在这里，我们将八皇后问题进行简化，选取四皇后问题讲解，其 game tree 如下：

![[eqgt1.png]]

共有 $4!$ 个叶节点，对应上面的 $n!$ 种选择。但是我们判断是否正确还有另一个条件，即对角线上只能有一个 Queen，即 $\frac{x_i +x_i}{i-j}!= \text{±}1$那我们在迭代到某一节点时可以对其进行条件判断，如果条件不满足，则直接返回上一层节点，不再对下层节点进行判断。

![[eqgt2.png]]

如图，对 game tree 进行深度优先搜索，每遍历到一个节点即进行一次判断，若条件仍然符合，则继续向下搜索；若条件不符合，返回父节点。最终可以找到第一条符合条件的路径： $(2,4,1,3)$

!!! note "实际上不用构建抽象树，只是用作讲解回溯的思想"

## The Turnpike Reconstruction Problem

即收费站重建问题，在一条公路上有 N 个收费站，则它们两两之间一共有 $N(N-1)/2$ 个距离。现在只给出它们之间两两距离的集合 $D$ ，尝试将这条公路上收费站的位置关系构建出来。

我们首先可以知道 $x_1$ 和 $x_N$ 距离最远，则第一步为设定 $x_1=0, x_N = MAX\{D\}$。

假设我们已经假定了 $k$ 个收费站的点位，接下来我们将要从 $D$ 取出第 $k$ 大的元素 $D_k$ 以确定第 $k+1$ 个收费站的位置，那么如果 $D_k$ 是与非 $x_1$ 或 $x_N$ 点之间的距离，则该节点与 $x_1$ 或 $x_N$ 之间的距离大于 $D_k$ ，与大于 $D_k$ 的距离均被取出这一条件违背。因此 $D_k$ 一定是与 $x_1$ 或 $x_N$ 之间的距离。

给定 $D=\{1, 2, 2, 2, 3, 3, 3, 4, 5, 5, 5, 6, 7, 8, 10\}$ ，则 $N=6$ ，$x_1=0, x_N = 10$。

可作出对应决策树如下：

![[ttrp.png]]


其思想写作代码如下：

```c
bool Reconstruct ( DistType X[ ], DistSet D, int N, int left, int right )
{ /* X[1]...X[left-1] and X[right+1]...X[N] are solved */
    bool Found = false;
    if ( Is_Empty( D ) )
        return true; /* solved */
    D_max = Find_Max( D );
    /* option 1：X[right] = D_max */
    /* check if |D_max-X[i]|in D is true for all X[i]’s that have been solved */
    // 即判断新加入的点产生的新Dist是否都在集合 D 中，且无重复
    OK = Check( D_max, N, left, right ); /* pruning */
    if ( OK ) { /* add X[right] and update D */
        X[right] = D_max;
        for ( i=1; i<left; i++ )  Delete( |X[right]-X[i]|, D);
        for ( i=right+1; i<=N; i++ )  Delete( |X[right]-X[i]|, D);
        Found = Reconstruct ( X, D, N, left, right-1 );
        if ( !Found ) { /* if does not work, undo */
            for ( i=1; i<left; i++ )  Insert( |X[right]-X[i]|, D);
            for ( i=right+1; i<=N; i++ )  Insert( |X[right]-X[i]|, D);
        }
    }
    /* finish checking option 1 */
    
    if ( !Found ) { /* if option 1 does not work */
        /* option 2: X[left] = X[N]-D_max */
        OK = Check( X[N]-D_max, N, left, right );
        if ( OK ) {
            X[left] = X[N] – D_max;
            for ( i=1; i<left; i++ )  Delete( |X[left]-X[i]|, D);
            for ( i=right+1; i<=N; i++ )  Delete( |X[left]-X[i]|, D);
            Found = Reconstruct (X, D, N, left+1, right );
            if ( !Found ) {
                for ( i=1; i<left; i++ ) Insert( |X[left]-X[i]|, D);
                for ( i=right+1; i<=N; i++ ) Insert( |X[left]-X[i]|, D);
            }
        }
        /* finish checking option 2 */
    }
    /* finish checking all the options */
    
    return Found;
}

```

因此，Backtracing的思想可以简化成如下模板：

```c
bool Backtracking ( int i )
{   Found = false;
    if ( i > N )
        return true; /* solved with (x1, …, xN) */
    for ( each xi in Si ) { 
        /* check if satisfies the restriction R */
        OK = Check((x1, …, xi) , R ); /* pruning */
        if ( OK ) {
            Count xi in;
            Found = Backtracking( i+1 );
            if ( !Found )
                Undo( i ); /* recover to (x1, …, xi-1) */
        }
        if ( Found ) break; 
    }
    return Found;
}
```


## Alpha-Beta 剪枝

α-β 剪枝是一种对抗性搜索算法，用以减小极小化极大算法（Minimax算法）搜索树的节点数，主要应用于机器游玩的二人游戏，如井字棋、象棋、围棋等。

!!! abstract "Minimax"
	是一种找出失败的最大可能性中的最小值的算法（最小化最坏情况）

以下棋为例子，先手希望下一步的局面是自己胜算最大的局面，而后手希望下一步的局面是先手胜算最小的局面。而使用上述两种算法可以对当前局面进行估价。

!!! info "Tic-tac-toc"
	以人机井字棋为例，估价函数 $f(P)=W_{Computer} - W_{Human}$，其中 $W$ 代表当前场面选手能够获胜可能的布局。
	
	![[tictactocrenji.png]]

那么我们用正方形代表先手，选择估价最大的局面；圆形代表后手，选择估价最小的局面。假设现在知道四步后所有可能场面的估价，其博弈树如下：

![[abbys1.png]]

在先手和后手都选择对自己最有利的条件下，我们可以从子节点一步一步逆推回去两个棋手在每一步会如何决策，原则是先手选择两个子节点中最大的，后手选择两个子结点中最小的：

=== "Step 1"
	![[abjcss1.png]]
=== "Step 2"
	![[abjcss2.png]]
=== "Step 3"
	![[abjcss3.png]]
=== "Step 4"
	![[abjcss4.png]]


因此，棋局最终的走向如图：

![[abjcslast.png]]

按照上述算法思想处理决策树，需要的计算量随着步数呈指数级增长，可以对其进行剪枝减少不必要的状态。

在 Alpha-Beta 剪枝中，使用 α 表示目前所有可能解的最大下界，β 表示目前所有可能解的最小上界。因此，在树的最优解路径上，节点的估价值一定满足：

$$
\alpha \le N \le \beta
$$

在算法运行过程中，最大下界和最小上界逐渐逼近，如果对于某一节点出现 $\alpha \gt \beta$ ，就可以将其剪枝去掉了。

同样对上面的博弈树进行操作，我们对根节点使用深度优先搜索，并且将父节点的 α,β 值继承给子节点，那么第一次到达叶节点的搜索路径如下：

![[abdfs1.png]]

该叶节点的权值为 3，而它的父节点是 Min 节点，由于 $\beta \gt 3$ ，所以更新最小上界为 3。搜索完左节点，下一步搜索到权值为 17 的右节点，但此时 $\beta =3\lt 17$ ，所以不对父节点进行更新。

搜索完该 Min 节点的所有子节点，返回上一层 Max 节点并对其 α 值进行更新，即更新为子结点中最大的已确定权值。此时由于 Max 节点的右子节点的 β 值仍是 $\infty$ ，所以 $\alpha =3$ ，并且右子节点继承父节点的 α,β 值：

![[abjcsdfs2.png]]

对该 Min 节点继续向下搜索，其左子节点的权值为 2，因此更新 $\beta =2$ ，此时 $\alpha \gt \beta$ ，则将其剪枝（α cut off），不再检查右子节点。

继续回溯到上一层 Min 节点，更新其 β 值为子节点中最小的已确定权值，即 $\beta =3$。继续向下搜索至叶节点：

![[abjcsdfs3.png]]

由于 $\beta \lt 15$ 所以不对其进行更新，仅将该 Min 节点权值设为 15 ，继续返回到上一层 Max 节点。此时由于 $\alpha \lt 15$ ，更新 $\alpha = 15$，又 $\beta \lt 15$ ，则对其进行剪枝（β cut off），不再搜索其右子树：

![[abjcsdfs4.png]]

剩余子树的搜索基本同上相同，最终的搜索路径为：

![[abjcsdfs5.png]]

相比于 Minimax 算法，有较大提升。

在维基百科中给出的伪代码如下：

```c
function alphabeta(node, depth, α, β, maximizingPlayer) is
    if depth == 0 or node is terminal then
        return the heuristic value of node
    if maximizingPlayer then
        value := −∞
        for each child of node do
            value := max(value, alphabeta(child, depth − 1, α, β, FALSE))
            if value > β then
                break (* β cutoff *)
            α := max(α, value)
        return value
    else
        value := +∞
        for each child of node do
            value := min(value, alphabeta(child, depth − 1, α, β, TRUE))
            if value < α then
                break (* α cutoff *)
            β := min(β, value)
        return value

(* Initial call *)
alphabeta(origin, depth, −∞, +∞, TRUE)
```


| Worst-case performance | Best-case Performance |
| ---------------------- | --------------------- |
| $O(b ^d)$              | $O(\sqrt{b ^d})$      |

!!! abstract "$\alpha -\beta$ 剪枝可以将要查找节点数减少至 $O(\sqrt{N})$"

<font style="font-weight: 1000;font-size: 20px" color="orange">Example：</font>

哪个节点最先被剪枝？

![[adsabjzlt1.png]]

??? info "Answer"
	节点c。当搜索到节点b时，回溯到 Min 祖节点 68 ，发生 β 剪枝，不需要再对节点 c 进行搜索。