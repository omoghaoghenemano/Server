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

#include <errno.h> //for error handling

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

char *copystring(char *d, size_t size, char *s)
{
    size_t ma;
    for (ma = 0; ma < size && s[ma]; ma++)
    {
        d[ma] = s[ma];
    }
    d[ma] = '\0';

    return d;
}

void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[DEFAULT_BUFLEN];
 
  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, DEFAULT_BUFLEN, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, DEFAULT_BUFLEN);
  }
  return;
}
 
char *lastN(char *str, size_t n)
{
    size_t len = strlen(str);
    return (char *)str + len - n;
}
void programcommand(int client)
{
    int temp[DEFAULT_BUFLEN], countrow=1, receivebufferlen = DEFAULT_BUFLEN;
       char data[DEFAULT_BUFLEN];
    FILE *fp;
    char receivebuffer[1024];

    while(countrow>0)
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
         else if ((countrow > 0 && strcmp(recvcmd, "PUT") == 0) || (countrow > 0 && strcmp(recvcmd, "PUT") == 0)){
              int n;
  FILE *fp;
  char *filename = getcommand;
  char mydata[DEFAULT_BUFLEN];
  char buffer[DEFAULT_BUFLEN];
  char mynewbuffer[DEFAULT_BUFLEN];

  memset(buffer, 0, strlen(buffer));
  char *getst = (char *)buffer;
   int ir;
   

  fp = fopen(filename, "w");
  while (1) {
    n = recv(client, buffer, DEFAULT_BUFLEN, 0);
 printf("Enter contents to store in file : \n");
 
      /*   int count = 0;
        for (count = 0; count < strlen(getst); count++)
        {
            if (isspace(getst[count]))
                break;
        }

        copystring(mydata, count, getst);
   
 */
strcpy(mydata, getst);


    fputs(mydata, fp);


    fclose(fp);
    //check this line.
    if(n ==0){
       char *newdata = "400 File cannot save on server side.\n";

               
                send(client, newdata, strlen(newdata), 0);

        

    }
    else{
         char dataToSend[150] = "200 ";
         
         
char con[DEFAULT_BUFLEN];


sprintf(con,"%d",n);

                strcat(dataToSend, con);
                strcat(dataToSend, "Byte test file retrieved by server and was saved.");
                strcat(dataToSend, "\n");
                int i;
                int l = 0;
                for (i = 0; dataToSend[i] != '\0'; i++)
                {
                    l++;
                }
                send(client, dataToSend, l, 0);


    }
    

    
    printf("File created and saved successfully. :) \n");


    fprintf(fp, "%s", buffer);
    bzero(buffer, DEFAULT_BUFLEN);
    return;
    if (n <= 0){
      break;
      return;
    }
    
  }
             
         }
        else if ((countrow > 0 && strcmp(recvcmd, "DEL") == 0) || (countrow > 0 && strcmp(recvcmd, "del") == 0))
        {
            if (remove(getcommand) == 0)
            {

                char dataToSend[150] = "400 ";
                strcat(dataToSend, getcommand);
                strcat(dataToSend, " deleted.");
                strcat(dataToSend, "\n");
                int i;
                int l = 0;
                for (i = 0; dataToSend[i] != '\0'; i++)
                {
                    l++;
                }
                send(client, dataToSend, l, 0);
            }
            else
            {

                char dataToSend[150] = "404 ";
                strcat(dataToSend, getcommand);
                strcat(dataToSend, " file not found.");
                strcat(dataToSend, "\n");
                int i;
                int l = 0;
                for (i = 0; dataToSend[i] != '\0'; i++)
                {
                    l++;
                }
                send(client, dataToSend, l, 0);
            }
        }

        else if ((countrow > 0 && strcmp(recvcmd, "GET") == 0) || (countrow > 0 && strcmp(recvcmd, "get") == 0))
        {

            fp = fopen(getcommand, "r");
            if (fp != NULL)
            {
                sizeofh[DEFAULT_BUFLEN] = '\0';

                while (fgets(sizeofh, 1025, fp) != NULL)
                {
                    if (send(client, sizeofh, sizeof(sizeofh), 0) == false)
                    {

                        exit(1);
                    }
                    sizeofh[DEFAULT_BUFLEN] = '\0';
                }
            }
            else
            {
                char *notfound = "404 file not found\n";

                send(client, notfound, strlen(notfound), 0);
            }
            if (countrow < 0)
            {
                printf("Send failed:\n");
                close(client);
                break;
            }
        }
        else if ((countrow > 0 && strcmp(recvcmd, "QUIT") == 0) || (countrow > 0 && strcmp(recvcmd, "quit") == 0))
        {

            char *bye = "Goodbye\n";
            send(client, bye, strlen(bye), 0);
            
              
           
               close(client);
          
            return;
        }
        countrow++;

    } 
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
            printf("Connection close by client \n");
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
        printf("usage: \n ./name server -p <portnumber> -d directory -u password file name\n");
        exit(1);
    }

    portno = atoi(argv[3]);
    char *servervalidation = argv[1];
    if (strcmp(servervalidation, "server") == 0)
    {
    }
    else
    {
        printf("for first argument please enter: server\n");
        exit(1);
    }
    char *passworder = argv[7];
    char *directorys = argv[5];
    if (chdir(directorys) != 0)
    {
        perror("this is not a directory");
    }

    while ((opt = getopt(argc, argv, "pdu:")) != -1)
    {
        switch (opt)
        {

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
    
    if (access(passworder, F_OK) == 0)
    {

        if (optind >= argc)
        {
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
            printf("server listening on localhost port %d\n", ntohs(serveraddr.sin_port));

            childfd = accept(parentfd, (struct sockaddr *)&clientaddr, (socklen_t *)&clilen);
            if (childfd < 0)
            {
                if (errno != EINTR)
                    error("ERROR on accept");
                else
                    continue;
            }

            pthread_create(&thread, NULL, &Child, &childfd);
        }
    }
    else{
        printf("ensure the password file is in same directory\n");

    }
    return 0;
}
