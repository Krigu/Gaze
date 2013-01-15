#include <opencv2/imgproc/imgproc.hpp>

#include "../../config/GazeConfig.hpp"
#include "Starburst.hpp"

#include "../../utils/IplExtractProfile.h"

#include "../../utils/log.hpp"
#include "../../utils/geometry.hpp"

using namespace cv;
using namespace std;



#if __DEBUG_STARBURST == 1
#include "../../utils/gui.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
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
 * - blur the image a little
 * - use starburst to find the pupil and its center
 * 
 */
bool Starburst::findPupil(cv::Mat& frame, vector<cv::Point> glint_centers,
		cv::Point2f glintcenter, cv::Point2f &pupil_center, float & radius) {

	Mat working_frame = frame.clone();
	bool found = false;
	Point2f startpoint(glintcenter.x,glintcenter.y);

	remove_glints(working_frame, glint_centers, GazeConfig::GLINT_RADIUS);
	medianBlur(working_frame, working_frame, 5);
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
 * removes each glint using the interpolation of the average
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
 * follows a starburst ray in the direction "current_angle". this method 
 * searches for an edge exceeding the edge_threshold.
 * 
 * @param gray the grayscale image to search in
 * @param start_point the start point of the search
 * @param current_angle the direction / angle of the line
 * @param edgePoint the edgePoint (if found)
 * @param line_length the length of the lines that should be checked
 * @param edge_threshold the threshold for the intensity
 * @return true if an edge has been found
 */
bool Starburst::followRay(cv::Mat &gray, const Point2f &start_point, 
            const double current_angle, Point2f &edgePoint, 
            const int line_length, const int edge_threshold){
    bool edgeFound=false;
    
    bool done;
    double dx, dy;
    dx = dy = 0;

    vector<unsigned char> profile = IplExtractProfile(&gray,
            start_point.x, start_point.y, 0, line_length, current_angle, done,
            dx, dy);

    //smooth_vector(profile);

    int vectorSize = profile.size();
    
    if (vectorSize > 5) {
        for (int i = 5; i < vectorSize; i++) {
            unsigned char current = profile.at(i);
            unsigned char last = profile.at(i - 5);

            if (current > (last + edge_threshold)) {

                float x = start_point.x + i * dx;
                float y = start_point.y + i * dy;
                edgePoint.x = x;
                edgePoint.y = y;
                edgeFound = true;
                break;

            }
        }
    }
    
    return edgeFound;
}

/**
 * applies the starburst algorithm to the given gray-image
 * @param gray the image
 * @param center the startpoint for the first iteration and the center of the pupil (if found)
 * @param radius the radius of the pupil (if found)
 * @param num_of_lines the number of lines the algorithm should use
 * @return true if the pupil has been found
 */
bool Starburst::starburst(cv::Mat &gray, Point2f &center, float &radius,
		int num_of_lines) {
	const double angle = 2 * PI / num_of_lines; // in radiants!
    const double lower_angle_limit = 130 * PI / 180; // 
    const double upper_angle_limit = 230 * PI / 180; // 

	bool found=false;

	std::vector<Point2f> points;

	Point2f start_point = Point2f(center.x, center.y);

    const int LINE_LENGTH=150;
    
	for(unsigned short iterations = 0; iterations < GazeConfig::MAX_STARBURST_ITERATIONS; ++iterations){

		points.clear();
        
		// calculate the lines in every direction
		for (unsigned short angleNum = 0; angleNum < num_of_lines; angleNum++) {
			// calculate the current degree
			const double current_angle = angle * angleNum;
            
            Point2f edgePoint;
            bool success = followRay(gray, start_point, current_angle, edgePoint, LINE_LENGTH, GazeConfig::STARBURST_EDGE_THRESHOLD);
            
            if(success){
                points.push_back(edgePoint);
                // now send some rays from the edgePoint into the other direction
                // we use +/- 50 dec around the ray in the other direction
                double new_angle = fmod((current_angle + lower_angle_limit), PI2);
                double target_angle = fmod((current_angle + upper_angle_limit), PI2);
                
                while(new_angle < target_angle){
                    Point2f new_edge;
                    bool success = followRay(gray, edgePoint, new_angle, 
                                    new_edge, LINE_LENGTH / 4, GazeConfig::STARBURST_EDGE_THRESHOLD);
                    
                    if(success)
                        points.push_back(new_edge);
                    
                    new_angle += angle;
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

		if (!points.empty()) {
			mean_x /= points.size();
			mean_y /= points.size();
		} else {
            //TODO report error?
			LOG_D("no mean calculated!");
            //mean_x = 0;
            //mean_y = 0;
			break;
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

	if(found){
		center = Point2f(x, y);
		radius = r;
	}

#if __DEBUG_STARBURST == 1
	Mat copy = gray.clone();
	for (std::vector<Point2f>::iterator it = points.begin(); it != points.end();
			++it) {
		cross(copy, *it, 3);
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
