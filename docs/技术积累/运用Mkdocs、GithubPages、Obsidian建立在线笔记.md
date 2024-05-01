占位行
## 认真阅读此文档，相信你能成功

[jobindjohn/obsidian-publish-mkdocs: A Template to Publish Obsidian/Foam Notes on Github Pages (uses MkDocs)](https://github.com/jobindjohn/obsidian-publish-mkdocs)


- 其它官方文档

[MkDocs](https://www.mkdocs.org/)

[Material for MkDocs](https://squidfunk.github.io/mkdocs-material/)

最好还是把这些弄好，这样子可以实现本地调试
## 关于git一些基本操作

```git
切换分支命令:
git checkout main
```

当你修改了仓库后，请执行以下三个命令来提交修改
注意，不用切换到`gh-pages`分支，在main分支push就好了
```git
git add .
git commit -m "The information you note"
git push
```

如果你commit错了，请reset取消commit
```git
git reset HEAD~1
```

如果push的时候报错了可以尝试取消代理
```git
//取消http代理
git config --global --unset http.proxy
//取消https代理 
git config --global --unset https.proxy
```

一般使用了如上操作都会成功，如果还是不行，请打开设置中的网络与Internet，选择代理，手动设置代理，将端口改为7890。
![[改端口.png]]
然后在git bash中输入
```git
git config --global http.proxy http://127.0.0.1:7890
```



## 本地服务器

如果你不希望每次做一些小小的调试就push到仓库中再打开网站看是否成功，你应该先把mkdocs安装好，并在你的电脑上把使用到的插件都安装上，再在`mkdocs.yml`所在目录运行下列命令启动本地服务器。
```
mkdocs serve
```
如果上面这个运行不了，请在前面加上`python -m`
```
python -m mkdocs serve
```

有时候，在创建本地服务器过程中，会遇到`PermissionError: [WinError 10013] 以一种访问权限不允许的方式做了一个访问套接字的尝试。`的提示，这告诉我们端口已经被占用了。
如果你开着酷狗音乐，请将它关闭后重试，因为酷狗默认端口就是8000🤣
如果没有，可以尝试:
```
netstat -ano|findstr 8000
```
来找到占用端口的PID，再taskill它来关闭
```
taskkill /pid THEPIDYOUFIND /F
```

## 一些显著的缺陷

Latex内联公式缺陷，似乎并不能很好地支持换行，并且会将几个相近的公式块连在一起(需要中间加个回车)以及会优先使用markdown语法

章节标题错误，公式如此，请自加占位行（如本文档的最上方）

Mkdocs-material不支持Obsidian的Note格式，不过它自带了其它note方式

不支持嵌套无序列表，或者说第二次需要用 `*` 而不是 `-`

右侧TOC只支持从二级标题开始显示，我前面导入的笔记懒得修改了，所有没有TOC

视频插入格式:
```
![type:video](image/帅头硬.mp4)
```
## TODO

- 有闲暇的话，把其它图片改用webq图片格式，节省空间

## Appendix

!!! note

!!! abstract

!!! info

!!! tip

!!! success

!!! question

!!! warning

!!! failure

!!! danger

!!! bug

!!! example

!!! quote

