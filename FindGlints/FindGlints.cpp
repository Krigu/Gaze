#include <iostream>
#include <math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
// TODO: entfernen
#include "opencv2/highgui/highgui.hpp"

#include "FindGlints.hpp"
#include "utils/log.hpp"
#include "utils/geometry.hpp"
#include "utils/gui.hpp"
#include "GazeConstants.hpp"

using namespace std;
using namespace cv;

bool FindGlints::findGlints(cv::Mat& frame, vector<cv::Point>& glintCenter) {

	Mat img = Mat(frame);

	Mat counturImage = Mat(frame);

	// Threshold image.
	threshold(frame, img, GazeConstants::GLINT_THRESHOLD, 255,
			cv::THRESH_TOZERO);

	Mat pointImage = Mat(img.clone());

	imshow("Thresholded image", img);

	dilate(img, img, Mat::ones(3,3, CV_8U));

	std::vector<std::vector<cv::Point> > contours;

	// Find all countours
	findContours(img, contours, // a vector of contours
			CV_RETR_EXTERNAL, // retrieve the external contours
			CV_CHAIN_APPROX_NONE); // all pixels of each contours

	LOG_D("Countours size: " << contours.size());



	vector<Vec4i> hierarchy;

	// TODO Add Shape analyses to make sure its really a circular form?
	for (int i = 0; i < contours.size(); i++) {

		// TODO:
		// Filter:
		// 1. nach Grösse -> grosse Objekte ausschliessen
		// 2. nach Rundheit -> unrunde objekte ausschliessen
		// 3. nach Position
		// 4. vorheriger Glint Position
		Moments m = moments(contours[i], false);
		int centerX = m.m10 / m.m00;
		int centerY = m.m01 / m.m00;

		cout << "Pixel size: " << m.m00 << endl;
		// TODO min & max size
		if (m.m00 > 0 && m.m00 < 50) { // Grösse

			// Check if object is circular
			if (centerX > 0 && centerY > 0) {
			Point p(centerX, centerY);
			LOG_D(
					"Point: centerX: " << centerX << " " << " centerY: " << centerY);

			cross(pointImage, p, 5);

			glintCenter.push_back(p);

			drawContours(counturImage, contours, i, Scalar(255, 255, 255),
					CV_FILLED, 8, hierarchy, 0, Point());
			}
		}

	}

	filterByNighbor(glintCenter);

	/// Show in a window
	//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", img);

	//namedWindow("Points", CV_WINDOW_AUTOSIZE);
	imshow("Points", pointImage);

	return true;

}

/**
 * Creates a matrix. The element is 1, if the distance between two blobs is in the configurated range
 */
cv::Mat FindGlints::distanceMatrix(vector<cv::Point>& glintCenter) {
	// Create identity matrix

	// Amount of glints
	int n = glintCenter.size();

	// Create identity matrix
	Mat distanceMat = Mat::eye(n, n, CV_8U);

	// Add all points to matrix where a adjacent point is in Glint distance range
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			//for (int j = 0; j < n; j++) {
			int dist = calcPointDistance(glintCenter.at(i), glintCenter.at(j));
			if (dist >= GazeConstants::GLINT_MIN_DISTANCE
					&& dist <= GazeConstants::GLINT_MAX_DISTANCE) {
				distanceMat.at<char>(i, j) = 1;
			}
		}
	}

	return distanceMat;

}

/**
 * Removes all blobs with less than 3 neigbors
 */
void FindGlints::filterByNighbor(vector<cv::Point>& blobs) {

	Mat a = distanceMatrix(blobs);

	LOG_D("DistanceMat out: " << endl << " " << a << endl);
	//cout << "DistanceMat: " << endl << " " << a << endl;

	Mat column_sum;
	reduce(a, column_sum, 1, CV_REDUCE_SUM, CV_32S);

	LOG_D("Sum: " << column_sum);

	// Iterate over all blobs and remove blobs with insufficient nighbors
	vector<cv::Point>::iterator iter;
	int i = 0;
	vector<cv::Point> centerPoints;
	for (iter = blobs.begin(); iter != blobs.end();) {
		if (column_sum.at<char>(i, 0) < GazeConstants::GLINT_COUNT)
			iter = blobs.erase(iter);
		else
			++iter;
		i++;
	}

}

