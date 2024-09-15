
# Leetcode 题目记录

!!! quote "打算偶尔去 Leetcode 练一练算法思想，所以在此记录一些题目的注意点或是思路"

## 1.两数之和

作为力扣的第一题难度还是很简单的，但是用C语言写的时候给出的接口为 `int* twoSum(int* nums, int numsSize, int target, int* returnSize)` 

一开始没搞懂 returnSize 的含义，它其实是你最终结果的长度，也就是 **2** 或 0

如果只是希望 $O(N^2)$ 解决的话，需要注意存放结果的数组需要用动态分配的方式初始化，不然会爆

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

可以看出来效率非常一般，以后有时间了解一下更高效的解法

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

