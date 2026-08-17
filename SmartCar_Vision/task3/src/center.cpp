#include "center.h"
#include "bezier.h"
#include "utils.h"
#include <cmath>

Center::Center() : sigma_center(0.0) {}

// ===== 中值法 =====
void Center::processMedian(const Track& track, int imgRows) {
    centerPoints.clear();

    // 按行组织左右点
    std::vector<int> leftRow(imgRows, -1), rightRow(imgRows, -1);
    for (const auto& p : track.leftPoints) leftRow[p.y] = p.x;
    for (const auto& p : track.rightPoints) rightRow[p.y] = p.x;

    for (int y = 0; y < imgRows; ++y) {
        if (leftRow[y] >= 0 && rightRow[y] >= 0) {
            float mx = (leftRow[y] + rightRow[y]) / 2.0f;
            centerPoints.emplace_back(mx, static_cast<float>(y));
        }
    }
}

// ===== 贝塞尔法 =====
void Center::processBezier(const Track& track, double dt) {
    centerPoints.clear();

    // ===== 安全检查：左右点必须至少有 12 个 =====
    if (track.leftPoints.size() < 12 || track.rightPoints.size() < 12) {
        // 点太少，直接放弃贝塞尔，什么都不做
        // （main.cpp 里会退化到中值法，或者你可以在这里直接调 processMedian）
        return;
    }

    auto getThirdPoints = [](const std::vector<TrackPoint>& pts) -> std::vector<cv::Point2f> {
        std::vector<cv::Point2f> result;
        if (pts.size() < 12) return result;
        int step = pts.size() / 3;
        for (int i = 0; i < 4; ++i) {
            int idx = i * step;
            if (idx < pts.size()) {
                result.emplace_back(static_cast<float>(pts[idx].x),
                                    static_cast<float>(pts[idx].y));
            }
        }
        return result;
    };

    auto leftThird = getThirdPoints(track.leftPoints);
    auto rightThird = getThirdPoints(track.rightPoints);

    // 如果采样出来的点不够 4 个，直接返回
    if (leftThird.size() != 4 || rightThird.size() != 4) return;

    std::vector<cv::Point2f> midPoints;
    for (int i = 0; i < 4; ++i) {
        midPoints.emplace_back((leftThird[i].x + rightThird[i].x) / 2.0f,
                               (leftThird[i].y + rightThird[i].y) / 2.0f);
    }

    centerPoints = Bezier(dt, midPoints);
}


// ===== 计算中心点集方差 =====
void Center::calSigmaCenter() {
    std::vector<int> xs;
    for (const auto& p : centerPoints) xs.push_back(static_cast<int>(p.x));
    sigma_center = calVariance(xs);
}

// ===== 绘制中心线 =====
void Center::drawCenterLine(cv::Mat& img, cv::Scalar color) {
    for (size_t i = 1; i < centerPoints.size(); ++i) {
        cv::line(img, centerPoints[i - 1], centerPoints[i], color, 2);
    }
}

// ===== 绘制方差文字 =====
void Center::drawSigmaText(cv::Mat& img, double sigmaLeft, double sigmaRight,
                            const cv::Point& pos) {
    std::string text1 = "Sigma Left: " + std::to_string(sigmaLeft).substr(0, 5);
    std::string text2 = "Sigma Right: " + std::to_string(sigmaRight).substr(0, 5);
    std::string text3 = "Sigma Center: " + std::to_string(sigma_center).substr(0, 5);

    cv::putText(img, text1, pos, cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 2);
    cv::putText(img, text2, cv::Point(pos.x, pos.y + 25), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 2);
    cv::putText(img, text3, cv::Point(pos.x, pos.y + 50), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 2);
}