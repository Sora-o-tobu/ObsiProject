
# Query Processing

!!! quote ""
	![[QueryProcessingOverview.png]]

Optimizer 做什么？

1. 应用等价变换
2. 生成多种等价执行计划
3. 使用静态策略评估各个计划的 Cost
4. 选择最有效率的执行计划


## Query Cost

Cost 通常通过时间来衡量，受到很多因素影响：

- Disk Access
- CPU
	- 本课程简单考虑
- Network Communication
	- 在分布式中受到影响

一般来讲 Disk Access 是主要的 Cost：

$$
b*t_T + S*t_S
$$

- $t_T$ ：time to transfer one block
	- 数据传输时间
	- e.g. 0.1 ms
- $t_S$ ：time for one seek
	- 读写头寻址时间
	- e.g. 4 ms

看作常量。因此我们实际的计算只与 $b$ 和 $S$ 有关。

!!! warning "本 Cost Model 中也没有考虑将最终结果写回磁盘需要的时间"

Cost 也依赖于操作系统给数据库分配的 Buffer。通常情况下，我们都以最坏情况下进行 Cost 的估计。

### SELECT

### JOIN

- Nested-Loop Join
- Block Nested-Loop Join
	- 以 Block 为单位的嵌套链接
- Indexed Nested-Loop Join
- Merge-Join
- Hash-Join

```c
for each tuple t_r in r do begin
	for each tuple t_s in s do begin
		test pair (t_r, t_s) if they satisfy the join condition
		if they do, add t_r * t_s to result
	end
end
```

在最坏情况，即 Memory 中只能放得下一个 Buffer 时，总共需要 $n_r *b_s + b_r$ 次 Block Transfer 以及 $n_r + b_r$ 次 Seeks。

!!! info "外层每选择一个 Record，内层遍历整个 File。遍历整个 File 只用一次 Seek"

