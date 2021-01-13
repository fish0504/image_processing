/*
 *  stereo_match.cpp
 *  calibration
 *
 *  Created by Victor  Eruhimov on 1/18/10.
 *  Copyright 2010 Argus Corp. All rights reserved.
 *
 */

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"

#include <stdio.h>
#include <sstream>

#include"convert.cpp"

#define sad 5
#define num 32     //disparity
// #define w 800
// #define h 600
const int w=800;
const int h=600;
    
#define roi_width 400
#define roi_height 400
using namespace cv;


enum { STEREO_BM=0, STEREO_SGBM=1, STEREO_HH=2, STEREO_VAR=3, STEREO_3WAY=4, STEREO_HH4=5 };
const int alg=STEREO_SGBM; 

std::string intrinsic_filename = "";
std::string disparity_filename = "dist.npy";
std::string point_cloud_filename = "";



bool no_display=false;
bool color_display=false;
const float scale=1.0;
int SADWindowSize= sad;
int numberOfDisparities= num;
Mat M1, D1, M2, D2;
Mat R, T, R1, P1, R2, P2;
Ptr<StereoBM> bm ;
Ptr<StereoSGBM> sgbm;
static void saveXYZ(const char* filename, const Mat& mat);
bool init_stereomatch();
cv::Mat getDepthImage(cv::Mat img1,cv::Mat img2);
 std::string disp_name;
const std::string intrinsic_filename_left="./cameraParams/r2_intrinsic.xml";
const std::string intrinsic_filename_right="./cameraParams/r2_intrinsic.xml";
const std::string extrinsic_filename="./cameraParams/camera.xml";
using namespace std;