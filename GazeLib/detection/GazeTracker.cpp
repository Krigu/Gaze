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
0), eyeFinder(FindEyeRegion(glintFinder)) {
}

void GazeTracker::getNextFrame(Mat& frame) {
    bool hasImage = imageSrc.nextGrayFrame(frame);
    if (!hasImage) {
        LOG_W("No image");
        throw NoImageSourceException();
    }
}

void GazeTracker::initializeCalibration() {
    Mat frame;
    // Copy for displaying image
    Mat fullFrame;
    Point2f glintCenter;
    vector<cv::Point> glints;

    bool continueSearching = true;
    do {
        // Endless search for eye region
        continueSearching = findEyeRegion(frame, glintCenter, true);

    } while (!glintFinder.findGlints(frame, glints, glintCenter) && continueSearching);

}

bool GazeTracker::findEyeRegion(Mat & frame,
        Point2f& frameCenter, bool calibrationMode) {

    bool foundEye = false;
    short tries = 0;
    while (!foundEye) {
        getNextFrame(frame);

        foundEye = eyeFinder.findEye(frame, frameRegion);
        tries++;

        if (calibrationMode) {
            //imshow("frame", frame);
            if (tracker_callback != NULL) {
                Mat fullFrame = frame.clone();
                rectangle(fullFrame, frameRegion, Scalar(255, 255, 255), 3);
                if(!tracker_callback->imageProcessed(fullFrame))
                    return false; // abort processing
            }
        }

        // No eye region found
        if (!calibrationMode && tries > GazeConfig::HAAR_FINDREGION_MAX_TRIES)
            throw EyeRegionNotFoundException();
        
    }

    frameCenter = calcRectBarycenter(frameRegion);
    frame = frame(frameRegion);
    
    // continue the processing
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

    frameRegion.x = x;
    frameRegion.y = y;

}

GazeTracker::~GazeTracker() {
    // TODO Auto-generated destructor stub
}

void GazeTracker::track(unsigned int duration) {

    Mat currentFrame;
    Point2f glintCenter;
    Point2f gazeVector;

    double ticks = 0;
    double maxTicks = 0;
    if (duration > 0)
        maxTicks = duration * getTickFrequency();   

    // Find eye 
    findEyeRegion(currentFrame, glintCenter);

    int noGlints = 0;

    double startTick = getTickCount();

    bool continueTracking = true;
    
    // main loop
    do {
        // Get next frame
        getNextFrame(currentFrame);
#if __DEBUG_HAAR == 1
        Mat f1 = currentFrame.clone();

        rectangle(f1, frameRegion, Scalar(255, 255, 255), 3);
        imshow("Frame from source", f1);

#endif     

        if ((frameRegion.x + frameRegion.width) > currentFrame.cols)
            frameRegion.x = currentFrame.cols - frameRegion.width;

        if ((frameRegion.y + frameRegion.height) > currentFrame.rows)
            frameRegion.y = currentFrame.rows - frameRegion.height;

        currentFrame = currentFrame(frameRegion);

        MeasureResult result = measureFrame(currentFrame, gazeVector, glintCenter);
        
        LOG_D("Measure result: " << result);

        Point2f smoothed_gaze_vec;
        switch (result) {
            case MEASURE_OK:

                this->smoothSignal(gazeVector, smoothed_gaze_vec,
                        this->last_gaze_vectors, framenumber);

                LOG_D("Current GazeVector: " << gazeVector);

                ++framenumber;
                break;

            case FINDGLINT_FAILED:
                noGlints++;
                if (noGlints > 20) { //TODO should be a constant
                    LOG_W("no glints found. need to reinitialize");
                    findEyeRegion(currentFrame, glintCenter);

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
            continueTracking = tracker_callback->imageProcessed(currentFrame, result, smoothed_gaze_vec);

        ticks = getTickCount() - startTick;
        
    } while (continueTracking && (maxTicks == 0 || ticks < maxTicks));
}

MeasureResult GazeTracker::measureFrame(Mat &frame, Point2f &gazeVector, Point2f glintCenter) {
    vector<cv::Point> glints;
    float radius;
    Point2f pupilCenter;

    if (glintFinder.findGlints(frame, glints, glintCenter)) {

        
        
        if (!starburst.findPupil(frame, glints, glintCenter,
                pupilCenter, radius)) {
            cross(frame, glintCenter, 10);
            return FINDPUPIL_FAILED;
        } else {
            cross(frame, glintCenter, 10);
        }

        adjustRect(glintCenter, frameRegion);

    } else {
        return FINDGLINT_FAILED;
    }

    circle(frame, pupilCenter, radius, Scalar(255, 255, 255));
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
