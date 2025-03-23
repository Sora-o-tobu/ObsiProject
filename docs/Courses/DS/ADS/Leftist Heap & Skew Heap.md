
## Leftist Heap

### Definition

左偏堆，亦称左偏树（Leftist Tree），是一种用于实现优先级队列的 Binary Heap 数据结构，它具有和堆一样的大小顺序，但表现形式为**非平衡二叉树**，因此不能使用数组表示和维护。

左偏堆的关键特征之一是计算和维护每个节点的 **Null Path Length** ，该长度定义为从节点 X 到最近的 Null 节点的距离，在程序结构中通常使用 `dist(X)` 或 `Npl(X)` 表示

- 定义 `Npl(NULL) = -1`，如果一个节点有 Null 子节点，则其 Npl 为 0
- `Npl(X) = Min{Npl(Left Child),Npl(Right Child)}+1`

对于左偏堆任一节点，都有 $Npl(Left\ Child)\ge Npl(Right\ Child)$ ，并能得到以下定理：

- <1> 从根到最右侧叶的路径是从根到叶的最短距离（这里采用拓展二叉树的形式，即叶节点为 Null 节点）
- <2> 如果到最右侧路径上有 $r$ 个节点，则左偏堆起码有 $2^r-1$ 个节点，这也意味着对于具有 $N$ 个节点的左偏堆，到最右侧叶节点的路径长度为 $O(\log N)$

!!! note "路径长度受限是减少维护代价的基础"

根据以上定义，左偏堆的声明通常如下：

```c
typedef struct node {
	ElementType Val;
	struct node *left;
	struct node *right;
	int Npl; // or Dist
} Node;
```


### Operation

对左偏堆的操作主要是 **Merge** ，其它操作都可以通过 Merge 来调整。

- `DeleteMin()` 或 `ExtractMin` 可以通过删除 root 并为左右子树调用 Merge 来完成
- `Insert()` 可以通过 Merge 原来的左偏堆以及单个节点的树来完成


| Function   | Complexity  |
| ---------- | ----------- |
| Get Min    | $O(1)$      |
| Delete Min | $O(\log N)$ |
| Insert     | $O(\log N)$ |
| Merge      | $O(\log N)$ |

由于右子树较小，因此 Merge 的通常想法是将右子树与其它树再进行 Merge。以下是其抽象步骤：

1. 将值较小的根作为新根，其 Left 子树不变
2. 递归合并其 Right 子树和另一棵树
3. 递归返回之前，自下而上更新各个 Merge 操作的根的 Npl；如果不符合左偏堆性质，则交换左右子树

从[修佬笔记](https://note.isshikih.top/cour_note/D2CX_AdvancedDataStructure/Lec04/#%E9%80%92%E5%BD%92%E5%BC%8F)抄来的递归实现方式：

```c
LeftistHeapNode * merge(Node * x, Node * y) {
    if (x == NULL) return y;
    if (y == NULL) return x;

    if (x->val > y->val) {
        swap(x, y);
    }

    x->rs = merge(x->rs, y);

    if (x->ls == NULL || x->ls->dist < x->rs->dist) {
        swap(x->ls, x->rs);
    }

    x->dist = x->rs->dist + 1;

    return x;
}
```

课件里的递归实现方式：

```c
Node* Merge(Node* H1, Node* H2){
	if(H1 == NULL) return H2;
	if(H2 == NULL) return H1;
	if(H1->Val < H2->Val) return Merge1(H1,H2);
	else return Merge1(H2,H1);
}

//静态属性使得其可以在写题时正确互相调用
static Node* Merge1(Node* H1, Node* H2) {
	if(H1->left == NULL)
		H1->left = H2;
	else {
		H1->right = Merge(H1->right,H2);
		if(H1->left->Npl < H1->right->Npl)
			swap(H1->left,H1->right);
	} // end else
	return H1;
}
```

!!! warning "迭代实现则需要使用栈结构"

![[lhexample1.png]]


相比于其它 Binary Heap ，Leftist Heap 的优缺点如下：

**Advantage of Leftist Heap:**

1. `DeleteMin` 和 `Merge` 操作时间复杂度为 $O(\log N)$ ，是此操作最有效的数据结构之一
2. 相对于其它 Binary Heap（如斐波那契堆），Letist Heap 的实现更加简单

**Disadvantage of Leftist Heap:**

1. `Insert` 操作时间复杂度为 $O(\log N)$ ，比其它 Binary Heap 慢（如二叉堆通过插入建堆方式耗时 $O(N)$，则均摊操作消耗为 $O(1)$）
2. Leftist Heap 使用了更多空间，因为它需要维护空路径长度的值


## Skew Heap

### Definition

**斜堆（Skew Heap）**是左偏堆的简单形式，具有和左偏堆类似的性质，但又有所不同。

其特点是在进行 Merge 操作时，无条件进行左右子树的 Swap，这样就不需要在递归调用结束时额外判断左右子树的 Npl 是否符合要求。

!!! example ""
	=== "合并前"
		![[斜堆合并前.png]]
	=== "合并后"
		![[斜堆合并后.png]]


!!! info "按照顺序插入 $1\sim 2^k-1$ ，最终的堆呈现满二叉树形状，不过不能据此简单填入数据"
	![[1215insertskewheap.png]]

### 均摊分析

对 Skew Heap 的 Merge 操作进行均摊分析，定义势能函数 $\phi(D_i)$ 为以 $D_i$ 为根节点的树的 Heavy Nodes 的数量，其中 Heavy Nodes 定义为右子树的节点数大于等于左子树的节点数的 Node。

那么对于 Heavy Nodes 以及相对的 Light Nodes，有如下性质：

- 任意不在右路径的节点，其在归并前后左右子树不变化，即轻重情况也不变
- 右路径上所有重节点在归并后全部变为轻节点，而轻节点不一定变为重节点

根据以上性质可以进行 Merge 操作的均摊分析。

由于非右路径节点不参与轻重变化，所以可以忽略它们的数量。以 $l_1,h _1$ 分别指代第一颗树右路径上轻节点和重节点的数量，$l,h$ 分别指代其它轻重节点，则最坏情况下，假设右路径上所有轻节点都变为重节点：

$$\begin{array}c
T_{worst} = l_1 +h_1+ l_2 +h_2 \\
\text{Before Merge: }\  \phi_i = h_1 +h_2 +h \\
\text{After Merge: }\ \phi_{i+1}\le l_1 + l_2 +h \\ 
T_{amortized} = T_{worst} +\phi _{i+1} -\phi_i \le 2(l_1 +l_2)
\end{array}
$$

又右路径长度受限于 $O(\log N)$ ，则 $T_{amortized} = O(\log N)$
