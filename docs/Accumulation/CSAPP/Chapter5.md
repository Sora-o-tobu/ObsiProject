# Chapter 5. Optimizing Program Performance
!!! quote "编写高效的程序"
	- 选择合适的算法和数据结构
	- 理解编译器的能力与局限性
	- 使用并行化

## 优化程序性能

### 编译器优化的局限性

!!! example "内存别名引用"
	![[内存别名引用.png]]
	函数 `add1` 运行过程需要读写六次内存；函数 `add2` 只需要读写三次内存。乍一看 `add2` 是 `add1` 的优化写法，实际上，若指针 `xp` 和 `yp` 都指向同一个内存地址，二者功能是不一样的。
	
	我们将两个指针可能指向同一个内存位置的情况称为内存别名引用

**More Example**：

```c
x = 1000;
y = 3000;
*p = x;
*q = y;
answer = *p;
```

编译器无法确定两个指针是否指向同一个位置，因此不会对其进行优化

```c
long f();

long function1(){
	return f()+f()+f()+f();
}

long function2(){
	return 4 * f();
}
```

每次调用 `f()` 的返回值可能不相同，因此编译器也不会将函数 `function1` 优化成 `function2`

### 程序结构的优化

!!! info "由于编译器的优化并不是特别激进，所以需要程序员花费更多的精力来编写高质量的代码"

设想有一个向量 `a` ，需要计算其前置和并存入向量 `p` 中，可以想到的一个较为高效的实现方法为：

```c
void method1(float a[], float p[], long n){
	long i = 0;
	p[0] = a[0];
	for(i=1;i<n;i++)
		p[i] = p[i-1] + a[i];
}
```

不过还有更加高效的实现方法：

```c
void method2(float a[], float p[], long n){
	long i = 0;
	p[0] = a[0];
	for(i=1;i<n-1;i+=2){
		float mid_val = p[i-1] + a[i];
		p[i] = mid_val;
		p[i+1] = mid_val + a[i+1];
	}
}
```

这种优化方式称为 **==循环展开==**  (访问向量 `p` 的次数减少了)

**接下来看另外一个程序优化例子：**

定义一个向量结构体，其结构包含向量长度 `len` 及数据 `data`

```c
#define IDENT 0;
#define OP +;
typedef int data_t;

typedef struct {
	long len;
	data_t *data;
} vec_rec, *vec_ptr;
```

给出其原始函数，用来计算其和：

```c
void combine1(vec_ptr v, data_t *dest){
	long i;
	*dest = IDENT;
	for(i = 0;i < vec_length(v); i++ ){
	//函数 vec_length 计算向量长度
		data_t val;
		get_vec_element(v,i,&val);
		//函数 get_vec_element 用来取向量元素
		*dest = *dest OP val;
	}
}
```

这段函数可优化的点在于：

- `for` 循环每次迭代都要调用函数 `vec_length`，实际上，向量长度不会随着调用而改变
	- 所以可以在循环开始前就调用 `vec_length` ，将结果赋值给局部变量
- 同理，`for` 循环中另一个函数调用 `get_vec_element` 也可以移出
	- 改写该函数，使其返回数组的起始地址，直接访问数组元素
- 每次循环都要读写 `*dest` 的值，这是不必要的内存引用
	- 可以引入一个临时变量 `acc`，用来记录累计的结果，直到循环结束后再把结果写入目的地址 (修改寄存器比内存快)

```c
void combine4(vec_ptr v, data_t *dest){
	long i;
	long length = vec_length(v);
	data_t *data = get_vec_start(v);
	data_t acc = IDENT;
	for(i = 0;i < length; i++ ){
		acc = acc OP data[i];
	}
	*dest = acc;
}
```

??? info "程序性能的提升"
	![[程序性能的提升.png]]

### 从处理器角度优化程序性能

现代处理器能够同时执行多条指令，这种技术被称为 **指令级并行**

!!! danger "这章内容不想做笔记了！"

!!! danger "这章内容不想做笔记了！"

!!! danger "这章内容不想做笔记了！"