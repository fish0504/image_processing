#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "opencv2/calib3d.hpp"
//#include <opencv/highgui.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
//#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
//#include "opencv2/highgui.hpp"
#include <cctype>
#include <stdio.h>
#include <string.h>
#include <time.h>

using namespace std;

#define IMAGE_NUM  (30)         /* 画像数 */
// #define PAT_ROW    (8)          /* パターンの行数 */
// #define PAT_COL    (14)         /* パターンの列数 */
//#define CHESS_SIZE (20.0)       /* パターン1マスの1辺サイズ[mm] */

//not rotated
#define PAT_ROW    (7)          /* パターンの行数 */
#define PAT_COL    (12)         /* パターンの列数 */

//after rotated
//#define PAT_ROW    (12)          /* パターンの行数 */
//#define PAT_COL    (7)         /* パターンの列数 */

#define CHESS_SIZE (10.0)       /* パターン1マスの1辺サイズ[mm] */
#define PAT_SIZE   (PAT_ROW*PAT_COL)
#define ALL_POINTS (IMAGE_NUM*PAT_SIZE)

bool calibration(string filepath,string intrinsic_filename,bool left);
void stereoCalib();
vector<cv::Point2f> l_corners;
vector<cv::Point2f> r_corners;
vector<cv::Point3f> object;
cv::Mat kl,kr,dl,dr;
cv::Size img_size;
 vector<vector<cv::Point3f>> obj_points;
 vector<vector<cv::Point2f>> img_points_l;
 vector<vector<cv::Point2f>> img_points_r;
const string intrinsic_filename_left="/home/kawahara/programs/image_input/cameraParams/l_intrinsic.xml";
const string intrinsic_filename_right="/home/kawahara/programs/image_input/cameraParams/r_intrinsic.xml";
const string extrinsic_filename="/home/kawahara/programs/image_input/cameraParams/extrinsic.xml";