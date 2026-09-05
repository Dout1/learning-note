#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

// ===== 图像尺寸常量（对齐赛曙例程，320x240）=====
#define ROWSIMAGE 240
#define COLSIMAGE 320

// ===== 日志宏（对齐赛曙例程）=====
#define LOGLN(x) std::cout << x << std::endl

/**
 * @brief 二维平面坐标点（行 = x，列 = y）
 *        与赛曙例程 common.hpp 中 POINT 保持一致
 */
struct POINT {
    int x;       // 行坐标（图像高度方向）
    int y;       // 列坐标（图像宽度方向）
    double slope; // 边缘斜率

    POINT() : x(0), y(0), slope(0) {}
    POINT(int _x, int _y) : x(_x), y(_y), slope(0) {}
};

/**
 * @brief 场景枚举（对齐赛曙例程 Scene）
 */
enum class Scene {
    NormalScene,     // 普通赛道
    CateringScene,   // 餐饮/补给场景
    ObstacleScene,   // 障碍物场景
    ParkingScene,    // 泊车场景
};

/**
 * @brief 方差计算（对齐赛曙例程 sigma）
 */
inline double sigma(const std::vector<int>& v) {
    if (v.size() < 2) return 0.0;
    double sum = 0.0, sum2 = 0.0;
    for (int x : v) { sum += x; sum2 += (double)x * x; }
    double mean = sum / v.size();
    double var = sum2 / v.size() - mean * mean;
    return var > 0 ? var : 0.0;
}
