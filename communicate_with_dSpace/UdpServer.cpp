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
/* ãƒã?¼ãƒˆç•ªå·ã€ã‚½ã‚±ãƒ?ãƒ? */
 unsigned short port = 50006;
 int recvSocket;

/* sockaddr_in æ§‹é€?ä½? */
 struct sockaddr_in recvSockAddr;

/* å?ç¨®ãƒ‘ãƒ©ãƒ¡ãƒ¼ã‚¿ */
 int status;
 int numrcv;
 //char buffer[BUFFER_SIZE];
 char buffer[data_num*8];
 unsigned long on = 1;

/* sockaddr_in æ§‹é€?ä½“ã?®ã‚»ãƒ?ãƒ? */
 memset(&recvSockAddr, 0, sizeof(recvSockAddr));
 recvSockAddr.sin_port = htons(port);
 recvSockAddr.sin_family = AF_INET;
 recvSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

/* ã‚½ã‚±ãƒ?ãƒˆç”Ÿæˆ? */
 recvSocket = socket(AF_INET, SOCK_DGRAM, 0);

 /* ãƒã‚¤ãƒ³ãƒ? */
 status = bind(recvSocket, (const struct sockaddr *) &recvSockAddr, sizeof(recvSockAddr));

/* ãƒ‘ã‚±ãƒ?ãƒˆå—ä¿¡ */
 while(1) {
   numrcv = recvfrom(recvSocket, buffer, BUFFER_SIZE, 0, NULL, NULL);
   if(numrcv == -1) { status = close(recvSocket); break; }
   printf("received: %c\n", buffer[5]);
 }
}