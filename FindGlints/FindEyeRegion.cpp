#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "FindEyeRegion.hpp"
#include "GazeConstants.hpp"

using namespace std;
using namespace cv;

bool FindEyeRegion::findRegion(Mat& frame, Rect& rect) {
	vector<Rect> faces;

	if (!cascade_eye.load(GazeConstants::inHomeDirectory("/Dropbox/gaze/haar/parojosG.xml"))) {
		std::cerr << "ERROR: Could not load classifier cascade" << std::endl;
		return false;
	}
	cascade_eye.detectMultiScale(frame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE,
			Size(30, 30));

	if (faces.empty()){
		cout << "No face detected!" << endl;
		return false;
	}

	int i = 0;
	// Offset damit mehr ins Bild hineingezoomt wird
	int offset = 20;

	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end();
			r++, i++) {
		// TODO: implementation correct?
		rect = cv::Rect(r->x + offset, r->y + offset, r->width - offset,
				r->height - offset);



	}

	return true;

}

