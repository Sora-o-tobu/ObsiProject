
对于一个 OpenGL 项目，需要修改的是 VC++->包含目录、 VC++->库目录、链接器->输入->附加依赖项，以及将 `glad.c` 添加进源文件。

```c++
// 处理输入
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 按下 ESC 键
		glfwSetWindowShouldClose(window, true);            // 设置窗口关闭

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		yoffset += 0.001f;
		if (yoffset >= 1.0f)
			yoffset = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		yoffset -= 0.001f;
		if (yoffset <= -1.0f)
			yoffset = -1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		xoffset -= 0.001f;
		if (xoffset <= -1.0f)
			xoffset = -1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		xoffset += 0.001f;
		if (xoffset >= 1.0f)
			xoffset = 1.0f;
	}
}
```