#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#define BUFFER_SIZE 256
#define data_num 14
int main() {
/* ポ�?�ト番号、ソケ�?�? */
 unsigned short port = 50006;
 int recvSocket;

/* sockaddr_in 構�?�? */
 struct sockaddr_in recvSockAddr;

/* �?種パラメータ */
 int status;
 int numrcv;
 //char buffer[BUFFER_SIZE];
 char buffer[data_num*8];
 unsigned long on = 1;

/* sockaddr_in 構�?体�?�セ�?�? */
 memset(&recvSockAddr, 0, sizeof(recvSockAddr));
 recvSockAddr.sin_port = htons(port);
 recvSockAddr.sin_family = AF_INET;
 recvSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

/* ソケ�?ト生�? */
 recvSocket = socket(AF_INET, SOCK_DGRAM, 0);

 /* バイン�? */
 status = bind(recvSocket, (const struct sockaddr *) &recvSockAddr, sizeof(recvSockAddr));

/* パケ�?ト受信 */
 while(1) {
   numrcv = recvfrom(recvSocket, buffer, BUFFER_SIZE, 0, NULL, NULL);
   if(numrcv == -1) { status = close(recvSocket); break; }
   printf("received: %c\n", buffer[5]);
 }
}