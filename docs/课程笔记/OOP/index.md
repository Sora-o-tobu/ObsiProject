
## Pre

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

友元函数能够访问类的私有成员：

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

根据我们之前所说，`m1 - m2` 得到一个临时对象，这个临时对象会在所在表达式结束时被销毁。而在这个语句中，我们又用这个临时对象构造了一个新的 `Matrix` 对象 `m`。这有些浪费——我们析构了一个对象，同时构造了一个跟它一模一样的对象；如果我们能够延长这个临时对象的生命周期，就可以节约一次构造和一次析构的开销[9](https://xuan-insr.github.io/cpp/cpp_restart/5_class_2/#fn:rvo)。

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
class Foo {
    static int a;
    static int b;
};

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

