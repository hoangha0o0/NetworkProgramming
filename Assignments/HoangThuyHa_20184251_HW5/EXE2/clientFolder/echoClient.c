#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 4096 /*max text line length*/
#define MAXIP 30 /* max length of Ip Address */
int SERV_PORT;
char IP_ADDRESS[MAXIP];
// #define SERV_PORT 3000 /*port*/

int StringToInt(char str[], int* num);
int send_file(FILE *fp, int sockfd);

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
        // -------------------- send file name, check existance of file ------------
        char filePath[MAXLINE+1];
        strcpy(filePath, sendline);
        FILE* file;
        file = fopen(filePath, "r");
        if(file == NULL) {
            printf("Error: File not found\n\n");
            continue;
        }
        send(sockfd, filePath, strlen(filePath), 0);
        if ((n = recv(sockfd, recvline, MAXLINE,0)) == 0){
            //error: server terminated prematurely
            perror("The server terminated prematurely"); 
            exit(4);
        }
        recvline[n] = '\0';
        if(strcmp(recvline, "Error: File is existent on server.") == 0) {
            printf("%s", "String received from the server: ");
            recvline[n] = '\0';
            printf("%s\n\n", recvline);
            continue;    
        }
        // --------------------------------------------------------------------------
        // -------------------- send content of file --------------------------------
        if(send_file(file, sockfd) != 0) {
            printf("Error: File tranfering is interupted\n\n");
            fclose(file);
            continue;
        }
        fclose(file);
        printf("%s\n\n", "Successful transfering.");
        // -------------------- confirm that copy file is done--------------------------------
        bzero(sendline, MAXLINE+1);
        strcpy(sendline, "Terminate copy this file. Prepare copy next file.");
        send(sockfd, sendline, strlen(sendline), 0);
        if ((n = recv(sockfd, recvline, MAXLINE,0)) == 0){
            //error: server terminated prematurely
            perror("The server terminated prematurely"); 
            exit(4);
        }
        printf("%s", "String received from the server: ");
        recvline[n] = '\0';
        printf("%s\n", recvline);

        bzero(sendline, MAXLINE+1);
        if(fgets(sendline, MAXLINE, stdin) == NULL)
            return 1;
        sendline[strlen(sendline) - 1] = '\0';
        send(sockfd, sendline, strlen(sendline), 0);
        // --------------------------------------------------------------------------
        bzero(recvline, MAXLINE+1);
        if ((n = recv(sockfd, recvline, MAXLINE,0)) == 0){
            //error: server terminated prematurely
            perror("The server terminated prematurely"); 
            exit(4);
        }
        printf("%s", "String received from the server: ");
        recvline[n] = '\0';
        printf("%s\n\n", recvline);
    }
    close(sockfd);
    exit(0);
}

int StringToInt (char str[], int* num) {
    *num = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9') 
            return -1;
        *num = (*num)*10 + (str[i] - '0');    
    }
    return 0;
}
int send_file(FILE *file, int sockfd){
    int n;
    char data[MAXLINE+1] = {0};

    while(fgets(data, MAXLINE, file) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            // perror("Error: File tranfering is interupted");
            return 1;
        }
        bzero(data, MAXLINE+1);
    }
    return 0;
}
