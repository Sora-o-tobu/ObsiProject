
# Data-Level Parallelism

!!! quote "DLP 指的是数据流中存在大量大量的并行性"
	**SIMD** 指令架构是实现数据级并行的主要方法，常用在面向矩阵的科学计算、面向媒体的图像/视频处理等。
	
	相比于 ILP 的 MIMD，SIMD 更高效节能，因为 SIMD 只需要一条指令流控制多个数据流，因此在移动端应用广泛。

## Vecter Processor

Vector 对标我们之前所说的 Scalar(标量)，它使用单独一条指令对一串类数组的向量数据进行操作，节省了 Instruction Fetch 和 Decode 的时间。

![[VectorProcessorOverview.png]]

!!! note "注意向量加法也不是同时并行计算的，也是单个单个计算，所以说节省了 IF 和 ID 的时间"

一个向量中的计算都不应该依赖于前面的结果，这个数据依赖是由**编译器**来保证的，硬件无需检查，从而确保较高的 Clock Rate。

向量处理器有大量的存储访问需求，其存储器经过特殊设计高度交错，并且不使用 Data Cache。

向量处理器有两种类型：

- **Memory-Memory**
	- 直接操作内存中的数据
	- e.g. CDC Star-100, TI ASC
	- 内存带宽要求较高
	- 检查 Memory 的依赖较为复杂
- **Vector Register**
	- 先将数据加载到寄存器中再进行操作
	- e.g. Cray-1, 以及自 1980s 后的所有 Vector Machines
	- 我们所说的 Vector Processor 都默认为 Vector Register 类型(包括后面讨论的)

以下程序用于简单对比二者区别：

```asm
# C code
for (i=0;i<N;i++)
{
	C[i] = A[i] + B[i];
	D[i] = A[i] - B[i];
}

# Memory-Memory
ADDV    C, A, B
SUBV    D, A, B

# Vector Register
LV      V1, A
LV      V2, B
ADDV    V3, V1, V2
SV      V3, C
SUBV    V4, V1, V2
SV      V4, D
```

在没有结构冲突和数据冲突的条件下，一个向量计算的执行时间取决于*向量长度*。RV64V 指令集中的 FU 一个周期可以处理一个向量元素，因此其执行时间可认定为向量的长度。

每个机器有自己的最大向量长度 **MVL**，即一条指令最多能处理 MVL 个元素。如果向量长度 VL>MVL，则需要对其拆分，生成 VL/MVL 个长度为 MVL 的向量循环。这一操作叫做 **Strip Mining**。

!!! example "有 $VL=N$, $MVL<N<2*MVL$"
	- 先处理 $N\mod MVL$ 个元素
	- 然后处理 $MVL$ 个元素

!!! danger "面向考试复习的话，下面四个最重要的是 Vector Chaining"

<font style="font-weight: 1000;font-size: 20px" color="orange">Vector Chaining</font>

!!! quote "相当于向量版本的 bypassing/forwarding"

![[VectorChainingEx1.png]]

对于有前后依赖的指令，如果有链接技术，则前一个指令的一个分量完成则可以直接进入下一个指令，并不用等待全部分量计算完成。

- **Convey:** 一组可以一起执行的指令
- **Chimes:** 通过链接技术产生的执行序列
- **Chaining:** 链接技术

![[conveyex1.png]]

!!! warning "这里最后的 `vst` 为什么不能和前面两条指令链在一起呢？"
	我们需要保证存储数据的正确性，所以存储指令 `vst` 必须等到整个向量均有效时才能开始写回内存。

!!! question
	=== "题面"
		![[VectorChainQuestion1.png]]
	=== "解答"
		在不使用 chain 的条件下，每条指令必须等待上一条指令所有元素都执行完毕才能启动，因此总时间为将各个指令的时间都相加。例如 `vadd` 指令需要 $1+7+64=72\ cycles$，其它类似，最后相加。
		
		使用 chain，则划分为三组(`vsd`只能单独一组)，总执行时间减去两个 64（前两组可以前递） 

<font style="font-weight: 1000;font-size: 20px" color="orange">Conditional Execution</font>

我们考虑额外设置一个 Masked Vector，通过掩码来控制向量的执行。例如掩码 1010，则只有第 0 个和第 2 个元素会执行运算。

这样，我们在执行时先扫描一遍掩码，只执行需要的元素。而传输数据时也只传输需要的元素，从而节省带宽。

<font style="font-weight: 1000;font-size: 20px" color="orange">Gather/Scatter on Sparse Matrix</font>

对于稀疏矩阵，我们访问或存入可以使用 Gather/Scatter 操作：

- 只把需要的元素的索引传输到寄存器中
- 存入时，只存入需要的元素

```c
// c code
for (i=0; i < N; ++i)
	A[i] = B[i] + C[D[i]];
```

```asm
LV      VD, RD        # load indices into vector D
LVI     VC, (RC, VD)  # load indirect from RC base
LV      VB, RB        # load vector B
ADDV.D  VA, VB, VC    # DO ADD
SV      VA, RA        # store result
```

<font style="font-weight: 1000;font-size: 20px" color="orange">Multi-Lane Implementation</font>

划分为多个 Lane，同一个向量的不同分量可以在不同的 Lane 上存储和计算：

![[MultiLaneEx1.png]]

![[MultiLaneEx2.png]]

## GPU

GPU 按线程执行，属于 Single Instruction Multiple Threads。

一组线程称为 Block，Block 又被排列成 Grid。

GPU 与向量机不同的是：

- 没有标量处理器
- 通过多线程来减小内存延迟
- 有非常多 FU

## Loop-Level Parallelism

基于循环的并行主要看循环的迭代之间是否有数据依赖（loop-carried dependence），例如：

```c
// 没有循环间依赖
for (i = 999; i >= 0; ++i)
	x[i] = x[i] + s;

// 有循环间依赖，S1 需要用到上一轮 S2 的输出
for (i = 0; i < 100; ++i)
{
	A[i]   = A[i] + B[i]; // S1
	B[i+1] = C[i] + D[i]; // S2
}
```

循环间依赖可以通过改写解决：

```c
A[0] = A[0] + B[0];
for (i = 0; i < 99; ++i)
{
	B[i+1] = C[i]  + D[i];
	A[i+1] = A[i+1]+ B[i+1];
}
B[100] = C[99] + D[99];
```

!!! note "改写后不仅解决了循环间依赖，还可以使用向量链接"

