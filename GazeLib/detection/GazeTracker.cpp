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

void GazeTracker::getNextFrame(Mat& frame) {
    bool hasImage = imageSrc.nextGrayFrame(frame);
    if (!hasImage) {
        LOG_W("No image");
        throw NoImageSourceException();
    }
}

void GazeTracker::initializeCalibration() {
    // TODO possiblilty to exit method
    Mat frame;
    // Copy for displaying image
    Mat fullFrame;
    Rect eyeRegion;
    Point2f glintCenter;
    vector<cv::Point> glints;

    do {
        getNextFrame(frame);
        // Endless search for eye region
        findEyeRegion(frame, eyeRegion, glintCenter, true);
        
    } while (!glintFinder.findGlints(frame, glints, glintCenter));

    // TODO more output
}

void GazeTracker::findEyeRegion(Mat & frame, Rect& frameRegion,
        Point2f& frameCenter, bool calibrationMode) {

    bool foundEye = false;
    short tries = 0;
    while (!foundEye) {
        getNextFrame(frame);

        usleep(20);

        foundEye = eyeFinder.findLeftEye(frame, frameRegion);
        tries++;

        // TODO init one window for whole calibration process
        // TODO init one window for whole calibration process
        if (calibrationMode) {
            //imshow("frame", frame);
            if (tracker_callback != NULL) {
                Mat fullFrame = frame.clone();
                rectangle(fullFrame, frameRegion, Scalar(255, 255, 255), 3);
                tracker_callback->imageProcessed(fullFrame);
            }
        }

        // TODO: Global var for waitkey?
        //        waitKey(20);
        //        QThread::msleep(20);

        // No eye region found
        if (!calibrationMode && tries > GazeConfig::HAAR_FINDREGION_MAX_TRIES)
            throw EyeRegionNotFoundException();
    }

    frameCenter = calcRectBarycenter(frameRegion);
    frame = frame(frameRegion);
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

void GazeTracker::track() {

    Mat currentFrame;
    Point2f glintCenter;
    Point2f gazeVector;


    // Find eye 
    getNextFrame(currentFrame);
    findEyeRegion(currentFrame, frameRegion, glintCenter);

    int noGlints = 0;

    // main loop
    do {
        // Get next frame
        getNextFrame(currentFrame);
#if __DEBUG_HAAR == 1
        Mat f1 = currentFrame.clone();

        rectangle(f1, frameRegion, Scalar(255, 255, 255), 3);
        imshow("Frame from source", f1);
#endif      
        currentFrame = currentFrame(frameRegion);

        MeasureResult result = measureFrame(currentFrame, gazeVector, glintCenter);

        Point2f smoothed_gaze_vec;
        switch (result) {
            case MEASURE_OK:

                this->smoothSignal(gazeVector, smoothed_gaze_vec,
                        this->last_gaze_vectors, framenumber);
                //c.printPoint(smoothed_gace_vec);

                LOG_D("Current GazeVector: " << gazeVector);

                ++framenumber;
                break;

            case FINDGLINT_FAILED:
                noGlints++;
                if (noGlints > 5) {
                    LOG_W("no glints found. need to reinitialize");
                    getNextFrame(currentFrame);
                    findEyeRegion(currentFrame, frameRegion, glintCenter);

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

#if __DEBUG_STEP_BY_STEP == 1    
        int keycode = waitKey(0);
#else
        int keycode = waitKey(50);
#endif        
        if (keycode == 32) // space
            while (waitKey(100) != 32)
                ;
    } while (true);
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


    // now calculate the gaze vector
    gazeVector.x = glintCenter.x - pupilCenter.x;
    gazeVector.y = glintCenter.y - pupilCenter.y;

    return MEASURE_OK;
}

void GazeTracker::smoothSignal(Point2f &measured, Point2f &smoothed, Point2f data[],
        unsigned int framenumber) {
    if (framenumber < GazeConfig::NUM_OF_SMOOTHING_FRAMES) {
        // nothing to smooth here
        smoothed.x = measured.x;
        smoothed.y = measured.y;
    } else {
        smoothed.x = 0;
        smoothed.y = 0;
        for (unsigned short i = 0; i < GazeConfig::NUM_OF_SMOOTHING_FRAMES;
                ++i) {
            smoothed.x += data[i].x;
            smoothed.y += data[i].y;
        }
        smoothed.x /= GazeConfig::NUM_OF_SMOOTHING_FRAMES;
        smoothed.y /= GazeConfig::NUM_OF_SMOOTHING_FRAMES;
    }
    data[framenumber % GazeConfig::NUM_OF_SMOOTHING_FRAMES] = measured;
}

CalibrationData GazeTracker::measurePoint(Point2f pointOnScreen,
        unsigned int duration) {

    Mat currentFrame;
    Point2f glintCenter;

    getNextFrame(currentFrame);
    findEyeRegion(currentFrame, frameRegion, glintCenter);

    vector<Point2f> measurements;
    double ticks = 0;
    double maxTicks = duration * getTickFrequency();

    double startTick = getTickCount();

    while (ticks < maxTicks) {

        Point2f gazeVector;

        getNextFrame(currentFrame);
        currentFrame = currentFrame(frameRegion);
        MeasureResult result = measureFrame(currentFrame, gazeVector, glintCenter);

        // notify our callback about the processed frames...
        if (this->tracker_callback != NULL)
            tracker_callback->imageProcessed(currentFrame);

        if (result == MEASURE_OK)
            measurements.push_back(gazeVector);

        ticks = getTickCount() - startTick;
    }

    CalibrationData data(pointOnScreen, measurements);
    return data;
}