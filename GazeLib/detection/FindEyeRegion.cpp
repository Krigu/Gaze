#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FindEyeRegion.hpp"
#include "../utils/log.hpp"

using namespace std;
using namespace cv;

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

bool FindEyeRegion::findLeftEye(Mat &image, Point& centerOfEye) {

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
			0 | CV_HAAR_SCALE_IMAGE, Size(30, 30), Size(60, 60));

	int i = 1;
	for (vector<Rect>::const_iterator r = eyes.begin(); r != eyes.end();
			r++, i++) {
		Rect rect = cv::Rect(r->x + eyeRegion.x, r->y + eyeRegion.y, r->width,
				r->height);
		rectangle(image, rect, Scalar(0, 255, 255), i * i * i, 8, 0);
	}

	// No eye detected
	if (eyes.size() == 0) {
		return false;
	}
	// One eye detected
	else if (eyes.size() == 1) {
		Rect r = eyes.at(0);
		centerOfEye = Point(r.x + eyeRegion.x, r.y + eyeRegion.y);
		return true;
	}
	// Multiple eyes. Filter false positives out
	else {
		Rect r = eyes.at(0);
		centerOfEye = Point(r.x + eyeRegion.x, r.y + eyeRegion.y);
		return true;
//		map<Rect&, int> areas;
//		for (vector<Rect>::const_iterator r = eyes.begin(); r != eyes.end();
//				r++) {
//			areas.insert(pair<Rect&, int>(*r, r->width * r->height));
//		}

	}

//	Rect* eye_region = &(cv::Rect(0, 0,
//			image.cols, image.rows));

	/*
	 // find some eyes
	 //bool found = findRegion(image, *eye_region);

	 //if (found) {
	 data->setEyeRegion(eye_region);

	 // now search for eyes within the region
	 vector<Rect> eyes;
	 eye_classifier.detectMultiScale(image(*eye_region), eyes, 1.1, 2,
	 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	 imshow("Eye_Region", image(*eye_region));

	 // order the eye by their horizontal position
	 // the left eye (from the viewers position) is the first element
	 long last_left_position = LONG_LONG_MAX;
	 vector<Rect> ordered_eyes; // Left, then right
	 for (vector<Rect>::const_iterator r = eyes.begin(); r != eyes.end();
	 r++) {
	 Rect eye = cv::Rect(eye_region->x + r->x, eye_region->y + r->y,
	 r->width, r->height);

	 if (r->x < last_left_position) {
	 ordered_eyes.insert(ordered_eyes.begin(), eye);
	 last_left_position = r->x;
	 } else {
	 ordered_eyes.insert(ordered_eyes.end(), eye);
	 }

	 imshow("Eye", image(eye));
	 }

	 if (ordered_eyes.size() >= 2) {
	 if (ordered_eyes.size() > 2)
	 removeFalsePositives(ordered_eyes);
	 data->setLeftEye(&(ordered_eyes.at(0)));
	 data->setRightEye(&(ordered_eyes.at(1)));
	 } else if (ordered_eyes.size() < 2) {
	 LOG_W("We only found " << ordered_eyes.size() << " eyes!");
	 }
	 //}
	 */

	return true;
}
