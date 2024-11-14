# Chapter 5. Memory Hierarchy

## Memory Hierarchy Overview

存储容量和读写速度不可兼得。

Cache的出现解决了主存速度不够快的问题，也是这门课程讨论的重点。


## Cache

!!! quote "局部性原理: 计算机程序在时间与空间上都表现出局部性"
	- 时间局部性 (Temporal Locality)
		- 最近被访问的存储器单元很快还会被访问
		- 保存近期频繁被访问的主存单元的数据
	- 空间局部性 (Spatial Locality)
		- 正在被访问的存储器单元附近很快会被访问
		- 从主存中取回待访问数据时，会同时取回与其位置相邻的主存单元的数据



## Virtual Memory