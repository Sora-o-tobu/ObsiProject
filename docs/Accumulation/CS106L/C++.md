
# A brief introduction of c++

- C++ 向后兼容低级语言，如使用 `asm(...);` 来执行汇编语言
- C++23 指的是23年
- C++ 编程范式
	- Object-Oriented Programming
	- Procedural Programming
	- Generic Programming

## C++ Basics

The first program in C++:

```c++
#include <iostream>

int main()
{
	std::out << "Hello, World!" << std::endl;
	return 0;
}
```

C++标准模板库 Standard Template Library, **STL** 是一套功能强大的C++模板类和函数的集合，它提供了一系列通用的、可复用的算法和数据结构，其核心包括以下几个重要组件：

| 组件                    | 描述                                                                                                                        |
| --------------------- | ------------------------------------------------------------------------------------------------------------------------- |
| 容器 Containers         | 容器是 STL 中最基本的组件之一，提供了各种数据结构，包括向量（vector）、链表（list）、队列（queue）、栈（stack）、集合（set）、映射（map）等                                     |
| 算法 Algorithms         | STL 提供了大量的算法，用于对容器中的元素进行各种操作，包括排序、搜索、复制、移动、变换等。这些算法在使用时不需要关心容器的具体类型，只需要指定要操作的范围即可                                         |
| 迭代器 Iterators         | 迭代器用于遍历容器中的元素，允许以统一的方式访问容器中的元素，而不用关心容器的内部实现细节。STL 提供了随机访问迭代器、双向迭代器、前向迭代器和输入输出迭代器等多种类型的迭代器                                 |
| 函数对象 Function Objects | 函数对象是可以像函数一样调用的对象，可以用于算法中的各种操作。STL 提供了一元函数对象、二元函数对象、谓词等多种函数对象                                                             |
| 适配器 Adapters          | 适配器用于将一种容器或迭代器适配成另一种容器或迭代器，以满足特定的需求。STL 提供了栈适配器（stack adapter）、队列适配器（queue adapter）和优先队列适配器（priority queue adapter）等多种适配器 |

C++ 还引入了命名空间概念作为附加信息以区分不同库中的同名函数、类、变量等。本质上，命名空间就是定义了一个范围。而 `std` 为 STL 的命名空间。

在某些简单代码中我们能够看到 `using namespace std;` 指令，来指定后续的代码都使用 `std` 作为命名空间，但这不是一个好的代码习惯，我们要尽量避免。

!!! note "使用 `using namespace std::cout` ，则接下来只有 `cout` 指令前面不需要加命名空间"

## Types

```c++
#include <string>

int val = 5;         // 4 bytes = 32 bits (usually)
char ch = 'F';        // 1 byte  = 8 bits  (usually)
float dVal1 = 5.0;   // 4 bytes = 32 bits
double dVal2 = 5.0;  // 8 bytes = 64 bits
bool bVal = true;    // 1 bit
std::string str = "Nimisora"; // C++ 字符串末尾不用 '\0'
```

C++ is a statically typed language, 即任何变量、函数在运行前都会获得一个 Types。

!!! tip "Python就属于Dynamically Typed，其变量和函数的类型都视当前值而定"
	这也是因为Python属于解释型语言，其运行时会一行一行读取并确定类型；而C++属于编译型语言，其类型需要在编译时就确定。

在 C++ 中，你不能重复定义多个完全相同的函数，不过当我们需要一个同样或类似功能的函数的多个版本的时候，可以通过更改函数类型或传入的参数个数来实现 **Overload** 达成多版本：

```c++
int half(int x)
{
	cout << "1" << endl;
	return x/2;
}
double half(double x)
{
	cout << "2" << endl;
	return x/2;
}
int half(int x, int divisor) // default parameter values
{
	cout << "3" << endl
	return x/divisor;
}

half(3);     // use version 1, return 1 
half(3.0);   // use version 2, return 1.5
half(3, 3);  // use version 3, return 1
```

除此之外，C++还支持 `auto` 关键字，它会让编译器自动识别该变量或函数的类型。

那么，当我们对一个变量赋值其他类型的值呢？

当我们尝试直接初始化，程序并不会报错，只是会按照一定规则将初始值转换为对应类型的值：

```c++
int numOne = 12.5;
int numTwo(12.5);

cout << "numOne is: " << numOne << endl;
cout << "numTwo is: " << numTwo << endl;

/*Output:
numOne is: 12
numTwo is: 12
*/
```

在 C++11 后，可以使用花括号进行 *Uniform Initialization* ，这将告诉编译器注意初始值和变量的类型：

```c++
int numOne = {12.5};
// or int numOne{12.5};

/*Error Message:
错误：narrowing conversion of ‘1.25e+1’ from ‘double’ to ‘int’ [-Wnarrowing]
    |    int num = {12.5};
*/
```

!!! warning "花括号的意义在于更好的安全性"

除了上述使用方法外，花括号在 C++ 赋初值中无处不在，例如下列代码实现对 `map` 和 `Vector` 的 Uniform Initialization：

```c++
#include <iostream>
#include <map>

int main() {
	// uniform initialization of map & vector
	std::map<std::string, int> ages {
		{"Amahamiu", 18},
		{"Inuisana", 17},
		{"Nimisora", 16}
	};
	std::vector<int> numbers = {1,2,3,4,5};

	// Accessing map elements
	std::cout << "Amaha's age: " << ages["Amahamiu"] << endl;
	std::cout << "Inui's age: " << ages.at("Inuisana") << endl;

	// Accessing vector elements
	for (int num : numbers)
		std::cout << num << " ";

	return 0;
}
```

!!! tip "`typename`"
	关键字 `typename` 用于向编译器表明某个标识符是一个类型，而不是变量或其它实体，它主要应用在模板编程中。例如：
	
	```c++
	template<class ForwardIt>
	ForwardIt min_elem(ForwardIt first, ForwardIt last)
	{
	  return min_elem(first, last, std::less<typename  std::iterator_traits<ForwardIt>::value_type>());
	}
	
	template<class ForwardIt, class Compare>
	ForwardIt min_elem(ForwardIt first, ForwardIt last, Compare comp)
	{
	  if (first == last)
	    return first;
	
	  ForwardIt smallest = first++;
	  for (; first != last; ++first) {
	    if (comp(*first, *smallest)) {
	      smallest = first;
	    }
	  }
	  return smallest;
	}
	```

**Named Cast** 是 C++ 支持的类型转换，四种分别有不同的用途：

| 转换类型                     | 作用与用途                                   | 是否有运行时检查 | 可进行的转换示例                                                                                    | 使用场景与注意点                                                        |
| ------------------------ | --------------------------------------- | -------- | ------------------------------------------------------------------------------------------- | --------------------------------------------------------------- |
| `static_cast<T>(…)`      | ✅ 编译期类型检查✅ 用于相关类型之间的安全转换（非多态）           | ❌ 无运行时检查 | 基本类型间（如 `int`→`double`）类层次无多态关系时向上/向下转换                                                     | 用于非多态的指针/引用转换，或基本类型间。下行转换需谨慎，无法检查目标是否真实对象。                      |
| `dynamic_cast<T>(…)`     | ✅ 支持多态类型安全下行转换（需基类有 `virtual`）✅ 运行时类型检查 | ✅ 有运行时检查 | ```c++<br>Base* b = …;<br>Derived* d = dynamic_cast<Derived*>(b);<br>```                    | 多态场景下想从基类指针/引用安全转为派生类。失败返回 `nullptr`（指针）或抛 `std::bad_cast`（引用）。 |
| `const_cast<T>(…)`       | ✅ 去除或添加 `const`/`volatile` 修饰           | ❌ 无运行时检查 | ```c++<br>const int* p = …; <br>int* q = const_cast<int*>(p);<br>```                        | 用于修改掉对象的常量属性；若目标本身真为常量，修改会造成未定义行为。                              |
| `reinterpret_cast<T>(…)` | ✅ 进行“最低层”比特重解释转换，不改变比特模式                | ❌ 无运行时检查 | ```c++<br>int* p;<br>char* c = reinterpret_cast<char*>(p);<br>```<br><br>或者从 `long` → 指针 等。 | 用于底层指针与整数之间、不同指针类型之间按位重解释。几乎不安全，仅在极端场景（如硬件映射、序列化）下使用。           |

**【23-24 Final】** 下面哪个代码会编译错误？**D**

```c++
// A. static_cast 可以用在基类和子类的互相转换，无编译错误，但是实际运行时有可能出错
struct U {};
struct V : public U {};
struct W : public U {};
int main()
{
	U* p = new V;
	W* q = static_cast<W*>(p);
	return q == nullptr;
}

// B. dynamic_cast 只能用在基类和子类是多态的时候，即基类一定要有 virtual 函数。
// 但是实际上只要 dynamic_cast<target>(expression) 中 expression 是多态类型即可，只不过此例中 W 和 U 无继承关系，会返回空指针
struct U { virtual void foo() {} };
struct V : public U {};
struct W {};
int main()
{
	U* p = new V;
	W* q = dynamic_cast<W*>(p);
	return q == nullptr;
}

// C. 理由同上，但是此例不返回空指针
struct U { virtual void foo() {} };
struct V : public U {};
struct W : public U {};
int main()
{
	U* p = new V;
	W* q = dynamic_cast<W*>(p);
	return q == nullptr;
}

// D. static_cast 不允许用于转换两个不相关的指针（除了 void*）
struct U {};
struct V : public U {};
struct W {};
int main()
{
	U* p = new V;
	W* q = static_cast<W*>(p);
	return q == nullptr;
}
```

`dynamic_cast<target>(expression)` 在运行时会检查基类指针 `expression` 指向的对象是否是 `target` 或其子类，否则只会返回 `nullptr`。其最常见的用途是多态场景中判断一个基类指针/引用指向哪个派生类型：

```c++
Base* pb = new Derived();
if (dynamic_cast<Derived*>(pb))
	cout << "OK" << endl;
else
	cout << "NO" << endl;
```

也就是说，`expression` 的**动态类型**必须为 `target` 或其派生类。

## Structs

A struct is a a group of named variables, each with their own type, that allows programmers to bundle different types together.

```c++
#include <iostream>

using namespace std;

struct Student{
    string name;
    string state;
    int age;
};

void printStudentInfo(Student s){
    cout << s.name << " from " << s.state;
    cout << " (" << s.age << ")" << endl; 
}

int main(void)
{
    Student s = {"Nimisora", "CS", 17};
    printStudentInfo(s);
}
```

与 C 语言略有不同的是，C++的结构变量可以直接使用结构体名声明定义，而C需要加上 `struct` 。

众所周知结构体也常用作函数返回多个结果的中介，不过C++的STL中也存在内置的结构体可用来完成这一操作。如可以用来存储任意两种类型的模板结构体 `std::pair` :

```c++
struct pair{
	fill_in_type first;
	fill_in_type second;
};
```

配合函数 `std::make_pair` 可以很方便达成传递两个返回值：

```c++
std::pair<bool, Student> lookupStudent(string name)
{
	Student blank;
	if (notFound(name)) return std::make_pair(false, blank);
	Student result = getStudentWithName(name);
	return std::make_pair(true, result);
}

std::pair<bool, Student> output = lookupStudent("Inuisana");
// auto output = lookupStudent("Inuisana")  // More Neat!
```

实际上使用 **Uniform Initialization** 即可：

```c++
#include <cmath>    // for sqrt
#include <iostream> // to print
#include <utility>  // for pairs

std::pair<bool, std::pair<int, int>> solveQuadratic(int a, int b, int c) {
    auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return {false, {0, 0}};
    int root1 = (-b + sqrt(discriminant)) / (2 * a);
    int root2 = (-b - sqrt(discriminant)) / (2 * a);
    return {true, {root1, root2}};
}

int main() {
    auto result = solveQuadratic(1, -3, 2);
    if (result.first)
        std::cout << "Root 1: " << result.second.first << std::endl << "Root 2: " << result.second.second << std::endl;
    else
        std::cout << "No real roots" << std::endl;
}
```

> [std::pair - cppreference.com](https://en.cppreference.com/w/cpp/utility/pair.html) from `#include <utility>`

## References

使用 `&` 来设置一个变量的别名：

```c++
int num = 5;
int& ref = num;
ref = 10;
cout << num << endl; // Output: 10
```

!!! tip "Type Restrictions"
	- 引用不能绑定引用
	- 不允许引用 arrays
	- 不允许将引用作为指针，但是允许引用绑定指针，即
		- `int&* p = ...;` 是非法的
		- `void f(int*& p);` 是合法的
	- 不允许重新绑定

由于 Reference 本质是和源变量指向同一块堆栈地址，因此修改它也会对源变量的值一同修改。利用这个性质，我们可以将别名作为形参，以实现直接对别的作用域的变量修改：

```c++
#include <iostream>
#include <math.h>

void square(int& n) {
    n = std::pow(n, 2);
}

int main(void) {
    int num = 2;
    square(num);
    std::cout << num << std::endl; // Output: 4
}
```

!!! info "使用别名作为形参"
	使用别名作为形参意味着告诉编译器这个函数接收参数时是直接使用参数对应的内存地址，而不是将其值复制进一个临时地址。

!!! warning "非 `const` 引用的只能绑定到左值 lvalue"
	```c++
	void func(int&);
	func(i * 3); // Error!
	```
	
	lvalue 指有名字、可寻址的变量。想要让上述函数编译通过，可选用中间变量来传递 `i*3`，或者改为 `func(const int&);` 来绑定右值

那么注意观察，下面这个函数会修改 `nums` 中的值吗？

```c++
void shift(std::vector<std::pair<int, int>> &nums)
{
	for (auto [num1, num2] : nums){
		num1++;
		num2++;
	}
}
```

答案是不会的，因为 `for` 循环的取值也是仅复制其值，而不是在同一内存地址上进行操作，将其修改如下，则可以达到预期效果：

```c++
void shift(std::vector<std::pair<int, int>> &nums)
{
	for (auto& [num1, num2] : nums){
		num1++;
		num2++;
	}
}
```

值得注意的是，在不使用引用时，对于一个对象（在本例中为一个STL容器 `pair`），这种拷贝遍历将带来很大的额外开销。而使用引用则减少了不必要的开销。

!!! tip "如果循环中不需要对元素进行修改，请加上 `const`"

C++11 开始引入了右值引用 `T&&` ，它**只能**绑定到右值上，允许我们暂时保留这些临时对象的资源，并且不用付出和拷贝一样大的开销：

```c++
#include <iostream>
#include <string>

void printString(const std::string& s) {
    std::cout << "Lvalue reference: " << s << "\n";
}

void printString(std::string&& s) {
    std::cout << "Rvalue reference (moved): " << s << "\n";
}

int main() {
    std::string name = "Alice";

    printString(name);           // 调用 const lvalue reference 版本
    printString(std::string("Bob")); // 调用 rvalue reference 版本
    printString(std::move(name));    // 强制把 name 转成右值 → 调用 rvalue 版本
}

/*Output:
Lvalue reference: Alice
Rvalue reference (moved): Bob
Rvalue reference (moved): Alice
*/
```

| 表达式                  | 类型       | 绑定方式                     |
| -------------------- | -------- | ------------------------ |
| `name`               | 左值（变量）   | 绑定到 `const std::string&` |
| `std::string("Bob")` | 右值（临时对象） | 绑定到 `std::string&&`      |
| `std::move(name)`    | 强制右值     | 绑定到 `std::string&&`      |

## Const

`const` is a qualifier for objects that declares they cannot be modified.

> From [cppreference](https://en.cppreference.com/w/cpp/language/cv)

增加修饰词 `const` 的变量相当于常量，不能被修改，这也包括 reference。

并且，也不允许用非常量reference来引用常量：

```c++
const std::vector<int> const_vec{1, 2, 3}; // a const vector
std::vector<int>& bad_ref{ const_vec };  // bad reference
```

编译以下代码，会发生编译警告：

```c++
int x;
std::cin >> x;
const int size = x;
double classAverage[size]; // error
classAverage[x - 1] = x;
std::cout << classAverage[x-1] << std::endl;
```

```bash
➜  code g++ main.cpp -std=c++17 -pedantic -Wall
main.cpp: In function ‘int main()’:
main.cpp:20:16: warning: ISO C++ forbids variable length array ‘classAverage’ [-Wvla]
   20 |         double classAverage[size]; // error
      |                ^~~~~~~~~~~~
```

这实际是因为 C++ 不允许使用运行时变量声明数组的大小，即便这个变量被声明为 `const`；即 C++ 要求数组大小在编译时就能确定。

在更好的实践中，我们可以用 `new` 来动态分配数组，或者使用标准 STL：

```c++
// Method 1
double* classAverage = new double[x]; // 记得要 delete

// Method 2
std::vector<double> classAverage(x);
```

!!! info "实际编译时可以正常运行，因为编译器自动对 C++ 做了非标准扩展，从而支持 VLA 特性"

除此之外，还需要注意指针 `*` 和 `const` 之间的位置关系：

```c++
int a[] = {53,54,55};

int * const p = a; // p is const
*p = 20; // OK
p++;     // ERROR

const int *p = a;  // (*p) is const
*p = 20; // ERROR!
p++;     // OK

int const *p = a;  // (*p) is const
// 与上一个完全等价
```

一个字符串作为右值，通常自带 `const` 属性，但是编译器允许将字符串常量赋值给一个非 `const` 指针：

```c++
char* s = "Hello, world!"; // 可以移动，不能修改，= const char*
char s[] = "Hello, world!";// 不能移动，可以修改，= *const char
```



## Streams

> **Recommended Reading:** https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1204/lectures/types/types.pdf

Streams is a general input/output abstraction for C++.

- **Input Streams** 
	- A way to read data from a source
	- 从 `std::istream` 中继承
- **Output Streams**
	- A way to write data to destination
	- 从 `std::ostream` 中继承

C++的IO库分为三个头文件：

| 头文件        | 类型                                                                              |
| ---------- | ------------------------------------------------------------------------------- |
| `iostream` | istream 从流读取数据<br>ostream 向流输出数据<br>iostream 读写流                                |
| `fstream`  | ifstream 从文件读取数据<br>ofstream 向文件输出数据<br>fstream 读写文件                            |
| `sstream`  | istringstream 从string读取数据<br>ostringstream 向string输出数据<br>stringstream 读写string |

### iostream

- `cin`: Standard Input
- `cout`: Standard Output
- `cerr`: unbuffered error output
- `clog`: buffered error output

一个基本的 `iostream` 应用如下：

```c++
#include <iostream>

int main(){
    double pi;
    std::cin >> pi;            // console in
    std::cout << pi << "\n";   // console out
    return 0;
}
```

!!! info "`std::endl` 和 `\n` 的区别"
	`std::endl` 不仅告诉stream要结束这一行，还要求其立即将buffer中的数据输出显示，即 **flush** 。但是 flush 操作开销较大，所以当需要频繁输出时，不妨尝试一下使用 `\n`，并且C++会自己判断什么时候要进行 flush。

考虑以下程序：

```c++
#include <iostream>

int main(void) {
    double d1, d2;
    std::string name;
    std::cin >> d1;
    std::cin >> name;
    std::cin >> d2;

    std::cout << "d1: " << d1 << "\n" << "d2: " << d2 << "\n" << "name: " << name << "\n";
}
```

由于英文姓名之间存在空格，假定我们的输入为 `3.14 Miu Amaha 1.11` ，这将会产生意料之外的输出：

```c++
d1: 3.14
d2: 0
name: Miu
```

这是因为 `std::cin >> name` 是以空格和换行符为分隔。为了解决这个问题，在读取时不以空格作为字符串分隔，我们应使用 `std::getline()` 函数：

```c++
#include <iostream>

int main(void) {
    double d1, d2;
    std::string name;
    std::cin >> d1;
    std::getline(std::cin, name);
    std::cin >> d2;

    std::cout << "d1: " << d1 << "\n" << "d2: " << d2 << "\n" << "name: " << name << "\n";
}
/*
Input:
3.14
Amaha miu
1.11
----------
Output:
d1: 3.14
d2: 0
name:

*/
```

但是答案仍然不符合预期，这是因为 `getline` 识别了上一行的换行符，使得 `name` 的内容仅为 `\n` 。因此，在这种情况，我们通常连续使用两次 `getline` 来解决：

```c++
#include <iostream>

int main(void) {
    double d1, d2;
    std::string name;
    std::cin >> d1;
    std::getline(std::cin, name);
    std::getline(std::cin, name);
    std::cin >> d2;

    std::cout << "d1: " << d1 << "\n" << "d2: " << d2 << "\n" << "name: " << name << "\n";
}
/*
Input:
3.14
Amaha miu
1.11
----------
Output:
d1: 3.14
d2: 1.11
name: Amaha miu
*/
```

!!! warning "实际上将 `getline` 和 `std::cin` 组合使用是不被推荐的"

### sstream

`stringstream` 定义于头文件 `<sstream>` ，它其实是一个别名，具体定义如下：

```c++
typedef basic_stringstream<char> stringstream;
```

`sstream` 允许你将字符串作为输入/输出流来使用，也常用于处理字符串。

```c++
#include <iostream>
#include <sstream>

int main(){
    std::string initial_quote = "I hate python, but I love C++";
    
    // create a stringstream object with initial value
    std::stringstream ss;
    ss << initial_quote;
    // or you can try:
    // std::stringstream ss(initial_quote);

    // data destination
    std::string first, second, third, quote;

    ss >> first >> second >> third >> quote;
    std::cout << first << " " << second << " " << third << " " << quote << std::endl;
}
```

上述代码创建了一个 `stringstream` 类对象，并使用 `>>` 和 `<<` 将字符串按空格分隔并输出。

最后能够看到输出 `I hate python, but` ，可是如果我们希望能够不按空格分隔，一直读取到最后，该怎么办？

```c++
istream& getline(istream& is, string& str, char delim = '\n');
```

`getline` 函数可以读取一个 input stream `is` 直到遇见字符 `delim` ，并把结果存储在字符串 `str` 中。其中 `delim` 默认是 `\n` 。

让我们简单修改一下上述代码，就可以实现将 `but I love C++` 存储进字符串对象 `quote` 中了：

```c++
    ss >> first >> second >> third;
    std::getline(ss, quote);
    std::cout << first << " " << second << " " << third << " " << quote << std::endl;
/*Output:
I hate python,  but I love C++
---------------------------------
If we use: std::getline(ss, quote, 'C');
Then the output: I hate python,  but I love
*/
```

!!! info "使用 `ss.str()` ，可以将对象 `ss` 内部字符串输出"

### fstream

```c++
#include <fstream>

int main(){
	// if `output.txt` not exit, the program will touch it.
    std::ofstream ofs("output.txt");
    if (ofs.is_open())
        ofs << "Hello, CS106L" << "\n";
    ofs.close();

	// bad try
    ofs << "this will not get written";

	// ios::app: 表示添加数据，而不是覆盖数据
    ofs.open("output.txt",  std::ios::app);
    ofs << "It's open again!";
    return 0;
}

/*output.txt:
Hello, CS106L
It's open again!
*/
```

```c++
int inputFileStreamExample() {
    std::ifstream ifs("output.txt");
    if (ifs.is_open()) {
        std::string line;
        std::getline(ifs, line);
        std::cout << "Read from the file: " << line << "\n"; 
    }
    if (ifs.is_open()) {
        std::string lin2;
        std::getline(ifs, lin2);
        std::cout << "Read from the file: " << lin2 << "\n";
    }
    return 0;
}

/*Output:
Read from the file: Hello, CS106L
Read from the file: It's open again!
*/
```

!!! warning "`ofs.close`"
	在实际尝试中，我的第二行数据一直读不出来。经过尝试发现是上面那个输出数据到文件的程序在输入第二行数据后并没有关闭文件，所以导致调用这个函数时该文件只有一行数据。但是程序运行完后能够发现第二行数据还是写入的，这是因为程序会在结束前关闭所有打开的文件，而关闭时才将数据写入。

### Define Extractor

可以通过全局函数重载来自定义 Stream Extractor 的工作流程，例如：

```c++
istream& operator>>(istream& is, T& obj) {
	// specific code to read obj
	return is;
}
ostream& operator<<(ostream& os, const T& obj) {
	// specific code to write obj
	return os;
}

cin >> a >> b >> c; // ((cin >> a) >> b) >> c;
cout << a << b << c; // ((cout << a) << b) << c;
```

!!! note "为什么需要 return？因为要做到 chaining"



## Containers

Container is an object that allows us to collect other objects together and interact with them in some way.

STL 中包含许多类型的容器：

- `<array>` 定长数组容器
- `<vector>` 动态数组容器
- `<deque>` 双端队列容器
- `<list>` 双端链表容器
- `<forward_list>` 单向链表容器
- `<stack>` 栈容器适配器
- `<queue>` 队列容器适配器
- `<priority_queue>` 优先队列容器适配器
- `<set>` 集合容器，基于平衡二叉树
- `<unordered_set>` 无序集合容器，基于哈希表
- `<map>` 映射容器（键值对），基于平衡二叉树
- `<unordered_map>` 无序映射容器，基于哈希表

![[containeranditerator.png]]

其中 `set` 系列的迭代器只有 `const` 版本，即不能用来修改值，因为集合按照元素的值作了哈希/排序，随意修改会破坏原本的顺序。

`map` 系列虽然有 `mutable` 迭代器，但是只能用来修改 value，并不能修改 key

!!! quote "所谓 Random Access Iterator，指的就是能够在常数时间内移动到任一元素（指针形式）"

此处只先列出关于 `<vector>` 的基本用法，其它的用到再说。

```c++
#include <vector>
#include <iostream>

int main(void) {
    // create a vector object that holds integers
    std::vector<int> v0; // empty vector
    std::vector<int> v1(5); // 5 integers, all initialized to 0
    std::vector<int> v2(5, 2); // 5 integers, all initialized to 2
    std::vector<int> v3 = {1, 2, 3, 4, 5}; // 5 integers, initialized to 1, 2, 3, 4, 5

    // use push_back to add elements to the end of the vector
    v0.push_back(1);
    v0.push_back(2);
    std::cout << v0[1] << "\n"; // or you can use v0.at(1)

    // use erase to remove elements from the vector
    v3.erase(v3.begin() + 1); // remove the second element

    // use size() to get the number of elements in the vector
    int size = v1.size();

    // use iterator to access elements in the vector
    for (auto it = v3.begin(); it != v3.end(); ++it)
        std::cout << *it << " ";

    // use range-based for loop to access elements in the vector
    for (int i : v3)
        std::cout << i << " ";

    // use clear to remove all elements from the vector
    v3.clear();
}
```

以下程序利用嵌套 vector 打印乘法表：

```c++
#include <vector>
#include <iostream>
#include <bits/stdc++.h>

int main(void) {
    // create a vector object that holds integers
    std::vector<std::vector<int>> outer;

    for (int i = 1 ; i < 10 ; ++i )
    {
        std::vector<int> inner;
        for (int j = 1 ; j < 10 ; ++j )
            inner.push_back(i * j);
        outer.push_back(inner);
    }

    for (auto row : outer)
    {
        for (auto elem : row)
            // 设置宽度为 4 个字符对齐，位于 <bits/stdc++.h> 库中
            std::cout << std::setw(4) << elem;
        std::cout << "\n";
    }
}
```



## Iterators

In STL, each container has its own iterator, which can have different behavior.

迭代器类似于一种特殊的指针。

- **Initializing:** `iter = s.begin();`
- **Incrementing:** `++iter;`
- **Dereferencing:** `*iter;`
- **Comparing:** `iter != s.end();`

迭代器根据包含关系 RANDOM-ACCESS\[BIDIRECTION\[FORWARD\]\] 可以分为三类：

- **forward:** 只能对迭代器对象执行递增一操作 `iter++`
- **bidirection:** 可以递增，也可以递减 `iter--`
- **random-access:** 可以手动设置改变的值 `iter += 5`
	- 从而支持了 `iter1 - iter2` 操作

!!! info "C++17 新增了 Contiguous Iterator，它在 random-access 基础上要求逻辑相邻的元素在内存中物理相邻"

此处再次把上一节的图放在这：

![[containeranditerator.png]]

!!! note "迭代器使算法更加通用，例如 `sort` 函数支持对数组、vector等容器进行排序"
	- `sort` 函数传入的迭代器需要满足 Random-Access，因为其中会包含 `it1 - it2`，`it + n` 等操作
	- 但是 `std::lower_bound`，`std::find` 等函数传入的迭代器只要满足 `forward` 即可，但是如果支持 Random-Access，则时间复杂度可以从 $O(N)$ 降至 $O(\log N)$

??? example "另一张图"
	![[iteratorclass.png]]

```c++
#include <map>
#include <iostream>

int main(void) {
    std::map<int, int> map{{1,6}, {2,8}, {0,3}, {3,9}};

    for (auto iter = map.begin(); iter != map.end(); ++iter) {
        const auto& [key, value] = *iter;
        std::cout << iter->first << " " << iter->second << std::endl;
        std::cout << key << " " << value << std::endl;
    }
}
```

!!! quote
	- `begin();` 返回指向容器头的迭代器
	- `cbegin();` const begin()
	- `rbegin();` 反向迭代器，返回指向容器最后一个元素的逆序迭代器



## Classes

### Basic

Classes are user-defined types that allow a user to encapsulate data and functionality using member variables and member functions.

!!! tip "`class` 默认为 `private`；`struct` 默认为 `public`"

C++属于面向对象的语言，类是其核心特性，通常称为用户定义的类型。它是一种封装了数据和函数的组合，类中数据称为成员变量，函数称为成员函数。可以使用类作为模板创建具有相同属性和行为的多个对象。

```c++
class classname
{
Access specifiers:        // private/public/portected...
	Data members;         // 成员变量
	Member functions() {} // 方法
}; // end of class
```

一个 class 的设计可以分为四个部分：

- Constructor
- Private member function/varibles
- Public member function/varibles
- Destructor

其基本思路是将声明和定义放在 `.hpp` 文件中，具体实现放在 `.cpp` 文件中：

```c++
//lec7.hpp
#include <string>

class Student {
private:
    std::string name;
    std::string major;
    int age;

public:
    // default constructor
    Student();
    // parameterized constructor
    Student(std::string name, std::string major, int age);

    std::string getName();
    std::string getMajor();
    int getAge();
    std::string setName(std::string name);
    std::string setMajor(std::string major);
    int setAge(int age);

    // destructor
    ~Student();
};

//lec7.cpp
#include "lec7.hpp"
#include <string>

Student::Student () {
    name = "Nimisora";
    major = "CS";
    age = 17;
}

Student::Student(std::string name, std::string major, int age) {
    this->name = name;
    this->major = major;
    if (age > 0)
        this->age = age;
}

std::string Student::getName() {
    return name;
}

std::string Student::getMajor() {
    return major;
}

int Student::getAge() {
    return age;
}

std::string Student::setName(std::string name) {
    this->name = name;
}

std::string Student::setMajor(std::string major) {
    this->major = major;
}

int Student::setAge(int age) {
    if (age > 0)
        this->age = age;
}

Student::~Student() {
    // destructor
}

int main()
{
    return 0;
}
```

!!! success "All containers in STL are classes!!!"

如果你写过 Python 的话，应该会知道 Python 中类方法的定义的第一个参数常为 `self`。实际上，C++ 中非 `static` 的成员函数也隐式地将自己作为一个参数传入，即 `this`。例如：

```c++
void Point::print();
// can be regarded as
void Point::print(Point *this);

a.print();
// can be regarded as
Point::print(&a);
```

??? abstract "`::` 称为 Resolver，为作用域解析运算符"
	- 如果 `::` 前面不加东西，则显式声明调用全局命名空间中的对象或方法
	- 对于一个命名空间内的对象，默认优先访问局部/类作用域中的变量（如果存在）
	
	```c++
	void S::f() {
	    ::f();     // ✅ 调用全局函数 f()
	    ::a++;     // ✅ 访问全局变量 a
	    a--;       // ❓如果存在，则访问 Class Scope 中的 'a'
	}
	```

!!! tip "`static` 成员函数不能直接访问非 `static` 成员，并且不能使用 `this`"

另外，`const` 对象不能调用非 `const` 成员函数，它们都需要保持自身外部可见状态不变：

```c++
class MyClass {
public:
    void normalFunc() {
        std::cout << "I'm a non-const function\n";
    }
    void constFunc() const {
        std::cout << "I'm a const function\n";
    }
};

int main() {
    const MyClass obj;     // 👈 这是个 const 对象

    obj.constFunc();       // ✅ OK：const 成员函数，可以被 const 对象调用
    obj.normalFunc();      // ❌ Error：不能对 const 对象调用非 const 成员函数
}
```

但是在关键字 `mutable` 修饰下，我们允许 `const` 成员函数在保证外部可见状态不变的情况下，修改内部状态：

```c++
class S {
  mutable bool _is_cached = false;
  mutable int _cache;
public:
  int f() const {
    if (!_is_cached) {
      /* some very, very expensive operation */
      _is_cached = true;
      _cache = 42;
    }
    return _cache;
  }
};
```


### Inheritance

作为面向对象，类同样可以继承。 

- **多态 Polymorphism**: Different objects might need to have the same interface
- **拓展 Extensibility**: Inheritance allows you to extend a class by creating a subclass with specific properties

```c++
class Shape {
public:
    virtual double area() const = 0;
    // virtual 关键字声明虚函数，可以在派生类中覆盖重写。 const = 0 说明该函数不会修改类的成员变量，该函数无任何实现，是一个纯虚函数，要求所有派生类都要重写该函数。
};

class Circle : public Shape {
public:
    // constructor with initialization list
    Circle(double radius) : _radius(radius) {};
    // 由于基类虚函数有关键字 const, 所以派生类重写的函数也要有 const 关键字
    double area() const override {
        return 3.14159 * _radius * _radius;
    }

private:
    double _radius;
};
```

!!! info "`const` 用于成员函数时，表明该函数不会修改任何成员变量"
	对于不修改成员变量的函数，尽量都加上 `const` ，且其在 `cpp` 中的实现也要加上 `const` ，否则编译器有时会因为无法确定该函数是否会修改成员变量而报错。

| Specifiers  | In Same Class | In Derived Class | Outside Class |
| ----------- | ------------- | ---------------- | ------------- |
| `private`   | ✅             | ❌                | ❌             |
| `protected` | ✅             | ✅                | ❌             |
| `public`    | ✅             | ✅                | ✅             |

其中，Derived Class 虽然可以继承到 `private` 修饰的成员，但是并不能在其函数中直接访问。对此，可以将需要被访问的成员改为 `protected`，或者在父类中添加一个公有接口：

```c++
class Base {
private:
    int secret = 42;
public:
    int getSecret() const { return secret; }
};

class Derived : public Base {
public:
    void show() {
	    // ❌ 编译错误，private 无法访问
	    // std::cout << secret;
	    // ✅ OK，通过公有接口访问 private 成员
        std::cout << getSecret();
    }
};

```

!!! info "具体报错信息为"
	```bash
	main.cpp: In member function ‘void Derived::show()’:
	main.cpp:14:22: error: ‘int Base::secret’ is private within this context
	   14 |         std::cout << secret;
	      |                      ^~~~~~
	main.cpp:5:9: note: declared private here
	    5 |     int secret = 42;
	      |         ^~~~~~
	```

此外，对于不同的 **inheritance type**，基类 `A` 中的成员对子类 `B` 的权限表如下：

| Inheritance Type | pulic in A     | protected in A | private in A   |
| ---------------- | -------------- | -------------- | -------------- |
| `: private A`    | private in B   | private in B   | not accessible |
| `: protected A`  | protected in B | protected in B | not accessible |
| `: public A`     | pulic in B     | protected in B | not accessible |

假定我们有子类 `Circle` 和基类 `Ellipse`，对于下列代码：

```c++
Ellipse elly(20f, 40f);
Circle circ(60f);
elly = circ;
```

此处调用的是 `Ellipse::operator=`，因此只有被 `Circle` 继承的数据才会被 Copy；

```c++
Ellipse* elly = new Ellipse(20f, 40f);
Circle* circ = new Circle(60f);
elly = circ;
```

此处原始的 `elly` 就丢失了，`elly` 和 `circ` 都指向了同一个 `Circle` 对象。如果 `Ellipse` 中设置 `render()` 为 `virtual` 的话，调用 `elly->render();` ，会因为多态调用 `Circle::render();`。

```c++
void func(Ellipse& elly) { elly.render(); }

Circle circ(60f);
func(circ);
```

在继承中，引用的效果和指针类似，此处调用的函数仍然是 `Circle::render();`。

对于返回值，在 **Type Relaxation** 下，子类可以返回 Return Type 的 Subclass，但是只支持指针和引用：

| 函数签名类型      | 是否支持协变重写 | 注意点            |
| ----------- | -------- | -------------- |
| `T* func()` | ✅ 支持     | 子类可返回更具体指针类型   |
| `T& func()` | ✅ 支持     | 子类可返回更具体引用类型   |
| `T func()`  | ❌ 不支持    | 子类必须返回完全相同类型的值 |

```c++
class Expr {
public:
    virtual Expr* newExpr();
    virtual Expr& clone();
    virtual Expr self();     // ← 返回值是 **值类型**
};

class BinaryExpr : public Expr {
public:
    virtual BinaryExpr* newExpr();   // ✅ OK: 返回类型是 Expr* 的子类型
    virtual BinaryExpr& clone();     // ✅ OK: 返回类型是 Expr& 的子类型
    virtual BinaryExpr self();       // ❌ ERROR!
};

```

### Template

**【Theorem】**在有函数重载的时候，优先考虑是否有完全匹配的函数，找不到再考虑模板，还是找不到再考虑隐式类型转换。

模板是创建泛型类或函数的蓝图或公式。库容器，比如迭代器和算法，都是泛型编程的例子，它们都使用了模板的概念，例如 `vector<int>` 。模板函数在你对其实例化前都不会被编译，当你调用了这个函数的一个版本，编译器会生成一个专属的版本以供后续使用。

```c++
#include <iostream>
#include <string>

template <typename T> // <typename T=int> => define a default type
T const& max(T const& a, T const& b)
{
    return a < b ? b : a;
}


int main()
{
    int a = 7, b = 42;
    std::cout << "max(a, b): " << max(a, b) << std::endl;
    // or you can call max<int>(a, b)

    double x = 7.1, y = 42.2;
    std::cout << "max(x, y): " << max(x, y) << std::endl;
	// or you can call max<double>(x, y)

    return 0;
}
```

??? note "Constraints and Concepts in C++20"
	```c++
	template <typename T>
	concept Addaptable = requires(T a, T b) {
	    a + b;
	}; // if a+b can be compile, then `add` work
	template <typename T> requires Addaptable<T>
	T add(T a, T b) {
	    return a + b;
	}
	template <Addaptable T> // this shorthand also OK!
	T add(T a, T b) {
	    return a + b;
	}
	```

同样，我们可以使用模板对类在实例化时进行指定：

```c++
// lec8.hh
template <typename T> // also <class T>
class Container
{
public:
    Container (T val);
    T getVal();

private:
    T _val;
};
// ====================================
// lec8.cpp
#include "lec8.hh"

template <typename T> // also <class T>
Container<T>::Container(T val) {
    this->_val = val;
}

template <typename T> // also <class T>
T Container<T>::getVal() {
    return _val;
}

int main()
{
    Container<int> intContainer(7);
    Container<std::string> stringContainer("Hello, world!");
    return 0;
}
```

!!! danger "Template 不允许隐式的形式转换，因此请确保调用时参数类型对应"

静态对象只能使用静态接口，所以将变量或对象作为常量传入时需注意其在函数内部是否调用了非静态接口。可能的解决方法：

```c++
// .hh
class arr
{
public:
    arr(int size);
    int& findItem(int item);
    const int& findItem(int item) const;

private:
    std::vector<int> _arr;
    int _size;
};
// ====================================
// .cpp
int& arr::findItem(int item)
{
    for (auto& elem : _arr)
        if (elem == item)
            return elem;

    throw std::out_of_range("Item not found");
}

// define a const version
const int& arr::findItem(int item) const
{
    return const_cast<arr&>(*this).findItem(item);
}
```

除此之外，template 还可用作 default argument，例如：

```c++
template <class T, int bounds = 100>
class FixedVector {
public:
	FixedVector();
	T& operator[](int);
private:
	T elements[bounds]; // fixed-size array!
};

FixedVector<int, 50> v1;
FixedVector<int, 10*5> v2;
FixedVector<int> v3; // => FixedVector<int, 100>
```

## Operator Overload

C++ 允许在同一作用域中的某个**函数**和**运算符**指定多个定义，分别称为**函数重载**和**运算符重载**。

重载声明是指一个与之前已经在该作用域内声明过的函数或方法具有相同名称的声明，但是它们的参数列表和定义（实现）不相同。

当您调用一个**重载函数**或**重载运算符**时，编译器通过把您所使用的参数类型与定义中的参数类型进行比较，决定选用最合适的定义。选择最合适的重载函数或重载运算符的过程，称为**重载决策**。

!!! info "以下运算符不能被重载"
	```c++
	.   .*   ::   ?:
	sizeof   typeid
	static_cast   dynamic_cast
	const_cast   reinterpret_cast
	```

重载的运算符是带有特殊名称的函数，函数名是由关键字 operator 和其后要重载的运算符符号构成的。与其他函数一样，重载运算符有一个返回类型和一个参数列表。

```c++
Integer operator-() const {
	return Integer(-i);
}
...
z = -x; // z.operator=(x.operator-());
```

下面是一个实现通讯录的程序，要求两个 User 相加时，二人的通讯录都互相增加对方。

```c++
// user.h
#include <set>
#include <string>

class User {
public:
    // constructor
    User() { this->name = ""; };
    User(std::string name);

    // getter functions
    std::string getName() const;
    std::set<User>& getFriends();
    const std::set<User>& getFriends() const;

    // setter functions
    void setName(std::string name);

    // operator overload
    bool operator< (const User &b) const;
    // 能加 const 就加，不然报错
    void operator+ (User& b);
    
private:
    std::string name;
    std::set<User> friends;

};
// =============================================
// main.cpp
#include <iostream>
#include "User.h"

void User::operator+ (User &b){
    this->friends.insert(b);
    b.friends.insert(*this);
}

bool User::operator< (const User &b) const {
    return this->name < b.name;
}

void printFriends(const User& user) {
    std::cout << user.getName() << " is friends with: " << std::endl;
    for(auto& user : user.getFriends()) {
        std::cout << "  " << user.getName() << std::endl;
    }
}

int main() {
    // create a bunch of users
    User alice("Alice");
    User bob("Bob");
    User charlie("Charlie");
    User dave("Dave");
    User Inuisana;
    Inuisana.setName("Inuisana");

    // make them friends
    alice + bob;
    alice + charlie;
    dave + bob;
    charlie + dave;
    alice + Inuisana;

    // print out their friends
    printFriends(alice);
    printFriends(bob);
    printFriends(charlie);
    printFriends(dave);
    
    return 0;
}

/*Output:
Alice is friends with: 
  Bob
  Charlie
  Inuisana
Bob is friends with:
  Alice
  Dave
Charlie is friends with:
  Alice
  Dave
Dave is friends with:
  Bob
  Charlie
*/
```

!!! failure "`this` 是一个指针，还请不要忘记"

上面代码重载的 `+` 是一个成员函数，如果要使用非成员函数作为重载的话，请使用：

```c++
User& operator+(User& fir, User& sec) {
    fir.friends.insert(sec);
    sec.friends.insert(fir);
    return fir;
}
/*
In this case, you can:
	alice + bob;  // operator+(alice, bob)
	alice = alice + bob;
ALl OK!

全局 operator 往往还需要在类中设置为 friend，如：
friend User& operator+(User& fir, User& sec);
*/
```

??? note "原因是 Member Function 中 `this` 是作为一个隐式的参数传递的，因此也相当于两个参数"
	```c++
	// 以下几个运算符依赖 this，因此不能以非成员函数形式重载(就算是 friend)
	operater=, operator(), operator[], operator->, operator->*
	```

我们需要注意区分 `++` 和 `--` 的函数重载形式：

```c++
class Integer {
public:
	...
	Integer& operator++();   //prefix++
	Integer operator++(int); //postfix++
	Integer& operator--();   //prefix--
	Integer operator--(int); //postfix--
...
};

Integer x(5);
++x; // calls x.operator++();
x++; // calls x.operator++(0);
--x; // calls x.operator--();
x--; // calls x.operator--(0);
```

对于 postfix forms，它的重载函数接收一个 `int` 类型参数，编译器会自动将 `0` 作为一个参数传递进来。

除此之外，还有一种运算符叫做 Conversion Operator，它常用在隐式的形式转换中：

```c++
class Rational {
public:
	operator double() const {
		return numerator / (double)denominator;
	}
}

Rational r(1,3);
double d = 1.3 * r; // r => double
```

**【19-20 Final】** 下列程序的输出是什么？

```c++
#include <cstring>
#include <iostream>

using namespace std;

class Str {
  char m_s[10];
  char* m_p;
public:
  Str(char *s) { strcpy(m_s, s); m_p = m_s; }
  operator char*() { return m_p; }
  char* operator++(){ return ++m_p; } 
  char operator[](int i) { return m_s[i]; }
};

int main() {
  Str s("hello");
  cout << *s << endl;   // prints "h", 这里先将 s 转换为 m_p，再解引用 *s 得到第一个值
  ++s;
  cout << s[0] << endl; // prints 'h'
  cout << *s << endl;   // prints 'e'
  return 0;
}
```

## Special Member Functions

C++ 中一共有六种 SMF，这些成员函数会在编译的时候默认生成，我们并不需要手动生成它们。

```c++
class Widget {
public:
	Widget ();                          // default constructor
	Widget (const Wiget& w);            // copy constructor
	Wiget& operator= (const Widget& w); // copy assignment constructor
	
	~Widget ();                         // destructor
	
	Widget (Widget&& rhs);              // move constructor
	Widget& operator= (Widget&& rhs);   // move assignment constructor
};
```

!!! tip "默认构造函数并不会对成员变量进行初始化，它们会保持 `uninitialized`"

!!! note "拷贝构造函数传入的同一类型对象只能是引用，因为在按值传参的过程中本身就会调用拷贝构造函数"
	拷贝构造函数被调用的场合具体发生在：
	
	- **初始化:** 例如 `T t = t1;`，`T t(t1);` 等
		- 这两种分别叫做拷贝初始化和直接初始化，区别可以再往下看
	- **函数参数传递:** 例如 `f(t)`，其中 `void f(T t)`
	- **函数返回:** 例如函数 `T f()` 的返回语句 `return T;`
	
	默认的拷贝函数是直接将指针的值复制过来，属于 shallow copy，需要注意。

另外，在定义了其它任何构造函数的情况下，编译器不会自动生成默认构造函数：

```c++
struct S {
  S(const S &) { std::cout << "copy constructor\n"; }
} s; // error: no matching constructor for initialization of 'struct S'
```

SMF 第二条和第三条虽然作用都是复制对象，但是实现方式不同：

```c++
// call Copy Constructor
Widget widgetOne;
Widget widgetTwo = widgetOne;

// call Copy Assignment Constructor
Widget widgetOne;
Widget widgetTwo;
widgeTwo = widgetOne;
```

即初始化(initialization)的时机是**对象创建时**；赋值(assignment)的时机是**对象已存在后**。

如果我们不希望这些函数运作的话，可以使用关键字 `delete` 手动将其删去：

```c++
Wiget& operator= (const Widget& w) = delete;
```

`copy constructor` 的行为逻辑是将内部成员值一个一个复制过去，运行速度较慢；而 `move constructor` 的行为逻辑是将目的对象指向原先的内容，运行速度较快。

定义 `lvalue` 为既可以在等号左边，又可以在等号右边的对象；定义 `rvalue` 为只能出现在等号右边的对象，其不能被 `&` 引用，但是我们可以使用 `&&` 来将其作为临时对象引用它：

```c++
// lvalue reference:
void upload (Photo& pic);

int main() {
	Photo selfie = takePhoto();
	upload(selfie);
}

// rvalue reference:
void upload (Photo&& pic);

int main() {
	upload(takePhoto());
}
```

值得注意的是，当使用 `&&` 引用 `rvalue` 时，并不保证其最终处于合法的状态。

!!! tip "`lvalue` 指有名字、可寻址的变量"

同时，也不一定所有左值都可以出现在等号左边，例如对于 `const char name[] = "Sora";`，`name[0] = "Sana";` 是非法的。尽管 `name[0]` 是个左值，但它的 type 是 `const char`，这样的左值被称为 **non-modifiable lvalues**。

**【Example】**

```c++
class A
{
public:
  A(int i) : mi(i) {}
  A(const A& rhs) : mi(rhs.mi)
  {
    cout << "A::A(&)" << endl;
  }
  A& operator=(const A&rhs)
  {
    mi = rhs.mi;
    cout << "A::operator=()" << endl;
    return *this;
  }
protected:
  int mi;
};

class B : public A
{
	...
}

int main()
{
	B b(3, 4);

	// 这里只是一个引用操作，并不是复制，所以不会触发 operator=
	// 所以无输出
	A& ra = b;

	// operator= 运算符只有在对象已经存在时才会被调用
	// 这里触发的是拷贝构造器
	A a = b;
}
```

最后只会输出 `A::A(&)`

在语义上，`C c1(7)` 称为**直接初始化(direct initialization)**，编译器直接调用 `C(int)` 来构造 `c1`；`C c2 = 7` 称为**拷贝初始化(copy initialization)**，编译器需要接受一个非 `explicit` 的构造函数来进行**隐式转换**。二者都会调用 Copy Constructor。

```c++
#include <iostream>

class C {
public:
    explicit C(int) { std::cout << "i" << std::endl; }
    C(double) { std::cout << "d" << std::endl; }
};

int main() {
    C c1(7);   // i
    C c2 = 7;  // d
}
```

最后，由于在对象构造过程中虚函数表还未产生，所以构造函数不能被声明为 `virtual`，会发生编译错误；但是我们应该尽可能在多态场景中为析构函数声明 `virtual`，这样当我们通过一个基类指针去 `delete` 一个派生对象时，析构可以按照派生类->...->基类的顺序正确调用，保证销毁。