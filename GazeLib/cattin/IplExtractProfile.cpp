//---------------------------------------------------------------------------

#include "IplExtractProfile.h"
#include "IplClipLineToImage.h"

//---------------------------------------------------------------------------



std::vector<unsigned char> IplExtractProfile( IplImage* image, double x0, double y0,
                           int r1, int r2, double phi, bool& done, double& dx, double& dy)
{
    int width = image->width;
    int height = image->height;
    
    // endpoints of profile
    double x1 = x0 + r1*std::cos(phi);
    double y1 = y0 + r1*std::sin(phi);
    double x2 = x0 + r2*std::cos(phi);
    double y2 = y0 + r2*std::sin(phi);
    
    // parameters of profile
    if( !IplClipLineToImage( image, x1, y1, x2, y2 ) )
    {
        done = false;
        std::vector<unsigned char> empty;
        return empty;
    }
    r2 = std::floor( std::sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0))+0.5 );
    int n = r2 - r1 + 1;
    //double
    dx = (x2-x1)/(n-1);
    //double
    dy = (y2-y1)/(n-1);

    // get profile (nearest neighbor)
    std::vector<unsigned char> profile;
    unsigned char* base = (unsigned char*)image->imageData;
    int step = image->widthStep;
    for( int i=0; i<n; ++i )
    {
        double x = x1 + i*dx;
        double y = y1 + i*dy;
        
        // bilinear interpolation
	    int xx = (int) floor(x);
	    int yy = (int) floor(y);

        if( xx>=0 && xx<width && yy>=0 && yy<height )
        {
            // get the four surrounding pixel positions 	 
	        int x1 = (x < xx) ? xx - 1 : xx;
	        int y1 = (y < yy) ? yy - 1 : yy;
	        int x2 = x1 + 1;
	        int y2 = y1 + 1;

            if( x1<0 || x1>width-1 )
                x1 = x;
            if( x2<0 || x2>width-1 )
                x2 = x;
            if( y1<0 || y1>height-1 )
                y1 = y;
            if( y2<0 || y2>height-1 )
                y2 = y;

            double t = x - x1;
	        double u = y - y1;
	//        double tu = t * u;

            double p1 = *(base + y1*step + x1);
            double p2 = *(base + y1*step + x2);
            double p3 = *(base + y2*step + x1);
            double p4 = *(base + y2*step + x2);

            double h1 = (1.0-t)*p1 + t*p2;
            double h2 = (1.0-t)*p3 + t*p4;

            double result = (1.0-u)*h1 + u*h2;

      //      double 	result = (1.0 - t - u + tu)*p1 + (1 - u)*p2 + (u - tu)*p3 + tu*p4;
    //        double 	result = (1.0 - t - u + tu) * p1 + (u - tu) * p2 + (t - tu) * p3 + tu * p4;
            profile.push_back( result );
        }
        else
            break; // stop clipping this line when outside of the image
        //    profile.push_back( 0 );
    }
    done = true;
    return profile;
}

std::vector<unsigned char> IplExtractProfile( cv::Mat* image, double x0, double y0,
                           int r1, int r2, double phi, bool& done, double& dx, double& dy){

	IplImage ipl_img = *image;
	return IplExtractProfile( &ipl_img,  x0,  y0, r1,  r2,  phi, done, dx, dy);
}
