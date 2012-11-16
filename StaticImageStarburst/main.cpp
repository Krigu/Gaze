/*
 * main.cpp
 *
 *	a simple test application for implementing/improving our Starburst algorithm
 *
 */

#include <string>
#include <iostream>

#include "detection/Starburst.hpp"
#include "GazeConstants.hpp"
#include "utils/gui.hpp"

using namespace cv;
using namespace std;

struct test_pix {
	string *image;
	int x_koord;
	int y_koord;
};
int main() {
	// setup up some test data using the starting position and an inputimage
	string im1 = "frame1.tif";
	string im2 = "test.tif";
	test_pix pics[2];
	pics[0].image = &im1;
	pics[0].x_koord = 350;
	pics[0].y_koord = 566;
	pics[1].image = &im2;
	pics[1].x_koord = 410;
	pics[1].y_koord = 426;

	for (char i = 0; i < 2; i++) {
		// read the current image and display it
		Mat im = imread(
				GazeConstants::inHomeDirectory(
						"/Dropbox/gaze/pics/starburst_tests/" + *(pics[i].image)));

		//short size = 5;
		 Point startpoint(pics[i].x_koord, pics[i].y_koord);
		//Scalar color(0,0,255);
		//cross(im, startpoint, size, color);

		imshow("full_image", im);

		// wait for a
		cout << "Start the detection using any key" << endl;
		waitKey(0);

		Point2f p = startpoint;

		Starburst starburst;
		starburst.setLastCenter(p);
		starburst.processImage(im);

		imshow("full_image", im);
		waitKey(0);

	}
}
