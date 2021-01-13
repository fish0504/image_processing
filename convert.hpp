#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <list>
#include <opencv2/opencv.hpp>
#define MAX_X 100 //受け渡したい行列のサイズ。
//#define WIDTH 800
//#define HEIGHT 600
#define CONFIRM_IMAGES 0            //debug show the result image and original one and its mat type
namespace np = boost::python::numpy;
std::string type2str(int type);
bool convertToPython(cv::Mat depth);  //execute convertion c++ to python
void init_converter();              //init and prepare
std::string script;

