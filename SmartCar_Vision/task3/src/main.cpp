#include <opencv2/opencv.hpp>
#include "track.h"
#include "center.h"
#include <iostream>

using namespace std;

// ===== 从二值图中提取左右边缘点（简化版，实际需根据你的任务二代码替换）=====
Track extractEdges(const cv::Mat& binary) {
    Track track;
    // 简单示例：扫描每一行，找最左和最右的白点
    for (int y = 0; y < binary.rows; ++y) {
        int leftX = -1, rightX = -1;
        for (int x = 0; x < binary.cols; ++x) {
            if (binary.at<uchar>(y, x) > 0) {
                if (leftX < 0) leftX = x;
                rightX = x;
            }
        }
        if (leftX >= 0 && rightX >= 0 && leftX != rightX) {
            track.leftPoints.emplace_back(leftX, y);
            track.rightPoints.emplace_back(rightX, y);
        }
    }
    return track;
}

int main() {
    cv::VideoCapture cap("../res/sample.avi");
    if (!cap.isOpened()) {
        cerr << "Cannot open video!" << endl;
        return -1;
    }

    cv::Mat frame, gray, binary;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        threshold(gray, binary, 200, 255, cv::THRESH_BINARY);

        Track track = extractEdges(binary);
        track.calSigma();

        // ===== 贝塞尔拟合 =====
        Center centerBezier;
        bool useBezier = (track.leftPoints.size() >= 12 && track.rightPoints.size() >= 12);
        if (useBezier) {
            centerBezier.processBezier(track, 0.02);
            centerBezier.calSigmaCenter();
            centerBezier.drawCenterLine(frame, cv::Scalar(0, 0, 255));  // 红色 = 贝塞尔
        }

        // ===== 中值法拟合 =====
        Center centerMedian;
        centerMedian.processMedian(track, frame.rows);
        centerMedian.calSigmaCenter();
        centerMedian.drawCenterLine(frame, cv::Scalar(255, 0, 0));      // 蓝色 = 中值法

        // ===== 方差文字 =====
        double sigmaCenter = useBezier ? centerBezier.sigma_center : centerMedian.sigma_center;
        // 用贝塞尔的方差，或者两个都画
        std::string text1 = "Sigma Left: " + std::to_string(track.sigma_left).substr(0, 5);
        std::string text2 = "Sigma Right: " + std::to_string(track.sigma_right).substr(0, 5);
        std::string text3 = "Sigma Center(Bezier): " + std::to_string(centerBezier.sigma_center).substr(0, 5);
        std::string text4 = "Sigma Center(Median): " + std::to_string(centerMedian.sigma_center).substr(0, 5);

        putText(frame, text1, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
        putText(frame, text2, cv::Point(10, 55), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
        putText(frame, text3, cv::Point(10, 80), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);   // 红字
        putText(frame, text4, cv::Point(10, 105), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);  // 蓝字

        cv::imshow("Center Line", frame);
        if (cv::waitKey(30) == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}