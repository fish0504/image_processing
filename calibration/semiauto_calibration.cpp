#include "calibration.hpp"


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
    bool success_right=calibration("/home/kawahara/programs/resources/calibration/right_rotated/right",intrinsic_filename_right,0);
    bool success_left=calibration("/home/kawahara/programs/resources/calibration/left_rotate/left",intrinsic_filename_left,1);

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

bool calibration(string filepath,string intrinsic_filename,bool left)
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
        //cv::rotate(src,src,cv::ROTATE_90_COUNTERCLOCKWISE);
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

    // (6)XMLファイルへの書き出し
    cv::FileStorage fs1(intrinsic_filename, cv::FileStorage::READ);
    if(!fs1.isOpened())
    {
        cerr << "File can not be opened." << endl;
        return -1;
    }
    if(left){
    fs1["M1"]>> cam_mat;
    //fs1["D1"]>> dist_coefs;
    }
    else{
    fs1["M2"]>>cam_mat;
    //fs1["D2"]>>dist_coefs;
    
    }
    fs1.release();
    
    cv::calibrateCamera(
        obj_points,
        img_points,
        srcImages[0].size(),
        cam_mat,
        dist_coefs,
        rvecs,
        tvecs,cv::CALIB_USE_INTRINSIC_GUESS
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
    

    return true;
}

void stereoCalib(){

std::cout << "Read intrinsics" << std::endl;
cv::Mat r,t,e,f;
stereoCalibrate(
    obj_points,
    img_points_l,img_points_r,
    kl,dl,kr,dr,
    img_size,r,t,e,f,cv::CALIB_FIX_INTRINSIC
);
printf("Done Calibration\n");


printf("Starting Rectification\n");
cv::Mat R1, R2, P1, P2, Q;
cv::stereoRectify(kl, dl,kr, dr, img_size, r, t, R1, R2, P1, P2, Q);

printf("Done Rectification\n");
cv::Mat map11, map12, map21, map22;
initUndistortRectifyMap(kl, dl, R1, P1, img_size, CV_16SC2, map11, map12);
initUndistortRectifyMap(kr, dr, R2, P2, img_size, CV_16SC2, map21, map22);
 cv::FileStorage fs(extrinsic_filename,cv:: FileStorage::WRITE);
fs<<"R"<<r;
fs<<"T"<<t;


}