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


/* Definations */
#define DEFAULT_BUFLEN 512
#define PORT 2131



struct getuserdata
{
    char *usernames;
    char *passwords;
};
//reference for copystring https://stackoverflow.com/questions/41869803/what-is-the-best-alternative-to-strncpy

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


char *lastN(char *str, size_t n)
{
    size_t len = strlen(str);
    return (char *)str + len - n;
}
int testing(int client,char *passwordfile)
{

    int length, rcnt, optval, count;
    char recvbuf[DEFAULT_BUFLEN], bmsg[DEFAULT_BUFLEN];
    char getcommand[DEFAULT_BUFLEN], myuser[DEFAULT_BUFLEN], mypassword[DEFAULT_BUFLEN];

    int recvbuflen = DEFAULT_BUFLEN;
    do
    {
        // Clear Receive buffer
        memset(&recvbuf, '\0', sizeof(recvbuf));
        rcnt = recv(client, recvbuf, recvbuflen, 0);
        char *recvstring = (char *)recvbuf;
        char recvcmd[4];
        if (rcnt > 0)
        {
            printf("Bytes received: %d\n", rcnt);

            int count = 0;
            for (count = 0; count < strlen(recvstring); count++)
            {
                if (isspace(recvstring[count]))
                    break;
            }
            
            strncpy(recvcmd, recvstring, count);
            strncpy(getcommand, recvstring + count + 1, strlen(recvstring) - (count + 2));
            int counttwo = 0;
            for (counttwo = 0; counttwo < strlen(getcommand); counttwo++)
            {
                if (isspace(getcommand[counttwo]))
                    break;
            }

            strncpy(myuser, getcommand, counttwo);
            strncpy(mypassword, getcommand + (counttwo + 1), strlen(getcommand) - count);

            if (strcmp(recvcmd, "USER") == 0 || strcmp(recvcmd, "user") == 0)
            {
                FILE *fp ;
                int countline = 0;
                char *val;

                struct getuserdata userd;

                char line[DEFAULT_BUFLEN] = {0};

                if (NULL == (fp = fopen(passwordfile, "r")))
                {
                    perror("404 can't open file\n");
                    exit(EXIT_FAILURE);
                }

                for (; fscanf(fp, "%s", line) != EOF;)
                {

                    val = strtok(line, ":");
                    userd.usernames = val;
                    val = strtok(NULL, ":");
                    if ((val) != NULL)
                    {
                        userd.passwords = val;
                    }

                    printf("400 a user entered command user\n");
                    if (strcmp(myuser, userd.usernames) == 0 && strcmp(mypassword, userd.passwords) == 0)
                    {
                        char *c2 = "200 User test granted to access.\n";

                        send(client, c2, strlen(c2), 0);
                        return;
                    }
                    else
                    {

                        char *invalid = "400 User not found. Please try with another user.\n";
                        send(client, invalid, strlen(invalid), 0);
                    }
                }
            }

            printf("Bytes sent: %d\n", rcnt);
        }

    } while (rcnt > 0);
}
void programcommand(int client, char filedir[DEFAULT_BUFLEN])
{
    int temp[DEFAULT_BUFLEN], countrow = 1, receivebufferlen = DEFAULT_BUFLEN;
    char data[DEFAULT_BUFLEN];
    FILE *fp;
    char receivebuffer[1024];

    while (countrow > 0)
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
            if (isspace(recvstring[count])){
             strncpy(recvcmd, recvstring, count);
              copystring(getcommand, strlen(recvstring) - (count + 2), recvstring + count + 1);

                break;
}

          

            
        }
       


       
        if (strcmp(recvcmd, "LIST") == 0 || strcmp(recvcmd, "list") == 0)
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
        else if (strcmp(recvcmd, "PUT") == 0 || strcmp(recvcmd, "put") == 0)
        {
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
            while (1)
            {
                n = recv(client, buffer, DEFAULT_BUFLEN, 0);

                strcpy(mydata, getst);

                fputs(mydata, fp);

                fclose(fp);

                if (n <= 0)
                {
                    char *newdata = "400 File cannot save on server side.\n";

                    send(client, newdata, strlen(newdata), 0);
                }
                else
                {
                    char dataToSend[150] = "200 ";

                    char con[DEFAULT_BUFLEN];

                    sprintf(con, "%d", n);

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
                    break;
                    return;
                }

                printf("File created and saved successfully. :) \n");

                fprintf(fp, "%s", buffer);
                bzero(buffer, DEFAULT_BUFLEN);
                return;
            }
        }
        else if (strcmp(recvcmd, "DEL") == 0 || strcmp(recvcmd, "del") == 0)
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

        else if (strcmp(recvcmd, "GET") == 0 || strcmp(recvcmd, "get") == 0)
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
        else if (strcmp(recvcmd, "QUIT") == 0 || strcmp(recvcmd, "quit") == 0)
        {

            char *bye = "Goodbye\n";
            send(client, bye, strlen(bye), 0);
            countrow =0;
            

            close(client);

            return;
        }
        else
        {
            char dataToSend[150] = "400 ";
            strcat(dataToSend, recvcmd);
            strcat(dataToSend, " Command not implemented");
            strcat(dataToSend, "\n");
            int l = 0;
            int i;
            for (i = 0; dataToSend[i] != '\0'; i++)
            {
                l++;
            }
            send(client, dataToSend, l, 0);
        }
        if (countrow > 0) {
            printf("Bytes received: %d\n", countrow);

        // Echo the buffer back to the sender
        
            if (countrow < 0) {
                printf("Send failed:\n");
                close(client);
                break;
            }
            printf("Bytes sent: %d\n", countrow);

        }
        else if (countrow == 0)
            printf("Connection closing...\n");
        else  {
            printf("Receive failed:\n");
            close(client);
            break;
        }
        countrow++;
    }
}

int main(int argc, char *argv[])
{
int server, client;
struct sockaddr_in local_addr;
struct sockaddr_in remote_addr;
int length,fd,rcnt,optval;
pid_t pid;
 int *childfd;
    int portno;
    char *password;
  int flags, opt;
   int clilen;
  
    int nsecs, tfnd;
/* Open socket descriptor */
if ((server = socket( AF_INET, SOCK_STREAM, 0)) < 0 ) { 
    perror("Can't create socket!");
    return(1);
}
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
    char *mypassworder = argv[7];
    char *directorys = argv[5];

    if (chdir(directorys) != 0)
    {
           
        
        perror("this is not a directory");
    }
        char *passworddir = directorys;
       
       

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
  if (access(mypassworder, F_OK) == 0)
    {
        

/* Fill local and remote address structure with zero */
memset( &local_addr, 0, sizeof(local_addr) );
memset( &remote_addr, 0, sizeof(remote_addr) );

/* Set values to local_addr structure */
local_addr.sin_family = AF_INET;
local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
local_addr.sin_port = htons(portno);

// set SO_REUSEADDR on a socket to true (1):
optval = 1;
setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

if ( bind( server, (struct sockaddr *)&local_addr, sizeof(local_addr) ) < 0 )
{
    /* could not start server */
    perror("Bind error");
    return(1);
}

if ( listen( server, SOMAXCONN ) < 0 ) {
        perror("listen");
        exit(1);
}

printf("Concurrent  socket server now starting on port %d\n",portno);
printf("Wait for connection\n");

while(1) {  // main accept() loop
    length = sizeof remote_addr;
    if ((fd = accept(server, (struct sockaddr *)&remote_addr, \
          &length)) == -1) {
          perror("Accept Problem!");
          continue;
    }

    printf("Server: got connection from %s\n", \
            inet_ntoa(remote_addr.sin_addr));

    /* If fork create Child, take control over child and close on server side */
    if ((pid=fork()) == 0) {
        close(server);
         
            char *sender = "Welcome to Bob server\n";
    send(fd, sender, strlen(sender), 0);
        if(testing(fd,mypassworder)){
            programcommand(fd,argv[5]);
        }
        printf("Child finished their job!\n");
        close(fd);
        exit(0);
    }

}

// Final Cleanup
close(server);
    }
    else{
        printf("can't  find  %s file \n",passworder);
            printf("usage: \n ./name server -p <portnumber> -d directory -u password.txt\n");
            exit(EXIT_FAILURE);

    }

}
