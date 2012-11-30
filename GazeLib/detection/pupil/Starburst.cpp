#include <opencv2/imgproc/imgproc.hpp>

#include "../../GazeConstants.hpp"
#include "Starburst.hpp"

#include "../../cattin/IplExtractProfile.h"

using namespace cv;
using namespace std;

#include "../../utils/log.hpp"

#if __DEBUG_STARBURST == 1
#include "../../utils/gui.hpp"
#include <opencv2/highgui/highgui.hpp>
#endif

Starburst::Starburst() {

	// pre calculate the sin/cos values
	float angle_delta = 1 * PI / 180;

	for (int i = 0; i < angle_num; i++) {
		this->angle_array[i] = i * angle_delta;
		this->sin_array[i] = sin(this->angle_array[i]);
		this->cos_array[i] = cos(this->angle_array[i]);
	}

}

/*
 * idea: only use the precalculated rectangle.
 * - remove the glints, 
 * - use min to find a point in the pupil
 * - use starburst to find the pupil and its center
 * 
 */
bool Starburst::processImage(cv::Mat& frame, vector<cv::Point> glint_centers,
		cv::Point2f startpoint, cv::Point2f &pupil_center, float & radius) {

	bool found = false;

	// only search within a limited region
	search_area = Rect(
			startpoint.x - GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT / 2,
			startpoint.y - GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT / 2,
			GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT,
			GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT);

	// get our working area of the image
	Mat without_glnts = frame.clone();
	Mat eye_area = without_glnts(search_area);

	Point2f relative_new_center(
			GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT / 2,
			GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT / 2);

#if __DEBUG_STARBURST == 1
	imshow("with glints", eye_area);
#endif

	// the algorithm: blur image, remove glint and starburst
	remove_glints(without_glnts, glint_centers, GazeConstants::GLINT_RADIUS);

#if __DEBUG_STARBURST == 1
	imshow("without glints", eye_area);
#endif

	medianBlur(eye_area, without_glnts, 3);
#if __FINDPUPIL_STARBURST == 1
	found = starburst(eye_area, relative_new_center, radius, 20);
#else
	pupil_threasholding(eye_area, relative_new_center, radius, 20, 1);
#endif

	// display the center on the source image
	if (found)
		pupil_center = Point2f(search_area.x + relative_new_center.x,
				search_area.y + relative_new_center.y);

	return found;
}

/**
 * removes each glint using the interpolatin of the average
 * perimeter intensity and the perimeter pixel for each angle
 * this
 */
void Starburst::remove_glints(cv::Mat &gray, vector<cv::Point> glint_centers,
		short radius) {

	for (vector<Point>::iterator it = glint_centers.begin();
			it != glint_centers.end(); ++it) {

		// this code is based on the algorithm by Parkhurst and Li
		// cvEyeTracker - Version 1.2.5 - http://thirtysixthspan.com/openEyes/software.html
		int i, r, r2, x, y;
		uchar perimeter_pixel[angle_num];
		int sum = 0;
		double avg;
		for (i = 0; i < angle_num; i++) {
			x = (int) (it->x + radius * cos_array[i]);
			y = (int) (it->y + radius * sin_array[i]);
			perimeter_pixel[i] = (uchar) (*(gray.data + y * gray.cols + x));
			sum += perimeter_pixel[i];
		}
		avg = sum * 1.0 / angle_num;

		for (r = 1; r < radius; r++) {
			r2 = radius - r;
			for (i = 0; i < angle_num; i++) {
				x = (int) (it->x + r * cos_array[i]);
				y = (int) (it->y + r * sin_array[i]);
				*(gray.data + y * gray.cols + x) = (uchar) ((r2 * 1.0 / radius)
						* avg + (r * 1.0 / radius) * perimeter_pixel[i]);
			}
		}
	}
}
void Starburst::pupil_threasholding(cv::Mat &gray, Point2f &center,
		float &radius, int num_of_lines, int distance_growth) {

	MatND hist;

	// TODO put variables in setUp
	int nbins = 256; // 256 levels histogram levels
	int hsize[] = { nbins }; // just one dimension
	float range[] = { 0, 255 };
	const float *ranges[] = { range };
	int chnls[] = { 0 };

	// Calc history
	calcHist(&gray, 1, chnls, Mat(), hist, 1, hsize, ranges);

	float pixelSum;
	int threshold = 0;

	// Find threshold beginning with brightest point
	for (int i = 0; i < hist.rows; i++) {
		float histValue = hist.at<float>(i, 0);
		pixelSum += histValue;
		if (pixelSum > 750) {
			threshold = i;
			break;
		}
	}

	cv::threshold(gray, gray, threshold, 0, cv::THRESH_TOZERO_INV);

//	Mat img;
//	// Threshold image.
//	threshold(gray, gray, 150, 0, cv::THRESH_TOZERO_INV);

	imshow("pupil thres", gray);
}

/**
 * 
 * @param gray a grayscale image of the eye
 * @param start a starting point for the algorithm inside the pupil
 * @param num_of_lines the number of lines to draw (5 degrees = 72 lines)
 * @param distance_growth how fast should the lines grow? smaller = exacter
 * @return the center of the pupil
 */
bool Starburst::starburst(cv::Mat &gray, Point2f &center, float &radius,
		int num_of_lines) {
	const double angle = 2 * PI / num_of_lines; // in radiants!
	const Scalar color = Scalar(255, 255, 255);

	bool found=false;

	std::vector<Point2f> points;

	Point2f start_point = Point2f(center.x, center.y);

	for(unsigned short iterations = 0; iterations < GazeConstants::MAX_RANSAC_ITERATIONS; ++iterations){

		points.clear();

		// calculate the lines in every direction
		for (unsigned short i = 0; i < num_of_lines; i++) {
			// calculate the current degree
			const double current_angle = angle * i;

			bool done;
			double dx, dy;
			dx = dy = 0;

			//TODO Herr cattin fragen (dx/dy rausgeben...)
			vector<unsigned char> profile = IplExtractProfile(&gray,
					start_point.x, start_point.y, 0, 50, current_angle, done,
					dx, dy);

			int vectorSize = profile.size();
			if (vectorSize > 3) {
				for (int i = 3; i < vectorSize; i++) {
					unsigned char current = profile.at(i);
					unsigned char last = profile.at(i - 3);

					if (current > (last + 10)) {
						float x = start_point.x + i * dx;
						float y = start_point.y + i * dy;
						Point2f p = Point2f(x, y);
						points.push_back(p);
						break;
					}
				}
			}
		}

		// calculate the mean of all points
		float mean_x = 0, mean_y = 0;
		for (vector<Point2f>::iterator it = points.begin(); it != points.end();
				++it) {
			mean_x += it->x;
			mean_y += it->y;
		}

		if (points.size() > 0) {
			mean_x /= points.size();
			mean_y /= points.size();
		} else {
			//TODO report error
			LOG_D("no mean calculated!");
			continue;
		}

		if ((fabs(mean_x - start_point.x) + fabs(mean_y - start_point.y)) < 4) {
			found = true;
			break; // the mean converged, lets fit a circle now
		}

		start_point.x = mean_x;
		start_point.y = mean_y;

	}

	// we didn't find a center within the
	// max iterations!
	if(!found)
		return false;

	float x, y, r;
	x = y = r = 0;
	Ransac ransac;
	found = ransac.ransac(&x, &y, &r, points);

	// find the radius and the circle center
	//minEnclosingCircle(points, center, radius);
	//LOG_D("Size: " << points.size());

	if(found){
		center = Point2f(x, y);
		radius = r;
	}

#if __DEBUG_STARBURST == 1
	Mat copy = gray.clone();
	for (std::vector<Point>::iterator it = points.begin(); it != points.end();
			++it) {
		cross(copy, *it, 3);
		//line(gray, start, *it, color);
		//circle(gray, *it, 1, color);
	}
	imshow("starburst result", copy);
#endif

	return found;
}

// the RANSAC stuff:

//
// algorithm:
// - chose 3 random points
// - fit circle
// - count points within circle +/+ a chosen "t
// - repeate above steps N times
//

bool Ransac::ransac(float * x, float * y, float * radius,
		std::vector<cv::Point2f> points) {
	// N: num of iterations
	const int N = 1000;
	// T: distance in which
	const float T = 2;

	bool found = false;

	if(points.size() < 3)
		return found;

	// initialize randomizer
	srand(time(NULL));

	int max_points_within_range = 0;

#if __DEBUG_STARBURST == 1
	Mat debug_result;
#endif

	// lets fit a circle into 3 random points
	// after N iterations the circle that fitted
	// the most points is chosen as the result
	for (int i = 0; i < N; i++) {
		float tmp_x, tmp_y, tmp_r;
		tmp_x = tmp_y = tmp_r = 0;
		int points_within_range = 0;

		// fit a random circle
		std::random_shuffle(points.begin(), points.end());
		fitCircle(&tmp_x, &tmp_y, &tmp_r, points);

		//TODO: pupil size should be configurable
		if (tmp_r < GazeConstants::PUPIL_MIN_RADIUS
				|| tmp_r > GazeConstants::PUPIL_MAX_RADIUS)
			continue;

#if __DEBUG_STARBURST == 1
		Mat debug = Mat::zeros(90, 90, CV_8UC3);
		cv::Point a = points.at(0);
		cv::Point b = points.at(1);
		cv::Point c = points.at(2);

		cross(debug, a, 5, Scalar(255, 0, 0));
		cross(debug, b, 5, Scalar(255, 0, 0));
		cross(debug, c, 5, Scalar(255, 0, 0));

		circle(debug, Point(tmp_x, tmp_y), tmp_r, Scalar(255, 255, 255));
		imshow("debug", debug);
#endif

		if (tmp_x == std::numeric_limits<float>::min()
				|| tmp_y == std::numeric_limits<float>::min()
				|| tmp_r == std::numeric_limits<float>::min())
			continue;

		const float lower_bound = tmp_r - T;
		const float upper_bound = tmp_r + T;

		// how many points lie within the lower/upper bound of the radius
		for (std::vector<cv::Point2f>::iterator it = points.begin();
				it != points.end(); ++it) {
			//TODO: opencv should do this with magnitude()
			// calculate the magnitude between the point and the center
			float delta_y = it->y - tmp_y;
			float delta_x = it->x - tmp_x;

			float magnitude = sqrt(pow(delta_y, 2) + pow(delta_x, 2));

			if (magnitude >= lower_bound && magnitude <= upper_bound) {
				points_within_range++;
#if __DEBUG_STARBURST == 1
				cross(debug, *it, 5, Scalar(0, 255, 0));
#endif
			} else {
#if __DEBUG_STARBURST == 1
				cross(debug, *it, 5, Scalar(0, 0, 255));
#endif
			}

		}

		if (points_within_range > max_points_within_range) {
			*x = tmp_x;
			*y = tmp_y;
			*radius = tmp_r;
			max_points_within_range = points_within_range;
			found = true;
#if __DEBUG_STARBURST == 1
			debug_result = debug;
#endif
		}
	}

#if __DEBUG_STARBURST == 1
	if(found) {
		LOG_D("RANSAC_RESULT: x=" << *x << " y=" << *y << " R=" << *radius);
		imshow("debug", debug_result);
	}
#endif

	return found;
}

//
// fits a circle into the first three points of the vector
//

void Ransac::fitCircle(float * x, float * y, float * radius,
		std::vector<cv::Point2f> points) {
	// http://www.exaflop.org/docs/cgafaq/cga1.html
	// "Subject 1.04: How do I generate a circle through three points?"
	cv::Point2f a = points.at(0);
	cv::Point2f b = points.at(1);
	cv::Point2f c = points.at(2);

	float A = b.x - a.x;
	float B = b.y - a.y;
	float C = c.x - a.x;
	float D = c.y - a.y;
	float E = A * (a.x + b.x) + B * (a.y + b.y);
	float F = C * (a.x + c.x) + D * (a.y + c.y);
	float G = 2.0 * (A * (c.y - b.y) - B * (c.x - b.x));

	// wenn G nahe bei null ist, so sind die drei Punkte
	if (G < 0.000000001) {
		*x = *y = *radius = std::numeric_limits<float>::min();
		return;
	}

	*x = (D * E - B * F) / G;
	*y = (A * F - C * E) / G;

	*radius = sqrt(pow(a.x - *x, 2) + pow(a.y - *y, 2));

	//A = b_0 - a_0;
	//B = b_1 - a_1;
	//C = c_0 - a_0;
	//D = c_1 - a_1;
	//E = A*(a_0 + b_0) + B*(a_1 + b_1);
	//F = C*(a_0 + c_0) + D*(a_1 + c_1);
	//G = 2.0*(A*(c_1 - b_1)-B*(c_0 - b_0));
	//p_0 = (D*E - B*F) / G;
	//p_1 = (A*F - C*E) / G;
	//If G is zero then the three points are collinear and no finite-radius circle through them exists. Otherwise, the radius of the circle is:
	//
	//r^2 = (a_0 - p_0)^2 + (a_1 - p_1)^2
}

void Starburst::smooth_vector(std::vector<unsigned char>& vector) {
	// TODO: optimize
	std::vector<unsigned char> average;
	int size = vector.size();
	for (int i = 0; i < size; i++) {
		average.push_back(calcRegionAverage(i, vector));
	}
	vector = average;
}

unsigned char Starburst::calcRegionAverage(int index,
		std::vector<unsigned char>& vector) {
	int offset = 10;
	int start = index - (offset / 2);
	int end = index + (offset / 2);

	start = (start < 1) ? 0 : start;
	end = (end >= vector.size() - 1) ? vector.size() : end;

	int sum = 0;
	int divisor = 0;
	for (int i = start; i < end; i++) {
		sum += vector.at(i);
		divisor++;
	}

	if (divisor > 0)
		return (sum / divisor);
	else
		return sum;

}
