/*
 * GazeConfig.hpp
 *
 *  Created on: Nov 9, 2012
 *      Author: fri
 */

#ifndef GAZECONFIG_HPP_
#define GAZECONFIG_HPP_

#include <string>

class GazeConfig {
public:

    //
    // global constants
    //

    // Amounts of glints (per eye)
    static const char GLINT_COUNT = 4;

    static const unsigned int MAX_RANSAC_ITERATIONS = 10;

    // how many frames should we use to smooth the signal?
    static const unsigned int NUM_OF_SMOOTHING_FRAMES = 5;

    static const int GLINT_RADIUS = 6;

    //
    // Glints Configuration
    //

    // fasch ds wichtigschte
    static int GLINT_THRESHOLD;

    static int GLINT_MIN_DIAMETER;
    static int GLINT_MAX_DIAMETER;

    // TODO calculate from diameter
    static int GLINT_MIN_PIXEL;
    // TODO calculate from diameter
    static int GLINT_MAX_PIXEL;
    // Min distance between two glints
    static int GLINT_MIN_DISTANCE;
    // Max distance between two glints
    static int GLINT_MAX_DISTANCE;
    // Max difference between barycentre
    static int GLINT_ANGLE_TOLERANCE;

    //
    // Haar configuration
    //

    // Max height of eyeregion
    static int HAAR_EYEREGION_MIN_HEIGHT;
    // Max width of eyeregion
    static int HAAR_EYEREGION_MIN_WIDTH;
    // Max height of eye
    static int HAAR_EYE_MIN_HEIGHT;
    // Max width of eye
    static int HAAR_EYE_MIN_WIDTH;
    // Max height of eye
    static int HAAR_EYE_MAX_HEIGHT;
    // Max width of eye
    static int HAAR_EYE_MAX_WIDTH;
    // Max amount of tries to find eye region
    static int HAAR_FINDREGION_MAX_TRIES;

    // takes the left eye (on screen) if value is true, right eye otherweise
    static bool DETECT_LEFT_EYE;
    // tries to detect the iris if true, otherwise the pupil
    static bool DETECT_IRIS;

    GazeConfig() {
        initConiguration();
    }

    ~GazeConfig() {

    }

    void initConiguration() {

        //
        // Glints Configuration
        //

        GazeConfig::GLINT_THRESHOLD = 230;
        GazeConfig::GLINT_MIN_DIAMETER = 2;
        GazeConfig::GLINT_MAX_DIAMETER = 6;

        //static const int GLINT_RADIUS = 6;

        GazeConfig::GLINT_MIN_PIXEL = 25;
        GazeConfig::GLINT_MAX_PIXEL = 160;
        GazeConfig::GLINT_MIN_DISTANCE = 1;
        GazeConfig::GLINT_MAX_DISTANCE = 100;

        // TODO: Implement in settings window
        GazeConfig::GLINT_ANGLE_TOLERANCE = 10;

        //
        // Haar configuration
        //
        GazeConfig::HAAR_EYEREGION_MIN_HEIGHT = 200;
        GazeConfig::HAAR_EYEREGION_MIN_WIDTH = 600;
        GazeConfig::HAAR_EYE_MIN_HEIGHT = 100;
        GazeConfig::HAAR_EYE_MIN_WIDTH = 150;
        GazeConfig::HAAR_EYE_MAX_HEIGHT = 500;
        GazeConfig::HAAR_EYE_MAX_WIDTH = 700;
        GazeConfig::HAAR_FINDREGION_MAX_TRIES = 10;

        // General Settings
        GazeConfig::DETECT_LEFT_EYE = true;
        GazeConfig::DETECT_IRIS = true;

    }

    /**
     * returns a path to the users homedirectory
     *
     * you may add a suffix for example to point directly to a file
     */
    static std::string inHomeDirectory(std::string suffix);
};

// create an instance and make sure, that "initConfiguration()" is run once
static GazeConfig GAZE_CONSTANTS;

#endif /* GAZECONFIG_HPP_ */
