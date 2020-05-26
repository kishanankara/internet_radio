#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>

struct HELLO {
    uint8_t command_type;
    uint16_t udpPort;
};

int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "Usage: ./snowcast_control servername serverport udpport\n");
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
    printf("Handshake Complete: %u %u",ntohl(buffer[0]), ntohl(buffer[1]));
    close(fd);
    return 0;
}
