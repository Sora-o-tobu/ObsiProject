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

