# Java 反序列化漏洞

!!! quote "应课程作业要求，写一篇关于反序列化漏洞的笔记"

## 序列化和反序列化
!!! abstact "含义"
	序列化就是把对象转换为字节序列(即可以存储或传输的形式)的过程，而反序列化则是他的逆操作，即把一个字节序列还原为对象的过程。
	
	这两个操作一般用于对象的保存和网络传输对象的字节序列等场景。

Java中，实现序列化和反序列化的类分别位于 `java.io.ObjectOutputStream` 和 `java.io.ObjectInputStream` 中

- **序列化** ObjectOutputStream类 => writeObject()
	- 该方法对参数指定的obj对象进行序列化，把字节序列写到一个目标输出流中，按Java的标准约定是给文件一个.ser扩展名
	- 流程：
		- 1. 创建一个对象输出流(也可以是文件输出流)
		- 2. 通过对象输出流的writeObject方法将对象序列化写入输出流
- **反序列化** ObjectInputStream类 --> readObject()
	- 该方法从一个源输入流中读取字节序列，再把它们反序列化为一个对象，并将其返回。
	- 流程：
		- 1. 创建一个对象输出流(也可以是文件输出流)
		- 2. 通过对象输出流的readObject方法将对象反序列化写入输出流

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

