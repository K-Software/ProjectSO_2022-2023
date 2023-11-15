/* -------------------------------------------------------------------------- */
/* steer_by_wire.c                                                            */
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
#include "socket_utils.h"
#include "steer_by_wire.h"
#include "string_utils.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */
#define PROCESS_NAME "STEER BY WIRE"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

void main(void)
{
    steerByWireStart();
} 

void steerByWireStart(void) 
{
    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(SBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildSBWSocketName(socketName);

    int fd = socketOpenReadMode(PROCESS_NAME, socketName);
    while(1) {
        char log_msg[MAX_ROW_LEN_LOG];
        char command[SBW_MSG_LEN] = "";
        socketReadData(PROCESS_NAME, fd, socketName, command);
        sprintf(log_msg, "lunghezza command: %ld", strlen(command));
        addLog(SBW_LOG_FILE_NAME, log_msg);
        if (strlen(command) != 0) {
            if (strcmp(command, ECU_COMMAND_RIGHT) == 0) {
                addLog(SBW_LOG_FILE_NAME, SBW_TURN_RIGHT_LOG_MSG);
                sleep(4);
            } else if (strcmp(command, ECU_COMMAND_LEFT) == 0) {
                addLog(SBW_LOG_FILE_NAME, SBW_TURN_LEFT_LOG_MSG);
                sleep(4);
            } else {
                addLog(SBW_LOG_FILE_NAME, LOG_MSG_WRONG_COMMAND);
                sleep(1);
            }
        } else {
            addLog(SBW_LOG_FILE_NAME, LOG_MSG_NO_ACTION);
            sleep(1);
        }
    }
}