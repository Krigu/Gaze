#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>



using namespace std;
using namespace cv;

class FindEyeRegion {

private:
	static const string HAAR_FILENAME;
	cv::CascadeClassifier cascade_eye;

public:
	bool findRegion(Mat & image, Rect& rect);

};
