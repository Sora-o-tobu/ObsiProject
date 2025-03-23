# index


## Environment with WSL

[【环境配置】Windows 11 的 WSL(Ubuntu2204) 安装OpenCV 4.5.4](https://blog.csdn.net/zhoujinwang/article/details/127741366)


## Eigen

```c++
#include<Eigen/Dense>
#include<iostream>

Eigen::Matrix2d mat1 << 1, 2,
						3, 4; // 定义二维整数矩阵
Eigen::Matrix2d inv = mat1.inverse(); // 矩阵求逆

Eigen::Matrix4f view = Eigen::Matrix4f::Identity(); // 定义四维浮点数单位矩阵
Eigen::Matrix2f subMat = view.block(0,0,2,2); // 从(0,0)开始取 2*2 子矩阵

Eigen::EigenSolver<Eigen::Matrix2f> eigenSolver(submat);
std::cout<< "Eigenvalue:\n" << eigenSolver.eigenvalues() << "\n\n";
// 计算特征值
```

