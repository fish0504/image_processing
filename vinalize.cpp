#include<opencv2/opencv.hpp>
#include<iostream>
#include"threshold.hpp"




int main(){

    cv::Mat imgl,imgr;
    imgl=cv::imread("./rotation_estimate/left/sample1.png",0);
    imgr=cv::imread("./rotation_estimate/right/sample1.png",0);
    cv::imshow("oringinl_left",imgl);
    cv::imshow("original_right",imgr);
    //SIMD::threshold2Uchar(imgl,imgl,255.0);
    //SIMD::threshold2Uchar(imgr,imgr,255.0);
    clock_t start=clock();
    for(int i=0;i<100;i++){
        cv::threshold(imgl,imgl,50,255,cv::THRESH_BINARY);
        cv::threshold(imgr,imgr,50,255,cv::THRESH_BINARY);
    }
    clock_t end=clock();
    printf("%lf :ms\n" ,((end-start)/100.0)/200);
    cv::imshow("vinalized_left",imgl);
    cv::imshow("vinalized_right",imgr);
    int key=cv::waitKey(-1);
    if(key=='q')return 0;

    return 0;
}