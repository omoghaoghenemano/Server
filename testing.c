#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dirent.h> 


/* Definations */
#define DEFAULT_BUFLEN 6024

#define PORT 1888


void PANIC(char* msg);
#define PANIC(msg)  { perror(msg); exit(-1); }

/*--------------------------------------------------------------------*/
/*--- Child - echo server                                         ---*/
/*--------------------------------------------------------------------*/

void uploadfile(FILE *fp, int sockfd)
{
    char type[1050] = {0};

    while (fgets(type, 1050, fp) != NULL)
    {
        if (send(sockfd, type, sizeof(type, 0) == -1,0))
        {
           
            exit(1);
        }
        bzero(type, 1050);
    }
}

void programcommand(int client, char* command, int* bytes_read){
    int i;
char* arg1st = strtok(command, " \n");
char users[DEFAULT_BUFLEN];
char pass[DEFAULT_BUFLEN];

  char getfilename[DEFAULT_BUFLEN];
   if(strcmp(command,"QUIT") == 0 ||strcmp(command,"quit") == 0 ){
            send(client,"Goodbye!\n", 9, 0);
            close(client);
            *bytes_read = 0;
            return;
        }
        else if(strcmp(command, "LIST")==0){
             DIR *d;
  struct dirent *directory;
  d = opendir(".");
  if (d) {
    while ((directory = readdir(d)) != NULL) {
     
       send(client,directory->d_name, strlen(directory->d_name), 0);
        send(client, "\n", 2, 0);
    }
    closedir(d);
    return;
  }
        }
        else if(strcmp(command, "GET")==0){
            FILE *fp;
            fp = fopen(getfilename, "r");
                    if (fp == NULL)
                    {
                        
                        
                        send(client, "404 File not found\n", 30, 0);
                        perror("Error reading file.");
                        exit(1);
                    }
                    uploadfile(fp, command);

           return;

            

        }
        
       

    
          
}


void* Child(void* arg)
{   char line[DEFAULT_BUFLEN];
    int bytes_read;
    int client = *(int *)arg;
     send(client, "Welcome to Oghenemanoserver\n", 27, 0);
       send(client, "\n", 2, 0);

    do
    {
        bytes_read = recv(client, line, sizeof(line), 0);
        if (bytes_read > 0) {
                if ( (bytes_read=send(client, line, bytes_read, 0)) < 0 ) {
                        printf("Send failed\n");
                        break;
                }
                 programcommand(client, line, &bytes_read);
        } else if (bytes_read == 0 ) {
                printf("Connection closed by client\n");
                break;
        } else {
                printf("Connection has problem\n");
                break;
        }
    } while (bytes_read > 0);
    close(client);
    return arg;
}

/*--------------------------------------------------------------------*/
/*--- main - setup server and await connections (no need to clean  ---*/
/*--- up after terminated children.                                ---*/
/*--------------------------------------------------------------------*/
int main(int argc, char *argv[])
{   int sd,opt,optval;
    struct sockaddr_in addr;
    unsigned short port=0;

    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
                port=atoi(optarg);
                break;
        }
    }


    if ( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
        PANIC("Socket");
    addr.sin_family = AF_INET;

    if ( port > 0 )
                addr.sin_port = htons(port);
    else
                addr.sin_port = htons(PORT);

    addr.sin_addr.s_addr = INADDR_ANY;

   // set SO_REUSEADDR on a socket to true (1):
   optval = 1;
   setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);


    if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
        PANIC("Bind");
    if ( listen(sd, SOMAXCONN) != 0 )
        PANIC("Listen");

    printf("System ready on port %d\n",ntohs(addr.sin_port));

    while (1)
    {
        int client, addr_size = sizeof(addr);
        pthread_t child;

        client = accept(sd, (struct sockaddr*)&addr, &addr_size);
        printf("Connected: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        if ( pthread_create(&child, NULL, Child, &client) != 0 )
            perror("Thread creation");
        else
            pthread_detach(child);  /* disassociate from parent */
    }
    return 0;
}
