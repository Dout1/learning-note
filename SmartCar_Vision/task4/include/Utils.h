#pragma once

#include "common.h"
#include <vector>

/**
 * @brief N 点贝塞尔曲线（对齐赛曙例程 Bezier）
 * @param dt     采样步长（0~1）
 * @param input  控制点集
 * @return 曲线采样点集
 */
std::vector<POINT> Bezier(double dt, const std::vector<POINT>& input);
