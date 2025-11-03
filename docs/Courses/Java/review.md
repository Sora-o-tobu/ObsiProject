

- Which one below is NOT a valid Java identifier?
	- A. Each
	- **B. goto**
	- C. 名字
	- D. $2
	- 标识符可以包括 unicode字符（包括中文），数字（不能在开头），下划线以及美元符号。
- About access control in Java, which statement below is correct?
	- A. A member without any access modifier is default to `private`
	- B. A member without any access modifier is default to `public`
	- C. A member without any access modifier is default to `private`
	- **D. Classes in the same package can access members without any access modifier.**
	- 没加任何访问控制修饰符，则只有同一个包内的类可以访问，不在同一个包的子类不能访问
- **[F]** There is a `length()` method for all the arrays to represent the number of elements
	- `String` 使用 `length()` 方法，数组使用 `length` 属性

```java
class B {}
class C extends B {}
```

| 选项                     | 能否接受 List\<B\> | 能否接受 List\<C\> | 说明                                                   |
| ---------------------- | :------------: | :------------: | ---------------------------------------------------- |
| A. `List<? extends B>` |       ✅        |       ✅        | 上界通配符，C 是 B 的子类                                      |
| B. `List<B>`           |       ✅        |       ❌        | **泛型不协变**，虽然 C 是 B 的子类，但是 List\<C\> 不是 List\<B\> 的子类 |
| C. `List`              |       ✅        |       ✅        | 原生类型，擦除泛型后都能接收                                       |
| D. `List<?>`           |       ✅        |       ✅        | 任意类型通配符                                              |


