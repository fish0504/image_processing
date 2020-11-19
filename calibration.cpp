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

#define IMAGE_NUM  (10)         /* 画像数 */
#define PAT_ROW    (8)          /* パターンの行数 */
#define PAT_COL    (14)         /* パターンの列数 */
#define PAT_SIZE   (PAT_ROW*PAT_COL)
#define ALL_POINTS (IMAGE_NUM*PAT_SIZE)
#define CHESS_SIZE (20.0)       /* パターン1マスの1辺サイズ[mm] */
bool calibration(string filename,string filepath,bool left);
void stereoCalib();

vector<cv::Point2f> l_corners;
vector<cv::Point2f> r_corners;
vector<cv::Point3f> object;
cv::Mat kl,kr,dl,dr;
cv::Size img_size;
 vector<vector<cv::Point3f>> obj_points;
 vector<vector<cv::Point2f>> img_points_l;
 vector<vector<cv::Point2f>> img_points_r;
int main(){
   
    
    // (2)3次元空間座標の設定

    
    for (int j = 0; j < PAT_ROW; j++)
    {
        for (int k = 0; k < PAT_COL; k++)
        {
            cv::Point3f p(
                j * CHESS_SIZE,
                k * CHESS_SIZE,
                0.0);
            
            object.push_back(p);
        }
    }
    for (int i = 0; i < IMAGE_NUM; i++)
    {
        obj_points.push_back(object);
    }
    bool success_right=calibration("camera_right.xml","./calib_img/right",0);
    bool success_left=calibration("camera_left.xml","./calib_img/left",1);

    if(success_left&&success_right){
        printf("calibration completed!\n");
        stereoCalib();
        return 0;
    }
    else{
        printf("calibration falied!\n");
        return 0;
    }
    
}

bool calibration(string filename,string filepath,bool left)
{
    int i, j, k;
    int corner_count, found;
    int p_count[IMAGE_NUM];
    // cv::Mat src_img[IMAGE_NUM];
    vector<cv::Mat> srcImages;
    cv::Size pattern_size = cv::Size2i(PAT_COL, PAT_ROW);
    
    vector<vector<cv::Point2f>> img_points;
    vector<cv::Point2f> corners;
    // (1)キャリブレーション画像の読み込み
    for (i = 0; i < IMAGE_NUM; i++)
    {
        ostringstream ostr;
        ostr << filepath << i << ".png";
        cv::Mat src = cv::imread(ostr.str());
        if (src.empty())
        {
            cerr << "cannot load image file : " << ostr.str() << endl;
        }
        else
        {
            srcImages.push_back(src);
        }
        img_size=src.size();
    }

    

    
    

    // ３次元の点を ALL_POINTS * 3 の行列(32Bit浮動小数点数:１チャンネル)に変換する 


    // (3)チェスボード（キャリブレーションパターン）のコーナー検出
    int found_num = 0;
    cv::namedWindow("Calibration", cv::WINDOW_AUTOSIZE);
    for (i = 0; i < IMAGE_NUM; i++)
    {
        bool found=cv::findChessboardCorners(srcImages[i], pattern_size, corners);
        if (found)
        {
            cout << setfill('0') << setw(2) << i << "... ok" << endl;
            found_num++;
        }
        else
        {
            cerr << setfill('0') << setw(2) << i << "... fail" << endl;
        }

        // (4)コーナー位置をサブピクセル精度に修正，描画
        cv::Mat src_gray = cv::Mat(srcImages[i].size(), CV_8UC1);
        cv::cvtColor(srcImages[i], src_gray, cv::COLOR_BGR2GRAY);
        cv::find4QuadCornerSubpix(src_gray, corners, cv::Size(3,3));
        cv::drawChessboardCorners(srcImages[i], pattern_size, corners, found);
        img_points.push_back(corners);

        cv::imshow("Calibration", srcImages[i]);
        cv::waitKey(0);
    }
    cv::destroyWindow("Calibration");

    if (found_num != IMAGE_NUM)
    {
        cerr << "Calibration Images are insufficient." << endl;
        return -1;
    }

    // (5)内部パラメータ，歪み係数の推定
    cv::Mat cam_mat; // カメラ内部パラメータ行列
    cv::Mat dist_coefs; // 歪み係数
    vector<cv::Mat> rvecs, tvecs; // 各ビューの回転ベクトルと並進ベクトル
    cv::calibrateCamera(
        obj_points,
        img_points,
        srcImages[0].size(),
        cam_mat,
        dist_coefs,
        rvecs,
        tvecs
    );
    if(left){
        img_points_l=img_points;
        //l_corners=corners;
        kl=cam_mat;
        dl=dist_coefs;
    }
    else{
        img_points_r=img_points;
        //r_corners.resize(corners.size());
        //std::copy(corners.begin(), corners.end(), r_corners.begin());
        //r_corners=corners;
        kr=cam_mat;
        dr=dist_coefs;
    }
    // (6)XMLファイルへの書き出し
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    if(!fs.isOpened())
    {
        cerr << "File can not be opened." << endl;
        return -1;
    }

    fs << "intrinsic" << cam_mat;
    fs << "distortion" << dist_coefs;
    fs.release();

    return true;
}

void stereoCalib(){

std::cout << "Read intrinsics" << std::endl;
cv::Mat r,t,e,f;
stereoCalibrate(
    obj_points,
    img_points_l,img_points_r,
    kl,dl,kr,dr,
    img_size,r,t,e,f
);
printf("Done Calibration\n");


printf("Starting Rectification\n");
cv::Mat R1, R2, P1, P2, Q;
cv::stereoRectify(kl, dl,kr, dr, img_size, r, t, R1, R2, P1, P2, Q);
printf("Done Rectification\n");


}