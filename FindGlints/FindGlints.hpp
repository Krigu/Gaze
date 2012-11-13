#include <string>

#include <opencv2/objdetect/objdetect.hpp>

#include "GlintCluster.hpp"

// TODO include ifdef

using namespace std;

class FindGlints {

private:
	cv::Mat distanceMatrix(vector<cv::Point>& glintCenter);
	void findClusters(vector<cv::Point>& blobs, vector<GlintCluster>& clusters);

public:
	bool findGlints(cv::Mat & image, vector<cv::Point>& glintCenter);

};
