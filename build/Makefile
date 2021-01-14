#~Q�~B��~C��~A��~L~G��~Z
CC = g++
# �~B��~C��~B��~C��~C��~C~I�~C~U�~B��~B��~C���~I
CFLAGS = -lm
LDFLAGS =-L /root/Workspace/opencv/lib  -std=gnu++11  -lpthread
LIBS =  -L /root/Workspace/opencv/lib -lopencv_core \
        -L /root/Workspace/opencv/lib -lopencv_highgui\
        -L /root/Workspace/opencv/lib -lopencv_cudaarithm\
        -L /root/Workspace/opencv/lib -lopencv_cudafilters\
        -L /root/Workspace/opencv/lib -lopencv_cudaimgproc\
        -L /root/Workspace/x86_64-linux-gnu -ljasper\
        -L /root/Workspace/x86_64-linux-gnu -ljbig\
        -L /root/Workspace/x86_64-linux-gnu -ltiff \
        -L /root/Workspace/opencv/lib -lopencv_imgcodecs\
        -L /root/Workspace/x86_64-linux-gnu -lpng16\
        -L /root/Workspace/opencv/lib -lopencv_imgproc\
        -L /root/Workspace/opencv/lib -lopencv_features2d\
        -L /root/Workspace/opencv/lib -lopencv_flann\
        -L /root/Workspace/opencv/lib -lopencv_calib3d \
        -L /root/Workspace/pylon/lib -lLog_gcc_v3_1_Basler_pylon\
        -L /root/Workspace/pylon/lib -Wl,-E -lpylonbase \
        -L /root/Workspace/pylon/lib -Wl,-E -lpylonutility \
        -L /root/Workspace/pylon/lib -Wl,-E -lNodeMapData_gcc_v3_1_Basler_pylon\
        -L /root/Workspace/pylon/lib -lMathParser_gcc_v3_1_Basler_pylon\
        -L /root/Workspace/pylon/lib -lXmlParser_gcc_v3_1_Basler_pylon\
        -L /root/Workspace/pylon/lib -Wl,-E -lGenApi_gcc_v3_1_Basler_pylon \
        -L /root/Workspace/pylon/lib -Wl,-E -lGCBase_gcc_v3_1_Basler_pylon \
        -L /root/Workspace/opencv/lib -lboost_atomic \
        -L /root/Workspace/opencv/lib -lboost_system \
        -L /root/Workspace/opencv/lib -lboost_thread \
        -L /root/Workspace/opencv/lib -lboost_signals \
        -L /root/Workspace/opencv/lib -lboost_numpy3 \
        -L /root/Workspace/opencv/lib -lboost_python3 \
        -L /usr/lib/python3.5/config-3.5m-x86_64-linux-gnu -lpython3.5

        #-lpython2.7 -lpython3.5
INCLUDE =   -I/root/Workspace/opencv/opencv4 \
                        -I/root/Workspace/pylon/include \
                        -I/usr/include/python3.5m \
                        -I/root/Workspace/boost_include
# ��~_��~L�~C~U�~B��~B��~C��~P~M
#TARGETS = main
TARGETS = convert_with_main


# compile
#main: main_camera_input.cpp
convert_with_main: main_camera_input.cpp
        $(CC) main_camera_input.cpp -o $(TARGETS) $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)
# make clean
