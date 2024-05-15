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




