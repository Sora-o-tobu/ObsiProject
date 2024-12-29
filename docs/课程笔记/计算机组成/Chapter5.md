# Chapter 5. Memory Hierarchy

## Memory Hierarchy Overview

存储容量和读写速度不可兼得。

Cache的出现解决了主存速度不够快的问题，也是这门课程讨论的重点。

!!! quote "Memory Hierarchy"
	![[memoryhierarchy1.png]]

- **Hit** The CPU accesses the upper level and succeeds
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
	对于 32bit 主存地址、4-Block Cache、1-word Block，则一共需要 2bit 作为索引index，2 bit 用作 Offset，剩余 28bit 用作 TAG。在我们之前已访问过主存 `0x04` 的情况下，执行 `load r1, 0x04` ：
	
	![[cachesimpleex1.png]]

```
Cache 地址映射：
+-------+--------+-------+
|标签TAG|索引INDEX|块偏移量|
+-------+--------+-------+

Cache 结构：
+----------------+----------+-------+-------+
|    标签TAG      | 数据DATA | Valid | Dirty |
+----------------+----------+-------+-------+
 64-(n+m+2) bit   2^m*32 bit   1bit   (1bit)

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

!!! danger "写题时出现的Block Size均指Data段大小，如 two-word block 指 64 bits data"

!!! danger "通常，1 word 的大小要看题目给出的地址位数决定；在 8 位系统中，1 word = 1 byte"

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
        - **write-through**，即每次写数据时，同时更新 Cache 和主存。这样的好处是 cache 和 main memory 总是一致的，但是这样相当于直接写主存，速度很慢。
            - 一个改进是引入一个 **write buffer**，即当需要写 main memory 的时候不是立即去写，而是先写入 Buffer 中，找机会再写进主存；此时 CPU 就可以继续运行了。当然，当 write buffer 满了的时候，也需要暂停处理器来做写入 main memory 的工作，直到 buffer 中有空闲的 entry。因此，如果 main memory 的写入速率低于 CPU 产生写操作的速率，多大的缓冲都无济于事。
        - **write-back**，只将修改的内容写在 cache 里，等到这个 block 要被覆盖掉的时候将其写回内存。这种情况需要一个额外的 **dirty bit** 来记录这个 cache block 是否被更改过，从而知道被覆盖前是否需要被写回内存。由于对同一个 block 通常会有多次写入，因此这种方式消耗的总带宽是更小的。
    - **Miss** 同样有两种方式：
        - **write allocate**，即像 read miss 一样先把 block 拿到 cache 里再写入。
        - **write around** (or **no write allocate**)，考虑到既然本来就要去一次 main memory，不如直接在里面写了，就不再拿到 cache 里了。
        - write-back 只能使用 write allocate；一般来说，write-through 使用 write around，其原因是明显的。

!!! quote
	![[cache读写操作总结.png]]
	
	`Sace dirty block first` 指的是，如果要读/写的Block的dirty位为1，则这个Block还未在主存中更新。所以要先 Write Memory 更新主存，在继续后面的操作。


!!! warning "在做Cache计算时间类题目中，注意每步操作的时间要是时钟频率的整数倍（结果可以不是）"

### Other Mapping

- 组相联 Set associated
- 全相联 Fully associated

n-way 组相联将每 n 个 Block 分为一组，找对应 Block 时先确定组号，再搜寻一遍该组 Block 找是否命中。direct mapping 其实就是 1-way set associative，而 fully associative 就是 n-way set associative，其中 n 是 cache block 的个数；我们称一组的大小为 **associativity**，那么这两种方法的 associativity 就分别是 1 和 n。

![[othermappingex1.png]]

!!! warning "对于组相联"
	由于实际应用中计算组别是用 块地址%组数 来得到的，因此一个组内 Block 对应的物理地址可能相差比较大。

替换算法：

- <1> 先入后出算法 FILO
- <2> 随机替换算法 RAND
	- 方法简单，易于实现；命中率较低
- <3> 先入先出算法 FIFO
	- 最先调入并被多次调用的Block可能会被优先替换，不符合局部性规律，所以命中率不满足要求
- <4> 近期最少使用算法 LRU
	- 较好反映了程序局部性规律，命中率较高

??? quote "The implement of Four-way Set-associative Cache"
	![[四路组相联cache示例.png]]

考试常考三种 Miss (3C)：

- 冷启动 Miss **(Compulsory Miss)**
	- 缓存刚上电为空，此时访问什么都是 Miss
- 容量 Miss **(Capacity Miss)**
	- 指**全相联**缓存中，内存中的块由于容量不够而被替换，再次访问该块时发生 Miss
- 冲突 Miss **(Conflict Miss)**
	- 指**组相联**缓存中，某个组内的块被替换，然后再次访问该块时发生 Miss
	- 如果是**全相联**则不会发生 Conflict Miss，因为还有别的空位（如果没有空位的话就该计入 Capacity Miss 了）

!!! example "提高 Cache 的组相联度可以优化？"
	- A. Capacity Miss
	- B. Hit time
		- 一组内块变多了，因此判断 hit 时要比较的 Tag 也变多了
	- C. Conflict Miss
		- 选 C
	- D. Compolsory Miss

### Performance

$$\begin{array}l
\text{Average Memory Access time}& = \text{hit time} \times \text{miss time} \\ & = \text{hit time} + \text{miss rate} \times \text{memory time}
\end{array}
$$

因此，提高 Cache 性能的关键在于增加命中率和减少 Miss Penalty 时间。

[Example](#) 假设：

- instruction cache miss rate:  2%
- data cache miss rate:  4%
- CPI without any memory stall:  2
- miss penalty:  100 cycles
- The frequency of all `load` and `store` in gcc: 36%

$$\begin{array}l
\text{Instruction miss cycles} =I\times 2\% \times 100 & = 2.00I\\
\text{Data miss cycles} = I\times 36\% \times 2\times 4\% \times 100 & = 1.44I \\
\text{Total memory-stall cycles} =2.00 I +1.44I & =3.44I \\
\text{CPI with stall} = 2 + 3.44 I / I & =5.44
\end{array}
$$

!!! info "这也能看出 Memory 的性能严重制约了 CPU"

[Solution](#)

- <1> 通过更灵活的Block替换规则来减少 Miss Rate
	- 组相连、LRU...
	- 随着 Assoiciativity 提高，Miss Rate 越来越低（Direct > Set > Full）
- <2> Least Recent Used
	- 随着 Assoiciativity 提高，实现 LRU 算法越来越困难
- <3> 选择合适 Block Size
	- 一般来讲，选择更大的Block可以降低 Miss Rate，但也会带来更大的 Miss Penalty
	- 并且对于一个大小有限的 Cache，更大的Block也意味着更少的Block数量，可能反而增加了 Miss Rate
	- 对于 256KB 往下的Cache，Block Size选 64 bytes 的效果最好
- <4> 设计一个存储系统来减少 Miss Penalty
	- 交叉地址访问...
- <5> 通过多级 Cache 减少 Miss Penalty
	- L1 Cache、L2 Cache...
- <6> Software Optimization Blocking


## Virtual Memory

### Mapping

!!! quote "虚拟存储可以看作是 DRAM 和 Disk 间的 Cache（尽管我认为差别很大）"

虚拟内存技术可以让多个程序之间高效、安全地共享内存，同时允许单个程序使用超过内存容量的内存。

操作系统会为每一个进程分配一个 Page Table，进程通过虚拟内存这一中介来跟内存和磁盘读写信息。

![[virtualmemory1.png]]

!!! info "虚拟内存允许单个程序使用超过内存容量的内存，超出的部分可以临时存放在Disk中"

虚拟地址转换到物理地址分为两步：

- <1> 根据 Virtual page number 读取 Page Table 上对应数据，记作 Physical page number
	- 如果 Valid 位为0，说明不在 DRAM 中，要去 Disk 中找
- <2> 将 Physical page number 与 Page Offset 合并，得到物理地址


```
Virtual Address:
+---------------------+-------------+
| Virtual page number | Page Offset |
+---------------------+-------------+
      || Translation
      \/
Physical Address:
+----------------------+-------------+
| Physical page number | Page Offset |
+----------------------+-------------+
```

!!! info "Page Offset"
	与 Cache 中的 Byte Offset 概念类似，代表了一个 Page 的 Byte 数。例如，对于 Page Size = 4KB 的 Cache，Page Offset 为 $2^{12}$ ，即 12bit。

由于 Virtual Memory 处于较下层的位置，如果出现 Page Fault 的情况，造成的性能损失会相当大，因此优化的主要目标在于减少 Page Fault 的发生。

所以，Virtual Memory 采用 LRU、全相联映射的方式来处理 Page，从而减小失效率；同样，Write Through 写 Disk 的代价也是非常昂贵的，所以使用Write Back（这也需要为 Page Table 增加 Dirty 位）。

### Translation-lookaside Buffer

每个进程的 Page Table 都存放在内存中；而 **Page Table Register** 则存放着该 Page Table 的物理地址，因此进行 **Translation** 时将 Virtual Page Number 对应的偏移量和 Page Table 的物理地址相加即可得到该 Entry 的地址。

但是上述方法存在效率问题，当进程希望访问内存中的数据时，都需要先访问内存中的 Page Table 以获取数据的物理地址，这意味着对内存访问时间会加倍。

一种解决方法是使用 **TLB**，其作用相当于 Page Table 的高速缓存，位于进程和内存中间。其基本的 Entry 结构为：

```
+-----+-----+---+---------------+-------------------------+
|Valid|Dirty|Ref|      Tag      |  Physical Page Address  |
+-----+-----+---+---------------+-------------------------+

- Reference 位: 最近是否被访问过(lru ?)
- TLB: 16-512 Entries
- Miss Rate: 0.01% - 1%
```

!!! note "TLB 在实际应用中选择组相联的方式，不过计组课程似乎并不深究原理"
	- 对于全相联 TLB 题目中的 Tag，一般认为就是 Virtual Page Number 。
	- 对于组相联 TLB 题目中的 Tag，则认为是 Virtual Page Number 减去对应的 index 位数
		- 例如，Virtual Page Number 为四位，TLB共有 2 个 set，则 Tag 为 `Virtual Page Number[3:1]`

当 TLB Miss 时，处理器才会去内存中的 Page Table 找对应项；如果 Page Table 对应项是 Valid 的，则将其拿到 TLB 中；如果 Page Table 对应项不是 Valid 的，则触发 Page Fault，要继续向下层读取；若被替换的 Entry 的 Dirty 位为 1，则要将其写回 Page Table。

![[TLBOverview.png]]

| TLB  | Page Table | Cache | Description                                                 |
| ---- | ---------- | ----- | ----------------------------------------------------------- |
| Hit  | Hit        | Miss  | 在TLB中得到物理地址，但是在 Cache 中找不到该地址对应数据，要在内存中找                    |
| Miss | Hit        | Hit   | TLB misses, 但是在 Page Table 中找到 Entry；Data在Cache中找到          |
| Miss | Hit        | Miss  | TLB misses, 但是在 Page Table 中找到 Entry；在Cache中找不到对应数据，最后去内存中找 |
| Miss | Miss       | Miss  | Page Fault；那么 Cache 中一定不存在这个地址及其数据                          |
| Hit  | Miss       | Miss  | **Impossible**；TLB 命中了，那 Page Table 也一定能命中                  |
| Hit  | Miss       | Hit   | **Impossible**；TLB 命中了，那 Page Table 也一定能命中                  |
| Miss | Miss       | Hit   | **Impossible**；Page Table 中没有这个地址，说明 Cache 中也没有             |

