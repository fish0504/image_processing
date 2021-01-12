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
#include<time.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;

// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 5000;
#define endOfPictures 96
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
const string saving_path_left="../../resources/rotation_estimate/left";
const string saving_path_right="../../resources/rotation_estimate/left";
constexpr int TIME_TO_SLEEP = 3850;

//This program take the pictures for calibration or rotation estimation
int main(int argc, char* argv[])
{
    // The exit code of the sample application.
    int exitCode = 0;
    using std::this_thread::sleep_for;
    // Before using any pylon methods, the pylon runtime must be initialized. 
    PylonInitialize();

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
        
        cv::namedWindow("left camera");//CV_WINDOW_NORMAL); 
        cv::namedWindow("right camera");//,CV_WINDOW_NORMAL); 
        // Grab c_countOfImagesToGrab from the cameras.
        int picNumber=0;
       bool takenPicture[2]={0};

       CGrabResultPtr ptrGrabResult;
       cameras.StartGrabbing(GrabStrategy_LatestImages);
       double now_angle=0.0;
        for( uint32_t i = 0; i < c_countOfImagesToGrab; ++i)
        {
            //cameras.GrabOne( 1000, ptrGrabResult);
           if(picNumber==endOfPictures)break;
        // This smart pointer will receive the grab result data.
            
            cameras.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);

            // When the cameras in the array are created the camera context value
            // is set to the index of the camera in the array.
            // The camera context is a user settable value.
            // This value is attached to each grab result and can be used
            // to determine the camera that produced the grab result.
            intptr_t cameraContextValue = ptrGrabResult->GetCameraContext();

#ifdef PYLON_WIN_BUILD
            // Show the image acquired by each camera in the window related to each camera.
            Pylon::DisplayImage(cameraContextValue, ptrGrabResult);
#endif

            // Print the index and the model name of the camera.
            cout << "Camera " <<  cameraContextValue << ": " << cameras[ cameraContextValue ].GetDeviceInfo().GetModelName() << endl;

            // Now, the image data can be processed.
            cout << "GrabSucceeded: " << ptrGrabResult->GrabSucceeded() << endl;
            cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
            cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
            const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
            cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;

            formatConverter.Convert(pylonImage, ptrGrabResult);//me
            // Create an OpenCV image out of pylon image
            openCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *)pylonImage.GetBuffer());//me
            std::ostringstream oss;
            cv::rotate(openCvImage,openCvImage,cv::ROTATE_90_COUNTERCLOCKWISE);
            if (cameraContextValue == 0)
            {
                oss<<saving_path_left<<now_angle<<".png";
                //cv::imshow(oss.str(), openCvImage);
                //cv::imwrite("left_img.png", openCvImage);
            }
            else if (cameraContextValue == 1)
            {

                oss<<saving_path_right<<now_angle<<".png";
                //cv::imshow(oss.str(), openCvImage);
                //cv::imwrite("right_img.png", openCvImage);

            }

            printf("picture %d |camera %ld \n",picNumber,cameraContextValue);
            cv::waitKey(1);
            printf("go to the next picture\n");
            
            //int key=cv::waitKey(1500);
           
            //if(key=='y' || key=='Y'){
                //cv::threshold(openCvImage,openCvImage,)
                cv::imwrite(oss.str(), openCvImage);
                cv::destroyAllWindows();
                takenPicture[cameraContextValue]=true;
                if(takenPicture[0]&takenPicture[1]){
                    picNumber++;
                    now_angle+=360.0/endOfPictures;
                    takenPicture[0]=takenPicture[1]=false;
                    sleep_for(std::chrono::milliseconds(TIME_TO_SLEEP));
                    imshow("now_image",openCvImage);
                    //printf("set_next_target if you are ready, please press r:");
                    // int next=cv::waitKey(1);
                    
                    // if(next=='r')continue;
                }
            // }
            // else{
            //     takenPicture[0]=takenPicture[1]=false;
            //     cv::destroyAllWindows();
            // }
             continue;
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
