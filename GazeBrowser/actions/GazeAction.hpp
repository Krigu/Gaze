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
#include <QtCore>

class GazeAction : public QObject {
    Q_OBJECT

public:
    GazeAction(std::string name, cv::Rect region, int prepareHits, int commitHits);
    virtual ~GazeAction();

    std::string getActionName() const;
    cv::Rect getRegion() const;

    void focus();
    void unfocus();
signals:
    void commitAction(cv::Point);
    void prepareAction(cv::Point,int);
    void abortAction();

private:
    std::string actionName;
    cv::Rect region;    
    int prepareHits;
    int commitHits;
    int hitCounter;

};

#endif	/* GAZEACTION_HPP */

