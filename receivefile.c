#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <errno.h>

using namespace std;

int receive_image(int socket){
    int imgc = 0;
    int bufferSize = 0, recv_size = 0, size = 0, read_size, write_size, packet_index=1, stat;

    char imagearray[10240], verify='1';
    FILE *image;
    int value = 0;

    value++;

    do {
        stat = read(socket, &size, sizeof(int));
    }while(stat < 0);

    fprintf(stderr, "size data %d\n", size);

    if(size > 1000){
        imgc = imgc + 1;
        char buffer[] = "Got it";
        do {
            stat = write(socket, &buffer, sizeof(int));
        }while(stat<0);
        printf("Reply sent\n");
        printf("\n");
        image = fopen("test_recv.jpg", "wb");

        if(image == NULL){
            printf("Error file open");
            return -1;
        }

        //loop while we have not receive the entire file yet
        int need_exit = 0;
        struct timeval timeout = {10, 0};

        fd_set fds;
        int buffer_fd, buffer_out;

        while(recv_size < size){
            FD_ZERO(&fds);
            FD_SET(socket, &fds);

            buffer_fd = select(FD_SETSIZE, &fds, NULL, NULL, &timeout);
            if(buffer_fd < 0){
                printf("error bad file descriptor set\n");
            } 
            if(buffer_fd == 0){
                printf("error bad file time out expired \n");
            }
	    if(buffer_fd >0){ 
                do{
                    read_size = read(socket, imagearray, 10240);
                }while(read_size<0);

                printf("Packent number received %i\n", packet_index);
                printf("Packent size %i\n", read_size);

                write_size = fwrite(imagearray, 1, read_size, image);
                printf("Written image size %i\n", write_size);

                if(read_size != write_size){
                    printf("error in read write\n");
                }
                recv_size += read_size;
                packet_index++;
                printf("Tottal received image size %i\n", recv_size);
                printf(" \n");
		}
            } 
            fclose(image);
            printf("Image successfully");
        		
	}
return 1;


    }



int main(int argc, char *argv[]){
    int socket_desc;
    struct sockaddr_in server;
    char *parray;

    socket_desc =  socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1){
        fprintf(stderr,"socketerror");
        return -1;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8000);

    //connect
    if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) <0){
        cout<<strerror(errno);
        close(socket_desc);
        puts("connect error");
        return 1;
    }
    puts("connectied\n");
    receive_image(socket_desc);
    close(socket_desc);
    return 0;
 }
