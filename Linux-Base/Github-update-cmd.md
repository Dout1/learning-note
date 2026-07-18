# GitHub 本地仓库全流程操作手册
Ubuntu环境，从初始化→账号绑定→拉取更新→上传推送
 
## 一、前期仓库初始化&远程绑定（首次使用仓库只做一次）
 
1. 本地新建/进入项目文件夹
 
```
  # 进入已有项目目录
cd ~/learning-note
# 如果是全新项目，先创建文件夹再进入
mkdir ~/learning-note && cd ~/learning-note  
``` 
 
2. 初始化为Git本地仓库
 
  
git init
```
 
3. 绑定GitHub远程仓库（origin是远程仓库默认别名）
 
``` 
# 格式：git remote add origin 你的仓库HTTPS/SSH地址
git remote add origin https://github.com/Dout1/learning-note.git
``` 
若之前配置错误，先删除旧远程再重新绑定：
 
```  
git remote remove origin
git remote add origin https://github.com/Dout1/learning-note.git
```  

4. 校验远程绑定是否成功
 
``` 
git remote -v
 ```
 
能看到fetch、push两条仓库地址，代表远程配置完成。
 
5. 账号身份基础配置（整台设备仅需配置一次）
 
配置提交时显示的用户名、邮箱，和GitHub账号信息对应：
 
```bash
  
git config --global user.name "Dout1" # 你的GitHub用户名
git config --global user.email "你的GitHub绑定邮箱"
``` 
 
 --global 代表全局生效，后续所有仓库都沿用这套身份；去掉 --global 仅对当前仓库生效。
 
6. HTTPS方式账号认证（Ubuntu最常用，重点）
 
GitHub不再支持账号密码登录推送，密码位置填个人访问令牌PAT：
 
1. GitHub网页端： Settings→Developer settings→Personal access tokens→Tokens(classic) ，新建令牌，勾选 repo 仓库权限，生成并复制令牌；
2. 终端执行推送/拉取时，用户名输入GitHub用户名，密码粘贴刚才复制的令牌；
3. 可选永久保存凭据，避免每次输入：
 
```bash
  
git config --global credential.helper store
``` 
 
首次输入账号令牌后，本地会缓存凭据，后续无需重复输入。
 
如果使用SSH方式：配置SSH密钥并把公钥上传GitHub，远程地址改用 git@github.com:Dout1/learning-note.git ，无需输入账号密码。
 
## 二、从GitHub拉取更新（远程→本地，日常同步远端代码）
 
场景1：本地无未保存修改，最简更新
 
```bash
  
# 完整拉取+合并
git pull origin main
# 简写（当前分支已追踪远程main）
git pull
``` 
 
场景2：稳妥分步更新（推荐）
 
```bash
  
git fetch origin # 仅下载远端更新，不修改本地文件，可先查看变更
git merge origin/main # 将远端main分支合并到本地
 ```
 
场景3：本地有未提交修改，想保留改动再更新
 
```bash
  
git stash # 临时储存本地改动，工作区清空
git pull origin main # 拉取远端最新内容
git stash pop # 恢复之前储存的本地修改，若冲突手动解决
``` 
 
场景4：强制本地同步远端，丢弃本地所有改动（谨慎使用）
 
```bash
  
git fetch origin
git reset --hard origin/main
``` 
 
冲突处理：拉取出现CONFLICT
 
1. 打开冲突文件，删除 <<<<<< HEAD 、 ====== 、 >>>>>> 远端提交号 标记，保留需要的代码；
2. 执行收尾：
 
```bash
  
git add .
git commit -m "解决拉取合并冲突"
 ```
 
## 三、本地上传推送（本地→GitHub，把改动传到远程仓库）
 
标准三步流程
 
```bash
  
# 1. 暂存所有改动文件
git add .
# 2. 提交到本地仓库，备注修改内容
git commit -m "更新：补充xxx笔记"
# 3. 推送到远程main分支
git push origin main
 ```
 
首次推送专属操作：本地仓库刚建立、远程暂无对应分支
 
```bash
  
git push -u origin main
``` 
 
 -u 作用：绑定本地main和远程origin/main分支，后续直接输入 git push 即可推送，无需写分支名。
 
## 四、高频配套实用命令
 
1. 查看当前文件改动状态： git status 
2. 查看提交历史： git log 
3. 查看全局账号配置： git config --global --list 
4. 撤销本地暂存、不删除文件： git reset HEAD . 
 
## 五、日常固定工作流（直接套用）
 
1. 开始写代码前： git pull 同步远端最新内容，避免后续冲突；
2. 修改完文件后： git add . → git commit -m "修改说明" → git push 上传远端；
3. 多人协作时优先用 git fetch 检查远端更新，再合并，降低代码出错概率。