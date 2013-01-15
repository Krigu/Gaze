#include <iostream>
#include <math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "FindGlints.hpp"
#include "GlintCluster.hpp"
#include "Blobs.hpp"
#include "../../utils/log.hpp"
#include "../../utils/geometry.hpp"
#include "../../utils/gui.hpp"
#include "../../config/GazeConfig.hpp"

#if __DEBUG_FINDGLINTS == 1
#include "opencv2/highgui/highgui.hpp"
#endif

using namespace std;
using namespace cv;

bool FindGlints::findGlints(cv::Mat& frame, vector<cv::Point>& glintCenters,
        cv::Point2f& lastMeasurement) {

    Mat img;

#if __DEBUG_FINDGLINTS == 1
    imshow("Image before threshold", frame);
#endif

    // Threshold image.
    threshold(frame, img, GazeConfig::GLINT_THRESHOLD, 255,
            cv::THRESH_TOZERO);

#if __DEBUG_FINDGLINTS == 1
    imshow("Original image", frame);
    imshow("Thresholded image", img);
#endif

    // Dilate the blobs. Sometimes a glint is just one small pixel
    dilate(img, img, Mat::ones(2, 2, CV_8U));

    std::vector<std::vector<cv::Point> > contours;

    vector<Vec4i> hierarchy;
    // Find all countours
    findContours(img, contours, // a vector of contours
            CV_RETR_EXTERNAL, // retrieve the external contours
            CV_CHAIN_APPROX_NONE); // all pixels of each contours

#if __DEBUG_FINDGLINTS == 1
    /// Draw contours
    RNG rng(12345);
    Mat drawing = Mat::zeros(img.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
                rng.uniform(0, 255));
        drawContours(drawing, contours, i, color, 2, 8);
    }
    imshow("Contours: ", drawing);
#endif

    LOG_D("Countours size: " << contours.size());
    Blobs blobs = Blobs(contours);
    blobs.blobCenters(glintCenters);
    LOG_D("Blobs size: " << glintCenters);

#if __DEBUG_FINDGLINTS == 1
    Mat glints = Mat::zeros(img.size(), CV_8UC3);
    std::vector<Point>::iterator iter;
    for (iter = glintCenters.begin(); iter != glintCenters.end(); ++iter) {
        cross(glints, *iter, 5);
    }
    imshow("Glints", glints);
#endif

    // Find all clusters
    vector<GlintCluster> clusters;
    findClusters(glintCenters, clusters, lastMeasurement);

    if (clusters.empty()) {
        return false;
    } else if (clusters.size() > 1) {
        sort(clusters.begin(), clusters.end());
    }


    // Adjust glint distance with current measurement
    // To make sure that the diagonals are also considered, the width of the cluster are multiplied with SQR(2)
    GazeConfig::GLINT_DISTANCE = 0.8 * GazeConfig::GLINT_DISTANCE + 0.2 * (clusters.at(0).width() * 1.4);

    glintCenters = clusters.at(0).glintsInCluster();
    lastMeasurement = clusters.at(0).centerPoint();

    return true;

}

/**
 * Creates a matrix. The element is 1, if the distance between two blobs is in the configurated range
 */
cv::Mat FindGlints::distanceMatrix(vector<cv::Point>& glintCenter) {
    // Create identity matrix

    // Amount of glints
    int n = glintCenter.size();

    // Create identity matrix
    Mat distanceMat = Mat::eye(n, n, CV_8U);

    // Add all points to matrix where a adjacent point is in Glint distance range
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            //for (int j = 0; j < n; j++) {
            int dist = calcPointDistance(&glintCenter.at(i), &glintCenter.at(j));
            // Consider only nighbours within distance
            // There is a tolerance added as well
            if (dist >= (GazeConfig::GLINT_DISTANCE - GazeConfig::GLINT_DISTANCE_TOLERANCE)
                    && dist <= (GazeConfig::GLINT_DISTANCE + GazeConfig::GLINT_DISTANCE_TOLERANCE)) {
                distanceMat.at<char>(i, j) = 1;
            }
        }
    }

    return distanceMat;
}

/**
 * Removes all blobs with less than 3 neigbors
 */
void FindGlints::findClusters(vector<cv::Point>& blobs,
        vector<GlintCluster>& clusters, cv::Point2f& lastMeasurement) {

    // Get the distance Matrix
    Mat nighbourMat = distanceMatrix(blobs);

    LOG_D("DistanceMat out: " << endl << " " << nighbourMat << endl);

    // Calculate number of nighbors per row
    Mat column_sum;
    reduce(nighbourMat, column_sum, 1, CV_REDUCE_SUM, CV_32S);
    LOG_D("Sum: " << column_sum);

    // Principle idea: Iterate over nighborMat and only consider lines
    // with at least 4 glints (3 nightbours). Those line creates a new cluster of glints.
    for (int row = 0; row < nighbourMat.rows; ++row) {
        uchar* p = nighbourMat.ptr(row);
        if (column_sum.at<char>(row, 0) >= GazeConfig::GLINT_COUNT) {
            std::vector<cv::Point> glints;
            for (int col = 0; col < nighbourMat.cols; ++col) {
                if (*p++ == 1) {
                    glints.push_back(blobs.at(col));
                }
            }
            // Check if glintcluster has a blobs which are rectangular aligned
            if (findRectangularCluster(glints)) {
                GlintCluster glintCluster(glints, lastMeasurement);
                clusters.push_back(glintCluster);
            }
        }
    }

}

/**
 * http://compprog.wordpress.com/2007/10/17/generating-combinations-1/
 * 
 * next_comb(int comb[], int k, int n)
 *       Generates the next combination of n elements as k after comb
 *
 * comb => the previous combination ( use (0, 1, 2, ..., k) for first)
 * k => the size of the subsets to generate
 * n => the size of the original set
 *
 *   Returns: 1 if a valid combination was found
 *      0, otherwise
 * 
 */
int next_comb(int comb[], int k, int n) {
    int i = k - 1;
    ++comb[i];
    while ((i > 0) && (comb[i] >= n - k + 1 + i)) {
        --i;
        ++comb[i];
    }

    if (comb[0] > n - k) /* Combination (n-k, n-k+1, ..., n) reached */
        return 0; /* No more combinations can be generated */

    /* comb now looks like (..., x, n, n, n, ..., n).
    Turn it into (..., x, x + 1, x + 2, ...) */
    for (i = i + 1; i < k; ++i)
        comb[i] = comb[i - 1] + 1;

    return 1;
}

bool FindGlints::findRectangularCluster(vector<cv::Point>& glints) {
    bool hasRectangularAlignment = false;

    // Less than 4 glints, so now rectangular structure is possible
    if (glints.size() < 4)
        return false;

    int n = glints.size();
    int k = 4; // The size of the subsets; for {1, 2}, {1, 3}, ... it's 2 
    int comb[n]; // comb[i] is the index of the i-th element in the combination

    // Setup comb for the initial combination 
    int i;
    for (i = 0; i < k; ++i)
        comb[i] = i;

    // Try first combination
    vector<cv::Point> quadruple;
    quadruple.push_back(glints.at(comb[0]));
    quadruple.push_back(glints.at(comb[1]));
    quadruple.push_back(glints.at(comb[2]));
    quadruple.push_back(glints.at(comb[3]));
    hasRectangularAlignment = isRectangle(quadruple, GazeConfig::GLINT_ANGLE_TOLERANCE);

    // Generate and print all the other combinations
    while (!hasRectangularAlignment && next_comb(comb, k, n)) {
        quadruple.clear();
        quadruple.push_back(glints.at(comb[0]));
        quadruple.push_back(glints.at(comb[1]));
        quadruple.push_back(glints.at(comb[2]));
        quadruple.push_back(glints.at(comb[3]));
        hasRectangularAlignment = isRectangle(quadruple, GazeConfig::GLINT_ANGLE_TOLERANCE);
    }

    LOG_D("isRect: " << hasRectangularAlignment);

    if (hasRectangularAlignment) {
        glints = quadruple;
    }

    return hasRectangularAlignment;
}
