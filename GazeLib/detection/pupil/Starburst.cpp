#include <opencv2/imgproc/imgproc.hpp>

#include "../../config/GazeConfig.hpp"
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
		cv::Point2f glintcenter, cv::Point2f &pupil_center, float & radius) {

	Mat working_frame = frame.clone();
	bool found = false;
	Point2f startpoint(glintcenter.x,glintcenter.y);

	remove_glints(working_frame, glint_centers, GazeConfig::GLINT_RADIUS);
	medianBlur(working_frame, working_frame, 3);
	found = starburst(working_frame, startpoint, radius, 20);

	// display the center on the source image
	if (found)
		pupil_center = startpoint;

#if __DEBUG_STARBURST == 1
	imshow("with glints", frame);
	imshow("without glints", working_frame);
#endif

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
	const float angle = 2 * PI / num_of_lines; // in radiants!
	const Scalar color = Scalar(255, 255, 255);

	bool found=false;

	std::vector<Point2f> points;

	Point2f start_point = Point2f(center.x, center.y);

	for(unsigned short iterations = 0; iterations < GazeConfig::MAX_RANSAC_ITERATIONS; ++iterations){

		points.clear();

		// calculate the lines in every direction
		for (unsigned short i = 0; i < num_of_lines; i++) {
			// calculate the current degree
			const double current_angle = angle * i;

			bool done;
			double dx, dy;
			dx = dy = 0;

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
	for (std::vector<Point2f>::iterator it = points.begin(); it != points.end();
			++it) {
		cross(copy, *it, 3);
		//line(gray, start, *it, color);
		//circle(gray, *it, 1, color);
	}
	imshow("starburst result", copy);
#endif

	return found;
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
	unsigned int offset = 10;
	unsigned start = index - (offset / 2);
	unsigned int end = index + (offset / 2);

	start = (start < 1) ? 0 : start;
	end = (end >= vector.size() - 1) ? vector.size() : end;

	int sum = 0;
	int divisor = 0;
	for (unsigned int i = start; i < end; i++) {
		sum += vector.at(i);
		divisor++;
	}

	if (divisor > 0)
		return (sum / divisor);
	else
		return sum;

}
