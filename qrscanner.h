#ifndef QRSCANNER_H
#define QRSCANNER_H

#include <utility>
#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <zbar.h>

using namespace std;
using namespace cv;
using namespace zbar;
/** @addtogroup CamController
 * @{
 */

/** Name and relative position of a detected code*/
struct Location {
    std::string name;
    Point position;
};

/**
 * @brief QR Scanner
 * 
 * Scans images for the presence of QR codes
 * and stores detected codes in a vector
 */
class QRScanner
{
public:
    /** Class constructor */
    QRScanner();
    
    /** 
     * @brief Searches the location in the list of detected location
     * @param locationName name of the location (encoded data)
     * @return position of the location ( (-1,-1) if the location has not been found)
     */
    std::pair<int,int> find(std::string locationName);
    
    /**
     * @brief Detect codes presented in the image
     * @param frame image to be scanned
     * @note Each time the function is called
     * the vector of detected codes is reset.
     */
    void scan(cv::Mat &frame, const vector<string>& locationlist);

private:
    std::vector<Location> _locations;
    std::mutex _locMtx;
   zbar::ImageScanner _scanner;
};

/** @} */

#endif // QRSCANNER_H