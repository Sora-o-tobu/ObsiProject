
## 可执行文件

!!! question "为什么计算机可以执行给定的程序呢"
	因为任何程序都将最终转化成 ==指令== 的形式由计算机执行

![[程序转化成指令的例子.png]]

!!! tip
	上图从高级语言转化到汇编语言的过程称为编译(Compile)，从汇编语言转化到指令的过程称为汇编(Assemble)

![[编译的过程.png]]

- 编译执行：通过编译器 (compiler) 将代码转化为机器指令格式的程序，进而执行
	- C++
	- Golang
	- 汇编
- 解释执行：通过解释器 (interpreter) 将代码转化为 VM 格式的程序（如字节码），进而在 VM上执行
	- Python
	- Java (虽然需要通过javac编译，但本质还是解释型语言)
	- PHP
	- Shell
	- Matlab


![[用户态可执行文件.png]]

!!! note "在CTF课程中，我们要面对的一般都是ELF格式的可执行文件"

- 可以通过命令行攻击静态检视 ELF 文件
	- file
	- objdump  (objdump -d  反汇编) 
	- readelf

## ELF 的编译、链接(以C为例)

- 编译(汇编)：从源代码到目标文件
- 链接：从目标文件到可执行文件

### 编译
源文件 `hello.c` :

```c
#include <stdlib.h>
int main(int argc , char* argv[])
{
	printf("Hello World!\n");
	return 0;
}
```

!!! tip "通过 cat 命令可以将文件内容输出到命令行"

编译指令： `gcc/clang hello.c`

但是我们希望得到编译的中间文件

- 预处理： `gcc/clang -E hello.c -o hello.c.i`
	- 得到的 `hello.c.i` 包含头文件替换、宏展开和替换

---

- 编译： `gcc/clang -S hello.c -o hello.s`
	- `-S`: 只进行 Compile 而不 Assemble 或 link (即进行到汇编语言)
	- `-Ox`: 优化等级，等级越高得到的汇编语言越间接
	- `-g`：启用调试

---

- 生成编译前端(AST or IR)：
	- 生成Clang AST： `clang -Xclang -ast-dump -S hello.c`
	- 生成LLVM IR： `clang -Xclang -emit-llvm -S hello.c -o hello.ll`
	- 生成gcc IR(以及其它细节)： `gcc -fdump-tree-all-graph -S hello.c`
- 生成编译后端：
	- 从LLVM IR到汇编代码： `llc hello.ll -o hello.s`
	- 从汇编代码到目标文件： `llvm-mc -filetype=obj hello.s -o hello.o`
	- 一步到位： `llc -filetype=obj hello.ll -o hello.o`
	- 从汇编代码到目标文件的gcc版本： `as hello.s -o hello.o`

### ELF的链接

- 静态链接
	- 把所有需要的库全部复制一份到调用它的ELF文件中
	- 缺点在于，如果你写了100份hello.c，那么就复制了100份printf的实现函数，空间需求大
	- 优点为，不管你电脑上有没有stdio.h的环境，程序都可以执行；以及跑的速度更快
- 动态链接
	- 跟静态链接相反，仅仅将程序链接到所用的库上

!!! tip "通过file命令查看可执行文件是statically link还是dynamically link"

一般编译器都默认使用动态链接，`gcc hello.c -static -o hello.o` 使用static参数强制转化为静态链接

!!! tip "使用ldd命令可以查看动态链接文件所链接的东西"

## ELF 的装载、运行

- 可执行程序是(Program)是静态、文件的概念
- 进程(Process)是动态、系统运行时的概念
	- 进程和线程(Thread)

```c
#include <stdio.h>
#include <stdlib.h>

  
__attribute__((constructor)) void func1() {

    printf("Before main\n");

}

  
__attribute__((destructor)) void func2() {

    printf("After main\n");

}


int main(void){

    printf("During main\n");

}
```

对于此程序，运行结果是：

```text
Before main
During main
After main
```

- ELF 程序的生命周期
	- 对于静态链接程序，内核以可执行文件的 e_entry 位置(即_start)作为起点
	- 对于动态链接程序，内核以interpreter文件的 e_entry 位置作为起点
		- loader 负责解析和装载其它符号，进而再跳转到给定可执行文件的_start

!!! tip "起点可以看可执行文件的Header"

## ELF 的交互、调试


## ELF 的逆向

### 静态逆向

- 反汇编：机器指令 $\Rightarrow$ 汇编指令(查表、准确)
	- objdump -d hello.elf (| less) 加上less显示的少一些
- 反编译：汇编指令 $\Rightarrow$ 编程语言
	- [IDA pro](https://hex-rays.com/ida-pro/)
	- [Binary Ninja](https://binary.ninja)
	- [Ghidra](https://github.com/NationalSecurityAgency/ghidra)
	- [Cutter/radare](https://github.com/rizinorg/cutter)
	- [大语言模型;D](https://mlm.lingyiwanwu.com/)


### 动态逆向

**可运行** 和 **可调试** 是高效解决