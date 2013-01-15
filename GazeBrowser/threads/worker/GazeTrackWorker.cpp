#include <iostream>

#include <opencv2/core/core.hpp>

#include "GazeTrackWorker.hpp"
#include "calibration/Calibration.hpp"
#include "detection/GazeTracker.hpp"
#include "video/LiveSource.hpp"
#include "exception/GazeExceptions.hpp"
#include "../Sleeper.hpp"
#include "../../ui/UIConstants.hpp"

using namespace std;

GazeTrackWorker::GazeTrackWorker(int width, int height, ImageSource *camera, QMutex *cameraLock)
: width(width), height(height), camera(camera), cameraLock(cameraLock), running(false) {

    tracker = new GazeTracker(*camera, this);
    calibration = NULL;

}

GazeTrackWorker::~GazeTrackWorker() {
    delete tracker;
    if (calibration != NULL)
        delete calibration;
}

void GazeTrackWorker::startCalibration() {
    if (!cameraLock->tryLock()) {
        emit error(tr("Cannot calibrate, is the camera in use?"));
        return;
    }

    running = true;
    tracking = false;

    // maybe a recalibration?
    delete calibration;
    calibration = NULL;

    try {
        bool calibrated = false;
        while (!calibrated && running) {
            calibration = new Calibration;
            calibrated = calibrate();
            if (!calibrated){
                delete calibration;
                calibration = NULL;
            }
        }

        cameraLock->unlock();

        if (calibrated)
            emit(calibrationFinished());
        else if (!running)
            emit hasStopped(nextStateAfterStop);

    } catch (GazeException& e) {
        cameraLock->unlock();
        emit error(e.what());
    }
}

bool GazeTrackWorker::isCalibrated() {
    return calibration != NULL;
}

void GazeTrackWorker::startTracking() {

    if (!isCalibrated()) {
        emit error(tr("Please Calibrate the System before Tracking!"));
        return;
    }

    if (!cameraLock->tryLock()) {
        emit error(tr("Cannot track, is the camera in use?"));
        return;
    }

    // Wait with begining of tracking until bookmark page is loaded
    Sleeper::msleep(4000);

    running = true;
    tracking = true;

    while (running) {
        try {

            tracker->track();

        } catch (EyeRegionNotFoundException &e) {
            emit info(tr("No Eye detected - Please hold your head still!"));
            tracker->initializeCalibration();
        } catch (GazeException &e) {
            emit error(e.what());
            break; // leave 
        }
    }

    cameraLock->unlock();

    // notify the threadmanager about the stop
    emit hasStopped(nextStateAfterStop);
    
}

bool GazeTrackWorker::calibrate() {
    int x_offset = 40;
    int y_offset = 40;

    int height = this->height - 2 * y_offset;
    int width = this->width - 2 * x_offset;

    tracker->initializeCalibration();

    int retryNum = 0;
    
    for (unsigned short i = 0; i < 3 && running; i++) {
        for (unsigned short j = 0; j < 3 && running; j++) {
            int point_x = width / 2 * j + x_offset;
            int point_y = height / 2 * i + y_offset;

            cout << "Calibration Point: " << point_x << "/" << point_y << endl;

            // Move the calibration point to the position inside the webview
            QString code = QString("calibrationCircle.move(%1,%2);")
                    .arg(point_x).arg(point_y);

            emit jsCommand(code);
            Sleeper::msleep(3000);

            // calculate the absolute screen point (webview + Menubar)
            Point2f p(point_x, point_y + MENUBAR_HEIGHT);
            measurements.clear();
            
            try {
                tracker->track(5);
            } catch(GazeException &e){
                retryNum++;
                if(retryNum > 5)
                    throw e;
                emit info("Could not find Eye. Please hold on.");
                tracker->initializeCalibration();
                j--;
                continue;
            }
            
            
            CalibrationData data(p, measurements);
            calibration->addCalibrationData(data);
            cout << "Point: " << p << "Vector: " << data.getMeasuredMedianVector() << endl;
        }
    }

    if (running)
        return calibration->calibrate(100, 3);
    else
        return false;
}

bool GazeTrackWorker::imageProcessed(Mat& resultImage) {
//#ifdef __APPLE__
//    Sleeper::msleep(33);
//#endif
    emit cvImage(new Mat(resultImage));
    return running;
}

bool GazeTrackWorker::imageProcessed(Mat& resultImage, MeasureResult &result, Point2f &gazeVector) {
//#ifdef __APPLE__
//    Sleeper::msleep(33);
//#endif
    if (result == MEASURE_OK) {
        if (tracking) {
            emit estimatedPoint(calibration->calcCoordinates(gazeVector));
        } else {
            measurements.push_back(gazeVector);
        }
    }
    if (!tracking)
        emit cvImage(new Mat(resultImage));

    std::cout << "Measured: " << result << " " << gazeVector << std::endl;

    return running;
}

void GazeTrackWorker::stop(PROGRAM_STATES nextState) {
    this->nextStateAfterStop = nextState;
    this->running = false;
}