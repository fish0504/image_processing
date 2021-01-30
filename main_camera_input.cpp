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
#include "main_camera_input.hpp"


void Display()
{

    
    cv::namedWindow(window[0]);//CV_WINDOW_NORMAL); 
    cv::namedWindow(window[1]);//,CV_WINDOW_NORMAL); 
    // Create an OpenCV image
    //cv::Mat openCvImage;//me
    cv::Mat frame[c_maxCamerasToUse];
    int cnt=0;
    printf("display_start!\n");
#if use_boost_stereomatch
    init_stereomatch();
    init_converter();
#endif
    //cv::Rect roi_1(cv::Point2i(0,(w-roi_width)-300),cv::Size(h,roi_width));
    //cv::Rect roi_2(cv::Point2i(0,(w-roi_width)-300),cv::Size(h,roi_width));
    Angle_estimate_init();
    
    clock_t start2 = clock();
    clock_t end2;
    double now_true_angle=0.0;
    while(cnt<c_countOfImagesToGrab)//!threads_exit.wait_for(lock, pause, [](){return !threads_run;}))
    {
        x.push_back(cnt),y.push_back(angles[rotation_index]);
        true_value.push_back(now_true_angle);
        cnt++;
        
         for (size_t i = 0; i<camerasDevices.numberOfCameras; i++){
            camerasDevices.cameraParams[i].imageBuffer.GetMat(frame[i], camerasDevices.cameraParams[i].width,
                                                              camerasDevices.cameraParams[i].height, camerasDevices.cameraParams[i].pixelormat);

            if (frame[i].cols > 0 && frame[i].rows > 0){
                //show the image
                cv::rotate(frame[i],frame[i],ROTATE_90_COUNTERCLOCKWISE);
            }
        end2=clock();
        double time2 = static_cast<double>(end2 - start2) / CLOCKS_PER_SEC;
        start2=clock();
        if(frame[0].empty() || frame[1].empty())continue;
       
        
        }
       
        
       
        
       
        //cv::rotate(frame[0],frame[0],ROTATE_90_COUNTERCLOCKWISE);
        //cv::rotate(frame[1],frame[1],ROTATE_90_COUNTERCLOCKWISE);
        cv::imshow(window[0], frame[0]);
      
         int Key=cv::waitKey(1);
        if(Key=='q')break;
        
        if(cnt%300==0){
             //left,right
            
            dexnet_thread=std::thread();
            dexnet_thread.detach();
            //dexnet_thread.detach();
            //std::thread DEX(getDepthImage,frame[1],frame[0]);
            
        }
        if(cnt%50==0){
            printf("now_estimated_angle %lf\n",angles[rotation_index]);
        }
        

        rotation_index=estimate_Angular(rotation_index,frame[1],frame[0]);

    }
    printf("display end!\n");
    if(dexnet_thread.joinable())dexnet_thread.join();
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
        
        printf("cameras input started!\n");
        for( uint32_t i = 0; i < c_countOfImagesToGrab && camerasDevices.cameras.IsGrabbing(); ++i)
        { 
            camerasDevices.cameras.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
            intptr_t cameraContextValue = ptrGrabResult->GetCameraContext();
            const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();

            //set the image buffer for later use
            camerasDevices.cameraParams[cameraContextValue].imageBuffer.SetBuffer(pImageBuffer, ptrGrabResult->GetBufferSize());
            camerasDevices.frameCount[cameraContextValue]++;

            #if realtime_display
            if(!display_started && i>=500){
            display_thread= std::thread(Display);

            //display_thread.detach();
            display_started=true;
            }
            #endif
            //cv::waitKey(1);
            if(i%1000==0){
                clock_t end = clock();
                const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
                printf("time %lf[ms]\n",time/1000);
                printf("fps %lf[fps]\n",((1000.0)/ time)*1000);
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
        //NotifyThreadsToExit();
        if (display_thread.joinable())
        display_thread.join();
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

    //ploting estimated angles
#if output_angles
    cout<<"matplotlib-cpp sample start"<<endl;
    plt::plot(x, y,"-b");
    //plt::plot(x,true_value,"-r");
    plt::show();
#endif
    return exitCode;
}