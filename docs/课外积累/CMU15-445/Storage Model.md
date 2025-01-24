
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

在 Amazon 页面，用户将商品添加到他们的购物车，这些操作只会影响自己的账户，属于 OLTP；但是 Amazon 如果要统计下雨天购买量最多的商品，这属于 OLAP 工作负载。

<font style="font-weight: 1000;font-size: 20px" color="red">HTAP: Hybrid Transaction + Analytical Processing</font>

一种新型工作负载，它尝试在同一个数据库上同时执行 OLTP 和 OLAP。

## Storage Models

Storage Model 就是数据库系统表示元组的方式。

### N-Ary Storage Model (NSM)



### Decomposition Storage Model (DSM)


## Database Compression

