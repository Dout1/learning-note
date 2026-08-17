#ifndef CENTER_H
#define CENTER_H

#include <vector>
#include "track.h"
#include <opencv2/opencv.hpp>

class Center {
public:
    std::vector<cv::Point2f> centerPoints;  // 浮点型中心点
    double sigma_center;

    Center();

    // 中值法拟合中心线
    void processMedian(const Track& track, int imgRows);

    // 贝塞尔法拟合中心线
    void processBezier(const Track& track, double dt = 0.05);

    // 计算中心点集方差
    void calSigmaCenter();

    // 绘制中心线到图像
    void drawCenterLine(cv::Mat& img, cv::Scalar color = cv::Scalar(0, 0, 255));

    // 绘制方差文字到图像
    void drawSigmaText(cv::Mat& img, double sigmaLeft, double sigmaRight,
                       const cv::Point& pos = cv::Point(10, 30));
};

#endif