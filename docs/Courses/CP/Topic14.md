
# Object-Oriented Languages

## Overview

面向对象语言有两个关键特征，编译器实现的难点也主要围绕它们展开。

!!! info "面向对象的两大特征"
	- **信息隐藏 / 封装（information hiding / encapsulation）**：一个模块可以提供某个类型的值，但该类型的**表示（representation）只有模块自己知道**；客户只能通过模块提供的**操作**来操纵这些值。在 OO 术语里：值 → **对象（objects）**，操作 → **方法（methods）**。
	- **扩展 / 继承（extension / inheritance）**：如果某个程序上下文（如函数或方法的形参）期望一个支持方法 m1、m2、m3 的对象，那么它**也能接受**一个支持 m1、m2、m3、m4 的对象。

第二条正是**子类型多态**的基础：

```c
int foo(A a1) {
    a1.m1(); a1.m2(); a1.m3();
}
```

只要传进来的对象支持 `A` 的所有方法，`foo` 就能正常工作——哪怕它实际上是 `A` 的某个子类、还多了别的方法。

## Object-Tiger

教材给 Tiger 语言扩展了**类声明**语法，得到 Object-Tiger。

```text
dec        → classdec
classdec   → class class-id extends class-id { {classfield} }
classfield → vardec
classfield → method
method     → method id(tyfields) = exp
method     → method id(tyfields) : type-id = exp
```

!!! note "`class B extends A { ... }` 的语义"
	- 声明一个**继承自 A** 的新类 B；该声明必须在声明 A 的 let-表达式作用域内。
	- A 的**所有字段和方法都隐式属于 B**。
	- B 可以**重写（override）**A 的某些方法（给出新定义），但**参数类型和返回类型必须完全一致**。
	- 但**字段不能被重写**。
	- 有一个预定义的类标识符 **`Object`**，它没有任何字段或方法（是所有类的根）。
	- B 中每个方法都有一个**隐式形参 `self`**，类型为 B——`self` 在每个方法内被自动绑定。

创建对象与调用方法的新表达式语法：

```text
exp → new class-id
    → lvalue . id()
    → lvalue . id(exp {, exp})
```

- `new B`：创建一个 B 类型的新对象；
- `b.x`：取对象 b 的字段 x；
- `b.f(x, y)`：调用 b 的方法 f，显式实参为 x、y，**隐式 `self` 参数取值 b**。

### 一个面向对象程序

```text
let start := 10
    class Vehicle extends Object {
      var position := start
      method move(int x) = (position := position + x)
    }
    class Truck extends Vehicle {
      method move(int x) =
          if x <= 55
          then position := position + x
    }
    class Car extends Vehicle {
      var passengers := 0
      method await(v: Vehicle) =
          if (v.position < position)
          then v.move(position - v.position)
          else self.move(10)
    }
    var t := new Truck
    var c := new Car
    var v : Vehicle := c
in
    c.passengers := 2;
    c.move(60);
    v.move(70);
    c.await(t)
end
```

!!! note "进入 `await` 时，作用域内有哪些变量？"
	- 外层 let 声明的 **`start`**；
	- 类标识符 **`Vehicle`、`Truck`、`Car`**（以及 `Object`）；
	- 显式形参 **`v`**（类型 Vehicle）；
	- 隐式形参 **`self`**（类型 Car）；
	- 通过 `self` 可访问的**字段** `position`（继承自 Vehicle）、`passengers`，以及**方法** `move`（继承自 Vehicle）、`await`。

这段程序还体现了**动态绑定**：`v` 的静态类型是 `Vehicle`，但实际指向一个 `Car`；`v.move(70)` 到底调用哪个 `move`，要到运行时才能确定。

### 取字段的难题

考虑 `v.position`，其中 `v` 属于类 `Vehicle`。要对它求值，编译器必须生成代码从 `v` 所指的对象（记录）中**取出 `position` 字段**。

**朴素想法**：从变量 `v` 的环境项里拿到 `Vehicle` 的类描述符，再从描述符里查出 `position` 的偏移。

!!! warning "但是……运行时 `v` 可能指向一个 `Car` 或 `Truck`！"
	那么 `position` 字段又会在什么位置？——这就要求对象的**内存布局**必须保证：同一个字段在父类和**所有子类**的记录里都位于**相同的偏移**。下一节的"前缀法"正是为此设计。

## Single Inheritance of Data Fields

**单继承（single inheritance）**语言中，每个类只扩展**一个**父类。

### 字段：前缀法（Prefixing）

!!! success "前缀法（Prefixing）"
	当 B 扩展 A 时，B 记录中**从 A 继承来的字段**排在**开头**，顺序与它们在 A 记录中的顺序**完全相同**；B 自己**新增**的字段排在其后。

```text
class A extends Object { var a := 0 }
class B extends A { var b := 0; var c := 0 }
class C extends A { var d := 0 }
class D extends B { var e := 0 }
```

![[cp_topic14_1.png]]

这样，字段 `a` 在 A、B、C、D 的记录里**永远位于偏移 0**；字段 `b` 在 B、D 里永远位于偏移 1……于是无论 `v` 实际指向哪个子类，编译器都能用**同一个编译期已知的偏移**取到该字段，**一条指令**即可完成。

### 方法（Methods）

一个**方法实例（method instance）**的编译方式很像普通函数：它变成驻留在指令空间某个**特定地址**的机器码。例如方法实例 `Truck_move` 的入口点对应机器码标号 `Truck_move`。

!!! note "每个类描述符（class descriptor）包含：指向**父类**的指针，以及一张**方法实例列表**。"

### 静态方法（Static Methods）

有些 OO 语言允许把方法声明为 **`static`**：**不能被子类重写**，在**编译期**就能确定调用目标。编译形如 `c.f()` 的调用时，编译器：

1. 找到 `c` 的类，设为 C；
2. 在 C 中查找方法 `f`，假设没找到；
3. 到 C 的父类 B 中查找，依此类推；
4. 若在某个祖先类 A 中找到了 `static` 方法 `f`，则直接编译成一个到标号 `A_f` 的**函数调用**。

```text
Class A extends Object { var x := 0  static method f() }
Class B extends A { method g() }
Class C extends B { method g() }
// c.f()  →  call A_f
```

### 动态方法（Dynamic Methods）

如果 A 中的 `f` 是**动态方法**呢？它可能在 C 的某个子类 D 中被**重写**。编译期**无法判断** `c` 指向的是 D 的对象（该调 `D_f`）还是 C 的对象（该调 `A_f`）。

```text
Class A extends Object { var x := 0  method f() }
Class B extends A { method g() }
Class C extends B { method g() }
Class D extends C { var y := 0  method f() }
// c.f() : 运行时才知道该调 A_f 还是 D_f
```

!!! success "解决：方法表（method table / vtable）"
	类描述符里放一个**向量（方法表）**，为每个（非静态）方法名存一个方法实例。当 B 继承 A 时，方法表**先放 A 已知的所有方法名的表项，再接上 B 新声明的方法**——和带继承的字段布局**完全一样**的前缀思想。

![[cp_topic14_2.png]]

上图中：每个对象的第 0 个字指向其类描述符；A 的方法表是 `[A_f]`，B 是 `[A_f, B_g]`，C 是 `[A_f, C_g]`，D 是 `[D_f, C_g]`（D 重写了 f，故 f 槽位换成 `D_f`）。注意 `f` 在所有类的方法表里都位于**相同的偏移**。

执行动态方法调用 `c.f()`，编译后的代码执行三步：

```text
1. 从对象 c 的偏移 0 处取出类描述符 d
2. 从 d 的 (常量) f 偏移处取出方法实例指针 p
3. 跳转到地址 p，并保存返回地址（即 call p）
```

!!! info "这正是 C++/Java 中**虚函数表（vtable）**的机制：字段访问 1 条指令（前缀法），动态方法分派 2 次取值 + 1 次调用。"

## Multiple Inheritance

在允许一个类 D 同时扩展**多个**父类 A、B、C 的语言里，查找字段偏移和方法实例就困难了：**不可能**同时把 A 的所有字段放在 D 的开头、又把 B 的所有字段放在 D 的开头。

### 全局图着色——字段

!!! info "全局图着色（Global Graph Coloring）"
	一次性**静态分析所有类**，用**图着色算法**为每个**字段名**找到一个偏移，使得它在**所有**包含该字段的记录里都能用这个偏移。

	- **节点**：一个不同的字段名；
	- **边**：两个字段在某个类中**共存**（coexist）；
	- **颜色**：偏移（0, 1, 2, …）。

```text
class A extends Object { var a := 0 }
class B extends Object { var b := 0; var c := 0 }
class C extends A { var d := 0 }
class D extends A,B,C { var e := 0 }
```

![[cp_topic14_3.png]]

由于相邻（共存）的字段必须取不同颜色（偏移），着色的结果是：每个字段名拿到一个全局唯一可用的偏移（如上图中 B 的偏移 1 处、C 的偏移 1、2 处留空）。

!!! warning "问题：对象中间出现空槽"
	全局着色会在对象**中间留下空槽（empty slots）**，浪费空间。

**解决**：把每个对象的字段**紧凑打包（pack）**，改由**类描述符**来告诉每个字段在哪里。

![[cp_topic14_4.png]]

这样**对象本身没有空槽**，空槽转移到了**类描述符**里——这是可接受的，因为 **对象数 ≫ 类描述符数**。代价是：每次取/存字段需要 **3 条指令**而非 1 条：

1. 从对象取出描述符指针；
2. 从描述符取出该字段的偏移值；
3. 在对象的相应偏移处取（或存）数据。

### 全局图着色——方法查找

多继承下查找方法实例，**同样用全局图着色**：把**方法名和字段名混在一起**，构成一张大的干涉图（interference graph）。

- 描述符中**字段**的表项 → 给出对象内的**位置（偏移）**；
- 描述符中**方法**的表项 → 给出方法实例的**机器码地址**。

### 问题：动态链接

!!! warning "全局图着色与动态链接的冲突"
	全局着色**只能在链接期（link-time）**完成。但很多面向对象系统支持**把新类动态载入正在运行的系统**；链接期图着色对**动态增量链接**很不友好。

### 哈希（Hashing）

!!! note "在每个类描述符里放一张哈希表"
	把字段名映射到偏移、方法名映射到方法实例。这种方案对**分离编译（separate compilation）**和**动态链接**都很友好。

	- **Ftab**：存字段偏移与方法实例；
	- **Ktab**：存字段名指针（用于**冲突检测**）。

取对象 `c` 的字段 `x`，编译器生成如下代码：

```text
1. 从对象 c 的偏移 0 处取出类描述符 d
2. 从地址 d + Ktab + hash_x 处取出字段名 f
3. 测试 f = ptr_x ?       （冲突检测）
4. 若相等，从 d + Ftab + hash_x 处取出字段偏移 k
5. 从 c + k 处取出字段内容
```

## Testing Class Membership

有些 OO 语言允许程序在**运行时检测**一个对象是否属于某个类。

| | Modula-3 | Java |
|---|---|---|
| 测试对象 x 是否属于类 C 或其任意子类 | `ISTYPE(x, C)` | `x instanceof C` |
| 把 C 类变量 x（实际指向 C 的子类 D 的对象）转换为编译期类型 D 的表达式 | `NARROW(x, D)` | `(D) x` |

（教材 **TABLE 14.6**：类型测试与安全转型的设施。）

### 朴素实现

假设没有多继承，`x instanceof C` 可以在运行时跑一个循环：

```text
   t1 ← x.descriptor
L1: if t1 = C goto true
    t1 ← t1.super          // t1.super 是类 t1 的超类（父类）
    if t1 = nil goto false
    goto L1
```

!!! warning "沿着父类链一路向上查找，**可能很慢**（与继承深度成正比）。"

### Display 方法（更快）

!!! success "用父类的 display（显示表）"
	假设类的**嵌套深度**有上界（如 20），在每个类描述符里预留一个 20 字的块。设类 D 的嵌套深度为 j（编译期已知），在 D 的描述符中：

	$$
	\text{display}[j]=D,\;\; \text{display}[j-1]=D.\text{super},\;\;\dots,\;\;\text{display}[0]=\texttt{Object}
	$$

	且对所有 $k > j$，$\text{display}[k]=\text{nil}$。

于是：若 `x` 是 D 或 D 的任意子类的实例，则在 `x` 的类描述符里 $\text{display}[j]=D$；否则不是。`x instanceof D` 只需**常数时间**三步：

```text
1. 从对象 x 的偏移 0 处取出类描述符 d
2. 取出 d 的第 j 个 class-pointer 槽
3. 与类描述符 D 比较
```

!!! info "这与活动记录里的 **display**（用于嵌套作用域变量访问）是同一种思想：用按深度索引的数组，把「沿链查找」变成「按下标直取」。"

### 类型强转（Type Coercions）

设变量 `c` 的类型为 C：

- 把 `c` 当作 C 的**任意超类**使用（**向上转型 upcast**），如 `var b : B := c`（其中 C extends B）——**合法且安全**；
- **反过来**不成立：赋值 `c ← b`（**向下转型 downcast**）只有当 `b` 在运行时**确实**是 C 的实例时才安全，否则——

```text
b ← new B
c ← b
c.some_field_of_C_but_not_B    // 不可预测的行为
```

!!! warning "向下转型的危险"
	若 `b` 实际并非 C 的实例，访问"C 有而 B 没有"的字段会读到越界/无意义的内存，导致**不可预测的行为（unpredictable behavior）**。

因此 Modula-3、Java 等会给"超类 → 子类"的转换**附带一次运行时类型检查**，除非运行时值确实是子类实例，否则**抛出异常**：

```text
Modula-3:                 Java:
IF ISTYPE(b, C)           if (b instanceof C)
  THEN f(NARROW(b, C))        f((C) b)
ELSE ...                  else ...
```

!!! warning "C++ 的 `static_cast` **不做**运行时检查 ⇒ 不安全。"

### Typecase

Modula-3 提供 **`TYPECASE`** 设施，把"测试-然后-窄化（test-then-narrow）"的惯用法写得更优雅、更高效：

```text
TYPECASE expr
OF C1 (v1) => S1
 | C2 (v2) => S2
 | ...
 | Cn (vn) => Sn
ELSE S0
END
```

!!! note "Typecase 的语义"
	- 若有**多个** Ci 匹配（例如其中一个是另一个的超类），只采用**第一个**匹配的子句；
	- 若**没有** Ci 匹配，则走 **ELSE** 子句。

它可以直接展开成一串 **else-if**，每个 if 做三件事：一次**实例测试（instanceof）**、一次**窄化（narrow）**、一次**局部变量声明**（把窄化后的对象绑定到 vi）。

## Private Fields and Methods

真正的面向对象语言能保护对象的字段，使其不被**其它对象的方法**直接操纵。

!!! note "私有（private）的定义"
	- **私有字段**：不能被对象**外部**声明的任何函数或方法取值或更新；
	- **私有方法**：不能从对象外部调用。

!!! info "私有性由编译器的**类型检查阶段静态地（statically）**强制实施。"
	实现上很简单：在一个类的符号表中，每个字段偏移、方法偏移旁边，附带一个**布尔标志**，标明该字段/方法是否私有。

