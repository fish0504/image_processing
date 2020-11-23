#include"convert.cpp"

int main(){
    cv::Mat img=cv::imread("disp_result.png",0);
    init_converter();
    if(img.empty()){
        printf("img is empty!\n");
        return -1;
    }
    if(convertToPython(img)){
        printf("successed!\n");
    }
    else{
        printf("failed!\n");
    }
    return 0;
}