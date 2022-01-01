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

#define DEFAULT_BUFLEN 1024

#define PORT 2676

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
    int length;

    int temp[DEFAULT_BUFLEN];
    int countrow;
    FILE *fp;
    char receivebuffer[1024];
    int receivebufferlen = DEFAULT_BUFLEN;
    do
    {
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
    int sd, opt, optval;
    struct sockaddr_in addr;
    unsigned short port = 0;

    while ((opt = getopt(argc, argv, "p:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            port = atoi(optarg);
            break;
        }
    }

    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        PANIC("Socket");
    addr.sin_family = AF_INET;

    if (port > 0)
        addr.sin_port = htons(port);
    else
        addr.sin_port = htons(PORT);

    addr.sin_addr.s_addr = INADDR_ANY;

    // set SO_REUSEADDR on a socket to true (1):
    optval = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
        PANIC("Bind");
    if (listen(sd, SOMAXCONN) != 0)
        PANIC("Listen");

    printf("System ready on port %d\n", ntohs(addr.sin_port));

    while (1)
    {
        int client, addr_size = sizeof(addr);
        pthread_t child;

        client = accept(sd, (struct sockaddr *)&addr, &addr_size);
        printf("Connected: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        if (pthread_create(&child, NULL, Child, &client) != 0)
        {

            perror("Thread creation");
        }
        else
            pthread_detach(child); /* disassociate from parent */
    }
    return 0;
}
