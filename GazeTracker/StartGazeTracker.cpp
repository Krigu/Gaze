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
#include "GazeConstants.hpp"


using namespace std;
using namespace cv;

int main(){

//	string path = GazeConstants::inHomeDirectory("Dropbox/gaze/videos/osx/krigu_cut.mov");
//
//
//	cout << path << endl;
//
//	LiveSource source;
//
//	GazeTracker tracker(source);
//
//	tracker.startTracking();

	VideoCapture cap(0);

	if(!cap.isOpened())
		return 0;


	int numOfMeasures = 0;

	Mat frame;
	namedWindow("test", 1);
	double totalTime = 0;

	while(true){
		double t = getTickCount();

		cap >> frame;
		imshow("test", frame);

		if(waitKey(30) == 27) // ESCAPE
			break;

		totalTime += ((double) getTickCount() - t) / getTickFrequency();
		numOfMeasures++;
		if (numOfMeasures >= 1000) {
			cout << "Average processing time in seconds(" << numOfMeasures
					<< " measures): "  << totalTime / numOfMeasures << endl;
			numOfMeasures = 0;
			totalTime = 0;
		}
	}


}
