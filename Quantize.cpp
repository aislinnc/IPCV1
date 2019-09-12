/** Implementation file for image quantization
 *
 *  \file ipcv/quantize/quantize.cpp
 *  \author Aislinn Campbell (aec3160@rit.edu)
 *  \date 31 Aug 2019
 */

#include "Quantize.h"

#include <iostream>

using namespace std;

/** Perform uniform grey-level quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 */
void Uniform(const cv::Mat& src, const int quantization_levels, cv::Mat& dst) {
	// How many intensity values are in a level
        double level_mult = 256.0/quantization_levels;

        // Go through all 3 channels
	for(int i = 0; i < 3; i++){
                // Go through each pixel
		for(int r = 0; r < src.rows; r++){
                        for(int c = 0; c < src.cols; c++){
                                // Get the original intensity
                                int intensity = src.at<cv::Vec3b>(r, c)[i];

                                // Get the new pixel intensity
                               	dst.at<cv::Vec3b>(r, c)[i] = intensity / level_mult;
                	}
        	}
	}
}

/** Perform improved grey scale quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 */
void Igs(const cv::Mat& src, const int quantization_levels, cv::Mat& dst) {	
	// How many intesnity values are in a level
        int level_mult = 256/quantization_levels;

	// The remainder from the previous pixel
	int intensity_remainder = 0;

        // Go through all 3 channels
        for(int i = 0; i < 3; i++){
                // Go through each pixel
                for(int r = 0; r < src.rows; r++){
                        for(int c = 0; c < src.cols; c++){
                                // Get the intesnity of the pixel by adding the original intesnity and the remainder from the previous pixel
                                int intensity = (src.at<cv::Vec3b>(r,c)[i]) + intensity_remainder;

                                // Change it to where it fits in the set number of levels
                                dst.at<cv::Vec3b>(r,c)[i] = intensity / level_mult;
                               
                                // Get the new remainder
                                intensity_remainder = intensity % level_mult;
	               	}
		}
	}
}

namespace ipcv {

bool Quantize(const cv::Mat& src, const int quantization_levels,
              const QuantizationType quantization_type, cv::Mat& dst) {
  dst.create(src.size(), src.type());

  switch (quantization_type) {
    case QuantizationType::uniform:
      Uniform(src, quantization_levels, dst);
      break;
    case QuantizationType::igs:
      Igs(src, quantization_levels, dst);
      break;
    default:
      cerr << "Specified quantization type is unsupported" << endl;
      return false;
  }

  return true;
}
}
