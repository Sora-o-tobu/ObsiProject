
# Leetcode 题目记录

!!! quote "打算偶尔去 Leetcode 练一练算法思想，所以在此记录一些题目的注意点或是思路"

## 1.两数之和

作为力扣的第一题难度还是很简单的，但是用C语言写的时候给出的接口为 `int* twoSum(int* nums, int numsSize, int target, int* returnSize)` 

一开始没搞懂 returnSize 的含义，它其实是你最终结果的长度，也就是 **2** 或 0

如果只是希望 $O(N^2)$ 解决的话，需要注意存放结果的数组需要用动态分配的方式初始化，不然会爆；另外记得没有符合条件的结果时也要有返回值

参考代码：

```c
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int *result=(int*)malloc(sizeof(int) * 2);
    for(int i = 0;i<numsSize;i++){
        for(int j = i+1;j<numsSize;j++){
            if(nums[i] + nums[j] == target){
                result[0] = i;
                result[1] = j;
                *returnSize = 2;
                return result;
            }
        }
    }*returnSize = 0;
    return NULL;
}
```


可以看出来效率非常一般，以后有时间了解一下更高效的解法 （其实就是哈希表）

## 2.两数相加

也是很简单的题目，按照全加器的思想写个 while 循环即可解决，随手写了份没优化的就通过了，还击败了 43% ...

参考代码：

```c
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode* result = (struct ListNode*)malloc(sizeof(struct ListNode));
    struct ListNode* start = result;
    struct ListNode* pre = result;
    int carry=0;
    int sum=0;
    while(l1!=NULL && l2!=NULL){
        sum = l1->val + l2->val + carry;
        carry = sum / 10;
        sum -= carry*10;
        result->val = sum;
        l1 = l1->next;
        l2 = l2->next;
        pre = result;
        result->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        result = result->next;
    }
    while(l1!=NULL){
        sum = l1->val + carry;
        carry = sum / 10;
        sum -= carry*10;
        result->val = sum;
        l1 = l1->next;
        pre = result;
        result->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        result = result->next;
    }
    while(l2!=NULL){
        sum =l2->val + carry;
        carry = sum / 10;
        sum -= carry*10;
        result->val = sum;
        l2 = l2->next;
        pre = result;
        result->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        result = result->next;
    }if(carry != 0){
        result->val = 1;
        result->next = NULL;
    }else{
        pre->next = NULL;
    }return start;
}
```

好孩子不要写屎山哦

看了下官方题解，修改成以下主体结构看起来确实优美很多：

```C
    while (l1 || l2) {
        int n1 = l1 ? l1->val : 0;
        int n2 = l2 ? l2->val : 0;
        int sum = n1 + n2 + carry;

        /*Main Code Omitted*/

        carry = sum / 10;
        if (l1)
            l1 = l1->next;
        if (l2)
            l2 = l2->next;
    }
    if (carry > 0) {
        tail->next = malloc(sizeof(struct ListNode));
        tail->next->val = carry;
        tail->next->next = NULL;
    }
    // 注意最后的进位不要忘记
```

## 3.无重复的最长子串

由于 ASCII 总共就 128 个，所以直接采用容量为 128 的表来存储对应字符是否已经出现过了

表中的数值是当前字符上一次出现的位置的 index +1 ，+1 是为了免去 `!store[(int)s[i]]` 的检测

每次检测到重复，就把表清空，并从上一个重复元素处重新检测子串

这样最坏时间复杂度都有 $O(N^2)$ 了，哈哈

```c
int lengthOfLongestSubstring(char* s) {
    int store[128];
    int length = 0, maxlength = 0;
    memset(store,0,sizeof(store));
    for(int i=0;i<strlen(s);++i){
        if(!store[(int)s[i]]){
            store[(int)s[i]] = i+1; 
            // index 为 0 被刷掉了，因此这里存的数值要大于等于 1
            ++length;
        }else{
            i = store[(int)s[i]]-1;
            memset(store,0,sizeof(store));
            maxlength = (maxlength > length)?maxlength:length;
            length = 0;
        }
    }return (maxlength > length)?maxlength:length;
    // 有可能整个字符串都没有重复元素
}
```

可以换个思路，`store` 表中存的不是 index ，而是该元素出现的次数；使用 `start` 和 `end` 维护子串的开始和结束位置：

```c
int lengthOfLongestSubstring(char* s) {
    int store[128];
    int length = 0, maxlength = 0;
    int start = 0, end = 0;
    memset(store,0,sizeof(store));
    for(;end<strlen(s);++end){
        ++ store[(int)s[end]];
        ++ length;
        while(store[s[end]] > 1){
            -- store[s[start++]];
            -- length;
        }
        maxlength = (maxlength > length)?maxlength:length;
    }return (maxlength > length)?maxlength:length;
}
```

我也不清楚为什么，总之消耗一下子降下来了！

!!! warning "其实是性能杀手 `memset` 搞的鬼！ (985ms -> 12ms)"

## 3.5 Manager of Tasks

其实是 ADS 模拟卷里的一个题（）

There are N tasks arranged in a sequence on a machine waiting to be executed, and their order cannot be changed. You need to divide these N tasks into several groups, each containing several consecutive tasks. Starting from time 0, the tasks are processed in groups, and the time required to execute the i-th task is Ti​. Additionally, the machine requires a startup time S before each group of tasks begins, so the time required for a group of tasks is the startup time S plus the sum of the time required for each task in this group.

After a task is executed, it will wait briefly in the machine until all tasks in that group are completely executed. That is to say, the tasks in the same group will be completed at the same time. The cost of each task is its completion time multiplied by a cost coefficient Ci.

Please plan a grouping scheme for the machine to minimize the total cost.

For all testing data, 1≤N≤1000,0≤S≤50,1≤Ti​,Ci​≤100

简单来讲就是有一个长度为 N 、先后关系要求不变的 Task 序列，要分配在一个处理器上执行。处理器可以将这些 Task 分成不同长度 group，同一 group 内的 Task 同时完成，且完成时间等于这些 Task 所需的时间和并加上一个 Setup Time(S)；而后面的组的时间计算要从前一个组的末尾开始。求这组任务最少的 Cost。

```text
Sample input:
5
1
1 3
3 2
4 3
2 3
1 4

Sample Output:
153

Sample Explanation:
We have grouped the tasks into 3 groups, which are `{1, 2}, {3}, {4, 5}`. The completion time corresponding to each task, in the order of the task numbers, is `{5, 5, 10, 14, 14}`. Similarly, the cost corresponding to each task, again in the order of the task numbers, is `{15, 10, 30, 42, 56}`. The total cost of these tasks is 153.
```

```c
#include <stdio.h>
#include <limits.h>

#define MAXN 1000

long long min_cost(int N, int S, int T[], int C[]);

int main() {
    int N, S;
    int T[MAXN], C[MAXN];
    scanf("%d%d", &N, &S);
    for (int i = 0;i < N; ++ i) {
        scanf("%d%d", &T[i], &C[i]);
    }
    printf("%lld\n", min_cost(N, S, T, C));
    return 0;
}

long long min_cost(int N, int S, int T[], int C[]) {
    long long dp[MAXN + 1];
    long long dptime[MAXN + 1] = {0};
    long long sumT[MAXN + 1] = {0}; // 前缀和：任务执行时间
    long long sumC[MAXN + 1] = {0}; // 前缀和：成本系数

    // 计算前缀和
    for (int i = 1; i <= N; i++) {
        sumT[i] = sumT[i - 1] + T[i - 1];
        sumC[i] = sumC[i - 1] + C[i - 1];
    }

    // 初始化 dp 数组
    dp[0] = 0; // 0 个任务的成本为 0
    for (int i = 1; i <= N; i++) {
        dp[i] = LLONG_MAX;
    }

    // 动态规划求解
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= i; j++) {
            // 以 j 为起点，i 为终点的任务组
            long long group_time = S + (sumT[i] - sumT[j - 1]) + dptime[j - 1];
            long long group_cost = (sumC[i] - sumC[j - 1]) * group_time;

            // 更新 dp[i]
            if( dp[i] + dptime[i] * (sumC[N] - sumC[i]) > dp[j - 1] + group_cost + group_time * (sumC[N] - sumC[i]))
            {
                dp[i] = dp[j - 1] + group_cost;
                dptime[i] = group_time;
            }
        }
    }

    return dp[N];
}
```

其实困住我的难点在于 `dp[i] + dptime[i] * (sumC[N] - sumC[i]) > dp[j - 1] + group_cost + group_time * (sumC[N] - sumC[i])` 这句判断条件的确定。一开始没能注意到，如果条件仅设为 `dp[i] > dp[j-1] + group_cost` 的话，该算法会将每一个 Task 都单独设置成一个 Group，从而陷入了局部最优解的陷阱！因此我们要高瞻远瞩，考虑多个 Group 带来的 Setup Time 对后面的组别的 Cost 的影响。