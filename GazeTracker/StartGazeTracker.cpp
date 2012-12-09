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
<<<<<<< HEAD
#include "video/LiveSource.hpp"
#include "config/GazeConstants.hpp"
=======
#include "config/GazeConfig.hpp"
>>>>>>> 65a94e832133cc233f02475eec2b7403880aa4d2


using namespace std;

int main(){

	string path = GazeConstants::inHomeDirectory("Dropbox/gaze/videos/k2.webm");



	cout << path << endl;

	VideoSource videoSource(path);
    
    LiveSource liveSource;

	GazeTracker tracker(videoSource);

	tracker.initializeCalibration();    
    tracker.track();


}
