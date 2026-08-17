#include "track.h"
#include "utils.h"

Track::Track() : sigma_left(0.0), sigma_right(0.0) {}

void Track::calSigma() {
    std::vector<int> leftX, rightX;
    for (const auto& p : leftPoints) leftX.push_back(p.x);
    for (const auto& p : rightPoints) rightX.push_back(p.x);
    sigma_left = calVariance(leftX);
    sigma_right = calVariance(rightX);
}