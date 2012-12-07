/* 
 * File:   GazeExceptions.hpp
 * Author: krigu
 *
 * Created on December 7, 2012, 2:09 PM
 */

#ifndef GAZEEXCEPTIONS_HPP
#define	GAZEEXCEPTIONS_HPP

#include <exception>

class GazeException : public std::exception {
public:
    GazeException();
private:

};

class GlintNotFoundException : public GazeException{
public:
    GlintNotFoundException();

};

class PupilNotFoundException : public GazeException{
public:
    PupilNotFoundException();
};

class EyeRegionNotFoundException : public GazeException{
public:
    EyeRegionNotFoundException();
};

class FaceRegionNotFoundException : public GazeException{
public:
    FaceRegionNotFoundException();
};


class NoImageSourceException : public GazeException{
public:
    NoImageSourceException();
};
#endif	/* GAZEEXCEPTIONS_HPP */

