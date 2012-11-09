#include <string>

#include <opencv2/objdetect/objdetect.hpp>



using namespace std;

class FindGlints {

private:
	cv::Mat distanceMatrix(cv::Mat & image, std::vector<std::vector<cv::Point> > & contours);

public:
	bool findGlints(cv::Mat & image, vector< cv::Point >& glintCenter);


};
