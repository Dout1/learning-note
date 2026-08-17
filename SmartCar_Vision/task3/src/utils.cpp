#include "utils.h"
#include <numeric>

double calVariance(const std::vector<int>& vec) {
    if (vec.size() < 1) return 0.0;
    double avg = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
    double sigma = 0.0;
    for (int v : vec) {
        sigma += (v - avg) * (v - avg);
    }
    sigma /= vec.size();
    return sigma;
}