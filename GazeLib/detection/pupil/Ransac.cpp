#if __DEBUG_STARBURST == 1
#include <opencv2/highgui/highgui.hpp>
#endif

#include "Ransac.hpp"
#include "../../config/GazeConfig.hpp"
#include "../../utils/geometry.hpp"
#include "../../utils/gui.hpp"
#include "../../utils/log.hpp"


//
// algorithm:
// - chose 3 random points
// - fit circle
// - count points within circle +/- a chosen "T" distance
// - repeat above steps N times
// - use the circle which had the most points within the
//   range
// - search for the circle who fits these circles best
//
bool Ransac::ransac(float * x, float * y, float * radius,
		std::vector<cv::Point2f> points) {
	// N: num of iterations
	const int N = GazeConfig::RANSAC_ITERATIONS;
	// T: distance in which
	const float T = 2;

	bool found = false;

	if(points.size() < 3)
		return found;

	// initialize randomizer
	srand(time(NULL));

    std::vector<cv::Point2f> points_in_range;

#if __DEBUG_STARBURST == 1
	cv::Mat debug_result;
#endif

	// lets fit a circle into 3 random points
	// after N iterations the circle that fitted
	// the most points is chosen as the result
	for (int i = 0; i < N; i++) {
		float tmp_x, tmp_y, tmp_r;
		tmp_x = tmp_y = tmp_r = 0;
		//int points_within_range = 0;
        std::vector<cv::Point2f> curr_points_in_range;

		// fit a random circle
		std::random_shuffle(points.begin(), points.end());
		fitCircle(&tmp_x, &tmp_y, &tmp_r, points);

#if __DEBUG_STARBURST == 1
		cv::Mat debug = cv::Mat::zeros(300, 300, CV_8UC3);
		cv::Point a = points.at(0);
		cv::Point b = points.at(1);
		cv::Point c = points.at(2);

		cross(debug, a, 5, cv::Scalar(255, 0, 0));
		cross(debug, b, 5, cv::Scalar(255, 0, 0));
		cross(debug, c, 5, cv::Scalar(255, 0, 0));

		circle(debug, cv::Point(tmp_x, tmp_y), tmp_r, cv::Scalar(255, 255, 255));
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
				curr_points_in_range.push_back(*it);
#if __DEBUG_STARBURST == 1
				cross(debug, *it, 5, cv::Scalar(0, 255, 0));
#endif
			} else {
#if __DEBUG_STARBURST == 1
				cross(debug, *it, 5, cv::Scalar(0, 0, 255));
#endif
			}

		}

		if (curr_points_in_range.size() > points_in_range.size()) {
			*x = tmp_x;
			*y = tmp_y;
			*radius = tmp_r;
            points_in_range.clear();
            points_in_range.insert(points_in_range.end(), 
                    curr_points_in_range.begin(), curr_points_in_range.end());
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
    
    // now calculate the circle who fits the points best
    if(found){
        bestFitCircle(x, y, radius, points_in_range);
        
#if __DEBUG_STARBURST == 1
        LOG_D("BestFitCircle: x=" << *x << " y=" << *y << " R=" << *radius);
#endif
    }
    
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
}
