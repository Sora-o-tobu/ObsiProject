占位行
# 认真阅读此文档，相信你能成功

[jobindjohn/obsidian-publish-mkdocs: A Template to Publish Obsidian/Foam Notes on Github Pages (uses MkDocs)](https://github.com/jobindjohn/obsidian-publish-mkdocs)


# 关于git一些基本操作

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


# 自定义域名（无需备案）

## 1. 购买域名
你可以从阿里云、华为云、腾讯云等国内知名公司申请购买一个域名，购买完成后不需要备案。
## 2. 在仓库中添加CNAME文件，并写入你的域名
不要在前面加Http和www!!!
![[CNAME填入域名.png]]
## 3. 在github上选择Setting->Pages->Custom domain，再次填入你的域名
![[域名1.png]]
## 4. 在域名解析记录中，添加你github.io域名的IP

要得到你github.io的IP，可以打开CMD ping一下:
![[得到IP.png]]
然后在你的域名解析中添加两个A记录，分别为www和@，记录值为你刚刚得到的IP
![[IP解析.png]]
我使用的华为云可以快速添加两个记录，其它解析操作类似不做讲解

## 5. 启用HTTPS（可选）
再次在域名解析中添加如下四个IP地址的至少一个

- 185.199.108.153
- 185.199.109.153
- 185.199.110.153
- 185.199.111.153

然后在GitHub仓库Setting->Pages中勾选`Enforce HTTPS`来强制走HTTPS


```markdown
!!! note 

其中对DNS的配置不一定立即生效，可以稍微等待几分钟
```

## 注意
每次push完后要重新填一遍github仓库中的custom domains
# 一些显著的缺陷

Latex内联公式缺陷，似乎并不能很好地支持换行，并且会将几个相近的公式块连在一起(需要中间加个回车)
章节标题错误，公式如此，请自加占位行（如本文档的最上方）

Mkdocs-material不支持Obsidian的Note格式，不过它自带了其它note方式

不支持嵌套无序列表，或者说第二次需要用 `*` 而不是 `-`

