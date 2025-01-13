
!!! quote "虽然之前听说过bhh的汇编很难，但是这门通识汇编我感觉还是很简单的，选就对了！"

**推荐看 TonyCrane 的汇编语言笔记**：https://note.tonycrane.cc/cs/pl/asm/

中断指令大全： [Ralf Brown's Interrupt List](./interruptlist/rbrown.htm)

```
tasm test.asm    ; masm test;

tlink test.asm   ; link test;

td test.asm      ; 
```

使用 `command` 打开 Dos，不过不用 Dos 也无所谓就是了。（神必windows更新还专门ban了16bit程序，搞毛呢）

[MS-DOS Player for Win32-x64 謎WIPページ](http://takeda-toshiya.my.coocan.jp/msdos/)


## 杂项

一般程序如何寻找 main 函数的入口？

看见三个连续的 `push`，一个 `call` ，那么 `call` 的地址就是 main 的入口

`0Dh` 代表回车(\\r)，表示回到行首；`0Ah` 代表换行(\\n)，表示垂直向下换行。所以在 Windows 系统中二者一起出现。

C语言的 `fgetc()` 函数读取文件时每次读取一个字节，但是当读到 `0Dh` 时，会自动把后面的 `0Ah` 一起读取合并成 `0Ah` （除非是 **rb** 方式打开文件）
