#include "GazeExceptions.hpp"
#include "../utils/log.hpp"

GazeException::GazeException(std::string msg) : errorMsg(msg) {
    LOG_W(msg);
}

GazeException::~GazeException() throw () {

}

const char * GazeException::what() const throw () {
    return errorMsg.c_str();
}

EyeRegionNotFoundException::EyeRegionNotFoundException() : GazeException("EyeRegion not found\nPlease check if the eye is still in the camera focus!") {

};

NoImageSourceException::NoImageSourceException() : GazeException("No image source \nPlease check whether the camera is connected to the system!") {

};

WrongArgumentException::WrongArgumentException(std::string msg) : GazeException(msg) {
    
}

WrongArgumentException::WrongArgumentException() : GazeException("This Argument-Type or Size is not supported!") {
    
}