#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

const int listen_n = 10;

int serve(short port){
    int sock;
    fd_set read_fd, write_fd;
    struct sockaddr_in address;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(sock < 0){
        perror("socket");
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(sock, (struct sockaddr*)&address, sizeof(address));

    listen(sock, listen_n);

    FD_ZERO(&read_fd);
    FD_SET(sock, &read_fd);

    while(1){
        int i;
        fd_set rfd;

        memcpy(&rfd, &read_fd, sizeof(read_fd));

        if(select(FD_SETSIZE, &rfd, NULL, NULL, NULL) <= 0){
            perror("select");
            return 1;
         }

        for(i = 0; i < FD_SETSIZE; i++){
            if(FD_ISSET(i, &rfd)){
                if(i == sock){
                    int fd = accept(sock, NULL, NULL);
                    if(fd < 0){
                        perror("accept");
                        return 1;
                    }else{
                        printf("connect to %d\n", fd);
                        FD_SET(fd, &read_fd);
                    }
                }else{
                    int read_size;
                    char buf[BUFFER_SIZE];

                    read_size = read(i, buf, BUFFER_SIZE);

                    if(read_size < 0){
                        perror("read");
                        return 1;
                    }else if(read_size == 0){
                        printf("disconnect to %d\n", i);
                        FD_CLR(i, &read_fd);
                    }else{
                        int size;
                        int index;
                        fd_set wfd, write_fd;

                        FD_ZERO(&write_fd);
                        FD_SET(i, &write_fd);

                        printf("read %d byted from %d\n", read_size, i);

                        for(index = 0; index < read_size; ){
                            memcpy(&wfd, &write_fd, sizeof(write_fd));
                            if(select(FD_SETSIZE, NULL, &wfd, NULL, NULL) < 0){
                                perror("select");
                                return 1;
                            }
                            if(FD_ISSET(i, &wfd)){
                                size = write(i, buf + index, read_size - index);
                                if(size < 0){
                                    perror("write");
                                    return 1;
                                }
                                index += size;
                            }
                        }
                    }
                }
            }
         }
    }

    close(sock);

    return 0;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "few arguments\n");
        return 1;
    }
    return serve((short)atoi(argv[1]));
}
