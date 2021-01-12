#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <signal.h>
#include <sstream>
#include"defs.h"
#include"stereomatch.cpp"
#include"Angle_estimate_pixel.cpp"

//#include"convert.hpp"
#include"matplotlib-cpp-starter/matplotlibcpp.h"

namespace plt = matplotlibcpp;
 std::vector<int>x;
 std::vector<double>y;
 std::vector<double>true_value;

const std::string window[2]={"right","left"};



// whether or not to show captured frames
bool enableDisplay = false;
const int c_countOfImagesToGrab=5000;
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
static CameraDevices camerasDevices;

//function declarations
void StreamingThread();
void DisplayThread();

int rotation_index=0;
#define realtime_display 0
#define use_boost_stereomatch 0
#define NOT_REALTIME 1