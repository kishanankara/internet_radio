#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 1024

int stream_track(int udpPort, int channel) {
         int bytes, total_bytes;
         int udpsock = socket(AF_INET, SOCK_DGRAM, 0);
         if (udpsock < 0) {
            close(udpsock);
            perror("error creating the udp socket\n");
         }

         struct sockaddr_in udpservaddr;
         char buffer[BUFSIZE];
         udpservaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
         udpservaddr.sin_port = htons(udpPort);
         int file = open(udpPort, O_RDONLY);
         while(read(file, buffer, BUFSIZE)) {
            if((bytes = sendto(udpsock, buffer, BUFSIZE, 0, (struct sockaddr *)&udpservaddr, sizeof(udpservaddr))) < 0) {
                perror("failed sendto\n");
                exit(0);
            }
            total_bytes += bytes;
         }
    printf("Call back this function.\n");    
    return 0;
}

volatile int stop = 0;
int handler(int sigint) {
    stop = 1;
    return 0;
}

int main(int argc, char **argv){
    int fd;
    int listener;
    int cl;
    struct sockaddr_in myaddr;
    if (argc < 3) {
        fprintf(stderr, "usage: ./server port file1 [file2 [file3 [...]]]");
        exit(1);
    }
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("Something went wrong creating the socket!");
    }

    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    myaddr.sin_port = htons(atoi(argv[1]));
    bind(listener, (struct sockaddr_in *) &myaddr, sizeof(myaddr));

    //Handshake
    if(listen(listener, 10) == -1) {
        perror("listen: failed");
        close(listener);
        exit(1);
    }
    int buffer[2];

    fd_set r_fds;
    fd_set master;
    FD_SET(listener, &master);
    fd = listener;
    int fdmin=listener;
    while(1) {
         r_fds = master;
         int rv = select(fd+1, &r_fds, NULL, NULL, NULL);
         //FD_SET(cl, &r_fds);
         struct sockaddr_in peer_addr;
         char addr[50];
         socklen_t peer_addr_len = sizeof(peer_addr);
         for (int i = fdmin; i<= fd; i++) {
             if (FD_ISSET(i, &r_fds)) {
                 if (i == listener) {
                    cl = accept(fd, (struct sockaddr*)&peer_addr, &peer_addr_len);
                    fd = cl;
                    FD_SET(cl, &master);
                    if (cl>0) {
                    inet_ntop(AF_INET, &peer_addr.sin_port, addr, sizeof(addr));
                    printf("Accepted connection: cl->%d , %s:%d\n", cl, addr, peer_addr.sin_port);
                    }
                    // Clean buffer for next op.
                    buffer[0] = NULL;
                    buffer[1] = NULL;
                    int udpPort;
                    int rec = recv(cl, buffer, sizeof buffer, 0);
                    printf("buffer %d %d %d\n", cl, ntohl(buffer[0]), ntohl(buffer[1]));
                    socklen_t len;
                    udpPort = ntohl(buffer[4]);
                    int welcomePacket[2];
                    welcomePacket[0] = htonl(0);
                    welcomePacket[1] = htonl(argc-2);
                    int bytes_sent = send(cl, &welcomePacket, sizeof(welcomePacket), 0);
                     }
             }
             else {
                   for (int j=fdmin; j <=fd; j++) {
                       if(FD_ISSET(j, &master)) {
                           if(j != listener && j != i) {
                               int buffer[2];
                               int rec;
                               if((rec = recv(cl, buffer, sizeof buffer, 0)) > 0)
                               {
                                   printf("value in buffer %d\n", ntohs(buffer[0]));
                               }
                           }
                       }
                }
             }
         }
     }
    close(fd);
    return 0;
}
