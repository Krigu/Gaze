//#include <QString>
//#include <QDir>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>

#include "../GazeConstants.hpp"
#include "Starburst.hpp"
#include "../utils/gui.hpp"
#include "../utils/log.hpp"

#include "../cattin/IplExtractProfile.h"

#define PI 3.14159265358979323846

using namespace cv;
using namespace std;

/**
 * finds the eye using haar, then create a rectangle for one eye
 * TODO: use a haar file, that tracks one eye
 */
void Starburst::setUp(cv::VideoCapture& capture) {

}

/*
 * idea: only use the precalculated rectangle.
 * - remove the glints, 
 * - use min to find a point in the pupil
 * - use starburst to find the pupil and its center
 * 
 */
void Starburst::processImage(cv::Mat& frame) {

	//return;
	search_area = Rect(
			last_center.x - GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT / 2,
			last_center.y - GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT / 2,
			GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT,
			GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT);

	// get our working area of the image
	Mat img = frame(search_area);
	Mat gray = img;

	Point2f new_center(GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT / 2,
			GazeConstants::PUPIL_SEARCH_AREA_WIDHT_HEIGHT / 2);

	// mark the position on the image
	Scalar color = Scalar(0, 255, 0);
	rectangle(frame, search_area, color, 2, 8, 0);

	cvtColor(img, gray, CV_BGR2GRAY);

	// draw a circle around the darkest point
	//circle(img, last_center, 1, color);
	// let's guess the pupil size and only remove the glints inside it
	//Rect pupil = Rect(last_center.x - 10, last_center.y - 10, 20, 20);
	//rectangle(img, pupil, Scalar(0, 0, 255), 2, 8, 0);
	// the algorithm: blur image, remove glint and starburst
	medianBlur(gray, gray, 3);
	//  remove_glints(gray, pupil, 4, 4);
	float radius = 0;
	starburst(gray, new_center, radius, 20, 1);

	// display the center on the source image
	Point absolute_center = Point(search_area.x + new_center.x,
			search_area.y + new_center.y);
	cross(frame, absolute_center, 4, Scalar(0, 0, 255));
	circle(frame, absolute_center, radius, Scalar(0, 0, 255));

	// display our working area
	imshow("StarBurstResult", gray);
}

void Starburst::setLastCenter(Point2f last_center) {
	this->last_center = last_center;
}

/**
 * calls itself recursively. searches for num_of_glints glints inside the pupil_area.
 * all found glints are removed by bi-linear interpolations using a window with 
 * the size "2 * interpolation_size".
 * @param gray
 * @param pupil_area
 * @param num_of_glints
 * @param interpolation_size
 */
void Starburst::remove_glints(Mat& gray, Rect& pupil_area, short num_of_glints,
		short interpolation_size) {

	if (num_of_glints == 0) {
		imshow("GlintsRemoved", gray);
		return;
	}

	double minVal = 0;
	double maxVal = 0;
	Point minLoc;
	Point glint;

	Mat search_area = gray(pupil_area);

	// are there any glints inside our "pupil"
	minMaxLoc(search_area, &minVal, &maxVal, &minLoc, &glint, Mat());

	glint.x += pupil_area.x;
	glint.y += pupil_area.y;

	if (maxVal < 120) {
		return remove_glints(gray, pupil_area, --num_of_glints,
				interpolation_size);
	}

	//    if the eyes are closed, we get a point outside of the image...
	//    if (glint.x > gray.cols - interpolation_size
	//            || glint.y > gray.rows - interpolation_size)
	//        return;

	const short cols = interpolation_size * 2 + 1;
	const float fraction = 1.0 / cols;
	float current_col = 1;
	for (signed short i = -interpolation_size; i <= interpolation_size; i++) {

		float current_row = 1;

		// calculate the interpolated values at the top and bottom row of the window
		float top_row = (cols - current_col) * fraction
				* (float) gray.at<uchar>(glint.y - interpolation_size,
						glint.x - interpolation_size);
		top_row += current_col * fraction
				* (float) gray.at<uchar>(glint.y - interpolation_size,
						glint.x + interpolation_size);
		float bottom_row = (cols - current_col) * fraction
				* (float) gray.at<uchar>(glint.y + interpolation_size,
						glint.x - interpolation_size);
		bottom_row += current_col * fraction
				* (float) gray.at<uchar>(glint.y + interpolation_size,
						glint.x + interpolation_size);

		// if there are two glints, don't interpolate the bright pixels...
		// let's darken it
		if (top_row > minVal + 10)
			top_row = minVal + 10;
		if (bottom_row > minVal + 10)
			bottom_row = minVal + 10;

		current_col++;

		// interpolate the values
		for (signed short j = -interpolation_size; j <= interpolation_size;
				j++) {
			uchar interpolated = (cols - current_row) * fraction * top_row;
			interpolated += current_row * fraction * bottom_row;
			gray.at<uchar>(glint.y + j, glint.x + i) = interpolated;
			//            gray.at<uchar > (glint.y + j, glint.x + i) = minVal;
			current_row++;
		}
	}

	// remove the next glint
	remove_glints(gray, pupil_area, --num_of_glints, interpolation_size);
}

void Starburst::tearDown() {

}

/**
 * 
 * @param gray a grayscale image of the eye
 * @param start a starting point for the algorithm inside the pupil
 * @param num_of_lines the number of lines to draw (5 degrees = 72)
 * @param distance_growth how fast should the lines grow? smaller = exacter
 * @return the center of the pupil
 */
void Starburst::starburst(cv::Mat &gray, Point2f &center, float &radius,
		int num_of_lines, int distance_growth) {
	const double angle = 2 * PI / num_of_lines; // in radiants!
	const Scalar color = Scalar(255, 255, 255);

	// the intensity of the point that is inside the pupil
	uchar start_val = gray.at<uchar>(center.y, center.x);

	std::vector<Point> points;

	// calculate the lines in every direction
	for (unsigned short i = 0; i < num_of_lines; i++) {
		// calculate the current degree
		const double current_angle = angle * i;

		bool done;
		double dx, dy;
		dx = dy = 0;

		//TODO Herr cattin fragen (dx/dy rausgeben...)
		vector<unsigned char> profile = IplExtractProfile(&gray, center.x,
				center.y, 0, 130, current_angle, done, dx, dy);

		unsigned char start_val = 0;
		if (profile.size() > 0)
			start_val = profile.at(0);
		unsigned char val = 0;

		int j = 0;
		for (vector<unsigned char>::iterator it = profile.begin();
				it != profile.end(); it++) {
			if (*it > start_val + 5) { // TODO: +10 is probably a hack

			// calculate
				double x = center.x + j * dx;
				double y = center.y + j * dy;

				Point p = Point(x, y);
				if (*it - start_val <= 30) {
					points.push_back(p);
				}
				break;
			}
			++j;
		}
	}

	Ransac ransac;
	float x, y, r;
	x = y = r = 0;
	ransac.ransac(&x, &y, &r, points);

	// find the radius and the circle center
	minEnclosingCircle(points, center, radius);
	//LOG_D("Size: " << points.size());

	//center = Point2f(x, y);
	//radius = r;

	for (std::vector<Point>::iterator it = points.begin(); it != points.end();
			++it) {
		cross(gray, *it, 3);
		//line(gray, start, *it, color);
		//circle(gray, *it, 1, color);
	}
}

// the RANSAC stuff:

//
// algorithm:
// - chose 3 random points
// - fit circle
// - count points within circle +/+ a chosen "t
// - repeate above steps N times
//

void Ransac::ransac(float * x, float * y, float * radius,
		std::vector<cv::Point> points) {
	// N: num of iterations
	const int N = 1000;
	// T: distance in which
	const float T = 5;

	// initialize randomizer
	srand(time(NULL));

	int max_points_within_range = 0;

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

		if (tmp_x == std::numeric_limits<float>::min()
				|| tmp_y == std::numeric_limits<float>::min()
				|| tmp_r == std::numeric_limits<float>::min())
			continue;

		const float lower_bound = tmp_r - T;
		const float upper_bound = tmp_r + T;

		// how many points lie within the lower/upper bound of the radius
		for (std::vector<cv::Point>::iterator it = points.begin();
				it != points.end(); ++it) {
			//TODO: opencv should do this with magnitude()
			// calculate the magnitude between the point and the center
			float delta_y = it->y - tmp_y;
			float delta_x = it->x - tmp_x;

			float magnitude = sqrt(pow(delta_y, 2) + pow(delta_x, 2));

			if (magnitude >= lower_bound && magnitude <= upper_bound)
				points_within_range++;
		}

		if (points_within_range > max_points_within_range) {
			*x = tmp_x;
			*y = tmp_y;
			*radius = tmp_r;
			max_points_within_range = points_within_range;
		}
	}
}

//
// fits a circle into the first three points of the vector
//

void Ransac::fitCircle(float * x, float * y, float * radius,
		std::vector<cv::Point> points) {
	// http://www.exaflop.org/docs/cgafaq/cga1.html
	// "Subject 1.04: How do I generate a circle through three points?"
	cv::Point a = points.at(0);
	cv::Point b = points.at(1);
	cv::Point c = points.at(2);

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
