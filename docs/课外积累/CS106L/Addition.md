
# C++ 补充

## Class

### Forward Declaration

如果当前作用域没有名为 `identifier` 的类，那么形如 `class-key attr identifier ;` 的声明是一个 forward declaration[class.name#2](https://timsong-cpp.github.io/cppwp/n4868/class.name#2)。

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

当然，你也可以声明 struct X 为静态结构体。

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

### 运算符重载 return 的临时变量声明周期

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

### static 成员变量

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

2/25 进度：
https://xuan-insr.github.io/cpp/cpp_restart/8_stl/#83-%E8%BF%AD%E4%BB%A3%E5%99%A8%E4%BD%95%E5%BF%85%E6%98%AF%E8%BF%AD%E4%BB%A3%E5%99%A8