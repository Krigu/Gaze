/*
 * main.cpp
 *
 *	a simple test application for implementing/improving our Starburst algorithm
 *
 */

#include "Starburst.hpp"
#include "utils/helper.hpp"

using namespace cv;
using namespace std;

int main() {

	Starburst starburst;
	cout << "Test" << endl;

	Mat im = imread("/Users/fri/Desktop/frame1.tif");
	imshow("bla",im);

	// wait for a
	waitKey(0);
}
