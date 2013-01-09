/* 
 * File:   TCallback.cpp
 * Author: krigu
 * 
 * Created on December 13, 2012, 9:55 PM
 */

#include <iostream>

#include "TCallback.hpp"

#include "opencv2/highgui/highgui.hpp"

TCallback::TCallback() {
}

TCallback::TCallback(const TCallback& orig) {
}

TCallback::~TCallback() {
}

bool TCallback::imageProcessed(Mat &result) {
    imshow("frame", result);
    int keycode = waitKey(1);
    if (keycode == 32) // space
        while (waitKey(10) != 32);
    return true;
}

bool TCallback::imageProcessed(Mat &resultImage, MeasureResult &result, Point2f &gazeVector) {
    imshow("frame", resultImage);
    
    Point p = calib.calcCoordinates(gazeVector);
    
    cout << "Point p: " << p << endl;
    
    int keycode = waitKey(1);
    if (keycode == 32) // space
        while (waitKey(10) != 32);
    return true;
}

