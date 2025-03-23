# CH 11 : Trees

## 11.1 Introduction to Trees

- Tree: connected undirected graph with no simple circuits
- Forest: undirected graph with no simple circuits (collection of trees)
- 一个无向图是树当且仅当在它的每对顶点之间存在唯一简单通路
- 选定一个根节点后，树可以被唯一地画出（Rooted Tree）
- Sibling: 有同样父节点的节点称为Siblings
- Ancestors: 从该节点到根节点Path上所有节点称为该节点的Ancestors
- Descendants: 所有以该节点为Ancestors的节点称为该节点的Descendants
- Internal Vertex: 有孩子的点叫内点
- Ordered Rooted Tree: 每个内点的孩子都排序的有根树，按从左到右的顺序
- Level: The level of vertex v in a rooted tree is the length of the unique path from the root to v ==例如，只有Root的树，Level和Height都为0，空树的Height为-1==
- Height: The height of a rooted tree is the maximum of the levels of its vertices
- Balance: 所有叶节点都在Levels h or h-1的Rooted Tree ==平衡树==
- 𝑛 个顶点的树含有 𝑛−1 条边
- 树都是二部图，$𝐾_{1,𝑛}$ 和 $𝐾_{𝑚,1}$ 是树

!!! note "Counting"
	给出节点数n，让你计算可以构造出Tree的数量，记得关注是unrooted tree还是rooted tree
	
	例子： n=5 , unrooted=3 , rooted=9.
	
	计算Rooted Tree个数的时候，可以从The longest path from root这个角度切入

- 对于任何树，都有 $|E|=|V|-1$
- 对于Full m-ary Tree with i internal vertices，共有 n=mi+1 vertices (Tips:n=l+i，因此也可以算出来叶节点的个数)
	- 满m叉树：值该树的节点要么是叶节点要么是满节点

!!! example
	How many leaves does a full 3-ary tree with 100 vertices have?
	
	$i=\frac{n-1}{m}=\frac{100-1}{3}=33$ ， $l=n-i=100-33=67$

!!! example "例子2"
	(£) There is an undirected tree with 2 vertices of 4 degrees, 3 vertices of 3 degrees. The remaining vertices are leaves. Then it contains 8 leaves
	
	2 * 4 + 3 * 3 + l = 2e = 2(2 + 3 + l -1)=2l + 8
	
	l = 9

## 11.2 Applications of Trees

=== "Binary Search Tree 二叉搜索树"
	很基本的构造
=== "Decision Tree 决策树"
	每一个Internal Vertex对应一个决策
=== "Prefix Codes 前缀码"
	- 为了确保一串bit string对应不超过一种Letter序列，一个Letter的编码不应该出现在另一个Letter编码的开头，如： e:0,a:10,b:110
	- 用一个Binary Tree来构造编码，往左的边对应0，往右的边对应1，则编码为该树叶节点对应的序列：
	![[PrefixCode.png]]
=== "Huffman Tree 哈夫曼树"
	使得每个叶节点都有权值，则定义 ==节点的带权路径长度== 为节点到Root的路径长度与节点权值的乘积。称所有节点的带权路径长度之和为树的带权路径长度。
	
	其中，带权路径长度最短的树称为Huffman Tree，按照哈夫曼树进行Prefix Coding，得到哈夫曼编码
	
	构造哈夫曼树的哈夫曼算法如下：
	
	- 有n个原始节点，权值分别为$\{W_1 ,W_2,..., W_n\}$ ，构造成n颗二叉树的集合$F=\{T_1, T_2,... ,T_n\}$ ，其中每颗二叉树 $T_i$ 只有一个带权为 $W_i$ 的根节点，左右孩子均空
	- 在F中pop两颗根节点权值最小的二叉树，作为树的左右孩子构造新树，且置根节点为左右孩子权值之和，将新二叉树压入F中
	- 重复上一步，直到F中只剩一颗树，这颗树就是Huffman Tree
	
	!!! danger
		遇到诸如 `What is the average number of bits required to encode a character?` ，注意最后结果不是单纯的所有bit位相加除以字母数，而是各个character的bit位数乘以它们的权值再相加 (即 ==加权平均数== )
## 11.3 Tree Traversal

- preorder traversal 对应 Prefix Form(波兰表达式)
- inorder traversal 对应 Infix Form(正常的表达式)
- postorder traversal 对应 Postfix Form(逆波兰表达式)

## 11.4 Spanning Tree

- G的生成树指包含每个顶点的子图
	- Find spanning tree by removing edges from simple circuits
- 简单图连通当且仅当它有生成树

### How to find a spanning tree

=== "Depth-First Search"
	- 从图中选择一个起始顶点作为根节点。
	- 使用 DFS 遍历图，记录遍历过程中访问的边。
	- 遍历完成后，记录的边将形成图的一棵生成树。
	
	```c
	  void dfs(int u) {
		  visited[u] = 1;
		  for (int v = 0; v < n; v++) {
	      if (adjMatrix[u][v] && !visited[v]) {
	          /* 记录边 */
	          // do something
	          dfs(v);
	      }
	  }
	}
	```
=== "Breadth-First Search"
	- 从图中选择一个起始顶点作为根节点。
	- 使用 BFS 遍历图，记录遍历过程中访问的边。
	- 遍历完成后，记录的边将形成图的一棵生成树。
	
	```c
	void bfs(int s) {
	    queue<int> q;
	    q.push(s);
	    visited[s] = 1;
	
	    while (!q.empty()) {
	        int u = q.front();
	        q.pop();
	        for (int v = 0; v < n; v++) {
	            if (adjMatrix[u][v] && !visited[v]) {
	                /* 记录边 */
	                // do something
	                q.push(v);
	                visited[v] = 1;
	            }
	        }
	    }
	}
	```


### How to find Minimum Spanning Tree

=== "Prim Algorithm"
	1. 从图中任意选择一个顶点作为起始顶点。
	2. 初始化一个集合，用于存储已经访问过的顶点。将起始顶点添加到该集合中
	3. 初始化一个优先队列，用来储存已访问到的顶点可以访问的边(该边不能连接两个已访问过的顶点)
	4. 选择一条权值最小的边并将其从队列中删除。将该边添加到最小生成树中，并将该边的未访问顶点标记为已访问，并将其添加到已访问顶点集合中。
	5. 重复步骤 4，直到所有顶点都被访问。
=== "Kruskal Algorithm"
	1. 将图中的所有边按照权值从小到大排序。
	2. 初始化一个空集合，用于存储最小生成树的边。
	3. 按顺序遍历所有边。选择不和现在已有顶点产生回路的最小边。
	4. 重复步骤 3，直到最小生成树中包含 𝑛−1 条边，其中 𝑛 为图中顶点的数量。

