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



## PS/2 键盘

![[PS2键盘对照表.jpg]]

### 模块参考代码

```verilog
module PS2(
	input clk, rst,
	input ps2_clk, ps2_data,
	output reg up, left, right, down ,enter
	);

reg ps2_clk_falg0, ps2_clk_falg1, ps2_clk_falg2;
wire negedge_ps2_clk = !ps2_clk_falg1 & ps2_clk_falg2;
reg negedge_ps2_clk_shift;
reg [9:0] data;
reg data_break, data_done, data_expand;
reg[7:0]temp_data;
reg[3:0]num;

always@(posedge clk or posedge rst)begin
	if(rst)begin
		ps2_clk_falg0<=1'b0;
		ps2_clk_falg1<=1'b0;
		ps2_clk_falg2<=1'b0;
	end
	else begin
		ps2_clk_falg0<=ps2_clk;
		ps2_clk_falg1<=ps2_clk_falg0;
		ps2_clk_falg2<=ps2_clk_falg1;
	end
end

always@(posedge clk or posedge rst)begin
	if(rst)
		num<=4'd0;
	else if (num==4'd11)
		num<=4'd0;
	else if (negedge_ps2_clk)
		num<=num+1'b1;
end

always@(posedge clk)begin
	negedge_ps2_clk_shift<=negedge_ps2_clk;
end


always@(posedge clk or posedge rst)begin
	if(rst)
		temp_data<=8'd0;
	else if (negedge_ps2_clk_shift)begin
		case(num)
			4'd2 : temp_data[0]<=ps2_data;
			4'd3 : temp_data[1]<=ps2_data;
			4'd4 : temp_data[2]<=ps2_data;
			4'd5 : temp_data[3]<=ps2_data;
			4'd6 : temp_data[4]<=ps2_data;
			4'd7 : temp_data[5]<=ps2_data;
			4'd8 : temp_data[6]<=ps2_data;
			4'd9 : temp_data[7]<=ps2_data;
			default: temp_data<=temp_data;
		endcase
	end
	else temp_data<=temp_data;
end

always@(posedge clk or posedge rst)begin
	if(rst)begin
		data_break<=1'b0;
		data<=10'd0;
		data_done<=1'b0;
		data_expand<=1'b0;
	end
	else if(num==4'd11)begin
		if(temp_data==8'hE0)begin
			data_expand<=1'b1;
		end
		else if(temp_data==8'hF0)begin
			data_break<=1'b1;
		end
		else begin
			data<={data_expand,data_break,temp_data};
			data_done<=1'b1;
			data_expand<=1'b0;
			data_break<=1'b0;
		end
	end
	else begin
		data<=data;
		data_done<=1'b0;
		data_expand<=data_expand;
		data_break<=data_break;
	end
end

always @(posedge clk) begin
	case (data)
	    10'h05A:enter <= 1;
        10'h15A:enter <= 0;
        10'h272:down <= 1;
        10'h372:down <= 0;
        10'h275:up <= 1;
        10'h375:up <= 0;
        10'h26B:left <= 1;
        10'h36B:left <= 0;
        10'h274:right <= 1;
        10'h374:right <= 0;
    endcase
end

endmodule
```

调用如下:

```verilog
PS2 ps2_unit(
    .clk(clk), .rst(0),
	.ps2_clk(PS2_clk), .ps2_data(PS2_data),
    .up(up), .left(left), .right(right), 
    .down(down),.enter(NewReset));
```


#### Hold 模块

由于键盘按下去产生的信号是瞬时性的，有时候我们需要对这个信号进行保持，使得其在下一次按键前都保持不变

```verilog
module key_hold(
    input clk,
    input up,
    input left,
    input right,
    input down,
    input reset,
    output reg up_hold,
    output reg left_hold,
    output reg right_hold,
    output reg down_hold
);

reg [3:0] last_key = 4'b0000; // 使用4位寄存器存储上一个按键的状态

always @(posedge clk) begin
    // 检查是否有按键被按下
    if (up == 1) last_key <= 4'b0001;
    else if (left == 1) last_key <= 4'b0010;
    else if (right == 1) last_key <= 4'b0100;
    else if (down == 1) last_key <= 4'b1000;

    if(reset) last_key <= 4'b0000;
    
    // 根据last_key的值设置输出
    case (last_key)
        4'b0001: begin
            up_hold <= 1; left_hold <= 0; right_hold <= 0; down_hold <= 0;
        end
        4'b0010: begin
            up_hold <= 0; left_hold <= 1; right_hold <= 0; down_hold <= 0;
        end
        4'b0100: begin
            up_hold <= 0; left_hold <= 0; right_hold <= 1; down_hold <= 0;
        end
        4'b1000: begin
            up_hold <= 0; left_hold <= 0; right_hold <= 0; down_hold <= 1;
        end
        default: begin
            up_hold <= 0; left_hold <= 0; right_hold <= 0; down_hold <= 0;
        end
    endcase
end

endmodule
```

### 约束文件

```xdc
set_property PACKAGE_PIN N18 [get_ports PS2_clk]
set_property IOSTANDARD LVCMOS33 [get_ports PS2_clk]
set_property PACKAGE_PIN M19 [get_ports PS2_data]
set_property IOSTANDARD LVCMOS33 [get_ports PS2_data]
```
