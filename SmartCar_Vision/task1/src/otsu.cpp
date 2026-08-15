#include "otsu.h"
#include <iostream>
#include <cmath>

int otsuThresholdManual(const cv::Mat& grayImage) {
    // ---- 1. 统计灰度直方图 ----
    int hist[256] = {0};
    for (int r = 0; r < grayImage.rows; ++r) {
        const uchar* row = grayImage.ptr<uchar>(r);
        for (int c = 0; c < grayImage.cols; ++c) {
            hist[row[c]]++;
        }
    }

    int totalPixels = grayImage.rows * grayImage.cols;

    // ---- 2. 遍历所有阈值，计算类间方差 ----
    double maxVariance = -1.0;
    int bestThreshold = 0;

    for (int t = 0; t < 256; ++t) {
        // 背景类（0 ~ t）
        int w0 = 0;
        long long sum0 = 0;
        for (int i = 0; i <= t; ++i) {
            w0 += hist[i];
            sum0 += (long long)i * hist[i];
        }

        // 前景类（t+1 ~ 255）
        int w1 = 0;
        long long sum1 = 0;
        for (int i = t + 1; i < 256; ++i) {
            w1 += hist[i];
            sum1 += (long long)i * hist[i];
        }

        if (w0 == 0 || w1 == 0) continue;

        double u0 = (double)sum0 / w0;   // 背景均值
        double u1 = (double)sum1 / w1;   // 前景均值
        double variance = (double)w0 * w1 * (u0 - u1) * (u0 - u1);

        if (variance > maxVariance) {
            maxVariance = variance;
            bestThreshold = t;
        }
    }

    return bestThreshold;
}

cv::Mat otsuBinarize(const cv::Mat& grayImage, int threshold) {
    cv::Mat binary(grayImage.size(), CV_8UC1);
    for (int r = 0; r < grayImage.rows; ++r) {
        const uchar* srcRow = grayImage.ptr<uchar>(r);
        uchar* dstRow = binary.ptr<uchar>(r);
        for (int c = 0; c < grayImage.cols; ++c) {
            dstRow[c] = (srcRow[c] > threshold) ? 255 : 0;
        }
    }
    return binary;
}

cv::Mat otsuOPENCV(const cv::Mat& grayImage, double& outThreshold) {
    cv::Mat binary;
    outThreshold = cv::threshold(
        grayImage, binary, 0, 255,
        cv::THRESH_BINARY | cv::THRESH_OTSU
    );
    return binary;
}

void plotGrayHistogram(const cv::Mat& grayImage, const std::string& savePath) {
    int hist[256] = {0};
    for (int r = 0; r < grayImage.rows; ++r) {
        const uchar* row = grayImage.ptr<uchar>(r);
        for (int c = 0; c < grayImage.cols; ++c) {
            hist[row[c]]++;
        }
    }

    // 找最大值用于归一化
    int maxVal = 0;
    for (int i = 0; i < 256; ++i) {
        if (hist[i] > maxVal) maxVal = hist[i];
    }

    // 创建直方图图像
    int histW = 512, histH = 400;
    cv::Mat histImg(histH, histW, CV_8UC3, cv::Scalar(255, 255, 255));

    int binW = cvRound((double)histW / 256);
    for (int i = 0; i < 256; ++i) {
        int h = cvRound(((double)hist[i] / maxVal) * histH);
        cv::rectangle(
            histImg,
            cv::Point(i * binW, histH - h),
            cv::Point((i + 1) * binW - 1, histH),
            cv::Scalar(0, 0, 0),
            -1
        );
    }

    if (!savePath.empty()) {
        cv::imwrite(savePath, histImg);
        std::cout << "直方图已保存至: " << savePath << std::endl;
    }

    cv::imshow("Gray Histogram", histImg);
    cv::waitKey(1);
}