/* -------------------------------------------------------------------------- */
/* brake_by_wire.c                                                            */
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
#include "log.h"
#include "brake_by_wire.h"
#include "socket_utils.h"
#include "string_utils.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */
#define PROCESS_NAME "BRAKE BY WIRE"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

void main(void)
{
    signal(SIGALRM, handlerArresto);

    brakeByWireStart();
} 

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void brakeByWireStart(void) 
{
    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(BBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildBBWSocketName(socketName);

    int fd = socketOpenReadMode(PROCESS_NAME, socketName);
    while(1) {
        char log_msg[MAX_ROW_LEN_LOG];
        char command[BBW_MSG_LEN] = "";
        socketReadData(PROCESS_NAME, fd, socketName, command);
        if (strlen(command) != 0) {
            if (strcmp(command, ECU_COMMAND_BRAKE) == 0) {
                addLog(BBW_LOG_FILE_NAME, BBW_BRAKE_MSG);
            } else {
                addLog(BBW_LOG_FILE_NAME, LOG_MSG_WRONG_COMMAND);
            }
        }
        sleep(1);
    }
}

void handlerArresto(int signum) {
    addLog(BBW_LOG_FILE_NAME, BBW_STOP_MSG);
}