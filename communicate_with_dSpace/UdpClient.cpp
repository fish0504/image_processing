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
 char *destination="10.1.196.179";
 //char *destination="127.0.0.1";
 unsigned short port = 50006;
 int destSocket;

/* sockaddr_in 構造体 */
 struct sockaddr_in destSockAddr;

/* 各種パラメータ */
 int i;
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
 for(i=0; i<10000; i++) {
   	std::this_thread::sleep_for(std::chrono::milliseconds(1));
  
   printf("sending...\n");
   sendto(destSocket,send_vec, 112, 0, (const sockaddr*)&destSockAddr, sizeof(destSockAddr));
   
 }

/* ソケットの終了 */
 close(destSocket);
}