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

    script=std::string((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());

}

bool convertToPython(cv::Mat depth){
    if(depth.empty()){
        printf("image empty()! failed!\n");
        return false;
    }
    // //Python、numpyモジュールの初期化
    // Py_Initialize();
    // //Pythonスクリプトの読み込み
    // std::ifstream ifs("mat_numpy.py");
    // script=std::string((std::istreambuf_iterator<char>(ifs)),
    //                     std::istreambuf_iterator<char>());

    np::initialize();
    //名前空間の確保
    auto main_ns = boost::python::import("__main__").attr("__dict__");
    
    printf("now2\n");
    //cv::Mat depth;
    //depth=cv::imread("../disp_result.png",0);
   
    //100x100行列の準備
    //boost::python::tuple shapeA = boost::python::make_tuple(MAX_X, MAX_X);
    int HEIGHT=depth.rows;
    int WIDTH=depth.cols;
    boost::python::tuple shapeA = boost::python::make_tuple(HEIGHT,WIDTH);

    np::ndarray A = np::zeros(shapeA, np::dtype::get_builtin<unsigned char>());
    int width = depth.cols;
    int height = depth.rows;
    int channels = depth.channels();
    for(int i=0; i != HEIGHT; i++) {
        int step=i*WIDTH;
        for(int j=0; j != WIDTH; j++) {
            int elm=j*depth.elemSize();
            A[i][j] = depth.data[step+elm];
            //A[i][j] =i+j;
        }
    }
    
    //mat_numpy.mulの実行
    boost::python::exec(script.c_str(), main_ns);
    auto func = main_ns["dex-net"];
    //func(A);
    

    //結果の受け取り
    //stl_input_iteratorを使ってタプル全要素を受け取る
    printf("now3\n");
    func(A);
    //auto ret=func(A);
#if CONFIRM_IMAGES
        boost::python::stl_input_iterator<np::ndarray> begin(pyresult_numpy), end;
        std::list<np::ndarray> pyresult_list(begin, end);
        cv::Mat received(600,800,CV_8UC1);
    for(auto itr = pyresult_list.begin(); itr != pyresult_list.end(); ++itr) {
        unsigned char *p = reinterpret_cast<unsigned char *>((*itr).get_data());
        //double *p = reinterpret_cast<double *>((*itr).get_data());
        //ndarrayでは基本的にメモリは連続領域上に保持されるので、
        //各要素には[]演算子を使ってアクセスできる
        for(int i=0;i<HEIGHT;i++){
            for(int k=0;k<WIDTH;k++){
                received.data[i*WIDTH+k]=p[i*WIDTH+k];
            }
        }
    }
        printf("returened\n");
        cv::imshow("original",depth);
        std::string ty = type2str(depth.type()); 
        printf("Matrix: %s %dx%d \n", ty.c_str(), depth.cols, depth.rows);  
        //std::cout<<"depth_type: "<<depth.type()<<std::endl;
        
        
        cv::imshow("returned_disp",received);
        cv::waitKey(-1);
#endif
        printf("now4\n");

        //std::cout << p[0] << ',' << p[1] << ',' << p[2] << ',' << p[MAX_X * MAX_X - 1] << std::endl;
    
    return true;
}