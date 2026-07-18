# Claude Code 通过cc-switch 接入VS code
## 一、前置准备
### 1. 安装`Node.js`
[Node.js下载](https://nodejs.org/zh-cn/download)  
作用：
1. Claude Code 基于 JS/TS 开发，**必须 Node.js 环境才能运行**；
2. 安装工具 npm 是 Node.js 自带配套程序，用来拉取安装 claude-code；
3. 运行 `claude` 命令时，由 Node 负责读本地配置、发起 DeepSeek API 网络请求、读写项目文件。

### 2.安装`cc-switch`
[官网链接](https://www.ccswitch.io/zh/)  
Ubuntu系统选择`Linux-x86_64.deb`后缀的安装包

### 3.安装`Claude Code`
[下载链接](https://code.claude.com/docs/zh-CN/overview)  
**注意别下载最上方的`Lucky`**

### 二、在cc-switch中配置API
以 deepseek v4 pro 为例：
