
# Introduction

本节课涉及**水印**（**Watermark**）和**隐写**（**Steganology**）两种信息与载体之间的关系，我们可以将其细分为以下四类：


|                             | Cover & Work<br>Dependent      | Cover & Work<br>Independent                     |
| --------------------------- | ------------------------------ | ----------------------------------------------- |
| **Existence**<br>**Hidden** | 隐藏(Covert)水印<br>例如机密文件中不同长度的空格 | 隐写                                              |
| **Existence**<br>**Known**  | 公开(Overt)水印<br>例如钞票            | 公开嵌入交流(Overt Embedded Communications)<br>例如广播电台 |

<font style="font-weight: 1000;font-size: 20px" color="orange">Watermark</font>

我们认为，Watermark 的 *Features* 有：

- **Imperceptible** 不可察觉性
- **Inseparable** 不可分离性
- **Transform along with work** 随载体变化

在此基础上，根据其应用场景不同，其具有如下 *Performance*：

- **Robustness** 鲁棒性
	- 传播过程中 work 被压缩、裁剪等处理，水印能否良好地保持？
- **Fidelity** 保真度
	- 水印的不可察觉性程度有多大？

!!! note "各个 Performance 的重要性随应用场景而变化，例如在脆弱/公开水印中，反而要求鲁棒性很低"

<font style="font-weight: 1000;font-size: 20px" color="orange">Steganology</font>

与加密相比，Steganoloy 的 *Feature* 为：

- **Hiding the presence/communicating**

在此基础上，根据其应用场景不同，其具有如下 *Performance*：

- **Statistical Undetectability** 统计上的不可检测性
	- 检测到其存在的难易程度？
- **Steganographic Capacity** 隐写容量
	- 在不失真的情况下，能够放入的最大 Payload？

