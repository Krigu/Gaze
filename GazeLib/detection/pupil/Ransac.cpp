
#include "Ransac.hpp"
#include "../../config/GazeConfig.hpp"


// the RANSAC stuff:

//
// algorithm:
// - chose 3 random points
// - fit circle
// - count points within circle +/+ a chosen "t
// - repeat above steps N times
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

		//only accept pupil sizes within a given range
		if (tmp_r < GazeConfig::PUPIL_MIN_RADIUS
				|| tmp_r > GazeConfig::PUPIL_MAX_RADIUS)
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
