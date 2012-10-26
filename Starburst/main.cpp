/*
 * main.cpp
 *
 *	a simple test application for implementing/improving our Starburst algorithm
 *
 */

#include <iostream>
#include <iomanip>

#include "Starburst.hpp"

using namespace cv;
using namespace std;

int main() {
	double totalTime = 0;
	short numOfMeasures = 0;
	int delay = 0;
	Mat frame;

	VideoCapture capture(
			"/Users/fri/Dropbox/gaze/videos/osx/2012-10-17-194103.mov");
	//VideoCapture capture(0);

	// Get the frame rate
	double rate = capture.get(CV_CAP_PROP_FPS);
	// TODO: hack, rate is always 0...
	delay = (rate == 0) ? 1000 / 15 : 1000 / rate;

	if (!capture.isOpened()) {
		// TODO throw an exception here...
		std::cerr << "wtf? no video file?" << endl;
		return 1;
	}

	Starburst starburst;
	starburst.setUp(capture);

	// the main loop
	while (capture.read(frame)) {

		double t = getTickCount();
		starburst.processImage(frame);
		totalTime += ((double) getTickCount() - t) / getTickFrequency();

		numOfMeasures++;
		if (numOfMeasures > 100) {
			cout << "Average processing time in seconds(100 measures): "
					<< setiosflags(ios::fixed) << setprecision(10) << totalTime / numOfMeasures << endl;
			numOfMeasures = 0;
			totalTime = 0;
		}

		// display the image
		imshow("bla", frame);

		// TODO: why doesnt wait() work here???
		if (waitKey(delay) == 27) // ESCAPE
			break;
	}

	capture.release();
	return 0;
}
