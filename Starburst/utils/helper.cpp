#include "helper.hpp"
#include <limits>>

cv::Rect createRect(cv::Mat &frame, cv::Point &position, int width, int height) {

    // make sure, that the rectangle stays within the image boundaries
    position.x = position.x >= 0 ? position.x : 0;
    position.y = position.y >= 0 ? position.y : 0;
    width = position.x + width > frame.cols ? frame.cols - position.x : width;
    width = position.y + height > frame.rows ? frame.rows - position.y : height;

    return cv::Rect(position.x, position.y, width, height);
}

void cross(cv::Mat & im, cv::Point &center, short size, cv::Scalar color) {
    cv::line(im, cv::Point(center.x - size, center.y), cv::Point(center.x + size, center.y), color);
    cv::line(im, cv::Point(center.x, center.y - size), cv::Point(center.x, center.y + size), color);
}

void cross(cv::Mat & im, cv::Point &center, short size) {
    cross(im, center, size, cv::Scalar(255, 255, 255));
}

// the RANSAC stuff:


// 
// algorithm: 
// - chose 3 random points
// - fit circle
// - count points within circle +/+ a chosen "t
// - repeate above steps N times
// 

void Ransac::ransac(float * x, float * y, float * radius, std::vector<cv::Point> points) {
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

void Ransac::fitCircle(float * x, float * y, float * radius, std::vector<cv::Point> points) {
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