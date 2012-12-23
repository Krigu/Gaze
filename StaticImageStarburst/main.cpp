/*
 * main.cpp
 *
 *	a simple test application for implementing/improving our Starburst algorithm
 *
 */

#include <string>
#include <iostream>
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "detection/pupil/Starburst.hpp"
#include "detection/glint/FindGlints.hpp"
#include "detection/eye/FindEyeRegion.hpp"
#include "config/GazeConfig.hpp"
#include "utils/gui.hpp"

using namespace cv;
using namespace std;

int main() {
		Mat im = imread(
				GazeConfig::inHomeDirectory(
						"/Dropbox/gaze/pics/starburst_tests/new_hardware.png"));

		imshow("full_image", im);

		// wait for a
		cout << "Start the detection using any key" << endl;
		waitKey(0);

		Point2f p;
		Point2f pupil_centre;

		// convert to grayscale
		cvtColor(im, im, CV_BGR2GRAY);
		FindGlints glints;
        
        FindEyeRegion eye(glints);
        Rect eyeRegion;
        Mat glint_search = im.clone();
        if(eye.findLeftEye(glint_search, eyeRegion)){
            glint_search = glint_search(eyeRegion);
        } else {
            cout << "No Eyes Found!" << endl;
            return 1;
        }
            
		vector<cv::Point> glint_centers;
		if (glints.findGlints(glint_search, glint_centers, p)) {
			float radius;
			Starburst starburst;
			starburst.processImage(glint_search, glint_centers, p, pupil_centre, radius);

			circle(glint_search, pupil_centre, radius, Scalar(255, 255, 255));
			cross(glint_search, pupil_centre, 5);
		}
		imshow("full_image", glint_search);
		waitKey(0);
}
