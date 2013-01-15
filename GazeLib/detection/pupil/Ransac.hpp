#ifndef RANSAC_HPP
#define	RANSAC_HPP

#include <vector>
#include <opencv2/core/core.hpp>

/**
 * Implementation of the ransac algorithm
 * 
 * - chose 3 random points
 * - fit circle
 * - count points within circle +/- a chosen "T" distance
 * - repeat above steps N times
 * - use the circle which had the most points within the
 *   range
 * - search for the circle who fits these circles best
 */
class Ransac {
public:
    /**
     * Fits a circle into a cloud of points
     * 
     * @param x component of the center
     * @param y component of the center
     * @param radius of the circle
     * @param points list of all detected feature points
     * @return if found true, false otherwise
     */
    bool ransac(float * x, float * y, float * radius, std::vector<cv::Point2f> points);

private:
    void fitCircle(float * x, float * y, float * r, std::vector<cv::Point2f>);
};


#endif	/* RANSAC_HPP */

