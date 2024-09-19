
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

![[两数之和.png]]

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

