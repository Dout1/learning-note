#include "otsu.h"
#include <iostream>
#include <string>

void processImage(const std::string& imagePath) {
    std::cout << "\n=======================================" << std::endl;
    std::cout << "prosessing image: " << imagePath << std::endl;

    //读取灰度图
    cv::Mat grayImage = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    if (grayImage.empty()) {
        std::cerr << "Error: Could not open or find the image!" << std::endl;
        return;
    }

    //方法1：OPENCV自带的Otsu算法
    double cvThreshold = 0.0;
    cv::Mat binaryCV = otsuOPENCV(grayImage, cvThreshold);
    std::cout << "OpenCV Otsu threshold: " << cvThreshold << std::endl;

    //方法2：手动Otsu算法
    double manualThreshold = 0.0;
    cv::Mat binaryManual = otsuBinarize(grayImage, manualThreshold);
    std::cout << "Manual Otsu threshold: " << manualThreshold << std::endl;

    //显示灰度直方图 
    plotGrayHistogram(grayImage, "hist_" + imagePath.substr(imagePath.find_last_of('/') + 1));

    //显示结果对比
    cv::imshow("Original Image", grayImage);
    cv::imshow("Binary Image (OpenCV Otsu)", binaryCV);
    cv::imshow("Binary Image (Manual Otsu)", binaryManual);
    cv::waitKey(0);

    //保存结果图像
    std::string baseName = imagePath.substr(imagePath.find_last_of('/') + 1);
    cv::imwrite("binary_cv_" + baseName, binaryCV);
    cv::imwrite("binary_manual_" + baseName, binaryManual);
}

int main(){
    std::cout << "============= OTSU 大津法=============" << std::endl;
    //测试正常光照
    processImage("../res/1.jpg");
    //测试极端光照
    processImage("../res/2.jpg");
    processImage("../res/3.jpg");
    processImage("../res/4.jpg");
    processImage("../res/5.jpg");

    cv::destroyAllWindows();
    std::cout << "\n所有图像处理完成，结果已保存" << std::endl;
    return 0;
}