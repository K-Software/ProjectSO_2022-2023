/* -------------------------------------------------------------------------- */
/* hmi_output.c                                                               */
/* -------------------------------------------------------------------------- */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h> /* For AF_UNIX sockets */
#include <unistd.h>
#include "common.h"
#include "hmi_output.h"
#include "socket_utils.h"
#include "string_utils.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */
#define PROCESS_NAME "HMI OUTPUT"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

void main(void) 
{
    hmiOutputStart();
}


int hmiOutputStart(void) 
{
    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(HMI_OUTPUT_SOCKET)+strlen(EXT_SOCKET)+1);
    buildHMIOutputSocketName(socketName);

    initSockets();

    printf("HMI OUTPUT\n");    
    int fd = socketOpenReadMode(PROCESS_NAME, socketName);
    while(1) {
        char log_msg[MAX_ROW_LEN_LOG];
        char command[HMI_OUTPUT_MSG_LEM] = "";
        socketReadData(PROCESS_NAME, fd, socketName, command);
        if (strlen(command) > 0) {
            printf(" -> %s\n", command);
        }
        //sleep(1);
    }
}

void initSockets(void)
{
    // HMI Output socket
    char *socketHMIOutput = malloc(strlen(PATH_SOCKET)+strlen(HMI_OUTPUT_SOCKET)+strlen(EXT_SOCKET)+1);
    buildHMIOutputSocketName(socketHMIOutput);
    mkfifo(socketHMIOutput, 0666);
    free(socketHMIOutput);
}