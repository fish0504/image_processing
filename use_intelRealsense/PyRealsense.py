## License: Apache 2.0. See LICENSE file in root directory.
## Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.

###############################################
##      Open CV and Numpy integration        ##
###############################################

import pyrealsense2 as rs
import numpy as np
import cv2

def use_realsense_py():
    WIDTH = 640
    HEIGHT = 480
    FPS = 60
    THRESHOLD = 0.80  # これより遠い距離の画素を無視する
    #ここでスケールがかわっているので、あとで*THRESHOLDする
    # Configure depth and color streams
    pipeline = rs.pipeline()
    config = rs.config()
    #config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
    config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
    config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

    # Start streaming
    profile=pipeline.start(config)
    depth_scale = profile.get_device().first_depth_sensor().get_depth_scale()
    max_dist = THRESHOLD/depth_scale
    cnt=0
    show_result=False
    try:
        #while True:
        for i in range(1,50):
        # Wait for a coherent pair of frames: depth and color
            frames = pipeline.wait_for_frames()
            depth_frame = frames.get_depth_frame()
            color_frame = frames.get_color_frame()
            #if not depth_frame or not color_frame:
                #continue
            # RGB画像
            color_image = np.asanyarray(color_frame.get_data())
            
            # 指定距離以上を無視した深度画像
            depth_image = np.asanyarray(depth_frame.get_data())
            depth_filtered_image = (depth_image < max_dist) * depth_image
            depth_gray_filtered_image = (depth_filtered_image * 255. / max_dist).reshape((HEIGHT, WIDTH)).astype(np.uint8)
            #cv2.imwrite("depth_gray_filtered_image.png",depth_gray_filtered_image)
            #cv2.imshow("gray_filtered_depth_uint",depth_gray_filtered_image)
            depth_gray_filtered_image =np.float64(depth_gray_filtered_image)
            #scale をここでもとに戻す
            depth_gray_filtered_image_float=depth_gray_filtered_image*THRESHOLD/255.0
            
            #np.save("./dexInput/clear_depth_images",depth_gray_filtered_image_float)
            
            black = np.zeros((HEIGHT,WIDTH), np.uint8)
            left_, right_, top_, bottom_ = [260, 355, 160, 255]
            img2 = depth_gray_filtered_image[top_ : bottom_, left_: right_]

            black[top_ : bottom_, left_: right_]=img2
            #cv2.imwrite("out_sample2.jpg", img2)
            ret,black= cv2.threshold(black,0,255,cv2.THRESH_BINARY)
            if 0:#show depth results
                cv2.imshow("black",black)
                cv2.imshow("gray_filtered_depth_float",depth_gray_filtered_image_float)

            #cv2.imwrite("./dexInput/Psagemask.png",black)
            # 指定距離以上を無視したRGB画像
            color_filtered_image = (depth_filtered_image.reshape((HEIGHT, WIDTH, 1)) > 0) * color_image

            
            
            color_image = np.asanyarray(color_frame.get_data())
            #cv2.imshow('Realsense',depth_image)
            depth_gray_filtered_image= cv2.rectangle(depth_gray_filtered_image,(left_,top_),(right_,bottom_),(255),2)
            #cv2.imshow("disparity",depth_gray_filtered_image)
            # Apply colormap on depth image (image must be converted to 8-bit per pixel first)
            depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.03), cv2.COLORMAP_JET)
            #depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=1.00), cv2.COLORMAP_JET)

            # Stack both images horizontally
            images = np.hstack((color_image, depth_colormap))

            # Show images
            #cv2.namedWindow('RealSense', cv2.WINDOW_AUTOSIZE)
            if 0:
                cv2.imshow('depth_color',depth_colormap)
                cv2.imshow('color',color_image)
                np.save('Pydepth',depth_image)
            #if i==99:
            #    cv2.waitKey()
            #if key =='q':
            #    break

    finally:
    # Stop streaming
        pipeline.stop()
    if 0:#when debug Realsense RGB get
        print(color_image.dtype)
        print(color_image.ndim)
        print(color_image.shape)
        cv2.imshow("ret",color_image)
        cv2.waitKey()
    return color_image
#if __name__=='__main__':
#    use_realsense_py()