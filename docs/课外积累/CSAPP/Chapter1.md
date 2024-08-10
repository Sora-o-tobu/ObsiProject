# Chapter 1. A Tour of Computer System

## Part 1. 程序结构与执行

本节主要讲述了一个简单的 Hello World 程序的生命进程

```c
#include <stdio.h>
int main()
{
	printf("Hello World\n");
	return 0;
}

// Save as hello.c
```

使用编译器 `gcc` 对其进行编译，并生成可执行文件 `hello` ： `gcc hello.c -o hello` 

实际上，编译过程可以分为 ==预处理、编译、汇编、链接== 四个阶段
![[编译的四个阶段.png]]

<font style="font-weight: 1000;font-size: 20px" color="orange">第一阶段</font> : 预处理器会根据以 `#` 开头的代码修改原始程序，例如上述 `hello.c` 中引入了头文件 `stdio.h` 

通过 `gcc -E hello.c -o hello.i` 来对 `hello.c` 进行预处理，能够看到生成的文件中仅仅对 `#include <stdio.h>` 进行了替换，而将 `stdio.h` 库的内容全部写入，这是一个 ascii 码的中间文件

!!! info "CPP"
	C预处理器 (c preprocessor) 也可以完成对c程序的预处理 `cpp main.c -o main.i`

<font style="font-weight: 1000;font-size: 20px" color="orange">第二阶段</font> : 编译器将 `hello.i` 文件翻译成 `hello.s` 文件，包括词法分析、语法分析、语义分析、中间代码生成与优化等一系列的中间操作

通过 `gcc -S hello.i -o hello.s` (或者直接 `gcc -S hello.c -o hello.s` 也是可行的) 来对 `hello.i` 进行编译，此处截取部分编译生成的汇编程序代码，不详细讲解，此事在 《编译原理》 中亦有记载

```c
        .file   "hello.c"
        .text
        .section        .rodata
.LC0:
        .string "Hello World"
        .text
        .globl  main
        .type   main, @function
main:
.LFB0:
        .cfi_startproc
        endbr64
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp
        .cfi_def_cfa_register 6
        leaq    .LC0(%rip), %rax
        movq    %rax, %rdi
```

!!! info "cc"
	同理，也可以使用C编译器 (c compiler) 来代替 `gcc` ： `cc -S main.i -o main.s`

<font style="font-weight: 1000;font-size: 20px" color="orange">第三阶段</font> : 汇编器根据指令集将汇编程序 `hello.s` 翻译成机器指令，并把这一系列的机器指令按照固定的规则进行打包，得到可重定位目标文件 `hello.o` 

通过 `as hello.s -o hello.o` 将汇编代码翻译为目标文件，不过此时生成的文件是一个 **01 二进制文件** ，我们并不能直观了解其功能，不过可以使用 `objdump -d hello.o` 指令输出其机器码形式：

```c
❯ objdump -d hello.o

hello.o：     文件格式 elf64-x86-64

Disassembly of section .text:

0000000000000000 <main>:
   0:   f3 0f 1e fa             endbr64
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   48 8d 05 00 00 00 00    lea    0x0(%rip),%rax        # f <main+0xf>
   f:   48 89 c7                mov    %rax,%rdi
  12:   e8 00 00 00 00          call   17 <main+0x17>
  17:   b8 00 00 00 00          mov    $0x0,%eax
  1c:   5d                      pop    %rbp
  1d:   c3                      ret
```

<font style="font-weight: 1000;font-size: 20px" color="orange">第四阶段</font> : 上述生成的二进制文件 `hello.o` 仍然不能执行，还需要经过链接器链接。

例如，此例中调用了标准C库的 `printf` 函数，其位于提前编译好的目标文件 `printf.o` 文件中，链接器(ld)负责把 `hello.o` 和 `printf.o` 按照一定规则进行合并，

!!! note "这也是为什么 `hello.o` 被称为 **可重定位文件** "

链接分为静态链接和动态链接，一般编译器都会选择使用动态链接生成可执行文件

- 静态链接
	- 把所有需要的库全部复制一份到调用它的ELF文件中
	- 缺点在于，如果你写了100份hello.c，那么就复制了100份printf的实现函数，空间需求大
	- 优点为，不管你电脑上有没有stdio.h的环境，程序都可以执行；以及跑的速度更快
- 动态链接
	- 跟静态链接相反，仅仅将程序链接到所用的库上

```c
# 通过 file 命令可以查看可执行文件是动态链接还是静态链接
❯ file hello
hello: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=07ab739e124b9c0d60132c4c40eb3fe7dfbe60a8, for GNU/Linux 3.2.0, not stripped

# 通过 ldd 命令可以查看动态链接文件所链接的东西
❯ ldd hello
        linux-vdso.so.1 (0x00007ffc43376000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x000074231f400000)
        /lib64/ld-linux-x86-64.so.2 (0x000074231f7f3000)
```

最终，我们生成了一个可以被加载到内存中的可执行文件 `hello`

!!! quote "为什么要理解编译系统是如何工作的"
	- 理解编译系统可以优化程序的性能
		- Switch 是否比 if-else 更好？while 和 for 循环哪个更高效？
	- 帮助理解链接过程中出现的错误
		- 一些奇奇怪怪的错误往往与链接器有关
	- 避免安全漏洞
		- 如 StackOverflow

## Part 2. 在系统上运行程序

在 `SHELL` 上输入如下指令即可运行可执行文件：

```bash
❯ ./hello
Hello World
```

在此之前，需要先简单了解一下计算机系统的硬件组成，以帮助理解 `hello` 程序的运行过程

![[计算机系统的硬件组成.png]]

- 中央处理单元 (Central Processing Unit , CPU) ，也称处理器
	- Program Counter (PC) 是一个大小为一个字(对于32bit系统，1 word=4 Bytes)的存储区域，其功能是存储下一条要执行的指令（在机器语言中）的内存地址
	- Register File 是 CPU 内部的一个存储设备，由一些单字节长度的寄存器构成，每个寄存器都有自己的唯一名字
	- 算数逻辑计算单元 (Arithmatic/Logic Unit , ALU) 顾名思义，且在数逻课程中详细讲过了，不懂的自行了解
- 处理器在执行程序时，程序指令及数据主要存放在内存中。从物理上讲，内存是由随机动态存储器芯片组成；从逻辑上讲，内存可以看成一个从零开始的大数组，每个字节都有对应的地址
- 内存和处理器之间通过总线 (I/O Bridge) 来进行数据的传递，总线贯穿了整个计算机系统，它负责把信息从一个部件传递给另一个部件。通常总线被设计成传送固定长度 *(1 word for example)* 的字节块

---

简单介绍完计算机系统的组成后，就可以讲讲 `./hello` 运行过程中到底发生了什么：

- `SHELL` 程序将键盘输入的 `hello` 字符逐一读入 CPU 的寄存器，当我们按下回车键完成命令输入时，CPU 会将 `hello` 这一字符串放入内存中
- `SHELL` 知道命令输入完成后，会执行一系列指令 (此处略讲) 来加载可执行文件 `hello` ，这些指令将 `hello` 中的数据和代码从磁盘 (Disk) 复制到内存中 (运用了 DMA 技术，数据可以不经过处理器从磁盘直接到内存)
- 当可执行文件的数据和代码被加载到内存中，处理器就开始执行 main 函数中的代码。
	- CPU 将 `Hello World\n` 这一字符串从内存复制到寄存器文件，然后再从寄存器文件复制到显示设备，最终显示在屏幕上 (大概过程)

!!! danger "上述实现过程省略了很多细节！！！"

通常，大容量存储设备的存取速度要比小容量的慢

| Caches Matter         |            |
| --------------------- | ---------- |
| Register File (寄存器文件) | 100-1000 B |
| L1 Cache (高速缓存)       | 10-100 KB  |
| L2 Cache (高速缓存)       | 0.1-10 MB  |
| L3 Cache (高速缓存)       | 10-100 MB  |
| Main Memory (内存)      | 1-100 GB   |
| Disk (磁盘)             | 1-1000 TB  |

处理器从寄存器文件读取数据比从内存读取差不多要快100倍，随着半导体技术的发展，它们之间的差距还在持续增大。针对这个差距，系统设计人员在寄存器文件和内存中引入了 ==高速缓存 (Cache)== ，一般较新的处理器都有三级高速缓存

!!! note "高速缓存为 **SRAM** ；内存为 **DRAM**"

## Part 3. 进程间的交互与通信

![[操作系统是中间层.png]]

操作系统是应用程序和硬件之间的中间层，所有应用程序对硬件的控制必须通过操作系统来实现

为了实现上述的功能，操作系统引入了几个抽象的概念：

- 文件是对IO设备的抽象
- 虚拟内存是对内存和磁盘IO的抽象
- 进程是对处理器、内存和IO设备的抽象

![[操作系统抽象.png]]

### 进程简述

接下来，仍然使用 `hello` 程序来解释一下进程：

- 最开始，只有 shell 进程在运行，等待输入指令
- 当我们通过 shell 进程加载 hello 进程时，shell 通过系统调用 (System call) 来执行我们的请求，系统调用会将控制权从 shell 进程传递给操作系统，操作系统保存 shell 进程的上下文，然后创建一个新的 hello 进程及其上下文，并将控制权转交给 hello 进程
- hello 进程执行完后，操作系统会恢复 shell 进程的上下文，并将控制权交给 shell 进程

!!! note "上下文指：进程运行过程中所需要的所有状态信息"

![[hello运行进程.png]]

在现代操作系统中，一个进程实际由多个线程组成，每个线程都运行在进程的上下文中，共享数据和代码。由于网络服务器对并行处理的需求，线程成为越来越重要的编程模型

### 虚拟内存简述

虚拟内存为每个进程提供一个假象，即每个进程都在独自占用整个内存空间。

每个进程看到的内存都是一样的，称之为 虚拟地址空间

![[栈与堆.png]]

学过 C语言 的大概懂里面的 Stack 和 Heap，此处不细讲

### 文件简述

!!! quote "Linux 系统的思想是：一切皆为文件"

所有的IO设备，包括键盘、磁盘、显示器，甚至网络，都可以看作文件。系统中所有输入输出都可以同个读写文件完成

Chapter 10 将详细讲述 Unix 10 ，此处也不细讲