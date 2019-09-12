/** Implementation file for image enhancement using histogram matching
 *
 *  \file ipcv/histogram_enhancement/MatchingLut.cpp
 *  \author Aislinn Campbell (aec3160@rit.edu)
 *  \date 09 Sep 2019
 */

#include "MatchingLut.h"

#include <iostream>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Create a 3-channel (color) LUT using histogram matching
 *
 *  \param[in] src   source cv::Mat of CV_8UC3
 *  \param[in] h     the histogram in cv:Mat(3, 256) that the
 *                   source is to be matched to
 *  \param[out] lut  3-channel look up table in cv::Mat(3, 256)
 */
bool MatchingLut(const cv::Mat& src, const cv::Mat& h, cv::Mat& lut) {
  // Histogram of the image
  cv::Mat_<int> src_h;
  ipcv::Histogram(src, src_h);

  // CDF of the image
  cv::Mat_<double> src_cdf;
  ipcv::HistogramToCdf(src_h, src_cdf);

  // CDF of the target image 
  cv::Mat_<double> tgt_cdf;
  ipcv::HistogramToCdf(h, tgt_cdf);

  lut.create(3, 256, CV_8UC1);

  // Blue channel
  double blueDiff; //the difference between a point on the src_cdf and the tgt_cdf
  int blueSmallestIdx; //the index of the smallest difference between them
  double blueSmallestDiff = 99999; //the smallest difference between them, set high so it can start off

  // Go through all the DC values in the src_cdf
  for(int i = 0; i < 256; i++){
    // Compare each DC in the tgt_cdf to one of the src_cdf
    for(int j = 0; j < 256; j++){  
      blueDiff = abs(src_cdf(0,i) - tgt_cdf(0,j));

      // If the difference is the smallest one yet, save it
      if(blueDiff < blueSmallestDiff){
        blueSmallestDiff = blueDiff;
	blueSmallestIdx = j;	
      }
    }

    // Set the LUT DC value to the one in the tgt_cdf to the one that's closest to the src_cdf
    lut.at<uchar>(0,i) = blueSmallestIdx;
    // Reset the difference
    blueSmallestDiff = 99999;
  }

  for(int i = 0; i < lut.cols; i++){
    cout << "DCin: " << i << endl;	
    cout << "DCout: " << static_cast<int>(lut.at<uint8_t>(0,i)) << endl;
  }

  // Green channel
  double greenDiff;
  int greenSmallestIdx;
  double greenSmallestDiff = 99999;

  for(int i = 0; i < 256; i++){
    for(int j = 0; j < 256; j++){
      greenDiff = abs(src_cdf(1,i) - tgt_cdf(1,j));

      if(greenDiff < greenSmallestDiff){
        greenSmallestDiff = greenDiff;
        greenSmallestIdx = j;
      }
    }

    lut.at<uchar>(1,i) = greenSmallestIdx;
    greenSmallestDiff = 99999;
  }

  // Red channel
  double redDiff;
  int redSmallestIdx;
  double redSmallestDiff = 99999;

  for(int i = 0; i < 256; i++){
    for(int j = 0; j < 256; j++){
      redDiff = abs(src_cdf(2,i) - tgt_cdf(2,j));

      if(redDiff < redSmallestDiff){
        redSmallestDiff = redDiff;
        redSmallestIdx = j;
      }
    }

    lut.at<uchar>(2,i) = redSmallestIdx;
    redSmallestDiff = 99999;
  }


/* 
  vector<double> diff;

  int minIdx[1];

  for(int c = 0; c < src_cdf.cols; c++){
    double blue = src_cdf.at<double>(0,c);
	  
    absdiff(blue, tgt_cdf, diff);
        
    cv::minMaxIdx(blue, NULL, NULL, minIdx, NULL); 	

    lut.at<uchar>(0,c) = minIdx[1];
 }
*/

  return true;
}
}
