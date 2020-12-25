#include<opencv2/opencv.hpp>
#include<iostream>
#include"threshold.hpp"
#include"calibration.hpp"
#define display  1
#define write 0
using namespace cv;
#define use_simple_compare 0

vector<Mat>img_stocks(15);
int angles[12];

//calculate similarity by comparing pixels
double  cal(Mat& img ,Mat& src){
    
    int width=img.cols;
    int height=img.rows;
    //int step=img.step;
    int elem=img.elemSize();
    double ret=0;
    for(int i=0;i<height;i++){
        int step=i*width;
        for(int k=0;k<width;k++){
           //ret+=img.data[step+k*elem]==src.data[step+k*elem] ? 1:0
           if(img.data[step+k*elem]==src.data[step+k*elem])ret++;
        }
    }
    return ret/(width*height);
}
void prepare(){
    for(int k=0;k<12;k++){
            //if(i==k)continue;
            std::ostringstream oss;
            oss<<"./binary_angles/bin_ang"<<angles[k]<<".png";
            Mat src=imread(oss.str(),0);
            //Rect roi=selectROI("select_roi_for_sample",src,true,false);
            Mat black(src.size(),0,cv::Scalar(0));
            Mat tmp=src(roi);
            tmp.copyTo(black(roi));
            img_stocks[k]=black;
            std::ostringstream oss_out;
            oss_out<<"./binary_angles/bin2_ang"<<angles[k]<<".png";
            imwrite(oss_out.str(),black);
    }
    return ;
    
}
int main(){
    
    const string filepath="./test_angle/ang";  
    //const string filepath="./binary_angles/bin_ang";
    
    angles[0]=0;
    for(int i=1;i<12;i++)angles[i]=angles[i-1]+15;
    //prepare();
    for(int i=0;i<13;i++){
        ostringstream ostr;
        ostr << filepath << angles[i] << ".png";
        cv::Mat imgl,imgr;
        imgl=cv::imread(ostr.str(),0);
#if display       
        cv::imshow("oringinl_left",imgl);
#endif
       
        cv::threshold(imgl,imgl,130,255,cv::THRESH_BINARY);
        clock_t end=clock();
        Rect roi=selectROI("select_roi",imgl,true,false);
        Mat black(imgl.size(),0,cv::Scalar(0));
        Mat tmp=imgl(roi);
        tmp.copyTo(black(roi));

        //imshow("black",black);
        imgl=black;
        clock_t start=clock();
        printf("%lf :ms\n" ,((end-start)/100.0));///200);
       
         double MAX=-1;
         double MIN=1.0;
        int best_match=-1;
        cv::Mat best;
        for(int k=0;k<12;k++){
            std::ostringstream oss_out;
            oss_out<<"./binary_angles/bin2_ang"<<angles[k]<<".png";
            Mat src=imread(oss_out.str(),0);
#if use_simple_compare
            double now=cal(imgl,src);
            if(MAX<now){
                MAX=now;
                best_match=k;
                best=src;
            }
#else
            double now=matchShapes(imgl,src,CONTOURS_MATCH_I1,0.5);
            if(MIN>now){
                    MIN=now;
                    best_match=k;
                    best=src;
            }
#endif
            printf("similarity with %d : %lf\n",angles[k],now);
        }
        printf("img %d  estimated ang %d\n\n",angles[i],angles[best_match]);
        cv::imshow("best_matched",best);
#if display
        //cv::imshow("vinalized_left",imgl);
        
        
        int key=cv::waitKey(-1);
        if(key=='d')continue;
#endif

#if write
        std::ostringstream oss;
        oss<<"./bina_angles/bin_ang"<<angles[i]<<".png";
        cv::imwrite(oss.str(),imgl);
#endif
        if(key=='q')return 0;
    }
    
    
    return 0;
}