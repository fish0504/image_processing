// Grab_MultipleCameras.cpp
/*
    Note: Before getting started, Basler recommends reading the "Programmer's Guide" topic
    in the pylon C++ API documentation delivered with pylon.
    If you are upgrading to a higher major version of pylon, Basler also
    strongly recommends reading the "Migrating from Previous Versions" topic in the pylon C++ API documentation.

    This sample illustrates how to grab and process images from multiple cameras
    using the CInstantCameraArray class. The CInstantCameraArray class represents
    an array of instant camera objects. It provides almost the same interface
    as the instant camera for grabbing.
    The main purpose of the CInstantCameraArray is to simplify waiting for images and
    camera events of multiple cameras in one thread. This is done by providing a single
    RetrieveResult method for all cameras in the array.
    Alternatively, the grabbing can be started using the internal grab loop threads
    of all cameras in the CInstantCameraArray. The grabbed images can then be processed by one or more
    image event handlers. Please note that this is not shown in this example.
*/

// Include files to use the pylon API.
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
// Namespace for using pylon objects.
using namespace Pylon;
float exposure_time = 2000.0; //microsecond
// Namespace for using cout.
using namespace std;
#define NOT_REALTIME 0
#define realtime 1
// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 10000;

#include"convert.cpp"
#include"stereomatch.cpp"
// Limits the amount of cameras used for grabbing.
// It is important to manage the available bandwidth when grabbing with multiple cameras.
// This applies, for instance, if two GigE cameras are connected to the same network adapter via a switch.
// To manage the bandwidth, the GevSCPD interpacket delay parameter and the GevSCFTD transmission delay
// parameter can be set for each GigE camera device.
// The "Controlling Packet Transmission Timing with the Interpacket and Frame Transmission Delays on Basler GigE Vision Cameras"
// Application Notes (AW000649xx000)
// provide more information about this topic.
// The bandwidth used by a FireWire camera device can be limited by adjusting the packet size.

std::mutex mtx;
cv::Mat cameraImage1;
cv::Mat cameraImage2;
cv::Mat cameraImages[2];
string window[2]={"right camera","left camera"};
cv::Mat left_img;
cv::Mat right_img;
void Display()
{

    
    cv::namedWindow(window[0]);//CV_WINDOW_NORMAL); 
    cv::namedWindow(window[1]);//,CV_WINDOW_NORMAL); 
    // Create an OpenCV image
    //cv::Mat openCvImage;//me
    cv::Mat frame[c_maxCamerasToUse];
    int cnt=0;
    printf("display_start!\n");
    //while(cnt<1e5)//!threads_exit.wait_for(lock, pause, [](){return !threads_run;}))
    init_stereomatch();
    init_converter();
    while(cnt<c_countOfImagesToGrab)//!threads_exit.wait_for(lock, pause, [](){return !threads_run;}))
    {
        cnt++;
         for (size_t i = 0; i<camerasDevices.numberOfCameras; i++){
            camerasDevices.cameraParams[i].imageBuffer.GetMat(frame[i], camerasDevices.cameraParams[i].width,
                                                              camerasDevices.cameraParams[i].height, camerasDevices.cameraParams[i].pixelormat);

            if (frame[i].cols > 0 && frame[i].rows > 0){
                //show the image
                cv::rotate(frame[i],frame[i],ROTATE_90_COUNTERCLOCKWISE);
               
                cv::imshow(window[i], frame[i]);

            }
           
         cv::waitKey(1);
        }
        if(cnt%10000)cv::Mat res=getDepthImage(frame[1],frame[0]);
     
    }
    printf("display end!\n");
    return ;
}
int main(int argc, char* argv[])
{
    // The exit code of the sample application.
    int exitCode = 0;

    // Before using any pylon methods, the pylon runtime must be initialized. 
    PylonInitialize();
   
    Pylon::PylonAutoInitTerm autoInitTerm;
    
    try
    {
        // Get the transport layer factory.
        CTlFactory& tlFactory = CTlFactory::GetInstance();

        // Get all attached devices and exit application if no device is found.
        DeviceInfoList_t devices;
        if ( tlFactory.EnumerateDevices(devices) == 0 )
        {
            throw RUNTIME_EXCEPTION( "No camera present.");
            // Releases all pylon resources.
            PylonTerminate();
        }

        // Create an array of instant cameras for the found devices and avoid exceeding a maximum number of devices.
       camerasDevices.numberOfCameras= std::min( devices.size(), c_maxCamerasToUse);
        camerasDevices.cameras.Initialize(camerasDevices.numberOfCameras);

        // Create and attach all Pylon Devices.
        // for ( size_t i = 0; i < cameras.GetSize(); ++i)
        // {
        //     cameras[ i ].Attach( tlFactory.CreateDevice( devices[ i ]));

        //     // Print the model name of the camera.
        //     cout << "Using device " << cameras[ i ].GetDeviceInfo().GetModelName() << endl;
        // }
                // Create and attach all Pylon Devices.
        for (size_t i=0; i<camerasDevices.cameras.GetSize(); ++i)
        {
            // attach the camera to camera array
            camerasDevices.cameras[i].Attach(tlFactory.CreateDevice(devices[i]));

            // Open the camera.
            camerasDevices.cameras[i].Open();

            //get the set image width and height
            camerasDevices.cameraParams[i].width = 800;//width.GetValue();
            camerasDevices.cameraParams[i].height = 600;//height.GetValue();

            //initialize frame count
            camerasDevices.frameCount[i] = 0;

            std::stringstream winName;
            winName << "Camera " << i + 1;
            camerasDevices.cameraParams[i].windowName = winName.str();

            
            // Print the model name of the camera.
            if (camerasDevices.cameras[i].GetDeviceInfo().IsSerialNumberAvailable())
                std::cout << "Using device " << camerasDevices.cameras[i].GetDeviceInfo().GetModelName() << "; Serial# " << camerasDevices.cameras[i].GetDeviceInfo().GetSerialNumber() << std::endl;
            else
                std::cout << "Using device " << camerasDevices.cameras[i].GetDeviceInfo().GetModelName() << std::endl;
        }

        
        CImageFormatConverter formatConverter;//me
        formatConverter.OutputPixelFormat = PixelType_BGR8packed;//me
        CPylonImage pylonImage;//me

        // Create an OpenCV image
        cv::Mat openCvImage;//me
        
        // Starts grabbing for all cameras starting with index 0. The grabbing
        // is started for one camera after the other. That's why the images of all
        // cameras are not taken at the same time.
        // However, a hardware trigger setup can be used to cause all cameras to grab images synchronously.
        // According to their default configuration, the cameras are
        // set up for free-running continuous acquisition.
        camerasDevices.cameras.StartGrabbing(GrabStrategy_LatestImages);

        // This smart pointer will receive the grab result data.
       
        

        // Grab c_countOfImagesToGrab from the cameras.
        
        bool display_started=false;
        clock_t start = clock();
        CGrabResultPtr ptrGrabResult;
        #if NOT_REALTIME
        cv::namedWindow("camera1");//CV_WINDOW_NORMAL); 
        cv::namedWindow("camera2");//,CV_WINDOW_NORMAL); 
        #endif
        printf("started!\n");
        for( uint32_t i = 0; i < c_countOfImagesToGrab && camerasDevices.cameras.IsGrabbing(); ++i)
        { 
            camerasDevices.cameras.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
            intptr_t cameraContextValue = ptrGrabResult->GetCameraContext();
            const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();

            //set the image buffer for later use
            camerasDevices.cameraParams[cameraContextValue].imageBuffer.SetBuffer(pImageBuffer, ptrGrabResult->GetBufferSize());
            camerasDevices.frameCount[cameraContextValue]++;
            //mtx.unlock();
            // When the cameras in the array are created the camera context value
            // is set to the index of the camera in the array.
            // The camera context is a user settable value.
            // This value is attached to each grab result and can be used
            // to determine the camera that produced the grab result.
            
             //const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
            //formatConverter.Convert(pylonImage, ptrGrabResult);//me
            
            // Create an OpenCV image
            
            // Create an OpenCV image out of pylon image
            //printf("cameraContextValue : %ld\n",cameraContextValue);
            //cv::Mat openCvImage;//me
            // openCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *)pylonImage.GetBuffer());//me
            // if (cameraContextValue == 0)
            // {
            //     cameraImages[0]=openCvImage;
            // }
            // else if (cameraContextValue == 1)
            // {
            //     cameraImages[1]=openCvImage;
            // }
            //cameraImages[cameraContextValue]=openCvImage;
            #if NOT_REALTIME
            if (cameraContextValue == 0)
            {
                //cameraImages[0]=openCvImage;
                cv::imshow("camera1", openCvImage);
                //cv::imwrite("right_img.png", openCvImage);
            }
            else if (cameraContextValue == 1)
            {
                cameraImages[1]=openCvImage;
                cv::imshow("camera2", openCvImage);
                //cv::imwrite("right_img.png", openCvImage);

            }
            cv::waitKey(1);
            #endif
            //cameraImages[cameraContextValue]=openCvImage;
            #if realtime
            if(!display_started && i>=500){
            std::thread display(Display);
            display.detach();
            display_started=true;
            }
            #endif
            //cv::waitKey(1);
            if(i%500==0){
                clock_t end = clock();
                const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
                printf("time %lf[ms]\n",time/500);
                printf("fps %lf[fps]\n",((1000.0)/ time)*500);
                start = clock();
            }

           
       
       
      
     
#ifdef PYLON_WIN_BUILD
            // Show the image acquired by each camera in the window related to each camera.
            Pylon::DisplayImage(cameraContextValue, ptrGrabResult);
#endif

            // Print the index and the model name of the camera.
            
#if 0
            cout << "Camera " <<  cameraContextValue << ": " << cameras[ cameraContextValue ].GetDeviceInfo().GetModelName() << endl;
            // Now, the image data can be processed.
            cout << "GrabSucceeded: " << ptrGrabResult->GrabSucceeded() << endl;
            cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
            cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
            const uint8_t *pImageBuffer = (uint8_t *)ptrGrabResult->GetBuffer();
            cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;
#endif
            
            
            
        }
    }
    catch (const GenericException &e)
    {
        // Error handling
        cerr << "An exception occurred." << endl
        << e.GetDescription() << endl;
        exitCode = 1;
    }

    // Comment the following two lines to disable waiting on exit.
    cerr << endl << "Press enter to exit." << endl;
    while( cin.get() != '\n');

    // Releases all pylon resources. 
    PylonTerminate(); 

    return exitCode;
}
