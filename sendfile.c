#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <errno.h>

using namespace std;

void send_image(int socket){
    FILE *picture;
    int size, read_size, stat, packet_index;
    char send_buffer[10240], read_buffer[256];

    packet_index = 1;
    picture = fopen("apple.jpg", "r");
    fprintf(stderr, "Getting picture size\n");
    if(picture ==NULL){
        fprintf(stderr, "Error openingpicture size\n");
    }
    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek(picture, 0, SEEK_SET);
    fprintf(stderr, "totla psize %i\n", size);

    //send picture
    fprintf(stderr, "sending psize %i\n", size);
    write(socket, (void *)&size, sizeof(int));
    fprintf(stderr, "sending [picture as a byte array] %i\n", size);
    do {//read while we get errors taht are due to signals
        stat = read(socket, &read_buffer, 255);
        fprintf(stderr, "Bytres read: %i\n", stat);

    }while(stat < 0);
    fprintf(stderr, "Received data\n");
        fprintf(stderr, "socket data %c\n", read_buffer);

    while(!feof(picture)){
        read_size = fread(send_buffer, 1, sizeof(send_buffer)-1, picture);
        do {
            stat = write(socket, send_buffer, read_size);
        }while(stat<0);
        fprintf(stderr, "Packet number:%i\n", packet_index);
                fprintf(stderr, "Packet size sent:%i\n", read_size);


        packet_index++;
        bzero(send_buffer, sizeof(send_buffer));

    }


}


int main(int argc, char *argv[]){
    int socket_desc, new_socket, c, read_size, buffer = 0;
    struct sockaddr_in server, client;
    char *readin;
    int value =0;
    socket_desc =  socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1){
        fprintf(stderr,"socketerror");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8000);

    if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
        puts("bind");
        return 1;
    }
    puts("bind done");
    listen(socket_desc, 3);
    puts("waiting for incoming connection");
    c = sizeof(struct sockaddr_in);
    if((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){
        puts("connectino accepted");
    }
    fflush(stdout);
    if(new_socket < 0){
        puts("accept failed");
        return 1;
    }
    while(value<2000){
        value++;
        send_image(new_socket);
    }
    close(socket_desc);
    fflush(stdout);
    return 0;
 }
