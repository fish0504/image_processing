# �~B��~C��~C~Q�~B��~C��~A��~L~G��~Z
CC = g++
# �~B��~C��~B��~C��~C��~C~I�~C~U�~B��~B��~C���~I
CFLAGS = -lm
LDFLAGS = -L/root/Workspace/opencv/lib -L/root/Workspace/pylon/lib -L/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu#-Wl,-E
LIBS = -lopencv_core \
        -lopencv_cudaimgproc\
        -lopencv_highgui\
        -lopencv_imgcodecs\
        -lopencv_imgproc\
        -lopencv_calib3d \
        -lpylonbase \
        -lpylonutility \
        -lGenApi_gcc_v3_1_Basler_pylon \
        -lGCBase_gcc_v3_1_Basler_pylon \
        -lboost_atomic \
        -lboost_system \
        -lboost_thread \
        -lboost_signals \
        -lboost_numpy3 \
        -lboost_python3 \
        -lpython3.5 \
        -lstdc++ \
        -pthread \
        -std=c++11

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
