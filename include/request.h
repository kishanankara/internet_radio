#ifndef REQUEST_H
#define REQUEST_H

typedef struct {
    uint8_t commandType = 0;
    uint16_t udpPort;
} hello;

typedef struct {
    uint8_t commandType = 1;
    uint16_t stationNumber;
} set_station

#endif
