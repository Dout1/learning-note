#include <opencv2/opencv.hpp>
#include "common.h"
#include "Tracking.h"
#include "Crossroad.h"
#include "Utils.h"
#include <iostream>
#include <cstdio>
#include <string>

int main(int argc, char** argv) {
    int choice = (argc > 1) ? std::atoi(argv[1]) : 1;
    bool dumpMode = false;
    for (int i = 1; i < argc; ++i)
        if (std::string(argv[i]) == "--dump") dumpMode = true;

    std::string video;
    if (choice == 1) video = "../res/cross1.mp4";
    else if (choice == 2) video = "../res/cross2.mp4";
    else if (choice == 3) video = "../res/cross3.mp4";
    else video = "../res/demo.avi";

    std::cerr << "[INFO] video = " << video << " dump=" << dumpMode << std::endl;
    cv::VideoCapture cap(video);
    if (!cap.isOpened()) {
        std::cerr << "[ERR] Cannot open: " << video << std::endl;
        return -1;
    }

    cv::Mat frame, gray, bin;
    int f = 0;

    Tracking track;
    Crossroad cross;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, bin, 128, 255, cv::THRESH_BINARY);

        track.trackRecognition(bin, Scene::NormalScene, 0, 0);
        cross.process(track);

        if (dumpMode) {
            int ptsL = (int)track.pointsEdgeLeft.size();
            int ptsR = (int)track.pointsEdgeRight.size();
            int inl = (int)track.inlines.size();
            int inlX = inl ? track.inlines[0].x : -1;
            int inlY = inl ? track.inlines[0].y : -1;
            int wbN = (int)track.widthBlock.size();
            std::cout
                << "f=" << f
                << " ptsL=" << ptsL
                << " ptsR=" << ptsR
                << " inl=" << inl
                << " inlX=" << inlX << " inlY=" << inlY
                << " endL=" << track.GetEndIndex(0)
                << " endR=" << track.GetEndIndex(1)
                << " midL=" << track.GetMiddleIndex(0)
                << " midR=" << track.GetMiddleIndex(1)
                << " stdevL=" << track.stdevLeft
                << " stdevR=" << track.stdevRight
                << " step=" << (int)cross.step
                << " type=" << (int)cross.type
                << " lb=" << (ptsL ? track.pointsEdgeLeft[0].y : -1)
                << " rb=" << (ptsR ? track.pointsEdgeRight[0].y : -1)
                << " lt=" << (ptsL ? track.pointsEdgeLeft[ptsL - 1].y : -1)
                << " rt=" << (ptsR ? track.pointsEdgeRight[ptsR - 1].y : -1)
                << " wbLast=" << (wbN ? track.widthBlock[wbN - 1].y : -1)
                << std::endl;

            bool save = (cross.step == Crossroad::Fix) || (f % 20 == 0);
            if (save) {
                cv::Mat anno = frame.clone();
                track.drawImage(anno);
                cross.draw_img(anno);
                char name[256];
                std::snprintf(name, sizeof(name), "../dump/nv%d_f%04d.png", choice, f);
                cv::imwrite(name, anno);
            }
        } else {
            track.drawImage(frame);
            cross.draw_img(frame);
            cv::imshow("Binary", bin);
            cv::imshow("Cross Process", frame);
            int key = cv::waitKey(30);
            if (key == 'q' || key == 27) break;
        }
        f++;
    }
    return 0;
}
