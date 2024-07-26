# index

平时分拿全，满绩很简单的一门课

!!! quote "ADT : The Abbreviation of Abstract Data Type"


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
