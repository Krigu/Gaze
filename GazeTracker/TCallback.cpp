/* 
 * File:   TCallback.cpp
 * Author: krigu
 * 
 * Created on December 13, 2012, 9:55 PM
 */

#include <iostream>

#include "TCallback.hpp"

#include "opencv2/highgui/highgui.hpp";

TCallback::TCallback() {
}

TCallback::TCallback(const TCallback& orig) {
}

TCallback::~TCallback() {
}

void TCallback::imageProcessed(Mat &result){
    imshow("frame", result);
    waitKey(100);
}

void TCallback::imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector){
    imshow("frame", resultImage);
    waitKey(100);
}

