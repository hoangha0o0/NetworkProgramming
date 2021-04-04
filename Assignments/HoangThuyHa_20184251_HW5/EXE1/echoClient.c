#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 4096 /*max text line length*/
#define MAXIP 30
int SERV_PORT;
char IP_ADDRESS[MAXIP];
// #define SERV_PORT 3000 /*port*/

int StringToInt(char str[], int* num);

int main(int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE+1], recvline[MAXLINE+1];

    //basic check of the arguments
    //additional checks can be inserted
    if (argc < 2) {
        perror("Usage: TCPClient <IP address of the server"); 
        exit(1);
    }
    strcpy(IP_ADDRESS, argv[1]);
    if (argc < 3) {
        perror("Usage: TCPClient <Port of the server"); 
        exit(1);
    }
    StringToInt(argv[2], &SERV_PORT); 

    //Create a socket for the client
    //If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    //Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(IP_ADDRESS);
    servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order

    //Connection of the client to the socket 
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
        perror("Problem in connecting to the server");
        exit(3);
    }

 
    while (fgets(sendline, MAXLINE, stdin) != NULL) {
        sendline[strlen(sendline) - 1] = '\0';
        send(sockfd, sendline, strlen(sendline), 0);

        if ((n = recv(sockfd, recvline, MAXLINE,0)) == 0){
            //error: server terminated prematurely
            perror("The server terminated prematurely"); 
            exit(4);
        }
        printf("%s\n", "String received from the server: ");
        recvline[n] = '\0';
        printf("%s\n\n", recvline);
    }

    exit(0);
}

int StringToInt (char str[], int* num) {
    *num = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9') 
            return -1;
        *num = (*num)*10 + (str[i] - '0');    
    }
    return 1;
}