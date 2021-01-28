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
        //rs2::frame depth=data.get_depth_frame();
        
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
        cv::Mat mono_disp1,mono_disp2;
        //printf("Mat type:%d\n",image.type());


        cv::cvtColor(image,mono_disp1,cv::COLOR_RGB2GRAY);
        cv::Mat segmask;
        
        
        std::cout<<"width: "<<w<<"height :"<<h<<std::endl;
        cv::Rect roi(w/2-100+20,h/2-100+30,100,100);
        cv::rectangle(image,roi,cv::Scalar(255,0,0),2);
        //imwrite("depth_realsense.png",mono_disp1);

        cv::cvtColor(image,mono_disp2,cv::COLOR_BGR2GRAY);

        //gray image
        cv::Mat mono;
        cv::cvtColor(image,mono,cv::COLOR_BGR2GRAY);
       
        //get binalized image
        cv::threshold(mono,segmask,100,255,0);
        

        //convertion to float
        //mono_disp1.convertTo(mono_disp1,CV_32FC1);

        //convert depth image to ndarraymake
        cv::resize(mono,mono,cv::Size(640,480));
        convertToPython(mono);

         //binalize and generate sagmask
        cv::threshold(mono_disp1,mono_disp1,150,255,1);
        
        // ROI
        Mat black = Mat(segmask.rows,segmask.cols, CV_8UC1, Scalar(0));
        Mat img_roi = segmask(roi);   // 元画像のROIを生成
        // copyto
        Mat target_roi=black(roi);
        img_roi.copyTo(target_roi);   // 貼り付ける画像をROIにコピー
        imshow("Color ROI copy",black);   // こうすると貼り付けできる
        std::cout<<"segmask width "<<black.cols<<" height :"<<black.rows<<std::endl;
        cv::resize(black,black,Size(640,480));
        //display result
        cv::rectangle(segmask,roi,cv::Scalar(255),2);
       cv::Mat src1,src2,show_image;
       cv::hconcat(image,color_image,src1);
       cv::hconcat(mono,segmask,src2);
       //cv::vconcat(src1,src2,show_image);
       imshow("disp_results",src1);
       imshow("gray to bin",src2);
        
        //save segmask image
        cv::imwrite("segmask.png",black);
       
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