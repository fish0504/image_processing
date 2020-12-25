#include<opencv2/opencv.hpp>
#include<iostream>
#include"threshold.hpp"
#include"calibration.hpp"


//parameters
//#define display  0
#define write 0
using namespace cv;
#define use_simple_compare 1
#define AngleDivision 64
#define w 800
#define h 600
#define roi_width 400
#define roi_height 400
#define BIN 1
const std::string savedAngleImagesLeft="./../rotation_estiamate/left/left";
const std::string savedAngleImagesRight="./../rotation_estiamate/right/right";
const std::string filepathLeft="./../rotation_estimate/left/left";
const std::string filepathRight="./../rotation_estimate/right/right";
vector<std::string>camera={"left","right"};

cv::Rect roi_1(cv::Point2i(0,(w-roi_width)-275),cv::Size(h,roi_width-25));
cv::Rect roi_2(cv::Point2i(0,(w-roi_width)-225),cv::Size(h,roi_width-25));

vector<cv::Rect>rois={roi_1,roi_2};//left,right


vector<Mat>img_stocks(15);
int search_range[3]={-1,0,1};
double angles[AngleDivision];
double  caluculate_similarity(Mat& img ,Mat& src,Rect roi);
int estimateAngular(int now_angular_index,cv::Mat& now_image);

