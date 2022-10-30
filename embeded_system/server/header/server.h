#ifndef UDP_SERVER
#define UDP_SERVER

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

enum direction {
    DIRECTION_NONE,
    DIRECTION_LEFT,
    DIRECTION_DOWN,
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_UP_LEFT,
    DIRECTION_DOWN_LEFT,
    DIRECTION_UP_RIGHT,
    DIRECTION_DOWN_RIGHT,
};
/**
 * configuration related information about the UDP
 * listener server
 */
typedef struct SV_SERVER_CONFIG {
    int port;
    int max_line;
    int should_respond;
} sv_conf;

typedef struct SV_MOTION_DATA {
    short dir_x;
    short dir_y;
    unsigned long timestamp;
} sv_motion;

/**
 * creates a UDP server, must be executed in a  separate process
 * @param arg arguments of server in server_config
 * @param write_pipe write file descriptor for the server
 */
void sv_listen(const sv_conf *args, int write_pipe);

/**
 * parses incoming UDP data into a valid struct
 * @param buffer incoming message as char array
 * @return struct data
 */
enum direction sv_motion_get_dir(short dir_x, short dir_y);

#endif
