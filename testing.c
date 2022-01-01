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
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


#include <errno.h>    //for error handling


#define DEFAULT_BUFLEN 1024

#define PORT 2671

void PANIC(char *msg);
#define PANIC(msg)   \
    {                \
        perror(msg); \
        exit(-1);    \
    }

void uploadfile(FILE *fp, int sockfd)
{
    char type[1050] = {0};

    while (fgets(type, 1050, fp) != NULL)
    {
        if (send(sockfd, type, sizeof(type, 0) == -1, 0))
        {

            exit(1);
        }
        bzero(type, 1050);
    }
}

struct getuserdata
{
    char *usernames;
    char *passwords;
};


char *copystring(char *dest, size_t size, char *src)
{

    if (size > 0)
    {
        size_t ma;
        for (ma = 0; ma < size && src[ma]; ma++)
        {
            dest[ma] = src[ma];
        }
        dest[ma] = '\0';
    }
    return dest;
}

char *lastN(char *str, size_t n)
{
    size_t len = strlen(str);
    return (char *)str + len - n;
}
void programcommand(int client)
{
    int temp[DEFAULT_BUFLEN], countrow, receivebufferlen = DEFAULT_BUFLEN;
    FILE *fp;
    char receivebuffer[1024];

    do
    {
        char sizeofh[DEFAULT_BUFLEN] = {0};
        memset(receivebuffer, 0, strlen(receivebuffer));
        memcpy(temp, receivebuffer, strlen(receivebuffer));
        countrow = recv(client, receivebuffer, receivebufferlen, 0);
        char recvcmd[4], recvcmdGET[3];
        char getcommand[DEFAULT_BUFLEN];
        char *recvstring = (char *)receivebuffer;
        int count = 0;
        for (count = 0; count < strlen(recvstring); count++)
        {
            if (isspace(recvstring[count]))
                break;
        }

        copystring(recvcmd, count, recvstring);
        copystring(getcommand, strlen(recvstring) - (count + 2), recvstring + count + 1);

        if ((countrow > 0 && strcmp(recvcmd, "LIST") == 0) || (countrow > 0 && strcmp(recvcmd, "list") == 0))
        {

            memset(recvcmd, 0, strlen(recvcmd));
            DIR *v;
            struct dirent *directory;
            v = opendir(".");
            if (v)
            {
                while ((directory = readdir(v)) != NULL)
                {
                    send(client, directory->d_name, strlen(directory->d_name), 0);
                    send(client, "\n", 2, 0);
                }
                closedir(v);
            }
        }
       
        else if ((countrow > 0 && strcmp(recvcmd, "QUIT") == 0) || (countrow > 0 && strcmp(recvcmd, "quit") == 0))
        {

            char *bye = "Goodbye\n";
            send(client, bye, strlen(bye), 0);
            close(client);
        }

    } while (countrow > 0);
}

int authentication(int client, char *command, int *bytes_read)
{
    int i;
    int num = 0;
    char *arg1st = strtok(command, " \n");

    if (strcmp(arg1st, "USER") == 0)
    {
        char *dataToSend = strtok(NULL, "\n");
        char *firts = malloc(sizeof(char) * 256);
        char *second = malloc(sizeof(char) * 256);

        strncpy(second, dataToSend, 4);

        strcpy(firts, lastN(dataToSend, 4));

        strcat(dataToSend, "\n");

        if (strcmp(second, "mano") == 0 && strcmp(firts, "keno") == 0)
        {
            char *c2 = "200 User test granted to access.\n";

            send(client, c2, strlen(c2), 0);
            return;
            //look into this if error
        }
        else
        {

            char *invalid = "400 User not found. Please try with another user.";
            send(client, invalid, strlen(invalid), 0);
        }
    }
}

void *Child(void *arg)
{
    char line[DEFAULT_BUFLEN];
    int bytes_read;
    int client = *(int *)arg;
    char *sender = "Welcome to Bob server\n";
    send(client, sender, strlen(sender), 0);
    send(client, "\n", 2, 0);

    do
    {
        bytes_read = recv(client, line, sizeof(line), 0);
        if (bytes_read > 0)
        {
            if ((bytes_read = send(client, line, bytes_read, 0)) < 0)
            {
                printf("Send failed\n");
                break;
            }

            if (authentication(client, line, &bytes_read))
            {
                programcommand(client);
            }
            else
            {

                exit(0);
            }
        }
        else if (bytes_read == 0)
        {
            printf("Connection closed by client\n");
            break;
        }
        else
        {
            printf("Connection has problem\n");
            break;
        }
    } while (bytes_read > 0);
    close(client);
    return arg;
}

int main(int argc, char *argv[])
{ 
    int parentfd;                 
    int *childfd;                 
    int portno;  
    char *password;                  
    int clilen;                 
    struct sockaddr_in serveraddr; 
    struct sockaddr_in clientaddr; 
    struct hostent *hostp;         
    char *hostaddrp;              
    int optval;                 
    int n;       
    int flags, opt;
    int nsecs, tfnd;                  

    pthread_t thread;

  if (argc != 8)
    {
        printf("usage: \n ./name server -p <portnumber> -d directory -u password.txt\n");
        exit(1);
    }


 portno = atoi(argv[3]); 

 char *passworder = argv[7];
 char * directorys = argv[5];
 if (chdir(directorys) != 0) {
	    		perror("chdir() failed"); 
	    		
			}
            
 
 
    while ((opt = getopt(argc, argv, "pdu:")) != -1) {
        switch (opt) {
         
        case 'p':
            
   
  

            break;
            case 'd':
            break;

            case 'u':

           
            
            break;
       
         default: /* '?' */
           printf("usage: \n ./name server -p <portnumber> -d directory -u password.txt\n");
            exit(EXIT_FAILURE);
        }
    }
if(strcmp(passworder,"password.txt")==0){
    
 


   if (optind >= argc) {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }



 

    parentfd = socket(AF_INET, SOCK_STREAM, 0);

    if (parentfd < 0)
        error("ERROR opening socket");

    optval = 1;
    setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,
               (const void *)&optval, sizeof(int));

    bzero((char *)&serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);
    

    if (bind(parentfd, (struct sockaddr *)&serveraddr,
             sizeof(serveraddr)) < 0)
        error("ERROR on binding");

    if (listen(parentfd, 0) < 0) /* allow 5 requests to queue up */
        error("ERROR on listen");


    clilen = sizeof(clientaddr);
    while (true)
    {
        int childfd;
            printf("server listening on localhost port %d\n",ntohs(serveraddr.sin_port));
       
        childfd = accept(parentfd, (struct sockaddr *)&clientaddr, (socklen_t *)&clilen);
        if (childfd < 0 )
        {
            if(errno!=EINTR)
                error("ERROR on accept");
            else
                continue;
        }

        
        
  
        pthread_create(&thread, NULL, &Child, &childfd);
    }
}
    return 0;
}
