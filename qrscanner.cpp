#include "qrscanner.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <zbar.h>
#include <mutex>
//#include "matplotlibcpp.h"
#include "qrscanner.h"

using namespace std;
using namespace cv;
using namespace zbar;

QRScanner::QRScanner() :
    _locations{}
{
  //  cv::namedWindow("window");
    // configure the reader
    _scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
}


std::pair < int, int > QRScanner::find(std::string locationName) {
  auto pos = std::make_pair(int(-1), int(-1));
  std::vector < Location > ::reverse_iterator revIt;
  _locMtx.lock();
  for (revIt = _locations.rbegin(); revIt != _locations.rend(); revIt++) {
    if (revIt->name == locationName) {
      pos = revIt->position;
      break;
    }
  }
  _locMtx.unlock();
  return pos;
}

void QRScanner::scan(cv::Mat &frame)
{
    // obtain image data
    cv::Mat imGray;


    cv::cvtColor(frame, imGray,CV_BGR2GRAY);
    _locations.clear();
    // wrap image data
    Image image(imGray.cols, imGray.rows, "Y800", imGray.data, imGray.cols * imGray.rows);

    // scan the image for barcodes
    int decodeStatus=_scanner.scan(image);

    if(decodeStatus == 1){
      for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
    	  _locMtx.lock();
        Location location;

        location.name = symbol->get_data();
        for (int i = 0; i < symbol->get_location_size(); i++) {
          auto location_x = 160 - symbol-> get_location_x(i)/2;
          auto location_y = 120 - symbol-> get_location_y(i)/2;
          location.position = (make_pair(location_x, location_y));
        }
        _locations.push_back(Location {
          location
        });
        _locMtx.unlock();
      }
    }

    else{
      //convert frames to canny frames
      Mat canny_frame;
      Canny(imGray, canny_frame, 50, 100, 3);
      Mat pattern = imread("/home/pi/cam-controller/TU.jpg", IMREAD_GRAYSCALE);

      //convert template to canny form
      Canny(pattern, pattern, 50, 100, 3);

      //Initialization of resultant frame for template matching
      int result_cols = canny_frame.cols - pattern.cols + 1;
      int result_rows = canny_frame.rows - pattern.rows + 1;
      Mat result;
      result.create(result_rows, result_cols, CV_32FC1);

      //match Template and normalize
      matchTemplate(canny_frame, pattern, result, CV_TM_CCORR);
      normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

      //localize max and min values in the resultant frame
      double minVal;
      double maxVal;
      Point minLoc;
      Point maxLoc;
      Point matchLoc;
      minMaxLoc(result, & minVal, & maxVal, & minLoc, & maxLoc, Mat());

      matchLoc = maxLoc;
      _locMtx.lock();
      Location location;
      auto location_x = 160 - (matchLoc.x + pattern.cols / 2);
      auto location_y = 120 - (matchLoc.y + pattern.rows / 2);
      location.position = make_pair(location_x,location_y);
      location.name = "Not close enough";
    //  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

      //push location in
      _locations.push_back(location);
      _locMtx.unlock();
    }

    /*
    // extract results
    _locMtx.lock();
    _locations.clear();
    for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {

        int x = 0; int y = 0; int n = 0;

        for (auto it = symbol->point_begin(); it != symbol->point_end(); ++it) {
            x += (*it).x;
            y += (*it).y;
            n++;
        }

        _locations.push_back(Location{symbol->get_data(), make_pair(x/n,y/n)});
    }
    _locMtx.unlock();
    image.set_data(nullptr, 0);
    */
    //imshow("window", frame);
    cv::waitKey(30);
}
