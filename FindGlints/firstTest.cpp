#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "utils/gui.hpp"
#include "detection/eye/FindEyeRegion.hpp"

#include "GazeConstants.hpp"

using namespace cv;
using namespace std;

int lowThreshold;
int const max_lowThreshold = 255;
char* window_name = "Threshold";

int main() {

	char* source_window = "Source image";

	// Load video capture
	cv::VideoCapture capture(
	GazeConstants::inHomeDirectory("/Dropbox/gaze/videos/k.webm"));
	//		GazeConstants::inHomeDirectory("/Dropbox/gaze/videos/f.webm"));
	// check if capture can be read
	if (!capture.isOpened())
		return 1;

	cout << "Stream is open" << endl;

	FindEyeRegion eye;
//	FindGlints pupil;

	// Get the frame rate
	double rate = 15; //capture.get(CV_CAP_PROP_FPS);

	cout << "Rate: " << rate << endl;

	cv::Mat image; // current video frame
	cv::namedWindow(source_window);

	// Delay between each frame in ms
	// corresponds to video frame rate
	int delay = 1000 / rate;

	if (!capture.read(image))
		return -1;

	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	// Convert to grayscale
	cvtColor(image, image, CV_RGB2GRAY);


	int tries;

	Rect r1;

	bool findFace = false;
	while (!findFace) {
		// read next frame (if available)
		if (!capture.read(image))
			break;

		cvtColor(image, image, CV_RGB2GRAY);

		findFace = eye.findRightEye(image,r1);

		tries++;
	}

	rectangle(image, r1, cvScalar(0, 255, 0), 2, 8, 0);

	imshow("hoo", image);


	while (waitKey(delay) != 32);

	return 0;


//	// k
//	Point intialPoint = Point(140, 85);
//	// Mockup
//	Rect eyeRegion = (intialPoint.x - 50, intialPoint.y - 50, 200, 200);
//
//	// Lurin
//	Point intialPoint = Point(0, 0);
//
//	Mat colorImage;
//// get all frames
//	while (true) {
//		// read next frame (if available)
//		if (!capture.read(colorImage))
//			break;
//
//		if (!colorImage.data)                         // Check for invalid input
//		{
//			cout << "Could not open or find the image" << endl;
//			return -1;
//		}
//
//		cvtColor(colorImage, image, CV_RGB2GRAY);
//
//		rectangle(image, rect, cvScalar(0, 255, 0), 2, 8, 0);
//
//		imshow("image", image); // Show our image inside it.
//
//		vector<cv::Point> centerPoints;
//
//		// TODO optimize
//		Mat img = image();
//		pupil.findGlints(img, centerPoints, intialPoint);
//
//		Point p = Point(intialPoint.x + rect.x, intialPoint.y + rect.y);
//		cross(colorImage, p, 6, Scalar(0, 0, 255));
//
//		imshow(window_name, colorImage); // Show our image inside it.
//
//		// check the key and add some busy waiting
//		int keycode = waitKey(delay);
//
//		if (keycode == 32) // space
//			while (waitKey(delay) != 32)
//				;
//
//		if (waitKey(delay) == 27) // ESCAPE
//			break;
//	}
//
//// Release video
//	capture.release();

	return 0;
}
