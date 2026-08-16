#include <opencv2/opencv.hpp>
#include "tracking.h"

/**
 * @brief 图像预处理：灰度化 + 二值化
 * @param frame 原始BGR图像
 * @return 二值化图像（白色=255）
 */
cv::Mat preprocess_frame(const cv::Mat& frame) {
    cv::Mat gray, binary;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, binary, 200, 255, cv::THRESH_BINARY);
    return binary;
}

/**
 * @brief 将左、右点集绘制在图像上
 * @param frame 原始图像
 * @param tracker Tracking对象
 * @return 绘制后的图像
 */
cv::Mat draw_points(const cv::Mat& frame, const Tracking& tracker) {
    cv::Mat result = frame.clone();

    // 左边线用绿色绘制
    for (const auto& p : tracker.getLeftPoints()) {
        cv::circle(result, cv::Point(p.y, p.x), 1, cv::Scalar(0, 255, 0), -1);
    }

    // 右边线用蓝色绘制
    for (const auto& p : tracker.getRightPoints()) {
        cv::circle(result, cv::Point(p.y, p.x), 1, cv::Scalar(255, 0, 0), -1);
    }

    return result;
}

int main() {
    // 打开视频文件
    cv::VideoCapture cap("../res/sample.avi");
    if (!cap.isOpened()) {
        return -1;
    }

    Tracking tracker;

    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        // 1. 预处理得到二值图像
        cv::Mat binary = preprocess_frame(frame);

        // 2. 边线搜索
        tracker.process(binary);

        // 3. 绘制点集
        cv::Mat result = draw_points(frame, tracker);

        // 4. 显示结果
        cv::imshow("Binary", binary);
        cv::imshow("Tracking Result", result);

        // 按 'q' 退出
        if (cv::waitKey(30) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}