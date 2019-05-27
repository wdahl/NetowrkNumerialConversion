#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <ctype.h>

void error(char* argv[]){
    printf("Usage: %s -p [port-number]\n", argv[0]);
    printf("Connect on specifed port to client and converts \n\n");
    printf("-p    Port number for the server to listen on\n");
    exit(0);
}

char* rtoa(char* value){
    int i = 0;
    int result = 0;
    char *buf;
    buf = malloc(sizeof(char)*1024);
    while(value[i]){
        switch(value[i]){
            case 'I':
                if(i+1 < strlen(value)){
                    switch (value[i+1]){
                        case 'V':
                            result += 4;
                            i+=2; 
                            break;

                        case 'X':
                            result += 9;
                            i+=2;
                            break;

                        default:
                            result++;
                            i++;
                    }
                }
                else{
                    result++;
                    i++;
                }
                break;

            case 'V':
                result += 5;
                i++;
                break;

            case 'X':
                if(i+1 < strlen(value)){
                    switch (value[i+1]){
                        case 'L':
                            result += 40;
                            i+=2; 
                            break;

                        case 'C':
                            result += 90;
                            i+=2;
                            break;

                        default:
                            result += 10;
                            i++;
                    }
                }
                else{
                    result += 10;
                    i++;
                }
                break;

            case 'L':
                result += 50;
                i++;
                break;

            case 'C':
                if(i+1 < strlen(value)){
                    switch (value[i+1]){
                        case 'D':
                            result += 400;
                            i+=2; 
                            break;

                        case 'M':
                            result += 900;
                            i+=2;
                            break;

                        default:
                            result += 100;
                            i++;
                    }
                }
                else{
                    result += 100;
                    i++;
                }
                break;

            case 'D':
                result += 500;
                i++;
                break;

            case 'M':
                result += 1000;
                i++;
                break;

            default:
                return "invlaid numerail\0";
        }
    }

    sprintf(buf, "%d", result);
    return buf;
}

char* ator(char* value){
    char *result;
    result = malloc(sizeof(char)*1024);
    int i = 0;
    int num;

    if(strlen(value) == 4){
        int j;
        num = value[0] - '0';

        for(j = 0; j < num; j++){
            result[i] = 'M';
            i++;
        }

        strcpy(value, value + 1);
    }

    if(strlen(value) == 3){
        int j;
        num = value[0] - '0';

        if(num == 9){
            result[i] = 'C';
            i++;
            result[i] = 'M';
            i++;
        }

        else if(num >= 5){
            result[i] = 'D';
            i++;

            int dif = num - 5;
            for(j=0; j < dif; j++){
                result[i] = 'C';
                i++;
            }
        }

        else if(num == 4){
            result[i] = 'C';
            i++;
            result[i] = 'D';
            i++;
        }

        else{
            for(j = 0; j < num; j++){
                result[i] = 'C';
                i++;
            }
        }

        strcpy(value, value + 1);
    }

    if(strlen(value) == 2){
        int j;
        num = value[0] - '0';
        if(num == 9){
            result[i] = 'X';
            i++;
            result[i] = 'C';
            i++;
        }

        else if(num >= 5){
            result[i] = 'L';
            i++;
            int dif = num - 5;
            for(j=0; j < dif; j++){
                result[i] = 'X';
                i++;
            }
        }

        else if(num == 4){
            result[i] = 'X';
            i++;
            result[i] = 'L';
            i++;
        }

        else{
            for(j = 0; j < num; j++){
                result[i] = 'X';
                i++;
            }
        }

        strcpy(value, value + 1);
    }

    if(strlen(value) == 1){
        int j;
        num = value[0] - '0';

        if(num == 9){
            result[i] = 'I';
            i++;
            result[i] = 'X';
            i++;
        }

        else if(num >= 5){
            result[i] = 'V';
            i++;

            int dif = num - 5;
            for(j=0; j < dif; j++){
                result[i] = 'I';
                i++;
            }
        }

        else if(num == 4){
            result[i] = 'I';
            i++;
            result[i] = 'V';
            i++;
        }

        else{
            for(j = 0; j < num; j++){
                result[i] = 'I';
                i++;
            }
        }

        strcpy(value, value + 1);
    }

    return result;
}

int main(int argc,char* argv[]) {
    if(argc < 3 || argc > 3){
        error(argv);
    } 

    if(strcmp(argv[1],"-h") == 0){
        error(argv);
    }

    if(strcmp(argv[1], "-p") != 0){
        error(argv);
    }

    int port = atoi(argv[2]);
    int server_fd;
    struct sockaddr_in server, client;
    char buffer[1024] = {0}; 

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&server, 0, sizeof(server)); 
    memset(&client, 0, sizeof(client));

    server.sin_family = AF_INET; 
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons(port); 

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    int size, j; 
    while(1){
        j = recvfrom(server_fd, (char *)buffer, 1024, MSG_WAITALL, ( struct sockaddr *) &client, &size); 
        buffer[j] = '\0';
        char* option = strtok(buffer, " ");
        char* value = strtok(NULL, " ");
        int i = 0;

        char* response;

        if(strcmp(option, "-r") == 0){
            while(value[i]) {
                value[i] = toupper(value[i]);
                i++;
            }

            response = rtoa(value);
        }
        else{
            response = ator(value);
        }

        sendto(server_fd, response, strlen(response), MSG_CONFIRM, (const struct sockaddr *) &client, size);
    }
      
    return 0; 
}