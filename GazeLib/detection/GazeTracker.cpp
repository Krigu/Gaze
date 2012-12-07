/*
 * GazeTracker.cpp
 *
 *  Created on: Nov 24, 2012
 *      Author: krigu
 */

#include "GazeTracker.hpp"
#include "../utils/geometry.hpp"
#include "../utils/gui.hpp"
#include "../utils/log.hpp"
#include "../exception/GazeExceptions.hpp"

//#if __DEBUG_FINDGLINTS == 1 || __DEBUG_TRACKER == 1
#include "opencv2/highgui/highgui.hpp"
//#endif

using namespace cv;

GazeTracker::GazeTracker(ImageSource & imageSource, TrackerCallback *callback) :
imageSrc(imageSource), tracker_callback(callback), isRunning(false), framenumber(
0) {

}

bool GazeTracker::initialize(cv::Mat& frame, cv::Rect& frameRegion,
        cv::Point2f& frameCenter) {

    bool foundEye = false;
    short tries = 0;

    // TODO: refactor
    while (!foundEye) {
        
        try {
            eyeFinder.findLeftEye(frame, frameRegion);
            foundEye = true;
        } catch (GazeException e) {
            cerr << e.what() << endl;
        }
        
        tries++;

        if (tries > GazeConstants::HAAR_FINDREGION_MAX_TRIES)
            return false;
    }

    frameCenter = calcRectBarycenter(frameRegion);
    frame = frame(frameRegion);

    return true;
}

void GazeTracker::adjustRect(cv::Point2f& currentCenter, cv::Rect& frameRegion) {
    int width = frameRegion.width;
    int height = frameRegion.height;

    int x = frameRegion.x + currentCenter.x - (width / 2);
    int y = frameRegion.y + currentCenter.y - (height / 2);

    // Range check
    x = (x > 0) ? x : 0;
    y = (y > 0) ? y : 0;

    frameRegion = Rect(x, y, width, height);
}

GazeTracker::~GazeTracker() {
    // TODO Auto-generated destructor stub
}

bool GazeTracker::startTracking() {

    Mat currentFrame;
    Point2f glintCenter;
    Point2f gazeVector;

    bool hasImage = imageSrc.nextGrayFrame(currentFrame);
    // TODO: return error?
    if (!hasImage) {
        LOG_W("No image");
        return false;
    }

    bool foundRegion = initialize(currentFrame, frameRegion, glintCenter);
    if (!foundRegion) {
        LOG_W("No region found");
        return false;
    }

    isRunning = true;
    int noGlints = 0;

    // main loop
    while (isRunning) {

        // Get next frame
        if (!imageSrc.nextGrayFrame(currentFrame)) {
            LOG_D("No more frames");
            break;
        }

        currentFrame = currentFrame(frameRegion);
        MeasureResult result = measureFrame(currentFrame, gazeVector, glintCenter);

        Point2f smoothed_gace_vec;
        switch (result) {
            case MEASURE_OK:

                this->smoothSignal(gazeVector, smoothed_gace_vec,
                        this->last_gaze_vectors, framenumber);
                //c.printPoint(smoothed_gace_vec);

                LOG_D("Current GazeVector: " << gazeVector);

                ++framenumber;
                break;

            case FINDGLINT_FAILED:
                noGlints++;
                if (noGlints > 5) {
                    LOG_W("no glints found. need to reinitialize");
                    imageSrc.nextGrayFrame(currentFrame);
                    initialize(currentFrame, frameRegion, glintCenter);

                    noGlints = 0;
                    framenumber = 0; // restart the smoothing
                }
                break;

            case FINDPUPIL_FAILED:
                //TODO
                break;
        }

        // notify our callback about the processed frames...
        if (this->tracker_callback != NULL)
            tracker_callback->imageProcessed(currentFrame);

        int keycode = waitKey(50);
        if (keycode == 32) // space
            while (waitKey(100) != 32)
                ;
    }
    return true;
}

GazeTracker::MeasureResult GazeTracker::measureFrame(Mat &frame, Point2f &gazeVector, Point2f glintCenter) {
    vector<cv::Point> glints;
    float radius;
    Point2f pupilCenter;

    if (glintFinder.findGlints(frame, glints, glintCenter)) {

        if (!starburst.processImage(frame, glints, glintCenter,
                pupilCenter, radius)) {
            return FINDPUPIL_FAILED;
        }

        adjustRect(glintCenter, frameRegion);

    } else {
        return FINDGLINT_FAILED;
    }

    circle(frame, pupilCenter, radius, Scalar(255, 255, 255));
    cross(frame, glintCenter, 10);
    cross(frame, pupilCenter, 5);
    imshow("Tracker", frame);

    // now calculate the gaze vector
    gazeVector.x = glintCenter.x - pupilCenter.x;
    gazeVector.y = glintCenter.y - pupilCenter.y;

    return MEASURE_OK;
}

void GazeTracker::smoothSignal(Point2f &measured, Point2f &smoothed, Point2f data[],
        unsigned int framenumber) {
    if (framenumber < GazeConstants::NUM_OF_SMOOTHING_FRAMES) {
        // nothing to smooth here
        smoothed.x = measured.x;
        smoothed.y = measured.y;
    } else {
        smoothed.x = 0;
        smoothed.y = 0;
        for (unsigned short i = 0; i < GazeConstants::NUM_OF_SMOOTHING_FRAMES;
                ++i) {
            smoothed.x += data[i].x;
            smoothed.y += data[i].y;
        }
        smoothed.x /= GazeConstants::NUM_OF_SMOOTHING_FRAMES;
        smoothed.y /= GazeConstants::NUM_OF_SMOOTHING_FRAMES;
    }
    data[framenumber % GazeConstants::NUM_OF_SMOOTHING_FRAMES] = measured;
}

CalibrationData GazeTracker::measurePoint(Point2f pointOnScreen, unsigned int duration) {

    Mat currentFrame;
    Point2f glintCenter;

    bool hasImage = imageSrc.nextGrayFrame(currentFrame);
    // TODO: return error?
    if (!hasImage) {
        LOG_W("No image");
        //return CalibrationData();
    }

    bool foundRegion = initialize(currentFrame, frameRegion, glintCenter);
    if (!foundRegion) {
        LOG_W("No region found");
        //return CalibrationData;
    }

    vector<Point2f> measurements;
    double totalTime = 0;
    int duration_millis = duration * 1000;

    double t = getTickCount();
    while (totalTime < duration_millis) {

        Point2f gazeVector;

        // Get next frame
        if (!imageSrc.nextGrayFrame(currentFrame)) {
            LOG_D("No more frames");
            break;
        }

        currentFrame = currentFrame(frameRegion);
        MeasureResult result = measureFrame(currentFrame, gazeVector, glintCenter);

        if (result == MEASURE_OK)
            measurements.push_back(gazeVector);

        totalTime = ((double) getTickCount() - t) / getTickFrequency();
    }

    CalibrationData data(pointOnScreen, measurements);
    return data;
}
