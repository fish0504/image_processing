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

   
    //Pythonスクリプトの読み込み
    //std::ifstream ifs("mat_numpy.py");
    std::ifstream ifs(pyfile);
    std::ifstream ifs_mat(matTonumpy_file);
    script_dex=std::string((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());
    script_mat=std::string((std::istreambuf_iterator<char>(ifs_mat)),
                        std::istreambuf_iterator<char>());

}

bool convertToPython(cv::Mat depth){
    //assert(!depth.empty());
    printf("now_convert1\n");
     //Python、numpyモジュールの初期化
    Py_Initialize();
    np::initialize();
    //名前空間の確保
    auto main_ns = boost::python::import("__main__").attr("__dict__");
    
    printf("now_convert2\n");
    // //for the convertion cv::Mat depth to depth.npy
    // int HEIGHT=depth.rows;
    // int WIDTH=depth.cols;
    // //100x100行列の準備
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
    //     }
    // }
    
    //mat_numpy.mulの実行
    boost::python::exec(script_dex.c_str(),main_ns);
    printf("now_convert2.5\n");
    auto func = main_ns["dexnet"];
    printf("now_convert2.8\n");
    func();
    //auto func = main_ns["matConvertToNumpy"];
    //
    //auto pyresult=func(A);
    //printf("pyresult:%lf\n",pyresult);

    //結果の受け取り
    //stl_input_iteratorを使ってタプル全要素を受け取る
    printf("now3\n");
    
#if 0 //CONFIRM_IMAGES
        boost::python::stl_input_iterator<np::ndarray> begin(pyresult_numpy), end;
        std::list<np::ndarray> pyresult_list(begin, end);
        cv::Mat received(600,800,CV_8UC1);
    for(auto itr = pyresult_list.begin(); itr != pyresult_list.end(); ++itr) {
        unsigned char *p = reinterpret_cast<unsigned char *>((*itr).get_data());
        //double *p = reinterpret_cast<double *>((*itr).get_data());
        //ndarrayでは基本的にメモリは連続領域上に保持されるので、
        //各要素には[]演算子を使ってアクセスできる
        int HEIGHT=600;
        int WIDTH=800;
        for(int i=0;i<HEIGHT;i++){
            for(int k=0;k<WIDTH;k++){
                received.data[i*WIDTH+k]=p[i*WIDTH+k];
            }
        }
    }
        printf("returened\n");
        
        //std::string ty = type2str(depth.type()); 
        //printf("Matrix: %s %dx%d \n", ty.c_str(), depth.cols, depth.rows);  
        //std::cout<<"depth_type: "<<depth.type()<<std::endl;
        
        
        cv::imshow("returned_disp",received);
        cv::waitKey(-1);
        std::ostringstream depth;
        depth<<"/home/kawahara/dex-net-withoutdocker/gqcnn/data/examples/single_object/primesense/depth_"<<dexcnt<<".npy";
        cv::imwrite(depth.str(),received);
#endif
        printf("now4\n");    
    return true;
}