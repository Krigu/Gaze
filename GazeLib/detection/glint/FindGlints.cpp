#include <iostream>
#include <math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
// TODO: entfernen
#include "opencv2/highgui/highgui.hpp"

#include "FindGlints.hpp"
#include "GlintCluster.hpp"
#include "Blobs.hpp"
#include "../../utils/log.hpp"
#include "../../utils/geometry.hpp"
#include "../../utils/gui.hpp"
#include "../../GazeConstants.hpp"

using namespace std;
using namespace cv;

bool FindGlints::findGlints(cv::Mat& frame, vector<cv::Point>& glintCenter,
		cv::Point& lastMeasurement) {

	Mat img = Mat(frame);

	Mat counturImage = Mat(frame);

	// Threshold image.
	threshold(frame, img, GazeConstants::GLINT_THRESHOLD, 255,
			cv::THRESH_TOZERO);

	Mat pointImage = Mat(img.clone());

	//imshow("Thresholded image", img);

	// Dilate the blobs. Sometimes a glint is just one small pixel
	dilate(img, img, Mat::ones(2, 2, CV_8U));

	std::vector<std::vector<cv::Point> > contours;

	// Find all countours
	findContours(img, contours, // a vector of contours
			CV_RETR_EXTERNAL, // retrieve the external contours
			CV_CHAIN_APPROX_NONE); // all pixels of each contours

	LOG_D("Countours size: " << contours.size());

	vector<Vec4i> hierarchy;

	Blobs blobs = Blobs(contours);
	blobs.removeInvalidSize();
	blobs.removeInvalidShape();

	blobs.blobCenters(glintCenter);

	LOG_D("Blobs size: " << glintCenter);

	// Find all clusters
	vector<GlintCluster> clusters;
	findClusters(glintCenter, clusters, lastMeasurement);

	// TODO
	// If there is more than one -> filter
	if (clusters.size() == 0) {
		return false;
	}
	// TODO remove hack
	glintCenter.clear();
	if (clusters.size() == 1) {

		Point p = clusters.at(0).centerPoint();
		cross(frame, p, 5);
	}
	if (clusters.size() > 1) {
		sort(clusters.begin(), clusters.end());

		lastMeasurement = clusters.at(0).centerPoint();
		cross(pointImage, lastMeasurement, 6);

	}

	/// Show in a window
	//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	//imshow("Contours", img);

	//namedWindow("Points", CV_WINDOW_AUTOSIZE);
	//imshow("Points", pointImage);

	glintCenter = clusters.at(0).glintsInCluster();

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
		for (int j = i; j < n; j++) {
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
void FindGlints::findClusters(vector<cv::Point>& blobs,
		vector<GlintCluster>& clusters, cv::Point& lastMeasurement) {

	Mat nighbourMat = distanceMatrix(blobs);

	LOG_D("DistanceMat out: " << endl << " " << nighbourMat << endl);

	// Calculate number of nighbors per row
	Mat column_sum;
	reduce(nighbourMat, column_sum, 1, CV_REDUCE_SUM, CV_32S);
	LOG_D("Sum: " << column_sum);

	// This beautiful piece of code might be reviewed by Mr. C++
	// Principle idea: Iterate over nighborMat and only consider lines
	// with at least 4 glints (3 nightbours). Those line creates a new cluster of glints.
	for (int row = 0; row < nighbourMat.rows; ++row) {
		uchar* p = nighbourMat.ptr(row);
		if (column_sum.at<char>(row, 0) >= GazeConstants::GLINT_COUNT) {
			std::vector<cv::Point> glints;
			for (int col = 0; col < nighbourMat.cols; ++col) {
				if (*p++ == 1) {
					glints.push_back(blobs.at(col));
				}
			}
			GlintCluster glintCluster(glints, lastMeasurement);
			clusters.push_back(glintCluster);
		}
	}

}

