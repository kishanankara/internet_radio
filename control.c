#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
//#include "include/request.h"
#define BUFSIZE 1024

struct HELLO {
    uint8_t command_type;
    uint16_t udpPort;
};

int send_to_server(int channel, int* fd) {
    char buffer[1024];
    printf("sending to server\n");
    int d = send(fd, htons(channel), sizeof channel, 0);
    printf("sent bytes %d\n", d);
    //return 0;
}

int process_input(char* buf){
    printf("process input\n");
    switch(*buf) {
        case '1':
            printf("returning 1\n");
            return 1;
        case 'q':
            return 0;
    }
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "Usage: ./control servername serverport udpport\n");
        exit(1);
    }
    struct sockaddr_in myaddr; 
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("Something went wrong creating the socket!");
    } 
    myaddr.sin_family = AF_INET;
    inet_aton(argv[1], &myaddr.sin_addr.s_addr);
    myaddr.sin_port = htons(atoi(argv[2]));
    int status = connect(fd, (struct sockaddr_in *) &myaddr, sizeof(myaddr));
    if (status < 0) {
        perror("Error connecting to server\n");
    }
    //Handshake
    int byte_array[2];
    byte_array[0]=htonl(0);
    byte_array[1]=htonl(atoi(argv[3]));    
    int data_to_send = send(fd, &byte_array, sizeof(byte_array), 0);

    char hostname[128];
    //struct hostent* host_entry;
    gethostname(hostname, sizeof hostname);
   // char* ip;
   // ip = inet_ntoa((struct in_addr*)host_entry->h_addr_list[0]);
    printf("%s -> %s\n", argv[1], "127.0.0.1");
    int buffer[2];
    int d = recv(fd, &buffer, sizeof(buffer), 0);
    //printf("Handshake Complete: %u %u",ntohl(buffer[0]), ntohl(buffer[1]));
    //Listen on stdin
    fd_set readfds;
    printf("Type in a number to set the station we're listening to to that number.\n");
    printf("Type in 'q' or press CTRL+C to quit.\n");
    //FD_ZERO(&readfds);
    //FD_SET(0, &readfds);

    char buf[1024];
    printf("> The server has %d stations.\n", ntohl(buffer[1]));
    // while(1) {
    for(;;) {
     printf("> ");
    //int retval = select(1, &readfds, NULL, NULL, NULL);

    //if (retval == -1)
    //   { perror("select()"); }

    //printf("\n> ");
//    while(1) {
    //else {
    scanf("%s", buf);
    int f[1], y;
    if (!(y=process_input(&buf))) {
        break;
    }
     f[0] = htons(y);
     printf("printing the value of f %d\n", f[0]);
      int d = send(fd, &f, sizeof f, 0);
      printf("sent bytes: %d\n", d);
    }
close(fd);
exit(0);
}
