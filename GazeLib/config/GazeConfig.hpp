#ifndef GAZECONFIG_HPP_
#define GAZECONFIG_HPP_

#include <string>

/**
 * the GazeConfig class provides the config-variables and constants for the 
 * Gaze Tracking. There are also helper methods for accessing files and directories
 * in and around the project directory.
 * <br />
 * if the GazeLib cannot find either the pupil or the glints you probably need
 * to check the constants in this class.
 */
class GazeConfig {
public:

    //
    // global constants
    //

    /// The total amount of glints (per eye)
    static const char GLINT_COUNT = 4;
    /// The maximum amount of tries to find the pupil using the starburst algorithm
    static const unsigned int MAX_STARBURST_ITERATIONS = 10;
    /// The number of RANSAC iterations per frame (and pupil)
    static const unsigned int RANSAC_ITERATIONS = 400;
    /// the number of frames used to smooth the measured gaze signal
    static const unsigned int NUM_OF_SMOOTHING_FRAMES = 5;
    /// the glint size used for removing them in the starburst algorithm
    static const int GLINT_RADIUS = 8;

    //
    // Starburst 
    //
    /// the intensity threshold at which the starburst algorithm accepts an edge
    static int STARBURST_EDGE_THRESHOLD;
    
    //
    // Glints Configuration
    //

    /// the threshold used for binarizing the glint images
    static int GLINT_THRESHOLD;
    /// the tolerance within two glints are accepted as neighbors
    /// @see GLINT_DISTANCE
    static int GLINT_DISTANCE_TOLERANCE;
    /// the maximum distance between two glints to be accepted as neighbors
    static int GLINT_DISTANCE;
    /// the accepted angle tolerance between glint to be accepted as a rectangle
    static int GLINT_ANGLE_TOLERANCE;

    //
    // Haar configuration
    //

    /// the minimum height of the eye region (depends on the hardware and zoom)
    static int HAAR_EYEREGION_MIN_HEIGHT;
    /// the minimum width of the eye region (depends on the hardware and zoom)
    static int HAAR_EYEREGION_MIN_WIDTH;
    /// the maximum number of tries to finde the eye region
    /// @see EyeRegionNotFoundException
    static int HAAR_FINDREGION_MAX_TRIES;

    /// configures whether the gaze tracking should measure the left or right eye
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
     * @param suffix filename or directory
     * @return the path to the homedirectory including the suffix
     */
    static std::string inHomeDirectory(std::string suffix);
    
    /**
     * returns a path to this applications working directory. 
     * the working directory depends on the directory where the application 
     * has been started
     * 
     * @param suffix an optional filename or directory name
     * @return the path to the applications working dir including suffix
     */
    static std::string inWorkingDir(std::string suffix);

};

// create an instance and make sure, that "initConfiguration()" is run once
static GazeConfig GAZE_CONSTANTS;

#endif /* GAZECONFIG_HPP_ */
