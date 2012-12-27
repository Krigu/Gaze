#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "../glint/FindGlints.hpp"

using namespace std;
using namespace cv;

typedef Rect* (*eyeCompareFunction)(Rect*, Rect*);

class FindEyeRegion {
private:
    FindGlints& findGlints;
    
    eyeCompareFunction leftEyeCompareFunc;
    eyeCompareFunction rightEyeCompareFunc;
    cv::CascadeClassifier eye_region_classifier;
    cv::CascadeClassifier eye_classifier;
    bool findEye(Mat &image, Rect& eyeRect, eyeCompareFunction& compareFunc);
    bool hasGlintsInRect(Mat &image, Rect& eyeRect);
    bool removeInvalidRects(Mat& image, vector<Rect>& regions);
public:
    FindEyeRegion(FindGlints& findGlints);
    bool findLeftEye(Mat &image, Rect& eyeRect);
    bool findRightEye(Mat &image, Rect& eyeRect);
    bool findEye(Mat &image, Rect& eyeRect);
};
