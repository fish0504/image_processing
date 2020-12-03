#pragma once
#include <opencv2/opencv.hpp>
#ifdef __aarch64__
#include "simde/x86/avx512.h"
#else
#include <x86intrin.h>
#endif


namespace SIMD
{
void threshold2Uchar(const cv::Mat& float_image, cv::Mat& uchar_image, float val);
}  // namespace SIMD
