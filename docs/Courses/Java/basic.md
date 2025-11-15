
# Java 基础

## Introduction

现代的编程语言几乎都是 C-like 语言，在语法上差异较小。语言的能力/适用领域主要由库和传统所决定，而 Java 适合于很多场合，不仅仅是服务器后端。

wk 老师的 PPT 这样推荐：

- 对计算机本身（体系结构、操作系统、编译）感兴趣 —> C
- 想编程解决手头的问题（统计、AI、桌面小程序）—> Python
- 有明确的需求（求职）—> 人家要什么学什么（PHP、JavaScript、C++）
- 还没想好 —> Java

!!! warning "企业中的“精通Java”通常指以 Java 为编程语言的全套体系，例如 Spring、Nginx 等"

Java 本身是一种**编译执行的、面向对象**的语言，编译出来的字节码通过 JVM 虚拟机执行，其诞生于 1995 年的 5 月 3 日。作为介绍，下面简单列出几点 Java 的关键设计理念：

- **跨平台与可移植性**
    - Java 延续了 UNIX 和 C 想实现的“可移植”梦想。
    - 通过 **JVM（Java 虚拟机）** 来实现“一次编写，到处运行”。
    - 字节码设计简单，既可以解释执行（低效），也有 JIT 编译提升性能（高效）。
    - 支持多线程并发。
- **稳定和简洁（KISS 原则）**
    - 自动检查数组下标（避免越界）。
    - 自动内存管理（垃圾回收 GC），不像 C 需要手动管理。
    - 风格接近 C++，但去掉了很多复杂性。
- **纯面向对象**
    - Java 可以看作 “C++ -”，其没有 C++ 的一些复杂特性
	    - 没有多重继承 Multi-inheritance
	    - 没有虚拟继承 Virtual inheritance
	    - 没有模板 Template
	    - 没有运算符重载 Operator overloading
    - 一切都是类和对象。
- **对象存储与生命周期**
    - 在 C 中，对象存储方式由程序员决定（效率优先）。
    - 在 Java 中，所有对象都在 **堆内存** 中构造。
    - 程序员只能创建对象，销毁由 GC 自动处理。
- **根类 Object**
    - 所有类都直接或间接继承自 `Object`。
    - 因此所有东西都是对象，容器即为能够存放对象的对象。

按照惯例，下面给出 The First Java Program：

```java
public class Main {
	public static void main(String[] args) {
		System.out.println("Hello World!");
	}
}
```

## Object

在 Java 中，对象本身一定存储在**堆内存**中，而**栈**中保存的是指向该对象的引用。在方法参数、变量、成员字段里出现的所谓“对象”，其实都是该对象值的**引用**（在 Java 中称为句柄 Handle）。

句柄类似于 C/C++ 的指针，但有几个重要区别：

- <1> 不能做指针运算
	- Java 中的句柄并不能做 `p+1`, `*(p+3)` 等操作
	- 程序员并不能直接操作该地址
- <2> 句柄是安全受控的
	- JVM 会管理句柄，确保你不会访问到非法内存
	- 一旦对象没有句柄指向，就会交给 GC 清理

### Primitive Types

在 Java 中，== 用在基本类型中时，比较二者值是否相等；用在对象引用中时，比较是不是指向同一个对象。

Java 中的基本类型（primitive types）共有 8 种，它们并不是对象，它们在栈上存放它们的值本身：

| Type      | Bits            | Min Value                  | Max Value                 |
| --------- | --------------- | -------------------------- | ------------------------- |
| `byte`    | 8 bits          | -128                       | 127                       |
| `short`   | 16 bits         | -32768                     | 32767                     |
| `int`     | 32 bits         | -2,147,483,648             | 2,147,483,647             |
| `long`    | 64 bits         | -9,223,372,036,854,775,808 | 9,223,372,036,854,775,807 |
| `char`    | 16 bits         |                            |                           |
| `float`   | 32 bits         | Approximately -3.4E+38     | Approximately 3.4E+38     |
| `double`  | 64 bits         | Approximately -1.7E+308    | Approximately 1.7E+308    |
| `boolean` | 1 bit or 1 byte |                            |                           |

!!! note "像我们熟知的 `Integer`, `Double`, `Boolean` 都是包装类，属于对象，不属于基本类型"

!!! warning "像 `2.0` 这种默认是 double 型常量，要赋值给 `float` 类型变量，必须显式加上后缀 `2.0f`"

!!! abstract "Some Special Number Format"
	```java
	145_345.23_12 // 分隔符 _ 可以加在任意数字之间，除了开头/结尾或小数点旁边
	0b11001011    // 前缀 0b 或 0B 表示二进制值
	0123          // 前缀 0 表示八进制值
	0x7B          // 前缀 0x 或 0X 表示十六进制值
	```

!!! danger "为什么 Java 只有 signed，而没有 unsigned？"
	为了简化设计、追求跨平台移植性、以及对有符号类型需求不强。
	
	Java 8 之后新增了一些无符号运算的 API，可以把 `int`/`long` 类型当成无符号数来操作。

### String

String 本身是一串 charecters 的序列。在 Java 中，`String` 是一个 immutable 的 object，这意味着 `String` 的内容在其创造后是不能改变的。但是我们可以将一个声明为 `String` 的变量指向别的 `String` 对象，例如：

```java
	String s1 = "hello";
	String s2 = "world";
	s1 = s2; // 此时 s1 和 s2 都指向堆中的对象 "world"
```

immutable 的字符串意味着我们信任字符串是只读的值，在并行计算中，我们不需要对字符串进行加锁，天然支持多线程。

!!! tip "C++ 的 strings 是 mutable 的，但是 Python 的 strings 其实也是 immutable 的"

除此之外，Java 的运算时由左向右的，这意味着对于如下运算，它们的输出结果对应为：

```java
	"I'm " + 18;    // "I'm 18"
	1 + 2 + "age";  // "3age"
	"age" + 1 + 2;  // "age12"
```

为了比较对象的值，Java 的包装类都内置了 `.equals()` 方法，我们常用的 equality 比较都要使用该方法：

```java
// test equality
if(input.equals("bye")) {
	// ...
}
```

在 Java 中，除了不可变的 `String` 类外，还有两个常用的可修改字符串类：

```java
// 线程安全，同步，性能稍慢，适用于多线程
StringBuffer sb1 = new StringBuffer("Hello");
sb1.append(" World");

// 线程不安全，不同步，性能更快，适用于单线程
StringBuilder sb2 = new StringBuilder("Hello");
sb2.append(" World");
```

假定我们有如下代码：

```java
String s = "";
for (int i = 0; i < 100; i++ )
	s += i; // s = new StringBuilder(s).append(i).toString()
```

这个过程，总共会创造出 201 个对象，其中一轮循环会创造出一个 `StringBuilder` 和一个 `String`。这种情况我们就可以使用 mutable 的 `StringBuffer`：

```java
StringBuffer sb = new StringBuffer();
for (int i = 0; i < 100; i++)
	sb.append("" + i);
String s = sb.toString();
```

`String` 有一个方法 `String.valueOf()`，它针对所有基本类型以及 `Object` 都有重载版本，我们在做数据拼接时，编译器会自动调用 `String.valueOf()` 方法。

所有类都继承自 `Object`，而 `Object` 提供了一个默认的 `toString()` 方法。默认的 `toString()` 输出是类名+"@"+哈希码。

!!! info "如果我们想要自定义类对象有更好的字符串可读性，最好对 `toString()` 方法进行重写"

类似的，其它包装类，如 `Integer`, `Long`, `Float`, `Double` 等也都有各自的 `valueOf()` 方法，可以将 `String` 类转变为其它类型的对象：

```java
String piStr = "3.14";
Float pi  = Float.valueOf(piStr);
```

### 预生成对象

为了避免频繁 `new`，JVM 在类加载时会提前创建、缓存一些常用范围内的包装类对象，后续可以直接复用它们，这被称为预生成对象，存储在缓存池中。

- 对于整数类型：-128 ~ 127
- 对于字符类型：0 ~ 127
- 对于布尔类型：true / false
- 对于浮点数：没有预生成对象

```java
// > jshell
Integer i1 = 12; // in cache
Integer i2 = 12;
i1 == i2; // true

i1 = 1234; // 超出缓存范围
i2 = 1234;
i1 == i2; // false
```

与预生成对象有些类似，Java 会对 immutable 的 `String` 类型做**编译期常量优化**，一些编译期能确定的 `String` 常量表达式会被编译器优化到字符串常量池（StringPool）中：

```java
public class Test {
    public static void main(String[] args) {
        String s1 = "ab";
        String s2 = "abc";
        String s3 = s1 + "c";   // 运行时用 StringBuilder 生成新对象
        System.out.println(s3 == s2); // false
        String s4 = "ab" + "c"; // 编译器直接优化为 "abc"
        System.out.println(s4 == s2); // true
    }
}
```



## Class

> https://dev.java/learn/inheritance/objects/

Java 中，局部变量不会自动初始化，必须在使用前显式赋值；而对于类成员变量，即便没有显式赋值，编译器或 JVM 都会自动初始化：

- 对于 primitive 类型，初始化为 0
- 对于 reference 类型，初始化为 `null`

```java
class Demo {
	// 初始化顺序即为变量在类中定义的顺序，本例中为 x,b,s
    int x;       // 没赋值，自动初始化为 0
    boolean b;   // 自动初始化为 false
    String s;    // 自动初始化为 null
}
```

!!! info "不同于 C，Java 的类后不用再加分号 `;`"

在 Java 中，如果你希望在一个构造函数中调用**本类的另一个构造方法**，可以使用 `this(...)` 完成，这种用法称为**构造器委托(delegating constructor)**，且必须写在构造方法的第一行：

```java
class Flower {
    int petals;
    String name;

    Flower(int petals) {
        this(petals, "NoName"); // 调用另一个构造函数
    }

    Flower(int petals, String name) {
        this.petals = petals;
        this.name = name;
    }
}
```

在对象被 GC 回收之前，JVM 会调用对象的 `finalize()` 方法。设计上它与 C++ 中的析构函数(destructor)有些类似，但其实完全不同。

在 C++ 中，对象的生命周期是可控的，在离开作用域时马上调用析构函数，保证资源的释放；而对于 Java，内存释放时机由 GC 决定，你不知道对象什么时候被回收，甚至有可能不会被回收。

并且 GC 只负责回收内存，它并不等同于销毁对象，所以不能依赖 `finalize()` 来做一些关键的清理工作，比如关闭文件、释放网络连接等。

??? warning "`finalize()` 方法已经被标记为 *Deprecated*"
	Finalization is deprecated and subject to removal in a future release. The use of finalization can lead to problems with security, performance, and reliability. See [JEP 421](https://openjdk.org/jeps/421) for discussion and alternatives.
	
	Subclasses that override `finalize` to perform cleanup should use alternative cleanup mechanisms and remove the `finalize` method. Use [`Cleaner`](https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/ref/Cleaner.html "class in java.lang.ref") and [`PhantomReference`](https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/ref/PhantomReference.html "class in java.lang.ref") as safer ways to release resources when an object becomes unreachable. Alternatively, add a `close` method to explicitly release resources, and implement `AutoCloseable` to enable use of the `try`-with-resources statement.
	
	This method will remain in place until finalizers have been removed from most existing code.

Java 的 Static Member 和 C++ 没什么太大的区别，包括静态方法不能直接访问实例的成员（因为没有 `this`）。除此之外，Java 还有一种静态代码块，可以用来做一些静态变量的初始化或准备工作，这会在类第一次**加载**时执行：

```java
class InitDemo {
    static int x;
    static {
	    // 类加载时执行
        x = 42;
        System.out.println("静态代码块执行");
    }
}
```

在 Java 里，`Class` 本身也是一个对象（元对象），由 `java.lang.Class` 表示。我们写的每一个类都会在编译时生成一个 `.class` 文件，程序运行时，JVM 会把这个 `.class` 文件加载进来，并生成一个对应的 `Class` 对象。

!!! tip "一个 `.java` 文件内可以有多个类，但最多只能有一个是 `public`，且该类名必须和文件完全一致"

```java
class Demo {}

public class Test {
    public static void main(String[] args) throws Exception {
        // 获取 Class 对象的几种方式：
        Class<?> c1 = Demo.class;               // 编译期方式
        Class<?> c2 = Class.forName("Demo");    // 运行期通过名字加载
        Demo d = new Demo();
        Class<?> c3 = d.getClass();             // 从对象获取

        System.out.println(c1 == c2); // true，说明只有一个 Class 对象
        System.out.println(c2 == c3); // true
    }
}
```

!!! note "`Class` 对象就是所有普通对象的“模板”和“元数据容器”"

总结下来，对于 Java 类的变量初始化顺序，我们分为类加载阶段和对象创建阶段：

- <1> **类加载阶段**
	- 第一次使用类（包括 new 一个对象、调用静态方法、访问静态字段等）时，JVM 会去加载这个类的 `.class` 文件
	- 此时类中的**静态代码块**被执行，**静态变量**被初始化
- <2> **对象创建阶段**
	- 例如 new 一个对象时，此时我们在堆中为对象分配空间
	- 首先我们默认零值填充，将所有基本类型成员设为默认值 0，对象引用设为 `null`
	- 然后才执行**字段初始化**，这一步在构造器之前执行
	- 最后调用构造函数，完成对象初始化逻辑


## Package

Package 是 Java 对程序进行组织的方式，通过关键字 `import` 引入。

| PL     | syntax                      | 实现                                      |
| ------ | --------------------------- | --------------------------------------- |
| C/C++  | `#include <stdio.h>`        | 文本插入，编译时只看原型，链接时需要编译后的二进制代码             |
| Java   | `import java.util.Scanner;` | 装载类，用 RTTI 了解类，编译和运行时均需要编译后的二进制代码，会自动编译 |
| Python | `import Pandas`             | 装在运行 .py 文件，需要源码可见                      |

```java
import java.lang.Math;
double r = Math.cos(Math.PI * theta);

// 通过 static import 可以引入类的静态对象
import static java.lang.Math.PI;
import static java.lang.Math.*;
double r = cos(PI * theta);
```

`package` 可以由用户自己定义，具体语法如下：

```java
// in MyClass.java
package mypackage;
public class MyClass {
}

// in other .java
import mypackage.*;
MyClass m = new MyClass();
```

另外，同一个 package 内的 class 可以访问该包中其它类的 `protected` 成员。

!!! tip "事实上，Java 中同包比子类更“亲密”一些"

## 泛型编程

### Inheritance

继承是面向对象编程重要的一个特性。在没有显式继承时，所有类都默认继承 `java.lang.Object`，所以所有对象都有 `toString()`, `equals()`, `hashCode()` 等方法。

!!! info "这也意味着所有类的根都是 `Object`"

在 Java 中，一个类只能继承一个父类（`extends`），因此不存在菱形继承的问题，其基本语法写作：

```java
class ThisClass extends SuperClass {
	// class body
}
```

对于继承关系中的对象构造顺序，其思想基本和 C++ 相同，即先构造父类，再构造子类。此处需要注意的是静态成员变量和静态代码块，它们只会在类加载阶段执行一次，要将它们与对象创建阶段分开看待：

```java
class Parent {
    // 静态成员
    static int staticVarP = initStaticVarP();

    // 静态代码块
    static {
        System.out.println("Parent 静态代码块");
    }

    // 非静态成员
    int instanceVarP = initInstanceVarP();

    // 非静态代码块
    {
        System.out.println("Parent 非静态代码块");
    }

    // 构造函数
    Parent() {
        System.out.println("Parent 构造函数");
    }

    // 工具方法
    static int initStaticVarP() {
        System.out.println("Parent 静态成员初始化");
        return 1;
    }
    int initInstanceVarP() {
        System.out.println("Parent 非静态成员初始化");
        return 2;
    }
}

class Child extends Parent {
    static int staticVarC = initStaticVarC();

    static {
        System.out.println("Child 静态代码块");
    }

    int instanceVarC = initInstanceVarC();

    {
        System.out.println("Child 非静态代码块");
    }

    Child() {
        System.out.println("Child 构造函数");
    }

    static int initStaticVarC() {
        System.out.println("Child 静态成员初始化");
        return 3;
    }
    int initInstanceVarC() {
        System.out.println("Child 非静态成员初始化");
        return 4;
    }
}

public class Demo {
    public static void main(String[] args) {
        System.out.println("=== 创建子类对象 ===");
        new Child();
    }
}
```

上述测试的输出结果为：

```java
=== 创建子类对象 ===
Parent 静态成员初始化
Parent 静态代码块
Child 静态成员初始化
Child 静态代码块
Parent 非静态成员初始化
Parent 非静态代码块
Parent 构造函数
Child 非静态成员初始化
Child 非静态代码块
Child 构造函数
```

在继承中，如果父类没有无参构造函数，或者说如果子类想要调用父类的**有参构造函数**，我们可以在子类中使用 `super(...)` 显式调用并传参。

```java
class Parent {
    Parent(int x) {
        System.out.println("Parent 构造函数, x = " + x);
    }
}

class Child extends Parent {
    Child(int y) {
        super(y); // 必须显式调用父类的有参构造
        System.out.println("Child 构造函数, y = " + y);
    }
}
```

- 如果父类有无参构造函数，子类没写 `super()`，则编译器会自动加一个 `super()`
- 如果父类**只有**有参构造函数，子类**必须**显式书写 `super(...)`，否则编译报错

!!! warning "`super(...)` 必须是子类构造函数的第一行"

Java 中的 Name Hiding、Overload 等和 C++ 中均相同，但是 Override 存在差异。对于 Java，你可以看作所有非 `static`, 非 `private`, 非 `final` 的方法都是**虚函数**，如果子类定义同签名方法，则默认就为**重写(override)**。

!!! note "Java 5 之后引入了 `@Override` 注解，让编译器进行检查"

!!! tip "仅有返回值类型不同不会被认为是 Overload，并且会导致编译错误！！！"

当我们调用类的方法 `someClass.someMethod()` 时，编译器/运行时需要决定应该调用哪一个具体的函数题，即 Method Call Binding。

- **Static Binding:** 在编译期就决定好，常用于 `static`, `final`, `private` 方法

```java
class Test {
    static void hello() { System.out.println("Hello"); }
    public static void main(String[] args) {
        Test.hello(); // 静态绑定，编译时就知道调哪个方法
    }
}
```

- **Dynamic Binding:** 运行时才决定，调用的方法取决于对象的实际类型，而不是引用类型

```java
class Animal {
    void speak() { System.out.println("Animal speaks"); }
}
class Dog extends Animal {
    @Override
    void speak() { System.out.println("Dog barks"); }
}
public class Test {
    public static void main(String[] args) {
        Animal a = new Dog(); 
        a.speak(); // 动态绑定 -> 调用 Dog 的方法
    }
}
```

!!! abstract "动态绑定是 Java 实现多态 Polymorphism 的关键"

一个只有声明，没有方法体的方法被称为**抽象方法（abstract method）**，它的作用是定义一个统一的接口规范，让子类去实现具体的功能：

```java
abstract class Animal {
    abstract void speak(); // 抽象方法，没有方法体
}
```

含有抽象方法的类必须声明为 `abstract`，并且不能使用 `new` 来构造它。但是我们仍然可以定义构造方法、普通方法，甚至静态方法。

子类继承抽象类后，要么实现所有抽象方法，要么把自己也定义为抽象类。

totally abstract class 被称为 **Interface**，它的所有方法都默认是 `public abstract` 的，所有成员变量都默认是 `public static final` 的，因此也可以说接口是由全局常量和公共抽象方法组成的特殊类：

```java
// 定义接口
interface Animal {
    void speak();  // 默认就是 public abstract
    int AGE = 3;   // 默认就是 public static final

	// default 方法：带默认实现，实现类可复用也可覆盖
	default void sayHi(String name) {
		System.out.println("Hi, " + name + " (from default method)");
	}
}

// 实现接口的类
class Dog implements Animal {
    @Override
    public void speak() {
        System.out.println("Dog barks");
    }
}
```

!!! note "注意要使用 `implements` 而不是 `extends`，但是一个类可以同时 `implements` 和 `extends`"

同时，因为接口不能被实例化，我们也不能为 Interface 定义构造方法。

接口本身也可以**继承**一个或多个接口，但是不能继承类（也包括抽象类）：

```java
// 定义一个基础接口
interface Animal {
    void eat();
}

// 让另一个接口继承它
interface Pet extends Animal {
    void play();
}

// 实现子接口的类
class Dog implements Pet {
    public void eat() {
        System.out.println("Dog is eating.");
    }

    public void play() {
        System.out.println("Dog is playing.");
    }

    public static void main(String[] args) {
        Dog d = new Dog();
        d.eat();   // 调用从 Animal 接口继承的方法
        d.play();  // 调用在 Pet 接口中新定义的方法
    }
}
```

!!! abstract "因此如果我们遇到 `extends sth1, sth2` 的情况，一定是接口在多继承接口"

!!! example
	=== "Common Situation"
		![[basic_1.png]]
	=== "Deeper Situation"
		![[basic_2.png]]

前面说过一个类只能继承一个父类，但是一个类可以实现多个接口。

如果一个类实现了两个接口，并且这两个接口中存在相同前面的默认方法，那么编译器就不知道改用哪个实现，出现冲突。因此我们需要手动显式指定使用哪个接口的实现，例如：

```java
public interface Jukebox {
	public default String rock() {
		return "... all over the world!";
	}
}
public interface Carriage {
	public default String rock() {
		return "... from side to side";
	}
}
public class MusicalCarriage implements Carriage, Jukebox {
	@Override
	public String rock() {
		// 直接重写也可以
		return Carriage.super.rock();
	}
}
```

当类和接口的默认方法发生冲突时，Java 的三条解析规则：

- <1> **父类的实现优先** Any class wins over any interface
	- 当一个类同时 `extends` 一个父类和 `implements` 一个接口，如果父类中已经有**具体**实现，哪怕接口中有默认方法也会直接使用父类中的实现
	- 即继承的优先级高于接口
- <2> **子接口优先** Subtype wins over super-type
	- 如果两个接口都提供了同名默认方法，并且一个接口继承了另一个接口，那么子接口的实现会覆盖父接口的实现
- <3> **必须解决冲突** No rule 3
	- 如果前两条规则不能解决冲突，编译器会报错，此时在子类中必须手动重写方法或显式指定调用哪个接口的默认实现


!!! tip "Staic Methods in Interface"
	接口中的静态方法属于接口本身，不能被实现类继承，也不能被实现类覆盖，只能通过接口名调用。

### ENUM

枚举 `enum` 其实也是一种类，也可以有成员变量、成员函数以及构造函数等，但是不能定义在方法内。













Java 支持可变参数，例如

```java
func(String... a) // 等价于 (String[] a)，只是调用的时候不用创建一个数组了
{
	// ...
}
```

## Container




## Stream

流式计算用于解决容器的遍历操作，尝试将计算送进容器中，而不是将容器内数据拿出来做计算，从而提高效率。

正常使用 for each 的方式：

```java
Arraylist<Artist> artists = new Arraylist<>();

int cnt = 0;
for (Artist artist : artists)
	if (artist.isFrom("London"))
		cnt ++;
System.out.println(cnt);
```

使用 stream 方式：

```java
long count = artists.stream()
			 .filter(artist -> artist.isFrom("London")) // Lambda
			 .count();
System.out.println(count);
```

`filter` 并不是单纯的遍历过滤，而是构建流水线，

一个流计算可以有多个中间操作，但是只能有一个终端操作。

`java.util.stream` 中提供了四套专门用于**基本类型**的流类型，它们都实现 `BaseStream` 接口，

TODO：直接书写一个 create stream 的表格





短路 eager，不跑完所有元素


## IO

> nio 和 nio2.0 不考


