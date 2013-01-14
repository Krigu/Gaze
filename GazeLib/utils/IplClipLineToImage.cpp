//---------------------------------------------------------------------------

#include "IplClipLineToImage.h"

//---------------------------------------------------------------------------

static int outCodes( IplImage* img, double x, double y )
{
    int code = 0;
    if( y < 0.0)
        code += 1; // code for top
    else if( y >=  (double)img->height )
        code += 2; /* code for bottom */
    if( x >= (double)img->width )
        code += 4; /* code for right */
    else if( x < 0.0)
        code += 8; /* code for left */

    return code;
}

inline static bool rejectCheck( int outCode1, int outCode2 )
{
    return ( (outCode1 & outCode2) != 0 );
} 


inline static bool acceptCheck(int outCode1, int outCode2)
{
    return ( (outCode1 == 0) && (outCode2 == 0) );
}

bool IplClipLineToImage( IplImage* img, double& x0, double& y0, double& x1, double& y1 )
{
    int outCode0,outCode1;
    double m = 0.0;
    if( x0 != x1 )
        m = (double)(y1 - y0)/ (double)(x1 - x0);
    int x = 0;
    int y = 0;

    while( true )
    {
        outCode0 = outCodes( img, x0, y0 );
        outCode1 = outCodes( img, x1, y1 );
        if( rejectCheck( outCode0, outCode1 ) )
            return false;
        if( acceptCheck( outCode0, outCode1 ) )
            return true;


        int outCode = (outCode0 == 0)? outCode1 : outCode0;
        if( outCode & 1) // top
        {
            if( m != 0.0 )
                x = x1 + (double)(1/m) * (double)(- y1);
            else
                x = x1;
            y = 0.0;
        }
        else if( outCode & 2) // bottom
        {
            if( m != 0.0 )
                x = x1 + (double)(1/m) * (double)(img->height-1 - y1);
            else
                x = x1;
            y = img->height-1;
        }

        if( outCode & 8 ) // left
        {
            x = 0.0;
            y = y1 + (double)(m) * (double)( - x1);
        }
        else if(outCode & 4 ) // right
        {
            x = img->width-1;
            y = y1 + (double)(m) * (double)(img->width-1 - x1);
        }

        if( outCode0 == outCode )
        {
            x0 = x;
            y0 = y;
            outCode0 = outCodes( img, x0, y0 );
        }
        else
        {
            x1 = x;
            y1 = y;
            outCode1 = outCodes( img, x1, y1 );
        }
    }
    return false; // just to quieten the compiler...
}
