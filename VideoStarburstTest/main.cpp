/*
 * main.cpp
 *
 *	a simple test application for implementing/improving our Starburst algorithm
 *
 */

#include <iostream>
#include <iomanip>

#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "config/GazeConfig.hpp"
#include "detection/pupil/Starburst.hpp"
#include "detection/glint/FindGlints.hpp"
#include "detection/eye/FindEyeRegion.hpp"
#include "utils/log.hpp"
#include "utils/gui.hpp"
#include "utils/geometry.hpp"

using namespace cv;
using namespace std;

int main() {
	double totalTime = 0;
	short numOfMeasures = 0;
	int delay = 0;
	Mat frame;

	VideoCapture capture(
			GazeConfig::inHomeDirectory(
					"/Dropbox/gaze/videos/osx/krigu.mov"));
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

	// first, lets find the eye region
	Rect eyeRegion;
	FindEyeRegion eye;

	bool findFace = false;
	int tries;
	while (!findFace) {
		// read next frame (if available)
		if (!capture.read(frame))
			break;

		cvtColor(frame, frame, CV_BGR2GRAY);

		findFace = eye.findLeftEye(frame, eyeRegion);

		tries++;
	}

	FindGlints glints;
	Starburst starburst;

	long i = 0;
	bool waiting = false;

	Point2f start = calcRectBarycenter(eyeRegion);

	// the main loop
	while (capture.read(frame)) {

		double t = getTickCount();

		cvtColor(frame, frame, CV_BGR2GRAY);

		Mat glint_search = frame(eyeRegion).clone();
		vector<cv::Point> glint_centers;
		float radius;

		bool found = glints.findGlints(glint_search, glint_centers, start);
        std::cout << "Glint found:" << found << std::endl;
		if (found) {
			// get the absolute coordinates
			start = Point(start.x + eyeRegion.x, start.y + eyeRegion.y);

			//TODO: worst fix ever,
			for (vector<Point>::iterator it = glint_centers.begin();
					it != glint_centers.end(); ++it) {

				// the glint_centers are relative to the search region...
				it->x = it->x + eyeRegion.x;
				it->y = it->y + eyeRegion.y;

			}

			starburst.processImage(frame, glint_centers, start, start, radius);
			totalTime += ((double) getTickCount() - t) / getTickFrequency();
			numOfMeasures++;

			// follow the eye with our search region
			eyeRegion = Rect(start.x - 100, start.y - 50, 200, 100);

			circle(frame, start, radius, Scalar(255, 255, 255));
			cross(frame, start, 5);
			rectangle(frame, eyeRegion, Scalar(255, 255, 255));
		}

		if (numOfMeasures > 100) {
			cout << "Average processing time in seconds(" << numOfMeasures
					<< " measures): " << setiosflags(ios::fixed)
					<< setprecision(10) << totalTime / numOfMeasures << endl;
			numOfMeasures = 0;
			totalTime = 0;
		}

		i++;

		// display the image
		imshow("bla", frame);
		//waitKey(0);

		// check the key and add some busy waiting
		int keycode = waitKey(delay);

		if (keycode == 32) // space
			waiting = true;
		else if (keycode == 27) // ESCAPE
			break;

		// busy waiting
		while (waiting) {
			keycode = waitKey(delay);
			if (keycode == 32) {
				waiting = false;
				break;
			} else if (keycode == 106) // "j"
				break; // process ONE frame
		}
	}

	capture.release();
	return 0;
}
