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

/* Definations */
#define DEFAULT_BUFLEN 6024

#define PORT 1889


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
struct getuserdata
{
  char *usernames;
  char *passwords;
};
void storing(char val1[], char val2[], int position) {
 int val2size,val1Size,i,newSize,currentPosition;
  val2size = strlen(val2);
 val1Size = strlen(val1);
  newSize = val2size + val1Size + 1;
   currentPosition = position - 1;
  i = 0;

  for (i = val1Size; i >= currentPosition; i--) 
  val1[i + val2size] = val1[i];
  for (i = 0; i < val2size; i++)
   val1[currentPosition + i] = val2[i];
  val1[newSize] = '\0';
}

char *lastN(char *str, size_t n)
{
    size_t len = strlen(str);
    return (char *)str + len - n;
}
void trial(char*command, int client){
    int i;int num =0;
char* arg1st = strtok(command, " \n");
    if(strcmp(arg1st, "ECHO")==0){
            char* dataToSend = strtok(NULL, "\n");
            
            strcat(dataToSend, "\n");
            if(strcmp(dataToSend,"mano\n")== 0){
                 char*c2 = "Command Server commands are\nECHO text : This is echo text message back\nRECHO text : This is echo text message in reverse order\n";
        char* c = "SUM A B :This is sum A + B and print result\nSUB A B :This is subtract B from A and print result\nMUL A B : This is multiply A and B and print result\nDIV A B :This is divide A and B and print result\n.\n";
            send(client,c2, strlen(c2), 0);
            }
            send(client, dataToSend, strlen(dataToSend), 0);
             char*c4 = "it worked";
        char* c5 = "SUM A B :This is sum A + B and print result\nSUB A B :This is subtract B from A and print result\nMUL A B : This is multiply A and B and print result\nDIV A B :This is divide A and B and print result\n.\n";
            send(client,c4, strlen(c4), 0);
        }
        

}
void authentication(int client, char* command, int* bytes_read){
     int i;int num =0;
char* arg1st = strtok(command, " \n");
     if(strcmp(arg1st, "USER")==0){
            char* dataToSend = strtok(NULL, "\n");
            char* firts = malloc(sizeof(char)*256);
            char* second = malloc(sizeof(char)*256);
         
             
            strncpy(second, dataToSend,4);
            

            
           strcpy(firts, lastN(dataToSend, 4));


            strcat(dataToSend, "\n");
            

            if(strcmp(second,"mano")== 0 && strcmp(firts,"keno")==0){
                 char*c2 = "200 User test granted to access.\n";
       
            send(client,c2, strlen(c2), 0);
            bool success = true;
            if(success){
                trial(command,client);
            }
          
            
            
           
           
     }
      else{
            
                char*invalid = "400 User not found. Please try with another user.";
                send(client, invalid, strlen(invalid),0);
            
            
       }
    
      
       
       
     
       

     
            }
       

}


void programcommand(int client, char* command, int* bytes_read){
 int i;int num =0;
char* arg1st = strtok(command, " \n");
 
    /* if(strcmp(command,"QUIT\n") ||strcmp(command,"QUIT") ){
                      send(client,"Goodbye!\n", 9, 0);
            close(client);
            *bytes_read = 0;
         
                 }
    
 else if(strcmp(command,"HELP\n") == 0 ||strcmp(command,"HELP") == 0 ){
        char*c2 = "Command Server commands are\nECHO text : This is echo text message back\nRECHO text : This is echo text message in reverse order\n";
        char* c = "SUM A B :This is sum A + B and print result\nSUB A B :This is subtract B from A and print result\nMUL A B : This is multiply A and B and print result\nDIV A B :This is divide A and B and print result\n.\n";
            send(client,c2, strlen(c2), 0);
            send(client, c, strlen(c), 0);  
            return;
        }*/
        
     if(strcmp(arg1st, "ECHO")==0){
            char* dataToSend = strtok(NULL, "\n");
            
            strcat(dataToSend, "\n");
            if(strcmp(dataToSend,"mano\n")== 0){
                 char*c2 = "Command Server commands are\nECHO text : This is echo text message back\nRECHO text : This is echo text message in reverse order\n";
        char* c = "SUM A B :This is sum A + B and print result\nSUB A B :This is subtract B from A and print result\nMUL A B : This is multiply A and B and print result\nDIV A B :This is divide A and B and print result\n.\n";
            send(client,c2, strlen(c2), 0);
            }
            send(client, dataToSend, strlen(dataToSend), 0);
             char*c4 = "it worked";
        char* c5 = "SUM A B :This is sum A + B and print result\nSUB A B :This is subtract B from A and print result\nMUL A B : This is multiply A and B and print result\nDIV A B :This is divide A and B and print result\n.\n";
            send(client,c4, strlen(c4), 0);
        }
       /*else if(strcmp(arg1st, "USER")==0){
            char* dataToSend = strtok(NULL, "\n");
            char* firts = malloc(sizeof(char)*256);
            char* second = malloc(sizeof(char)*256);
         
             
            strncpy(second, dataToSend,4);
            

            
           strcpy(firts, lastN(dataToSend, 4));


            strcat(dataToSend, "\n");
            

            if(strcmp(second,"mano")== 0 && strcmp(firts,"keno")==0){
                 char*c2 = "200 User test granted to access.\n";
       
            send(client,c2, strlen(c2), 0);
          
            
            
           
           
     }
      else{
            
                char*invalid = "400 User not found. Please try with another user.";
                send(client, invalid, strlen(invalid),0);
            
            
       }
    
      
       
       
     
       

     
            }
           
            
            
        */
       

     

    else if(strcmp(arg1st, "RECHO")==0){
            char* s = strtok(NULL, "\n");
            size_t l = strlen(s);
            char* dataToSend = (char*)malloc((l + 1) * sizeof(char));
            dataToSend[l] = '\0';
            int i;
            for(i = 0; i < l; i++) {
                dataToSend[i] = s[l - 1 - i];
            }
            strcat(dataToSend, "\n");
            send(client, dataToSend, l+1, 0);
            free(dataToSend);
            
        }
    else if(strcmp(arg1st, "SUM")==0){
            int firsNUm = atoi(strtok(NULL, " "));
            int secondNum = atoi(strtok(NULL, " \n"));
            int sum = firsNUm+secondNum;
            char dataToSend[20];
            sprintf(dataToSend, "%d", sum);
            int l=0;
            for(i=0;dataToSend[i]!='\0';i++){
                l++;
            }
            strcat(dataToSend, "\n");
            send(client, dataToSend, strlen(dataToSend), 0);
            
        }
    else if(strcmp(arg1st, "SUB")==0){
            int firsNUm = atoi(strtok(NULL, " "));
            int secondNum = atoi(strtok(NULL, " \n"));
            int diff = firsNUm-secondNum;
            char dataToSend[20];
            sprintf(dataToSend, "%d", diff);
            int l=0;
            for(i=0;dataToSend[i]!='\0';i++){
                l++;
            }
            strcat(dataToSend, "\n");
            send(client, dataToSend, strlen(dataToSend), 0);
            
        }
    else if(strcmp(arg1st, "MUL")==0){
            int firsNUm = atoi(strtok(NULL, " "));
            int secondNum = atoi(strtok(NULL, " \n"));
            int mul = firsNUm*secondNum;
            char dataToSend[20];
            sprintf(dataToSend, "%d", mul);
            int l=0;
            for(i=0;dataToSend[i]!='\0';i++){
                l++;
            }
            strcat(dataToSend, "\n");
            send(client, dataToSend, strlen(dataToSend), 0);
            
        }
    else if(strcmp(arg1st, "DIV")==0){
            int firsNUm = atoi(strtok(NULL, " "));
            int secondNum = atoi(strtok(NULL, " \n"));
            float ans = (float)firsNUm/secondNum;
            char dataToSend[20];
            sprintf(dataToSend, "%f", ans);
            int l=0;
            for(i=0;dataToSend[i]!='\0';i++){
                l++;
            }
            strcat(dataToSend, "\n");
            send(client, dataToSend, strlen(dataToSend), 0);
            
        }
    else{
            char dataToSend[150]="400 ";
            strcat(dataToSend, arg1st);
            strcat(dataToSend, " Command not implemented");
            strcat(dataToSend, "\n");
            int l =0;
            for(i=0;dataToSend[i]!='\0';i++){
                l++;
            }
            send(client, dataToSend, l, 0);
            
        }
    
          
}


void* Child(void* arg)
{   char line[DEFAULT_BUFLEN];
    int bytes_read;
    int client = *(int *)arg;
    char *sender = "Welcome to Bob server\n";
     send(client, sender, strlen(sender), 0);
       send(client, "\n", 2, 0);

    do
    {
        bytes_read = recv(client, line, sizeof(line), 0);
        if (bytes_read > 0) {
                if ( (bytes_read=send(client, line, bytes_read, 0)) < 0 ) {
                        printf("Send failed\n");
                        break;
                }
                
                 authentication(client, line, &bytes_read);
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

