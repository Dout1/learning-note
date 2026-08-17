#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include <opencv2/opencv.hpp>

// 阶乘
int factorial(int n);

// 贝塞尔曲线拟合，dt 控制步长
std::vector<cv::Point2f> Bezier(double dt, const std::vector<cv::Point2f>& input);

#endif