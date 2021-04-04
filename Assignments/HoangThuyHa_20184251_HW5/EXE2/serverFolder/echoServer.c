#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
// #define SERV_PORT 3000 /*port*/
int SERV_PORT;
#define LISTENQ 8 /*maximum number of client connections */

int StringToInt(char str[], int* num);
int write_file(FILE* filePath, int sockfd);

int main (int argc, char **argv)
{
    int listenfd, connfd, n;
    pid_t childpid;
    socklen_t clilen;
    char buf[MAXLINE+1];
    struct sockaddr_in cliaddr, servaddr;

    //basic check of the arguments
    //additional checks can be inserted
    if (argc != 2) {
        perror("Usage: TCPClient <Port of the server"); 
        exit(1);
    }
    StringToInt(argv[1], &SERV_PORT); 

    //creation of the socket
    listenfd = socket (AF_INET, SOCK_STREAM, 0);

    //preparation of the socket address 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen (listenfd, LISTENQ);

    printf("%s\n","Server running...waiting for connections.");

    for ( ; ; ) {		
        clilen = sizeof(cliaddr);
        connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
        printf("%s\n","Received request...");
            
        while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {
            printf("%s","\nString received from the client:");
            buf[n] = '\0';
            printf("--%s--\n", buf);
            // -----receive filePath and check exist or not-----    
            FILE* file;
            char filePath[MAXLINE+1];
            strcpy(filePath, buf);
            file = fopen(filePath, "r");
            if(file != NULL) {
                strcpy(buf, "Error: File is existent on server.");
                printf("%s","String sent to the client:");
                printf("--%s--\n", buf);
                send(connfd, buf, strlen(buf), 0);
                fclose(file);
                continue;
            }
            strcpy(buf, "File is not existent on server.");
            send(connfd, buf, strlen(buf), 0);
            // -----------------------------------------------
            // ------------ receive content of file ----------
            file = fopen(filePath, "w");
            write_file(file, connfd);
            fclose(file);
            strcpy(buf, "Successful transfering.");
            // -----------------------------------------------
            printf("%s","String sent to the client:");
            printf("--%s--\n", buf);
            send(connfd, buf, strlen(buf), 0);
        }

        if (n < 0) {
            perror("Read error"); 
            exit(1);
        }
        close(connfd);
    }
    //close listening socket
    close (listenfd); 
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
int write_file(FILE* file, int sockfd) { 
  int n;
  char buffer[MAXLINE+1];

  while (1) {
    n = recv(sockfd, buffer, MAXLINE+1, 0);
    if (n <= 0){
      break;
      return 0;
    }
    // ----------- check finish copy ---------------------------------------
    if(strcmp(buffer, "Terminate copy this file. Prepare copy next file.") == 0) {
        char tmp[MAXLINE+1];
        strcpy(tmp, "-- Confirm that copy file is done (yes, no) ? --");
        send(sockfd, tmp, strlen(tmp), 0);
        bzero(tmp, MAXLINE+1);
        n = recv(sockfd, tmp, MAXLINE+1, 0);
        if (n <= 0) {
            break;
            return 0;
        }
        if( strcmp(tmp, "yes") == 0)
            return 0;
    }
    // ---------------------------------------------------------------------
    fprintf(file, "%s", buffer);
    bzero(buffer, MAXLINE+1);
  }
  return 0;
}