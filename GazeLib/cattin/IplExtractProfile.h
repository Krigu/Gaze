//---------------------------------------------------------------------------

#ifndef IplExtractProfileH
#define IplExtractProfileH
//---------------------------------------------------------------------------

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

std::vector<unsigned char> IplExtractProfile( IplImage* image, double x0, double y0,
                           int r1, int r2, double phi, bool& done );

std::vector<unsigned char> IplExtractProfile( cv::Mat* image, double x0, double y0,
                           int r1, int r2, double phi, bool& done );

#endif
