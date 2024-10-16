
## Structure

Leftist Heap 和 Skew Heap 并不支持 Decrease-Key 操作（减少某个key值，并进行调整），但本节讨论的 **Binomial Queue(二项队列)** 的 Decrease-Key 操作仅需 $O(\log N)$ 

!!! quote "对于Dijk算法来说，Decrease-Key操作是必要的"

定义 Binomial Tree 为满足 Heap-ordered 的大小顺序，且结构类似于二进制性质的数，其合并方式为令其中一颗树直接成为另一颗树的子树。

![[binomialtree.png]]

!!! note "二项树的每个child本身也是二项树(eg: 1000-1=111)"

而二项队列就是一系列二项树的集合，每个二项树的阶数 k 都不相同

由于二项树符合堆的性质，但又不能用数组来表示，我们采用 Left-Child-Next-Sibling Tree with Linked List 的结构来表示该数：

```c
typedef struct BinNode* BinTree;
typedef struct Collection* BinQueue;

struct BinNode{
	ElementType Element;
	BinTree LeftChild;
	BinTree NextSibling;
}

struct Collection{
	int CurrentSize;
	BinTree TheTrees[MaxTrees];
}
```


## Operation

### Find-Min

最小值存储于队列其中一个二项树的根节点上，因此只需要遍历一遍根节点即可，时间复杂度为 $O(\log N)$ 

!!! warning "有些结构会使用额外一个指针维护最小值，时间复杂度可以为 $O(1)$"

### Merge

类似二进制的加法，且时间复杂度为 $O(\log N)$

![[BQMerge操作.png]]

那么，**Insert** 也可以看作是 Merge 一个 B<sub>0</sub> 和一个二项队列，可复用合并过程。

```c
BinQueue Merge(BinQueue H1, BinQueue H2) {
	BinTree T1, T2, Carry = NULL;
	int i,j;
	if(H1->CurrentSize + H2->CurrentSize > Capacity)
		ErrorMessage();
	H1->CurrentSize += H2->CurrentSize;
	for(i=0,j=1;j<=H1->CurrentSize;i++,j*=2){
		T1 = H1->TheTrees[i];
		T2 = H2->TheTrees[i];
		switch(4*!!Carry+2*!!T2+!!T1){
			case 0://000
			case 1://001
				break;
			case 2://010
				H1->TheTrees[i] = T2;
				H2->TheTrees[i] = NULL;
				break;
			case 3://011
				Carry = Combine(T1, T2);
				H1->TheTrees[i] = NULL;
				H2->TheTrees[i] = NULL;
				break;
			case 4://100
				H1->TheTrees[i] = Carry;
				Carry = NULL;
				break;
			case 5://101
				Carry = Combine(Carry, T1);
				break;
			case 6://110
				Carry = Combine(Carry, T2);
				break;
			case 7://111
				H1->TheTrees[i] = Carry;
				Carry = Combine(T1, T2);
				H2->TheTrees[i] = NULL;
				break;
		}//End-Switch
	}return H1;
}

BinTree Combine(BinTree T1, BinTree T2) {
	if(T1->Element > T2->Element)
		return Combine(T2, T1);
	T2->NextSibling = T1->LeftChild;
	T1->LeftChild = T2;
	return T1;
}
```

### Delete-Min

删除最小节点操作分四步：

- **Step 1:** FindMin in $B_k$
	- $O(\log N)$
- **Step 2:** Remove $B_k$ from $H$
	- $O(1)$
- **Step 3:** Remove Root from $B_k$
	- $O(\log N)$
- **Step 4:** Merge($H'$,$H''$)
	- $O(\log N)$

其中 $H'$ 为 $H$ 删去 $B_k$ 后的二项队列；$H''$ 为 $B_k$ 删去根节点后形成的二项队列。


## 均摊分析

结论：对于二项队列单步插入建立数据结构，所需的时间复杂度为 $O(N)$

先使用累加法（Aggerate）进行分析，图中 `step` 代表建树的消耗，`link` 代表合并树的消耗：

![[BQAM1.png]]

可以看到，每一步插入的消耗符合：

```c
if(H=...0) credit = 1;
if(H=...01) credit = 2;
if(H=...011) credit = 3;
...
```

那么总消耗为：

$$
\sum Credit = 1* \frac{N}{2}+ 2* \frac{N}{4} + 3* \frac{N}{8} ... = O(N)
$$

和 PPT 上略有不同，但最终结果是一样的。

接下来使用势能分析法分析，设定势能函数为：

$$
\phi _i =\text{Number of trees after ith insertion}
$$

其中，每步的势能变化已经标注在上图左侧红色方框内，可以看出每步的均摊消耗：

$$
C_i +\phi_i -\phi_{i-1} = 2
$$

那么

$$
\sum C_i +\phi_i - \phi_{i-1} = 2N - \phi_N \le 2N = O(N)
$$

## Project2: Fibonacci Queue

YouTube: https://www.youtube.com/watch?v=6JxvKfSV9N

