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

Rect takeLeftEye(const Rect r1, const Rect r2) {
	return (r1.x < r2.x) ? r1 : r2;
}

Rect takeRightEye(const Rect r1, const Rect r2) {
	return (r1.x > r2.x) ? r1 : r2;
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

bool FindEyeRegion::findEye(Mat &image, Rect& eyeRect, eyeCompareFunction& compareFunc) {
	vector<Rect> faces;
	eye_region_classifier.detectMultiScale(image, faces, 1.1, 0,
			0 | CV_HAAR_SCALE_IMAGE, Size(400, 100));

	if (faces.size() < 1) {
		LOG_W("No face detected!");
		return false;
	}

	// TODO: What to do with multiple detections?
	Rect eyeRegion = faces.at(0);

	// TODO extract min and maxsize to constants
	vector<Rect> eyes;
	Mat region = image(eyeRegion);
	eye_classifier.detectMultiScale(region, eyes, 1.1, 2,
			0 | CV_HAAR_SCALE_IMAGE, Size(20, 20), Size(200, 200));

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

		int minDistance = 1000;
		// TODO: Maybe use pointers?
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
		eyeRect = compareFunc(r1, r2);
	}

	// Add offset
	eyeRect.x += eyeRegion.x;
	eyeRect.y += eyeRegion.y;

	return true;
}

bool FindEyeRegion::findRightEye(Mat &image, Rect& eyeRect) {
	// TODO: in konstruktor?
	eyeCompareFunction compFunc = &takeRightEye;
	return findEye(image, eyeRect, compFunc);
}

bool FindEyeRegion::findLeftEye(Mat &image, Rect& eyeRect) {
	// TODO: in konstruktor?
	eyeCompareFunction compFunc = &takeLeftEye;
	return findEye(image, eyeRect, compFunc);
}
