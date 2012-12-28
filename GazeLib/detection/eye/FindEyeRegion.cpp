#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FindEyeRegion.hpp"
#include "../../utils/log.hpp"
#include "../../config/GazeConfig.hpp"

using namespace std;
using namespace cv;

// Compares the size of two rects

bool compareRect(Rect r1, Rect r2) {
    return ((r1.width * r1.height) < (r2.width * r2.height));
}

Rect* takeLeftEye(Rect* r1, Rect* r2) {
    return (r1->x < r2->x) ? r1 : r2;
}

Rect* takeRightEye(Rect* r1, Rect* r2) {
    return (r1->x > r2->x) ? r1 : r2;
}

FindEyeRegion::FindEyeRegion(FindGlints& findGlints) : findGlints(findGlints) {
    // TODO exit when classiefier can0t be loaded
    if (!eye_region_classifier.load(
            GazeConfig::inHomeDirectory(
            "/Dropbox/gaze/haar/parojosG.xml"))) {
        LOG_W("ERROR: Could not load left eye classifier cascade");
    }

    if (!eye_classifier.load(
            GazeConfig::inHomeDirectory(
            "/Dropbox/gaze/haar/haar_left_eye.xml"))) {
        LOG_W("ERROR: Could not load left eyes classifier cascade");
    }

    // Init compare functions
    rightEyeCompareFunc = &takeRightEye;
    leftEyeCompareFunc = &takeLeftEye;
}

bool FindEyeRegion::findEye(Mat &image, Rect& eyeRect,
        eyeCompareFunction& compareFunc) {
    vector<Rect> faces;
    eye_region_classifier.detectMultiScale(image, faces, 1.1, 1,
            0 | CV_HAAR_SCALE_IMAGE,
            Size(GazeConfig::HAAR_EYEREGION_MIN_WIDTH,
            GazeConfig::HAAR_EYEREGION_MIN_HEIGHT));

    cout << "Faces detected: " << faces.size() << endl;
    if (faces.size() < 1) {
        LOG_W("No face detected!");
        return false;
    }

    // TODO: What to do with multiple detections?
    Rect eyeRegion = faces.at(0);

    rectangle(image, faces.at(0), Scalar(255, 255, 255), 3);


    // TODO extract min and maxsize to constants
    vector<Rect> eyes;
    Mat region = image(eyeRegion);
    eye_classifier.detectMultiScale(region, eyes, 1.1, 2,
            0 | CV_HAAR_SCALE_IMAGE,
            Size(GazeConfig::HAAR_EYE_MIN_WIDTH,
            GazeConfig::HAAR_EYE_MIN_HEIGHT),
            Size(GazeConfig::HAAR_EYE_MAX_WIDTH,
            GazeConfig::HAAR_EYE_MAX_HEIGHT));

    cout << "Eyes detected before filter: " << eyes.size() << endl;
    
    // removes all rects with no glints in it
    removeInvalidRects(region, eyes);
    
    cout << "Eyes detected after filter: " << eyes.size() << endl;
    
    // No eye detected
    if (eyes.empty()) {
        return false;
    }        // One eye detected
    else if (eyes.size() == 1) {
        eyeRect = eyes.at(0);
    }// Multiple eyes. Filter false positives out
        // Take one of the two most similar rects
    else {
        sort(eyes.begin(), eyes.end(), compareRect);

        // Take a high value so minDistance will be overriden with the first iteration
        int minDistance = 100000;
        Rect* r1 = NULL;
        Rect* r2 = NULL;

        for (std::vector<int>::size_type i = 0; i != (eyes.size() - 1); i++) {

            int distance = (eyes[i + 1].width * eyes[i + 1].height)
                    - (eyes[i].width * eyes[i].height);

            if (distance < minDistance) {
                r1 = &eyes[i];
                r2 = &eyes[i + 1];
                minDistance = distance;
            }

            rectangle(image, eyes.at(i), Scalar(255, 255, 255), 3);
            rectangle(image, eyes.at(i + 1), Scalar(255, 255, 255), 3);
        }
        // TODO Check if r1, r2 is not null
        // Take left rect
        eyeRect = *compareFunc(r1, r2);
    }

    // Add offset
    eyeRect.x += eyeRegion.x;
    eyeRect.y += eyeRegion.y;

    return true;
}

// Checks if a region has glints in it

bool FindEyeRegion::hasGlintsInRect(Mat &image, Rect& eyeRect) {
    vector<cv::Point> glints;
    Point2f glintCenter;
    Mat img = image(eyeRect);
    return findGlints.findGlints(img, glints, glintCenter);
}

// Removes all eye regions with no glints in it

bool FindEyeRegion::removeInvalidRects(Mat& image, vector<Rect>& regions) {
    std::vector<Rect>::iterator iter;

    for (iter = regions.begin(); iter != regions.end();) {
        if (!hasGlintsInRect(image, *iter))
            iter = regions.erase(iter);
        else
            ++iter;
    }
}

bool FindEyeRegion::findRightEye(Mat &image, Rect& eyeRect) {
    return findEye(image, eyeRect, rightEyeCompareFunc);
}

bool FindEyeRegion::findLeftEye(Mat &image, Rect& eyeRect) {
    return findEye(image, eyeRect, leftEyeCompareFunc);
}

bool FindEyeRegion::findEye(Mat &image, Rect& eyeRect) {
    if (GazeConfig::DETECT_LEFT_EYE)
        return findLeftEye(image, eyeRect);
    else
        return findRightEye(image, eyeRect);
}
