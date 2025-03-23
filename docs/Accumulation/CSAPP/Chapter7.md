# Chapter 7. Linking

!!! quote "链接是将各种代码和数据收集并组合成一个文件的过程"

## 编译器驱动程序示例

构造大型程序时经常有缺少库文件或者库文件的版本不兼容而导致的链接错误，解决这类问题需要理解链接器是如何使用库文件来解析引用的。除此之外，学习链接还可以帮助理解编程语言中的作用域规则是如何实现的。

```c
 > nano main.c
 1 int sum(int *a, int n);
 2
 3 int array[2] = {1,2};
 4
 5 int main(){
 6         int val = sum(array,2);
 7         return val;
 8 }
 > nano sum.c
 1 int sum(int *a, int n){
 2         int i,s = 0;
 3         for(i=0;i<n;i++)
 4                 s+=a[i];
 5         return s;
 6 }
 > gcc -Og main.c sum.c -o prog
# 使用此指令得到可执行程序 prog
```

关于该程序的单独编译过程，此处不再复制，请查看 [Chapter 1. 计算机系统漫游](https://www.nimisora.top/%E8%AF%BE%E5%A4%96%E7%A7%AF%E7%B4%AF/CSAPP/Chapter1/)

需要注意的是，当我们手动调用链接器来构造可执行程序时，除了需要用到汇编阶段生成的 `main.o` 和 `sum.o` 外，还需要 `crt1.o`,`crti.o`,`crtbeginT.o`,`crtend.o`,`crtn.o`，其中，`crt` 是 **c runtime** 的缩写

## 可重定位目标文件

接下来我们使用如下C语言程序来演示可重定位目标文件：

```c
#include<stdio.h>

int count = 10;
int value;

void func(int sum){
	printf("sum is: %d\n",sum);
}

int main(){
	static int a = 1;
	static int b = 0;
	int x = 1;
	func(a+b+x);
	return 0;
}
```

使用命令 `gcc -c main.c` 将这个源文件 `main.c` 翻译成可重定位目标文件 `main.o` ，其中编译选项 `-c` 表示只进行编译和汇编，不执行链接操作

每一个可重定位目标文件可以大致分为三部分，分别是 `ELF header`,不同的`Sections`,以及描述这些`section`信息的表(`Section header table`)

!!! info "ELF : Executable and Linkable Format(可执行可链接格式)"

我们可以使用 `readelf -h main.o` 来查看 `ELF header` 的具体内容，其中 `-h` 选项表示只显示 header 信息：

```
❯ readelf -h main.o
ELF 头：
  Magic：   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  类别:                              ELF64
  数据:                              2 补码，小端序 (little endian)
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI 版本:                          0
  类型:                              REL (可重定位文件)
  系统架构:                          Advanced Micro Devices X86-64
  版本:                              0x1
  入口点地址：               0x0
  程序头起点：          0 (bytes into file)
  Start of section headers:          992 (bytes into file)
  标志：             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         14
  Section header string table index: 13
```

首先分析 ELF Header 中最开始的十六位字节 **Magic** ：

- **Magic** 最开始的四个字符 `7f 45 4c 46` 是 `ELF` 文件的魔数，分别与 ASCII 码中的 DEL 控制符、字符 E 、字符 L 以及字符 F 对应
- 接下来的一个字节 `02` 用来表示 `ELF` 文件类型，`0x01` 表示 32 位，`0x02` 表示 64 位
- 再下一个字节表示字节序，其中 `0x01` 表示小端序，`0x02` 表示大端序
- 第七个字节表示 ELF 文件的版本号，通常都是 `0x01`
- 最后九个字节 ELF 的标准中没有定义，用 `0x00` 填充

除此之外，还需注意的有该 ELF 文件的 type 位 REL (Relocation File) ，除了 REL 外，还有可执行文件和共享文件两种类型；`Size of this header` 行给出了 Header 的长度，据此可知 sections 在该 ELF 文件的起始位置为 `0x40` ；`Start of section headers` 给出了 section headers 的起始地址

---

接着使用命令 `readelf -S main.o` 来查看 Section header (大写 S):

```
❯ readelf -S main.o
There are 14 section headers, starting at offset 0x3e0:

Section header：
  [Nr] Name              Type             Address            Offset
       Size              Entsize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       000000000000005f  0000000000000000  AX       0     0     1
  [ 2] .rela.text        RELA             0000000000000000  000002c0
       0000000000000078  0000000000000018   I      11     1     8
  [ 3] .data             PROGBITS         0000000000000000  000000a0
       0000000000000008  0000000000000000  WA       0     0     4
  [ 4] .bss              NOBITS           0000000000000000  000000a8
       0000000000000008  0000000000000000  WA       0     0     4
  [ 5] .rodata           PROGBITS         0000000000000000  000000a8
       000000000000000c  0000000000000000   A       0     0     1
  [ 6] .comment          PROGBITS         0000000000000000  000000b4
       0000000000000027  0000000000000001  MS       0     0     1
  [ 7] .note.GNU-stack   PROGBITS         0000000000000000  000000db
       0000000000000000  0000000000000000           0     0     1
  [ 8] .note.gnu.pr[...] NOTE             0000000000000000  000000e0
       0000000000000020  0000000000000000   A       0     0     8
  [ 9] .eh_frame         PROGBITS         0000000000000000  00000100
       0000000000000058  0000000000000000   A       0     0     8
  [10] .rela.eh_frame    RELA             0000000000000000  00000338
       0000000000000030  0000000000000018   I      11     9     8
  [11] .symtab           SYMTAB           0000000000000000  00000158
       0000000000000138  0000000000000018          12     8     8
  [12] .strtab           STRTAB           0000000000000000  00000290
       000000000000002d  0000000000000000           0     0     1
  [13] .shstrtab         STRTAB           0000000000000000  00000368
       0000000000000074  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), l (large), p (processor specific)
```

可以看到，除了 `[0]` ，其它每一个表项对应着一个 Section ，整个 ELF 文件共有 13 个 Section 。其中 `Offset` 表示每个 Section 的起始位置，`Size` 表示 Section 的大小

例如，`.text section` 起始位置为 `0x40` 紧跟在 `ELF header` 之后，section 中存放着已经编译好的机器代码，需要使用 `objdump` 转为汇编代码进行查看：

```
❯ objdump -s -d main.o

main.o：     文件格式 elf64-x86-64

Contents of section .text:
 0000 f30f1efa 554889e5 4883ec10 897dfc8b  ....UH..H....}..
 0010 45fc89c6 488d0500 00000048 89c7b800  E...H......H....
 0020 000000e8 00000000 90c9c3f3 0f1efa55  ...............U
 0030 4889e548 83ec10c7 45fc0100 00008b15  H..H....E.......
 0040 00000000 8b050000 000001c2 8b45fc01  .............E..
 0050 d089c7e8 00000000 b8000000 00c9c3    ...............
Contents of section .data:
 0000 0a000000 01000000                    ........
Contents of section .rodata:
 0000 73756d20 69733a20 25640a00           sum is: %d..
Contents of section .comment:
 0000 00474343 3a202855 62756e74 75203133  .GCC: (Ubuntu 13
 0010 2e322e30 2d323375 62756e74 75342920  .2.0-23ubuntu4)
 0020 31332e32 2e3000                      13.2.0.
Contents of section .note.gnu.property:
 0000 04000000 10000000 05000000 474e5500  ............GNU.
 0010 020000c0 04000000 03000000 00000000  ................
Contents of section .eh_frame:
 0000 14000000 00000000 017a5200 01781001  .........zR..x..
 0010 1b0c0708 90010000 1c000000 1c000000  ................
 0020 00000000 2b000000 00450e10 8602430d  ....+....E....C.
 0030 06620c07 08000000 1c000000 3c000000  .b..........<...
 0040 00000000 34000000 00450e10 8602430d  ....4....E....C.
 0050 066b0c07 08000000                    .k......

Disassembly of section .text:

0000000000000000 <func>:
   0:   f3 0f 1e fa             endbr64
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   48 83 ec 10             sub    $0x10,%rsp
   c:   89 7d fc                mov    %edi,-0x4(%rbp)
   f:   8b 45 fc                mov    -0x4(%rbp),%eax
  12:   89 c6                   mov    %eax,%esi
  14:   48 8d 05 00 00 00 00    lea    0x0(%rip),%rax        # 1b <func+0x1b>
  1b:   48 89 c7                mov    %rax,%rdi
  1e:   b8 00 00 00 00          mov    $0x0,%eax
  23:   e8 00 00 00 00          call   28 <func+0x28>
  28:   90                      nop
  29:   c9                      leave
  2a:   c3                      ret

000000000000002b <main>:
  2b:   f3 0f 1e fa             endbr64
  2f:   55                      push   %rbp
  30:   48 89 e5                mov    %rsp,%rbp
  33:   48 83 ec 10             sub    $0x10,%rsp
  37:   c7 45 fc 01 00 00 00    movl   $0x1,-0x4(%rbp)
  3e:   8b 15 00 00 00 00       mov    0x0(%rip),%edx        # 44 <main+0x19>
  44:   8b 05 00 00 00 00       mov    0x0(%rip),%eax        # 4a <main+0x1f>
  4a:   01 c2                   add    %eax,%edx
  4c:   8b 45 fc                mov    -0x4(%rbp),%eax
  4f:   01 d0                   add    %edx,%eax
  51:   89 c7                   mov    %eax,%edi
  53:   e8 00 00 00 00          call   58 <main+0x2d>
  58:   b8 00 00 00 00          mov    $0x0,%eax
  5d:   c9                      leave
  5e:   c3                      ret
```

!!! info "`.data` 用来存放已初始化且非零的全局变量和静态变量的值；除局部变量外其它变量都存放在 `.bss` 中"
	回到上面的 Section header ，可以看到 `.bss` 的初始位置与 `.rodata` 相同。实际上， `.bss` 并不占据实际空间，它仅仅是一个占位符，区分已初始化和未初始化的变量是为了节省空间，当程序运行时，会在内存中分配这些变量，并把初始值设为 0


| Section       | Introduction                          |
| ------------- | ------------------------------------- |
| **.comment**  | 存放编译器版本信息                             |
| **.symtab**   | Symbol Table ，符号表                     |
| **.rel.text** | Relocation Table ，重定位表                |
| **.debug**    | 调试信息                                  |
| **.line**     | 原始 C 程序行号和 **.text section** 中机器指令的映射 |
| **.strtab**   | String Table ，字符串表                    |

## 符号与符号表

要正确执行可重定位目标文件之间的链接，需要 `.symtab` 中的符号作为粘合才行。

使用命令 `readelf -s main.o` 查看符号表的内容 (小写 s) ：

```
❯ readelf -s main.o

Symbol table '.symtab' contains 13 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 .data
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 .bss
     5: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .rodata
     6: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    3 a.1
     7: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    4 b.0
     8: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    3 count
     9: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    4 value
    10: 0000000000000000    43 FUNC    GLOBAL DEFAULT    1 func
    11: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
    12: 000000000000002b    52 FUNC    GLOBAL DEFAULT    1 main
```

<font style="font-weight: 1000;font-size: 20px" color="MediumVioletRed">首先，我们先解析一下符号 `main` 和 `func` : </font>

- 在源程序中定义了函数 `main` 和函数 `func` ，所以符号表中二者的 Type 为 `FUNC` ；由于它们同时是全局可见的，因此这里的 `Binding` 字段也是 `GLOBAL` 
- `Ndx` 为 section 的索引值，可以查看 section header table 来确定
	- 例如，我们知道函数 `main` 和 `func` 位于 `.text` 中，所以它们的 `Ndx` 为 1
- `Value` 表示该符号相对于其对应 section 起始位置的偏移量
- `Size` 表示所占字节数，所以有 `main` 的 `Value` 等于 `func` 的 `Size`
- 字段 `Vis` 在 C 语言中并未使用，因此忽略

关于符号 `printf` ，虽然它也是一个函数，但是其在 `main.c` 中并未定义，所以它的 `Ndx` 是 `UND` **(undefine)** 

<font style="font-weight: 1000;font-size: 20px" color="MediumVioletRed">接下来解析全局变量对应的符号 `count` 和 `value` :</font>

- 其 `Type` 均为 `OBJECT` ，表明该符号是数据对象。变量和数组在符号表中的类型均为 `OBJECT`
- 二者 `Ndx` 不同，说明它们处于不同的 section 中，这一点在上一节也有说明
	- `count` 有初始化，位于 `.data` 中；`value` 没有初始化，位于 `.bss` 中

<font style="font-weight: 1000;font-size: 20px" color="MediumVioletRed">与之类似，对于符号 `a` 和 `b` :</font>

- 局部静态变量 `a` 进行了初始化，所以其 `Ndx` 为 3
- 局部静态变量 `b` 也进行了初始化，但是其初始化值为 0 ，所以其位于 `.bss` 中
- 二者的后缀 `a.1` , `b.0` 为名称修饰，作用为防止静态变量静态变量名字冲突

!!! note "有时符号表中有符号名为空，它们的符号名就是对应 section 的名称"

我们可以发现 `main` 函数中的局部变量 `x` 并没有出现在符号表中，这是因为局部变量在运行中在栈中管理，链接器不管理此类符号，所以局部变量不会出现在符号表中


每个可重定位目标文件都有符号表，包含该模块定义和引用的符号信息，在链接器的上下文中，有三种不同的符号：

- **(1)** 由该模块定义，同时能被其它模块引用的全局符号 **Global Symbols**
	- 例如，`main.c` 中的函数 `func` 以及全局变量 `count` 和 `value`
- **(2)** 被其它模块定义，同时被该模块引用的全局符号，名为外部符号 **External Symbols**
- **(3)** 只能被该模块定义和引用的局部符号
	- 区别局部符号和全局符号的关键在于 `static` 属性，带有 `static` 属性的函数和变量不能被其它模块引用，功能类似 Java 中的 `public` 和 `private`

### 符号解析

在编译时，编译器会定义每个 ==全局符号== 为强符号或弱符号，并把强弱信息隐含在符号表中：

- **Strong Symbols** ： 函数和已初始化的全局变量
- **Weak Symbols** ： 未初始化的全局变量

接下来，分三种情况讲解如何处理多重定义的符号名

<font style="font-weight: 1000;font-size: 20px" color="Gold">多个同名的强符号一起出现</font>

```c
foo1.c
int main(){
	return 0;
}
---
bar1.c
int main(){
	return 0;
}
```

两个源文件中都有函数名为 `main` 的函数，强符号 `main` 被定义了两次，在这种情况下，链接器会生成一条错误的信息

```c
foo2.c
int x=114514;
int main(){
	return 0;
}
---
bar1.c
int x=114514;
int func(){
	return 0;
}
```

同理，具有相同初始化的全局变量名也会产生类似错误

<font style="font-weight: 1000;font-size: 20px" color="Gold">一个强符号和多个同名弱符号一起出现</font>

```c
foo3.c
void f(void);
int x = 114514;
int main(){
	f();
	printf("x=%d\n",x);
	return 0;
}
---
bar3.c
int x;
void f(){
	x = 1919810;
}
```

由于 `bar3.c` 中的 `int x` 未初始化，为弱符号，那么对二者执行链接生成可执行文件的时候，链接器会选择在 `foo3.c` 中定义的强符号，不会提示错误或者警告（即，`bar3.c` 中`x=1919810` 的 `x` 指的是 `foo3.c` 中的全局变量 `x`）

输出 `x=1919810`

```c
foo4.c
void f(void);
int y = 1919810;
int x = 114514;
int main(){
	f();
	printf("x=%d y=%d\n",x,y);
	return 0;
}
---
bar4.c
double x;
void f(){
	x = -0.0;
//10 00 00 ... 00 00
}
```

由于 `double` 类型占 8 个字节，而 `int` 类型占 4 个字节，对于地址紧邻的全局变量（在堆中），`x` 和 `y` 的值都会改变

为了避免这种情况，可以在编译的时候添加 `-fno-common` 编译选项，当遇到多重定义的全局符号时，会触发一个错误

!!! note "使用 `-Werror` 选项，把所有警告变成错误"

<font style="font-weight: 1000;font-size: 20px" color="Gold">多个同名的弱符号一起出现</font>

**待办中...**

### 静态库

在 linux 系统中，静态库以 archive 的特殊文件格式存放在磁盘上。archive (存档)是一组可重定位目标文件的集合，文件后缀为 `.a`

其中，C语言的库函数大多都在 `libc.a` 的库中，可以使用 `objdump -t` 命令查看其内容

```
❯ objdump -t /usr/lib/x86_64-linux-gnu/libc.a > libc
❯ grep -n printf libc
5403:asprintf.o：     文件格式 elf64-x86-64
5438:dprintf.o：     文件格式 elf64-x86-64
5461:fprintf.o：     文件格式 elf64-x86-64
5735:printf.o：     文件格式 elf64-x86-64
6182:reg-printf.o：     文件格式 elf64-x86-64
6280:snprintf.o：     文件格式 elf64-x86-64
6291:sprintf.o：     文件格式 elf64-x86-64
6427:vfprintf.o：     文件格式 elf64-x86-64
6571:vfwprintf.o：     文件格式 elf64-x86-64
6714:vprintf.o：     文件格式 elf64-x86-64
6889:fxprintf.o：     文件格式 elf64-x86-64
7505:iovsprintf.o：     文件格式 elf64-x86-64
7841:fwprintf.o：     文件格式 elf64-x86-64
7852:swprintf.o：     文件格式 elf64-x86-64
7863:vwprintf.o：     文件格式 elf64-x86-64
7874:wprintf.o：     文件格式 elf64-x86-64
7920:vswprintf.o：     文件格式 elf64-x86-64
8381:vasprintf.o：     文件格式 elf64-x86-64
8401:iovdprintf.o：     文件格式 elf64-x86-64
8429:vsnprintf.o：     文件格式 elf64-x86-64
8446:obprintf.o：     文件格式 elf64-x86-64
33275:dl-printf.o：     文件格式 elf64-x86-64
```

由于内容过多，我们在后面加上 `> libc` 将其输出到当前目录的 `libc` 文件下，并使用 `grep` 命令查找 `printf` ，可以看到 `printf` 位于 5735 行

打开这个文件，找到对应行，可以看到 `printf` 定义在 `printf.o` 中：

```
 5735 printf.o：     文件格式 elf64-x86-64
 5736
 5737 SYMBOL TABLE:
 5738 0000000000000000 l    d  .text  0000000000000000 .text
 5739 0000000000000000 g     F .text  00000000000000c6 __printf
 5740 0000000000000000         *UND*  0000000000000000 stdout
 5741 0000000000000000         *UND*  0000000000000000 .hidden __vfprintf_internal
 5742 0000000000000000         *UND*  0000000000000000 __stack_chk_fail
 5743 0000000000000000 g     F .text  00000000000000c6 _IO_printf
 5744 0000000000000000 g     F .text  00000000000000c6 printf
```

!!! tip "使用 `ar -x /usr/lib/x86_64-linux-gnu/libc.a` 将 `libc.a` 的所有目标文件解压"

<font style="font-weight: 1000;font-size: 20px" color="SpringGreen">接下来通过一个简单的例子演示构造静态库的过程</font>


```c
addvec.c:
 1 int addcnt = 0;
 2 void addvec(int *x,int *y,int *z,int n){
 3         int i;
 4         addcnt++;
 5         for(i=0;i<n;i++)
 6                 z[i] = x[i] + y[i];
 7 }
 ---
 multvec.c:
  1 int multcnt = 0;
 2 void multvec(int *x,int *y,int *z,int n){
 3         int i;
 4         multcnt++;
 5         for(i=0;i<n;i++)
 6                 z[i] = x[i] * y[i];
 7 }
```

`addvec` 用来实现向量元素累加，`multvec` 用来实现向量元素累积。使用 `gcc -c addvec.c multvec.c` 来编译这两个文件，其中编译选项 `-c` 表示只进行编译和汇编，不进行链接的操作，从而得到可重定位文件 `addvec.o` 和 `multvec.o` 

使用 `ar rcs libvecter.a addvec.o multvec.o` 命令构造静态库文件，得到一个名为 `libvector.a` 的静态库，其内容可以通过 `objdump` 查看：

```
❯ objdump -t libvecter.a
在归档文件 libvecter.a 中：

addvec.o：     文件格式 elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 addvec.c
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 g     O .bss   0000000000000004 addcnt
0000000000000000 g     F .text  0000000000000083 addvec



multvec.o：     文件格式 elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 multvec.c
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 g     O .bss   0000000000000004 multcnt
0000000000000000 g     F .text  0000000000000085 multvec
```

构造测试函数，函数 `main` 调用了函数 `addvec` ，头文件 `vector.h` 中定义了 `libvector.a` 中的函数原型

```c
main.c:
 1 #include <stdio.h>
 2 #include "vector.h"
 3
 4 int x[2] = {1,2};
 5 int y[2] = {3,4};
 6 int z[2];
 7
 8 int main(){
 9         addvec(x,y,z,2);
10         printf("z = [%d,%d]\n",z[0],z[1]);
11         return 0;
12 }
```

其中，头文件 `vector.h` 的内容如下：

```c
#ifndef _vector_H_
#define _vector_H_
    void addvec(int *x,int *y,int *z,int n);
    void multvec(int *x,int *y,int *z,int n);
#endif
```

使用如下语句，依次进行编译、链接：

```
> gcc -c main.c
> gcc -static -o prog main.o ./libvector.a
```

从而得到可执行文件 `prog` ，可以正确运行

```c
❯ ./prog
z = [4,6]
```

!!! info "更多静态库使用方式"
	不导入 `vector.h` ，使用 `gcc main.c ./libvector.a` 可以生成正确的可执行程序
	
	也可以使用 `gcc main.c -L./ -lvector` ，其中 `-L` 选项指定了库文件搜索默认目录， `-l` 后面要接库名，即不包含`lib`和拓展名的字符串

![[静态库构建示例.png]]



!!! question "为什么使用了库文件 `libc.a` 但是手动链接时不需要导入呢？"
	编译器驱动程序总是自动把 `libc.a` 传给链接器，因此其实上述链接命令的完整写法为 `gcc -static -o prog main.o ./libvector.a libc.a`

在链接器对库文件进行扫描的过程中，实际上需要维护三个集合：

- **集合E**：用来存放链接器扫描过程中发现的可重定位文件。在连接即将完成的时候，这个集合中的文件会被合并形成一个可执行文件。
- **集合U**：用来存放被引用了，但是尚未定义的符号。
- **集合D**：用来存放输出文件中已定义的符号。

对于命令行中每一个输入文件，链接器都会判断其是一个目标文件还是一个静态库文件，如果是目标文件，则放入集合E中，同时修改集合U和D来反映该输入文件中的符号定义和引用

如上例中，链接器首先判断 `main.o` 为一个目标文件，将其置入集合E，然后根据其符号表，将 `UND` 类型的 `addvec` 和 `printf` 置入集合U，其它已定义的符号置入集合D

!!! info ""
	![[判断maino后.png]]

当链接器读取到静态库文件 `libvector.a` 时，链接器会发现该静态库文件成员 `addvec.o` 中存在符号 `addvec` 的定义，此时将 `addvec.o` 加入集合E，并移动集合U中的 `addvec` 到集合D中

对 `libc.a` 的判断同理，不再解释。

!!! warning "如果对所有输入文件的扫描均已完成，但集合U非空，则链接器输出一个错误并终止"

因此，命令行中对输入文件的顺序也有要求。

例如，`foo.c` 调用了 `libx.a` 和 `libz.a` 中的函数，`libx.a` 和 `libz.a` 又调用了 `liby.a` 中的函数，此时链接方式为：

```
> gcc foo.c libx.a libz.a liby.a
```

（拓扑排序）

另一个特殊例子，如果 `libx.a` 调用了 `liby.a` 的函数，但是 `liby.a` 也调用了 `libx.a` 的函数，即存在相互引用，此时 `libx.a` 需要在命令行中重复出现：

```
> gcc foo.c libx.a liby.a libx.a
```

!!! note "也可以直接把 `libx.a` 和 `liby.a` 合并成一个静态库文件"

## 重定位

重定位过程中，链接器合并输入模块，并为每个符号分配运行时的地址，具体分为两步：

- **第一步**：重定位 section 和符号定义
- **第二步**：重定位 section 中的符号引用

### 重定位 section 和符号定义

假设要对 `main.o` 和 `sum.o` 进行链接生成可执行文件，藉此讲解重定位第一步做了什么：

```c
> gcc -Og main.c sum.c -o prog
```

链接器将 `main.o` 和 `sum.o` 中所有相同类型的 section 合并为一个新的 section

![[合并section.png]]

由于在 64 位 linux 系统中，ELF 可执行文件的默认从地址 `0x400000` 处开始分配，所以原书中假设合成后的 `.text` 的起始地址为 `0x4004d0` 

合成完成后，程序中每条指令和全局变量都有了唯一的运行时内存地址

### 重定位 section 中的符号引用

#### 重定位条目

即 **Relocation Entries**

当汇编器遇到最终位置不确定的符号引用时，会产生一个重定位条目，用来告诉汇编器在合成可执行文件时应该如何修改这个引用

代码的重定位条目放在 `.rel text` 中，而已初始化数据的重定位条目位于 `.rel data` 中

ELF 重定位条目的结构体定义如下：

```c
typedef struct{
	long offset;
	long type:32,
		symbol:32;
	long addend;
} ELF64_Rela
```

- 每个条目由四个字段组成
	- **offset** 表示被修改的引用的节偏移量
	- **type** 是链接器修改新的引用的参考，ELF 中定义了 32 种重定位类型，一般只记如下两种即可
		- `R_X86_64_PC32` PC 相对地址
		- `R_X86_64_32` 绝对地址
	- **symbol** 表示被修改的引用是哪个符号
	- **addend** 是符号常数，一些类型的重定位要使用它对被修改应用的值做偏移调整

#### 重定位相对引用

对于第二步，假设函数 `main` 中调用了函数 `sum` ，那么其机器码的 `call` 指令所调用的地址在链接过程中必须重定位到 `sum` 的真正地址

```
0000000000000000 <main>:
   0:   48 83 ec 08             mov    $0x8 ,%rsp
   4:   be 02 00 00 00          mov    $0x2, %esi
   9:   bf 00 00 00 00          mov    $0x0, %edi
                                a : R_X86_64_32 array
   e:   e8 00 00 00 00          callq  13<main+13>
                                f : R_X86_64_PC32 sum
  13:   48 83 c4 08             add    $0x8, %rsp
  17:   c3                      retq
```

在这个例子中，汇编器产生了两个重定位条目，一个是对全局变量 `array` 的引用，另一个是对 `sum` 的引用

在重定位前，指令 `call` 的起始地址位于偏移 `0xe` 的地方，`0xe8` 表示指令的操作码，紧跟在操作码后的内容被填充为 0 ，接下来，链接器根据对于函数 `sum` 的重定位条目来确定这部分的具体内容：

```
r.offset = 0xf
r.symbol = sum
r.type = R_X86_64_PC32
r.addend = -4
```

所以函数 `sum` 在引用的运行时地址可以由函数 `main` 的地址与 `r.offset` 相加得到：

先前我们假设 `.text` 起始地址为 `0x4004d0` ，函数 `main` 起始地址自然就是 `.text` 的起始地址，且原书有假设函数 `sum` 的地址为 `0x4004e8` ，那么根据地址之间的关系，有：

```
ref_addr = ADDR(main) + r.offset
         = 0x4004d0 + 0xf
         = 0x4004df
*ref_ptr = ADDR(sum) - ref_addr + r.addend
         = 0x4004e8 - 0x4004df + (-4)
         = 0x5
```

实际上，这一步就是求两个地址间的相对位置，最终修改得到该 `call` 指令在可执行程序中的形式为：

```
e:       e8 00 00 00 00     callq  13<main+13>
         ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ 
4004de:  e8 05 00 00 00     callq  4004e8<sum>
```

!!! info "`addend`"
	当程序执行到该 `call` 指令时，PC 的值为 `0x4004e3` ，即下一条指令的地址，这也是为什么计算时需要加上 `r.addend`
	
	PC 已经存储着下一条指令 `add` 的地址，但因为指令 `call` 要执行函数调用实现跳转，这时候程序：
	
	**(1)** 把 `add` 的地址(当前 PC 的值)压栈保存
	
	**(2)** 修改 PC 的值，将当前 PC 的值加上偏移量 `0x5` ，得到 `sum` 的地址 `0x4004e8` 


#### 重定位绝对引用

指令 `mov` 尝试把 `array` 的起始地址传给寄存器 `%edi` ，这一条指令的起始地址为 `0x9` ，操作码为 `0xbf` ，后面被填充为 0 的内容即对符号 `array` 引用的绝对地址

```
9:   bf 00 00 00 00          mov    $0x0, %edi
```

对符号 `array` 的引用也对应一条可重定位条目：

```c
r.offset = 0xa
r.symbol = array
r.type = R_X86_64_32
r.append = 0
```

`offset` 字段告诉编译器要从偏移量 `0xa` 处开始修改（即 `mov` 指令引用地址起始位置）

假设链接器已经确定 `array` 所在的 `.data` 位于 `0x601018` ，那么这里的绝对地址引用为：

```
*ref_ptr = ADDR(array) + r.addend
         = 0x601018 + 0x0
```

那么最终修改指令为：

```
9:      bf 00 00 00 00      mov  $0x0, %edi
        ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ 
4004d9: bf 18 10 60 00      mov  $0x601018, %edi
```

!!! note "因为是 `x86` ，所以自然是小端法哦~"

---

综上所述，修改结果为：

```
00000000004004d0 <main>:
   4004d0:   48 83 ec 08        mov    $0x8 ,%rsp
   4004d4:   be 02 00 00 00     mov    $0x2, %esi
   4004d9:   bf 18 10 60 00     mov    $0x0, %edi
   4004de:   e8 05 00 00 00     callq  4004e8<sum>
   4004e3:   48 83 c4 08        add    $0x8, %rsp
   4004e7:   c3    
00000000004004e8 <sum>:
	... ...
```


## 可执行目标文件

可执行目标文件格式与之前介绍的可重定位目标文件格式类似

!!! tip ""
	![[可执行目标文件格式.png]]


- `ELF header` 描述了文件的整体格式，包括程序的入口，即程序运行时执行的第一条指令的地址
- 可执行文件中的 `.init` section 定义了一个名为 `_init` 的函数，程序的初始化代码会调用这个函数进行初始化
- 其它 section 功能与可重定位目标文件中对应节功能相同，不过已经经过重定位到最终运行内存地址上，因此不再需要 `.rel` 相关的 section
- 程序运行时，代码段和数据段需要被加载到内存中执行，符号表和调试信息不会被加载到内存

**Segment header table** 描述了代码段、数据段与内存的映射关系，以一个例子讲解其中各个字段代表的含义：

![[程序段头部表.png]]

<font style="font-weight: 1000;font-size: 20px" color="MediumSlateBlue">对于 **Read-only code segment** </font>

- `off` 表示这个段在可执行文件中的偏移量，由于代码段位于可执行文件的开始处，所以 `off` 等于 0
- `vaddr` 和 `paddr` 表明该段开始于内存地址 `0x400000` 处
- 代码段大小 `filesz` 为 `0x69c` 个字节，所以在内存中也占用 `memsz` ，`0x69c` 字节
	- 这些内容包括 `ELF header` , 程序头部表 , `.init` , `.text` , `.rodata` 等内容
- 末尾的 `flags` 标识表明该段只有读和执行的权限，其中 `r` 表示读， `x` 表示执行， `-` 表示没有写入的权限

!!! note "要求 `vaddr % align = off % align`"
	其实是用来对齐，是的程序执行过程中，可执行文件中的段能够高效传入内存中

<font style="font-weight: 1000;font-size: 20px" color="MediumSlateBlue">对于 **Read/Write data segment** </font>

起始地址为 `0x600df8` ，这个 segment 在目标文件中占 `0x228` 字节，不过在内存中占用 `0x230` 字节，多出来的八个字节用来存放 `.bss section` 的数据

!!! info "`.bss` 不占用可执行文件空间，但是运行时需要被初始化为 0"

---

每一个 Linux 程序在运行时都有一个内存镜像。

!!! danger ""
	![[程序运行内存镜像.png]]

在 Linux x86-64 系统中，代码段总是从地址 `0x400000` 开始，代码段后是数据段，运行时产生的堆在数据段之后。

堆的增长方向是从低地址到高地址，例如 C 语言的 `malloc` 函数申请的空间就属于堆。

堆后面的区域是为共享区域保留的，这个区域将堆和栈隔开。

用户栈的起始地址为 $2^{48}-1$ ，这也是最大的合法用户地址，栈的增长方向是从高地址到低地址。

地址 $2^{48}$  以上的地址，为操作系统的代码和数据保留，这部分内存空间堆用户代码不可见

!!! info "实际上由于数据段地址对齐的要求，代码段和数据段之间存在间隙"

程序运行，加载器为程序创造出上图内存镜像，根据程序头部表的内容，加载器将可执行文件的代码段和数据段复制到内存相应的位置

接下来，加载器跳转到程序的入口，即 `_start` 函数所在地址，`_start` 调用系统启动函数 `__libc_start_main` ，进行初始化执行环境，并调用用户层的 `main` 函数

实际上，想要明白程序加载是如何工作的，需要了解进程、虚拟内存以及内存映射的概念，这些内容在 **Chapter 8** 和 **Chapter 9** 详细介绍

## 动态链接共享库

共享库是一种特殊的可重定位目标文件，在 Linux 系统中后缀通常为 `.so` 

!!! tip "Windows 中 `.dll` 结尾的文件也属于共享库 (Dynamic Link Library)"

共享库在运行或加载时，可以被加载到任意的内存地址，还能和一个在内存中运行的程序进行链接，这个过程即 ==动态链接== ，具体是由动态链接器来执行的

之前的小节中我们曾用 `ar rcs libvector.a addvec.o multvec.o` 来创建一个静态库，对于共享库的创建，所用命令如下：

```
gcc -shared -fpic -o libvector.so addvec.o multvec.o
```

其中 `-shared` 选项是指示编译器创建一个共享的目标文件；`-fpic` 选项告诉编译器生成位置无关的代码，使得共享库可以被加载到任意内存位置

得到名为 `libvector.so` 的共享库后，可以使用这个库来构造可执行程序：

```
gcc -o prog2 main.c ./libvector.so
```

相比于静态库的链接，共享库 `libvector.so` 的内容并没有被复制到可执行文件 `prog2` 中，这个操作只是复制了符号表和一些重定位信息。

当可执行程序 `prog2` 被加载运行时，加载器会发现程序中存在一个名为 `.interp` 的 section ，这个 section 包含了动态链接器的路径名，加载器将这个动态链接器加载到内存中运行，由动态链接器执行重定位代码和数据的工作

!!! not "实际上，动态链接器本身也是一个共享目标文件(`ld-linux.so`)"

- 许多 Windows 应用开发者常常利用共享库来进行软件版本的更新
	- 共享库版本更新后，用户将其下载下来，下一次运行时，应用程序将自动链接和加载新的共享库
- 动态链接还可以用来构建高性能的 Web 服务器，如生成动态内容
	- 实现思路是将每个生成动态内容的函数打包到共享库中，当一个来自浏览器的请求到达时，Web 服务器动态地加载和链接适当地函数，然后进行调用。由于函数会一直缓存在服务器地地址空间中，所以只需要一个简单的函数调用就可以处理之后的请求

**动态链接和静态链接的优缺点如下：**

静态链接库的优点：

- 代码装载速度快，执行速度比动态链接库快
- 开发者发布的时候不需要管用户电脑中是否具备程序运行所需的环境

动态链接库的优点：

- 更加节省内存并减少页面交换
- 库文件与可执行文件独立，只要输出接口不变，更换库文件不会对可执行文件造成任何影响，因而极大地提高了可维护性和可扩展性
- 不同编程语言编写的程序只要按照函数调用约定就可以调用同一个库
- 适用于大规模的软件开发，使开发过程独立、耦合度小，便于不同开发者和开发组织之间进行开发和测试
