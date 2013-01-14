#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "../glint/FindGlints.hpp"

using namespace std;
using namespace cv;

/**
 *
 */
typedef void (*eyeRegionAdjustFunction)(Rect*);

/**
 * This class tries to find the eye region and the
 * left/right eye. 
 * 
 * This class uses two haar cascades to identify these regions.
 * 
 */
class FindEyeRegion {
private:
    FindGlints& findGlints;

    eyeRegionAdjustFunction leftEyeRegionClipper;
    eyeRegionAdjustFunction rightEyeRegionClipper;
    cv::CascadeClassifier eye_region_classifier;
    cv::CascadeClassifier eye_classifier;
    bool findEye(Mat &image, Rect& eyeRect, eyeRegionAdjustFunction& compareFunc);
    bool hasGlintsInRect(Mat &image, Rect& eyeRect);
    void removeInvalidRects(Mat& image, vector<Rect>& regions);
public:
    /**
     * 
     * @param findGlints 
     */
    FindEyeRegion(FindGlints& findGlints);

    /**
     * Tries to identify the left eye and adjusts 
     * the provided eyeRect to the area where the eye 
     * has been found.
     * 
     * @param image Image in which an eye is searched
     * @param eyeRect if found, the region of the left eye
     * @return true if the left eye has been found, false otherwise
     */
    bool findLeftEye(Mat &image, Rect& eyeRect);

    /**
     * Tries to identify the right eye and adjusts 
     * the provided eyeRect to the area where the eye 
     * has been found.
     * 
     * @param image Image in which an eye is searched
     * @param eyeRect if found, the region of the right eye
     * @return true if the left eye has been found, false otherwise
     */
    bool findRightEye(Mat &image, Rect& eyeRect);

    /**
     * Tries to identify the eye and adjusts 
     * the provided eyeRect. The config property
     * GazeConfig::DETECT_LEFT_EYE determines if 
     * the left or right eye will be returned.
     * 
     * @param image Image in which an eye is searched
     * @param eyeRect if found, the region of the eye
     * @return true if the left eye has been found, false otherwise
     */
    bool findEye(Mat &image, Rect& eyeRect);
};
