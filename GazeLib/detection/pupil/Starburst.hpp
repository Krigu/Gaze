#ifndef STARBURST_HPP
#define	STARBURST_HPP

#include <vector>
#include <opencv2/core/core.hpp>

#define PI 3.14159265358979323846

class Starburst {
private:
	cv::Rect search_area;

	// needed for the precalculation of sin/cos
	static const int angle_num = 360;
	double angle_array[angle_num];
	double sin_array[angle_num];
	double cos_array[angle_num];

public:
	Starburst();
	void processImage(cv::Mat & image, std::vector<cv::Point> glint_centers,
			cv::Point startpoint, cv::Point & pupil_center, float & radius);

private:
	void starburst(cv::Mat &image, cv::Point2f &center, float &radius,
			int num_of_lines, int distance_growth);
	void pupil_threasholding(cv::Mat &image, cv::Point2f &center, float &radius,
			int num_of_lines, int distance_growth);
	void remove_glints(cv::Mat &gray, std::vector<cv::Point> glint_centers,
			short interpolation_size);
	void smooth_vector(std::vector<unsigned char>& vector);
	unsigned char calcRegionAverage(int index, std::vector<unsigned char>& vector);
};

class Ransac {
public:
	void ransac(float * x, float * y, float * radius, std::vector<cv::Point>);

private:
	void fitCircle(float * x, float * y, float * r, std::vector<cv::Point>);
};

#endif	/* STARBURST_HPP */

