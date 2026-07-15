# learning-note 完整仓库目录规划（适配你的全部学习内容）
## 总目录结构（直接照这个创建，逻辑清晰、分层分明）
```
learning-note/
├── README.md                # 仓库总索引，快速跳转所有分类
├── .gitignore               # 屏蔽编译缓存、系统垃圾文件
├── push.sh                  # Ubuntu一键上传脚本
├── assets/                  # 全局截图、图片、报错截图（所有笔记共用）
├── daily-log/               # 每日学习流水日志（按日期命名 2026-07-15.md）
│   └── template.md          # 日志空白模板，每天复制使用
├── Linux-Base/              # Ubuntu系统、Git/GitHub基础专区（你要的操作系统知识）
│   ├── terminal-cmd.md      # Ubuntu终端常用命令合集（cd/mkdir/git等）
│   ├── fcitx5-input.md      # 输入法配置踩坑记录
│   ├── vscode-setup.md      # VS Code安装、代理、权限、插件配置笔记
│   ├── git-github-basic.md  # Git命令、clone/push/pull、代理、登录全套教程
│   └── dual-boot-time.md    # 双系统时间同步、双系统云端同步方案
├── SLAM14/                  # 视觉SLAM十四讲专项
│   ├── theory-notes/        # 理论推导（李群、SE3、位姿变换LaTeX公式）
│   ├── code-analysis/       # 配套代码逐行注释、CMake编译流程
│   └── error-log/           # 编译报错、依赖缺失、OpenCV/Eigen踩坑记录
├── MCU-ESP32/               # ESP32单片机嵌入式专区
│   ├── platformio-setup.md  # Windows VSCode ESP32插件环境配置
│   ├── uart-serial.md       # 串口调试、烧录、驱动权限问题
│   └── demo-code/           # 例程笔记、硬件实验记录
├── C-Cpp/                   # C++基础编程（SLAM前置语法）
├── Machine-Learning/        # 机器学习、深度学习、CUDA加速笔记
└── Tools-Skill/             # 通用工具杂项（Markdown语法、代理配置、软件技巧）
```

## 各文件夹详细用途说明
### 1. assets（全局图片文件夹）
所有分类笔记的截图统一放在这里，笔记使用**相对路径**引用，Windows、Ubuntu、GitHub网页图片全部正常加载
示例引用写法：
```markdown
![Git连接失败截图](../assets/git-clone-fail.png)
```

### 2. daily-log 每日日志
每天新建一份日期命名的md文件，记录当天踩坑、零散知识点，流水记录；专项文件夹存放系统化整理后的干货，分开不混乱。
例：`daily-log/2026-07-15.md`

### 3. Linux-Base（你要求的系统/Git专区，重点）
专门存放底层系统知识，所有Ubuntu、Git、双系统、VS Code配置问题全部归类在这里：
- Ubuntu终端基础命令、文件夹操作、apt/snap/deb包区别
- Git完整教程：clone、add/commit/push/pull、代理配置、双系统同步
- VS Code安装避坑、权限只读修复、GitHub登录代理设置
- Fcitx5输入法、Clash代理、双系统时间同步等系统踩坑合集

### 4. SLAM14 视觉SLAM专项
独立分区存放十四讲系统学习内容，理论、代码、报错完全分开，后期复习查找方便，LaTeX矩阵公式全部写在此目录md内。

### 5. MCU-ESP32 单片机专区
专门放Windows下ESP32开发相关笔记，环境配置、串口、硬件实验、例程记录，和Linux系统知识完全隔离。

### 6. C-Cpp
SLAM前置C++语法、面向对象、指针、智能指针、Eigen矩阵语法笔记。

### 7. Machine-Learning
机器学习、深度学习、PyTorch/CUDA环境搭建、数据集训练记录。

### 8. Tools-Skill
通用工具小技巧：Markdown LaTeX公式语法、PicGo图床、Clash Verge代理配置等通用工具内容。

## 配套仓库根目录README.md 索引模板
```markdown
# 个人全栈学习笔记仓库
## 目录导航
1. 📅 [每日学习流水日志](./daily-log)
2. 🐧 [Ubuntu系统 & Git/GitHub 基础](./Linux-Base)
3. 👁️ [视觉SLAM十四讲学习](./SLAM14)
4. 🛠️ [ESP32单片机嵌入式开发](./MCU-ESP32)
5. C/C++ 编程基础 [C-Cpp](./C-Cpp)
6. 🧠 [机器学习/深度学习](./Machine-Learning)
7. 🔧 [通用工具技巧](./Tools-Skill)

## 同步规则
1. Ubuntu/Windows切换前执行 `./push.sh` 上传全部修改
2. 开机先执行 `git pull` 拉取云端最新笔记
3. 所有截图统一存放 `assets` 文件夹，使用相对路径引用
```

## 实操创建命令（Ubuntu终端一键生成全部文件夹）
进入你的仓库根目录 `~/learning-note`，复制执行：
```bash
# 创建一级文件夹
mkdir -p assets daily-log Linux-Base SLAM14 MCU-ESP32 C-Cpp Machine-Learning Tools-Skill
# 创建二级子目录
mkdir -p SLAM14/theory-notes SLAM14/code-analysis SLAM14/error-log
mkdir -p MCU-ESP32/demo-code
# 新建日志模板
touch daily-log/template.md
```

## 核心优势适配你的学习场景
1. **隔离清晰**：系统基础、SLAM、单片机三大核心学习内容完全分文件夹，不会混杂；
2. 分层管理：零散流水日志 `daily-log` 和系统化整理笔记分开，复习不用翻大量日期文件；
3. 跨系统兼容：所有路径使用相对地址，Windows/Ubuntu打开图片、跳转目录无异常；
4. 便于GitHub归档，后期求职可单独导出SLAM/单片机笔记作为作品集。