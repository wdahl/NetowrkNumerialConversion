#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

void error(char* argv[]){
    printf("Usage: %s -s [servername] -p [port-number] [-r|-a] [value]\n", argv[0]);
    printf("Converts Roman numrials to Arabic numbers and vise versa.\n\n");
    printf("-s    Server name for the cleint to connect to\n");
    printf("-p    Port number for the client to conect to on the server\n");
    printf("-r    Value should be of type string consiting of roman numrials\n");
    printf("-a    Value should be a numeric type consiting of arabic numbers\n");
    exit(0);
}

int main(int argc,char* argv[]) {
    if(argc < 7 || argc > 7){
        error(argv);
    } 

    if(strcmp(argv[1],"-h") == 0){
        error(argv);
    }

    if(strcmp(argv[1], "-s") != 0){
        error(argv);
    }

    if(strcmp(argv[3], "-p") != 0){
        error(argv);
    }

    if(strcmp(argv[5], "-r") != 0 && strcmp(argv[5], "-a") != 0){
        error(argv);
    }

    struct sockaddr_in address; 
    int sock; 
    char buffer[1024] = {0};

    char* hostname = argv[2];
    int port = atoi(argv[4]);
    char* option = argv[5];
    char* value = argv[6];
    char* mesg = malloc(strlen(option) + strlen(value) + 2);
    char* space = " ";
    strcpy(mesg, option);
    strcat(mesg, space);
    strcat(mesg, value);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET; 
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(hostname); 

    if(connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0) 
    { 
        printf("\n Error : Connect Failed \n"); 
        exit(0); 
    } 

    int size, i;
    sendto(sock, mesg, strlen(mesg), MSG_CONFIRM, (const struct sockaddr *) &address,  sizeof(address));     
    i = recvfrom(sock, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &address, &size); 
    buffer[i] = '\0';
    printf("%s\n",buffer ); 
    return 0; 
}