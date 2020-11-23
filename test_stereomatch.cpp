#include"stereomatch.cpp"

int main(){
   
    if(!init_stereomatch()){
        printf("init_faield\n");
    }
    cv::Mat left_img=cv::imread("./calib_img2/left/matching_sample_left.png",-1);
    cv::Mat right_img=cv::imread("./calib_img2/right/matching_sample_right.png",-1);
    printf("channels :%d\n",left_img.channels());//<<std::endl;
    if(left_img.empty()|| right_img.empty()){
        printf("img is empty!\n");
        return -1;
    }
    cv::Mat depth_result=getDepthImage(left_img,right_img);
    if(!depth_result.empty()){
        printf("successed!\n");
    }
    else{
        printf("failed!\n");
    }
    return 0;
}