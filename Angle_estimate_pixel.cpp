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
int estimate_Angular(int now_angular_index,cv::Mat now_image_left,cv::Mat now_image_right){
 
    
    int ret=-1;
    //for(int now_angular_index=0;now_angular_index<AngleDivision;now_angular_index++){
        for(int l=0;l<2;l++){
        //ostringstream ostr;
        
        //input the pictures taken now
        // if(l==0)ostr << filepathLeft << angles[now_angular_index] << ".png";
        // else{
        //     ostr << filepathRight << angles[now_angular_index] << ".png";
        // }
        // cv::Mat imgl;
        // imgl=cv::imread(ostr.str(),0);
        cv::Mat imgl;
        if(l==0)imgl=now_image_left;
        else{
            imgl=now_image_right;
            continue;
        }
#if 0 
        cv::imshow("oringinl_left",imgl);
#endif
        

        
       
        double MAX=-1;
        double MIN=1.0;
        int best_match=-1;
        cv::Mat best;
        imgl=imgl(rois[l]);
#if BIN
        cv::threshold(imgl,imgl,50,255,cv::THRESH_BINARY);
#endif
        //clock_t start=clock();
        for(int k=0;k<10;k++){
            int index=now_angular_index+search_range[k];
            if(index<0)index+=AngleDivision;
            else if(index>=AngleDivision)index%=AngleDivision;

            std::ostringstream oss_out;
            if(l==0)oss_out<< filepathLeft<<angles[index]<<".png";
            else{
                oss_out<< filepathRight<<angles[index]<<".png";
            }
            Mat src=imread(oss_out.str(),0);
#if use_simple_compare
            double now=caluculate_similarity(imgl,src,rois[l]);
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

           // printf("similarity with %lf : %lf\n",angles[index],now);
        }
       /// clock_t end=clock();
        
        // if(l==0)printf("camera left img_angle %lf  estimated ang %lf\n\n",angles[now_angular_index],angles[best_match]);
        // else
        // {
        //     printf("camera right img_angle %lf  estimated ang %lf\n\n",angles[now_angular_index],angles[best_match]);
        // }
        
        //printf("%lf :ms\n" ,((end-start)/1000.0));///200);
        //cv::imshow("best_matched",best);

        
#if 0
        //cv::imshow("vinalized_left",imgl);
        
        
        
        if(key=='d')continue;
#endif

#if write
        std::ostringstream oss;
        oss<<"./bina_angles/bin_ang"<<angles[i]<<".png";
        cv::imwrite(oss.str(),imgl);
#endif
        //printf("continue ? press[y/n]");
        //cv::imshow("best_matched",best);
        // int key=cv::waitKey(-1);
        
        // if(key=='y')continue;
        // if(key=='n')return 0;
        }
       
   // }
    
    
    return ret;
}