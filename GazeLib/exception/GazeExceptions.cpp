/* 
 * File:   GazeExceptions.cpp
 * Author: krigu
 * 
 * Created on December 7, 2012, 2:09 PM
 */

#include "GazeExceptions.hpp"

GazeException::GazeException(std::string msg) : errorMsg(msg) {
}

GazeException::~GazeException() throw () {

}

const char * GazeException::what() const throw () {
    return errorMsg.c_str();
}

GlintNotFoundException::GlintNotFoundException() : GazeException("Glint not found\nPlease check if the eye is still in the camera focus!") {

};

PupilNotFoundException::PupilNotFoundException() : GazeException("Pupil not found\nPlease check if the eye is still in the camera focus!") {

};

EyeRegionNotFoundException::EyeRegionNotFoundException() : GazeException("EyeRegion not found\nPlease check if the eye is still in the camera focus!") {

};

NoImageSourceException::NoImageSourceException() : GazeException("No image source \nPlease check whether the camera is connected to the system!") {

};

