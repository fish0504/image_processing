# コンパイラの指定
CC = g++
# インクルードファイル等
CFLAGS = -lm
LDFLAGS = -L/usr/local/lib -L/opt/pylon/lib #-Wl,-E
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
	-lpython2.7 \
	-lstdc++ \
	-pthread \
	-std=c++11 
INCLUDE =   -I/usr/local/include/opencv4 \
			-I/opt/pylon/include 
# 実行ファイル名
TARGETS = auto_photo

# compile
auto_photo: take_pictures.cpp
	$(PRE)$(CC) take_pictures.cpp -o $(TARGETS) $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)
# make clean
