
# 使用 Reveal-md 写 Slides

!!! quote
	收到短学期课程 CTF 影响，我也对 Reveal-md 写 Sldes 的方式产生了较大兴趣，因此参考[修佬的教程](https://note.isshikih.top/others/reveal-md2Slides/)写了篇自用的笔记

我写的一个简陋测试 Slides： [🔗](https://slides.nimisora.top/test-dir/test.html#/)

## 简介

`reveal-js` 是一个开源的 HTML 表现框架。这是一个工具，使得任何人可以在 Web 创建功能齐全，美丽的 Slides。

而 `reveal-md` 则使得这一效果能充分通过 `Markdown files` 实现

你可以通过 github-pages 等网页托管平台将其部署为静态页面并分享

!!! note "[`reveal-md` 的 github repo](https://github.com/webpro/reveal-md)"

## 使用

使用如下命令安装 `reveal-md` ：

```shell
npm install -g reveal-md
```

!!! tip "安装完成后，可以通过 `reveal-md --help` 来查看帮助"

基础命令：

```shell
## 最基础的命令，根据 Markdown 文件在本地 1948 端口生成 Slides 服务。
> reveal-md your-md-file.md (-w)
## 加上-w参数，可以实现自动刷新页面内容，方便修改

## 为了实现在线部署，需要导出为静态资源。
> reveal-md your-md-file.md --static your-static-dir
```

常用按键：

- `f` 全屏
- `o` 总览
- `s` 进入演讲者模式
- `v` 黑屏

## MD 语法

### 分页

使用 Markdown 的分隔符来分离 Slides：

- `\n---\n` 左右分页
- `\n----\n` 上下分页

```markdown
# Title

- Point 1
- Point 2

---

## Second slide

> Best quote ever.

Note: speaker notes FTW!
```

### 代码段

高亮某些行：

```markdown
```python [1|3-6]
n = 0
while n < 10:
  if n % 2 == 0:
    print(f"{n} is even")
  else:
    print(f"{n} is odd")
  n += 1
\``` # 这里实际上是 ``` 
```

使用 `|` 来分隔高亮块，如上述例子就是先高亮 `n = 0` ，再高亮 `while` 函数内部

## 风格调整

与 Mkdocs 调整类似，需要自己写一份 CSS 文件进行修改美化

!!! tip "通过浏览器的`检查`按钮查看属性"

由于我的定制基本都是从别人那里抄过来的，就不在这里放出了

## 打印为 pdf 格式


```shell
# in pixels when no unit is given
reveal-md slides.md --print slides.pdf --print-size 1024x768

# valid units are: px, in, cm, mm
reveal-md slides.md --print slides.pdf --print-size 210x297mm

# valid formats are: A0-6, Letter, Legal, Tabloid, Ledger
reveal-md slides.md --print slides.pdf --print-size A4
```

## 部署到 Github Pages 上

由于默认生成的 css 文件夹名字是 `_assests` ，但是 `_assests` 会被 Github Pages 忽略，所以我们生成静态页面的时候需要指定 assests 的名字

```shell
reveal-md .\test.md --static static-dir --assets-dir assets
```

