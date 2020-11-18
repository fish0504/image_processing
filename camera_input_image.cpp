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
//#include "defs.h"
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;

// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 5000;

// Limits the amount of cameras used for grabbing.
// It is important to manage the available bandwidth when grabbing with multiple cameras.
// This applies, for instance, if two GigE cameras are connected to the same network adapter via a switch.
// To manage the bandwidth, the GevSCPD interpacket delay parameter and the GevSCFTD transmission delay
// parameter can be set for each GigE camera device.
// The "Controlling Packet Transmission Timing with the Interpacket and Frame Transmission Delays on Basler GigE Vision Cameras"
// Application Notes (AW000649xx000)
// provide more information about this topic.
// The bandwidth used by a FireWire camera device can be limited by adjusting the packet size.
static const size_t c_maxCamerasToUse = 2;
std::mutex mtx;
cv::Mat cameraImage1;
cv::Mat cameraImage2;
cv::Mat cameraImages[2];
string window[2]={"right camera","left camera"};
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
    while(cnt<100000)//!threads_exit.wait_for(lock, pause, [](){return !threads_run;}))
    {
        //mtx.lock();
        //for(int p=0;p<2;p++){
            if(cameraImages[0].empty()||cameraImages[1].empty()){
                printf("no image!\n");
                continue;
            }
        cv::imshow(window[0],cameraImage1);
        cv::imshow(window[1],cameraImage2);
        //cv::imshow("left camera",cameraImage2);
        //if(!cameraImage1.empty() && cnt%2==0)cv::imshow("right camera",cameraImage1);
        // else{
        //     printf("not_exist!\n");
        // }
        //if(!cameraImage2.empty()&& cnt%2==1)cv::imshow("left camera",cameraImage2);
        // else{
        //     printf("not_exist!\n");
        // }
        //mtx.unlock();
         cv::waitKey();
       // }
        cnt++;
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
    //PylonInitialize();
    //PylonTerminate(); 
    //return 0;
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
        }

        // Create an array of instant cameras for the found devices and avoid exceeding a maximum number of devices.
        CInstantCameraArray cameras( min( devices.size(), c_maxCamerasToUse));

        // Create and attach all Pylon Devices.
        for ( size_t i = 0; i < cameras.GetSize(); ++i)
        {
            cameras[ i ].Attach( tlFactory.CreateDevice( devices[ i ]));

            // Print the model name of the camera.
            cout << "Using device " << cameras[ i ].GetDeviceInfo().GetModelName() << endl;
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
        cameras.StartGrabbing();

        // This smart pointer will receive the grab result data.
       
        

        // Grab c_countOfImagesToGrab from the cameras.
        
        bool display_started=false;
        clock_t start = clock();
        CGrabResultPtr ptrGrabResult;
        cv::namedWindow("camera1");//CV_WINDOW_NORMAL); 
        cv::namedWindow("camera2");//,CV_WINDOW_NORMAL); 
        
        for( uint32_t i = 0; i < c_countOfImagesToGrab && cameras.IsGrabbing(); ++i)
        {
            
           
            
            cameras.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
            //mtx.unlock();
            // When the cameras in the array are created the camera context value
            // is set to the index of the camera in the array.
            // The camera context is a user settable value.
            // This value is attached to each grab result and can be used
            // to determine the camera that produced the grab result.
            intptr_t cameraContextValue = ptrGrabResult->GetCameraContext();
             //const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
            formatConverter.Convert(pylonImage, ptrGrabResult);//me
            
            // Create an OpenCV image
            
            // Create an OpenCV image out of pylon image
            //printf("cameraContextValue : %ld\n",cameraContextValue);
            //cv::Mat openCvImage;//me
            openCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *)pylonImage.GetBuffer());//me
           
            //cameraImages[cameraContextValue]=openCvImage;
        if (cameraContextValue == 0)
            {
                //cameraImage1=openCvImage;
                cv::imshow("camera1", openCvImage);
                
                //imwrite("right_img.png", openCvImage);
            }
            else if (cameraContextValue == 1)
            {
               // cameraImage2=openCvImage;
                cv::imshow("camera2", openCvImage);
                //cv::waitKey(1);
                //imwrite("right_img.png", openCvImage);

            }
            cv::waitKey(1);
            if(i%500==0){
                clock_t end = clock();
                const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
                printf("time %lf[ms]\n",time/500);
                printf("fps %lf[fps]\n",((1000.0)/ time)*500);
                start = clock();
            }

           
        // Create an OpenCV image
       
      
     
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
            
            cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;
#endif
            if(!display_started && i>=500&&0){
            std::thread display(Display);
            display.detach();
            display_started=true;
            }
            
            
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
