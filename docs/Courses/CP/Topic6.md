
# Activation Records

## Stack Frame

普通语言（C、Pascal、Tiger）：局部变量在函数返回后销毁 → 可以安全用栈。

但支持**嵌套函数 + 函数作为返回值**的语言（ML、Scheme）不能完全依赖普通栈来存放所有局部变量：

```c
fun f(x) = let fun g(y) = x + y in g end
val h = f(3)
val z = h(5)  // f(3)(5)
```

![[cp_topic6_1.png]]

在编译原理中，我们称一个函数的栈帧即为 **Activation Records**，它是栈上一块存储了关于该函数的 *local variables、parameters、RA* 以及一些 *Temporaries* 的区域。

一般来讲，Stack Pointer 指向当前栈顶，而 Frame Pointer 指向当前帧的基准位置：

![[cp_topic6_2.png]]

在一个 Caller 调用一个函数 Callee 时，会先将当前旧的 Frame Pointer 存储到栈中，然后设置 $FP=SP$，$SP=SP-\#framesize$。从 Callee 退出时，先设置 $SP=FP$，然后从栈中取出并恢复旧的 Frame Pointer。

!!! note "如果 Frame Size 时固定的，则退出时直接设置 $FP=FP+\#frameszie$ 即可"

实际进行参数传递时，前 k 个参数（通常4~6个）会用寄存器传递，其它参数才通过栈传递。这是因为寄存器操作比栈内存操作快得多。

作为优化，我们可以有以下几种策略：

- **Leaf procedure**（不调用其他函数的函数）：无需把传入参数写入内存。
- **过程间寄存器分配**（interprocedural）：全局分析，复用寄存器。
- **寄存器窗口**（某些架构）：每个调用自动切换新寄存器组。

很多变量/临时值优先放**寄存器**，只有**必要时**才写到栈帧。必要原因（escape变量）：

- 该变量之后会被引用，因此需要一个 Memory Address
	- 例如 C 语言中的 `&`
- 该变量被嵌套函数访问
- 变量太大，不能放入单个寄存器中
- 该变量是 array，需要地址操作
- 寄存器不够用 → **spill**（溢出到栈）

!!! info "**Escaping Variable**：必须分配到内存的变量"

**Static Links** 用来处理嵌套（声明）函数。Pascal、ML、Tiger 等语言允许函数嵌套声明，并且内层函数能够访问外层变量。**实现方式**：

- 每个函数调用时，额外传递一个**静态链（Static Link）**：指向**词法上直接外层**函数的帧。
- 通过**静态链链**（可能多次fetch）访问更外层的变量。
- 链长度 = 嵌套深度差，也决定了访问开销


## Frames in Tiger Compiler

略