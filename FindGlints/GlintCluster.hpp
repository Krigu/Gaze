/*
 * GlintCluster.hpp
 *
 *  Created on: Nov 13, 2012
 *      Author: krigu
 */

#ifndef GLINTCLUSTER_HPP_
#define GLINTCLUSTER_HPP_

#include <opencv2/core/core.hpp>

class GlintCluster {

private:
	// TODO: There was a &
	std::vector<cv::Point> glints;

public:
	//TODO ask mr C++ if this is valid code
	GlintCluster(std::vector<cv::Point> & glints, cv::Point initialPoint) :
			glints(glints) {
	}
	;

	int averageDistance();
	cv::Point center();
// TODO: WTF?
	std::vector<cv::Point> const& glintsInCluster() const;          // Getter

};

#endif /* GLINTCLUSTER_HPP_ */
