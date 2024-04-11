电路优化
# Canonical Forms 规范型
**Two common Canonical Forms:**
- Sum of Minterms (SOM)
- Product of Maxterms (POM)
==不化简哦~==
Example:
$F =\overline{X}\overline{Y} +X\overline{Y} = \sum_{m}(0,2)$     SOM
$F =(X+\overline{Y})(\overline{X}+\overline{Y}) = \prod_{m}(1,3)$    POM

![[转换成SOM的方法.png]]![[转换成POM的方法.png]]
==特别重要公式：x+yz = (x+y)(x+z)==

# Standard Forms 标准型
将规范型化至最简便是标准型
- Standard Sum-of-Products (SOP) form:
$ABC+\overline{A}\overline{B}C+B$
- Standard Product-of-Sums (POS) form:
$(A+B)(A+\overline{B}+\overline{C})C$

## 注意：

$(AB+C)(A+C)、AB\overline{C}+AC(A+B)$ 都不是标准型！！

# Cost Criteria
- Literal cost（L）
- Gate input cost（G）
- Gate input cost with NOTs（GN）==数非门的时候同一个字母不用数两次==
![[门消耗数数案例.png]]

# K-map
![[四维卡诺图.png]]

# BUFFER 缓冲器
功能函数： F = X
An electronic amplifier used to improve circuit voltage levels and speed.

当一个输入需要同时接入多个逻辑门的时候使用：
![[buffer.png]]
## 3-state Buffer （三态门）
![[三态门.png]]
当使能信号EN为0时，输出高阻Z。

**Truth Table:**

| EN  | IN  | OUT  |
| --- | --- | ---- |
| 0   | x   | Hi-Z |
| 1   | 0   | 0    |
| 1   | 1   | 1    |

# Derived use

- A - AND
- O - OR
- I - Inverter
eg:
2 - 2 AOI -> $F=\overline{WX+YZ}$
2 - 2 - 1 AO -> $F= WX+YZ+V$
2 - 2 AI -> $F=\overline{(W+X)(Y+Z)}$
