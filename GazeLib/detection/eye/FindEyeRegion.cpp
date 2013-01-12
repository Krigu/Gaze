#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FindEyeRegion.hpp"
#include "../../utils/log.hpp"
#include "../../config/GazeConfig.hpp"

#include "../../exception/GazeExceptions.hpp"

using namespace std;
using namespace cv;

/**
 * Adjusts the width and takes only the left side
 * 
 * @param r2 Eyeregion
 */
void takeLeftEye(Rect* r2) {
    r2->width = (r2->width / 2);
}

/**
 * Adjusts the widht and the x-coordinate and takes only 
 * the right part of the provided rect
 * 
 * @param r2 Eyeregion
 */
void takeRightEye(Rect* r2) {
    r2->x += (r2->width / 2);
    r2->width = (r2->width / 2);
    //  return (r1->x > r2->x) ? r1 : r2;
}

FindEyeRegion::FindEyeRegion(FindGlints& findGlints) : findGlints(findGlints) {
    if (!eye_region_classifier.load(
            GazeConfig::inWorkingDir(
            "../__haarcascades/haarcascade_mcs_eyepair_big.xml"))) {
        throw GazeException("ERROR: Could not load eyepair classifier cascade");
    }

    if (!eye_classifier.load(
            GazeConfig::inWorkingDir(
            "../__haarcascades/haarcascade_eye.xml"))) {
        throw GazeException("ERROR: Could not load eye classifier cascade");
    }

    // Init adjust functions
    rightEyeRegionClipper = &takeRightEye;
    leftEyeRegionClipper = &takeLeftEye;
}

bool FindEyeRegion::findEye(Mat &image, Rect& eyeRect,
        eyeRegionAdjustFunction& compareFunc) {
    vector<Rect> faces;
    eye_region_classifier.detectMultiScale(image, faces, 1.1, 1,
            0 | CV_HAAR_SCALE_IMAGE,
            Size(GazeConfig::HAAR_EYEREGION_MIN_WIDTH,
            GazeConfig::HAAR_EYEREGION_MIN_HEIGHT));

    LOG_D("Faces detected: " << faces.size());
    if (faces.size() < 1) {
        LOG_W("No face detected!");
        return false;
    }

    Rect eyeRegion = faces.at(0);
    rectangle(image, faces.at(0), Scalar(255, 255, 255), 3);

    // Extract right or left part of eye region
    (*compareFunc)( &eyeRegion);

    vector<Rect> eyes;
    Mat region = image(eyeRegion);
    eye_classifier.detectMultiScale(region, eyes, 1.2, 1,
            0 | CV_HAAR_SCALE_IMAGE,
            Size(region.rows / 2,
            region.rows / 2) ,
            Size(region.rows ,
            region.rows ));

    LOG_D("Eyes detected before filter: " << eyes.size());
    // removes all rects with no glints in it
    removeInvalidRects(region, eyes);
    LOG_D("Eyes detected after filter: " << eyes.size());

    // No eye detected
    if (eyes.empty()) {
        return false;
    }// One or multiple eye detected
    else if (eyes.size() > 0) {
        eyeRect = eyes.at(0);
    }

    // Add offset
    eyeRect.x += eyeRegion.x;
    eyeRect.y += eyeRegion.y;

    return true;
}

/**
 * Checks if a region has glints in it
 * 
 * @param image
 * @param eyeRect
 * @return 
 */
bool FindEyeRegion::hasGlintsInRect(Mat &image, Rect& eyeRect) {
    vector<cv::Point> glints;
    Point2f glintCenter;
    Mat img = image(eyeRect);

    return findGlints.findGlints(img, glints, glintCenter);
}

/**
 * Removes all eye regions with no glints in it
 * 
 * @param image
 * @param regions
 * @return 
 */
void FindEyeRegion::removeInvalidRects(Mat& image, vector<Rect>& regions) {
    std::vector<Rect>::iterator iter;

    for (iter = regions.begin(); iter != regions.end();) {
        if (!hasGlintsInRect(image, *iter))
            iter = regions.erase(iter);

        else
            ++iter;
    }
}

/**
 * 
 * @param image
 * @param eyeRect
 * @return 
 */
bool FindEyeRegion::findRightEye(Mat &image, Rect& eyeRect) {

    return findEye(image, eyeRect, rightEyeRegionClipper);
}

/**
 * 
 * @param image
 * @param eyeRect
 * @return 
 */
bool FindEyeRegion::findLeftEye(Mat &image, Rect& eyeRect) {

    return findEye(image, eyeRect, leftEyeRegionClipper);
}

/**
 * 
 * @param image
 * @param eyeRect
 * @return 
 */
bool FindEyeRegion::findEye(Mat &image, Rect& eyeRect) {
    if (GazeConfig::DETECT_LEFT_EYE)
        return findLeftEye(image, eyeRect);
    else
        return findRightEye(image, eyeRect);
}
