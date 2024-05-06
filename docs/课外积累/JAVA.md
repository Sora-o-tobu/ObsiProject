
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

字符串定义
`String str1 = "不想学习...";`

数组定义

==Java中数组分配空间后有默认值，整数和char都默认为0，boolean值默认为false==


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
    }  
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
        //128_2 = 10000000 可以看作 127+1 = -128 一个圆循环  
  
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

要调用控制台输入，需要先导包`import java.util.Scanner;`

```java
import java.util.Scanner;//导包  
  
public class TestScanner {  
    public static void  main(String[] args){  
        Scanner input = new Scanner(System.in);//定义扫描仪  
  
        System.out.println("请输入用户名：");//提示输入  
        String username = input.next();//使用字符串类型去接收输入  
        char firstChar = username.charAt(0);//获取第一个字符  
        System.out.println("第一个字符TEST "+username.charAt(0));  
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
    public static void add(int... nums){ //可变参数，可以传任意数量参数进去方法中，得到一个数组  
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

在图书馆饿死了，因此出去觅食，望周知

更新：觅食回来怎么五一假期结束了... 我宣布JAVA大学习无限期延迟