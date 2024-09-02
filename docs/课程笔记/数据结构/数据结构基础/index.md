# index

平时分拿全，满绩很简单的一门课

!!! quote "ADT : The Abbreviation of Abstract Data Type"

## 评分标准

- 日常作业 **10%**
- 两次小测 **10%**
- 期中考试 **15%**
- 两个 Bonus 题目 **4%**
- 三个 Project **25/30%**
	- 选 Hard 才有5分上限加分，但是上面所有分数合起来不超过 **60%** (平时分)
	- 真是互互又评评啊
- 期末考试 **40%**

!!! info "期末考试可以覆盖期中考试成绩(我就是期末96覆盖了)"

## Pandoc

使用 pandoc 制作实验报告：

```
pandoc .\Project.md -s --highlight-style tango -o pr.pdf （--pdf-engine=xelatex）
```

如果需要pandoc转换中文字符，请在md文件开头加入:

```
---
documentclass:
	- ctexart
---
```
