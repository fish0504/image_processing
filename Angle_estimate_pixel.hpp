#include<opencv2/opencv.hpp>
#include<iostream>
#include"threshold.hpp"
//#include"calibration.hpp"


//parameters
//#define display  0
//#define write 0
using namespace cv;
#define use_simple_compare 1
#define AngleDivision 96
//#define w 800
//#define h 600
#define roi_width 400
#define roi_height 400
#define BIN 0
//const std::string savedAngleImagesLeft="/home/kawahara/programs/resources/rotation_estimate/left";
//const std::string savedAngleImagesRight="./rotation_estiamate/right/right";
const std::string filepathLeft="/home/kawahara/programs/resources/rotation_estimate/left";
const std::string filepathRight="./rotation_estimate/right/right";
std::vector<std::string>camera={"left","right"};

cv::Rect roi_1(cv::Point2i(0,(800-roi_width)-275),cv::Size(600,roi_width-25));
cv::Rect roi_2(cv::Point2i(0,(800-roi_width)-225),cv::Size(600,roi_width-25));

std::vector<cv::Rect>rois={roi_1,roi_2};//left,right


std::vector<Mat>img_stocks(15);
int search_range[10]={-5,-4,-3,-2,-1,0,1,2,3,4};
double angles[AngleDivision];
double  caluculate_similarity(Mat& img ,Mat& src,Rect roi);
int estimate_Angular(int now_angular_index,cv::Mat now_image_left,cv::Mat now_image_right);
void Angle_estimate_init();

