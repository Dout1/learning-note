#include "Utils.h"
#include <cmath>

static int factorial(int n) {
    int res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

std::vector<POINT> Bezier(double dt, const std::vector<POINT>& input) {
    std::vector<POINT> output;
    int n = (int)input.size() - 1;
    if (n < 0) return output;

    for (double t = 0.0; t <= 1.0; t += dt) {
        double x_sum = 0.0, y_sum = 0.0;
        for (int i = 0; i <= n; ++i) {
            double comb = static_cast<double>(factorial(n)) /
                          (factorial(i) * factorial(n - i));
            double k = comb * std::pow(1 - t, n - i) * std::pow(t, i);
            x_sum += k * input[i].x;
            y_sum += k * input[i].y;
        }
        output.emplace_back((int)x_sum, (int)y_sum);
    }
    return output;
}
