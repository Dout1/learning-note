#ifndef TRACK_H
#define TRACK_H

#include <vector>
#include <opencv2/opencv.hpp>

struct TrackPoint {
    int x;
    int y;
    TrackPoint() : x(0), y(0) {}
    TrackPoint(int x_, int y_) : x(x_), y(y_) {}
};

class Track {
public:
    std::vector<TrackPoint> leftPoints;
    std::vector<TrackPoint> rightPoints;
    double sigma_left;
    double sigma_right;

    Track();
    void calSigma();
};

#endif