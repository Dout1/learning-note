#include "tracking.h"
#include <algorithm>

Tracking::Tracking()
    : start_flag(true),
      _top_cut(30),
      _bottom_cut(20) {
    // 构造函数：初始化参数
}

void Tracking::process(const cv::Mat& img_binary) {
    // 重置状态
    start_flag = true;
    LeftPoints.clear();
    RightPoints.clear();
    _white_block.clear();

    int height = img_binary.rows;
    int width  = img_binary.cols;

    // 从下往上遍历有效行（避开底部车身和顶部噪声）
    for (int row = height - _bottom_cut - 1; row >= _top_cut; --row) {
        _white_block.clear();

        // 1. 搜索当前行的所有白色色块
        search_white_block(img_binary, row);

        if (_white_block.empty()) {
            continue;  // 当前行无白色区域，跳过
        }

        if (start_flag) {
            // 2. 起始行搜索
            search_start_line(row);
        } else {
            // 3. 非起始行搜索（利用连通性）
            search_normal_line(row);
        }
    }
}

void Tracking::search_white_block(const cv::Mat& img_binary, int row) {
    const uchar* ptr = img_binary.ptr<uchar>(row);
    int width = img_binary.cols;

    bool in_block = false;
    int start = 0;

    for (int col = 0; col < width; ++col) {
        if (ptr[col] == 255 && !in_block) {
            // 遇到白色像素，开始新色块
            in_block = true;
            start = col;
        } else if (ptr[col] != 255 && in_block) {
            // 白色色块结束
            in_block = false;
            _white_block.emplace_back(start, col - 1);
        }
    }

    // 处理行末仍在白色块中的情况
    if (in_block) {
        _white_block.emplace_back(start, width - 1);
    }
}

void Tracking::search_start_line(int row) {
    if (_white_block.empty()) return;

    // 找到宽度最大的白色色块
    auto widest = std::max_element(
        _white_block.begin(), _white_block.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return (a.second - a.first) < (b.second - b.first);
        }
    );

    int start = widest->first;
    int end   = widest->second;

    // 记录左、右点
    LeftPoints.emplace_back(row, start);
    RightPoints.emplace_back(row, end);

    // 起始行完成，后续进入正常搜线
    start_flag = false;
}

void Tracking::search_normal_line(int row) {
    if (_white_block.empty()) return;

    // 上一行的左、右点列坐标
    int last_left  = LeftPoints.back().y;
    int last_right = RightPoints.back().y;

    // 寻找与上一行左点连通的色块（最左匹配）
    std::pair<int, int> left_block = _white_block.front();
    for (const auto& block : _white_block) {
        if (last_left >= block.first && last_left <= block.second) {
            left_block = block;
            break;
        }
    }

    // 寻找与上一行右点连通的色块（最右匹配）
    std::pair<int, int> right_block = _white_block.back();
    for (auto it = _white_block.rbegin(); it != _white_block.rend(); ++it) {
        if (last_right >= it->first && last_right <= it->second) {
            right_block = *it;
            break;
        }
    }

    // 记录当前行的左、右点
    LeftPoints.emplace_back(row, left_block.first);
    RightPoints.emplace_back(row, right_block.second);
}