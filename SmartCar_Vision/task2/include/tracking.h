#ifndef TRACKING_H
#define TRACKING_H

#include <opencv2/opencv.hpp>
#include <vector>

/**
 * @brief 二维平面坐标点（行 = x，列 = y）
 */
struct Point {
    int x;  // 行坐标（图像高度方向）
    int y;  // 列坐标（图像宽度方向）

    Point() : x(0), y(0) {}
    Point(int row, int col) : x(row), y(col) {}
};

/**
 * @brief 赛道边线搜索类（遍历搜线法）
 */
class Tracking {
public:
    Tracking();

    /**
     * @brief 主处理函数：输入二值图像，提取左、右边线点集
     * @param img_binary 输入的二值化图像（白色=255 为可行域）
     */
    void process(const cv::Mat& img_binary);

    /**
     * @brief 获取左边线点集
     */
    const std::vector<Point>& getLeftPoints() const { return LeftPoints; }

    /**
     * @brief 获取右边线点集
     */
    const std::vector<Point>& getRightPoints() const { return RightPoints; }

private:
    std::vector<Point> LeftPoints;   // 左边线点集
    std::vector<Point> RightPoints;  // 右边线点集

    bool start_flag;                 // 是否为起始行搜索阶段

    int _top_cut;                    // 顶部裁切行
    int _bottom_cut;                 // 底部裁切行

    /**
     * @brief 存储一行中搜索到的白色色块，每个元素为 (start_col, end_col)
     */
    std::vector<std::pair<int, int>> _white_block;

    /**
     * @brief 遍历一行像素，提取所有白色色块
     * @param img_binary 二值图像
     * @param row 当前行号
     */
    void search_white_block(const cv::Mat& img_binary, int row);

    /**
     * @brief 起始行搜索：选择最宽的白色色块作为赛道入口
     * @param row 当前行号
     */
    void search_start_line(int row);

    /**
     * @brief 非起始行搜索：基于上一行边线，利用连通性寻找当前行边线
     * @param row 当前行号
     */
    void search_normal_line(int row);
};

#endif // TRACKING_H