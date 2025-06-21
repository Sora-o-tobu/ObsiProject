# Misc 专题

## Misc 专题一 更多隐写

工具

- Steghide
- Zsteg
- StegSolve

### 图像格式

- 图像信息：宽高、色彩模式、色彩空间等
    - EXIF 信息：拍摄设备、拍摄时间、GPS 信息等
- 像素数据：每个像素的颜色信息；二值、灰度、RGB、CMYK、调色盘等
    - 对于标准 RGB 图像，每个像素需要 24 bits
    - 对于一张 1080p 图像，需要 6.22 MB，4K 则需要 24.88 MB
    - BMP 格式
- 图像格式的压缩算法
    - PNG 无损，JPEG 有损
    - GIF 有损且只支持 256 色
    - 新兴格式如 HEIF、WebP、AVIF 等

#### JPEG 文件结构
![[jpeg文件格式.png]]

JPEG 使用分段的结构来进行存储，各段以 0xFF 开头，后接一个字节表示类型：

- FFD8（SOI）：文件开始
- FFE0（APP0）：应用程序数据段，包含文件格式信息（上图没有）
- FFE1（APP1）：应用程序数据段，包含 EXIF 信息（上图没有）
- FFDB（DQT）：量化表数据
- FFC0（SOF）：帧数据，包含图像宽高、色彩模式等信息
- FFC4（DHT）：huffman 表数据
- FFDA（SOS）：扫描数据，包含数据的扫描方式，huffman 表的使用方式等
- FFD9（EOI）：文件结束

!!! info "压缩原理"
	JPEG采用DCT(离散余弦变换)和Huffman编码进行图片压缩

#### PNG 文件结构
![[png文件结构.png]]

- 文件头 89 50 4E 47 0D 0A 1A 0A | .PNG....
- 采用分块的方式存储数据
    - 每块的结构都是 4 字节长度 + 4 字节类型 + 数据 + 4 字节 CRC 校验
    - 四个标准数据块：IHDR、PLTE、IDAT、IEND
    - 其他辅助数据块：eXIf、tEXt、zTXt、tIME、gAMA……
        - eXIf 元信息，tIME 修改时间，tEXt 文本，zTXt 压缩文本

四种标准数据块：

- IHDR：包含图像基本信息，必须位于开头
    - 4 字节宽度 + 4 字节高度
	    - 通过修改图像的宽度高度数据，使得图像显示不全从而隐藏flag
	    - 修改宽高，还需要修改CRC校验码 (Windows校验码错了也能打开)
    - 1 字节位深度：1、2、4、8、16
    - 1 字节颜色类型：0 灰度，2 RGB，3 索引，4 灰度透明，6 RGB 透明
    - 1 字节压缩方式，1 字节滤波方式，均固定为 0
    - 1 字节扫描方式：0 非隔行扫描，1 Adam7 隔行扫描
- PLTE：调色板，只对索引颜色类型有用
- IDAT：图像数据，可以有多个，每个数据块最大 2 31 -1 字节
- IEND：文件结束标志，必须位于最后，内容固定
    - PNG 标准不允许 IEND 之后有数据块，但是位于IEND块之后的数据不会被显示，所以可以在这里随意藏数据

!!! info "压缩算法"
	PNG采用Deflate压缩算法 (一种LZ77结合Huffman编码的算法)


### ZIP 文件结构

先拿一个正常的压缩包举例，在下图中

- 前面的灰色背景的十六进制数字为 **压缩源文件数据区**，  
- 中间紫色背景的十六进制数字为 **压缩源文件目录区**，  
- 后面黄色背景的十六进制数字为 **压缩源文件目录结束标志**。

![[zip文件结构.png]]


接下来按顺序描述不同数据的含义：

==压缩源文件数据区：==

- `50 4b 03 04` 头文件标记
- `0A 00` 解压文件所需要的pkware版本
	- 有些题目会将这个版本调的很高，让你无法解压
- `00 00` 全局方式位加密
	- 通过此处判断有无加密
	- `0x 00` 如果x是偶数，则未加密；奇数，则加密
	- 和压缩文件目录区的全局方式位标记配合查看，如果一个是加密，一个不是，那么这就是伪加密
- `00 00` 压缩方式
- `72 98` 最后修改文件时间
- `C3 52` 最后修改文件日期
- `A7 DE 6A 5F` CRC-32 校验码
- `07 00 00 00` 压缩后尺寸
- `07 00 00 00` 未压缩尺寸
- `08 00` 文件名长度
- `00 00` 拓展记录长度

---

==压缩源文件目录区：==

- `50 4B 01 02` 目录中文件文件头标记
- `3F 00` 压缩使用的pkware版本
- `0A 00` 解压所需pkware版本
- `00 00` 全局方式位标记


---

==压缩源文件目录结束标志==

- `50 4B 05 06` 目录结束标记


### 音频文件结构

- mp3：有损压缩
    - 具体格式不多介绍，遇到了基本上也就是声音本身的隐写
- wav：无损无压缩（waveform）
    - 直接存储的是音频的波形数据，可操作性更高
    - 文件结构也是分 chunk 的，有 RIFF、fmt、data 等
    - 编码音频数据的 sample 也可以进行 LSB 隐写
- flac：无损压缩，如果出现可能考虑转换为 wav
- 使用 Python 的 soundfile / librosa 库进行音频处理
	- 版本兼容问题挺严重的，我搞不好这玩意



## Misc 专题二 流量取证与区块链基础

### UDP(DNS) 流量分析

#### 传输通道 DNS协议

!!! info "`DNS` 通常为 `UDP` 协议"

报文格式：

```
+-------------------------------+ 
| 报文头                         | 
+-------------------------------+ 
| 问题 (向服务器提出的查询部分)     | 
+-------------------------------+ 
| 回答 (服务器回复的资源记录)       | 
+-------------------------------+ 
| 授权 (权威的资源记录)            | 
+-------------------------------+ 
| 额外的 (额外的资源记录)          | 
+-------------------------------+
```

查询包只有头部和问题两个部分， `DNS` 收到查询包后，根据查询到的信息追加回答信息、授权机构、额外资源记录，并且修改了包头的相关标识再返回给客户端。

每个 `question` 部分:

```
   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5 
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
 |                    QNAME                      |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
 |                    QTYPE                      | 
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
 |                    QCLASS                     | 
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
```

- `QNAME` ：为查询的域名，是可变长的，编码格式为：将域名用. 号划分为多个部分，每个部分前面加上一个字节表示该部分的长度，最后加一个 `0` 字节表示结束
- `QTYPE` ：占 `16` 位，表示查询类型，共有 `16` 种，常用值有：`1` ( `A` 记录，请求主机 `IP` 地址)、`2` ( `NS` ，请求授权 `DNS` 服务器)、`5` ( `CNAME` 别名查询)

---

- 编码
	- 两方数据都以十六进制编码的字符串传输，例如 `AAA` 变为 `414141`
	- 忽略domain中的句点，即 `41.4141`， `414.141`和`414141`完全等价
	- 不区分大小写，因此 `5b` 和 `5B` 也是等价的
- 发送/接收
	- 对于数据，客户端可以选择为其添加自己的权威域名或在前面加上静态标签 (dnscat,使用UDP/53直连)，如：
		- `<encoded data>.<domain>`
		- `<tag>.<encoded data>`
	- 任何不是这种形式的数据，或者不是不受支持的记录中的数据键入，或者具有 DNSCAT 未知的附加域服务器，可以被服务器丢弃或转发到上游 DNS 服务器
	- dnscat2 服务器必须使用格式正确的 DNS 响应进行响应 直接发送到发出请求的主机，如果有多个answer，选择第一个
- 消息类型
	- dnscat 服务器支持最常见的 DNS 消息类型：`TXT`、`MX`、`CNAME`、`A`、`AAAA`

#### DNSCAT 协议

!!! quote "dnscat2是一个DNS隧道工具，通过DNS协议创建加密的命令和控制通道"

上面定义了通过DNS发送数据的 DNS Transport Protocol；下面是客户端和服务器交流的 dnscat protocol

- 会话由客户端向服务器发送一个 `SYN` 数据包并且服务器响应一个 `SYN` 数据包来启动。客户发送 `MSG` 数据包，服务器使用 `MSG` 数据包进行响应。
- 当客户端决定连接已结束，它会将 `FIN` 数据包发送到服务器，服务器以 `FIN` 数据包响应。
- 当服务器决定连接结束，它会用 `FIN` 响应来自客户端的 `MSG` 并且客户端不应再响应。

!!! note "A `flags` field is exchanged in the `SYN` packet. These flags affect the entire session."


良好的连接如下图所示：

```
+----------------+
| Client  Server |
+----------------+
|  SYN -->  |    |
|   |       v    |
|   |  <-- SYN   |
|   v       |    |
|  MSG -->  |    |
|   |       v    |
|   |  <-- MSG   |
|   v       |    |
|  MSG -->  |    |
|   |       v    |
|   |  <-- MSG   |
|  ...     ...   |
|  ...     ...   |
|  ...     ...   |
|   |       |    |
|   v       |    |
|  FIN -->  |    |
|           v    |
|      <-- FIN   |
+----------------+
```

如果服务器决定连接已结束，则服务器返回 `FIN` :

```
+----------------+
| Client  Server |
+----------------+
|  SYN -->  |    |
|   |       v    |
|   |  <-- SYN   |
|   v       |    |
|  MSG -->  |    |
|   |       v    |
|   |  <-- MSG   |
|   v       |    |
|  MSG -->  |    |
|   |       v    |
|   |  <-- FIN   |
|   v            |
| (nil)          |
+----------------+
```

如果收到意外的 `MSG`，服务器将响应错误 (`FIN`)：

```
+----------------+
| Client  Server |
+----------------+
|  MSG -->  |    |
|   |       v    |
|   |  <-- FIN   |
|   v            |
| (nil)          |
+----------------+
```

如果收到意外的 `FIN`，服务器将忽略它：

```
+----------------+
| Client  Server |
+----------------+
|  FIN -->  |    |
|           v    |
|         (nil)  |
+----------------+
```


**MESSAGE_TYPE_SYN \[0x00\]**

- (uint16_t) packet_id
- (uint8_t) message_type \[0x00\]
- (uint16_t) session_id
- (uint16_t) initial sequence number
- (uint16_t) options
- If OPT_NAME is set:
    - (ntstring) session_name

**MESSAGE_TYPE_MSG:\ [0x01\]**

- (uint16_t) packet_id
- (uint8_t) message_type \[0x01\]
- (uint16_t) session_id
- (uint16_t) seq
- (uint16_t) ack
- (byte\[\]) data


**MESSAGE_TYPE_FIN: \[0x02\]**

- (uint16_t) packet_id
- (uint8_t) message_type \[0x02\]
- (uint16_t) session_id
- (ntstring) reason

**MESSAGE_TYPE_ENC: \[0x03\]**

- (uint16_t) packet_id
- (uint8_t) message_type \[0x03\]
- (uint16_t) session_id
- (uint16_t) subtype
- (uint16_t) flags
- If subtype is ENC_SUBTYPE_INIT:
    - (byte\[32\]) public_key_x
    - (byte\[32\]) public_key_y
- If subtype is ENC_SUBTYPE_AUTH:
    - (byte\[32\]) authenticator

---

`SEQ` (sequence) and `ACK` (acknowledgement) numbers 用法和TCP中类似。在连接的开始，客户端和服务器都选择一个随机的 `ISN` (Initial Sequence Number) 并发送给对方。

客户端的 `SEQ` 就是服务器的 `ACK` ；服务器的 `SEQ` 就是客户端的 `ACK` 。这也意味着双方都知道预期的字节偏移量

### 内存取证
#### 基础介绍

- 内存取证就是从内存（RAM）中找到、提取有价值的信息
    - 系统运行中会大量使用内存，内存中会包含很多信息
    - 这些信息包括运行的进程、网络连接、内核模块、打开的文件等等
- 一般这种题目都会提供一个巨大的 raw 文件
    - 大小几乎等于靶机 RAM 的大小
    - 是直接 dump 出来的 raw 内存数据
        - 利用 DumpIt（Win）、LiME（Linux）等工具可以 dump 出内存
    - Windows 的居多（利用更丰富）
    - 通过工具分析内存数据，找到有效有价值的信息


#### volatility 工具的使用

!!! quote "[Volatility](https://volatilityfoundation.org/)是一款开源内存取证框架，能够对导出的内存镜像进行分析，通过获取内核数据结构，使用插件获取内存的详细情况以及系统的运行状态。"

- 查看 volatility 支持的 profile、插件等
    - volatility --info
- 查看内存可能的系统版本以及对应的 volatility profile
    - volatility -f mem.raw imageinfo
    - 可能会有多个 profile，都试一试
- volatility -f <mem_file\> --profile=\<profile\> \<command\>
    - 列出进程：pslist psxview psscan pstree
	- 列出命令行信息等：cmdscan consoles cmdline
	- 列出环境变量：envars
	- 列出网络连接：netscan connscan
	- 文件相关：filescan dumpfiles mftparser
		- volatility -f mem.raw --profile=\<profile\> dumpfiles -Q \<Offset\> ./
	- 注册表相关：hivelist hivedump printkey
	- 服务运行状态：svcscan
	- 获取进程内存：memdump -p \<pid\> -D \<output_dir\>
	- 用户密码信息：hashdump（存的是 hash，得不到原密码）
	- 其他：editbox privs iehistory clipboard notepad userassist screenshot

!!! note "如果得到加密卷的话需要通过 cncrypt 等软件打开"

### 以太坊区块链基础

#### 以太坊模型

!!! tip ""
	![[区块与世界状态.png]]

- 每个区块包含三颗 Merkle 树根节点
    - `stateRoot` 即世界状态树根节点，状态是一组用户状态的组合
- 区块由“矿工”或“验证者”将交易打包形成，后广播到网络中
    - 每条交易会引发世界状态的转变，消耗一定 gas

---

- 公开链：真实的交易
    - 通过 [https://etherscan.io/](https://etherscan.io/) 查看
    - 主网（mainnet）：真正的金钱交易，很少使用
    - 测试链：Sepolia / Holesky 链，可以通过 faucet 获取免费代币
        - [https://sepolia-faucet.pk910.de/](https://sepolia-faucet.pk910.de/)
        - Ethernaut 等大型公开合约 CTF 平台会使用
- 私链：自己搭建的链，模拟真实的链
    - 一般 CTF 题目都使用私链部署
    - 可以通过 geth 等工具部署私链

#### 账户
- 外部账户（Externally Owned Account）
    - 有一对公私钥，用于签署交易
        - 私钥是随机生成的 256 位数（32 字节）
        - 公钥由私钥经过 ECDSA 算法计算而来，是一个 64 字节的数
        - 地址由公钥经过 Keccak-256 哈希后取前 20 字节得到

!!! note "安全性"
	privatekey 和 publickey 都是私密的，只有 address 是公开的
	![[公钥私钥地址.png]]

- 合约账户（Contract Account）
    - 由 EOA 通过交易创建的账户，其中包含合约代码
    - 合约可以存储、拥有以太币
    - 向合约账户发送交易相当于调用合约中的函数
    - 合约本身不能主动发起交易，但可以在被调用时向外发送交易

#### 交易

!!! danger ""
	![[交易与世界状态改变.png]]

- 每一条交易都会引起状态的改变
- 多个交易打包到一起，最终状态就是新区块存储的状态
- 交易信息中包含 hash/v/r/s 为交易签名，用于验证交易的合法性
- 合约在 EVM 上执行，执行过程中也有各种漏洞

!!! note "EVM 是一个栈结构的虚拟机 (Ethereum Virtual Machine)"

- 一条交易包含以下内容：
    - from：交易发送者地址
    - to：交易接收者地址，如果为空则表示是在创建智能合约
    - value：交易金额，即发送方要给接收方转移的以太币数量（wei 为单位）
    - data：交易数据，如果是创建智能合约则是智能合约代码，如果是调用智能合约则是调用的函数名和参数
    - gasPrice：交易的 gas 价格，即每单位 gas 的价格（wei 为单位）
    - gasLimit：交易的 gas 上限，即交易允许执行的最大 gas 数量
    - nonce：交易的序号，即发送者已经发送的交易数量
- 除此之外发送的交易数据包还需要包含：
    - hash：交易的哈希值，由前面的内容和 chainId 计算得到
    - v、r、s：交易签名的三个部分，由发送者私钥对交易哈希值进行签名得到

以太币单位换算网站： [Ethereum Unit Converter](https://converter.murkin.me/)

以太坊的交易可以分为三种场景：

- 转账交易：从一个账户向另一个账户发送以太币
    - 必须要指定 from、to、value，分别表示发送者、接收者、转移的以太币数量（其它字段有默认值）
    - 交易数据包中的 data 为空
    - 接收方可以是 EOA 也可以是合约账户
- 创建合约交易：创建一个合约账户（将合约部署到链上）
    - 必须要指定 from、data，分别表示发送者、合约代码（编译后的字节码）
    - 交易数据包中的 to 为空
- 调用合约交易：调用一个合约账户的函数
    - 必须要指定 from、to、data，分别表示发送者、接收者、调用的信息
    - data 字段是编码后的函数名（选择子）和参数

#### 智能合约安全


[Solidity 语言](https://docs.soliditylang.org/en/latest/index.html)是以太坊官方的编写智能合约的语言

- IDE：[https://remix.ethereum.org/](https://remix.ethereum.org/)
- 通过 contract 关键字声明一个合约
- 通过 function 定义一个可以调用的函数
    - public、internal、external、private
    - 属性（状态）会自动创建 getter 函数
    - 通过 view、pure 关键字定义一个不改变状态的函数
- 通过 payable 关键字定义一个可以接收以太币的函数
- 特殊函数：constructor、fallback、receive

!!! tip "[TonyCrane的Solidity笔记](https://note.tonycrane.cc/ctf/blockchain/eth/solidity/)"

##### 常见漏洞 之 整型溢出

[Ethernaut](https://ethernaut.openzeppelin.com/) 的题目 `Token`：

```Solidity
mapping(address => uint256) balances; 
function transfer(address _to, uint256 _value) public returns (bool) { 
	require(balances[msg.sender] - _value >= 0);
	balances[msg.sender] -= _value; balances[_to] += _value;
	return true; 
}
```

- balances 字典是 uint256，无符号减法有溢出风险
- 给任意地址转账，原余额 20 Token，转 21 个，则 balances 会变的巨大
- 通过事先检查或者使用 SafeMath 库可以避免
- 新版本 solidity 自动开启了溢出检查

##### 常见漏洞 之 重入攻击

[Ethernaut](https://ethernaut.openzeppelin.com/) 的题目 `Re-entrancy`，很经典的智能合约漏洞

```Solidity
contract Bank { 
	mapping(address => uint256) balances; 
	... 
	function withdraw(uint256 amount) public { 
		require(balances[msg.sender] >= amount); 
		msg.sender.call.value(amount)(""); 
		balances[msg.sender] -= amount; 
	} 
}
```

- withdraw 时先转钱再更新 balances
- 转钱的时候会进入到目标合约的 fallback 函数，可以再次调用 withdraw
- 再次调用时 require 检查时，balances还没有更新
- 这样可以把钱取空

##### 常见漏洞 之 伪随机数

- 区块链需要所有以太坊节点验证交易计算出相同结果达成共识
- 无法实现真随机数
- 伪随机可以破解：
    - 利用区块变量作为随机数：可以获取
    - 利用 blockhash 作为随机数：只保留最近 256 个区块
    - 回滚攻击：不断 revert 来猜随机数

[Ethernaut](https://ethernaut.openzeppelin.com/) 的题目 `Coin Flip`

该题目只需要根据源代码中生成随机数的方法进行复现即可，可以藉此学习 Remix IDE 与测试链的交互 (Beyond The Console)

#### CTF 比赛中的私链题目交互

- 一般会过滤掉大部分 geth rpc 接口
    - 防止其他队伍扒链蹭车 / 重放
    - 白名单示例可见 [chainflag/solidctf](https://github.com/chainflag/solidctf/blob/main/fogeth/proxy/eth-jsonrpc-access.js) 中的白名单，一般就是这些
    - geth 手动操作很复杂（只能发 raw），remix/metamask 可能会无法连接
- 可以 / 推荐通过 [web3.py](https://pypi.org/project/web3/) 进行交互
    - 通过 eth.contract 和 abi 与 addr/bytecode 创建合约对象
    - 通过 contract.functions.f().build_transaction() 构建交易
    - 通过 eth.account.sign_transaction(txn, privateKey) 签名
    - 得到 rawTransaction 后 eth.send_raw_transaction(raw) 发送
    - 通过 eth.wait_for_transaction_receipt(hash) 等待交易完成
    - 无需交易的 view 函数可以直接 contract.functions.f().call()

#### Web3.py 交互

非常推荐使用 `Web3.py` 进行交互，以某 Lab 一个题目的代码为例子：

- 创捷合约实例
	- `SimpleStorage = w3.eth.contract(abi=contract_abi, bytecode=contract_bytecode)`
- 构建部署交易
```python 
	transaction = SimpleStorage.constructor().build_transaction({
	    'from': Account,
	    'nonce': w3.eth.get_transaction_count(Account),
	    'gas': 2000000,
	    'gasPrice': w3.to_wei('50', 'gwei')
	})
```
- 私钥签名
	- `signed_txn = w3.eth.account.sign_transaction(transaction, privateKey)`
- 发送交易、获取交易回执
	- `txn_hash = w3.eth.send_raw_transaction(signed_txn.rawTransaction)`
	- `txn_receipt = w3.eth.wait_for_transaction_receipt(txn_hash)`
- 获取合约地址
	- `contract_address = txn_receipt.contractAddress`
- 创捷已部署合约的实例
	- `simple_storage = w3.eth.contract(address=contract_address, abi=contract_abi)`
- 构建 geteth() 函数
	- 要记得加 `value: w3.to_wei(0.001,'ether')
```python
	set_txn = simple_storage.functions.geteth().build_transaction({
	    'from': Account,
	    'value': w3.to_wei(0.001, 'ether'),
	    'nonce': w3.eth.get_transaction_count(Account),
	    'gas': 200000,
	    'gasPrice': w3.to_wei('50', 'gwei')
	})
```
- 调用 geteth() 函数
	- `stored_data = simple_storage.functions.geteth().call()`

