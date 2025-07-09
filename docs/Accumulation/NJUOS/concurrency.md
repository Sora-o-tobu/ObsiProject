
# 并发

!!! quote "并发和并行的区别"
	- 并发是逻辑上的“同时执行”，是由操作系统或运行库模拟出的**轮流执行**
		- 真正意义的同时执行当然也属于并发
	- 并行是真正意义上的“同时执行”，有共享内存的多个处理器同时执行指令

## 多处理器编程

为了应对多线程编程的需求，操作系统新增加了一个**可共享内存**的进程管理 API：`spawn()`。

使用 `spawn()` 创建的状态机和原进程共享**全局变量**，而拥有独立的栈。

jyy 课上用的是他自己写的小头文件 `thread.h`，只有 `spawn(fn)` 和 `join` 两个函数。这里我们还是直接使用标准库 `pthread.h`：

- `pthread_create(&tid, NULL, fn, &arg)` 创建一个入口函数为 `fn` 的线程，并立即开始执行
	- `tid` 用于接收保存线程 ID，
- `pthread_join(tid, NULL)` 等待指定线程的返回，第二个参数用于接收返回值，一般为 `NULL`
	- `main` 函数默认会 join 所有线程

```c++
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
} ThreadArg;

int a;

void* thread_func(void* arg) {
    ThreadArg* t_arg = (ThreadArg*)arg;
    pthread_t tid = pthread_self(); // 获取线程自身的 ID
    printf("Thread id = %d, tid = %lu, a = %d\n", t_arg->id, (unsigned long)tid, a++);
    pthread_exit(NULL);
}

int main() {
    a = 0;
    const int THREAD_COUNT = 5;
    pthread_t threads[THREAD_COUNT];
    ThreadArg args[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; ++i) {
        args[i].id = i;
        pthread_create(&threads[i], NULL, thread_func, &args[i]);
    }

    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

/* Sample Output:
Thread id = 0, tid = 140572740683328, a = 0
Thread id = 1, tid = 140572732229184, a = 1
Thread id = 3, tid = 140572715320896, a = 2
Thread id = 4, tid = 140572706866752, a = 3
Thread id = 2, tid = 140572723775040, a = 4
*/
```

可以看到 `id` 并没有按照顺序被打印出来，这是因为程序是**并发**的。

!!! info "使用结构体进行传参是一个较通用的实践，适合传递不同类型参数"

虚拟化使得进程认为**世界上只有自己**，即除了系统调用，程序的行为是确定性的，在同一初始状态每次重新运行都有相同的结果。

但是并发打破了这一点，并发程序每次 *non-deterministically* 选择一个线程执行，这种非确定性程序使得程序理解起来相当困难。

!!! danger "并发这种**非确定性**带来的负面影响和数据库中的并发类似，此处也不再展开"

除此之外，编译器优化的一个重要前提是程序是确定性的，反过来这也意味着我们在并发编程时不仅要注意多线程乱序带来的影响，还要思考编译器对源代码的优化。

要控制编译器优化的行为，我们可以插入不可优化的代码块或者标记变量为不可优化：

```c
/// Method 1
while (!flag)
{
	asm volatile ("" ::: "memory"); // 声明这段代码会改写内存
}

/// Method 2 
int volatile flag;
while (!flag);
```

![[UMA.png]]

## 并发控制

### 互斥

我们希望引入新机制，阻止并发程序的并发，以维持并发程序的正确性。一种一般的思想是为某段我们不想它并发的*代码块*施加互斥锁标记，从而阻止并发的发生：

```c
lock();
sum++;
unlock();
```

历史上，对于互斥有很多不同的尝试，在本课程从以下三点讲述。

#### Method1: 共享内存

!!! danger "实际上，这是一个不太正确的尝试方向，它做了并发编程中不正确的假设"
	- 假设 Load/Store 指令是瞬间完成且生效的
	- 假设指令都按照书写顺序执行

1981 年 Gray Peterson 在软件层面上提出了一个互斥算法，它只能用于解决两个进程之间的互斥问题，保证两个进程不会同时进入临界区。

!!! info "临界区：可以是共享内存"

对于两个进程 `p0`，`p1` 和一个临界区 `M`，算法定义了三个控制变量：

```c
bool flag[2]; // flag[0] for p0, flag[1] for p1
int turn;     // for M
```

当一个进程想要访问临界区时，需要先设置自己的 `flag`，然后将临界区的 `turn` 设置为**另一个进程**的值。设置完后，该进程进入观察模式，如果另一个进程的 `flag` 为否，或者另一个进程 `flag` 为是但是临界区的 `turn` 上是自己，则允许进入并独占临界区。

```c
/// p0 action
// entry
flag[0] = true;
turn = 1;
while (flag[1] && turn == 1);

/* do sth critical */
...

// exit
flag[0] = false;
```

这样，当两个进程同时想要访问临界区，由于对于 `turn` 的设置有先后关系，后设置 `turn` 的进程一定会把 `turn` 设置成先访问的进程的值，这样先发出访问操作的进程就可以先进入临界区，从而实现互斥。

!!! warning "注意 entry 部分 `flag` 和 `turn` 的设置顺序不能改变"

#### Method2: 原子指令

Peterson 算法试图在软件层面 Load/Store 上实现互斥是没有意义的，我们可以向硬件层面寻求帮助，这就是我们所说的原子指令。

!!! tip "体系结构中有讲，在 RISC-V 中表现为 `LR/SC` 和自旋锁等"

![[Spinlock.png]]

但是随着线程数的增加，性能会断崖式的下降，这是 Spinlock 的不足。

#### Method3: 系统调用

上述方法的自旋锁存在两点性能问题：

- 除了获得锁的进程，其它处理器的进程都在空转等待锁释放
- 应用程序不能关中断

这个问题线程无法自己解决，我们需要将锁的实现放到操作系统的 API 中。而我们的一些库又对这些 API 做了一些封装，从而得到了现在我们使用的 Mutex：

```c
#include <pthread.h>

pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);

pthread_mutex_lock(&lock);
pthread_mutex_unlock(&lock);
```

总的来说，单论性能，Atomic > Mutex > Spin。

### 同步

#### 条件变量

Lock 解决了并发的**原子性**问题，但是没有给我们**确定性**。我们使用**同步**控制事件发生的先后顺序。

事件的顺序相当于一个 Precedence Graph，而同一个线程内的事件天然存在 happens-before 关系，非同一个线程的事件则需要我们额外设置。

!!! note "没有 happens-before 的代码就是并发的，在多处理器可以并行执行"

如果我们手动为线程构造一个循环，当前一个事件的完成标志不为 `True` 时则循环等待，则又陷入了自旋锁空转浪费 CPU 性能的问题，因此这个操作也要由操作系统的 API 完成。

过去的操作系统设计的不那么复杂：

- 当条件不满足时等待，让出 CPU
	- `wait`：直接等待睡眠
- 条件满足时由前一个进程发出信号继续
	- `signal/broadcast`：唤醒所有等待的进程

C++11 中引入了条件变量 `condition_variable`，可以实现类似 `wait_until(condition)` 的功能，它使用 lambda 表达式作为条件参数：

```c++
std::mutex mtx;
std::condition_variable cv;

void fn() {
	std::unique_lock lk(mtx);

	cv.wait(lk,
		[]{ return can_proceed; }
	);

	// 此时一定有 can_proceed = True 存在
	// do sth here

	cv.notify_all(); // 唤醒等待进程
	lk.unlock();
}
```

接下来我们以一个经典同步问题 **生产者-消费者问题** 作为例子讲解：

Producer 和 Consumer 共享一个缓冲区。如果缓冲区有空位，则 Producer 放入数据，否则等待；如果缓冲区有数据，则 Consumer 取出数据，否则等待。同一个 object 的生产必须先于消费。

```c
void produce(Object obj);
Object consume();
```

此处我们简化问题，假设 `produce` 打印左括号，`consume` 打印右括号，只需要打印的括号序列正确即可：

```c
void produce() { printf("("); }
void consume() { printf(")"); }
```

!!! warning "此外，缓冲区大小 `n` 可能有限，括号嵌套深度不能超过 `n`"

首先要明确执行条件：

- 什么时候可以 produce？
	- 缓冲区有空位
- 什么时候可以 consume？
	- 缓冲区非空

```c
void produce() {
    mutex_lock(🔒);
    while (!(depth < n)) {
	    // 由于唤醒是广播的，所以这里要放入 while 中确保条件满足再进行
        cond_wait(&cv, 🔒); // 等待 cv 唤醒
    }

    assert(depth < n);
    depth++;
    printf("("); // put object to queue

    cond_broadcast(&cv); // 广播唤醒所有 wait(cv)
    mutex_unlock(🔒);
}
```

!!! warning "如果使用 `cond_signal(&cv)`，则只会随机唤醒一个 `wait` 可能导致程序错误"

总而言之，为了分析一个程序能否使用并行计算，我们总是构造一个该程序各个计算任务的有向无环图 Dependency Graph。如果节点“独立计算时间”够长，算法就是可以高效并行的。

例如大规模动态规划问题大概率是可以高效并行的，因为动态规划某点 $(i,j)$ 的依赖关系一般是依赖于 $(i-1,j-1), (i-1,j), (i,j-1)$ 的。

!!! tip "单个计算步骤运算量较小的话，可以将一组计算合并作为一个计算节点，以减小锁操作时间占比"

#### 信号量

不同于条件变量，这里依靠锁的 Acquire-Release 先后顺序实现计算图，基本同数据库中的锁作用相同。

与互斥锁相比，信号量扩展了资源“数量”这一维度，适用于**计数资源同步场景**，其核心思想是**资源可用性计数控制与线程阻塞调度机制结合**。两个基本操作：

- `P()`（Proberen，尝试，也称为 `wait()` 或 `down()`）：将信号量值减1。如果信号量值已为0，则阻塞直到有资源可用。
- `V()`（Verhogen，增加，也称为 `signal()` 或 `up()`）：将信号量值加1。如果有线程阻塞在该信号量上，则唤醒一个。

| 特性         | 互斥锁 (Mutex) | 信号量 (Semaphore)           |
| ---------- | ----------- | ------------------------- |
| **计数值**    | 只有 0 或 1    | 任意非负整数                    |
| **用途**     | 实现互斥        | 实现互斥和同步                   |
| **拥有者**    | 有线程所有权      | 没有所有权概念                   |
| **操作名**    | lock/unlock | P()/V() 或 wait()/signal() |
| **可实现的同步** | 不可直接表示线程同步  | 可用于线程间的执行顺序控制（例如先后执行）     |

总之，信号量的两种典型应用分别为：实现一次临时的 happens-before 关系；管理计数型资源。

!!! info "计数型资源：停车场只有 n 个位置；餐厅只有 n 个桌子..."

## 并发 BUG

1. 数据竞争
2. 死锁
3. 原子性和顺序违反

> [17 - 并发 Bugs 和应对 [2025 南京大学操作系统原理]](https://www.bilibili.com/video/BV1cR5QzBErQ?spm_id_from=333.788.videopod.sections&vd_source=b856d3bdc44aa3fa77a923e6ba628c0f) 有闲情再看

## 不同应用场景下的并发编程

用条件变量或者信号量会在工程代码中引入众多干扰代码。一些编程语言提供了功能受限的语法，能够避免很多问题，其本质上也是描述计算图。

### 高性能计算

高性能计算源自数值密集型科学计算任务。物理世界具有“空间局部性”，因此模拟物理世界可以很自然地运用到并行计算。

HPC 主流有两个库：

- **MPI, message passing libraries**
- **OpenMP**

```c
#ifdef OMP
#include <omp.h>
#endif

#pragma omp parallel num_threads(128)
for (int i; i < 1024; i++) {
}
```

!!! note "使用 `omp.h`，编译时要加上 `-fopenmp` 编译选项，不然会忽视 `#pragma`"

### WEB

主要是 Javascript 发展至今各种库使用各种不同的语法设置计算图。现代前端代表作品：

- Angular、React、Vue
- Express.js、Next.js
- Bootstrap、Tailwindcss
- Electron
- asm.js（包括 WebAssembly）

### 数据中心

数据中心程序以海量分布式数据存储为中心，支持实时的小数据处理以及离线的大数据处理，要求高吞吐、低延迟的多副本分布式存储与计算。

- **多线程/多进程并发模型**：用于节点内高并发处理（如 Java 的线程池、Go 的 goroutine）
- **事件驱动模型（Reactor 模式）**：如 Nginx、Netty 等用于高并发 I/O 请求的组件
- **消息队列异步通信**：如 Kafka、RabbitMQ 用于服务解耦与异步处理
- **数据流模型**：如 Apache Beam 定义统一的数据处理语义，支持有状态流处理并发
- **MapReduce 及其演进**：通过对数据切分、任务划分，天然支持并行化执行


### GPU

GPU 属于 Single Instruction Multiple Threads。

```c
__device__ int *map;

// row: [1 -> 1080]
// col: [1 -> 1920]
void kernel(int row, int col) {
	color = ...; // 任意 C 代码
	map[row * 1920 + col] = color;
}
```

对于一般 CPU 的想法，我们使用两层循环遍历行列，每次都调用 kernel：

```c
for (int row = 1; row <= 1080; row++)
	for (int col = 1; col <= 1920; col++)
		kernel(row, col);
```

而对于现代 GPU，我们可以直接启动 1920\*1080 个线程，每个线程都调用 kernel。

