#ifndef OTSU_H
#define OTSU_H

#include <opencv2/opencv.hpp>
#include <vector>

int otsuThresholdManual(const cv::Mat& grayImage);

cv::Mat otsuBinarize(const cv::Mat& grayImage, int threshold);

cv::Mat otsuOPENCV(const cv::Mat& grayImage, double& outThreshold);

void plotGrayHistogram(const cv::Mat& grayImage, const std::string& savePath = "");

#endif // OTSU_H