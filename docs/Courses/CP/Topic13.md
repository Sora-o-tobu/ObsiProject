
# Garbage Collection

## Manual vs. Automatic Memory Management

堆（heap）上的记录何时分配、何时回收，有两种管理方式。

**手动管理（Manual Memory Management）**——C、C++ 的做法：用 `malloc` / `free` 显式地分配和释放指针指向的动态内存。它的问题很多：

- 极易出错：**内存泄漏（memory leak）**、**重复释放（double free）**、**释放后使用（use after free）**；
- 破坏**类型安全（type safety）**；
- 存储 bug **极难定位**——一个 bug 造成的可见后果，往往在**时间上和代码位置上都远离**其根源。

另一种方式是**自动管理（Automatic Memory Management）**——内存回收由系统自动完成。这里的核心概念是**垃圾（garbage）**：已分配但**不再被使用**的存储。

!!! info "垃圾的理想定义 vs. 可判定的近似"
	- **理想定义**：任何动态不活跃（not dynamically live，即将来不会再被使用）的记录就是垃圾。
	- 但"一个对象是否是垃圾"是**不可判定的（undecidable）**——无法精确判断将来是否会用到。
	- 因此必须依赖一个保守近似（conservative approximation）：凡是无法从程序变量经由任何指针链到达（reachable）的堆记录，就是垃圾。

这条近似的方向性必须记清楚：

$$
\text{不可达}\;\Rightarrow\;\text{是垃圾}\qquad\qquad \text{是垃圾}\;\not\Rightarrow\;\text{不可达}
$$

也就是说"可达"是对"活跃"的**安全过近似**：可达集 ⊇ 活跃集。被判为可达的对象可能其实已经死亡（这种对象称为**漂浮垃圾 / floating garbage**，本轮不会被回收），但**绝不会**把真正还要用的对象误判为垃圾——这保证了正确性。

**可达性（reachability）的递归定义**：对象 x 是可达的，当且仅当

- 某个**寄存器**中含有指向 x 的指针，**或**
- 另一个**可达对象** y 中含有指向 x 的指针。

!!! note "什么是垃圾回收（GC）"
	**垃圾回收（Garbage Collection）**：在**没有显式 `free` 调用**的情况下，回收"已分配但不再使用"的存储的过程。GC **不是由编译器**完成，而是由**运行时系统（runtime system，与编译后代码链接在一起的支持程序）**完成。

**【一个贯穿全章的例子】**

```text
let
  type list = {link: list, key: int}
  type tree = {key: int, left: tree, right: tree}
  function maketree() = ···
  function showtree(t: tree) = ···
in
  let var x := list{link=nil, key=7}
      var y := list{link=x, key=9}
  in x.link := y
  end;
  let var p := maketree()
      var r := p.right
      var q := r.key
  in garbage-collect here        // 在此处触发 GC
     showtree(r)
  end
end
```

触发 GC 时，上一个 let 块中的 `x` 和 `y` 理应被回收。

下图给出此刻的堆：左侧是程序变量 `p、q、r`，右侧是堆中的记录。**哪些节点可达、哪些不可达？**——这正是 GC 要回答的问题。

![[cp_topic13_1.png]]

下面依次介绍四类经典算法：**Mark-and-Sweep**、**Reference Counts**、**Copying Collection**，以及 GC 与编译器的接口。

## Mark-and-Sweep Collection

程序变量与堆记录构成一张**有向图**：变量是图的**根（roots）**，记录是节点，指针是边。一个节点**可达**，当且仅当存在一条从某个根出发的有向路径 $r \to \cdots \to n$。

标记-清扫法分两个阶段。

### Mark 阶段

用一次图搜索（如**深度优先搜索 DFS**）标记所有可达节点：

```text
function DFS(x)
  if x is a pointer into the heap
    if record x is not marked
      mark x
      for each field f_i of record x
        DFS(x.f_i)
```

对每个根 `v` 调用 `DFS(v)`，结束后所有**可达**记录都被打上标记（mark bit）。

### Sweep 阶段

任何**未被标记**的节点必是垃圾，应当回收。做法是**线性扫描整个堆**，从第一个地址到最后一个地址：

- 遇到**未标记**的记录 → 它是垃圾，**链入空闲链表（freelist）**；
- 遇到**已标记**的记录 → 把它的标记位**清除（unmark）**，为下一次 GC 做准备。

![[cp_topic13_2.png]]

左图 (a) 是 Mark 之后（黑块表示标记位），右图 (b) 是 Sweep 之后：可达记录保留并被清除标记，不可达记录被串入 freelist。

完整算法（教材 **Algorithm 13.3**）：

```c
Mark phase:
  for each root v
    DFS(v)

Sweep phase:
  p ← first address in heap
  while p < last address in heap
    if record p is marked
      unmark p
    else
      let f1 be the first field in p
      p.f1 ← freelist          // 把 p 头插进 freelist
      freelist ← p
    p ← p + (size of record p)
```

GC 结束后程序恢复执行；每当需要堆分配一条新记录，就从 **freelist** 取一个；若 freelist 为空，就再触发一次 GC 来补充。

### 代价分析

!!! info "时间代价"
	- **Mark（DFS）**：与它标记的节点数（可达节点数 $R$）成正比；
	- **Sweep**：与整个堆的大小 $H$ 成正比。

设标记每个可达字的代价为 $c_1$、清扫每个堆字的代价为 $c_2$，一次 GC 的代价约为 $c_1 R + c_2 H$，回收得到 $H-R$ 个可用字。则**每回收一个字的摊还代价**为：

$$
\frac{c_1 R + c_2 H}{H - R}
$$

当 $H \approx R$（堆几乎被占满、可回收的很少）时，这个值**急剧增大**——刚回收完没多久又得再 GC。反之，**堆越大、$R$ 相对越小，单位回收代价越低**（趋近常数 $c_2$）。这说明 mark-sweep 要工作得高效，需要保证堆有足够的空闲比例。

!!! warning "空间代价：递归 DFS 的栈可能爆掉"
	DFS 是递归的，最坏情况下（一条长链）调用栈的深度会**超过整个堆**——而每个活动记录（activation record）比一个堆字大得多。
	**解决：用显式栈代替递归**，这样只需 $H$ 个字，而不是 $H$ 个活动记录。

#### 用显式栈

把递归改成一个显式的工作表（worklist）栈，`t` 是栈顶下标：

```text
function DFS(x)
  if x is a pointer and record x is not marked
    mark x
    t ← 1; stack[t] ← x
    while t > 0                       // 栈非空
      x ← stack[t]; t ← t − 1
      for each field f_i of record x
        if x.f_i is a pointer and record x.f_i is not marked
          mark x.f_i
          t ← t + 1; stack[t] ← x.f_i
```

但显式栈在最坏情况下仍可能需要 $H$ 个字的额外空间。能不能**完全不用额外的栈**？

#### 指针反转（Pointer Reversal）

关键观察：当 `x.f_i` 的内容被压栈后，算法**再也不会去读原来的 `x.f_i` 这个位置**了。与其浪费这个空间，不如让 **`x.f_i` 反过来指向"我们是从哪条记录到达 x 的"**——即把栈链**藏进记录的字段里**。等处理完 `x.f_i` 子树、需要回溯时，再通过它找回父节点，并把字段**恢复原值**。这就是**指针反转（pointer reversal）**：用记录自身的指针字段临时充当 DFS 栈，弹栈时逐一复原。

```c
function DFS(x)
  if x is a pointer and record x is not marked
    t ← nil                           // t: 栈顶（反转链的头）
    mark x; done[x] ← 0               // done[x]: x 已处理的字段数
    while true
      i ← done[x]
      if i < (# of fields in record x)
        y ← x.f_i
        if y is a pointer and record y is not marked
          x.f_i ← t; t ← x; x ← y     // 下潜：反转指针、压栈
          mark x; done[x] ← 0
        else
          done[x] ← i + 1
      else                            // x 的字段都处理完了，回溯
        y ← x; x ← t
        if x = nil then return
        i ← done[x]
        t ← x.f_i; x.f_i ← y          // 弹栈：恢复 x.f_i 原值
        done[x] ← i + 1
```

- `done[x]`：记录每个节点已经处理了几个字段；当 `i = done[x]` 时，`x.f_i` 正是指向上一层节点的"栈链"。
- 下潜时把 `x.f_i` 改写成指向父节点（`t`），完成"压栈"；
- 回溯（弹栈）时，把 `x.f_i` **恢复**为它本来的值 `y`。

### 碎片化（Fragmentation）

![[cp_topic13_4.png]]

!!! note "两种碎片"
	- **外部碎片（External fragmentation）**：程序想分配大小为 $n$ 的记录，freelist 里有很多比 $n$ **小**的空闲块，却没有恰好合适的——空闲内存被切得太碎，散落在已用记录**之间**。
	- **内部碎片（Internal fragmentation）**：程序图省事，用了一块**过大**的记录而不去切分，于是未用的内存被困在记录**内部**。

Mark-and-sweep 由于"原地回收、不移动对象"，会逐渐产生**外部碎片**。后面的复制式收集（copying collection）可以彻底消除碎片。

## Reference Counts

Mark-sweep 是**先找出可达的**，再反推垃圾。引用计数则**直接**统计指向每条记录的指针个数。

!!! info "引用计数（reference count）"
	一条记录的**引用计数** = 指向该记录的指针数目。把这个计数**和记录存在一起**。

**如何维护？** 编译器额外生成指令：每当把 `p` 存入 `x.f_i` 时，

- 把 `p` 的引用计数 **加 1**；
- 把 `x.f_i` 原先所指对象的引用计数 **减 1**。

若某记录 `z` 的引用计数**降到 0**，则把 `z` 链入 freelist，并把 **`z` 所指向的所有其它记录的引用计数也各减 1**（递归减计数）。

单条 `x.f_i ← p` 因此被展开成一长串指令：

```text
z ← x.f_i
c ← z.count
c ← c − 1
z.count ← c
if c = 0 call putOnFreelist      // 递归减计数在此发生
x.f_i ← p
c ← p.count
c ← c + 1
p.count ← c
```

!!! note "优化：把递归减计数推迟到「从 freelist 取出时」再做"
	与其在 `r` 被放入 freelist 时就立刻递归减少 `r.f_i` 的计数，不如等到 `r` 被**从 freelist 取出**时才做，理由有二：
	
	1. 把"递归减计数"的长任务**拆成小块**，程序运行更平滑（这对**交互式 / 实时程序**很重要）；
	2. 递归减计数的代码**只需在分配器（allocator）一处**出现。

!!! warning "问题一：垃圾环（reference cycle）无法回收"
	**引用环**是一组相互循环引用的对象（如上例中存 7 的记录和存 9 的记录互指）。引用计数统计的是**引用的数目**，而不是**可达引用的数目**——环中每个对象的计数都 ≥ 1，即使整个环已经从根不可达，也永远不会降到 0。这是采用引用计数的语言/系统的**主要缺陷**（例如 Perl、Firefox 2 都曾深受其害）。

!!! warning "问题二：维护计数非常昂贵"
	本可由**一条**机器指令 `x.f_i ← p` 完成的事，现在要执行上面那一长串指令（读旧值、减计数、判零、写新值、增计数……）。频繁的指针赋值会被显著拖慢，而且这些额外的内存读写还会**损害缓存**。

小结：引用计数的优缺点

- **优点**：实现简单。
- **缺点**：无法回收所有不可达对象（环）；一旦触发大规模递归回收可能很慢；**明显拖慢赋值操作**。

## Copying Collection

**思路**：把内存分成**两半**，通过**复制**来收集。

可达部分的堆是一张有向图（节点=记录、边=指针、根=程序变量）。复制式收集：

- 在堆的一半（称为 **from-space**）中遍历这张图；
- 在另一块新鲜区域（称为 **to-space**）中构建一份**同构的副本（isomorphic copy）**；
- to-space 中的副本是**紧凑的**：占用**连续内存、没有碎片**；
- 把根改为指向 to-space 的副本——于是整个 from-space 都变成不可达，可整体丢弃。

![[cp_topic13_6.png]]

!!! success "复制式收集的两大好处"
	- **分配极快**：只需 `p = next; next = next + n`（指针前移），无需遍历 freelist。
	- **没有碎片问题**：存活对象被压实到连续区域。

收集结束后，from-space 与 to-space **角色互换**，下次收集时反向复制。

### 转发（Forwarding）

- **初始化**：把指针 `next` 指向 to-space 起点。
- **转发**：给定一个指向 from-space 的指针 `p`，让 `p` 指向它在 to-space 中的副本。

```text
function Forward(p)
  if p points to from-space
    if p.f1 points to to-space        // 情况 1：已经复制过
      return p.f1                     //   p.f1 是"转发指针"，指向副本位置
    else                              // 情况 2：尚未复制
      for each field f_i of p
        next.f_i ← p.f_i              //   把各字段拷到 to-space
      p.f1 ← next                     //   在原记录留下转发指针
      next ← next + size of record p
      return p.f1
  else                                // 情况 3：不是指针，或指向 from-space 之外
    return p
```

三种情况：(1) 已复制——`p.f1` 是指向副本的**转发指针**，直接返回；(2) 未复制——拷贝字段、留下转发指针、`next` 后移；(3) 不是指向 from-space 的指针——原样返回。

### Cheney 算法

!!! info "Cheney's Algorithm"
	用广度优先搜索（BFS）遍历可达数据，**巧妙之处是直接用 to-space 本身充当 BFS 的队列**，无需额外的队列存储，也无需指针反转。

```text
scan ← next ← beginning of to-space
for each root r
  r ← Forward(r)
while scan < next
  for each field f_i of record at scan
    scan.f_i ← Forward(scan.f_i)
  scan ← scan + size of record at scan
```

- `next`：指向"已被复制到 to-space"的记录的末尾（新对象在此之后追加）；
- `scan`：指向"其字段尚未被转发处理"的记录；
- **`scan` 与 `next` 之间的区域，就是 BFS 的队列**：先把所有根转发（入队），再不断扫描 `scan` 指向的记录、转发其字段、推进 `scan`，直到 `scan` 追上 `next`。

![[cp_topic13_7.png]]

上图三步：(a) 收集前；(b) 根被转发到 to-space（`12` 等被复制，`scan`/`next` 就位）；(c) 扫描完一条记录后，其字段也被转发、`scan` 前移。

!!! warning "BFS 复制的局部性很差"
	- 用广度优先复制出来的指针数据结构，**引用局部性（locality of reference）很差**：若地址 `a` 的记录指向地址 `b` 的记录，复制后 `a` 与 `b` 很可能**相距很远**。
	- 在带**虚拟内存**或**高速缓存**的系统里，好的局部性很重要：程序访问地址 `a` 后，内存子系统**预期** `a` 附近的地址不久也会被访问。
	- **深度优先复制**的局部性更好（父子相邻），但 DFS 复制需要**指针反转**，既不方便又慢。

### 混合算法（Hybrid）

!!! note "部分深度优先 + 部分广度优先"
	**基本思想**：以广度优先复制为主，但**每复制一个对象，就顺便看看能不能把它的某个孩子复制到它旁边**，从而兼顾局部性。

```text
function Forward(p)
  if p points to from-space
    if p.f1 points to to-space
      return p.f1
    else Chase(p); return p.f1
  else return p

function Chase(p)
  repeat
    q ← next
    next ← next + size of record p
    r ← nil
    for each field f_i of record p
      q.f_i ← p.f_i
      if q.f_i points to from-space and
         q.f_i.f1 does not point to to-space
        then r ← q.f_i              // 记下一个尚未复制的孩子
    p.f1 ← q
    p ← r                           // 下一轮去复制那个孩子（紧挨着放）
  until p = nil
```

这样能在不做指针反转的前提下，获得**可接受的局部性**。

## Interface to the Compiler

被 GC 管理的语言，其编译器要与垃圾收集器在四个方面打交道：

1. 生成**分配记录**的代码；
2. 为每次 GC 周期**描述根（roots）的位置**；
3. **描述堆上数据记录的布局（layout）**；
4. 为某些增量收集生成实现**读/写屏障（read / write barrier）**的指令。

### 快速分配（Fast Allocation）

创建堆记录有可观的开销。若采用**复制式收集**，分配空间是一块**连续的空闲区**：`next` 指向下一个空闲位置，`limit` 指向区域末尾。

分配一条大小为 N 的记录，原本要 6 步：

```text
1. 调用 allocate 函数
2. 测试 next + N < limit ?            (若失败则调用 GC)
3. 把 next 移入 result
4. 清零 M[next], M[next+1], …, M[next+N−1]
5. next ← next + N
6. 从 allocate 函数返回
   ── 之后 ──
A. 把 result 移到某个有用的位置
B. 把有用的值存入该记录
   (步骤 A、B 不算分配开销)
```

通过一系列优化可大幅削减开销：

- **步骤 1、6**：通过**内联展开（inline expand）** allocate 函数消除（省掉调用/返回）；
- **步骤 3**：常可与步骤 A **合并**消除（直接把 `next` 移到目标位置）；
- **步骤 4**：可由步骤 B 代替消除——反正马上要往记录里**存有用的值**，这些初始化写入本身就完成了"清零"的效果；
- **步骤 2、5**：**无法消除**，但可以在一个基本块内的**多次分配之间共享**（合并边界检查与指针推进）。

!!! success "最终代价：约 4 条指令"
	把 `next` 和 `limit` **保存在寄存器**中，步骤 2、5 总共只需 **3 条指令**。综合上述技巧，分配一条记录（以及最终回收它）的代价可降到**约 4 条指令**。

### 描述数据布局（Describing Data Layouts）

收集器必须能处理程序声明的**任意类型**的记录：要能确定**每条记录有几个字段**、以及**每个字段是否是指针**。这些信息从**语义分析（semantic analysis）**得来。

!!! note "类型/类描述符（type-/class-descriptor）"
	对静态类型语言（如 Tiger、Pascal）或面向对象语言（如 Java）：让**每个对象的第一个字**指向一条特殊的**类型描述符 / 类描述符**记录，其中记录了：

	- 对象的**总大小**；
	- **每个指针字段的位置**。

	类型/类描述符由编译器根据静态类型信息（语义分析结果）生成。

开销对比：

- **静态类型语言**：每条记录多花**一个字**（描述符指针）的开销；
- **面向对象语言**：**没有额外的、归因于 GC 的每对象开销**——因为它们本来就需要这个描述符指针来实现**动态方法查找（dynamic method lookup）**。

### 指针映射（Pointer Map）

编译器必须向收集器提供一张**指针映射（pointer map）**：标明**每一个含指针的临时变量和局部变量**，以及它当前**在寄存器里还是在活动记录（栈帧）里**。

由于活跃的临时变量**在每条指令处都可能变化**，指针映射在程序的每个点都不同。简化做法是：**只在"可能开始一次新 GC"的点描述指针映射**。哪些点会触发 GC？

- 对 `alloc` 函数的调用；
- **任何函数调用**——因为被调用的函数可能在内部再调用 `alloc`。

!!! info "指针映射按返回地址（return address）索引"
	最好用**返回地址**作为指针映射的键，因为返回地址正是收集器在**下一个活动记录**里会看到的东西。
	**寻找所有根**的过程：收集器从**栈顶向下扫描**；每个返回地址索引到描述**下一个栈帧**的指针映射条目；在每个栈帧中，收集器从那些指针出发进行 mark（或 forward，若是复制式收集）。

!!! warning "Callee-save 寄存器需要特殊处理"
	设函数 `f` 调用 `g`，`g` 又调用 `h`。`g` 的指针映射必须说明：在调用 `h` 这一点上，它的**哪些 callee-save 寄存器含有指针**，以及哪些是从 `f` "**继承（inherited）**"下来的（`g` 自己并不知道这些继承值是不是指针，需如实标注以便正确回溯）。

### 派生指针（Derived Pointers）

有时编译出的程序持有一个指向**堆记录中间**、或指向记录**之前/之后**的指针。例如 `a[i-2000]` 在内部可能算成 `M[a-2000+i]`：

```text
t1 ← a − 2000
t2 ← t1 + i
t3 ← M[t2]
```

若 `a[i-2000]` 在**循环内**，编译器可能把 `t1 ← a − 2000` **提到循环外**（hoist）以避免每次重算。这时若循环里还有 `alloc`，且 GC 恰在 `t1` 活跃时发生：**`t1` 并不指向某个对象的开头**，甚至（更糟）指向一个**不相关的对象**。

!!! info "处理办法：在指针映射中登记派生关系"
	我们说 `t1` 是从**基指针（base pointer）`a`** **派生（derived）**而来的。收集器会被 `t1` 迷惑，所以：

	- 指针映射必须**标明每个派生指针及其基指针**；
	- 当收集器把 `a` 重定位到新地址 `a'` 时，必须把 `t1` 相应调整为 `t1 + a' − a`；
	- **只要 `t1` 还活跃，`a` 就必须保持活跃**。

考虑下面的循环（`a` 在赋给 `t1` 之后看似就死了）：

```text
let
  var a := int array[100] of 0
in
  for i := 1930 to 1990
    do f(a[i-2000])
end
```

```text
r1 ← 100
r2 ← 0
call alloc
a ← r1
t1 ← a − 2000
i ← 1930
L1: r1 ← M[t1 + i]
    call f
L2: if i ≤ 1990 goto L1
```

`a` 在 `t1 ← a − 2000` 之后看似为死变量；但这样一来，与返回地址 `L2` 关联的指针映射就**无法"解释" `t1`**（不知道它的基指针还在不在、被重定位到哪里）。**因此，一个派生指针会隐式地让它的基指针保持活跃**——`a` 必须一直活到 `t1` 不再使用为止。

