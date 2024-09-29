
# 倒排文件索引

## Definition

一个 Inverted File Index 的例子如下：

![[IFIG.png]]

它以文章中出现的 Term 作为键值，以链表(?)的形式连接各个保存该 Term 出现信息的节点。以单词 shipment 为例，`<2;(1;2),(3;3,7)>` 最前面的 2 代表总共在两篇文章中出现过，括号 `(3;3,7)` 中，分号前的三是 Document 的标号，后面的 3 和 7 是该 Term 在文章中出现的位置。

其中，索引生成部分可以写成如下伪代码：

![[IFIM.png]]

## 提取 Term

当读取文章中的 Term 用作索引时，还需要：

- Word Stemming
	- 提取该 Term 的词根，如 says,said,saying 都看作词根 say
	- https://tartarus.org/martin/PorterStemmer/
	- https://github.com/wooorm/stmr.c
- Stop Words
	- 一些 Term 太过常见，没有价值，所以需要过滤它们
	- 搜索 Word Stop List 即可找到过滤列表

## 存储 Term

存储 Term 的数据结构按照需求一般选用 Search Tree 或 Hashing

当我们内存不足以放下所有索引时，可以采用如下几种方式解决：

### LRU

![[IFINEM.png]]

### Distributed indexing

在多台机器上，按照一定规则进行分布式存储

![[IFIDI.png]]

### Dynamic indexing

（好像不怎么涉及？）

![[IFIDYI.png]]

### Compression

压缩节点存储的数据。如不直接指向 Document 的编号，而是存储该 Document 编号与上一个 Document 的差值

![[IFICompression.png]]


## Measure of Search Engine


- **精确率** 是检索到的相关记录与检索到的记录总数的比率。
- **召回率** 是检索到的相关记录与相关记录总数的比值。

![[IFIMoSE.png]]

两个指标往往是不可兼得的，因此要求搜索引擎设计者衡量二者对用户体验的作用

![[IFIMoSEG.png]]

!!! danger "复习的时候重新看一遍HW3，太神经了"