/*
 * GazeConstants.hpp
 *
 *  Created on: Nov 9, 2012
 *      Author: fri
 */

#ifndef GAZECONSTANTS_HPP_
#define GAZECONSTANTS_HPP_

#include <string>

class GazeConstants {
public:
    
    //
    // global constants
    //
    
	// Amounts of glints (per eye)
	static const char GLINT_COUNT = 4;

	static const unsigned short MAX_RANSAC_ITERATIONS = 10;

	// how many frames should we use to smooth the signal?
	static const unsigned short NUM_OF_SMOOTHING_FRAMES = 5;
    
	static const short GLINT_RADIUS = 6;

	//
	// Glints Configuration
	//
    
    // fasch ds wichtigschte
	static double GLINT_THRESHOLD;
    
	static short GLINT_MIN_DIAMETER;
	static short GLINT_MAX_DIAMETER;
	
    // TODO calculate from diameter
	static short GLINT_MIN_PIXEL;
	// TODO calculate from diameter
	static short GLINT_MAX_PIXEL;
	// Min distance between two glints
	static short GLINT_MIN_DISTANCE;
	// Max distance between two glints
	static short GLINT_MAX_DISTANCE;

	//
	// Haar configuration
	//

	// Max height of eyeregion
	static short HAAR_EYEREGION_MAX_HEIGHT;
	// Max width of eyeregion
	static short HAAR_EYEREGION_MAX_WIDTH;
	// Max height of eye
	static short HAAR_EYE_MIN_HEIGHT;
	// Max width of eye
	static short HAAR_EYE_MIN_WIDTH;
	// Max height of eye
	static short HAAR_EYE_MAX_HEIGHT;
	// Max width of eye
	static short HAAR_EYE_MAX_WIDTH;
	// Max amount of tries to find eye region
	static short HAAR_FINDREGION_MAX_TRIES;

    //
	// RANSAC stuff
	//
    static short PUPIL_MIN_RADIUS;
	static short PUPIL_MAX_RADIUS;

	GazeConstants(){
        initConiguration();
    }

    ~GazeConstants(){
        
    }
    
    void initConiguration(){
        
        //
        // Glints Configuration
        //

        GazeConstants::GLINT_THRESHOLD = 230;
        GazeConstants::GLINT_MIN_DIAMETER = 2;
        GazeConstants::GLINT_MAX_DIAMETER = 6;

        //static const short GLINT_RADIUS = 6;

        GazeConstants::GLINT_MIN_PIXEL = 1;
        GazeConstants::GLINT_MAX_PIXEL = 50;
        GazeConstants::GLINT_MIN_DISTANCE = 1;
        GazeConstants::GLINT_MAX_DISTANCE = 40;
        
        //
        // Haar configuration
        //

        GazeConstants::HAAR_EYEREGION_MAX_HEIGHT = 100;
        GazeConstants::HAAR_EYEREGION_MAX_WIDTH = 400;
        GazeConstants::HAAR_EYE_MIN_HEIGHT = 20;
        GazeConstants::HAAR_EYE_MIN_WIDTH = 20;
        GazeConstants::HAAR_EYE_MAX_HEIGHT = 200;
        GazeConstants::HAAR_EYE_MAX_WIDTH = 200;
        GazeConstants::HAAR_FINDREGION_MAX_TRIES = 10;
        
        //
        // RANSAC stuff
        //
        GazeConstants::PUPIL_MIN_RADIUS = 15;
        GazeConstants::PUPIL_MAX_RADIUS = 17;
    }
    
	/**
	 * returns a path to the users homedirectory
	 *
	 * you may add a suffix for example to point directly to a file
	 */
	static std::string inHomeDirectory(std::string suffix);
};

static GazeConstants GAZE_CONSTANTS;

#endif /* GAZECONSTANTS_HPP_ */
