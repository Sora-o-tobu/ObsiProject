
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
int hald(int x, int divisor) // default parameter values
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

值得注意的是，如果不适用引用时，对于一个对象（在本例中为一个STL容器 pair），这种遍历方式实际上是在进行不断拷贝遍历，这将带来很大的额外开销。而使用引用则减少了不必要的开销。

!!! tip "如果循环中不需要对元素进行修改，请尽可能加上 `const`"

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

这是因为 `std::cin >> name` 同样是以空格和换行符为分隔。为了解决这个问题，在读取时不以空格为作为字符串分隔，我们应使用 `std::getline()` 函数：

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

`map` 系列虽然由 `mutable` 迭代器，但是只能用来修改 value，并不能修改 key

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


| Container      | Type of iterator |
| -------------- | ---------------- |
| Vector         | Random-Access    |
| Deque          | Random-Access    |
| List           | Bidirectional    |
| Map            | Bidirectional    |
| Set            | Bidirectional    |
| Stack          | No iterator      |
| Queue          | No iterator      |
| Priority Queue | No iterator      |

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

!!! info "WHY CONST???"
	迭代器的属性为 `const` ，这意味着我们不能通过迭代器修改容器内的值。不过上述代码中的 `const` 可以不用加，`auto` 会自动为其附加这个属性。

## Classes

Classes are user-defined types that allow a user to encapsulate data and functionality using member variables and member functions.

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
-----------------------------------
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
----------------------------------------------
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


## Operator Overload

C++ 允许在同一作用域中的某个**函数**和**运算符**指定多个定义，分别称为**函数重载**和**运算符重载**。

重载声明是指一个与之前已经在该作用域内声明过的函数或方法具有相同名称的声明，但是它们的参数列表和定义（实现）不相同。

当您调用一个**重载函数**或**重载运算符**时，编译器通过把您所使用的参数类型与定义中的参数类型进行比较，决定选用最合适的定义。选择最合适的重载函数或重载运算符的过程，称为**重载决策**。

重载的运算符是带有特殊名称的函数，函数名是由关键字 operator 和其后要重载的运算符符号构成的。与其他函数一样，重载运算符有一个返回类型和一个参数列表。

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
---------------------------------------------------
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
	alice + bob;
	alice = alice + bob;
ALl OK!
*/
```

以下运算符不能被重载：

- `.` 成员访问运算符
- `.*`, `->*` 成员指针访问运算符
- `::` 域运算符
- `sizeof` 长度运算符
- `? :` 条件运算符
- `#` 预处理符号

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

!!! note "拷贝构造函数传入的同一类型对象只能是引用，因为在按值传参的过程中本身就会调用拷贝构造函数"
	拷贝构造函数被调用的场合具体发生在：
	
	- **初始化:** 例如 `T t = t1;`，`T t(t1);` 等
	- **函数参数传递:** 例如 `f(t)`，其中 `void f(T t)`
	- **函数返回:** 例如函数 `T f()` 的返回语句 `return T;`

其中第二条和第三条虽然作用都是复制对象，但是实现方式不同：

```c++
// call Copy Constructor
Widget widgetOne;
Widget widgetTwo = widgetOne;

// call Copy Assignment Constructor
Widget widgetOne;
Widget widgetTwo;
widgeTwo = widgetOne;
```

如果我们不希望这些函数运作的话，可以使用关键字 `delete` 手动将其删去：

```c++
Wiget& operator= (const Widget& w) = delete;
```

`copy constructor` 的行为逻辑是将内部成员值一个一个复制过去，运行速度较慢；而 `mov constructor` 的行为逻辑是将目的对象指向原先的内容，运行速度较快。

定义 `lvalue` 为既可以在等号左边，又可以在等号右边的对象；定义 `lvalue` 为只能出现在等号右边的对象，其不能被 `&` 引用，但是我们可以使用 `&&` 来将其作为临时对象引用它：

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