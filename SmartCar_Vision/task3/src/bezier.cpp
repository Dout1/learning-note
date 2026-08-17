#include "bezier.h"
#include <cmath>

int factorial(int n) {
    int res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

std::vector<cv::Point2f> Bezier(double dt, const std::vector<cv::Point2f>& input) {
    std::vector<cv::Point2f> output;
    int n = input.size() - 1;
    if (n < 0) return output;

    for (double t = 0.0; t <= 1.0; t += dt) {
        float x_sum = 0.0f, y_sum = 0.0f;
        for (int i = 0; i <= n; ++i) {
            double comb = static_cast<double>(factorial(n)) /
                          (factorial(i) * factorial(n - i));
            double k = comb * std::pow(1 - t, n - i) * std::pow(t, i);
            x_sum += static_cast<float>(k * input[i].x);
            y_sum += static_cast<float>(k * input[i].y);
        }
        output.emplace_back(x_sum, y_sum);
    }
    return output;
}