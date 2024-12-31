
# Chapter 2. Instruction: Language of Machine

!!! quote "精简指令集"
	前面我们讨论过，计算机的 Performance 受每条指令执行的时间影响，而不同的 CPU 可能使用不同的指令架构集（ISA）。
	
	RSIC-V 是 UC Berkeley 大学设计的第五代RISC芯片，其特点是开源、架构简单、模块化设计，是本课程所要讲解的 ISA。

## 寄存器

RISC-V 一共有 32 个寄存器，每个寄存器宽度均为 64 位，命名从 `x0` 到 `x31` 

!!! tip "不同于8086，RISC-V中 `1 word = 32 bit`，所以一个寄存器有 `doubleword`"

![[riscvreg.png]]

- `x0` 保持常数 `0` ，常用于函数返回时作为无意义地址的接收方： `jalr x0, 0(x1)`
	- `x0` 的读写是无效的，处理器并不会去读写它的值
- `x1` 用来保存返回地址，即 `return address` 。
	- 在调用函数前，使用 `jal x1, FUNC_ADDR` 保存PC当前指令的下一条指令地址，即 `PC+4` 。
	- 调用的函数返回时，使用 `jalr x0, 0(x1)` 返回调用指令的下一条指令继续执行。
- `x2` 即栈指针，始终指向栈顶元素。栈从高地址向低地址增长。
- `x3` 指向全局变量和静态数据区的起始地址，称为 `global pointer` ，用于访问和管理全局变量。
- `x4` 是线程寄存器，指向当前线程的局部寄存器，即线程私有的数据区域。
- `x5-x7` 以及 `x28-x31` 是临时寄存器，通常用于存储计算的中间结果或临时变量，它们是调用者保存的寄存器。
- `x8-x9` 以及 `x18-x27` 也可以用来存储临时数据，不过它们是被调用者保存的寄存器，即被调用函数在开头和结尾分别需要入栈和出栈以恢复它们的值。
	- `x8` 一般也用作帧指针 `frame pointer` ，指向栈的底端
- `x10-x17` 是参数寄存器，函数调用的前八个参数会存储在这些寄存器中，但如果参数超过 8 个就需要放到栈中传递（如果参数为 8 bytes，`sp+8` 是第九个参数，`sp+16` 是第十个参数...）。同时，过程的结果也会放到这些寄存器上，对于C语言这种只有一个返回值的语言，可能只会用到 `x10` 。

!!! warning "Preserved on call"
	是否保证调用前后这些寄存器的值不变。如果为 `yes`，则被调用函数开头结尾分别要将这些寄存器入栈出栈以恢复它们的值；如果为 `no` ，则需要主函数上自行入栈出栈恢复值。

!!! success "过程调用"
	RISC-V 使用 `jal x1, ProcedureAddress` 来调用子程序，使用 `jalr x0, 0(x1)` 返回母程序。在程序调用中，RISC-V 必须使用额外的指令在调用前将调用者保存的寄存器压入内存栈，在调用后将这些寄存器数据弹出内存栈，从而保证这些数据的不变性。

## 指令格式

不同于 8086 无定长的机器码，RISC-V 的指令都是 32 bit ，且有固定格式的。

<img src="../image/riscvasm.png" style="max-height:100%"/>

!!! note "只有 `load` 和 `store` 系列可以访问 Memory"
	![[loadstoreinstr.png]]

RISC-V 指令格式如下：

![[RISCVIGSG.png]]

- `opcode`: 指 **operation** 的编码，大部分时候需要和 `funct3` 和 `funct7` 一起决定指令的种类。
- `rd`: 即 **Destination Register** ，目标寄存器。
- `funct3`: 3 bit 的 function code，相当于 additional opcode。
- `rs1`: 即 **First Source Register** ，第一个源寄存器。
- `rs2`: 即 **Second Source Register** ，第二个源寄存器。
- `funct7`: 7 bit 的 function code，相当于 additional opcode。
- `i`: 立即数，需要注意是有符号数，例如 `i[11:0]` 的范围是 $-2^{11}$ ~ $2^{11}-1$

可以观察到 I-Type 有两个条目，下面那个条目只对应 `slli`,`srli`,`srai` ，因为立即数移位操作并不可能对一个 64 位寄存器进行大于 63 位的移位操作，因此 immediate 中只有后 6 位能实际用上，因此前六位可以用来当作额外的操作码字段。

??? note "UJ 格式无条件跳转指令 `jal` 和 SB 格式条件跳转指令 `branch`"
	注意到，这两个格式指令的立即数没有最低位，这是因为地址偏移量必须是 2 的整数倍，即最低位默认看作 0
	
	- **Offset of `jal`** : $(-2^{20}, 2^{20}-2)$
	- **Offset of `branch`** : $(-2^{12} , 2^{12}-2)$

=== "R-type"
	![[RtypeEncoding.png]]
=== "I-type"
	![[ItypeEncoding.png]]
=== "Others"
	![[OthertypeEncoding.png]]

??? example "例子：对于 R 型指令 `add`"
	![[add转化机器码.png]]

??? example "考察对 IS 理解的题目"
	假设有一指令 `rpt x29, loop` 的效果为 `x29` 自减 `1` ，若 `x29>0` ，则跳转到 `loop` 。如果该指令在 RISC-V 指令集内，它应该是什么格式的。
	
	答案其实是 **U-type** ，因为它只用到一个 rd 寄存器和一个立即数地址。

RISC-V 使用 `Branch` 系列指令进行跳转，举一例演示其应用：

```asm
C CODE:
if(i==j) f = g + h;
else f = g - h;

RISC-V ASM CODE:
assume x19:f, x20:g, x21:h, x22:i, x23:j
	bne x22, x23, else # goto else if i != j
	add x19, x20, x21  # f = g + h
	beq x0, x0, EXIT   # goto EXIT
else:
	sub x19, x20, x21  # f = g - h
EXIT:
	...statement
```

同时，也可以使用无条件跳转指令 `jal` 和 `jalr` (Jump And Link Register) 来达成C语言 switch 语句的效果：

```asm
comment @
模拟 Jump Address Table
@

C CODE:
switch(k){
	case 0: f = i + j; break;
	case 1: f = g + h; break;
	case 2: f = g - h; break;
	case 3: f = i - j; break;
}

RISC-V ASM CODE:
assume x20~x25:f~k, x5:constant 4, x6:JumpTable 的首地址
	blt x25, x0, EXIT
	bge x25, x5, EXIT # if k < 0 or k >= 4, goto EXIT
	slli x7, x25, 3   # x7 = k * 8 ，因为地址表每一项都是 8 Bytes 的
	add x7, x7, x6    # x7 = Address of JumpTable[k]
	ld x7, 0(x7)      # x7 = JumpTable[k]
	jalr x1, 0(x7)    # jump
	...Next Instruction
	...statement
	...
L0:
	add x20, x23, x24 # f = i + j
	jalr x0, 0(x1)    # return Next Instruction
L1:
	add x20, x21, x22 # f = g + h
	jalr x0, 0(x1)
L2:
	sub x20, x21, x22 # f = g - h
	jalr x0, 0(x1)
L3:
	sub x20, x23, x24 # f = i - j
	jalr x0, 0(x1)
```

!!! note "与8086不同，RISC-V中跳转指令的立即数取值并不是目标地址减去下一条指令的地址"

!!! success "几种寻址方式"
	![[几种寻址方式.png]]
	
	- **立即数寻址**（从指令中取立即数数据）
	- **寄存器寻址**（从 Register 中取数据）
	- **基址寻址**（从 Memory 中取数据）
	- **PC相对寻址**（数据为相对PC的立即数）

!!! example "More Example: String Copy"
	![[stringcopyexample.png]]

!!! example "(21-22 Final) 如何在不适用额外寄存器的情况下交换 `x10` 和 `x11` 的值"
	```asm
	xor x10, x10, x11
	xor x11, x10, x11
	xor x10, x10, x11
	```

## 杂项

### 大数加载

由于受指令宽度限制，立即数宽度不能超过 12bit，但是这个大小限制并不能涵盖计算机的常规工作范围。为此，RISC-V 通过组合 `addi` 和 `lui`(Load Upper Immediate) 指令来实现存储 32bit 大小立即数：

`lui` 指令的作用为读取一个 20bit 的立即数，存储进寄存器低 32 位的高 20 位，左侧 32 位全部填充 bit 31（类似符号扩充），右侧 12 位全部填充 0，其指令格式为：

```
31                    11       6        0
+---------------------+--------+--------+
|  immediate[31:12]   |   rd   | Opcode |
+---------------------+--------+--------+
        20bits           5bits    7bits
```

若 `addi` 所加载的 12bit 立即数的最高位为 0，那么很明显符合逻辑：

```asm
lui x19, 976 # 0000 0000 0011 1101 0000
addi x19, x19, 1280 # 0101 0000 0000
# [63:32]0,00000000 00111101 0000//0101 00000000 = 976*2^{12} + 1280
```

但是由于 RISC-V 的立即数都视为有符号数，若 `addi` 加载的数最高位为 1 ，该立即数会被视为负数，并对其进行 sign extension（即用 1 填充至 64bit），这种情况相当于额外增加了 `0xFFFFF000` ，为了补偿这个错误，需要加上 `0x00001000` 抵消，即对 `lui` 的立即数加一。例如，加载 976 * 2<sup>12</sup> + 2304 = 4000000：

```asm
lui x19, 977         # 976 + 1
addi x19, x19, 2304  # 计算机视其为 negative
```

??? tip "Addition"
	![[addix19x192304.png]]

在 [Compiler Explorer](https://godbolt.org/) 中用 RISC-V(32bit) gcc 编译下列代码：

```c
int main(void) {
	int a;
	a = 4000000;
}
```

可以看到 `a = 4000000` 对应的汇编代码为：

```asm
 003d17b7
 lui    a5,0x3d1 # a5 == x15, 0x3d1 = 977
 90078793
 addi   a5,a5,-1792 # -1792 == 2304
 fef42623
 sw a5,-20(s0)
```

!!! example "(23-24 Final) 将 `0x12345678ABCDEF` 加载到寄存器 `x10` 中"
	```asm
	lui x10, 0x12345
	addi x10, x10, 0x678  # x10 = 0x12345678
	lui x11, 0xABD        # 0xABC + 1
	addi x11, x11, 0xDEF  # x11 = 0xABCDEF, also `addi x11, x11, -529`
	slli x10, x10, 24
	add x10, x10, x11 # or x10, x10, x11
	```

### 寻址

采用小端寻址 **little endian** 。

![[littleendianriscv.png]]

### 嵌套调用

<font style="font-weight: 1000;font-size: 20px" color="orange">斐波那契数列：</font>

当调用函数时，使用栈来存储函数返回地址、传递的参数（且 `x10` 还充当 return 值）。例如，将下列递归计算斐波那契数列的函数转换成 RISC-V 汇编语言：

```c
int fib(int n) {
	if (n == 0)
		return 0;
	else if (n == 1)
		return 1;
	else
		return fib(n-1) + fib(n-2);
}
```

**完全优化版本：**

```asm
fib:
	beq x10, x0, done// n == 0
	addi x5, x0, 1
	beq x10, x5, done// n == 1
//分配栈空间
	addi x2, x2, -16 // allowcate stack space
	sd x10, 8(x2)    // push x10 (the value of n)
	sd x1, 0(x2)     // push x1 (the return address)
	addi x10, x10, -1
	jal x1, fib      // fib(n-1)
	ld x5, 8(x2)     // load n in this loop
	sd x10, 8(x2)    // push fib(n-1)
	addi x10, x5, -2 // x10 = n-2
	jal x1, fib      // fib(n-2)
	ld x5, 8(x2)     // x5 = fib(n-1)
	add x10, x10, x5 // x10 = fib(n-1) + fib(n-2)
//恢复栈
	ld x1, 0(x2)     // load return address
	addi x2, x2, 16  // restore the stack
done:
	jalr x0, x1
```

**无优化，但是易于理解的版本：**

```asm
main:
    addi a0, x0, 6
    jal ra, fib    # call fib(6)
    add x0, x0, x0 # NOP

fib:
    addi sp, sp, -16
    sw s0, 8(sp)
    sw ra, 0(sp)
    
    add s0, a0, x0
    beq s0, x0, done0 # if(n==0) return 0
    addi t0, x0, 1
    beq s0, t0, done0 # if(n==1) return 1
    addi a0, s0, -1  # n-1
    
    addi sp, sp, -16 # 调用者保存
    sw t0, 8(sp)
    sw t1, 0(sp)
    
    jal ra, fib      # call fib(n-1)
    
    lw t0, 8(sp)     # 调用者恢复
    lw t1, 0(sp)
    addi sp, sp, 16
    
    add t0, a0, x0   # t0 = fib(n-1)
    addi a0, s0, -2  # n-2
    
    addi sp, sp, -16 # 调用者保存
    sw t0, 8(sp)
    sw t1, 0(sp)
    
    jal ra, fib      # call fib(n-2)
    
    lw t0, 8(sp)     # 调用者恢复
    lw t1, 0(sp)
    addi sp, sp, 16
    
    add t1, a0, x0   # t1 = fib(n-2)
    add a0, t0, t1   # a0 = fib(n-1) + fib(n-2)
    jal x0, done
done0:
    add a0, x0, s0   # a0 = n
done:
    lw s0, 8(sp)
    lw ra, 0(sp)
    addi sp, sp, 16
    jalr x0, ra, 0   # return a0
```


<font style="font-weight: 1000;font-size: 20px" color="orange">阶乘：</font>

```c
long long int fact(long long int n) {
    if ( n < 1 ) return 1;
    else         return n * fact(n - 1);
}
```


```asm
fact: 
	  addi sp, sp, -16      # adjust stack for 2 items
      sd   x1, 8(sp)        # save the return address
      sd   x10, 0(sp)       # save the argument n
      addi x5, x10, -1      # x5 = n - 1
      bge  x5, x0, L1       # if n >= 1, go to L1
      addi x10, x0, 1       # return 1
      addi sp, sp, 16       # adjust stack to pop 2 items (no need to ld)
      jalr x0, 0(x1)        # return to caller
L1:   
	  addi x10, x10, -1     # n >= 1: argument gets (n - 1)
      jal  x1, fact         # call fact with (n - 1)
      addi x6, x10, 0       # move result of fact (n - 1) to x6
      ld   x10, 0(sp)       # restore argument n
      ld   x1, 8(sp)        # restore the return address
      addi sp, sp, 16       # adjust stack to pop 2 items
      mul  x10, x10, x6     # return n * fact(n - 1)
      jalr x0, 0(x1)        # return to the caller
```

> From [Minjoker](https://note.minjoker.top/courses/co/note1/)

!!! note "读取和存储时都要注意位宽"
	对基址为 `a0` 的 `int` 型数组，取第 `i` 个元素，需要使用 `load word` 指令：`lw rd, i*4(a0)` 
	
	像我们递归使用存 64bit 寄存器的值，就使用 `load doubleword`

<font style="font-weight: 1000;font-size: 20px" color="orange">互相调用，存在两个ra：</font>

```c
int foo(a, b) {
	int c = bar(a, b);
	return 2 * c;
}

int bar(a, b) {
	return a + b;
}
```

```asm
foo:
	addi sp, sp, -24  # 开辟栈空间，将函数返回地址 ra 以及
	sd s0, 0(sp)      # 可能用到的临时栈寄存器都压入栈中，
	sd s1, 8(sp)      # 在函数返回前，恢复它们的值
	sd ra, 16(sp)     # 通常开辟 n+1 个位置
	add s0, x0, a0    # s0 = a
	add s1, x0, a1    # s1 = b
	
	add a0, x0, s0    # 准备调用 bar，传入参数 a
	add a1, x0, s1    # 同上，传入参数 b
	jal x1, bar       # 调用 bar
	add s0, x0, a0    # 返回值 c 存在 a0 中，先暂存在 s0 中
	slli s0, s0, 1    # c = c * 2
	add a0, x0, a0    # 将 foo 的返回值存入 a0
	
	ld ra, 16(sp)     # 恢复值
	ld s1, 8(sp)
	ld s0, 0(sp)
	addi sp, sp, 24   # 清理栈
	
	jalr x0, 0(ra)    # 回到主函数
```

```asm
bar:
	addi sp, sp, -24    # 开辟栈空间，凭经验来讲传入
	sd s0, 0(sp)        # n个参数，就开n+1个位置 
	sd s1, 8(sp)        # (n+1)*8
	sd ra, 16(sp)       #
	add s0, x0, a0      # 参数转移到当前占用的寄存器中
	add s1, x0, a1      # 同上，s1 = b
	
	add s0, s0, s1      # a = a + b
	add a0, x0, s0      # 返回值存入 a0
	
	ld ra, 16(sp)       # 恢复栈
	ld s1, 8(sp)        #
	ld s0, 0(sp)        # 
	addi sp, sp, 24     # 
	
	jalr x0, 0(ra)      # 回到 foo 函数
```

更严格的来讲，对于过程开头开辟栈空间部分:

- **需要保留的寄存器**
	- Saved Registers: `x8-x9`,`x18-x27` (**s0,s1...**)
	- Return Address: `x1` (**ra**)
	- Stack Point: `x2` (**sp**)
	- Frame Point: `x8` (**tp**)
	- Stack Above: `sp` 之上的数据
- **不需要保留的寄存器**
	- Argument Registers: `x10-x17` (**a0,a1,...**)
	- Temporary Registers: `x5-x7`,`x28-x31` (**t0,t1,...**)
	- Stack Below: `sp` 之下的数据

<font style="font-weight: 1000;font-size: 20px" color="orange">(23-24 Final) 使用了没教过的 rem 和 div 指令???</font>

```asm
/*
int sum(int num) {
    if(num < 10) return num;
    return num % 10 + sum(num / 10);
}
*/

sum:
    addi sp, sp, -16
    sd ra, 0(sp)
    sd s0, 8(sp)

    add s0, a0, x0    # s0 = a0
    addi t0, x0, 10   # t0 = 10
    blt s0, t0, done0 # if(s0 < 10) goto done0

    div a0, s0, t0    # a0 = num / 10
    rem s0, s0, t0    # s0 = num % 10
    jal ra, sum       # sum(num / 10)

    add a0, a0, s0    # a0 = num / 10 + sum(num % 10)
    beq x0, x0, done  # goto done

done0:
    add a0, s0, x0    # a0 = num
done:
    ld ra, 0(sp)
    ld s0, 8(sp)
    addi sp, sp, 16
    jalr x0, 0(ra)
```


### 有用资源

- [RISC-V 指令译码/解码](https://luplab.gitlab.io/rvcodecjs/)
- [Venus](https://venus.cs61c.org/)
- [Rimulator](https://hggshiwo.github.io/rimulator/index.html)
