## VGA 显示图片

- Step 1: 将需要转换的图片用Python程序转换成 `.coe` 格式

```python
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt


img = Image.open("Start.png")
pix = img.load()
width = img.size[0]
height = img.size[1]
f = open("Start.coe", "w")
f.write("memory_initialization_radix = 2;\n")
f.write("memory_initialization_vector = \n")

for y in range(height):
    for x in range(width):
        r, g, b = pix[x, y]
        r_ok = r>>4
        g_ok = g>>4
        b_ok = b>>4
        f.write("%s%s%s,\n"%('{:04b}'.format(r_ok), '{:04b}'.format(g_ok), '{:04b}'.format(b_ok),))
```

> 由于我们所用的VGA模块，RGB的顺序实际上是 `BGR` ，因此上面程序可以将 `r_ok` 和 `b_ok` 交换。但是在贪吃蛇大程序中我并没有这么做，而是手动将其反转，如: `VGA_RGB = {StartPage_RGB[3:0],StartPage_RGB[7:4],StartPage_RGB[11:8]};`

打开得到的 coe 文件，可以看到类似如下内容:

![[coe文件内容.png]]

- Step 2: 创建 ROM 并载入数据
	- 在IP Catalog中搜索ROM，选择 Block Memory Generator
		- ![[ROMIP核.png]]
	- Basic 中选 Single Port Rom
		- ![[Step22.png]]
	- 按照下图填写(其中Depth大于某个数即可)，取消勾选 Primitives Output Register
		- ![[Step23.png]]
	- Load Init File，选择你的coe文件
		- ![[Step24.png]]
	- 调用方法如下： `ROM_StartPage StartPage_unit(.clka(clk),.addra(row*640+col),.douta(StartPage_RGB),.ena(1));`






