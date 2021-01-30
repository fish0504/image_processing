#include "convert.hpp"
#include"Angle_estimate_pixel.cpp"

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
    std::ifstream ifs_pyRealsense(pyRealsense_file);
    script_dex=std::string((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());
    script_mat=std::string((std::istreambuf_iterator<char>(ifs_mat)),
                        std::istreambuf_iterator<char>());
    script_pyRealsense=std::string((std::istreambuf_iterator<char>(ifs_pyRealsense)),
                        std::istreambuf_iterator<char>());


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


//bool convertToPython(){
int main(){
    init_converter();    
    //assert(!depth.empty());
    printf("now_convert1\n");
     //Python、numpyモジュールの初期化
    Py_Initialize();
    np::initialize();
    //名前空間の確保
    auto main_ns = boost::python::import("__main__").attr("__dict__");
    
    printf("now_convert2\n");
    //for the convertion cv::Mat depth to depth.npy
    
    
    //mat_numpy.mulの実行
    boost::python::exec(script_pyRealsense.c_str(),main_ns);
    printf("now_convert2.5\n");
   // auto func = main_ns["matConvertToNumpy"];
    printf("now_convert2.8\n");
    //std::cout<<func(A) <<std::endl;
    auto func = main_ns["use_realsense_py"];
    //
    auto pyresult=func();
    //printf("pyresult:%lf\n",pyresult);

    //結果の受け取り
    //stl_input_iteratorを使ってタプル全要素を受け取る
    printf("now3\n");
    
#if 1 //CONFIRM_IMAGES
        boost::python::stl_input_iterator<np::ndarray> begin(pyresult), end;
        std::list<np::ndarray> pyresult_list(begin, end);
        
        cv::Mat received(480,640,CV_8UC3);
    for(auto itr = pyresult_list.begin(); itr != pyresult_list.end(); ++itr) {
        unsigned char *p = reinterpret_cast<unsigned char *>((*itr).get_data());
        //double *p = reinterpret_cast<double *>((*itr).get_data());
        //ndarrayでは基本的にメモリは連続領域上に保持されるので、
        //各要素には[]演算子を使ってアクセスできる
        int HEIGHT=480;
        int WIDTH=640;
        // for(int i=0;i<HEIGHT;i++){
        //     for(int k=0;k<WIDTH;k++){
        //         for(int c=0;c<3;c++){
        //         received.data[i*WIDTH+k]=p[i*WIDTH+k];
        //         }
        //     }
        // }
        int width = received.cols;
        int height = received.rows;
        int channels = received.channels();
        printf("receive_frame:cols  %d  rows %d  channel %d\n",width,height,channels);
        for(int j=0; j<height; j++)
        {
            int step = j*width*channels;
            for(int i=0; i<width; i++)
            {
                int elm = i*channels;

                for(int c=0; c<channels; c++)
                {
                    received.data[step+elm+ c] = p[step+elm+c];
                }
            }
        }
        break;
    }
        printf("returened\n");
        
        //std::string ty = type2str(depth.type()); 
        //printf("Matrix: %s %dx%d \n", ty.c_str(), depth.cols, depth.rows);  
        //std::cout<<"depth_type: "<<depth.type()<<std::endl;
        
        
        cv::imshow("returned_color",received);
        cv::waitKey(-1);
        // std::ostringstream depth;
        // depth<<"/home/kawahara/dex-net-withoutdocker/gqcnn/data/examples/single_object/primesense/depth_"<<dexcnt<<".npy";
        // cv::imwrite(depth.str(),received);
#endif
    boost::python::exec(script_dex.c_str(),main_ns);

    printf("now4\n");    
    return true;
}