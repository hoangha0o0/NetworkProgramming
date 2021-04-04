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
int SplitChaAndNum (char str[], char cha[], char num[]);

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
            // -----split char and num------    
            char cha[MAXLINE+1], num[MAXLINE+1];
            if (SplitChaAndNum(buf, cha, num) != 1)
                strcpy(buf, "Error"); 
            else {
                strcpy(buf, num);
                if(strlen(num) != 0)
                    strcat(buf, "\n");
                strcat(buf, cha);
            }  
            // ----------------------------- 
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
    return 1;
}
int SplitChaAndNum (char str[], char cha[], char num[]) {
    int ic = 0;
    int in = 0;
    if(strlen(str) == 0)
        return -2;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            num[in] = str[i]; 
            in++;
        }
        else if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
            cha[ic] = str[i];
            ic++;
        }
        else       
            return -1;
    }
    cha[ic] = '\0';
    num[in] = '\0';
    return 1;
}