#include"Angle_estimate_pixel.hpp"
//calculate similarity by comparing pixels





double  caluculate_similarity(Mat& img ,Mat& src,Rect roi){
    
    int width=img.cols;
    int height=img.rows;
    int elem=img.elemSize();
    double ret=0;
    //img=img(roi);
    src=src(roi);
    #if  BIN
    cv::threshold(src,src,50,255,cv::THRESH_BINARY);
    #endif
    for(int i=0;i<height;i++){
        int step=i*width;
        for(int k=0;k<width;k++){
           if(img.data[step+k*elem]==src.data[step+k*elem])ret++;
        }
    }
    return ret/(width*height);
}
//preparing for estimations
void Angle_estimate_init(){
        angles[0]=0.0;
    for(int i=1;i<AngleDivision;i++)angles[i]=angles[i-1]+(360.0/AngleDivision);
    return;
}


//input (now_angular_index,now_image)
//output (estimated_angular_index)
int estimate_Angular(int now_angular_index,cv::Mat imgl,bool realsense){
        
        int ret=-1;
#if 0 
        cv::imshow("oringinl_left",imgl);
#endif
        if(realsense){
            cv::cvtColor(imgl,imgl,COLOR_RGB2BGRA);
        }
        double MAX=-1;
        double MIN=1.0;
        int best_match=-1;
        cv::Mat best;
        int roi_index= realsense ? 1:0;
        imgl=imgl(rois[roi_index]);

#if BIN
        cv::threshold(imgl,imgl,50,255,cv::THRESH_BINARY);
#endif
        //clock_t start=clock();

        // Baslerの最初の一回目と　RealSenseのときは全通り試す
        int end= now_angular_index<0 ?  AngleDivision:10;
        //printf("compare numbers : %d\n",end);
        for(int k=0;k<end;k++){
            int index=now_angular_index+k;//search_range[k];
            if(index<0)index+=AngleDivision;
            else if(index>=AngleDivision)index%=AngleDivision;

            std::ostringstream oss_out;
            if(realsense){
            oss_out<< filepathRealSense<<angles[index]<<".png";
            }
            else{
            oss_out<< filepathLeft<<angles[index]<<".png";
            }
            Mat src=imread(oss_out.str(),0);
#if use_simple_compare
            double now=caluculate_similarity(imgl,src,rois[0]);
            if(MAX<now){
                MAX=now;
                best_match=index;
                best=src;
                ret=best_match;
            }
#else
            double now=matchShapes(imgl,src,CONTOURS_MATCH_I1,0.5);
            if(MIN>now){
                    MIN=now;
                    best_match=k;
                    best=src;
            }
#endif
        }

#if 0
        std::ostringstream oss;
        oss<<"./bina_angles/bin_ang"<<angles[i]<<".png";
        cv::imwrite(oss.str(),imgl);
#endif
      
    return ret;
}