// Includes
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string>
#include <netinet/in.h>
#include <vector>
#include <unistd.h>

typedef struct sockaddr s_sockaddr;
typedef struct sockaddr_in s_sockaddr_in;
typedef int socket_t;
const int BUFFER_SIZE = 30720;

// type definitions
static const short AF = AF_INET;
static const char *ADDRESS = "0.0.0.0";
