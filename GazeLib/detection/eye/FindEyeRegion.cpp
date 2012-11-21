#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FindEyeRegion.hpp"
#include "../../utils/log.hpp"

using namespace std;
using namespace cv;

// Compares the size of two rects
bool compareRect(Rect r1, Rect r2) {
	return ((r1.width * r1.height) < (r2.width * r2.height));
}

FindEyeRegion::FindEyeRegion() {
	if (!eye_region_classifier.load(
			"/home/krigu/Dropbox/gaze/haar/parojosG.xml")) {
		LOG_W("ERROR: Could not load left eye classifier cascade");
	}

	if (!eye_classifier.load(
			"/home/krigu/Dropbox/gaze/haar/haar_left_eye.xml")) {
		LOG_W("ERROR: Could not load left eyes classifier cascade");
	}

}

bool FindEyeRegion::findLeftEye(Mat &image, Rect& eyeRect) {

	vector<Rect> faces;
	eye_region_classifier.detectMultiScale(image, faces, 1.1, 0,
			0 | CV_HAAR_SCALE_IMAGE, Size(400, 100));

	if (faces.size() < 1) {
		LOG_W("No face detected!");
		return false;
	}

	// TODO: What to do with multiple detections?
	Rect eyeRegion = faces.at(0);

	//rectangle(image, eyeRegion, Scalar(0, 255, 255), 20, 8, 0);

	// TODO extract min and maxsize to constants
	vector<Rect> eyes;
	Mat region = image(eyeRegion);
	eye_classifier.detectMultiScale(region, eyes, 1.1, 2,
			0 | CV_HAAR_SCALE_IMAGE, Size(20, 20), Size(200, 200));

//	int i = 1;
//	for (vector<Rect>::const_iterator r = eyes.begin(); r != eyes.end();
//			r++, i++) {
//		Rect rect = cv::Rect(r->x + eyeRegion.x, r->y + eyeRegion.y, r->width,
//				r->height);
//		rectangle(image, rect, Scalar(0, 255, 255), i * i * i, 8, 0);
//	}

	cout << "Size: " << eyes.size() << endl;

	// No eye detected
	if (eyes.size() == 0) {
		return false;
	}
	// One eye detected
	else if (eyes.size() == 1) {
		eyeRect = eyes.at(0);
	}
	// Multiple eyes. Filter false positives out
	// Take one of the two most similar rects
	else {
		sort(eyes.begin(), eyes.end(), compareRect);

		int minDistance = 100000;
		Rect r1;
		Rect r2;

		for (std::vector<int>::size_type i = 0; i != (eyes.size() - 1); i++) {
			int distance = (eyes[i + 1].width * eyes[i + 1].height)
					- (eyes[i].width * eyes[i].height);

			if (distance < minDistance) {
				r1 = eyes[i];
				r2 = eyes[i + 1];
				minDistance = distance;
			}
		}

		// Take left rect
		eyeRect = (r1.x < r2.x) ? r1 : r2;

	}

	// Add offset
	eyeRect.x += eyeRegion.x;
	eyeRect.y += eyeRegion.y;

	return true;
}
