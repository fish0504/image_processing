# コンパイラの�?�?
CC = g++
# インクルードファイル�?
CFLAGS = -I/usr/local/include  -I/usr/local/include/opencv4 -std=c++11 -pthread -lm
LDFLAGS = -L/usr/lib/x86_64-linux-gnu
LIBS =-lboost_atomic -lboost_system  -lboost_thread -lboost_signals 
INCLUDE =
# 実行ファイル�?
TARGETS = udp_send2myself

# compile
udp_send2myself: UdpClient.cpp
	$(CC) UdpClient.cpp -o $(TARGETS) $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)
# make clean
