#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __WIN32__
#include <ws2tcpip.h>
#include <winsock2.h>
#include <mswsock.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#endif

// #define SERV_PORT 1255
#define MAXLINE 255

int SERV_PORT;

int StringToInt (char str[], int* num) 
{
  *num = 0;
  if(strlen(str) == 0)
    return -2;
  for (int i = 0; i < strlen(str); i++) 
  {
    if (str[i] < '0' || str[i] > '9') 
    {
      return -1;
    }
    *num = (*num)*10 + (str[i] - '0');    
  }
  return 1;
}

int SplitChaAndNum (char str[], char cha[], char num[]) {
  int ic = 0;
  int in = 0;
  if(strlen(str) == 0)
    return -2;
  for (int i = 0; i < strlen(str); i++) 
  {
    if (str[i] >= '0' && str[i] <= '9') 
    {
      num[in] = str[i]; 
      in++;
    }
    else if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
    {
      cha[ic] = str[i];
      ic++;
    }
    else
    {
      return -1;
    }
  }
  cha[ic] = '\0';
  num[in] = '\0';
  return 1;
}

int main (int argc, char *argv[])
{
  // ------------add SERV_PORT----------------
  if (argc < 2) 
  {
    perror("SERV_PORT failed");
    return 0;
  }
  if (StringToInt(argv[1], &SERV_PORT) != 1)
  {
    perror("SERV_PORT failed");
    return 0;
  }
  printf("SERV_PORT: %d\n", SERV_PORT);
  // -------------------------------------------

  int sockfd, n;
  socklen_t len;
  char mesg[MAXLINE + 1]; 
  struct sockaddr_in servaddr, cliaddr;
 
  sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
  bzero(&servaddr, sizeof(servaddr)); 
  servaddr.sin_family = AF_INET; 
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
  servaddr.sin_port = htons(SERV_PORT); 
  if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))>=0)
  {
    printf("Server is running at port %d\n", SERV_PORT);
  }
  else
  {
    perror("bind failed");
    return 0;
  }

  for ( ; ; ) { 
    len = sizeof(cliaddr); 
    printf("Receiving data ...\n");
    n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len); 
    // printf("--n = %d--\n", n);
    mesg[n] = '\0';
    printf("Raw data:----%s---\n", mesg);
    // -----split char and num------    
    char cha[MAXLINE+1], num[MAXLINE+1];
    if (SplitChaAndNum(mesg, cha, num) != 1)
    {
      strcpy(mesg, "Error"); 
    }
    else
    {
      strcpy(mesg, num);
      if(strlen(num) != 0)
        strcat(mesg, "\n");
      strcat(mesg, cha);
    }    
    // ----------------------------- 
    printf("Sending data :%s\n\n", mesg);
    sendto(sockfd, mesg, strlen(mesg), 0, (struct sockaddr *) &cliaddr, len); 
  }
  close(sockfd);  
}
