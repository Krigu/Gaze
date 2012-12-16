/* 
 * File:   GazeExceptions.hpp
 * Author: krigu
 *
 * Created on December 7, 2012, 2:09 PM
 */

#ifndef GAZEEXCEPTIONS_HPP
#define	GAZEEXCEPTIONS_HPP

#include <exception>
#include <string>

class GazeException : public std::exception {
private:
    std::string errorMsg;

public:
    GazeException(std::string msg);
    ~GazeException() throw ();
    const char *what() const throw ();

};

class GlintNotFoundException : public GazeException {
public:
    GlintNotFoundException();

};

class PupilNotFoundException : public GazeException {
public:
    PupilNotFoundException();
};

class EyeRegionNotFoundException : public GazeException {
public:
    EyeRegionNotFoundException();
};

class FaceRegionNotFoundException : public GazeException {
public:
    FaceRegionNotFoundException();
};

class NoImageSourceException : public GazeException {
public:
    NoImageSourceException();
};

class WrongArgumentException : public GazeException {
public:
    WrongArgumentException(std::string msg);
    WrongArgumentException();
};
#endif	/* GAZEEXCEPTIONS_HPP */

