
# 倒排文件索引

## Definition

一个 Inverted File Index 的例子如下：

![[IFIG.png]]

它以文章中出现的 Term 作为键值，以链表(?)的形式连接各个保存该 Term 出现信息的节点。以单词 silver 为例，`<2;(1;2),(3;3,7)>` 最前面的 2 代表总共在两篇文章中出现过，括号 `(3;3,7)` 中，分号前的 3 是 Document 的标号，后面的 3 和 7 是该 Term 在文章中出现的位置。

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

- Hashing
    - 理想情况下，hashing的单个查询更快
    - Hashing的问题在于冲突以及不支持快速的范围查询。比如要查找一堆re开头的单词，会需要一个个算哈希值。
- 字典树 Tries
    - 每个叶节点都代表一个Term
    - 若单词有相同前缀，则会经过相同父节点
    - 复杂度就是单词的长度

![[invertedindexfiletries.png]]

当我们内存不足以放下所有索引时，可以采用如下几种方式解决：

### LRU

![[IFINEM.png]]

### Distributed indexing

在多台机器上，按照一定规则进行分布式存储

![[IFIDI.png]]

- 第一种存储很复杂，但是检索方便
- 第二种存储方便，但是检索很复杂
	- 一般采取 Solution 2，当一个设备损坏时，不会导致某个字母段数据全部丢失

### Dynamic indexing

将 index 分为 Main Index(快读慢写) 和 Auxiliary Index(慢读快写)。每当有新文档出现时先将其产生的 index 写入 Auxiliary Index 中。以一定周期(如一天)统一地将 Auxiliary Index 写入 Main Index 中。

但是搜索 Index 的时候要同时搜索 Main Index 和 Auxiliary Index。

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