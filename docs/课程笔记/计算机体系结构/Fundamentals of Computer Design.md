
# 1. Fundamentals of Computer Design

Classes of Parallel Architectures:

**S**ingle/**M**ultiple **I**nstruction/**D**ata stream

- SISD
	- Chapter 3
- SIMD
	- Chapter 4
- MISD
	- 不存在
- MIMD
	- Chapter 5, 6

Computer Architecture 定义：Instruction Design & Implementation 指令集及其实现

CA must design the organization and hardware to meet functional requirements as well as price, power, performance, and availability goals


Class of ISA:

- Register-Memory ISA: 80x86, 许多指令可以直接访问内存
- Load-Store ISA: RISC-V, ARMv8, 只有 load, store 能够访问内存



## Trends

Performance-Driven: Quantatives

**Technology Trends:**

- 芯片工艺 Integrated Circuit Logic Technology
	- 摩尔定律逐渐失效
- Memory 工艺 Semiconductor DRAM
	- 容量、延迟、带宽
- SSD 工艺 Semiconductor Flash
- 磁盘工艺 Magnetic Disk Technology
- 网络工艺 Network Technology
	- 延迟、带宽

传输线上消耗的功耗比 IC 还多

**Power and Energy in IC Trends:**

