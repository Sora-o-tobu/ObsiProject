
## WebVPN

任何校内网站的访问都可以通过 **WebVPN** 实现： https://webvpn.zju.edu.cn/

其链接规则如下：

- 只有 HTTP 协议，不支持 HTTPS
- 都通过 `.webvpn.zju.edu.cn:8001` 访问
- 四级域名的规则为：
    - 原域名的 `.` 替换为 `-`，`-` 替换为 `--`
    - 如果要通过 HTTPS 访问，则在末尾加 `-s`
- URL 资源路径直接接在链接后面
- 可见 ip 的 `.` 以及 ip 和端口间的 `:` 都替换为了 `-` 并在结尾加了 `-p`

例如：

- 通过链接 http://www-cc98-org-s.webvpn.zju.edu.cn:8001/ 校外访问论坛 *cc98* 
- 一个校内 CTF 题目部署在 http://10.214.160.13:10002/ ，校外可以通过 http://10-214-160-13-10002-p.webvpn.zju.edu.cn:8001/ 进行访问

## 代理

### EasyConnect

即 RVPN ，网址为 https://rvpn.zju.edu.cn/ 

### ZJU-Connect

应该是一个学长写的软件

## 科学上网的同时进内网

在 clash 的 bypass domain 里写入：

```
bypass:
  - localhost
  - 127.*
  - 10.*
  - 172.16.*
  - 172.17.*
  - 172.18.*
  - 172.19.*
  - 172.20.*
  - 172.21.*
  - 172.22.*
  - 172.23.*
  - 172.24.*
  - 172.25.*
  - 172.26.*
  - 172.27.*
  - 172.28.*
  - 172.29.*
  - 172.30.*
  - 172.31.*
  - 192.168.*
  - "*cc98.org*"
  - <local>
  - "*westlake.edu.cn*"
  - "*zju.edu.cn*"
  - "*sciencedirect.com*"
  - "*springer.com*"
  - "*wiley.com*"
  - "*elsevier.com*"
  - "*pubmed.gov*"
  - "*jstor.org*"
  - "*plos.org*"
  - "*sagepub.com*"
  - "*sciencemag.org*"
  - "*science.org*"
  - "*nature.com*"
  - "*aps.org*"
  - "*acm.org*"
  - "*cell.com*"
  - "*ieee.org*"
  - "*academic.oup.com*"
  - "*cambridge.org*"
  - "*rsc.org*"
  - "*annualreviews.org*"
  - "*acs.org*"
  - "*pnas.org*"
  - "*sciencedirectassets.com*"
  - "*doi.org*"
  - "*aip.org*"
  - "*141.66.193.6*"
  - "*webofscience*"
```