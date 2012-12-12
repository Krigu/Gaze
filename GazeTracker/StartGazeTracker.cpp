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



using namespace std;

int main(){

	string path = GazeConfig::inHomeDirectory("Dropbox/gaze/videos/k.webm");



	cout << path << endl;

	VideoSource videoSource(path);
    
    LiveSource liveSource;

	GazeTracker tracker(liveSource);

	tracker.initializeCalibration();    
    tracker.track();


}
