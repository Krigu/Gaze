//---------------------------------------------------------------------------

#ifndef IplClipLineToImageH
#define IplClipLineToImageH
//---------------------------------------------------------------------------
#include <opencv2/core/core.hpp>

/// clips a line onto an image using the cohen-sutherland algorithm
bool IplClipLineToImage( IplImage* img, double& x0, double& y0, double& x1, double& y1 );


#endif
