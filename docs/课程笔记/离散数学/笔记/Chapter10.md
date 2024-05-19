# CH 10 : Graphs

## 10.1 Graphs and Graphs Models

- Simple graph: 同一对顶点之间只有一条边
- Multigraph: 同一对顶点之间可能有多条边
- Pseudograph: 可能存在自环、同一对顶点之间可能有多条边
- Path: $\{v_0 ,v_1\},...,\{v_{n-1} ,v_{n}\}$ ，长度为n的路径
	- 长度为0的路径只含一个点
- Circuit: 首尾是同一个点的path

## 10.2 Graph Terminology  and Special Types of Graphs

- 对于无向图，$\Sigma _{v\in V} deree(v)=2e$  （==自环算增加两个degree==）
	- 因此有引理：无向图拥有奇数个degree的vertex一定有偶数
- 对于有向图，$\Sigma _{v\in V} deree^+(v) = \Sigma _{v\in V} deree^-(v) =e$

### Some Special Simple Graphs

=== "Complete Graphs "
	- Denoted as $K_n$
	- Has $\frac{n(n-1)}{2}$ edges
	![[完全图.png]]
=== "Cycle Graphs"
	- Denoted as $C_n$ where $n\gt 2$
	![[cyclegraph.png]]
=== "Wheel Graphs"
	- Denoted as $W_n$ where $n\gt 2$
	![[WheelGraph.png]]
=== "n-Cubes Graphs"
	- Denoted as $Q_n$
	- 每个顶点之和相差一位(==按照bit position==)的数相连
	![[ncubesgrpah.png]]

#### Bipartite Graphs 二部图

- 节点集可以分成两个 不相交 的子集，使得每条边都连接两个不同子集中的结点。即在同一个子集中的结点之间没有边相连

!!! example 
	![[二部图例子1.png]]

[Theorem](#)
A simple graph is bipartite if and only if it is possible to assign one of two different colors to each vertex of the graph so that no two adjacent vertices are assigned the same color.

一个Simple Graph为二部图当且仅当可以分配两种颜色给所有vertex使得每一个相邻的vertex颜色都不同

#### Regular Graph

A simply graph is called regular if every vertex of this graph has the same degree.

!!! note
	A regular graph is called n-regular if every vertex in this graph has degree n.
	
	如 $K_n$ 就是 (n-1)-Regular




