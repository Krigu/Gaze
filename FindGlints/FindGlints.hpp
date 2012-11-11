#include <string>

#include <opencv2/objdetect/objdetect.hpp>



using namespace std;

class FindGlints {

private:
	cv::Mat distanceMatrix(vector<cv::Point>& glintCenter);
	void filterByNighbor(vector<cv::Point>& blobs);

public:
	bool findGlints(cv::Mat & image, vector< cv::Point >& glintCenter);


};
