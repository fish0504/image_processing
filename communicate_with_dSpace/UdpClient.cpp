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
#define data_num 14
int main() {
/* IPアドレス、ポート番号、ソケット */
 //char destination[80];
 char *destination="10.1.196.179";//as external device to dSpace
 //char *destination="192.168.140.7";as host PC
 //char *destination="127.0.0.1"; //to myself
 unsigned short port = 50006;
 int destSocket;

/* sockaddr_in 構造体 */
 struct sockaddr_in destSockAddr;

/* 各種パラメータ */
 
double send_vec[data_num];
for(int k=0;k<data_num;k++)send_vec[k]=k*1.0;
/* 相手先アドレスの入力 */
 printf("Connect to IP address %s  Port : %d\n",destination,port);
 //scanf("%s", destination);
 
 memset(&destSockAddr, 0, sizeof(destSockAddr));
 destSockAddr.sin_addr.s_addr = inet_addr(destination);
 destSockAddr.sin_port = htons(port);
 destSockAddr.sin_family = AF_INET;

/* ソケット生成 */
 destSocket = socket(AF_INET, SOCK_DGRAM, 0);

 /* パケット送出 */
 for(int i=0; i<10000; i++) {
   	std::this_thread::sleep_for(std::chrono::milliseconds(1));
  
  
   int now_sending=sendto(destSocket,send_vec, 112, 0, (const sockaddr*)&destSockAddr, sizeof(destSockAddr));
    printf("sending... %d bytes\n",now_sending);
 }

/* ソケットの終了 */
 close(destSocket);
}