#include<iostream>
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include "../convert.cpp"
int main(){
    // Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline p;

    // Configure and start the pipeline
    p.start();

    // Block program until frames arrive
    rs2::frameset frames = p.wait_for_frames();

    // Try to get a frame of a depth image
    rs2::depth_frame depth = frames.get_depth_frame();
//     // Get the depth frame's dimensions
//     float width = depth.get_width();
//     float height = depth.get_height();

//     // Query the distance from the camera to the object in the center of the image
//     float dist_to_center = depth.get_distance(width / 2, height / 2);

//     // Print the distance
//     std::cout << "The camera is facing an object " << dist_to_center << " meters away \n";

//     // Query frame size (width and height)
const int w = depth.as<rs2::video_frame>().get_width();
const int h = depth.as<rs2::video_frame>().get_height();

// Create OpenCV matrix of size (w,h) from the colorized depth data
cv::Mat image(cv::Size(w, h), CV_8UC3, (void*)depth.get_data(), cv::Mat::AUTO_STEP);
cv::imshow("result",image);
cv::Mat mono(cv::Size(w, h), CV_32FC1);
cv::imshow("mono_img",mono);

cv::waitKey(-1);
}