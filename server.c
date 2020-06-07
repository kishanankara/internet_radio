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

struct Welcome {
    uint8_t replyType;
    uint16_t numStations;
};

int main(int argc, char **argv){
    int fd;
    int cl;
    struct sockaddr_in myaddr;
    char *msg = "WELCOME";
    if (argc < 3) {
        fprintf(stderr, "usage: ./server port file1 [file2 [file3 [...]]]");
        exit(1);
    }
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("Something went wrong creating the socket!");
    }

//    val->replyType = 0;
//    val->numStations = argc-2;
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    myaddr.sin_port = htons(atoi(argv[1]));
    bind(fd, (struct sockaddr_in *) &myaddr, sizeof(myaddr));
    
    //Handshake
    if(listen(fd, 1) == -1) {
        perror("listen: failed");
        close(fd);
        exit(1);
    }
    int buffer[2];
    while(1) {
      cl = accept(fd, NULL, NULL);

   /* Basically we want to handle requests for connections that have 
       been accepted, while we keep processing incoming requests. */

     fd_set readfds;
     struct timeval tv;

     FD_ZERO(&readfds);

     FD_SET(cl, &readfds);

     tv.tv_sec = 10;
     tv.tv_usec = 500000;

     int rv = select(cl+1, &readfds, NULL, NULL, &tv);

     if (rv == -1) {
        perror("select failed");
     } else if (rv == 0) {
        printf("Timeout occured! \n");
     } else {
         int rec = recv(cl, &buffer, sizeof(buffer), 0);
         socklen_t len;
         struct sockaddr_storage addr;
         char ipstr[1024];
         int port;
         int *addrlen;
         len = sizeof addr;
         int peerval = getpeername(cl, &addr, &len);
         struct sockaddr_in *s = (struct sockaddr_in *)&addr;
         port = ntohs(s->sin_port);
         inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
         printf("session id:%d \n", cl);
         int udpPort = ntohl(buffer[1]);
         printf("Udp Port: %d\n", ntohl(buffer[1]));
         int welcomePacket[2];

         welcomePacket[0] = htonl(0);
         welcomePacket[1] = htonl(argc-2);
         int bytes_sent = send(cl, &welcomePacket, sizeof(welcomePacket), 0);

         printf("Bytes sent - %d\n", bytes_sent);

        close(cl);
     }

    }

   // int udpsock = socket(AF_INET, SOCK_DGRAM, 0);
  //  if (udpsock < 0) {
  //      close(udpsock);
  //      perror("error creating the udp socket\n");
   // }

   // struct sockaddr_in udpservaddr;
   // char * my_message = "this is a test message to portray how much data can be sent";
   // udpservaddr.sin_family = AF_INET;
   // udpservaddr.sin_port = htons(atoi(udpPort));

   // if(sendto(udpsock, my_message, strlen(my_message), 0, (struct sockaddr *)&udpservaddr, sizeof(udpservaddr)) < 0) {
    //    perror("failed sendto\n");
   //     return 0;
   // }

    //struct addrinfo hints, servinfo, *p;
    //int rv;
    //int numbytes;
    //memset(&hints, 0, sizeof hints);
    //hints.ai_family = AF_INET;
    //hints.ai_socktype = SOCK_DGRAM;

    //if ((rv = getaddrinfo(NULL, udpPort, &hints, &servinfo)) != 0) {
    //    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    //    return i;
    //}

    close(fd);
    return 0;
}
