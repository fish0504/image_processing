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

#define IMAGE_NUM  (11)         /* 画像数 */
#define PAT_ROW    (8)          /* パターンの行数 */
#define PAT_COL    (14)         /* パターンの列数 */
#define PAT_SIZE   (PAT_ROW*PAT_COL)
#define ALL_POINTS (IMAGE_NUM*PAT_SIZE)
#define CHESS_SIZE (20.0)       /* パターン1マスの1辺サイズ[mm] */
bool calibration(string filepath,string intrinsic_filename,bool left);
void stereoCalib();