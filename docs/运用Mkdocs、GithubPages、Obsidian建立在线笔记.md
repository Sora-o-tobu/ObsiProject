占位行
# 认真阅读此文档，相信你能成功

[jobindjohn/obsidian-publish-mkdocs: A Template to Publish Obsidian/Foam Notes on Github Pages (uses MkDocs)](https://github.com/jobindjohn/obsidian-publish-mkdocs)


# 关于git一些基本操作

```
切换分支命令:
git checkout main
```

当你修改了仓库后，请执行以下三个命令来提交修改
注意，不用切换到`gh-pages`分支，在main分支push就好了
```
git add .
git commit -m "The information you note"
git push
```

如果push的时候报错了可以尝试取消代理
```
//取消http代理
git config --global --unset http.proxy
//取消https代理 
git config --global --unset https.proxy
```

# 一些显著的缺陷

Latex内联公式缺陷，似乎并不能很好地支持换行，并且会将几个相近的公式块连在一起(需要中间加个回车)
章节标题错误，公式如此，请自加占位行（如本文档的最上方）
