/*
 * StartGazeTracker.cpp
 *
 *  Created on: Nov 24, 2012
 *      Author: krigu
 */

#include <iostream>

#include "detection/GazeTracker.hpp"
#include "detection/pupil/Starburst.hpp"
#include "video/VideoSource.hpp"
#include "video/LiveSource.hpp"
#include "config/GazeConfig.hpp"
#include "TCallback.hpp"



using namespace std;

int main(){

	string path = GazeConfig::inHomeDirectory("Dropbox/gaze/videos/k.webm");



	cout << path << endl;

	VideoSource videoSource(path);
    
    LiveSource liveSource;
    
    TrackerCallback* callback;
            callback = new TCallback();
    
	GazeTracker tracker(liveSource, callback);
    
    

	tracker.initializeCalibration(); 
    
    
    tracker.track();
    cout << "----------------- " << endl;
    cout << "----------------- " << endl;
    cout << "Done initializing " << endl;
    cout << "----------------- " << endl;
    cout << "----------------- " << endl;
    delete callback;

}
