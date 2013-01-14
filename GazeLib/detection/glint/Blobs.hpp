#ifndef BLOBS_HPP_
#define BLOBS_HPP_

/**
 * Simple struct to represent a blob
 * with its center of gravity and the 
 * size in Pixels
 */
struct Blob {
	/// Size in pixels
	int size;
	/// Center of gravity X
	int centerX;
	/// Center of gravity Y
	int centerY;
};

/**
 * Container for a list of blobs.
 *
 * The blobs and their central moments are derived
 * from the contours provided in the constructor.
 * 
 */
class Blobs {
private:
	std::vector<Blob> blobs;
public:
    /**
     * Constructor which calculated the central moments
     * of the provided contours.
     * 
     * @param contours 
     */
	Blobs(std::vector<std::vector<cv::Point> > & contours);

    /**
     * 
     * Clears the provided points and adds the centers of gravity 
     * for each blob
     * 
     * @param points
     */
	void blobCenters(std::vector<cv::Point> & points);


};

#endif /* BLOBS_HPP_ */
