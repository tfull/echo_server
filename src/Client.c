#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int loop(const char *server, short port){
    int sock;
    struct sockaddr_in address;
    char send_buf[BUFFER_SIZE], recv_buf[BUFFER_SIZE];

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(sock < 0){
        perror("socket");
        return 1;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(server);
    address.sin_port = htons(port);

    if(connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("connect");
    }

    while(scanf("%s", send_buf) != EOF){
        int index;
        int length = strlen(send_buf);

        for(index = 0; index < length; ){
            int size = send(sock, send_buf + index, length - index, 0);
            if(size < 0){
                perror("send");
            }
            index += size;
        }

        for(index = 0; index < length + 1; ){
            int size = recv(sock, recv_buf + index, length - index, 0);
            if(size < 0){
                perror("recv");
            }
            index += size;
        }

        printf("%s\n", recv_buf);
    }

    close(sock);
    return 0;
}

int main(int argc, char *argv[]){
    if(argc < 3){
        perror("few arguments");
        return 1;
    }

    return loop(argv[1], (short)atoi(argv[2]));
}