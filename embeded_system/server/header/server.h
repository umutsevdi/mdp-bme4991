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
} sv_conf;

typedef struct SV_MOTION_DATA {
  short dir_x;
  short dir_y;
  unsigned long timestamp;
} sv_motion;

/**
 * creates a UDP server, must be executed in a separate
 * thread
 * @param arg {@link server_config}
 */
void sv_listen(void *arg);

enum direction sv_motion_get_dir(short dir_x, short dir_y);

#endif /* ifndef UDP_SERVER */

