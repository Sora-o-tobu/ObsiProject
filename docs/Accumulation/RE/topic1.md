
# 逆向技术基础

## 逆向分析法

代码逆向工程（**Reverse Code Engineering, RCE**）是逆向工程（RE）在软件领域中的应用。通常我们将 RE, RCE 混用，不做特意区分。

RCE 的主要对象是*可执行文件*，分析可执行文件时主要使用**静态分析法**和**动态分析法**两种方法：

- <1> **静态分析法**
	- 并不执行代码，而是观察代码的外部特征，获取文件的类型、大小、PE 头信息、Import/Export API、内部字符串、是否运行时解压缩、注册信息、调试信息、数字证书等多种信息
	- 使用反汇编工具 Disassembler 查看内部代码也属于静态分析
- <2> **动态分析法**
	- 通过*调试*分析代码流，获得内存的状态
	- 动态分析通常借用调试器 Debugger 完成

接下来我们以一个最简单的、涉及 Windows API 的 Hello World 程序为例，它的源代码如下：

```c++
#include "windows.h"
#include "tchar.h"

int main(int argc, char* argv[])
{
    MessageBox(NULL, _T("Hello, World!"), _T("Hello"), MB_OK);
    return 0;
}
```

我们使用 `g++` 对其进行编译，运行生成的二进制文件 `hello.exe`，其效果为弹出一个标题为 "Hello"，内容为 "Hello World!" 的弹窗。

[OllyDbg](https://ollydbg.de/) 是一个专业的 Win32 调试工具，现代 PC 基本为 64 位架构，因此要编译出 32 位 PE 文件可能需要如下操作：

```bash
# 安装 32-bit 编译工具
pacman -S mingw-w64-i686-toolchain

# 使用 32-bit 编译器编译
i686-w64-mingw32-g++ .\hello.cpp -o hello.exe -luser32
```

!!! warning "过程中可能涉及的环境问题请自行解决"

此时通过 `file` 命令查看，得到的结果理应如下：

```bash
> file hello.exe
hello.exe: PE32 executable (console) Intel 80386, for MS Windows, 18 sections
```

使用 OllyDbg 打开 `hello.exe`，页面如下：

![[re_topic1_1.png]]

- 左上角为**代码窗口**，默认显示反汇编代码，还用于显示各种注释、标签、分析代码时显示循环、跳转位置等信息
- 右上角为**寄存器窗口**，实时显示 CPU 寄存器信息，并允许修改
- 左下角为**内存窗口**，实时显示进程的内存地址，并允许修改
- 右下角为**栈窗口**，显示 ESP 指向的进程栈内存，并允许修改


| 指令                  | 快捷键     | 含义                            |
| ------------------- | ------- | ----------------------------- |
| Restart             | Ctrl+F2 | 重新开始调试                        |
| Step Into           | F7      | 执行一条指令，如果遇到调用命令（Call），则进入函数内部 |
| Step Over           | F8      | 执行一条指令，如果遇到调用命令（Call），则执行函数本身 |
| Execute till Return | Ctrl+F9 | 一直在函数内部运行，直到遇到 RETN 指令        |

我们观察到第二条指令会跳转到地址 `0x00C71160`，该地址通常是程序的“启动函数（Stub Code）”。但为了寻找我们的 `main` 函数，需要进入该启动函数进行跟踪：

![[re_topic1_2.png]]

!!! info "启动函数是不同编译器根据自身特点添加的"

对于初学者来说，一个笨拙的方法是在启动函数种的每个 `call` 调用时都使用 step into 进入，然后检查代码中是否调用了我们的 MessageBox API，如果没有，则 Execute till Return 快速退出。

经过一番搜索后，我们成功在 `C712E9` 处找到正确的 `main` 函数入口。

```asm
00C712D4   .  894424 08  mov     [esp+0x8], eax
00C712D8   .  A1 1860C70>mov     eax, [0xC76018]
00C712DD   .  894424 04  mov     [esp+0x4], eax
00C712E1   .  A1 1C60C70>mov     eax, [0xC7601C]
00C712E6   .  890424     mov     [esp], eax
00C712E9   .  E8 0202000>call    00C714F0
```

!!! note "根据 bhh 老师的说法，通常三个连续的 `push` 接一个 `call` 即为 `main` 函数"

`main` 函数对应的汇编代码如下：

![[re_topic1_3.png]]

调试是一个大工程，为了在每次重新运行时快速回到几个重要节点，可以使用如下几个方法：

- **Goto**
	- 使用 Goto 命令（*Ctrl+G*），快速跳转到某一位置，然后执行 Execute till Cursor（*F4*）
	- 例如对于上例，在 Goto 命令的框中输入 `00C714F0`，下方选择 `VA/API` 选项即可
- **BP**
	- 设置 BreakPoint（*F2*），程序运行到断点后会暂停
	- 可以通过 View-BP 选项（*ALT+B*）查看当前所有断点
- **注释**
	- 通过快捷键 *;* 在指定代码处添加注释，然后可以在右键菜单中的“查找->用户定义的注释”中找到
	- 用户光标和注释在同一行的话，仅显示用户光标，注释为“初始 CPU 选择”
- **标签**
	- 通过快捷键 *:* 设置指定地址为标签，此后该地址会被替换为该标签，更加直观

事实上，我们直到 `hello.exe` 程序会弹出一个标题为 "Hello"，内容为 "Hello, World!" 的消息框，使用了 MessageBox API，因此我们可以使用一些更直接的方法来寻找 `main` 函数的地址。

=== "字符串检索法"
	第一种方式是在右键菜单中的“查找->所有参考文本字串”中找到对应的字符串，通过双击即可定位到对应位置。
	
	![[re_topic1_4.png]]
=== "API 检索法 1"
	第二种方式是在右键菜单中的“查找->所有模块间调用”中找到对应的 API 函数，例如对于本例找 `user32.MessageBoxA` 即可。
	
	![[re_topic1_5.png]]
=== "API 检索法 2"
	有时，压缩器/保护器会对可执行文件进行压缩/保护，导致我们可能无法列出所有的 API 调用列表。这种情况下我们可以对该 API 所在的 DLL 下断点。
	
	我们打开 View-Memory 窗口（*ALT+M*），可以看到进程的内存映射情况，在之中可以看到 `user32` 库被加载到内存中：
	
	![[re_topic1_6.png]]
	
	在右键菜单中的“查找->所有模块的名称”中，我们可以看到每个库提供的所有 API，单击“名称”栏可以按名称进行排序，然后键盘输入 MessageBox 即可自动定位到相关 API 名称。
	
	![[re_topic1_7.png]]
	
	!!! tip "我们需要的是 Export 类型的 MessageBoxA，在书中则用的是 MessageBoxW"
	
	双击进入 MessageBoxA 的代码， 并在入口处设置断点。此后运行程序，会在该入口处暂停，此时查看栈，可以得知栈帧情况。第一个参数即为该 API 的最终返回地址：
	
	![[re_topic1_8.png]]

接下来我们尝试对这个程序进行一些更高级的把戏——**Patch**。为了修改消息框显示的内容，我们同样有两种基本思路。

我们知道，MessageBoxA 调用的参数通过栈进行传递，并且传递是 ASCII 字符串的地址而不是 ASCII 字符串本身。

=== "修改内存中字符串数据"
	第一种方法通过修改内存中对应地址处的字符串数据完成。我们通过汇编代码得知消息框的内容 "Hello, World!" 位于地址 `00C7404A` 处，我们在左下角的 dump 窗口通过 Ctrl+G 跳转到该位置：
	
	![[re_topic1_9.png]]
	
	选中该字符串区域，通过快捷键 *Ctrl+E* 对其进行编辑。需要注意的是，要保持前后长度不变，后面需要补齐 NULL。如果要修改的数据长于原本数据，则需要注意该数据之后的值被修改后是否会影响程序运行。
	
	![[re_topic1_10.png]]
	修改完后，直接运行，结果如下：
	
	![[re_topic1_11.png]]
=== "使用一段新的内存，改变传递的参数"
	我们找到一块没有使用的内存区域，将其作为新的参数传递给 MessageBoxA：
	
	![[re_topic1_12.png]]
	
	程序的运行结果如下：
	
	![[re_topic1_13.png]]
	
	!!! warning "第一种方法可以被保存，第二种方法不行，但是可以支持更长字符串"
		因为内存地址都是以 offset 形式存在的，我们自定义的地址并没有相应的 offset。

## IA-32 寄存器

Intel Architecture 32-bit 支持如下寄存器类型：

```text
Basic Program Execution Registers
- 8 个 32-bit General Purpose Registers
- 6 个 16-bit Segment Registers (CS, DS, SS, ES, FS, GS)
- 1 个 32-bit Program Status & Control Register (EFLAGS)
- 1 个 32-bit Instruction Pointer (EIP)

x87 FPU Registers

MMX Registers

XMM Registers

Control Registers

Memory Management Registers

Debug Registers

Memory Type Range Registers

Machine Specific Registers

Machine Check Registers

...
```

Basic Program Execution Registers 的相关知识想必已经在其它课程中熟知，这个阶段也仅需了解该内容即可。

!!! bug "后面有讲其它寄存器的话可能会回来补充"

## Abex' crackme

> 我是从[这里](https://github.com/jaehyeon2/reverse_engineering)下载的二进制文件

Abex' crackme 是一系列用于破解练手及教学的简单小程序。

### #1

直接用汇编语言书写的，非常简明易懂，直接把光标处的 `je` 该成无条件跳转 `jmp` 即可。

![[re_topic1_14.png]]

### #2

第二个 exe 使用 VB 引擎，其代码的绝大部分也都是该引擎的代码。为了快速定位我们所需的用户代码，我们搜索弹出的文本框中的字符串文字：

![[re_topic1_15.png]]

我们在 "Congratulaitons!" 上方找到了判断是否正确的相关逻辑，它会调用字符串比较函数 `_vbaVarTstEq` 来对正确的序列号和用户输入的序列号进行比较，如果相等则返回 1。随后通过 `TEST` 和 `JE` 进行逻辑跳转。 


![[re_topic1_16.png]]

我们需要搞清楚序列号生成算法的核心逻辑：

- **0040318B (`call __vbaVarForInit`)**: 初始化循环，`For i = 1 To 4`。
- **00403191 (`call rtcMidCharVar`)**: 相当于 VB 中的 `Mid(Name, i, 1)`，即取出当前位置的字符。
- **004031F7 (`call rtcAnsiValueBstr`)**: 相当于 VB 中的 `Asc(Char)`，获取该字符的 ASCII 码数值。
- **00403233**: `mov dword ptr [ebp-0xD4], 0x64`。这里将 **0x64** (十进制的 **100**) 放入变量中。
- **00403243 (`call __vbaVarAdd`)**: 将字符的 ASCII 码与 **100** 相加。
    - 算法公式：`Temp = Asc(Char) + 100`
- **0040325B (`call rtcHexVarFromVar`)**: 将相加后的结果转换为十六进制字符串。
    - 算法公式：`HexStr = Hex(Temp)`
- **0040327B (`call __vbaVarCat`)**: 字符串拼接。将转换好的 Hex 字符串追加到最终的序列号变量中。
    - 算法公式：`Serial = Serial & HexStr`
- **0040329A (`call __vbaVarForNext`)**: 循环继续，直到处理完 4 个字符。

```c
void CKengen_TemplateDlg::OnBtnDecrypt() 
{
    CString str;
    GetDlgItemText( IDC_EDIT_NAME,str );                    //获取用户名字串基本信息。
    int len = str.GetLength();

    if ( len >= 4 ){                                        //格式控制。
        CString PassWord = "";
        CString Temp = "";

        for ( int i = 0 ; i != 4 ; i++ ){
            Temp.Format( "%X",( str[i] + 0x64 ) );
            PassWord += Temp;
        }

        SetDlgItemText( IDC_EDIT_PASSWORD,PassWord );
    }
    else
        MessageBox( "用户名格式错误！" );
}
```

!!! success "VB 过于古老，看不懂函数和变量命名也是情有可原的😂"
	就像这里只循环四次，而不是 `Len` 次，就是由 VB 函数的参数含义决定的。

![[re_topic1_17.png]]

!!! note "VB 字符串为 Unicode 格式，因此直接在内存中查看时需要注意转换（可在右键菜单中改变视图）"

## 函数调用约定

**cdecl**, **stdcall**, **fastcall** 是三种主要的函数约定。但不管哪种方式，都是通过栈来传递参数。

- **cdecl** 主要用于 C 语言，规定由调用者清理栈

```c
int main()
{ return add(1, 2); }
```

编译后结果为：

```asm
PUSH  EBP
MOV   EBP, ESP
PUSH  2
PUSH  1
CALL  add
ADD   ESP, 8
POP   EBP
```

- **stdcall** 主要用于 Win32 API，规定由被调用者清理栈

```c
int add(int a, int b)
{ return a + b; }
```

编译后结果为：

```asm
PUSH  EBP
MOV   EBP, ESP
MOV   EAX, DWORD PTR [EBP+8]
ADD   EAX, DWORD PTR [EBP+4]
POP   EBP
RETN  8
```

!!! info "相比 cdecl，不用每次调用函数前后都维护清理栈，并且有着更好的兼容性和安全性"

- **fastcall** 与 stdcall 类似，但使用寄存器来传递前两个参数（`ECX`, `EDX`）

用寄存器传参实现更快的速度，但需要付出额外开销管理 `ECX` 和 `EDX` 两个寄存器。