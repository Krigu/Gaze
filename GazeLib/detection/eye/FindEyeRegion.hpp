#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;
using namespace cv;




class EyeRegionData {
private:
	Rect *eye_region;
	Rect *left_eye;
	Rect *right_eye;
	Rect getRegion(Rect *region);
public:
	//TODO: setter und getter kopieren
	EyeRegionData();
	~EyeRegionData();
	bool isDataSet();
	Rect getLeftEye();
	Rect getRightEye();
	Rect getEyeRegion();
	void setLeftEye(Rect *left_eye);
	void setRightEye(Rect *right_eye);
	void setEyeRegion(Rect *eyes);
};

class FindEyeRegion {

private:
	cv::CascadeClassifier eye_region_classifier;
	cv::CascadeClassifier eye_classifier;
	//void removeFalsePositives(vector<Rect> &eyes);

public:
	FindEyeRegion();
	bool findLeftEye(Mat &image, Rect& eyeRect);
};
