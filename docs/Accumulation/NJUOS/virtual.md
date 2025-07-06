
# 虚拟化

!!! quote "虚拟化"
	把物理计算机抽象成虚拟计算机，每个程序都好像独占计算机运行。

## 程序与进程

程序是状态机的**静态描述**，描述了所有可能的程序状态。程序(动态)运行起来，就成为**进程**。

### 进程管理 API

!!! quote "操作系统 API：非必要不提供，避免代码臃肿"
	- **进程管理:** fork, execve, exit, waitpid
	- **内存管理:** mmap
	- **文件(对象)管理:** open, read, write, dup, close, pipe

既然如此，我们就需要操作系统提供进程管理的 API。

一个直观的想法是使用 `spawn(path, argv)` 创建进程(状态机)，用 `_exit()` 销毁进程。这个想法在 Windows 中得到应用，分别对应 API 为 `CreateProcess()` 和 `TerminateProcess()`。

而在 Unix 中，它使用**复制**和**复位**来代替创建和销毁：

<font style="font-weight: 1000;font-size: 20px" color="orange">pid_t fork(void);</font>

完整复制一份状态机，包括内存、寄存器

- 执行 `fork` 的进程返回子进程的进程号，它们是“父子”关系
- 子进程则返回 0；如果返回 -1 则说明复制失败，通过 `errno` 返回错误原因

```c
int pid = fork();
if (pid < 0) {
	// 失败处理
}

// 在两个进程中同时执行
// 根据返回值的不同在父子进程中做不同行为
if (pid == 0) {
	// new
} else {
	// old
}
```

进程创建的关系形成了进程树，子进程结束时需要发送 `SIGCHILD` 信号给父进程。但是要是父进程此时已经终止了怎么办？UNIX 的托孤机制会将子进程的 `ppid` 设置为 1，并且 1 号进程收到该孤儿进程的信号不会做任何事。

!!! tip "实际上，OS 也可能会其托孤至 `systemd --usr` 上，这个进程的 `pid` 不为 1"

!!! note "addition：测试框架"
	- jyy 的测试框架中，父进程调用一个完全复制自己状态的子进程来 `run_test`
	- 父进程等待子进程结束 `waitpid`，根据子进程返回的结果判断测试是否通过

<font style="font-weight: 1000;font-size: 20px" color="orange">int execve(const char * filename, char * const argv[], char * const envp[]);</font>

将当前进程**重置**成一个可执行文件描述状态机的初始状态。

- `filename` 可执行文件的文件名
- `argv[]` 程序的参数
- `envp[]` 环境变量

!!! tip "只复位了用户可见的状态，OS 内部维护的状态没变，例如进程号、目录、打开的文件等"

`execve` 是唯一能够“**执行程序**”的系统调用，因此当我们执行任何可执行文件时，第一个调用的肯定时 `execve`，我们可以用 `strace` 命令进行验证：

```bash
➜ strace ./main &| less
Missing filename ("less --help" for help)
execve("./main", ["./main"], 0x7ffff1b1c230 /* 38 vars */) = 0 
...
```

在 C 程序中执行 `execve` 后，如果该系统调用没有 error，则后面的代码都不会被执行，因为这个程序的状态机直接被重置了。例如我们对 gcc 进行 `strace`，观察这一部分代码：

```bash
[pid 28369] execve("/usr/local/sbin/as", ["as", "--64", ...
[pid 28369] execve("/usr/local/bin/as", ["as", "--64", ...
[pid 28369] execve("/usr/sbin/as", ["as", "--64", ...
[pid 28369] execve("/usr/bin/as", ["as", "--64", ...
```

这里在环境变量 `$PATH` 中寻找汇编器 `as`，并遍历调用 `execve`。从上到下当有一个 `execve` 可以正确执行后，下面的指令就不会再被执行。

<font style="font-weight: 1000;font-size: 20px" color="orange">void _exit(int status);</font>

立即摧毁状态机，允许有一个返回值，可被父进程获取。

通过这三个系统调用就实现了整个 UNIX 世界中的所有进程调用。

```c
int pid = fork();
if (pid == -1) { // 错误
    perror("fork"); goto fail;
} else if (pid == 0) { // 子进程
    execve(...);
    perror("execve"); exit(EXIT_FAILURE);
} else { // 父进程
    ...
    int status;
    waitpid(pid, &status, 0); // testkit.c
}
```

### 进程地址空间

进程 `execve` 后的初始状态在 ABI 中得到规定，但是不同操作系统的规定并不相同，例如你可以查看 [System-V ABI](https://jyywiki.cn/OS/manuals/sysv-abi.pdf) 的 3.4 节 *Process Initializaiton*。进程的地址空间中，大部分段都是不可读的，访问它们会出现 `segment fault`。

!!! info "Application Binary Interface: 二进制文件和底层系统的接口"

进程的初始状态只有 ELF 文件中声明的内存和一些操作系统分配的内存，其它任何指针的访问都是非法的。但是进程的地址空间不是固定的，一定存在一个**系统调用**可以改变进程的地址空间：

```c
// 映射
void *mmap(void *addr, size_t length, int prot, int flags,
           int fd, off_t offset);
int munmap(void *addr, size_t length);

// 修改映射权限
int mprotect(void *addr, size_t length, int prot);
```

- `addr`: 指定映射起始地址；对于 `mmap`，通常设为 `NULL`，由系统自动选择合适位置
- `prot`: 指定内存保护方式
	- `PROT_READ` 可读
	- `PROT_WRITE` 可写
- `flags`: 控制映射行为
	- `MAP_SHARED` 共享映射
	- `MAP_PRIVATE` 私有映射
	- `MAP_ANONYMOUS` 匿名申请内存
- `fd`: 把文件 “搬到” 进程地址空间中 (例子：加载器)
- `offset`: 文件中的偏移量，表示从文件中的哪个位置开始映射

!!! note "可以用 `pmap` 命令查看进程的地址空间，当然也可以看 `/proc/[pid]/map`"

## 操作系统对象

### 文件

!!! quote "Everything is a File"
	```bash
	➜ cat /dev/urandom | head -c 16 | hexdump
	0000000 d8e4 1e43 4a06 bce4 e7ec 64a9 557f 0ffa
	0000010
	```

**文件**是带有“名字”的数据对象，广义上可以包含字节流(终端、`urandom`等)和字节序列(普通文件)；而**文件描述符**是指向操作系统对象的指针，通过指针可以访问一切。

在 Unix 系统中，操作系统对象（如文件、设备、管道等）的访问都需要通过文件描述符（指针）。常见的文件描述符操作包括：

- `open()` - 打开/创建对象，会在地址空间中分配一个未分配的最小文件描述符
	- `p = malloc(sizeof(FileDescriptor));`
- `close()` - 关闭对象
	- `delete(p);`
- `read()/write()` - 读写对象，属于解引用
	- `*(p.data++);`
- `lseek()` - 改变读写位置
	- `p.data += offset;`
- `dup()` - 复制文件描述符
	- `q = p`

!!! info "文件描述符 0,1,2 初始默认占用，它们都指向同一个对象，一般为终端"

文件描述符是进程状态的一部分，那么我们在执行 `fork` 复制进程的时候对于该块地址空间会发生什么？

对于字节流，我们可以没有直接将指针的值复制到子进程中，相当于*浅拷贝*；对于字节序列，文件描述符指向的是该文件的一个 `offset`，因此我们复制的指针值实际上指向的为该 `offset`，这个操作和 `dup()` 相同。

可以尝试以下代码：

```bash
➜  ~ wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/fd/"
➜  ~ cd fd && make
➜  fd ./fd-offset   
Content of sample.txt: AB
Content of sample.txt after fork: DC
```

Windows 系统中的文件描述符比 Unix 更像一个指针，称为 **Handle(句柄)**。和 Unix 相反的是，Windows 中默认 handle 是不继承的，这符合面向工程的最小权限原则。

!!! abstract "Linux 操作系统一般遵循 **Filesystem Hierarchy Standard** 规范(FHS)"
	![[LinuxFHS.png]]

除了上述用来访问系统中对象的系统调用，UNIX 还提供了 `mount`, `pipe`, `mkfifo` 等能够“创建”操作系统对象的系统调用。

UNIX 管道(pipe)是一个特殊的流式文件，它常用作进程间通信机制，允许数据在不同进程之间单向流动。其中一个进程将数据写入管道的一端，而另一个进程从管道另一端读取数据。

```c
// 匿名管道
int pipe(int pipefd[2]);
// pipefd[0] 是读口，pipefd[1] 是写口
// 匿名管道在两个口均无人引用时会被操作系统自动回收
```

那么我们平常在终端使用管道 `|` 传递输出时，OS 的机制会创建一个管道对象，将前一个程序指向终端(`stdout`)的文件描述符转移到管道的写口，后一个程序指向终端(`stdin`)的文件描述符转移到管道的读口。

### UNIX Shell

**Session(会话)** 是一组进程的集合，通常由一个登录会话（login, ssh 等）创建。每个 Session 包含一个或多个 **Process Group(进程组)**，并关联到一个 **Controlling Terminal(控制终端)**。

由 `fork` 命令创建出来的进程属于同一个进程组，例如管道命令等。

**Controlling Terminal** 记录当前正在前台的 Process Group ID。用户在 Shell 中输入 `Ctrl+C`(中断)、`Ctrl+Z`(停止)、`Ctrl+\`(退出) 等字符时实际上是发送给 OS，由 OS 向位于前台的进程组中的**所有进程**发送对应 signal：

![[ShellSession.png]]

!!! note "`Ctrl+Z` 相当于最小化，可以通过 `jobs` 查看暂停的程序；通过 `fg` 指令继续执行暂停程序"

## 可执行文件

### LIBC

在操作系统 API 上，为了服务应用程序，有必要设计一套“好用”的库函数。虽然 `libc` 今天已经谈不上“好用”，但它成就了 C 语言今天的地位，以及以 ISO 标准的形式支撑了操作系统生态上的万千应用。

`libc` 大部分代码可以通过 C 语言本身实现，少部分需要一些底层支持，例如体系结构相关的内联汇编语句。

### 链接和加载

!!! danger "链接、虚拟内存部分在别的课学过了，此处不再记录"


### 应用生态

操作系统仅有两个机制：**初始状态+系统调用**

UNIX 启动时初始状态：

<1> `initramfs` 中的对象，包括解压并挂载**根文件系统** -> <2> `/dev/console`，是用于 IO 的关键设备 -> <3> 查找并加载 `init` 脚本，此时才会挂载**真实根文件系统**，例如 `/dev/sda1`，通过 `pivot_root` 或 `switch_root` 等系统调用切换到真实根文件系统，然后再执行 `/sbin/init` 全面初始化。

!!! note "现代 Linux 执行 `\sbin\init` 会定向到 `systemd`"

```bash
Bootloader -> Kernel ->
initramfs 解压 -> 挂载为 `/` ->
/dev/console 可用 ->
执行 `/init` ->
准备环境、挂载真实根文件系统 ->
切换到真实根 ->
执行 `/sbin/init` -> 用户空间全面启动

```

