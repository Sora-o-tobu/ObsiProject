
!!! abstract 
	Java和C++一样是面向对象的程序设计语言，而C是面向过程的语言

## 第一个程序 Hello World

```java
public class Hello {  
    public static void main(String[] args){  
        System.out.println("Hello, World!");  
    }  
}
```


- 其中，`class`代表类，可以看作代码的容器，所有代码都应该放在类里面，一个源文件中可以有多个类，但是public最多能有一个，且public类名应与文件名字相同，如上述程序的文件名为`Hello.java`
- `main`是主方法(method)，是程序的入口

### 编译运行

在文件所在目录打开git bash(也可以使用cmd或终端)，并输入下述命令进行编译
```git
javac Hello.java
```
可以看到目录下新增了一个`.class`的二进制文件，如果你用二进制文件查看器打开它，会发现java编译出来的文件开头都是`cafe babe`，这也是java(一种咖啡)名称的由来。

同样在文件目录打开git bash，在上一步的基础上输入：
```git
java Hello
```
注意，直接输入类名，不用添加`.class`或者`.java`。

## 变量


变量是计算机内存中的一块存储空间，是存储数据的基本单元

### 变量名
 
变量名语法规定基本与C相同，但是追加可以由货币符号组成

!!! question
	`main、String、$8、_3$`都是合法的变量名


### Java数据类型

- **基本数据类型**
	- 整数
	- 小数
	- 布尔
	- 字符
- **引用数据类型**
	- 字符串
	- 数组
	- 对象

#### 基本数据类型表格，共分为八种：

| 数据类型    | 大小     | 范围                      |                           |
| ------- | ------ | ----------------------- | ------------------------- |
| byte    | 1 byte | $-2^{7}$ to $2^{7}-1$   | -128 to 127               |
| short   | 2 byte | $-2^{15}$ to $2^{15}-1$ | -32768 to 32767           |
| int     | 4 byte | $-2^{31}$ to $2^{31}-1$ | -2147483648 to 2147483647 |
| long    | 8      | $-2^{63}$ to $2^{63}-1$ | 略                         |
| float   | 4      | N/A                     | N/A                       |
| double  | 8      | N/A                     | N/A                       |
| char    | 2      | 0 to $2^{16}-1$         | 0 to 65535                |
| boolean | N/A    | true/false              | 默认false                   |

#### 引用数据类型

!!! note "Java中数组分配空间后有默认值，整数和char都默认为0，boolean值默认为false"

- 字符串定义: `String str1 = "不想学习...";`
	- 字符串的比较: `str1.equals("想学习...")`  (False)
- 数组定义:

```java
int[] a = new int[5]; //声明并且分配空间

int[] a; //也可以分开来写
a = new int[5];

System.out.println(a.length); //输出数组的长度
```

```java
int[] a = new int[]{1,2,3,4,5}; 
//两种都可以，声明并且赋值不可以分开
int[] a = {1,2,3,4,5};
```

- 二维数组:
```java
int[][] array = new int[3][5];

int[][] array = new int[3][];//高维必须写
array[0] = new int[4];
array[1] = new int[3];
array[2] = new int[2];
//低维可以长度不一

int[][] array = {{1,2},{2,3},{3,4}}
```

还要导入包才能直接打印数组
```java
import java.util.Arrays;  
  
public class Testarray {  
    public static void main(String[] args){  
        int[] nums = {1,2,3,4,5};  
        int[] newnums = new int[10];  
        for(int i=0;i<nums.length;i++){  
            newnums[i] = nums[i];  
        }  
        nums = newnums; //Java特有操作，类似指针  
        System.out.println(Arrays.toString(nums));  
        //Output: [1, 2, 3, 4, 5, 0, 0, 0, 0, 0]  
        System.out.println(Arrays.toString(Arrays.copyOf(newnums,5)));  
        //也可以直接用Arrays.copyOf()方法，返回带有原值的新数组实现数组长度更改  
        //Output: [1, 2, 3, 4, 5]  
        nums = Arrays.copyOf(nums,15);  
        //Arrays.copyOf()方法不会直接修改原数组，而是返回一个新数组  
        System.out.println(Arrays.toString(nums));  
        //Output: [1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
}
```

#### 类型转换

自动类型转换与强制类型转换语法同 C 相同，如：
```java
short s = 123;
byte b = (byte)s; //最好不要造成截断，符号位变化可能会变成负数
```


### 总结

```java
public class TestType {  
    public static void main(String[] args){  
        //Test basic data type  
        byte b1 = 100;  
        System.out.println("b1 = "+b1);  
        //Test overflow  
        byte b2 = (byte)128; //Byte: -128 to 127  
        System.out.println("b2 = "+b2);  
        //Output: b2 = -128  
        //128_2 = 10000000 是 -128 的补码 
  
        long l1 = 1000000000000000000L; //后面要加L，不然编译器会认为是int类型  
        float f1 = 3.14f; //后面要加f，不然编译器会认为是double类型  
        double d1 = 3.1415926;  
        System.out.println("l1 = "+l1+"; f1 = "+f1+"; d1 = "+d1);  
        //Output: l1 = 1000000000000000000; f1 = 3.14; d1 = 3.1415926  
  
        char c1 = 'A';  
        System.out.println("c1 = "+c1);  
        //Output: c1 = A  
        System.out.println("c1 = "+(int)c1);  
        //Output: c1 = 65  
        System.out.println((int)'死');  
        //Output: 27515 输出字符的Unicode编码  
  
        boolean is_True = true;  
        System.out.println("is_True = "+is_True); //Boolean类型不能转换成int类型  
        //Output: is_True = true  
  
        String myGF = "新海天";  
        System.out.println("我老婆是 "+myGF);  
    }  
}
```


## 运算符
!!! info "优先级"
	==算数＞关系＞赋值== 和 C语言 相同，但我老是忘，所以在这里再提一嘴
	
	如`a=b+c>d`的逻辑顺序为`a=((b+c)>d)`

用法基本与 C语言 相同，此处仅列出略有不同的部分

```java
public class Testop {  
    public static void main(String[] args){  
        int a=10,b=8;  
        System.out.println("a+b = "+a+b);  
        //Output: a+b = 108 这是因为字符串的加法会将整体都提升为字符串  
        System.out.println("a+b = "+(a+b));  
        //Output: a+b = 18  
  
        System.out.println("a-b = "+(a-b));  
        //同样，减法和乘除也要加上括号  
  
        System.out.println("a/b = "+(double)(a/b));  
        //Output: a/b = 1.0  
        System.out.println("a/b = "+((double)a/b));  
        //Output: a/b = 1.25  
  
        //System.out.println("5/0 = "+(5/0)); //运行时会显示错误  
        System.out.println("-5.0/0 = "+(-5.0/0));  
        //Output: -5.0/0 = -Infinity  
        //在底层二进制中，会认为浮点数是不精确的，因此(5.0/0)中的0其实是无穷小，所以结果是无穷大  
        System.out.println("0.0/0 = "+(0.0/0));  
        //Output: 0.0/0 = NaN (Not a Number)  
    }  
}
```


## 输入输出

```java
System.out.print("Hello world"); //在控制台输出双引号里的内容，注意要带分号
System.out.println("Hello world"); //加了ln，则会在后面自动输出回车
```

要调用控制台输入，需要先导包 `import java.util.Scanner;`

```java
import java.util.Scanner;//导包  
  
public class TestScanner {  
    public static void  main(String[] args){  
        Scanner input = new Scanner(System.in);//定义扫描仪  
  
        System.out.println("请输入用户名：");//提示输入  
        String username = input.next();//使用字符串类型去接收输入  
        char firstChar = username.charAt(0);//获取第一个字符  
        System.out.println("第一个字符TEST "+firstChar);  
        System.out.println("您用户名为"+username);  
  
        System.out.println("请输入您的ID：");  
        int id = input.nextInt();//使用整型去接收输入  
        System.out.println("您的ID为"+id);  
  
    }  
}
```

Scanner类中对应的方法

- .nextInt() 获得整数
- .nextDouble() 获得小数
- .next() 获得字符串
- .next().charAt(0) 获得单个字符

## 条件选择语句

### if-else 语句

和 C语言 一模一样，不记笔记

### switch 分支结构

和 C语言 一模一样，不记笔记

## 循环语句

`While` `do-while` `for`循环都和 C语言 一模一样，不记笔记

## 方法(面向对象)/函数(面向过程)

与 C语言 类似，但不用事先声明

如下述方法用来打印分隔符

```java
public static void printsign(int count , char sign){
	for(int i=0;i<count;i++){
		System.out.print(sign);
	}System.out.println();
}
```

有返回值的方法
```java
public static int add(int a,int b){
	return a+b;
}
```

可变参数，传进去变为数组(也可以直接把数组传进去)

```java
import java.util.Arrays;  
  
public class Testarray {  
    public static void main(String[] args){  
        int[] nums = {1,2,3,4,5};  
        add(1,2,3,4,5,6);//Output: Sum = 21  
        add(nums);//Output: Sum = 15  
    }  
    public static void add(int... nums){ 
    //可变参数，可以传任意数量参数进去方法中，得到一个数组  
        int sum=0;  
        for(int i=0;i<nums.length;i++){  
            sum+=nums[i];  
        }  
        System.out.println("Sum = "+sum);  
    }  
}
```



[60-数组排序_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1994y1r7uV?p=60&spm_id_from=pageDriver&vd_source=b856d3bdc44aa3fa77a923e6ba628c0f)
2024/5/1 14:06 看到P60 排序了

## 数组排序

java内置对数组快速排序，默认升序

```java
import java.util.Arrays;  
  
public class TestSort {  
    public static void main(String[] args){  
        int[] a={10,9,8,7,6,5,4,3,2,1};  
        System.out.println("排序前："+Arrays.toString(a));  
        //BubbleSort(a);  
        Arrays.sort(a); //内置优化的快速排序,默认升序排  
        System.out.println("排序后："+Arrays.toString(a));  
    }
    public static void BubbleSort(int[] nums){  
        for(int i=0;i<nums.length-1;i++){  
            for(int j=0;j<nums.length-i-1;j++){  
                if(nums[j]>nums[j+1]){  
                    int temp = nums[j];  
                    nums[j] = nums[j+1];  
                    nums[j+1] = temp;  
                }  
            }  
        }  
    }  
}
```

## 面向对象

!!! danger ""
	类是模板，对象是类的实例化，一个类可以产生多个对象

### 类与对象

首先尝试定义一个描述狗狗的类:

```java
package ObjTest;  //都放在同一个package中
  
public class Dog {  
    //属性  
    String Name;  //类的属性可以不赋值就可以在方法中调用，取默认值(NULL)
    String Brand;  
    int Age;  
    String Color;  
  
    //方法  
    public void eat(){
    //String Name = "拉布拉多"; 加上这句局部变量会覆盖属性中的Name 
        System.out.println(Name+" is eating");  
    //System.out.println(this.Name+" is eating");
    //加上this关键字后，会强制选择成员变量
    }  
    public void sleep(){  
        System.out.println(Name+" is sleeping");  
    }  
}
```

在另一个文件中可以调用这个类进行实例化，产生对象 `mydog`

```java
package ObjTest;  
  
public class ObjectTest {  
    public static void main(String[] args){  
        Dog mydog = new Dog();  
        mydog.Name = "天羽美羽";  
        mydog.Brand = "柴犬";  
        mydog.Age = 2;  
        mydog.Color = "白色";  
  
        System.out.println("Name:"+mydog.Name+"\tBrand:"+mydog.Brand+"\tAge:"+mydog.Age+"\tColor:"+mydog.Color);  
        mydog.eat();  
        mydog.sleep();  
/* Output: 
Name:天羽美羽	Brand:柴犬	Age:2	Color:白色
天羽美羽 is eating
天羽美羽 is sleeping
*/
    }  
}
```

### 方法重载

!!! note "定义"
	一个类中定义多个相同名称的方法

```java
public void eat(int a){
	System.out.println("吃No."+a+"药物中......");
}

public void eat(String a){
	System.out.println("吃"+a+"中");
}
```

### 构造方法重载

```java
package ObjTest.Stu;  
  
public class Student {  
    //属性  
    String Name;  
    int Age;  
  
    //构造方法  
    public Student(){  
        System.out.println("Student() Executed");  
    }  
    public Student(String Name){  
        System.out.println("Student(String Name) Executed");  
        this.Name = Name;  
    }  
    public Student(String Name, int Age){  
        System.out.println("Student(String Name, int Age) Executed");  
        this.Name = Name;  
        this.Age = Age;  
    }  
}
```


```java
package ObjTest.Stu;  
  
public class ObjectTest2 {  
    public static void main(String[] args){  
        Student stu1 = new Student();  
        Student stu2 = new Student("InuiSana");  
        Student stu3 = new Student("Nimisora", 2);  
  
        System.out.println("stu1.Name:"+stu1.Name+"\tstu1.Age:"+stu1.Age);  
        System.out.println("stu2.Name:"+stu2.Name+"\tstu2.Age:"+stu2.Age);  
        System.out.println("stu3.Name:"+stu3.Name+"\tstu3.Age:"+stu3.Age);  
    }  
}

/* Output: 
Student() Executed
Student(String Name) Executed
Student(String Name, int Age) Executed
stu1.Name:null	stu1.Age:0
stu2.Name:InuiSana	stu2.Age:0
stu3.Name:Nimisora	stu3.Age:2
*/
```

## 面向对象三大特性
### 封装
访问修饰符: ==private==  可将属性修饰为私有，仅本类可用，实现对外界隐藏实现细节

如，当你给出一个Student的类，别人使用时可能会赋值出 s.Age = 20000 的离谱操作，作为类提供者应当在类中避免这种情况

```java
package ObjTest.Stu2;  
  
public class Student {  
    //属性  
    String Name;  
    private int Age;  
    int score;  
  
    //方法  
    public boolean SetAge(int Age){  
        if(Age < 0 || Age > 120){  
            System.out.print("您输入的年龄范围有误 ");  
            return true;  
        }  
        this.Age = Age;  
        return false;  
    }  
    public int GetAge(){  
        return this.Age;  
    }  
}
```


```java
package ObjTest.Stu2;  
import java.util.Scanner;  
  
public class ObjectTest3 {  
    public static void main(String[] args){  
        Scanner input = new Scanner(System.in);//定义扫描仪  
  
        Student stu = new Student();  
        stu.Name = "InuiSana";  
        System.out.println("请输入年龄：");  
        while(stu.SetAge(input.nextInt())){  
            System.out.println("请重新输入年龄：");  
        }  
        stu.score = 100;  
  
        System.out.println("姓名："+stu.Name+" 年龄："+stu.GetAge()+" 分数："+stu.score);  
    }  
}
```

??? info
	![[GetterAndSetter.png]]
	
	在Idea中右键点击 生成 -> Getter和Setter 可以一键生成输入输出的封装

### 继承
父类有的，所有子类都应该有

![[继承的例子.png]]

- 使用 ==extends== 关键词继承父类

```java
package ObjTest.Inheritance.Pet;  
  
public class Pet {  
    private String Name;  
    private int Age;  
    private String Color;  
  
    public String getName() {  
        return Name;  
    }  
  
    public void setName(String name) {  
        Name = name;  
    }  
  
    public int getAge() {  
        return Age;  
    }  
  
    public void setAge(int age) {  
        Age = age;  
    }  
  
    public String getColor() {  
        return Color;  
    }  
  
    public void setColor(String color) {  
        Color = color;  
    }  
    
    public void PrintInformation(){  
        System.out.println("Name: " + Name + " Age: " + Age + " Color: " + Color);  
    }  
}
```

```java
package ObjTest.Inheritance.Pet;  
  
public class Dog extends Pet{  
    private String strain;  
  
    public String getStrain() {  
        return strain;  
    }  
  
    public void setStrain(String strain) {  
        this.strain = strain;  
    }  
}
```

```java
package ObjTest.Inheritance.Pet;  
  
public class InheritTest {  
    public static void main(String[] args) {  
        Dog mydog = new Dog();  
        mydog.setName("天羽美羽");  
        mydog.setAge(3);  
        mydog.setColor("Purple");  
        mydog.setStrain("学姐");  
  
        System.out.println("Name: " + mydog.getName() + " Age: " + mydog.getAge() + " Color: " + mydog.getColor() + " Strain: " + mydog.getStrain());  
        mydog.PrintInformation();  
    }  
}

/* Output: 
Name: 天羽美羽 Age: 3 Color: Purple Strain: 学姐
Name: 天羽美羽 Age: 3 Color: Purple
*/
```

- 继承的特点
	- 继承可以传递，可以多级继承
	- 单根性: 一个类只能有一个直接父类，但是可以有多个间接父类
	- 父类的私有属性可以被继承，但是不能访问
	- 当子类和父类有同名属性的时候可以加上关键词 ==super== 区分，不加super默认访问当前类的属性

#### 方法重写 Override

当父类提供的方法无法满足子类需求时，可在子类中定义和父类相同的方法进行重写

```java
package ObjTest.Inheritance.Pet;  
  
public class Dog extends Pet{  
    private String strain;  
  
    public String getStrain() {  
        return strain;  
    }  
  
    public void setStrain(String strain) {  
        this.strain = strain;  
    }  

	public void PrintInfromation() {
		super.PrintInformation();
		System.out.println(" strain: "+strain);
	}
}
```

- 方法重写的特点:
	- 访问权限不能变小 public > protected > default > private
	- 返回值类型要和父类一至，也可以是父类返回类型的子类 (如父类中返回类型如果为Pet，那么子类重写的方法可以返回Dog类型)
	- 方法名和参数列表要一样

#### 继承下的构造执行
在执行子类的构造的时候，会默认调用父类的构造 `super()` 

```java
public class ConstructorTest {  
    public static void main(String[] args) {  
        C Test = new C();  
    }  
}  
  
class A {  
    public A(){  
	    //super();
        System.out.println("A");  
    }  
}  
class B extends A {  
    public B(){
	    //super();  
        System.out.println("B");  
    }  
}  
class C extends B {  
    public C(){  
	    //super();
        System.out.println("C");  
    }  
}

/* Output:
A
B
C
*/
```

也可以手动调用super来覆盖默认的无参super

```java
public class ConstructorTest {  
    public static void main(String[] args) {  
        C Test = new C(1,2,3);  
        System.out.println("Construct is "+Test.a+Test.b+Test.c);  
    }  
}  
  
class A {  
    int a;  
    public A(int a){  
        this.a = a;  
        System.out.println("A");  
    }  
}  
class B extends A {  
    int b;  
    public B(int a , int b ){  
        super(a);  
        this.b = b;  
        System.out.println("B");  
    }  
}  
class C extends B {  
    int c;  
    public C(int a , int b , int c ){  
        super(a,b);  
        this.c = c;  
        System.out.println("C");  
    }  
}

/*Output:
A
B
C
Construct is 123
*/
```

!!! tip "最好还是把无参构造方法补上哦"

### 多态

父类引用指向子类对象，从而产生多种形态，如 `Pet pet = new Dog();`

!!! note "多态的表现形式"
	调用父类的方法，执行子类的方法

```java
public class TestCast {  
    public static void main(String[] args) {  
        Pet pet = new Dog();  
        pet.Name = "天羽美羽";  
        pet.eat();  
    }  
}  
class Pet {  
    String Name;  
  
    public void eat(){  //父类一定要有同名方法，不然报错
        System.out.println(Name + " is eating");  
    }  
}  
class Dog extends Pet {  
  
    public void eat() {  
        System.out.println(Name + " is eating me !");  
    }  
}
/* Output : 
天羽美羽 is eating me !
*/
```

- 应用 向上转型

```java
public class Test {
	public static void main(String[] args) {
		Master Me = new Master();
		Me.feed(new dog());
		Me.feed(new cat());
	}
}

class Master {
	public void feed(Pet pet) {
		pet.eat();
	}
}
/*Output: 
狗狗在吃骨头
猫猫在吃鱼
*/
```

```java
public class Test {
	public static void main(String[] args) {
		Master Me = new Master();
		Pet pet = Me.getPet("Dog");
		Me.feed(pet);
	}
}

class Master {
	public void feed(Pet pet) {
		pet.eat();
	}
	public Pet getPet(String type) {
		if(type.equals("Dog"))
			return new Dog();
		else
			return new Cat();
	}
}

/*
Output : 狗狗在吃骨头
*/
```

- 向下转型，将父类引用的真实子类对象强行转为子类本身类型，才能调用子类的方法

```java
public class Test {
	public static void main(String[] args) {
		Master Me = new Master();
		Me.Play(new Dog());
	}
}

class Master {
	public void Play(Pet pet) {
		if(pet instanceof Dog){
			Dog dog = (Dog)pet;
			dog.CatchDisk();//狗独有的方法，其它Pet子类没有
		}else{
			Cat cat = (Cat)pet;
			cat.AimaoTV();
		}
	}
}
```

!!! info "instanceof : 类型判断语句"






