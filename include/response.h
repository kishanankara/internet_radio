#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct {
    uint8_t replyType;
    uint16_t numStations;
} welcome;

typedef struct {
    uint8_t replyType;
    uint8_t songnameSize;
    char* songname;
} announce;

typedef struct {
    uint8_t replyType;
    uint8_t replyStringSize;
    char* replyString;
} invalid_command;

#endif
