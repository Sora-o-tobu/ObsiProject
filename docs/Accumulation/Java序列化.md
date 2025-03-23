# Java 反序列化漏洞

!!! quote "应课程作业要求，写一篇关于反序列化漏洞的笔记"

## 序列化和反序列化
!!! abstact "含义"
	序列化就是把对象转换为字节序列(即可以存储或传输的形式)的过程，而反序列化则是他的逆操作，即把一个字节序列还原为对象的过程。
	
	这两个操作一般用于对象的保存和网络传输对象的字节序列等场景。

![[序列化与反序列化.png]]

Java中，实现序列化和反序列化的类分别位于 `java.io.ObjectOutputStream` 和 `java.io.ObjectInputStream` 中

- **序列化** ObjectOutputStream类 => writeObject()
	- 该方法对参数指定的obj对象进行序列化，把字节序列写到一个目标输出流中，按Java的标准约定是给文件一个.ser扩展名
	- 流程：
		- (1) 创建一个对象输出流(也可以是文件输出流)
		- (2) 通过对象输出流的writeObject方法将对象序列化写入输出流
- **反序列化** ObjectInputStream类 --> readObject()
	- 该方法从一个源输入流中读取字节序列，再把它们反序列化为一个对象，并将其返回。
	- 流程：
		- (1) 创建一个对象输出流(也可以是文件输出流)
		- (2) 通过对象输出流的readObject方法将对象反序列化写入输出流

```java
import java.io.*;

public class ReflectionPlay implements Serializable {
//实现Serializable接口(或Externaluzable)的类的对象才能被序列化
    private void exec() throws Exception {
        String s = "hello";
        byte[] ObjectBytes=serialize(s);
        String after = (String) deserialize(ObjectBytes);
        System.out.println(after);
    }

    /*
     * 序列化对象到byte数组
     */
    private byte[] serialize(final Object obj) throws IOException {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        ObjectOutputStream objOut = new ObjectOutputStream(out);
        objOut.writeObject(obj);
        return out.toByteArray();
    }

    /*
     * 从byte数组中反序列化对象
     */
    private Object deserialize(final byte[] serialized) throws IOException, ClassNotFoundException {
        ByteArrayInputStream in = new ByteArrayInputStream(serialized);
        ObjectInputStream objIn = new ObjectInputStream(in);
        return objIn.readObject();
    }
    public static void main(String[] args) throws Exception {
        new ReflectionPlay().exec();
    }
}
```

该代码使用两个方法分别实现 serialize(序列化) 和 deserialize(反序列化) 

<div style="text-align: center;">
<img src="../image/ReflectionPlay.png" width="50%" style="margin: 0 auto;">
</div>

从运行结果可以看到，我们将String类型的对象序列化后又进行了反序列化，得到的还是原来的对象，成功将 `hello` 输出

### PHP 反序列化

在了解 Java 的反序列化漏洞前，可以先对 PHP 反序列化有一个大致认识

php 将数据序列化和反序列化会用到两个函数

- **serialize** 将对象格式化成有序的字符串
- **unserialize** 将字符串还原成原来的对象

!!! quote "在PHP中，序列化和反序列化一般用做缓存，比如session缓存，cookie等。"

一个简单的例子如下：

```php
<?php
$user=array('xiao','shi','zi');
$user=serialize($user);
echo($user.PHP_EOL);
print_r(unserialize($user));
?>
/* Output
a:3:{i:0;s:4:"xiao";i:1;s:3:"shi";i:2;s:2:"zi";}
Array
(
    [0] => xiao
    [1] => shi
    [2] => zi
)
*
*上面输出中，i代表整型数据 int ，s代表字符串，后接长度
*/
```

---

```php
<?php
 
class FileHandler {
    public  $op = 2;
    public  $filename = "flag.php";
}
 
$a = new FileHandler();
$b = serialize($a);
echo $b;
 
?>
# Output: O:11:"FileHandler":2:{s:2:"op";i:2;s:8:"filename";s:8:"flag.php";}
```

## 反序列化漏洞

### 漏洞分析

!!! warning "如果反序列化方法执行的是我们特殊构造的字节序列，那么反序列化漏洞就发生了"

如果Java应用对用户输入，即不可信数据做了反序列化处理，那么攻击者可以通过构造恶意输入，让反序列化产生非预期的对象，非预期的对象在产生过程中就有可能带来任意代码执行。

Java 反序列化漏洞分为 **触发点** 和 **利用链** ，在与客户交互的页面中，某些方法输入的参数不可控（触发点），能够给攻击者构造任意参数的机会。将攻击者构造的参数传递给后台去执行（利用链），一般会读取输入的参数、计算输入的参数，从而执行了攻击者构造的恶意参数。

Java中间件通常通过网络接收客户端发送的序列化数据，而在服务端对序列化数据进行反序列化时，会调用 readObject( ) 等方法。如果某个对象重写了readObject( ) 等方法（在Java中如果重写了某个类的方法，就会优先调用经过修改后的方法），且在方法中能够执行任意代码，那服务端在进行反序列时，也会执行相应代码。

如果能够找到满足上述条件的对象进行序列化并发送给Java中间件，Java中间件也会去执行指定的代码，即存在 ==反序列化漏洞==

!!! info "反射"
	通过调用 obj.getClass() 等函数，能够获取一个对象所属的类，进而能够执行该类中的方法（包括私有方法）
	
	而Apache Commons Collections中有一个特殊的接口，其中有一个实现该接口的类可以通过调用Java的反射机制来 **调用任意函数** ，叫做InvokerTransformer。

Apache是一个著名开源项目，而Commons Collections作为其重要组件，被广泛应用于各种Java应用的开发。Commons Collections提供了一个类包来拓展和增加标准的Java的collection框架，也就是说在其开发中，不可避免会遇到大量的序列化、反序列化的实现和调用，从而导致了反序列化漏洞利用的普遍性和严重性

!!! note "Java中的Collection"
	Java中的collection可以理解为set，list，queue的抽象，collection里面的对象称为collection的对象。

### 漏洞利用

如果一个类的方法被重写，那么在调用这个函数时，会优先调用经过修改的方法。因此， **如果某个可序列化的类重写了readObject()方法，并且在readObject()中对Map类型的变量进行了键值修改操作，且这个Map变量是可控的，我么就可以实现攻击目标**。

```java
public class test{
    public static void main(String args[]) throws Exception{
        //定义myObj对象
        MyObject myObj = new MyObject();
        myObj.name = "hi";
        //创建一个包含对象进行反序列化信息的”object”数据文件
        FileOutputStream fos = new FileOutputStream("object");
        ObjectOutputStream os = new ObjectOutputStream(fos);
        //writeObject()方法将myObj对象写入object文件
        os.writeObject(myObj);
        os.close();
        //从文件中反序列化obj对象
        FileInputStream fis = new FileInputStream("object");
        ObjectInputStream ois = new ObjectInputStream(fis);
        //恢复对象
        MyObject objectFromDisk = (MyObject)ois.readObject();
        System.out.println(objectFromDisk.name);
        ois.close();
    }
}

class MyObject implements Serializable{
    public String name;
    //重写readObject()方法
    private void readObject(java.io.ObjectInputStream in) throws IOException, ClassNotFoundException{
        //执行默认的readObject()方法
        in.defaultReadObject();
        //执行打开计算器程序命令
        Runtime.getRuntime().exec("open /Applications/Calculator.app/");
    }
}
```


我们注意到MyObject类重写了readObject方法，在原本功能外额外增加了 `Runtime.getRuntime().exec("open /Applications/Calculator.app/");` 偷偷把计算器打开了

!!! danger
	当然，这只是Java反序列化漏洞的基本原理，实际上基于此可以引申为更复杂的应用，在此不过多介绍 (懒，且暂时没有用处)

### 漏洞防范

- (1) **类白名单校验**
	- 在 ObjectInputStream 中 resolveClass 里只是进行了 class 是否能被 load ，自定义 ObjectInputStream , 重载 resolveClass 的方法，对 className 进行白名单校验
- (2) **禁止 JVM 执行外部命令 Runtime.exec**
	- 通过扩展 SecurityManager 实现



## 参考

- [https://blog.csdn.net/qq_37019068/article/details/120717474](https://blog.csdn.net/qq_37019068/article/details/120717474)
- [https://paper.seebug.org/312/](https://paper.seebug.org/312/)