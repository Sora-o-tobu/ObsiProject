
!!! quote "Misc 即杂项"
	- 隐写、取证、OSINT(信息收集)、PPC(编程类)
	- 游戏类题目、工具运用类题目
	- 编解码、古典密码    ——不那么Crypto的Crypto
	- 网络解密、网站代码审计    ——不那么Web的Web
	- 代码审计、沙箱逃逸    ——不那么Binary的Binary
	- Blockchain、IoT、AI

## 基础编解码知识

- 一切信息在计算机看来都是0和1
	- 编解码/加解密/哈希都是在 01 串之间变换
- 为什么你看见的输入是字符？
	- 计算机通过字符编码规则将 01 串转化为了可见字符


!!! info "三种常见的 01 串转换方式"
	![[常见的01串变换.png]]

### 常见的字符编码

!!! warning "为什么会出现乱码"
	用一种字符编码规则解读另一种字符编码的 01 串，就有可能出现乱码

- ASCII码：一共128项，每个字符可以用一个 7 位的 01 串表示（或一字节）
	- ASCII码的第八位一般用于奇偶校验位，但许多基于x86的系统都支持使用拓展ASCII码，将每个字符的第八位用于确定附加的128个特殊符号字符、外来语字母和图形符号
	- 00-1F：控制字符；20-7E：可见字符；7F：控制字符（DEL）
- Latin-1（ISO-8859-1）：扩展了 ASCII，一共 256 个项
	- 80-9F：控制字符；A0-FF：可见字符
	- 特点：任何字节流都可以用其解码
- 中国国标字符集系列编码
	- GB 2312 / GBK / GB 18030-2022
- 利用 [Unicode](https://note.tonycrane.cc/cs/unicode/) 字符集的一系列编码
	- UTF-8 / UTF-16 / UTF-32 / UCS
	- UTF-8是目前最常用的编码方式，它是可变宽度字符编码，每个字符被编码成1到4字节，每个字节都是由开头的几个比特前缀(一般需要几个字节就有几个1)和剩余的数据部分组成 ==兼容ASCII==
		- ![[UTF-8编码方式.png]]
	- UTF-16也是可变宽度，它将一个字符编码成1~2个16比特长的码元(亦即2/4字节) ==不兼容ASCII==

!!! info "编码的兼容性"
	![[编码的兼容性.png]]

??? info "GB系列补充"
	- GB 2312使用分区管理，共计94个区，每个区含94个码位，共8836个码位
		- 01-09区收录除汉字外的682个字符
		- 10-15区为空白区
		- 16-55区收录3755个一级汉字，按拼音排序
		- 56-87区收录3008个二级汉字，按部首/笔画排序
		- 88-94为空白区
		- ![[GB2312.png]]
		- 编码举例：汉字 `侃` 在57区09位，分别转成16进制为 0x39,0x09，再加上0xA0即可得到GB2312编码 `0xD90xA9` 
			- 因为GB 2312编码开头都大于0xA0，所以与ASCII码兼容
	- GBK不再规定低位要大于127，新增了20000个汉字和符号
	- GB 18030采取混合编码，新增了几千多个少数民族字符

常见的字符集不兼容的部分互相编解码：

- 用 GBK 解码 UTF-8 编码的文本
- 用 UTF-8 解码 GBK 编码的文本
- 用 latin-1 解码 UTF-8 编码的文本
- 用 latin-1 解码 GBK 编码的文本
- 先用 GBK 解码 UTF-8 编码的文本，再用 UTF-8 解码前面的结果
- 先用 UTF-8 解码 GBK 编码的文本，再用 GBK 解码前面的结果

!!! tip "� 是UTF-8的'非预期'的意思"

---

上述都是 01 串 $\Leftrightarrow$  字符，接下来看一种 字符 $\Leftrightarrow$  字符

- 摩尔斯电码(Morse Code)：利用点划来表示字符
	- 点 · ：1单位； 划 - ：3单位
	- 点划之间间隔：1 单位；字符之间间隔：3 单位；单词之间间隔：7 单位
- 字符集：A-Z、0-9、标点符号（.:,;?='/!-\_"()$&@+）
- 表示中文：电码表（一个汉字对应四个数字），数字使用短码发送
- ![[摩尔斯电码.png]]

---

接下来是 01 串 $\Leftrightarrow$ 01 串，即Base编码家族

- Base16：即16进制表示字节流，长度翻倍
- Base32：按照 5 bit一组(每个 0-31)，按照字符表（A-Z2-7）映射
	- 结果长度必须是 5 的倍数，不足的用 = 不齐（明显特征）
- Base64：按照 6 bit 一组，按照字符表映射（最常用）
	- 标准字符表：A-Za-z0-9+/
	- 另有多种常用字符表，如 URL 安全字符表：A-Za-z0-9-_
	- 结果长度必须是 4 的倍数，不足的用 = 不齐（1~2 个，明显特征）

!!! info "分别用Base32和Base64编码AAA"
	![[Base编码.png]]

Base-n 系列本质：字节流 -> 整数 -> n 进制 -> 系数查表

所以除去前面的整规则，还有一些其它的Base编码

- 分组：
    - Base85：4 字节整数 -> 85 进制 -> 5 个系数
        - 常用字符表：0-9A-Za-z!#$%&()\*+-;<=>?@^\_\`{|}~
        - 标准字符表：!-u（ASCII 编码中 0x21-0x75）
- 作为大整数转换进制：
    - Base62：0-9A-Za-z（比 Base64 少了 +/）
    - Base58：0-9A-Za-z 去除 0OIl
    - Base56：比 Base58 少了 1 和 o
    - Base36：0-9A-Z（比 Base62 少了 a-z）

## OSINT 基础

!!! abstract "Open Source INTelligence：开源网络情报"

- 通过完全公开的信息进行合理的推理，获取情报
- 一般在 misc 题目中出现即泛指信息搜集，有几种情况：
    - 构造了一个全新的虚拟身份，搜集得到出题人准备好的信息
    - 根据图片、文档等附件泄漏的信息进行推理 **（主要）**
        - 包括根据图片内容推理找到拍摄位置、当时环境等信息

---
### 信息搜查/查询基础

- 一系列数字信息搜集工具：
    - [https://github.com/ffffffff0x/Digital-Privacy](https://github.com/ffffffff0x/Digital-Privacy)
- 用户名批量查询：
    - sherlock: [https://github.com/sherlock-project/sherlock](https://github.com/sherlock-project/sherlock)
    - namechk: [https://namechk.com/](https://namechk.com/)
- 高级搜索方式
    - filetype:...
    - site:...
    - inurl:... intitle:... intext:...
- Wayback Machine [web.archive.org](http://web.archive.org/)
    - 查找网页的历史快照（以及可以创建快照）
    - 有时可以找到出题人特意保存快照后删除的内容

### 文件信息泄露

- 各种文档的元信息（metadata）可能包括作者、修改时间等信息
    - 图片的 EXIF 信息，可通过 exiftool 查看
    - 一般以 xml 形式存储，可以直接通过二进制抹除，或者通过操作系统
- 工程文件夹泄漏信息
    - Visual Studio 的各种配置文件，.vs 文件夹中信息
    - .vscode 文件夹中的配置文件
    - .git 文件夹，泄漏全部修改历史、提交信息、提交者等
- 文件夹路径信息泄漏
    - .DS_Store 文件，macOS 下的文件夹布局信息
    - 前面各种工程配置文件等也会泄漏（比如 vs 的 pdb 调试信息）
    - markdown 文件图片路径（本地路径 / 图床用户 / 自建图床网站）

### 照片信息分析 - 图片搜索

一些常用的搜索引擎：

- 百度识图搜索：中文互联网图片搜索结果
- Google 图片搜索：用来搜索外国范围的图片
- Bing 图片搜索：和 Google 差不多，都可以参考
- Yandex 图片搜索：
    - 搜索相似图片
    - 搜索风景时更常用
- TinEye：搜索完全相同的图片（找来源）

---

其他环境信息的分析：

- 太阳角度、阴影长度等太阳相关
    - 时间 <=> 位置互相估计
    - [suncalc.org](https://www.suncalc.org/)
    - [sunearthtools.com](https://www.sunearthtools.com/cn/index.php)
- 天气信息、云层信息等
- 飞机航班信息
    - 估计方向，位置，时间等
    - [flightaware.com](https://flightaware.com/)
    - [flightradar24.com](https://www.flightradar24.com/)
    - [adsbexchange.com](https://www.adsbexchange.com/)
- 风景信息 -> Yandex 搜索


