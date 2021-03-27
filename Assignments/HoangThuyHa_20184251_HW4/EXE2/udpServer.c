#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef __WIN32__
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#else
#include <unistd.h>
#include <netinet/in.h>
#include<sys/socket.h>
#include<netdb.h>	//hostent
#include<arpa/inet.h>
#endif

// #define SERV_PORT 1255
#define MAXLINE 255
#define MAXLENGTH_IP 20
#define MAXLENGTH_HOSTNAME 20

int SERV_PORT;

int isNum(char a) {
  if(a < '0' || a > '9')
    return 0;
  return 1;
}

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

int HostnameToIP (char hostname[], char ip[]) 
{
  struct hostent *host_entry;
  struct in_addr **addr_list;

  if( (host_entry = gethostbyname(hostname)) == NULL) 
  {
    // get the host information
    herror("gethostbyname");
    return -1;
  }
  
  addr_list = (struct in_addr **) host_entry->h_addr_list;

  for(int i = 0; addr_list[i] != NULL; i++) 
  {
    // Return the first one;
    strcpy(ip, inet_ntoa(*addr_list[i]));
    printf("IP: %s\n", ip);
    return 1;
  }
  return -1;
}

int IPToHostname (char ip[], char hostname[]) 
{
  struct hostent *host_entry;
  // printf("ok\n"); return 0;
  struct in_addr addr;
  inet_aton(ip, &addr);
  if( (host_entry = gethostbyaddr((const char*)&addr, sizeof(addr), AF_INET)) == NULL) 
  {
    // get the host information
    herror("gethostbyaddr");
    return -1;
  }
  // printf("ok\n"); return 0;
  strcpy(hostname, host_entry->h_name);
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
    // printf("Raw data:----%s---\n", mesg);

    // -----get IP or domain name------    
    char ip[MAXLENGTH_IP]; ip[0] = '\0';
    char hostname[MAXLENGTH_HOSTNAME]; hostname[0] = '\0';
    if (strlen(mesg) < MAXLENGTH_HOSTNAME && isNum(mesg[0]) == 0) 
    {
      printf("Hostname: %s\n", mesg);
      if (HostnameToIP(mesg, ip) == 1) 
      {
        printf("Host IP: %s\n", ip);
        strcpy(mesg, "Host IP: ");
        strcat(mesg, ip);
      }
    }
    if (strlen(mesg) < MAXLENGTH_IP && strlen(ip) == 0) 
    {
      printf("IP: %s\n", mesg);
      if (IPToHostname(mesg, hostname) == 1) 
      {
        printf("Host name: %s\n", hostname);
        strcpy(mesg, "Host name: ");
        strcat(mesg, hostname);
      }
    }
    if (strlen(hostname) + strlen(ip) == 0) 
    {
      strcpy(mesg, "Not found information\0");
    }
    // ----------------------------- 

    printf("Sending data :%s\n\n", mesg);
    sendto(sockfd, mesg, strlen(mesg), 0, (struct sockaddr *) &cliaddr, len); 
  }
  close(sockfd);  
}
