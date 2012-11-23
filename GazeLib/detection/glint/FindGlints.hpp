#ifndef FINDGLINTS_HPP_
#define FINDGLINTS_HPP_

#include <string>

#include <opencv2/objdetect/objdetect.hpp>

#include "GlintCluster.hpp"

using namespace std;

class FindGlints {

private:
	cv::Mat distanceMatrix(vector<cv::Point>& glintCenter);
	void findClusters(vector<cv::Point>& blobs, vector<GlintCluster>& clusters,
			cv::Point& lastMeasurement);

public:
	bool findGlints(cv::Mat & image, vector<cv::Point>& glintCenters,
			cv::Point& lastMeasurement);

};

#endif /* FINDGLINTS_HPP_ */
