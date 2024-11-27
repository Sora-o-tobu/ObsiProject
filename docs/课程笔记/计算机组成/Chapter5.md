# Chapter 5. Memory Hierarchy

## Memory Hierarchy Overview

存储容量和读写速度不可兼得。

Cache的出现解决了主存速度不够快的问题，也是这门课程讨论的重点。

!!! quote "Memory Hierarchy"
	![[memoryhierarchy1.png]]

- **hit** The CPU accesses the upper level and succeeds
- **Miss** The CPU accesses the upper level and fails
- **Hit time** 访问上层存储的时间，包括决定该次访问是否命中所需的时间
- **Miss penalty** 将上层存储的 Block 替换为下层 Block 所需的时间，包括将这个 Block 传递给处理器所需的时间

## Cache

!!! quote "局部性原理: 计算机程序在时间与空间上都表现出局部性"
	- 时间局部性 (Temporal Locality)
		- 最近被访问的存储器单元很快还会被访问
		- 保存近期频繁被访问的主存单元的数据
	- 空间局部性 (Spatial Locality)
		- 正在被访问的存储器单元附近很快会被访问
		- 从主存中取回待访问数据时，会同时取回与其位置相邻的主存单元的数据

- **Block** 块：cache 和主存(下层)之间信息交互的单元
- **Word** 字：cache 和 CPU(上层)之间信息交互的单元
- **hit** 命中：处理器所需数据在本层存储中找到
- **miss** 失效：处理器所需数据未能在本层存储中找到
- **命中率** ：在访问本层存储时命中的次数占总次数的比例，通常作为层次结构的性能衡量指标之一
- **失效率** ：在访问本层存储时失效的次数占总次数的比例。命中率与失效率相加为1

### Directed Mapping

常见有三种映射方式：

- 直接映射 Direct Mapping
- 组相联 Set associated
- 全相联 Fully associated

!!! exmaple "A Simple Example"
	对于 32bit 主存地址、4-Block Cache、1-word Block，则一共需要 2bit 作为索引index。在 0+2 bit 用作 Offset，剩余 28bit 用作 TAG。在我们之前已访问过主存 `0x04` 的情况下，执行 `load r1, 0x04` ：
	
	![[cachesimpleex1.png]]

```
Cache 地址映射：
+-------+--------+------+
|标签TAG|索引INDEX|块偏移量|
+-------+--------+------+

Cache 结构：
+----------------+----------+------+------+
|    标签TAG      | 数据DATA |Valid |Dirty |
+----------------+----------+------+------+
 64-(n+m+2) bit   2^m*32 bit  1bit  (1bit)

- Valid(1 bit) 表示 Cache 中数据是否有效
- Dirty(1 bit) 表示 Cache 中数据是否是最新且还没写回主存
```

例如，主存为 64-bit 地址，Cache 含有 $2^n$ 个 Block，每个 Block 含有 $2^m$ 个 word：

- **Byte Offset** m+2 bit
	- 以 byte 为单位的偏移量，即 $\log _2 (2^m\cdot 4)$ (1 word = 4 byte = 32 bit)
- **Cache index** n bit
- **Tag field** 64-(n+m+2) bit

那么该 Cache 的大小为：

$$
2^n\times (2^m \times 32 +(64-n-m-2)+1)
$$

!!! info "这里最后的 1 是 Valid 位。有时候还需要加上 Dirty 位"

![[cacheex1111.png]]

=== "求 Cache 大小"
	![[qiucachedaxiao.png]]
=== "给出 Byte Address，求 Block Number"
	![[byteaddressqiublocknumber.png]]

### Cache Hit & Miss

显然，读和写的时候发生 cache miss 的处理方式是不一样的；另外读也有读数据和读指令之分（指令 cache 和数据 cache 通常是分离的）。我们分别对其进行讨论：

- **Read**
    - **Hit**
        - 直接从 cache 里读就好了
    - **Miss**
        - **Data cache miss**
            - 从 memory 里把对应的 block 拿到 cache，然后读取对应的内容。
        - **Instruction cache miss**
            - 暂停 CPU 运行，从 memory 里把对应的 block 拿到 cache，从第一个 step 开始重新运行当前这条指令。
- **Write**
    - **Hit** 有两种可以选的方式：
        - **write-through**，即每次写数据时，同时更新 Cache 和主存。这样的好处是 cache 和 main memory 总是一致的，但是这样很慢。
            - 一个改进是引入一个 **write buffer**，即当需要写 main memory 的时候不是立即去写，而是先写入 Buffer 中，找机会再写进主存；此时 CPU 就可以继续运行了。当然，当 write buffer 满了的时候，也需要暂停处理器来做写入 main memory 的工作，直到 buffer 中有空闲的 entry。因此，如果 main memory 的写入速率低于 CPU 产生写操作的速率，多大的缓冲都无济于事。
        - **write-back**，只将修改的内容写在 cache 里，等到这个 block 要被覆盖掉的时候将其写回内存。这种情况需要一个额外的 **dirty bit** 来记录这个 cache block 是否被更改过，从而知道被覆盖前是否需要被写回内存。由于对同一个 block 通常会有多次写入，因此这种方式消耗的总带宽是更小的。
    - **Miss** 同样有两种方式：
        - **write allocate**，即像 read miss 一样先把 block 拿到 cache 里再写入。
        - **write around** (or **no write allocate**)，考虑到既然本来就要去一次 main memory，不如直接在里面写了，就不再拿到 cache 里了。
        - write-back 只能使用 write allocate；一般来说，write-through 使用 write around，其原因是明显的。

!!! quote
	![[cache读写操作总结.png]]
	
	`Sace dirty block first` 指的是，如果要读/写的Block的dirty位为1，则这个Block还在主存中更新。所以要先 Write Memory 更新主存，在继续后面的操作。

### Other Mapping

- 组相联 Set associated
- 全相联 Fully associated

n-way 组相联将每 n 个 Block 分为一组，找对应 Block 时先确定组号，再搜寻一遍该组 Block 找是否命中。direct mapping 其实就是 1-way set associative，而 fully associative 就是 n-way set associative，其中 n 是 cache block 的个数；我们称一组的大小为 **associativity**，那么这两种方法的 associativity 就分别是 1 和 n。

![[othermappingex1.png]]



## Virtual Memory