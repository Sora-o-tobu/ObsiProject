
# 4. Storage Model & Compression

## Database Workloads

![[databaseworkloadfigure.png]]

<font style="font-weight: 1000;font-size: 20px" color="red">OLTP: Online Transaction Processing</font>

OLTP 工作负载的特点是快速且短时间运行的操作、单步对单个对象进行操作的简单请求、以及重复操作。

简单来说 OLTP 就是一些简单的请求，它们只会读写单个对象之中很少的数据，例如：

```sql
SELECT P.*, R.*
FROM pages AS P INNER JOIN revisions AS R
ON P.latest = R.revID
WHERE P.pageID = 65576;
```

<font style="font-weight: 1000;font-size: 20px" color="red">OLAP: Online Analytical Processing</font>

OLAP 将会是复杂的请求，它们将尝试从已经收集的现有数据库数据中推断出新的信息。

例如，以下请求尝试找出所有以 `.zju.edu.cn` 邮箱结尾登录的人：

```sql
SELECT COUNT(U.lastlogin),
	EXTRACT(month FROM U.lastlogin) AS month
FROM useracct AS U
WHERE U.hostname LIKE '%.zju.edu.cn'
GROUP BY EXTRACT(month FROM U.lastlogin);
```

在 OLAP 工作中，数据库系统将分析从 OLTP 端收集的现有数据。

!!! info "一个形象的例子"
	在 Amazon 页面，用户将商品添加到他们的购物车，这些操作只会影响自己的账户，属于 OLTP；但是 Amazon 如果要统计下雨天购买量最多的商品，这属于 OLAP 工作负载。

<font style="font-weight: 1000;font-size: 20px" color="red">HTAP: Hybrid Transaction + Analytical Processing</font>

一种新型工作负载，它尝试在同一个数据库上同时执行 OLTP 和 OLAP。

## Storage Models

Storage Model 就是数据库系统表示元组的方式。

### N-Ary Storage Model (NSM)

N-Ary 存储模型就是我们之前一直作为例子的模型：

![[NArymodels.png]]

它往往只对一个对象进行操作，因为只需要一次 fetch 就能获取单个 tuple 的全部属性。这种模型非常适合 OLTP 负载。

但是如果我们需要对其进行 OLAP 查询等扫描一个较大子集操作，获取到的结果包含每一个 tuple 的全部属性，而我们可能只需要其中的一小部分，导致无用的 I/O 消耗过多。

### Decomposition Storage Model (DSM)

DSM 则将单个属性（或者col）连续存储在同一个 page 中，因此，它也被称为 column store(列存储)。

![[Decompositionmodelfigure.png]]

此模型非常适合进行许多只读查询的 OLAP 负载。

为了能将各个 Page 中的属性对齐到同一个元组上还需要进行一些额外的策略：

- **Choice 1:** Fixed-length Offsets
	- 各个属性都等长，这样可以直接使用相同的 offset 调用同一个 tuple 的属性
- **Choice 2:** Embedded Tuple ID
	- 在 Page 中为属性内嵌一个 ID，这样可以通过 ID 来调用

## Database Compression

压缩广泛应用于基于磁盘的 DBMS，因为我们需要尽可能缩小 I/O 开销这个性能瓶颈。

尤其是在只读工作的负载中，如果 DBMS 已经事先压缩，则可以获取更多有用的 tuples，但同时需要更大的压缩、解压计算开销。

如果数据集是完全随机的数值位，那我们不能对其进行压缩。但是一般的数据集可能具有以下两个属性：

- <1> 属性值具有高度倾斜分布
	- 例如，对英文语言进行词频统计，使用最多的是 `the`
- <2> 同一元组的两个属性具有高度相关性
	- 例如，同一个城市的邮政编码是相同的

对于压缩完的数据，我们有以下三点目标：

- <1> 压缩后的值等长，方便通过 offset 引用
	- 对于非定长数据，将其放到额外的 pool 中，维护指向它们的指针
- <2> 在查询过程中尽可能晚地解压缩
	- 理想情况是，我们的 DBMS 能直接在压缩后的数据上运行
- <3> 必须是无损方案

### Naive Compression

朴素压缩是在 Block-Level 上进行压缩，即 Compress a block of tuples for the same table.

!!! note "这里的 Naive 意为数据库系统不能理解压缩后数据的含义"

DBMS 使用通用算法（gzip, LZO, LZ4, Snappy, Brotli, Oracle OZIP, Zstd 等）压缩数据，工程师一般倾向于使用压缩率更低但是压缩/解压更快的算法。

使用 Naive Compression 的一个例子是 MySQL InnoDB。它将一个 16KB 的 Page 压缩为 1,2,4,8 KB 大小的 Compressed Page。当我们需要对内部的 tuple 进行读取时，将该 Compressed Page 读取到 Buffer 中，然后进行解压缩。此时，Buffer Pool 中既有压缩后的 Page 也有解压后的 Page。


一个重要缺点是，这些朴素的压缩方案没有考虑数据的高级含义或语义，DBMS 无法判断数据的解压缩能推迟到多晚进行。


### Columnar Compression

- <1> **Run-Length Encoding**
	- 对重复、连续的属性进行聚类。通常需要 DBMS 事先对其进行智能排序。
	- 例如，假如有一性别列：`MMMFMFF` ，压缩后为 `M<0,3>, F<3,1>, M<4,1>, F<5,2>`
- <2> **Bit-Packing Encoding**
	- 例如，对于一列 `int64` 数据，如果它们都只用到了前八位，可以将它们压缩为 `int8`
- <3> **Mostly Encoding**
	- 对 mostly 情况进行 Bit-Packing Encoding 或其它编码，把异常值单独拿出作 lookup table
	- ![[mostlyencodingfigure.png]]
- <4> **Bitmap Encoding**
	- DBMS 为特定属性的每一种值都设置一个 Bitmap
- <5> **Delta Encoding**
	- ![[deltaencodingfigure.png]]
- <6> **Incremental Encoding**
	- 额外维护一个与前一个值相同的前缀的长度
	- ![[incrementalencodingfigure.png]]
- <7> **Dictionary Compression**
	- 字典压缩是最常用的数据库压缩方案，并且进行数据查询时，可以直接使用编码后的 code，不需要解压（数字的比较也通常比字符串的匹配更快）。
	- ![[dictionarycompressionfigure.png]]

