# 智能车视觉 · 十字路口识别与轨迹修复 — 项目交接日志

> **文档性质**：离职交接文档。目标读者是接手本项目的下一位开发者（Claude Code 或人力）。
> **阅读目标**：读完本文即可完整了解项目背景、当前进度、已知问题，并知道**下一步该做什么**。
> **最后更新**：2026-09-05（忠实移植完成 + 三视频验证快照）

---

## 一、项目背景与目标

### 1.1 业务目标

为**智能车（巡线小车）**实现视觉模块：摄像头采集赛道画面，识别**十字路口（crossroad）**，并在十字区域内（赛道边界缺失/混乱时）进行**轨迹修复（补线）**，引导车辆安全通过。

赛道特征：
- 白色赛道 + 蓝色（或深色）背景，二值化后赛道为白色
- 赛道含**直道、弯道、十字路口（P 型结构）**
- 十字处赛道突然变宽，且存在**岔路（多连通色块）**

### 1.2 核心任务拆解

| 模块 | 职责 | 状态 |
|------|------|------|
| 图像预处理 | 灰度 + 二值化 | ✅ 完成 |
| 赛道提取 | 逐行连通色块 → 左右边界点集、岔路点 | ✅ 忠实移植例程 |
| 特征计算 | GetEndIndex / GetMiddleIndex / stdev / widthBlock | ✅ 忠实移植例程 |
| 状态机 | 判入/判出十字路口（None ↔ Fix） | ✅ 事件完整配对 |
| 轨迹修复 | 十字内 N 点贝塞尔补线 | ✅ 忠实移植例程 |
| 可视化 | 边界点、岔路、状态文字 | ✅ 完成 |

### 1.3 整体算法流程

```
视频帧 → 灰度 → 二值化 → [赛道提取 trackRecognition] → 左右边界点集 + 岔路点(inlines)
                            ↓
                       [特征计算] → GetEndIndex/GetMiddleIndex/stdev/widthBlock
                            ↓
                       [状态机 Crossroad::process] → None ↔ Fix（判入/判出）
                            ↓
                       [轨迹修复] → N 点贝塞尔引导线
                            ↓
                       可视化输出
```

---

## 二、项目结构与代码清单

> ⚠️ **本工程已按“忠实重写官方例程”重构**。早期版本中的 `Track.h` / `CrossProcessor.cpp` / `fillTrackFromExample()` 等**自研文件已全部删除**，请勿再按旧文档（历史提交）寻找这些文件。

### 2.1 目录结构

```
SmartCar_Vision/task4/
├── CMakeLists.txt              # 构建脚本
├── main.cpp                    # 主程序：视频读取 + trackRecognition→process→draw + dump
├── include/
│   ├── common.h                # POINT/Scene/ROWSIMAGE/COLSIMAGE/LOGLN/sigma（重建的 common.hpp）
│   ├── Tracking.h              # 忠实移植 tracking.cpp（header-only）
│   ├── Crossroad.h             # 忠实移植 crossroad.cpp（header-only）
│   └── Utils.h                 # N 点 Bezier(dt, vector<POINT>)
├── src/
│   └── Utils.cpp               # Bezier 实现
├── cpp参考代码/
│   ├── tracking.cpp            # 官方权威例程（赛曙）
│   └── crossroad.cpp           # 官方权威例程（赛曙）
├── res/
│   ├── cross1.mp4              # 测试视频 1
│   ├── cross2.mp4              # 测试视频 2
│   ├── cross3.mp4              # 测试视频 3
│   └── demo.avi                # 备选视频
├── dump/                       # dump 模式输出的标注 PNG
└── build/                      # 编译产物
```

### 2.2 文件职责一览

| 文件 | 核心内容 | 交接备注 |
|------|---------|---------|
| `include/common.h` | `POINT{x,y,slope}`（x=行,y=列）、`Scene`、`ROWSIMAGE=240`/`COLSIMAGE=320`、`LOGLN`、`sigma` | 对应例程 `common.hpp` |
| `include/Tracking.h` | `trackRecognition()`、`FindBlockinRow`、`FilterbyDE`、`GetEndIndex`、`GetMiddleIndex`、`stdevEdgeCal`、车库/斑马线检测 | **header-only**，忠实移植 |
| `include/Crossroad.h` | `process()` 状态机、`FindBottomPoint/2`、`WholeBlock`、贝塞尔补线 | **header-only**，忠实移植 |
| `include/Utils.h` + `src/Utils.cpp` | `Bezier(dt, vector<POINT>)` N 点贝塞尔 | 替代旧 bezierCubic |
| `main.cpp` | 主循环：`trackRecognition → cross.process → drawImage/draw_img`，含 `--dump` 模式 | — |

### 2.3 参考例程（外部权威）

`cpp参考代码/tracking.cpp` + `crossroad.cpp` 是**权威参考**，本工程即它们的忠实移植。关键约定：
- 坐标 `POINT(row, col)`，`.x=行`（垂直），`.y=列`（水平）；**绘图时用 `cv::Point(y, x)`**（行列对调）
- 两态状态机 `CrossStep {None, Fix}`，`CrossType {Left=0, Right, Mid}`
- 逐行连通色块提取，形态学 `FilterbyDE`（先膨胀后腐蚀）
- 特征判据基于**边界点集形态**（`GetEndIndex`/`GetMiddleIndex` 检测贴边），**非宽度/面积突变**
- `widthCounter` 统计连续 `width==319`（整行全白）的行数

---

## 三、当前进度

### 3.1 已完成 ✅（忠实移植）

1. **重建 `common.h`**：补齐例程缺失的 `common.hpp`
2. **忠实移植 `tracking.cpp` → `Tracking.h`**（含车库/斑马线逻辑，完整保留）
3. **忠实移植 `crossroad.cpp` → `Crossroad.h`**（含 N 点贝塞尔补线）
4. **`Utils` 增加 `Bezier(dt, vector<POINT>)`** 供补线使用
5. **重写 `main.cpp`**：`trackRecognition → cross.process → draw`，保留 `--dump` 调试模式
6. **清理**：删除自研的 `Track.h`/`CrossProcessor.h`/`src/Track.cpp`/`src/CrossProcessor.cpp`
7. **三视频验证**：编译通过、状态机事件完整配对（见第四节）

### 3.2 相对官方例程的有意改动（偏离逐字移植，已在源码注释标注）

> 全部是为“让代码能正确编译/语义正确”而做的修正，逻辑上仍等价于官方意图。

| # | 位置 | 官方原样 | 改为 | 说明 |
|---|------|---------|------|------|
| 1 | [Tracking.h:503](include/Tracking.h#L503) | `counter == 0;`（比较表达式，无副作用） | `counter = 0;` | 官方笔误；修正后左边缘连续计数才正确 |
| 2 | [Tracking.h:507](include/Tracking.h#L507) | `if(threshold>10) return index; else return -1;` | `return index;` | `threshold` 恒为 30，`threshold>10` 恒真，`else` 为死代码 |
| 3 | `Crossroad.h` | `bool process()` 无任何 `return`（未定义行为） | `void process()` | 返回值从未被使用 |
| 4 | `Crossroad.h` Left 分支 | 游离的 `LOGLN(bottom_left_index);` | 删除 | 调试残留，避免刷屏 |

---

## 四、三视频判入/判出验证结果（2026-09-05）

### 4.1 Fix 片段总表

| 视频 | Fix 片段（类型 / 帧区间 / 时长） |
|---|---|
| cross1 | 前 27 次 1 帧抖动(12–64) → RIGHT(66–217) → RIGHT(228 孤立) → MID(244–357) → LEFT(808–881) |
| cross2 | MID(224–473) → RIGHT(1054–1159) → MID(1162–1307) |
| cross3 | RIGHT(214–537) → LEFT(898–1014) → MID(1020–1391) → **LEFT(1594–2279，686 帧)** → LEFT(2282–2381) → LEFT(2600–2635) |

> 验证方式：dump 逐帧打印特征值 + 二值图 ASCII 逐帧比对。**稳定片段（MID/RIGHT/LEFT）均落在真实十字路口上**（十字处可见横向整块白条带，见 `widthBlock` 整行 319 的宽行）。判入是准的。

### 4.2 两处“继承自官方例程”的行为（非移植错误）

#### 行为一：cross1 开头 27 次 1 帧抖动（帧 12–64）

- **现象**：起始段弯道反复 1 帧一进的 RIGHT 片段。
- **根因**：`right_flag`/`left_flag` 是类成员变量，被 `FindBottomPoint` 置 true 后**粘住不重置**；起始弯道误触发 `RIGHT CASE 1`（[Crossroad.h:64](include/Crossroad.h#L64)），但下一帧 `inlines[0].y > 220` 立刻判出（[Crossroad.h:109](include/Crossroad.h#L109)），于是 1 帧一抖。判入阈值 `inlines[0].y < 100`、判出 `> 220` 是官方原值。
- **性质**：官方例程固有特性，非移植 bug。

#### 行为二：cross3 LEFT 片段 686 帧超长（约 23 秒）

- **现象**：LEFT(1594–2279) 卡死近 23 秒才判出。
- **根因**：判出检查 `inlines[0].y < 100 || width_counter > 40` 被包在 `if (!track.inlines.empty())` 内（[Crossroad.h:116-141](include/Crossroad.h#L116-L141)）。实测该片段 f=1600–2270 共 670 帧 `inl=0`（未检测到内联线），判出分支整段被跳过，状态卡死在 `Fix/Left`，直到 f=2279 `inlY=101` 才在 f=2280 判出。
- **性质**：官方原代码结构即如此，属于“**内联线消失时状态机不退出**”的固有缺陷。

---

## 五、历史问题（已解决 / 已废弃）

> 以下为早期“自研版本”踩过的坑，**教训仍然有效**，但相关代码已删除。保留供参考，避免重蹈。

| # | 问题 | 结论 |
|---|------|------|
| 1 | 种子生长版"什么也识别不出来" | 弃用，改连通色块 |
| 2 | 轮廓法宽度判据始终不触发（直道已宽 277，十字变化不足） | **弃用轮廓法**，回归例程边界点集法 |
| 3 | 自研版特征退化为 `GetEndIndex=size-1`、`GetMiddleIndex=n/2`、阈值 25/20/15 | 全错；**应以例程原文为准**（100/60/140） |
| 4 | `inlines` 语义搞反（当中心点而非左边界） | 例程存的是 `mblock[i].x`（左边界） |
| 5 | 视频闪退/段错误（空 vector 访问、越界） | 加空保护与边界 clamp |

**核心教训：当不确定语义时，回去读 `cpp参考代码/` 里的例程原文，而不是自己重构。**

---

## 六、关键代码路径速查

| 要改什么 | 去哪里改 |
|---------|---------|
| **赛道提取**（连通色块、rowCutUp=10、噪点过滤） | `include/Tracking.h` → `trackRecognition()` / `FindBlockinRow()` |
| **形态学**（FilterbyDE 先膨胀后腐蚀） | `include/Tracking.h` → `FilterbyDE()` |
| **特征计算**（GetEndIndex/GetMiddleIndex/stdev） | `include/Tracking.h` |
| **判入条件阈值**（100/60/140） | `include/Crossroad.h` → `process()` 的 `None` 分支 |
| **判出条件阈值**（inlines y / width_counter） | `include/Crossroad.h` → `process()` 的 `Fix` 分支 |
| **底部点检测 / flag 设置** | `include/Crossroad.h` → `FindBottomPoint()` |
| **宽行计数** | `include/Crossroad.h` → `WholeBlock()` |
| **补线贝塞尔** | `include/Crossroad.h` → `Fix` 分支 + `include/Utils.h` → `Bezier()` |
| **可视化 / dump** | `main.cpp` |

---

## 七、🔴 下一步行动清单（按优先级）

> 忠实移植已完成并验证。以下为**可选增强**，非阻塞。

### 第一步（建议，修复“内联线消失卡死”）：加判出看门狗

针对 `4.2 行为二`：在 `Crossroad::process` 的 `Fix` 分支外层，增加“**连续 N 帧无 inlines 则强制 step=None**”的看门狗。这是对官方逻辑的增强，不属移植范围，需在 `Crossroad` 类内加一个计数器成员。

### 第二步（可选）：消除开头抖动

针对 `4.2 行为一`：可在判入后加“连续 M 帧满足判入条件才真正进入 Fix”的去抖，或调整 `inlines[0].y < 100 / > 220` 阈值。**注意：改阈值前先 dump 看真实数据**。

### 第三步（可选）：阈值标定

如需在自建赛道上泛化，参考例程判入阈值 100/60/140、判出 `width_counter>40` 都是针对赛曙赛道标定的；换赛道需用 `--dump` 重新采集直道帧/十字帧数据再调。

### 第四步（可选）：车库/斑马线联调

`Tracking.h` 内车库/斑马线检测已完整保留但未在三个 cross 视频中触发验证；如需使用，另行构造对应场景视频测试。

---

## 八、构建与运行

```bash
cd ~/learning-note/SmartCar_Vision/task4/build
rm -rf *
cmake ..
make -j$(nproc)
./cross_app 1          # 1=cross1, 2=cross2, 3=cross3；实时窗口，q/ESC 退出
./cross_app 1 --dump   # dump 模式：逐帧打印特征值，判入帧 + 每 20 帧存标注 PNG 到 ../dump/
```

**画面说明**：绿点=左边界，黄点=右边界，红点=岔路（inlines），左上角=step/type/flag/width 状态文字。

**dump 模式输出字段**：`f`（帧号）、`ptsL/ptsR`（边界点数）、`inl`（岔路数）、`inlX/inlY`、`endL/endR`、`midL/midR`、`stdevL/R`、`step/type`、`lb/rb/lt/rt`、`wbLast`。

---

## 九、交接状态

- [x] 代码已上传至仓库（当前分支即交接快照）
- [x] 本日志已放置于项目根目录 `HANDOFF.md`
- [x] 忠实移植 + 三视频验证通过
- [x] 判入/判出事件完整配对（无悬挂 Fix 态）
- [ ] 判出看门狗（可选，未实施）
- [ ] 车库/斑马线场景验证（可选，未实施）

---

**祝顺利接手。核心一句话：代码已是官方例程的忠实移植，改逻辑前先对照 `cpp参考代码/` 原文，改阈值前先 `--dump` 看数据。**

— 前任开发者
