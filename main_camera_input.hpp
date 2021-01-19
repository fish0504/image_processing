#include <pylon/PylonIncludes.h>
#include <opencv2/opencv.hpp>
#include<thread>
#include<mutex>
#include<time.h>
#include "defs.h"
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <signal.h>
#include <sstream>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif
static CameraDevices camerasDevices;

#include"stereomatch.cpp"
#include"Angle_estimate_pixel.cpp"
#define output_angles 0

#if output_angles
#include"matplotlib-cpp-starter/matplotlibcpp.h"
namespace plt = matplotlibcpp;
 
#endif
 std::vector<int>x;
 std::vector<double>y;
 std::vector<double>true_value;

std::mutex mtx;
cv::Mat cameraImage1;
cv::Mat cameraImage2;
cv::Mat cameraImages[2];
const string window[2]={"left camera","right camera"};
cv::Mat left_img;
cv::Mat right_img;

// whether or not to show captured frames
bool enableDisplay = false;
static const uint32_t c_countOfImagesToGrab = 50000;
// exposure time default
float exposure_time = 2000.0; //microsecond

// Namespace for using GenApi objects.
using namespace GenApi;

std::atomic<bool> threads_run;

//thread synchronization
std::condition_variable threads_exit;
std::mutex threads_mutex;

//streaming thread captures and displays frames from the cameras
std::thread streaming_thread;

//display thread
std::thread display_thread;

//yolo dnn object
//object for holding camera devices and configurations

//function declarations

int rotation_index=0;
#define realtime_display 1
#define use_boost_stereomatch 1
#define NOT_REALTIME 0