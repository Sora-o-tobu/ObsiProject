
## 红黑树

**定义：**

- 每个节点要么是红色要么是黑色
- 根节点为黑色
- 红黑树的叶节点定义为 NULL(NIL) ，叶节点都是黑色，不显式画出
- 如果一个节点是红色，那么它的孩子都应该为黑色
- 每个节点到它的后代叶节点的路径包含相同数量的黑色节点
	- 定义 black-height 为任一节点x到叶节点所经过的黑色节点数量（包括x）

!!! abstract "Lemma"
	一个具有 N 个内部节点（Internal nodes）的红黑树，它的高度不超过 $2\ln (N+1)$

### Insert

该小节内容直接参考 [Oneko](https://www.yuque.com/oneko/something/rbtree-bptree) 的笔记

插入的节点默认颜色为红色，以下的图片通过 Insert 4 来展示关于红黑树插入的所有情况。

其中应注意，节点 15 应该为红色，该图有错误

![[Insert4.png]]

时间复杂度为 $O(\ln N)$

!!! success "红黑树维护的关键思想在于使用 Rotate 以及变色保持黑高相同"

### Delete

红黑树的删除分为两个步骤：

<font style="font-weight: 1000;font-size: 20px" color="red">第一步：</font>

- **删除叶子节点**
	- 如果该节点为红色，使用 NIL 代替该节点，且无需进行第二步调整
	- 如果该节点为黑色，使用 NIL 代替该节点，且需进行第二步调整，将该 NIL 节点记为 **x**
- **删除度为 1 的节点**
	- 用它的孩子代替该节点，并改为黑色，无需进行第二步调整
	- 这种情况，只可能是该节点为黑色，子节点为红色，否则会违反红黑树性质
- **删除度为 2 的节点**
	- 用左子树中最大的节点或是右子树中最小的节点代替该节点，然后删除用作代替的节点，重新进入上述两种情况之一

!!! info "其实就是对普通二叉搜索树的删除操作，但是删除过程中减少了子树黑高导致失衡"

<font style="font-weight: 1000;font-size: 20px" color="red">第二步：</font>

能进入第二步调整，说明最终删除的是黑色叶子节点，将用作替代的 NIL 记为 x，它的 sibling 记为 w ，它的父节点称为 p

<font style="font-weight: 1000;font-size: 20px" color="orange">Case 1：</font>

节点 w 为红色节点，那么父节点一定是黑色节点。

调整操作为对节点 p 和 w 都进行变色，然后对节点 p 进行 Rotate：

![[红黑树删除case1.png]]

!!! tips "最后一步多出来的黑色节点是节点 w 原先的左孩子"

此时需要继续调整，转入 Case 2，3，4

<font style="font-weight: 1000;font-size: 20px" color="orange">Case 2：</font>

节点 w 为黑色节点，且 w 的两个子节点均为黑色节点

调整操作为将节点 w 变色为红色。若节点 p 为红色，直接将其变为黑色结束调整；若节点 p 为黑色，重新将其命名为节点 x ，需要继续调整，转入 Case 2，3，4

![[红黑树删除Case2.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">Case 3：</font>

节点 w 为黑色节点，且它的唯一红色子节点与 w,p 构成 RL 关系（对称情况不列出）

调整操作为将节点 w 与其唯一红色子节点变色，并对节点 w 进行一次右旋，需要继续调整，转入 Case 4

![[红黑树删除Case3.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">Case 4：</font>

节点 w 为黑色节点，且它存在红色子节点与 w,p 构成 RR 关系（对称关系不列出）

调整操作为将该红色子节点变色为黑色，节点 w 变色为父节点 p 的颜色，节点 p 变色为黑色，再对节点 p 进行一次左旋完成调整

![[红黑树删除Case4.png]]

??? example "删除实例"
	![[红黑树删除实例.png]]



**Numbers Of Rotations：**

|               | AVL         | Red-Black Tree |
| ------------- | ----------- | -------------- |
| **Insertion** | $\le 2$     | $\le 2$        |
| **Deletion**  | $O(\log N)$ | $\le 3$        |


## B+ 树

**一个 Order M 的 B+ 树定义如下：**

- 根节点要么是叶节点，要么有 2 到 $M$ 个子节点
- 所有除了根节点的非叶节点都有 $\lceil \frac{M}{2} \rceil$ 到 $M$ 个子节点
- 所有叶节点深度相同