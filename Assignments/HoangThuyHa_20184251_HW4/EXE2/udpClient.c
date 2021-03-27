#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __WIN32__
#include <ws2tcpip.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

// #define SERV_PORT 1255
#define MAXLINE 255

int SERV_PORT;
char IP_ADDRESS[];

int StringToInt (char str[], int* num) 
{
  *num = 0;
  for (int i = 0; i < strlen(str); i++) 
  {
    if (str[i] < '0' || str[i] > '9') 
      return -1;
    *num = (*num)*10 + (str[i] - '0');    
  }
  return 1;
}

int main (int argc, char* argv[])
{
  // add IP_ADRESS
  if (argc < 2) 
  {
    perror("IP_ADDRESS failed");
    return 0;
  }
  strcpy(IP_ADDRESS, argv[1]);

  // add SERV_PORT
  if (argc < 3) 
  {
    perror("SERV_PORT failed");
    return 0;
  }
  if (StringToInt(argv[2], &SERV_PORT) == -1)
  {
    perror("SERV_PORT failed");
    return 0;
  }
  printf("SERV_PORT: %d\n", SERV_PORT);

  int sockfd, n, from_len;
  struct sockaddr_in servaddr, from_socket;
  socklen_t addrlen = sizeof(from_socket);
  char sendline[MAXLINE + 1], recvline[MAXLINE + 1];
  FILE *fp;

  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(SERV_PORT); 
  servaddr.sin_addr.s_addr=inet_addr(IP_ADDRESS);
  //inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr); 

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  printf("creat socket\n\n");

  while (fgets(sendline, MAXLINE, stdin) != NULL) 
  { 
    sendline[strlen(sendline)-1] = '\0'; // eliminate '\n'
    printf("To Server: %s\n", sendline);
    sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
    n = recvfrom(sockfd, recvline, MAXLINE, 0,  (struct sockaddr *) &from_socket, &addrlen);
    recvline[n] = '\0'; //null terminate  
    printf("From Server: %s %d\n%s\n\n", inet_ntoa(from_socket.sin_addr), htons(from_socket.sin_port), recvline);
  } 
}
