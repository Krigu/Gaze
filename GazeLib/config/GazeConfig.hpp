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
    // Max amount of ransac iterations
    static const unsigned int MAX_RANSAC_ITERATIONS = 10;
    // how many frames should we use to smooth the signal?
    static const unsigned int NUM_OF_SMOOTHING_FRAMES = 5;
    // Estimated size ot glint radius
    static const int GLINT_RADIUS = 8;

    //
    // Starburst 
    //
    static int STARBURST_EDGE_THRESHOLD;
    
    //
    // Glints Configuration
    //

    // fasch ds wichtigschte
    static int GLINT_THRESHOLD;
    // Min distance between two glints
    static int GLINT_DISTANCE_TOLERANCE;
    // Max distance between two glints
    static int GLINT_DISTANCE;
    // Max difference between barycentre
    static int GLINT_ANGLE_TOLERANCE;

    //
    // Haar configuration
    //

    // Max height of eyeregion
    static int HAAR_EYEREGION_MIN_HEIGHT;
    // Max width of eyeregion
    static int HAAR_EYEREGION_MIN_WIDTH;
    // Max amount of tries to find eye region
    static int HAAR_FINDREGION_MAX_TRIES;

    // takes the left eye (on screen) if value is true, right eye otherweise
    static bool DETECT_LEFT_EYE;

    GazeConfig() {
        initConiguration();
    }

    ~GazeConfig() {

    }

    void initConiguration() {

        //
        // Starburst
        //
        GazeConfig::STARBURST_EDGE_THRESHOLD = 30;
        
        //
        // Glints Configuration
        //

        GazeConfig::GLINT_THRESHOLD = 220;
        GazeConfig::GLINT_DISTANCE_TOLERANCE = 20;
        GazeConfig::GLINT_DISTANCE = 25;

        // TODO: Implement in settings window
        GazeConfig::GLINT_ANGLE_TOLERANCE = 10;

        //
        // Haar configuration
        //
        GazeConfig::HAAR_EYEREGION_MIN_HEIGHT = 200;
        GazeConfig::HAAR_EYEREGION_MIN_WIDTH = 600;
        GazeConfig::HAAR_FINDREGION_MAX_TRIES = 10;

        // General Settings
        GazeConfig::DETECT_LEFT_EYE = true;

    }

    /**
     * returns a path to the users homedirectory
     *
     * you may add a suffix for example to point directly to a file
     */
    static std::string inHomeDirectory(std::string suffix);
    
    
    static std::string inWorkingDir(std::string suffix);

};

// create an instance and make sure, that "initConfiguration()" is run once
static GazeConfig GAZE_CONSTANTS;

#endif /* GAZECONFIG_HPP_ */
