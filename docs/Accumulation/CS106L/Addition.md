
# C++ 补充

> From [咸鱼暄的代码空间](https://xuan-insr.github.io/cpp/cpp_restart/) & ZJU OOP

## Class

### Forward Declaration

如果当前作用域没有名为 `identifier` 的类，那么形如 `class-key attr identifier;` 的声明是一个 forward declaration [class.name#2](https://timsong-cpp.github.io/cppwp/n4868/class.name#2)。

例如 `class Foo;`，它声明了一个叫 `Foo` 的类；但直到这个类被定义之前，它的类型是不完整的[basic.types.general#5](https://timsong-cpp.github.io/cppwp/n4868/basic.types.general#5)。

不完整的类型有一些限制，但是也有一些可以完成的操作。例如不完整的类型不能用来定义变量（包括成员变量）、作为函数声明或定义的参数或者返回值类型等；但是可以定义指向它的指针。

例如，常见的用途是，两个类可能会互相使用。这时就可以写出类似下面这样的代码：

```c++
struct X;
struct Y {
    X* ptr;
    // X mem; // Error: field has incomplete type 'X'
    X* foo();
};
struct X {
    Y* ptr;
    Y* bar();
};
```

这时第 1 行是必须的，否则第 3 行的 `X` 就是一个未知的类型。

当然，你也可以声明 `struct X` 为静态结构体。

### Using

C++11 引入了 `using` 来声明类型别名，它的用途和 `typedef` 类似，如 `typedef struct arraylist_* arraylist;` 可以写成 `using arraylist = struct arraylist_ *;`。

类型别名的声明也可以是类的成员，其作用域是类的作用域，同样受 access-specifier 的影响。例如：

```c++
struct Foo {
    using elem = int;
    elem x;     // OK, x has type int
    elem add(elem v) { x += v; return x; }
private:
    using type = char;
    type c;     // OK, c has type char
};

// elem y;      // Error: unknown type name 'elem'
Foo::elem z;    // OK, z has type int
// Foo::type c; // Error: 'type' is a private member of 'Foo'
```

`using` 一般是为了支持模板而使用。

### friend

在对一个矩阵类进行乘法运算符重载时，我们可以很轻松的构造出 `Matrix * int` 的情况，只需要在类中写入：

```c++
	Matrix operator*(int x)
	{
		...
	}
```

但是 `int * Matrix` 又如何实现呢？该语句被解释为 `int::operator*(Matrix)` ，但是 `int` 类中又没有对应重载，我们只能将运算符重载放在全局作用范围内，并使用友元函数使其能够访问类的私有成员：

```c++
const int M = 100;
class Matrix {
    int data[M][M];
public:
    Matrix operator+(Matrix mat) { /* */ }
    Matrix operator*(int x) { /* */ }
    Matrix operator*(Matrix mat) { /* */ }
    friend Matrix operator*(int x, Matrix mat); // Designates a function as friend of this class
};
Matrix operator*(int x, Matrix mat) {
    Matrix tmp = mat;   // copy mat
    for (int i = 0; i < M; i++)
        for (int j = 0; j < M; j++)
            tmp.data[i][j] *= x;        // can access private member Matrix::data
    return tmp;
}
```

!!! note "`x * y` 会被重载解析解释为 `x.operator*(y)` 或者 `operator*(x, y)`"

当然，更好的方式是：

```c++
const int M = 100;
class Matrix {
    int data[M][M];
public:
    Matrix operator+(Matrix mat) { /* */ }
    Matrix operator*(int x) { /* */ }
    Matrix operator*(Matrix mat) { /* */ }
};
Matrix operator*(int x, Matrix mat) {
    return mat * x;
}
```

### Virtual

Virtual 为子类提供了实现的接口。其底层实现方式是一个 8 字节的地址 `vptr` 指向一个表，可以用如下一个简单程序进行验证：

```c++
class base {
public:
    base() : data (10) {}
    void foo() { std::cout << "base::foo()" << std::endl; }
    virtual void bar() { std::cout << "base::bar()" << std::endl; }
private:
    int data;
};

int main()
{
    base b;
    b.foo(); // Calls base::foo()
    b.bar(); // Calls base::bar()

    std::cout << "Size of b is: " << sizeof(b) << std::endl;
    /* @output:
    * Size of b is: 16
    * The size of the private member 'data' is 4 bytes.
    * The size of the address of the virtual function table (vptr) is 8 bytes on a 64-bit system.
    * And the remaining 4 bytes are for the padding to align the size of the class to 8 bytes.
    * So the total size is 4 + 8 + 4 = 16 bytes.
    */

    int * p = (int*)(&b);
    std::cout << *p << std::endl; // 4206736
    p++;
    std::cout << *p << std::endl; // 1
    p++;
    std::cout << *p << std::endl; // 10

    return 0;
}
```

!!! note "这也能说明 `private` 成员从外部可以通过指针访问，只需要我们摸清内部内存结构"

一个父类的成员函数被声明为 `virtual` 后，它的所有子类同名函数都会被**隐式**地声明为虚函数，因此每个类都有其各自且不同的 `vtpr`。

一个简单的例子：

```c++
class A {
public:
	A() { f(); }
	virtual void f() { cout << "A::f()"; }
};
class B : public A {
public:
	B() { f(); }
	void f() { cout << "B::f()"; }
};
B temp;

// A::f()B::f()
```

虚函数根据运行时**实际对象**的类型来决定最终调用哪个函数，这一过程称为**多态(Polymorphism)**。

```c++
#include <iostream>

struct A {
    virtual void f() { std::cout << "Af" << std::endl; }
    virtual void f(int) { std::cout << "Afi" << std::endl; }
    virtual void f(int, int) { std::cout << "Afii" << std::endl; }
};

struct B : public A {
    void f() override { std::cout << "Bf" << std::endl; }
    void f(int) override { std::cout << "Bfi" << std::endl; }
};

struct C : public B {
    void f() override { std::cout << "Cf" << std::endl; }
};
int main()
{
    C obj;

    C &c = obj;
    B &b = obj;
    A &a = obj;

    a.f();       // Cf
    a.f(1);      // Bfi
    a.f(1,2);    // Afii

    b.f();       // Cf
    b.f(1);      // Bfi
    // b.f(1,2); // compile error

    c.f();       // Cf
    // c.f(1);   // compile error
    // c.f(1,2); // compile error
}
```

当编译器看到被调用的函数签名为 `virtual` 类型，它就会去查询实际类型 `C` 对应的 virtual function table，`C::vtable` 在继承和覆写后，最终变化如下：

| Offset | Function         |
| ------ | ---------------- |
| 0      | `C::f()`         |
| 1      | `B::f(int)`      |
| 2      | `A::f(int, int)` |

这也是为什么最终输出只有 `Cf, Bfi, Afii` 三种。那么又是什么导致编译错误呢？这是 C++ 关于覆写的另一个特性 **Name Hiding**：

- 如果子类定义了基类的某个**函数名**，那么基类的所有同名重载都将被隐藏，即便它们的参数不同
	- 这一操作位于查找 `vtable` 之前，并且是依据**静态类型**的函数名查找和静态解析
- 此例中，子类 `C` 覆写了 `f()`，导致 `f(int)` 和 `f(int, int)` 都被隐藏，此时即便它们都位于 `C::vtable` 中，实际可见的也只有 `C::f()`（在查找 `vtable` 前编译就失败了）

???+ note "调用 `b.f(1);` 的过程"
	- **Step 1:** 名字查找
		- 编译器看到 `b.f(1)`，分析得到静态类型为 `B&`
		- 在 `B` 中查找函数名 `f`，找到了两个重载
			- `void f()`
			- `void f(int)`
	- **Step 2:** 重载决议
		- 根据传入参数，匹配合适版本
		- 此处 `void f(int)` 符合，编译通过
	- **Step 3:** 虚函数调度
		- 根据其动态类型 `C` 查找 `vtable`，找到槽位 `B::f(int)`

如果希望编译器能找到所有重载，可以使用 `using` 显式引入：

```c++
struct B : public A {
    using A::f;  // 👈 显式引入所有 A::f
    void f() override { std::cout << "Bf" << std::endl; }
    void f(int) override { std::cout << "Bfi" << std::endl; }
};

struct C : public B {
    using B::f;  // 👈 显式引入所有 B::f
    void f() override { std::cout << "Cf" << std::endl; }
};
```

!!! danger "名字隐藏是静态类型驱动的行为，而虚函数调度是动态类型驱动的行为"

### 生命周期

!!! quote "C++ Memory Model"
	![[C++MemoryModel.png]]

对于一个类对象，它的生命周期(lifetime)从它的构造函数完成开始，到它的析构函数被调用时结束。

任何一个对象都会占用存储，这部分存储的最小生命周期称为这个对象的 **storage duration**。对象的 lifetime 小于等于 storage duration。

!!! note "最小生命周期"
	最小指的是，对象被析构后，对应的存储不一定被立刻回收

在 C++11 前，任何对象的 storage duration 的分类有：

- <1> **automatic storage duration：** 非 `static` 的局部对象，位于 `STACK`
- <2> **static storage duration：** 全局对象、`static` 局部对象、`static` 类成员对象，位于 `CODE AND DATA`
- <3> **dynamic storage duration：** `new` 出来的对象，位于 `HEAP`

!!! note "`Global` 对象可以被其它文件以 `extern` 关键字声明引入；如果该全局对象是 `static` 的则不行"

成员变量的构造函数会比自己的构造函数更先调用。对于带有基类的对象，则首先会先调用基类的构造函数：

```c++
#include <iostream>

struct X {
    X() {
        std::cout << "X::X()" << std::endl;
    }
    ~X() {
        std::cout << "X::~X()" << std::endl;
    }
};

struct Y {
    Y() {
        std::cout << "Y::Y()" << std::endl;
    }
    ~Y() {
        std::cout << "Y::~Y()" << std::endl;
    }
};

struct Parent {
    Parent() {
        std::cout << "Parent::Parent()" << std::endl;
    }
    ~Parent() {
        std::cout << "Parent::~Parent()" << std::endl;
    }
    X x;
};

struct Child : public Parent {
    Child() {
        std::cout << "Child::Child()" << std::endl;
    }
    ~Child() {
        std::cout << "Child::~Child()" << std::endl;
    }
    Y y;
};

int main() {
    Child c;
}
```

尝试思考一下输出结果是什么吧。

另外，构造函数的 Initializer List 中的顺序并不会影响初始化的顺序，这个顺序实际由成员变量在类中定义的顺序(**Declaration Order**)决定：

```c++
#include <map>
#include <iostream>

class axis
{
public:
    axis(int as_)
    {
        as = as_;
        std::cout
	        << "axis::axis() is called with as = "
	        << as << std::endl;
    }
	axis(const axis& w)
    {
        as = w.GetAxis();
        std::cout
	        << "axis::CopyCtor is called with as = "
	        << as << std::endl;
    }
    int GetAxis() const { return as; }
private:
    int as;
};

class point
{
public:
    point(axis xa, axis ya)
        : y(ya), x(xa) {}
private:
    axis x;
    axis y;
};

int main()
{
    axis x(1), y(2);
    point p(x,y);
}

// ================= OUTPUT =======================
// default ctors
axis::axis() is called with as = 1
axis::axis() is called with as = 2

// copy ctors
/*
* 参数传参导致拷贝发生，由于入栈顺序，右侧 ya 的拷贝先一步发生
* 因此这里会出现先 2 后 1
* 更推荐的方式是这里使用引用，避免拷贝发生
*/
axis::CopyCtor is called with as = 2
axis::CopyCtor is called with as = 1

// 尽管 Initializer List 的顺序是先 ya 再 xa，但是实际执行仍然是先 xa 再 ya
axis::CopyCtor is called with as = 1
axis::CopyCtor is called with as = 2
```

#### 运算符重载 return 的临时变量声明周期

```c++
Matrix m = m1 - m2;
```

根据我们之前所说，`m1 - m2` 是通过运算符重载 return 得到的一个临时对象，这个临时对象会在所在表达式结束时被销毁。而在这个语句中，我们又用这个临时对象构造了一个新的 `Matrix` 对象 `m`。这有些浪费——我们析构了一个对象，同时构造了一个跟它一模一样的对象；如果我们能够延长这个临时对象的生命周期，就可以节约一次构造和一次析构的开销。

因此，C++ 规定：可以将一个临时对象绑定给一个 `const` 引用，这个临时对象的生命周期被延长以匹配这个 `const` 引用的生命周期[class.temporary#6](https://timsong-cpp.github.io/cppwp/n4868/class.temporary#6)。例如：

```c++
int function()
{
	// temporary `Matrix` has same lifetime as `m`
	const Matrix & m = m1 - m2;
	...
    // at the end of this function block, the lifetime of `m` ends,
    // so the lifetime of temporary `Matrix` ends, d'tor called.
}
```

### Static Member

!!! info "static function"
	在多文件编程中，`static` 属性一般用来声明该函数的作用域，表示该函数只能在该文件中使用

在类中的 static 成员变量 **只是声明** [class.static.data#3](https://timsong-cpp.github.io/cppwp/n4868/class.static.data#3)。也就是说，我们必须在类外给出其定义，才能让编译器知道在哪里构造这些成员：

```c++
// in .hpp
class Foo {
    static int a;
    static int b;
};

// in .cpp
int Foo::a = 1;
int Foo::b; // 初始化为 0
```

这一要求的动机是，我们通常会把类的定义放到头文件中，而头文件通常会被多个翻译单元（多个源文件）包含；如果 `static` 成员变量在类中定义，这样多个翻译单元就会有多个这个静态变量的定义，因此链接就会出错。

不过有一个例外，如果该静态成员变量同时也是一个 `const` 整数，那么它可以有初始值:

```c++
struct Foo {
    const static int i = 1; // OK
};

int main() {
    cout << Foo::i << endl;
}
```

在 C++17 之后，也可以为其添加 `inline` 属性，编译器会将多个定义合并为一个，从而避免重复定义：

```c++
struct Foo {
    inline static int i = 1; // OK since C++17
}
```

??? quote "关键字 `inline`"
	`inline` 本身用作 `macro` 更安全的替代使用，它以增大 code size 作为代价减少了函数调用的 cost。通常，我们对那些只有 2-3 行的小函数、调用频繁的函数添加 `inline` 关键字。注意不要对递归函数添加。
	
	C++ 标准规定，凡是在类内定义直接给出函数体的成员函数，都会隐式带有 `inline` 属性。
	
	不过现在编译器会自动判断某些函数是否要 inline，就算你添加了 `inline` 也不一定接受你的要求。`inline` 关键字被更多地用于指示 "Multiple Definitions are permitted"。

- `const` 对象除了可以调用 `const member function`，还可以调用 `static member function`
- 静态成员函数也会和其它成员函数一起 Overload

```c++
class A {
public:
	static void f(double) { cout << "A" << endl; }
	void f(int) { cout << "B" << endl; }
};

int main()
{
	const A a;
	a.f(3);    // A
	A b;
	b.f(3.1);  // A
}
```

## Template

下面是一个使用模板定义的类，当需要实例化时，需要给出对应的模板参数：

```c++
template<typename T>
class Container {
    T* data;
    unsigned size, capa;
public:
    Container(unsigned capa = 512) : data(new T[capa]) {}
    ~Container() { delete[] data; }
    T& operator[](unsigned index) { return data[index]; }
    // ...
};

Container<int> ci;
Container<double> cd;
```

虽然模板实例化必须知道每个模板参数，但是在调用者没有指定的情况下编译器会从函数参数中推断出没有给出的模板参数，这一机制叫做**模板参数推导 (template argument deduction)**：

```c++
// template definition: abs is a function template
template<typename T> T abs(T x) { return x > 0 ? x : -x; }

int main() {
    // instantiates and calls abs<int>(int)
    int x = abs<int>(-1);
    // instantiates and calls abs<double>(double), template argument deduced
    double y = abs<>(-1.0);
    // instantiates and calls abs<float>(float), template argument deduced
    float z = abs(-2.0f);
}
```

对于多个模板参数，也可以只给出前几个参数，后面的由编译器推导：

```c++
template<typename To, typename From>
To convert(From f);

void g(double d) 
{
    int i = convert<int>(d);    // calls convert<int, double>(double)
    char c = convert<char>(d);  // calls convert<char, double>(double)
    int(*ptr)(float) = convert; // instantiates convert<int, float>(float) 
                                // and stores its address in ptr
}
```

### lambda 表达式

!!! quote "对于所有“能单独定义一个函数”的地方，都可以用 lambda 函数来代替，可内联优化、无虚调用开销"

`std::sort` 函数可选的第三个参数可以传入我们自定义的比较函数，例如：

```c++
using std::vector
using std::sort
using std::endl

bool cmp(const int& a, const int& b) { return a > b; }
vector<int> v = {3, 1, 4, -2, 5, 3};
sort(v.begin(), v.end(), cmp);
```

使用 lambda 表达式，则可以在不引入额外的比较函数的情况下传入比较函数：

```c++
vector<int> v = {3, 1, 4, -2, 5, 3};
sort(v.begin(), v.end(), [](const int& a, const int& b) { return a > b; });
```

lambda 表达式形如 `[](){}`，其中 `()` 中是参数列表，`{}` 中是函数体，返回值类型由编译器根据 `return` 语句推断。

```c++
// std::count(first, last, p)
// 在 [first, last) 范围内查找满足谓词 p 的个数
int n = std::count_if(v.begin, v.end(), [](int i) { return i % 4 == 0; });

// std::for_each(first, last, f)
// 在 [first, last) 范围内依次执行函数 f，本例让 v 所有元素变为两倍
std::for_each(v.begin(), v.end(), [](int &n){ n *= 2; });
```

```c++
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "Original vector: ";
    for (int elem : v)
        std::cout << elem << ' ';

    auto it = std::partition(v.begin(), v.end(), [](int i){
        return i % 2 == 0;
    });

    std::cout << "\nPartitioned vector: ";
    auto print = [](int x){ std::cout << x << ' '; };
    std::for_each(v.begin(), it, print);
    std::cout << "* ";
    std::for_each(it, v.end(), print);
}

/* Output:
Original vector: 0 1 2 3 4 5 6 7 8 9 
Partitioned vector: 0 8 2 6 4 * 5 3 7 1 9
*/
```

每个 lambda 表达式都具有独立且唯一的类型，称为**闭包类型(Closure Type)**，在声明时只能使用 `auto`，且没有 `operator=` 等操作符：

```c++
auto p = [](int x) { return x * x; };
auto q = [](int x) -> int { return x + x; };
q = p; // error: no viable overloaded '='

// 没有捕获的 lambda 可以隐式转换为函数指针
int y = 20;
void (*foo)(int) = [](int x) { std::cout << x; }; // OK
void (*bar)() = [y] { std::cout << y; }; // error: no viable conversion
void (*baz)() = [=] {}; // error: no viable conversion
```

`[]` 是 lambda 函数的捕获列表，定义了从 lambda 函数体之内访问的外部变量，例如：

```c++
// calculate sum
int sum = 0;
std::for_each(v.begin(), v.end(), [&sum](int x) {
    sum += x;
});
```

注意**捕获**分为按值捕获和按引用捕获两种方式，区别在于按值捕获拷贝了外部变量的值，lambda 内部得到的是一个副本：

```c++
int x = 10;

auto by_value = [x]() { std::cout << "value: " << x << "\n"; };
auto by_ref   = [&x]() { std::cout << "ref: " << x << "\n"; };

x = 20;

by_value();  // 输出：value: 10   ← 捕获时拷贝了 x=10
by_ref();    // 输出：ref: 20     ← 捕获的是引用，x 后来变了
```

一般来讲，按值捕获不能修改外部变量的值。但是可以使用关键字 `mutable` 声明其可修改，并且每次调用时共享该副本值的状态变化：

```c++
int x = 5;

auto f = [x]() mutable {
    x += 10;
    std::cout << "x in lambda = " << x << "\n";
};

f();        // x in lambda = 15
std::cout << x << "\n"; // 5
f();        // x in lambda = 25
```

??? note "只有局部变量需要捕获，全局变量、静态变量、类成员变量不需要捕获"
	```c++
	[&] {}; // 默认按引用捕获
	[=] {}; // 默认按值捕获
	[x] {}; // 捕获 x 的值
	[&x] {}; // 捕获 x 的引用
	[&, x] {}; // 默认按引用捕获，但 x 按值捕获
	[=, &x] {}; // 默认按值捕获，但 x 按引用捕获
	[&, &i] {}; // 错误：默认已经是按引用捕获
	[i, &] {}; // 错误：& 或 = 必须在最前
	[i, i] {}; // 错误：重复捕获
	[i](int i) {}; // 错误：参数 i 与捕获的 i 重名
	```

## Exceptions

C++ 程序在编译时通过静态类型和编译检查能够发现大部分语法或类型错误，但是运行时仍然会发生各种不可预见的错误。为了避免程序把所有潜在错误都写成多分支返回的错误码形式，C++引入了异常机制，将错误处理和正常逻辑分离，提升代码可读性和可维护性。

接下来，我们尝试为一个自定义的 Vector 类设计 Exception。首先需要定义一个表示 ERROR 的类：

```c++
class VectorIndexError {
public:
	VectorIndexError(int v) : m_badValue(v) { }
	~VectorIndexError() { }
	void diagnostic() {
		cerr << "index " << m_badValue
			 << "out of range!";
	}
private:
	int m_badValue;
};

template <class T>
T& Vector<T>::operator[](int idx){
	if (idx < 0 || idx >= m_size)
		throw VectorIndexError(idx); // the data object
	return m_elements[idx];
}
```

在 Caller 处，我们通过 `try {...} catch {...}` 块来捕获这个异常，并对其进行特殊处理：

```c++
void outer() {
	try {
		func();
		func2();
	} catch (VectorIndexError& e) {
		e.diagnostic();
		// This exception does not propagate
	} catch (...) {
		// all other exception
		// handler code
	}
	cout << "Control is here after exception";
}
```

一种更优秀的设计是利用继承，设计多层次的 Exception Type：

```c++
class MathErr {
	...
	virtual void diagnostic();
};

class OverflowErr : public MathErr { ... }
class UnderflowErr : public MathErr { ... }
class ZeroDivideErr : public MathErr { ... }

try {
	...
} catch (MathErr& m) {
	m.diagnostic();
}
```

!!! tip "`noexcept`"
	对于一些函数，我们不希望其能够抛出异常，以使其更高效地运行，例如 dtor、move ctor 等。此时，我们需要为其添加 `noexcept` 修饰符，当异常被抛出时，程序直接 `std::terminate` 终止：
	
	```c++
	void abc(int a) noexcept { ... }
	```

!!! info "如果抛出的异常没有被 Catch，也会直接调用 `std::terminate`"

## NEW vs MALLOC

- `new`, `delete` 是C++的关键字；`malloc`, `free` 是库函数，需要引入相应头文件才可使用
- `new` 分配的空间在自由存储区；`malloc` 分配的空间在堆空间
	- 自由存储区可以是堆、全局/静态存储区等
- `new` 操作符返回类型与对象严格匹配；`malloc` 返回 `void*` ，需要手动强制转换
- `new` 分配失败返回 `std::bad_alloc` 异常；`malloc` 分配失败返回 `NULL`
- `new` 和 `delete` 会正确调用对象的 `Ctor` 或 `Dtor`

!!! info "`new` 操作符的底层通常也是用 `malloc` 实现的"

!!! danger "不要对同一块内存 `delete` 两次；但是对 `nullptr` 使用 `delete` 是安全的"

## Smart Pointers

手动使用 `new` 和 `delete` 常常导致内存泄漏或悬挂指针，智能指针的 `smart` 体现在：

- **自动回收资源:** 在生命周期结束后智能指针自动释放资源
- **正确调用:** 根据指针类型，自动选择 `delete` 或 `delete[]`
- **额外限制:** 例如强制要求智能指针不为空

---

- `std::unique_ptr` 假设自己是对象的唯一所有者，它会在自己析构时自动调用合适的 `delete`
	- `unique_ptr` 类中拷贝构造和拷贝赋值函数均被标记为 `delete`，即它不能被拷贝或赋值；但是我们可以通过 `std::move` 转移所有权
	- 如果一个类中含有非静态的不可拷贝成员，这个类也无法被拷贝

```c++
#include <iostream>
#include <memory>

struct Foo {
    Foo() { std::cout << "Foo created\n"; }
    ~Foo() { std::cout << "Foo destroyed\n"; }
    void say() { std::cout << "Hello from Foo\n"; }
};

int main() {
    std::unique_ptr<Foo> p1 = std::make_unique<Foo>();
    p1->say();

    // std::unique_ptr<Foo> p2 = p1;      ❌ 错误，不能复制
    std::unique_ptr<Foo> p2 = std::move(p1);  // ✅ 转移所有权

    if (!p1) std::cout << "p1 is now null\n";
}
```

- 多个 `std::shared_ptr` 可以共享同一个对象，其内部会维护引用计数，当最后一个 `shared_ptr` 被销毁时，资源才被释放
	- 可以被浅拷贝，被拷贝的值指向同一个对象
	- 接受右值引用 `unique_ptr`，将独占所有权转换为共享所有权

```c++
#include <iostream>
#include <memory>

struct MyClass {
    MyClass()  { std::cout << "MyClass created\n"; }
    ~MyClass() { std::cout << "MyClass destroyed\n"; }
};

int main() {
    std::unique_ptr<MyClass> uptr = std::make_unique<MyClass>();

    std::shared_ptr<MyClass> sptr = std::move(uptr);  // 转移所有权到 shared_ptr

    std::cout << "uptr is " << (uptr ? "not null" : "null") << "\n";
    std::cout << "sptr use_count: " << sptr.use_count() << "\n";
}
```

!!! danger "Double Free"
	```c++
	T* p = ...;
	std::shared_ptr<T> a(p);
	std::shared_ptr<T> b(p);
	// runtime error: double free
	// =============================
	auto a = std::make_shared<T>();
	std::shared_ptr<T> b(a.get());
	// runtime error: double free
	// =============================
	auto a = std::make_shared<T>();
	std::shared_ptr<T> b(a);
	std::shared_ptr<T> c;
	c = b;
	// Good
	```

## Some STL..

### STACK

基本操作：使用 stack 来匹配括号

```c++
#include <iostream>
#include <stack>
#include <string>

using namespace std;

bool is_balance(string s)
{
    stack<char> st;

    for (char c : s)
    {
        if (c == '(' || c == '[' || c == '{')
            st.push(c);
        else if (c == ')' || c == ']' || c == '}')
        {
            if (st.empty())
                return false;
            char top = st.top();
            st.pop(); // pop 是 void，不返回 pop 出的元素
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{'))
                return false;
        }
    }
    return st.empty();
}

int main()
{
    string a = "a{b(c[d]e)f}";
    string b = "a{b(c[d]e)f";

    cout << "a: " << (is_balance(a) ? "Balanced" : "Not Balanced") << endl;
    cout << "b: " << (is_balance(b) ? "Balanced" : "Not Balanced") << endl;
}
```

实际上，在已有其他容器（比如 `vector`，`deque`）等容器的前提下，我们很容易就能写出自己的一个 `stack`：

```c++
template <typename T>
class Stack
{
public:
    virtual T& top() = 0;
    virtual bool empty() = 0;
    virtual size_t size() = 0;
    virtual void push(T) = 0;
    virtual void pop() = 0;
};

template <typename T>
class VectorStack : public Stack<T>
{
public:
    T& top() override { return c.back(); }
    bool empty() override { return c.empty(); }
    size_t size() override { return c.size(); }
    void push(T t) override { c.push_back(t); }
    void pop() override { c.pop_back(); }
private:
    vector<T> c;
};
```

其中，上面的 `Stack` 类仅充当 adapter 作用，你也可以在下面写入基于其它结构实现的 stack 类。

### MAP

`map` 的下标引用会自动创建一个键值对，因此要防止乱用

```c++
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map <std::string, int> price;

    // 插入元素
    price["apple"] = 1;
    price["banana"] = 2;
    price["orange"] = 3;

    // 迭代遍历访问
    for (const auto& item : price) {
        std::cout << item.first << ": $" << item.second << std::endl;
    }

    std::string item;
    int total = 0;
    while(std::cin >> item) {
        if (item == "quit")
            break;
        // 如果 item 不在 map 中，price[item] 会插入一个默认值，造成污染
        if (price.find(item) != price.end())
            total += price[item]; 
        else
            std::cout << "Item not found." << std::endl;
    }

    std::cout << "Total price: $" << total << std::endl;
}
```

