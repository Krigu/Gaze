#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "../glint/FindGlints.hpp"

using namespace std;
using namespace cv;

typedef void (*eyeRegionClipFunction)(Mat&, Rect*);

class FindEyeRegion {
private:
    FindGlints& findGlints;

    eyeRegionClipFunction leftEyeRegionClipper;
    eyeRegionClipFunction rightEyeRegionClipper;
    cv::CascadeClassifier eye_region_classifier;
    cv::CascadeClassifier eye_classifier;
    bool findEye(Mat &image, Rect& eyeRect, eyeRegionClipFunction& compareFunc);
    bool hasGlintsInRect(Mat &image, Rect& eyeRect);
    bool removeInvalidRects(Mat& image, vector<Rect>& regions);
public:
    FindEyeRegion(FindGlints& findGlints);
    bool findLeftEye(Mat &image, Rect& eyeRect);
    bool findRightEye(Mat &image, Rect& eyeRect);
    bool findEye(Mat &image, Rect& eyeRect);
};
