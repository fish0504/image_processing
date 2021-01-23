#include "stereomatch.hpp"



static void saveXYZ(const char* filename, const Mat& mat)
{
    const double max_z = 1.0e4;
    FILE* fp = fopen(filename, "wt");
    for(int y = 0; y < mat.rows; y++)
    {
        for(int x = 0; x < mat.cols; x++)
        {
            Vec3f point = mat.at<Vec3f>(y, x);
            if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
            fprintf(fp, "%f %f %f\n", point[0], point[1], point[2]);
        }
    }
    fclose(fp);
}
void showDexResult(){
    printf("filestorage will open\n");
    cv::FileStorage fs;
    fs.open(y_file, cv::FileStorage::READ);
    
    if(!fs.isOpened())
        {
            printf("Failed to open file dexresult\n");
            return ;
        }

    double q,lean,depth,point_x,point_y;
    fs["q_value"]>>q;
    fs["lean"]>>lean;
    fs["depth"]>>depth;
    fs["point_x"]>>point_x;
    fs["point_y"]>>point_y;
    // double q= double(fs["q_value"]);
    // double lean= double(fs["lean"]);
    // double depth= double(fs["depth"]);
    // double point_x= double(fs["point_x"]);
    // double point_y= double(fs["point_y"]);
    printf("q_value:%lf\n",q);
    printf("lean:%lf\n",lean);
    printf("depth:%lf\n",depth);
    printf("point_x:%lf\n",point_x);
    printf("point_y:%lf\n",point_y);


}

bool init_stereomatch()
{
#if test_convert
    return true;
#endif
bm = StereoBM::create(16,9);
sgbm = StereoSGBM::create(0,16,3);
    if( !intrinsic_filename_left.empty() )
    {
        // reading intrinsic parameters
        FileStorage fs(intrinsic_filename_left, FileStorage::READ);
        if(!fs.isOpened())
        {
            printf("Failed to open file %s\n", intrinsic_filename_left.c_str());
            return -1;
        }

        
        fs["M1"] >> M1;
        fs["D1"] >> D1;
        fs.open(intrinsic_filename_right, FileStorage::READ);
        fs["M2"] >> M2;
        fs["D2"] >> D2;

        M1 *= scale;
        M2 *= scale;

        fs.open(extrinsic_filename, FileStorage::READ);
        if(!fs.isOpened())
        {
            printf("Failed to open file %s\n", extrinsic_filename.c_str());
            return -1;
        }

        
        fs["R"] >> R;
        fs["T"] >> T;
        std::ostringstream oss;
        std::string save_name;
        oss << "disparity  " << (alg==STEREO_BM ? "bm" :
                                 alg==STEREO_SGBM ? "sgbm" :
                                 alg==STEREO_HH ? "hh" :
                                 alg==STEREO_VAR ? "var" :
                                 alg==STEREO_HH4 ? "hh4" :
                                 alg==STEREO_3WAY ? "sgbm3way" : "");
        save_name=oss.str();
        //oss << "  blocksize:" << (alg==STEREO_BM ? SADWindowSize : sgbmWinSize);
        oss << "  max-disparity:" << numberOfDisparities;
        disp_name = oss.str();
        namedWindow(disp_name, cv::WINDOW_AUTOSIZE);
    }
    else{
        return false;
    }
    return true;
}
 //left,right
void getDepthImage(cv::Mat img1,cv::Mat img2){
   
    int color_mode = alg == STEREO_BM ? 0 : -1;
#if 1 //test_convert
    printf("call dexnet\n");
    cv::Mat tmp;
    convertToPython(tmp);
    dexcnt++;
    showDexResult();
    return;
#endif
    //Mat img1 = imread(img1_filename, color_mode);
    //Mat img2 = imread(img2_filename, color_mode);
    if (img1.empty())
    {
        printf("Command-line parameter error: could not load the first input image file\n");
        //return -1;
    }
    if (img2.empty())
    {
        printf("Command-line parameter error: could not load the second input image file\n");
        //return -1;
    }
    //cv::Rect roi(cv::Point2i(w/2-roi_width/2,h/2-roi_height/2),cv::Size(roi_width,roi_height));
    //cv::Rect roi(cv::Point2i(w/2-roi_width/2-100,h/2-roi_height/2+150),cv::Size(roi_width,roi_height));


    //experimental
    //cv::Rect roi(0,0,img1.cols,img1.rows);
    //cv::Rect roi(200,100,300,400);
    cv::Rect roi(60,270,480,355);
    //cv::Rect roi_two(200,100,300,400);
    cv::Rect roi_two(60,270,480,355);
    //GaussianBlur(img1,img1, Size(35,35), 13, 13);
    //GaussianBlur(img2,img2, Size(35,35), 13,13);
    

    GaussianBlur(img1,img1, Size(35,25), 15, 15);
    GaussianBlur(img2,img2, Size(35,25), 15,15);
    //medianBlur(img1,img1,5);
    //medianBlur(img2,img2,5);
#if 1
    //cv::Rect roi_show(cv::Point2i(w/2-roi_width/2-100,h/2-roi_height/2+200),cv::Size(roi_width,roi_height-100));
    cv::Mat tmpl,tmpr;
    tmpr=img2,tmpl=img1;
    cv::rectangle(tmpl,roi,cv::Scalar(255,255,255),2);
    //cv::imshow("rectangle_1" ,img1);
    cv::rectangle(tmpr,roi_two,cv::Scalar(255,255,255),2);
    //cv::imshow("rectangle_2" ,img2);
    cv::Mat showImage;
    cv::hconcat(tmpl,tmpr,showImage);
    cv::imshow("camera",showImage);
    cv::waitKey(1);
#endif
    //printf("now1\n");
    
    if (scale != 1.f)
    {
        Mat temp1, temp2;
        int method = scale < 1 ? INTER_AREA : INTER_CUBIC;
        cv::resize(img1, temp1, Size(), scale, scale, method);
        img1 = temp1;
        cv::resize(img2, temp2, Size(), scale, scale, method);
        img2 = temp2;
    }
    img1=img1(roi);
    img2=img2(roi_two);
    //std::swap(img1,img2);

    printf("here!");
    Size img_size = img1.size();
    Rect roi1,roi2;
    Mat Q;
    
    printf("now2\n");
    cv::stereoRectify( M1, D1, M2, D2,img_size , R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1,img_size, &roi1, &roi2 );
    printf("now3\n");
    Mat map11, map12, map21, map22;
    cv::initUndistortRectifyMap(M1, D1, R1, P1, img_size, CV_16SC2, map11, map12);
    cv::initUndistortRectifyMap(M2, D2, R2, P2, img_size, CV_16SC2, map21, map22);
    
    Mat img1r, img2r;
    cv::remap(img1, img1r, map11, map12, INTER_LINEAR);
    cv::remap(img2, img2r, map21, map22, INTER_LINEAR);
    
    img1 = img1r;
    img2 = img2r;
    numberOfDisparities = numberOfDisparities > 0 ? numberOfDisparities : ((img_size.width/8) + 15) & -16;

    bm->setROI1(roi1);
    bm->setROI2(roi2);
    bm->setPreFilterCap(31);
    bm->setBlockSize(SADWindowSize > 0 ? SADWindowSize : 9);
    bm->setMinDisparity(0);
    bm->setNumDisparities(numberOfDisparities);
    bm->setTextureThreshold(10);
    bm->setUniquenessRatio(15);
    bm->setSpeckleWindowSize(100);
    bm->setSpeckleRange(32);
    bm->setDisp12MaxDiff(1);

    sgbm->setPreFilterCap(63);
    int sgbmWinSize = SADWindowSize > 0 ? SADWindowSize : 3;
    sgbm->setBlockSize(sgbmWinSize);

    int cn = img1.channels();

    sgbm->setP1(8*cn*sgbmWinSize*sgbmWinSize);
    sgbm->setP2(32*cn*sgbmWinSize*sgbmWinSize);
    sgbm->setMinDisparity(0);
    sgbm->setNumDisparities(numberOfDisparities);
    sgbm->setUniquenessRatio(10);
    sgbm->setSpeckleWindowSize(100);
    sgbm->setSpeckleRange(32);
    sgbm->setDisp12MaxDiff(1);
    if(alg==STEREO_HH)
        sgbm->setMode(StereoSGBM::MODE_HH);
    else if(alg==STEREO_SGBM)
        sgbm->setMode(StereoSGBM::MODE_SGBM);
    else if(alg==STEREO_HH4)
        sgbm->setMode(StereoSGBM::MODE_HH4);
    else if(alg==STEREO_3WAY)
        sgbm->setMode(StereoSGBM::MODE_SGBM_3WAY);

    Mat disp, disp8;
    // Mat img1p, img2p, dispp;
    // copyMakeBorder(img1, img1p, 0, 0, numberOfDisparities, 0, IPL_BORDER_REPLICATE);
    // copyMakeBorder(img2, img2p, 0, 0, numberOfDisparities, 0, IPL_BORDER_REPLICATE);
    
    int64 t = getTickCount();
    float disparity_multiplier = 1.0f;
    if( alg == STEREO_BM )
    {
        bm->compute(img1, img2, disp);
        if (disp.type() == CV_16S)
            disparity_multiplier = 16.0f;
    }
    else if( alg == STEREO_SGBM || alg == STEREO_HH || alg == STEREO_HH4 || alg == STEREO_3WAY )
    {
        sgbm->compute(img1, img2, disp);
        if (disp.type() == CV_16S)
            disparity_multiplier = 16.0f;
    }
    t = getTickCount() - t;
    //printf("Time elapsed: %fms\n", t*1000/getTickFrequency());

    //disp = dispp.colRange(numberOfDisparities, img1p.cols);
    if( alg != STEREO_VAR )
        disp.convertTo(disp8, CV_8U, 255/(numberOfDisparities*16.));
    else
        disp.convertTo(disp8, CV_8U);

    Mat disp8_3c;
    
    if( !no_display )
    {   cv:Mat tmp1,tmp2,tmp3,show_image;
        cv::hconcat(img1, img2, tmp1);
        cv::hconcat(tmp1,disp8,showImage);
        imshow("remapped Images"+disp_name,showImage);
        // int key=waitKey(-1);
        // printf("do you preserve result? [y/n]\n");
        // if(key=='y'){
        //     cv::imwrite("./disp_results/good_result.png",disp8);
        // }
        //cv::hconcat(disp8,tmp3, tmp2);
       // cv::vconcat(tmp1, tmp2, show_image);
        //cv::rotate(img1,img1,ROTATE_90_COUNTERCLOCKWISE);
        //imshow("camera_image_1",img1);
        //cv::rotate(img2,img2,ROTATE_90_COUNTERCLOCKWISE);
        //imshow("camera_image_2",img2);
        
        //imshow(disp_name, disp8);
        //imshow("stereo match",show_image);
        
    }

    return;
}