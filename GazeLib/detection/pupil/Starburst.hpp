#ifndef STARBURST_HPP
#define	STARBURST_HPP

#include <vector>
#include <opencv2/core/core.hpp>

#include "Ransac.hpp"

/**
 * Implementation of the Startburst algorithm
 */
class Starburst {
private:
	cv::Rect search_area;

	// needed for the precalculation of sin/cos
	static const int angle_num = 360;
	double angle_array[angle_num];
	double sin_array[angle_num];
	double cos_array[angle_num];
    
    Ransac ransac;

public:
	Starburst();
	bool findPupil(cv::Mat & image, std::vector<cv::Point> glint_centers,
			cv::Point2f startpoint, cv::Point2f & pupil_center, float & radius);

private:
	bool starburst(cv::Mat &image, cv::Point2f &center, float &radius,
			int num_of_lines);
    bool followRay(cv::Mat &gray, const cv::Point2f &start_point, 
            const double current_angle, cv::Point2f &edgePoint, 
            const int line_length, const int edge_threshold);
	void remove_glints(cv::Mat &gray, std::vector<cv::Point> glint_centers,
			short interpolation_size);
	void smooth_vector(std::vector<unsigned char>& vector);
	unsigned char calcRegionAverage(int index, std::vector<unsigned char>& vector);
};

#endif	/* STARBURST_HPP */

