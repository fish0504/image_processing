# コンパイラの指定
CC = g++
# インクルードファイル等
CFLAGS = -I/usr/local/include  -I/usr/local/include/opencv4 -std=c++11 -pthread -lm
LDFLAGS = -L/usr/lib/x86_64-linux-gnu
LIBS =-lboost_atomic -lboost_system  -lboost_thread -lboost_signals 
INCLUDE =
# 実行ファイル名
TARGETS = eosens_sample

# compile
upd_send: UdpClient.cpp
	$(CC) UdpClient.cpp -o $(TARGETS) $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)
# make clean
