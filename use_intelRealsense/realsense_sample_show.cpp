// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include "../convert.cpp"
int main(int argc, char * argv[]) try
{
    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();

    using namespace cv;
    const auto window_name = "Display Image";
    namedWindow(window_name, WINDOW_AUTOSIZE);

    while (waitKey(1) < 0 && getWindowProperty(window_name, WND_PROP_AUTOSIZE) >= 0)
    {
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
        rs2::frame depth = data.get_depth_frame().apply_filter(color_map);
        rs2::frame color=data.get_color_frame();

        
        // Query frame size (width and height)
        const int w = depth.as<rs2::video_frame>().get_width();
        const int h = depth.as<rs2::video_frame>().get_height();

        const int wc = color.as<rs2::video_frame>().get_width();
        const int hc = color.as<rs2::video_frame>().get_height();
        // Create OpenCV matrix of size (w,h) from the colorized depth data
        Mat image(Size(w, h), CV_8UC3, (void*)depth.get_data(), Mat::AUTO_STEP);
        Mat color_image(Size(wc, hc), CV_8UC3, (void*)color.get_data(), Mat::AUTO_STEP);
        cv::cvtColor(color_image,color_image,cv::COLOR_RGB2BGR);


        cv::resize(color_image,color_image,cv::Size(w,h));
        //cv::imshow("color",color_image);
        cv::Mat mono_disp1,mono_disp2;
        //printf("Mat type:%d\n",image.type());

        cv::cvtColor(image,mono_disp1,cv::COLOR_RGB2GRAY);
        cv::Mat segmask;
        
        
        std::cout<<"width: "<<w<<"height :"<<h<<std::endl;
        cv::Rect roi(w/2-100,h/2-100,200,200);
        cv::rectangle(image,roi,cv::Scalar(255,0,0),2);
        //imwrite("depth_realsense.png",mono_disp1);

        cv::cvtColor(image,mono_disp2,cv::COLOR_BGR2GRAY);
        imshow("gray_disparity_bgr",mono_disp2);

        //convertion to float
        //mono_disp1.convertTo(mono_disp1,CV_32FC1);

        //convert depth image to ndarray
        convertToPython(mono_disp1);

         //binalize and generate sagmask
        cv::threshold(mono_disp1,mono_disp1,150,255,1);
        cv::imwrite("segmask.png",mono_disp1);
        cv::cvtColor(color_image,segmask,cv::COLOR_RGB2GRAY);


        //gray
        cv::imshow("segmask_gray",segmask);
        cv::threshold(segmask,segmask,100,255,0);
        cv::rectangle(segmask,roi,cv::Scalar(255),2);
        //binalized
        cv::imshow("segmask",segmask);
        imshow(window_name, image);
        
        
        cv::Mat black(w,h,CV_8UC1,cv::Scalar(0));
        cv::Mat target=segmask(roi);
        target.copyTo(black);
        cv::imshow("black_segmask",black);
        

       
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}