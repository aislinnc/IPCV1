/** Implementation file for image enhancement using linear histogram statistics
 *
 *  \file ipcv/histogram_enhancement/LinearLut.cpp
 *  \author Aislinn Campbell (aec3160@rit.edu)
 *  \date 05 Sep 2019
 */

#include "LinearLut.h"

#include <iostream>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Create a 3-channel (color) LUT using linear histogram enhancement
 *
 *  \param[in] src          source cv::Mat of CV_8UC3
 *  \param[in] percentage   the total percentage to remove from the tails
 *                          of the histogram to find the extremes of the
 *                          linear enhancemnt function
 *  \param[out] lut         3-channel look up table in cv::Mat(3, 256)
 */
bool LinearLut(const cv::Mat& src, const int percentage, cv::Mat& lut) {
  // Make a histogram of src and store it in h
  cv::Mat_<int> h;
  ipcv::Histogram(src, h);

  // Create a cdf of the histogram
  cv::Mat_<double> cdf;
  ipcv::HistogramToCdf(h, cdf);

  // Create a lookup table, 3 channels, 256 bins
  lut.create(3, 256, CV_8UC1);

  // Check if the percentage is valid 
  double remainder = percentage % 2;
  if(remainder != 0){
    return false;
  }	

  // Take half the percentage off the bottom and top of the LUT
  double percent = percentage / 100.0;
  double halfPercent = percent / 2;

  // Find the min and max digital count values after the top and bottom are cut off
  vector<int> minFinder;
  vector<int> maxFinder;
	
  // Go through all 3 channels
  for(int r = 0; r < cdf.rows; r++){
    // Go through all 256 digital counts
    for(int c = 0; c < cdf.cols; c++){
      // Where the CDF is less than halfPercent
      if(cdf.at<double>(r, c) < halfPercent){
        minFinder.push_back(c);
      }

      // Where the CDF is more than 1 - halfPercent
      if(cdf.at<double>(r,c) > (1 - halfPercent)){
        maxFinder.push_back(c);
      }
    }
  }

  // Find the largest value in minFinder
  int maxMin = 0;
  for(int i = 0; i < minFinder.size(); i++){
    if(minFinder[i] > maxMin){
      maxMin = minFinder[i];
    }
  }

  // Find the smallest value in maxFinder
  int minMax = 255;
  for(int j = 0; j < maxFinder.size(); j++){
    if(maxFinder[j] < minMax){
      minMax = maxFinder[j];
    }
  }

  // Find the slope of the LUT line
  int m = 255 / (minMax - maxMin); 

  // Find the y-intercept of the LUT
  int b = -m * maxMin;

  // Fill in the LUT tables using the LUT line
  // Go through all 3 channels
  for(int r = 0; r < lut.rows; r++){
    // Go through all the digital counts  
    for(int c = 0; c < lut.cols; c++){
      if(c <= maxMin){
        lut.at<uchar>(r,c) = 0;
      }
      else if(c >= minMax){
        lut.at<uchar>(r,c) = 255;
      }
      else{
        lut.at<uchar>(r,c) = (m*c)+ b;
      }
    }
  }	
  
  return true;
}
}
