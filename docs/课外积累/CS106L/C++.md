
# A brief introduction of c++

- C++ 向后兼容低级语言，如使用 `asm(...);` 来执行汇编语言
- C++23 指的是23年

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
char ch = 'F'        // 1 byte  = 8 bits  (usually)
float dVal1 = 5.0;   // 4 bytes = 32 bits
double dVal2 = 5.0;  // 8 bytes = 64 bits
bool bVal = true;    // 1 bit
std::string str = "Nimisora";
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
int hald(int x, ind divisor) // default parameter values
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


## References

使用 `&` 来设置一个变量的别名：

```c++
int num = 5;
int& ref = num;
ref = 10;
cout << num << endl; // Output: 10
```

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

答案是不会的，因为 `for` 循环的取值也是仅调用其值，而不是在同一内存地址上进行操作，将其修改如下，则可以达到预期效果：

```c++
void shift(std::vector<std::pair<int, int>> &nums)
{
	for (auto& [num1, num2] : nums){
		num1++;
		num2++;
	}
}
```

## Const

`const` is a qualifier for objects that declares they cannot be modified.

> From [cppreference](https://en.cppreference.com/w/cpp/language/cv)

增加修饰词 `const` 的变量相当于常量，不能被修改，这也包括 reference。

并且，也不允许用非常量reference来引用常量：

```c++
const std::vector<int> const_vec{1, 2, 3}; // a const vector
std::vector<int>& bad_ref{ cont_vec };  // bad reference
```

## Streams

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



