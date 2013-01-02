/* 
 * File:   GazeAction.hpp
 * Author: krigu
 *
 * Created on January 2, 2013, 1:07 PM
 */

#ifndef GAZEACTION_HPP
#define	GAZEACTION_HPP

#include <string>

#include <opencv2/core/core.hpp>

class GazeAction {
public:
    GazeAction(std::string name, cv::Rect region, int minHits);
    virtual ~GazeAction();
    
    std::string getActionName() const;
    cv::Rect getRegion() const;
    
    void focus();
    void unfocus();
    void triggerAction();
private:
    std::string actionName;
    cv::Rect region;
    int hitCounter;
    int minHits;
};

#endif	/* GAZEACTION_HPP */

