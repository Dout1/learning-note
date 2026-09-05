# 任务四 · 十字路口处理 (task4)

智能车视觉模块：识别十字路口并在十字区域内做轨迹修复（贝塞尔补线）。

## 目录结构

```
task4/
├── CMakeLists.txt          # 构建脚本
├── main.cpp                # 主程序：trackRecognition → process → draw + dump
├── include/
│   ├── common.h            # POINT / Scene / 图像尺寸常量 / LOGLN / sigma
│   ├── Tracking.h          # 赛道提取（连通色块、边界点、特征、车库/斑马线）
│   ├── Crossroad.h         # 十字状态机（判入/判出）+ 贝塞尔补线
│   └── Utils.h             # N 点 Bezier 声明
├── src/
│   └── Utils.cpp           # N 点 Bezier 实现
├── cpp参考代码/
│   ├── tracking.cpp        # 官方权威例程
│   └── crossroad.cpp       # 官方权威例程
├── res/
│   ├── cross1.mp4
│   ├── cross2.mp4
│   ├── cross3.mp4
│   └── demo.avi
└── dump/                   # dump 模式输出的标注 PNG（运行后生成）
```

## 构建

需要本机已安装 OpenCV（CMake 自动探测）。

```bash
cd task4
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 运行

可执行文件为 `build/cross_app`，**从 `build/` 目录运行**以保证 `../res/` 相对路径有效：

```bash
cd build
./cross_app 1          # cross1.mp4
./cross_app 2          # cross2.mp4
./cross_app 3          # cross3.mp4
./cross_app 4          # demo.avi
```

窗口打开后按 `q` 或 `ESC` 退出。

**画面说明**：绿点=左边界，黄点=右边界，红点=岔路（inlines），左上角=step/type/flag/width 状态文字。

### dump 调试模式

追加 `--dump` 参数即可：逐帧打印特征值，判入帧 + 每 20 帧保存一张标注 PNG 到 `../dump/`（`nv{n}_f{帧号}.png`）。

```bash
./cross_app 1 --dump
```

输出字段：`f`（帧号）、`ptsL/ptsR`（边界点数）、`inl`（岔路数）、`inlX/inlY`、`endL/endR`、`midL/midR`、`stdevL/R`、`step/type`、`lb/rb/lt/rt`、`wbLast`。
