#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FindEyeRegion.hpp"
#include "utils/helper.hpp"

using namespace std;
using namespace cv;

FindEyeRegion::FindEyeRegion() {
	if (!eye_region_classifier.load(
			"/Users/fri/Dropbox/gaze/haar/parojosG.xml")) {
		LOG_W("ERROR: Could not load eye-region classifier cascade");
	}

	if (!eye_classifier.load(
			"/Users/fri/Dropbox/gaze/haar/haarcascade_eye.xml")) {
		LOG_W("ERROR: Could not load eyes classifier cascade");
	}

}

bool FindEyeRegion::findRegion(Mat& frame, Rect& rect) {
	vector<Rect> faces;

	eye_region_classifier.detectMultiScale(frame, faces, 1.1, 2,
			0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	if (faces.empty()) {
		LOG_W("No face detected!");
		return false;
	}

	int i = 0;
	// Offset damit mehr ins Bild hineingezoomt wird
	//int offset = 20;
	int offset = 0;

	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end();
			r++, i++) {
		// TODO: implementation correct?
		rect = cv::Rect(r->x + offset, r->y + offset, r->width - offset,
				r->height - offset);
	}

	return true;

}

EyeRegionData FindEyeRegion::findEyes(Mat &image) {
	EyeRegionData* data = new EyeRegionData;
	//Rect* eye_region = new Rect;
	Rect* eye_region = &(cv::Rect(0, 0,
			image.cols, image.rows));

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

	return *data;
}

void FindEyeRegion::removeFalsePositives(vector<Rect> &eyes) {
	// todo: this is rubbish! measure the distance and use this info
	while (eyes.size() > 2) {
		eyes.erase(eyes.begin() + 1); // remove the middle element (probably the nose)
	}
}

//
// EyeRegionData
//
EyeRegionData::EyeRegionData() {
	eye_region = NULL;
	left_eye = NULL;
	right_eye = NULL;
}

EyeRegionData::~EyeRegionData() {
	/*if (eye_region != NULL)
		delete eye_region;
	if (left_eye != NULL)
		delete left_eye;
	if (right_eye != NULL)
		delete right_eye;*/
}

bool EyeRegionData::isDataSet() {
	return eye_region != NULL && left_eye != NULL && right_eye != NULL;
}

Rect EyeRegionData::getRegion(Rect* region) {
	if (region == NULL)
		throw "Was wosch?"; // TODO
	return *region;
}

Rect EyeRegionData::getLeftEye() {
	return getRegion(left_eye);
}

Rect EyeRegionData::getRightEye() {
	return getRegion(right_eye);
}

Rect EyeRegionData::getEyeRegion(){
	return getRegion(eye_region);
}

void EyeRegionData::setLeftEye(Rect *left_eye) {
	this->left_eye = left_eye;
}
void EyeRegionData::setRightEye(Rect *right_eye) {
	this->right_eye = right_eye;
}
void EyeRegionData::setEyeRegion(Rect *eyes) {
	this->eye_region = eyes;
}
