
**推荐看 TonyCrane 的汇编语言笔记**：https://note.tonycrane.cc/cs/pl/asm/

中断指令大全： [Ralf Brown's Interrupt List](./interruptlist/rbrown.htm)

```
tasm test.asm

tlink test.asm

td test.asm
```

使用 `command` 打开 Dos，不过不用 Dos 也无所谓就是了。

一般程序如何寻找 main 函数的入口？

看见三个连续的 `push`，一个 `call` ，那么 `call` 的地址就是 main 的入口