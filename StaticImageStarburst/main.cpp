/*
 * main.cpp
 *
 *	a simple test application for implementing/improving our Starburst algorithm
 *
 */

#include "detection/Starburst.hpp"
#include "utils/helper.hpp"

using namespace cv;
using namespace std;

int main() {

	Starburst starburst;
	cout << "Test" << endl;

	Mat im = imread("/home/krigu/Dropbox/gaze/pics/face_with_ir_grayscale.png");
	imshow("bla",im);

	// wait for a
	waitKey(0);
}
