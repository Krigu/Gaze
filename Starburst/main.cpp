/*
 * main.cpp
 *
 *	a simple test application for implementing/improving our Starburst algorithm
 *
 */

#include <iostream>
#include <iomanip>

#include "Starburst.hpp"
#include "utils/helper.hpp"

using namespace cv;
using namespace std;

int main() {
	double totalTime = 0;
	short numOfMeasures = 0;
	int delay = 0;
	Mat frame;

	VideoCapture capture(
			"/Users/fri/Dropbox/gaze/videos/osx/fri.mov");
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

	long i = 0;
	bool waiting = false;

	// the main loop
	while (capture.read(frame)) {

		double t = getTickCount();
		try{
			starburst.processImage(frame);
		} catch(cv::Exception &e) {
			cerr << i << "\t" << e.what();
			break;
		}

		totalTime += ((double) getTickCount() - t) / getTickFrequency();

		numOfMeasures++;
		if (numOfMeasures > 100) {
			cout << "Average processing time in seconds(100 measures): "
					<< setiosflags(ios::fixed) << setprecision(10) << totalTime / numOfMeasures << endl;
			numOfMeasures = 0;
			totalTime = 0;
		}

		i++;

		// display the image
		imshow("bla", frame);

		// check the key and add some busy waiting
		int keycode = waitKey(delay);

		if(keycode == 32) // space
			waiting = true;
		else if(keycode == 27) // ESCAPE
			break;

		// busy waiting
		while(waiting){
			keycode = waitKey(delay);
			if(keycode == 32 ){
				waiting = false;
				break;
			}
			else if (keycode == 106) // "j"
				break; // process ONE frame
		}
	}

	capture.release();
	return 0;
}
