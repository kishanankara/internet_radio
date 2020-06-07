#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#define BUFSIZE 1024

struct HELLO {
    uint8_t command_type;
    uint16_t udpPort;
};

void process_input(char* buf){
    switch(*buf) {
        case '1':
            printf("Cool!\n");
            break;
        case 'q':
            printf("Calling Quits!\n");

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

    int buffer[2];
    int d = recv(fd, &buffer, sizeof(buffer), 0);
    //printf("Handshake Complete: %u %u",ntohl(buffer[0]), ntohl(buffer[1]));
    printf("Sent a hello message to server.\n");
    //Listen on stdin
    fd_set readfds;

    //FD_ZERO(&readfds);
    //FD_SET(0, &readfds);

    char buf[1024];

    //while(1) {
    for(;;) {
     printf("> ");
    //int retval = select(1, &readfds, NULL, NULL, NULL);

    //if (retval == -1)
    //   { perror("select()"); }

    //printf("\n> ");
//    while(1) {
    //else {
    scanf("%s", buf);
    process_input(&buf);
    }
//}
close(fd);

    return 0;
}
