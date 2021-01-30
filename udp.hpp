#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include<array>
#include <thread>
#include<chrono>
#define data_num 14         //送信するデータの数
#define limit_packet 100000 //送信するパケット数の上限

double goal_pos[4]={5.0,10.0,15.0,30.0};

//ソケット通信でdSpaceとUDP通信を行い
//ハンド制御の目標座標を送信する
void UDPsend(){

/* IPアドレス*/

 char *destination="192.168.140.9";  //
 //char *destination="127.0.0.1"; //to myself　デバック用
 unsigned short port = 50006;
 int destSocket;

/* sockaddr_in 構�?�? */
 struct sockaddr_in destSockAddr;

// 各種パラメータ 
 
double  send_vec[data_num];
for(int k=0;k<data_num;k++)send_vec[k]=k*1.2;

/* 相手�?�アドレスの入�? */
 printf("Connect to IP address %s  Port : %d\n",destination,port);
 //scanf("%s", destination);
 
 memset(&destSockAddr, 0, sizeof(destSockAddr));
 destSockAddr.sin_addr.s_addr = inet_addr(destination);
 destSockAddr.sin_port = htons(port);
 destSockAddr.sin_family = AF_INET;

//ソケット生成
 destSocket = socket(AF_INET, SOCK_DGRAM, 0);
 //パケット送信
 for(int i=0; i<limit_packet; i++) {
   	std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for(int i=0;i<4;i++)send_vec[i]=goal_pos[i];

  
   int now_sending=sendto(destSocket,send_vec, sizeof(send_vec), 0, (const sockaddr*)&destSockAddr, sizeof(destSockAddr));
//printf("sending... %d bytes\n",now_sending);
 }

//ソケット通信終了
 close(destSocket);
 printf("UDP sending end!\n");
}