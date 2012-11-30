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
#include "GazeConstants.hpp"


using namespace std;

int main(){

	string path = GazeConstants::inHomeDirectory("Dropbox/gaze/videos/osx/krigu_cut.mov");


	cout << path << endl;

	VideoSource videoSource(path);

	GazeTracker tracker(videoSource);

	tracker.startTracking();


}