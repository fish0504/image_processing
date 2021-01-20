#ifndef DEFS_H
#define DEFS_H

#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
//added
//#include "opencv2/cudaobjdetect.hpp"
//#include "opencv2/cudaimgproc.hpp"
//#include "opencv2/cudawarping.hpp"


// Include files to use the pylon API.
#include <pylon/PylonIncludes.h>

// to suppress parameter unused warning
#define UNUSED(x) (void)x;

// use cuda for dnn
#define DNN_GPU

// Namespace for using pylon objects.
using namespace Pylon;

// Limits the amount of cameras used for grabbing.
// It is important to manage the available bandwidth when grabbing with multiple cameras.
// This applies, for instance, if two GigE cameras are connected to the same network adapter via a switch.
// To manage the bandwidth, the GevSCPD interpacket delay parameter and the GevSCFTD transmission delay
// parameter can be set for each GigE camera device.
// The "Controlling Packet Transmission Timing with the Interpacket and Frame Transmission Delays on Basler GigE Vision Cameras"
// Application Notes (AW000649xx000)
// provide more information about this topic.
// The bandwidth used by a FireWire camera device can be limited by adjusting the packet size.
const size_t c_maxCamerasToUse = 2;

const int image_width  = 800;
const int image_height = 600;


// A buffer class for image exchange between streaming and dnn threads
class ImageBuffer
{
public:
    ImageBuffer(): buffer(nullptr), size(0) {}
    virtual ~ImageBuffer(){ DeleteBuffer(); }

    // Set the buffer with the captured image
    void SetBuffer(const uint8_t* srcBuffer, size_t bufferSize){
        if (size < bufferSize)
            AllocateBuffer(bufferSize);
        memcpy(buffer, srcBuffer, bufferSize);
    }

    // Get the most recent captured image
    void GetMat(cv::Mat& aMat, int64_t width, int64_t height, String_t pixelformat) {
        //printf("getting_mat...\n");
        if (!buffer) return;
        //printf("getting_mat2...\n");
        cv::cuda::GpuMat inputMat, outputMat;
        cv::Mat srcMat(height, width, CV_8UC1, (void*)buffer);

        inputMat.upload(srcMat, stream);
        if (pixelformat == "Mono8"){
            //convert the grayscale image to RGB for processing and display
            cv::cuda::cvtColor(inputMat, outputMat, cv::COLOR_GRAY2RGB, 0, stream);


            //cv::cvtColor(srcMat,srcMat,cv::COLOR_GRAY2RGB,0);
            //cv::cvtColor(inputMat, outputMat, cv::COLOR_GRAY2RGB,0);
            //outputMat=inputMat;
        }else{
            //the color image is in BayerBG8, need to convert ot RGB for processing and display
            //cv::cuda::cvtColor(inputMat, outputMat, cv::COLOR_BayerBG2RGB, 0, stream);
            cv::cuda::cvtColor(inputMat, outputMat, cv::COLOR_BayerBG2GRAY, 0, stream);


            //cv::cvtColor(srcMat,srcMat, cv::COLOR_BayerBG2RGB,0);
            //outputMat=inputMat;
        }
        
        //printf("Got_mat!\n");
        outputMat.download(aMat, stream);
        if(aMat.empty()){
            printf("getting_mat_failed!\n");
        }
        //cv::imshow("get_mat",aMat);
        return;
    }

private:

    void AllocateBuffer(size_t bufferSize){
        DeleteBuffer();
        buffer = new uint8_t[bufferSize];
        size = bufferSize;
    }

    void DeleteBuffer(){
        if (buffer){
            delete[] buffer;
            size = 0;
        }
    }

    //raw buffer storage
    uint8_t* buffer;
    size_t size;

    // cuda stream for asynchronous operation
    cv::cuda::Stream stream;

};

//camera parameters
struct CameraParams{
    int64_t width;
    int64_t height;
    String_t pixelormat;
    std::string windowName;
    ImageBuffer imageBuffer;
};

//camera devices
struct CameraDevices{
    size_t numberOfCameras;
    CInstantCameraArray cameras;
    CameraParams cameraParams[c_maxCamerasToUse];
    uint64_t frameCount[c_maxCamerasToUse];

};

#endif // DEFS_H
