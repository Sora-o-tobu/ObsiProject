
# Greedy Algorithm

贪心算法在每个决策阶段选择当前阶段可行的最优解。其核心思想在于通过局部最优选择达成全局最优，但是贪心算法并不能保证全局最优性。

!!! warning "Greedy algorithm works only if the local optimum is equal to the global optimum"

## 经典案例：Activity Selection Problem

有一个同时段只能举办一场活动的活动场地，在该场地安排一组活动 $S=\{a_1, a_2, a_3,..., a_n\}$ ，每个活动起始结束时间为 $[s_i, f_i)$ ，并且两个活动之间不能有交叉。要求在该活动场地中给出最多能举办的活动数。

对于该问题，贪心算法的原则是局部决策选取结束时间最早、且与目前已确定举办的活动无时间冲突的活动。

因此，对于贪心算法，我们仅需要对活动按照结束时间进行排序，再对其进行一次遍历即可得到结果。其中，排序时间复杂度为 $O(N\log N)$ ，遍历仅需 $O(N)$ ，总时间复杂度为 $O(N\log N)$。

```c
Recursive_activity_selector(s, f, k, n)
{
	m = k + 1;
	while (m <= n && s[m] < f[k])
		++m;
	if (m <= n)
		return {a_m} ∪ Recursive_activity_selector(s,f,m,n);
	else
		return {∅};
}

sort(s,f,n); // sort by f
Recursive_activity_selector(s,f,0,n);

---

Iterative_activity_seletor(s, f)
{
	n = s.length();
	A = {a_1};
	k = 1;
	for(m = 2;m < n;++m)
		if (s[m] >= f[k]){
			A = A ∪ {a_m};
			k = m;
		}
	return A;
}
```


!!! failure "使用 DP 算法，则需要 $O(N^2)$ 时间复杂度"

正确性证明：

- <1> 每一步决策是可行的
- <2> 结果是最优的

## 经典案例：哈夫曼编码

构造一个优先队列，每次操作取出两个根值最小的节点合并，新根值为二者之后，然后再入队。重复操作，知道队列只剩一个节点，所得的树即为哈夫曼编码树。

```c
void Huffman(PriorityQueue heap[] = C)
{
	n = |C|;
	for i = 1 to n - 1:
		allocate a new node z;
		z.left = x = ExtractMin(C);
		z.right = y = ExtractMin(C);
		z.freq = x.freq + y.freq;
		Insert(z);
	return ExtractMin(C);
}
```

!!! danger "正确性证明怎么办呢🥺"