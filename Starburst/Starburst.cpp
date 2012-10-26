//#include <QString>
//#include <QDir>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>

#include "Starburst.hpp"
#include "utils/helper.hpp"

#define PI 3.14159265358979323846

using namespace cv;
using namespace std;

/**
 * finds the eye using haar, then create a rectangle for one eye
 * TODO: use a haar file, that tracks one eye
 */
void Starburst::setUp(cv::VideoCapture& capture) {
    CascadeClassifier cascade_eye;
    cascade_eye.load("/Users/fri/Dropbox/gaze/haar/parojosG.xml");

    Mat frame;

    // search for a face in the image and store the position of one eye
    bool faceFound = false;
    while (!faceFound) {
        capture.read(frame);

        vector<Rect> faces;
        cascade_eye.detectMultiScale(frame, faces,
                1.1, 2, 0
                //|CV_HAAR_FIND_BIGGEST_OBJECT
                //|CV_HAAR_DO_ROUGH_SEARCH
                | CV_HAAR_SCALE_IMAGE
                ,
                Size(30, 30));

        vector<Rect>::const_iterator r = faces.begin();
        for (int i = 0; r != faces.end(); r++, i++) {
            eye = cv::Rect(r->x, r->y, r->width / 2.5, r->height);
            faceFound = true;
            break;
        }
    }

    namedWindow("StarBurstResult");
    namedWindow("GlintsRemoved");
}

/*
 * idea: only use the precalculated rectangle.
 * - remove the glints, 
 * - use min to find a point in the pupil
 * - use starburst to find the pupil and its center
 * 
 */
void Starburst::processImage(cv::Mat& frame) {
    // get our working area of the image
    Mat img = frame(eye);
    Mat gray = img;

    // mark the position on the image
    Scalar color = Scalar(0, 255, 0);
    rectangle(frame, eye, color, 2, 8, 0);

    cvtColor(img, gray, CV_BGR2GRAY);

    //TODO: don't overwrite the last_center at every step...

    //if (last_center.x == 0 && last_center.y == 0) {
    // find the darkest point. it's probably inside the pupil
    double minVal = 0;
    double maxVal = 0;
    Point minLoc;
    Point maxLoc;

    minMaxLoc(gray, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    last_center = minLoc;
    //}

    // draw a circle around the darkest point
    //circle(img, last_center, 1, color);

    // let's guess the pupil size and only remove the glints inside it
    Rect pupil = Rect(last_center.x - 10, last_center.y - 10, 20, 20);
    //rectangle(img, pupil, Scalar(0, 0, 255), 2, 8, 0);

    // the algorithm: blur image, remove glint and starburst
    medianBlur(gray, gray, 3);
    remove_glints(gray, pupil, 2, 4);
    float radius = 0;
    starburst(gray, last_center, radius, 20, 1);

    // display the center on the source image
    Point absolute_center = Point(last_center.x + eye.x, last_center.y + eye.y);
    cross(frame, absolute_center, 4, Scalar(0, 0, 255));
    circle(frame, absolute_center, radius, Scalar(0, 0, 255));

    // display our working area
    imshow("StarBurstResult", gray);
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
void Starburst::remove_glints(Mat& gray, Rect& pupil_area, short num_of_glints, short interpolation_size) {

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
        return remove_glints(gray, pupil_area, --num_of_glints, interpolation_size);
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
        float top_row = (cols - current_col) * fraction * (float) gray.at<uchar > (glint.y - interpolation_size, glint.x - interpolation_size);
        top_row += current_col * fraction * (float) gray.at<uchar > (glint.y - interpolation_size, glint.x + interpolation_size);
        float bottom_row = (cols - current_col) * fraction * (float) gray.at<uchar > (glint.y + interpolation_size, glint.x - interpolation_size);
        bottom_row += current_col * fraction * (float) gray.at<uchar > (glint.y + interpolation_size, glint.x + interpolation_size);

        // if there are two glints, don't interpolate the bright pixels...
        // let's darken it
        if (top_row > minVal + 10)
            top_row = minVal + 10;
        if (bottom_row > minVal + 10)
            bottom_row = minVal + 10;

        current_col++;

        // interpolate the values 
        for (signed short j = -interpolation_size; j <= interpolation_size; j++) {
            uchar interpolated = (cols - current_row) * fraction * top_row;
            interpolated += current_row * fraction * bottom_row;
            gray.at<uchar > (glint.y + j, glint.x + i) = interpolated;
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
void Starburst::starburst(cv::Mat &gray, Point2f &center, float &radius, int num_of_lines, int distance_growth) {
    const float angle = 2 * PI / num_of_lines; // in radiants!
    const Scalar color = Scalar(255, 255, 255);

    // the intensity of the point that is inside the pupil
    uchar start_val = gray.at<uchar > (center.y, center.x);

    std::vector<Point> points;

    // calculate the lines in every direction
    for (unsigned short i = 0; i < num_of_lines; i++) {
        // calculate the current degree
        const float current_angle = angle * i;

        // initialize the distance
        int distance = distance_growth;

        // follow in this direction and increase the distance
        while (true) {
            int x = center.x + cos(current_angle) * distance;
            int y = center.y + sin(current_angle) * distance;

            // get the intensity at the current position
            int val = gray.at<uchar > (y, x);


            //            if (val > 100) {
            //                break;
            //            } else if (val > 40) {

            // if the intensity made a jump, its the end of the pupil
            if (val > start_val + 10) { // TODO: +10 is probably a hack
                Point p = Point(x, y);
                if (val - start_val <= 30) {
                    points.push_back(p);
                }
                break;
            }

            // if nothing was found, increase the line length
            distance += distance_growth;
        }
    }

    Ransac ransac;
    float x, y, r;
    x = y = r = 0;
    ransac.ransac(&x, &y, &r, points);

    // find the radius and the circle center
    minEnclosingCircle(points, center, radius);
    cout << "Size:" << points.size() << endl;

//    center = Point2f(x,y);
//    radius = r;
    
    for (std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
        cross(gray, *it, 3);
        //line(gray, start, *it, color);
        //circle(gray, *it, 1, color);
    }
}
