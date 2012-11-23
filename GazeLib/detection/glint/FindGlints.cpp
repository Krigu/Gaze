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

/**
 * Gets the center of the glints and attaches the center of the glintcluster
 * to the variable lastMeasurement
 */
bool FindGlints::findGlints(cv::Mat& frame, vector<cv::Point>& glintCenters,
		cv::Point& lastMeasurement) {

	Mat img = Mat(frame);

	// Threshold image.
	threshold(frame, img, GazeConstants::GLINT_THRESHOLD, 255,
			cv::THRESH_TOZERO);

#ifdef __DEBUG
	imshow("Thresholded image", img);
#endif

	// Dilate the blobs. Sometimes a glint is just one small pixel
	dilate(img, img, Mat::ones(2, 2, CV_8U));

	std::vector<std::vector<cv::Point> > contours;

	vector<Vec4i> hierarchy;
	// Find all countours
	findContours(img, contours, // a vector of contours
			CV_RETR_EXTERNAL, // retrieve the external contours
			CV_CHAIN_APPROX_NONE); // all pixels of each contours

#ifdef __DEBUG
	/// Draw contours
	RNG rng(12345);
	Mat drawing = Mat::zeros(img.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8);
	}
	imshow("Contours: ", drawing);
#endif

	LOG_D("Countours size: " << contours.size());

	Blobs blobs = Blobs(contours);
	LOG_D("Blobs size 1: " << blobs.blobSize());
	blobs.removeInvalidSize();
	LOG_D("Blobs size 2: " << blobs.blobSize());
	blobs.removeInvalidShape();
	LOG_D("Blobs size 3: " << blobs.blobSize());

	blobs.blobCenters(glintCenters);

	LOG_D("Blobs size: " << glintCenters);

#ifdef __DEBUG
	Mat glints = Mat::zeros(img.size(), CV_8UC3);
	std::vector<Point>::iterator iter;
	for (iter = glintCenters.begin(); iter != glintCenters.end(); ++iter) {
		cross(glints, *iter, 5);
	}
	imshow("Glints", glints);
#endif

	// Find all clusters
	vector<GlintCluster> clusters;
	findClusters(glintCenters, clusters, lastMeasurement);

	if (clusters.size() == 0) {
		return false;
	} else if (clusters.size() == 1) {

		lastMeasurement = clusters.at(0).centerPoint();
	} else if (clusters.size() > 1) {

		sort(clusters.begin(), clusters.end());
		lastMeasurement = clusters.at(0).centerPoint();
	}

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

	// Get the distance Matrix
	Mat nighbourMat = distanceMatrix(blobs);

	LOG_D("DistanceMat out: " << endl << " " << nighbourMat << endl);

	// Calculate number of nighbors per row
	Mat column_sum;
	reduce(nighbourMat, column_sum, 1, CV_REDUCE_SUM, CV_32S);
	LOG_D("Sum: " << column_sum);

	// TODO: This beautiful piece of code might be reviewed by Mr. C++
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

