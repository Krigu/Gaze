#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "utils/gui.hpp"
#include "utils/geometry.hpp"
#include "utils/log.hpp"
#include "detection/eye/FindEyeRegion.hpp"
#include "detection/glint/FindGlints.hpp"

#include "GazeConstants.hpp"

using namespace cv;
using namespace std;

int lowThreshold;
int const max_lowThreshold = 255;



int main() {

	// Load video capture
	cv::VideoCapture capture(
			GazeConstants::inHomeDirectory("/Dropbox/gaze/videos/k.webm"));
	//		GazeConstants::inHomeDirectory("/Dropbox/gaze/videos/f.webm"));
	// check if capture can be read
	if (!capture.isOpened())
		return 1;

	FindEyeRegion eye;
	// Get the frame rate
	double rate = 15; //capture.get(CV_CAP_PROP_FPS);

	cout << "Rate: " << rate << endl;

	cv::Mat image; // current video frame

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
	cvtColor(image, image, CV_BGR2GRAY);

	Rect eyeRegion;

	bool findFace = false;
	int tries;
	while (!findFace) {
		// read next frame (if available)
		if (!capture.read(image))
			break;

		cvtColor(image, image, CV_BGR2GRAY);

		findFace = eye.findLeftEye(image, eyeRegion);

		tries++;
	}

	rectangle(image, eyeRegion, cvScalar(0, 255, 0), 2, 8, 0);

	Point lastMeasurement = calcRectBarycenter(eyeRegion);
//
//	cross(image, lastMeasurement, 6);
//	imshow("hoo", image);
//
	//while (waitKey(delay) != 32)
//		;

	FindGlints pupil;
	Mat colorImage;
// get all frames
	while (true) {
		// read next frame (if available)
		if (!capture.read(colorImage))
			break;

		if (!colorImage.data)                         // Check for invalid input
		{
			cout << "Could not open or find the image" << endl;
			return -1;
		}

		cvtColor(colorImage, image, CV_RGB2GRAY);

		vector<cv::Point> centerPoints;

		rectangle(colorImage, eyeRegion, Scalar(255, 0, 0), 5);

		// TODO optimize
		Mat img = image(eyeRegion).clone();
		if (pupil.findGlints(img, centerPoints, lastMeasurement)) {
//
			Point p = Point(lastMeasurement.x + eyeRegion.x,
					lastMeasurement.y + eyeRegion.y);
			cross(colorImage, p, 6, Scalar(0, 0, 255));
			LOG_D(
					"Last measurement x: " << lastMeasurement.x << " y: " << lastMeasurement.y);

			eyeRegion = Rect(p.x - 100, p.y - 50, 200, 100);
		}

		imshow("image", colorImage); // Show our image inside it.

		// check the key and add some busy waiting
		int keycode = waitKey(delay);

		if (keycode == 32) // space
			while (waitKey(delay) != 32)
				;

		if (waitKey(delay) == 27) // ESCAPE
			break;
	}

// Release video
	capture.release();

	return 0;
}
