
#include "bestFitCircle.hpp"

#include <iostream>

/**
 * 
 * original matlab code:
 * 
 *  N = size(points,1);
 *  x = points(:,1);
 *  y = points(:,2);
 *  M = [2*x 2*y ones(N,1)];
 *  v = x.^2+y.^2;
 *  pars = M\v;
 *  x0 = pars(1);
 *  y0 = pars(2);
 *  center = pars(1:2);
 *  R = sqrt(sum(center.^2)+pars(3));
 *  if imag(R)~=0,
 *      error('Imaginary squared radius computed');
 *  end;
 * 
 * @param x
 * @param y
 * @param radius
 * @param pointsToFit
 */
void bestFitCircle(float * x, float * y, float * radius,
        std::vector<cv::Point2f> pointsToFit) {

    unsigned int numPoints = pointsToFit.size();
    
    //setup an opencv mat with our points
    // col 1 = x coordinates and col 2 = y coordinates
    cv::Mat1f points(numPoints, 2);

    unsigned int i=0;
    for (std::vector<cv::Point2f>::iterator it = pointsToFit.begin(); it != pointsToFit.end(); ++it) {
        points.at<float>(i,0) = it->x;
        points.at<float>(i,1) = it->y;
        ++i;
    }

    cv::Mat1f x_coords = points.col(0);
    cv::Mat1f y_coords = points.col(1);
    cv::Mat1f ones = cv::Mat::ones(numPoints, 1, CV_32F);
    
    // calculate the M matrix
    cv::Mat1f M(numPoints, 3);
    cv::Mat1f x2 = x_coords * 2;
    cv::Mat1f y2 = y_coords * 2;
    x2.col(0).copyTo(M.col(0));
    y2.col(0).copyTo(M.col(1));
    ones.col(0).copyTo(M.col(2));
    
    // calculate v
    cv::Mat1f v = x_coords.mul(x_coords) + y_coords.mul(y_coords);
    
    //TODO some error handling in solve()?
    cv::Mat pars;
    cv::solve(M, v, pars, cv::DECOMP_SVD);
    
    *x = pars.at<float>(0,0);
    *y = pars.at<float>(0,1);
    *radius = sqrt(pow(*x,2) + pow(*y,2) + pars.at<float>(0,2)); 
}
