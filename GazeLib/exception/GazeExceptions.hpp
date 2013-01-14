#ifndef GAZEEXCEPTIONS_HPP
#define	GAZEEXCEPTIONS_HPP

#include <exception>
#include <string>

/**
 *
 * GazeException is the parent class of all Exceptions thrown in GazeLib.
 * when a GazeExceptino is thrown, the reason is ALWAYS printed to stdout
 */
class GazeException : public std::exception {
private:
    std::string errorMsg;

public:
    GazeException(std::string msg);
    ~GazeException() throw ();
    const char *what() const throw ();

};

/**
 * EyeRegionNotFoundException is thrown when within HAAR_FINDREGION_MAX_TRIES
 * tries no eye region has been found. @see HAAR_FINDREGION_MAX_TRIES
 */
class EyeRegionNotFoundException : public GazeException {
public:
    EyeRegionNotFoundException();
};

/**
 * NoImageSourceException is thrown when a LiveSource or VideoSource is opened
 * with the wrong parameters. this could be an non existent video file
 * or a wrong channel for an usb camera.
 */
class NoImageSourceException : public GazeException {
public:
    NoImageSourceException();
};

/**
 * WrongArgumentExcepion means that a client of GazeLib misused one of the API calls.
 */
class WrongArgumentException : public GazeException {
public:
    WrongArgumentException(std::string msg);
    WrongArgumentException();
};
#endif	/* GAZEEXCEPTIONS_HPP */

