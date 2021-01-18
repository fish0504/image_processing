#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <streambuf>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <list>
#include <opencv2/opencv.hpp>
#define MAX_X 100 //受け渡したい行列のサイズ。
//#define WIDTH 800
//#define HEIGHT 600
#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB
#define CONFIRM_IMAGES 0            //debug show the result image and original one and its mat type
namespace np = boost::python::numpy;
std::string type2str(int type);
bool convertToPython(cv::Mat depth);  //execute convertion c++ to python
void init_converter();                //init and prepare
std::string script_dex;
std::string script_mat;
const std::string pyfile="/home/kawahara/dex-net-withoutdocker/gqcnn/examples/policy.py";
const std::string matTonumpy_file="/home/kawahara/programs/image_input/mat_numpy.py";
std::string y_file = "/home/kawahara/programs/image_input/dex_result/results.yaml";

