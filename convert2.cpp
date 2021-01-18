#include "convert.hpp"
std::string type2str(int type) { 
    std::string r; 

    uchar depth = type & CV_MAT_DEPTH_MASK; 
    uchar chans = 1 + (type >> CV_CN_SHIFT); 

    switch (depth) { 
    case CV_8U: r = "8U"; break; 
    case CV_8S: r = "8S"; break; 
    case CV_16U: r = "16U"; break; 
    case CV_16S: r = "16S"; break; 
    case CV_32S: r = "32S"; break; 
    case CV_32F: r = "32F"; break; 
    case CV_64F: r = "64F"; break; 
    default:  r = "User"; break; 
    } 

    r += "C"; 
    r += (chans+'0'); 

    return r; 
} 
void init_converter(){

    //Python、numpyモジュールの初期化
    Py_Initialize();
    //Pythonスクリプトの読み込み
    //std::ifstream ifs("mat_numpy.py");
    std::ifstream ifs(pyfile);
    std::ifstream ifs_mat(matTonumpy_file);
    script_dex=std::string((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());
    script_mat=std::string((std::istreambuf_iterator<char>(ifs_mat)),
                        std::istreambuf_iterator<char>());

}
int main (){
    // if(depth.empty()){
    //     printf("image empty()! failed!\n");
    //     return false;
    // }
    init_converter();
    
    int cnt=0;
    printf("now2\n");
    //cv::Mat depth;
    //depth=cv::imread("../disp_result.png",0);
   
    //100x100行列の準備
    //boost::python::tuple shapeA = boost::python::make_tuple(MAX_X, MAX_X);

    //comment outed
    // int HEIGHT=depth.rows;
    // int WIDTH=depth.cols;
    // boost::python::tuple shapeA = boost::python::make_tuple(HEIGHT,WIDTH);

    // np::ndarray A = np::zeros(shapeA, np::dtype::get_builtin<unsigned char>());
    // int width = depth.cols;
    // int height = depth.rows;
    // int channels = depth.channels();
    // for(int i=0; i != HEIGHT; i++) {
    //     int step=i*WIDTH;
    //     for(int j=0; j != WIDTH; j++) {
    //         int elm=j*depth.elemSize();
    //         A[i][j] = depth.data[step+elm];
    //         //A[i][j] =i+j;
    //     }
    // }
    
    
        np::initialize();
        //名前空間の確保
        auto main_ns = boost::python::import("__main__").attr("__dict__");
        //mat_numpy.mulの実行
        boost::python::exec(script_mat.c_str(), main_ns);
        auto func = main_ns["matConvertToNumpy"];


   
        
    auto pyresult_numpy=func(cnt);
    

    //結果の受け取り
    //stl_input_iteratorを使ってタプル全要素を受け取る
    printf("now3\n");
    
#if 1 //CONFIRM_IMAGES
        boost::python::stl_input_iterator<np::ndarray> begin(pyresult_numpy), end;
        std::list<np::ndarray> pyresult_list(begin, end);
        cv::Mat received(600,800,CV_8UC1);
    for(auto itr = pyresult_list.begin(); itr != pyresult_list.end(); ++itr) {
        unsigned char *p = reinterpret_cast<unsigned char *>((*itr).get_data());
        //double *p = reinterpret_cast<double *>((*itr).get_data());
        //ndarrayでは基本的にメモリは連続領域上に保持されるので、
        //各要素には[]演算子を使ってアクセスできる
        int HEIGHT=800;
        int WIDTH=600;
        for(int i=0;i<HEIGHT;i++){
            for(int k=0;k<WIDTH;k++){
                received.data[i*WIDTH+k]=p[i*WIDTH+k];
            }
        }
    }
        printf("returened\n");
        
        
        cv::imshow("returned_disp",received);
        cv::waitKey(-1);
        std::ostringstream depth;
        depth<<"/home/kawahara/programs/image_input/depth_"<<cnt<<".png";
        cv::imwrite(depth.str(),received);
#endif
        //boost::python::exec(script_dex.c_str(), main_ns);
        printf("now4\n");

        //std::cout << p[0] << ',' << p[1] << ',' << p[2] << ',' << p[MAX_X * MAX_X - 1] << std::endl;
  
    return true;
}