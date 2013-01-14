//---------------------------------------------------------------------------

#ifndef IplExtractProfileH
#define IplExtractProfileH
//---------------------------------------------------------------------------

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/// extracts a vector of intensity values starting at point (x0/y0) and following a 
/// line in the directino of angle phi
std::vector<unsigned char> IplExtractProfile( IplImage* image, double x0, double y0,
                           int r1, int r2, double phi, bool& done, double& dx, double& dy );

/// extracts a vector of intensity values starting at point (x0/y0) and following a 
/// line in the directino of angle phi
std::vector<unsigned char> IplExtractProfile( cv::Mat* image, double x0, double y0,
                           int r1, int r2, double phi, bool& done ,double& dx, double& dy);

#endif
