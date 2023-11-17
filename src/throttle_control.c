/* -------------------------------------------------------------------------- */
/* throttle.c                                                                 */
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
#include "string_utils.h"
#include "throttle_control.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */
#define PROCESS_NAME "THROTTLE CONTROL"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

void main(void)
{
    throttleControlStart();
} 

void throttleControlStart(void) 
{
    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(TC_SOCKET)+strlen(EXT_SOCKET)+1);
    buildTCSocketName(socketName);

    int fd = socketOpenReadMode(PROCESS_NAME, socketName);
    while(1) {
        char log_msg[MAX_ROW_LEN_LOG];
        char command[TC_MSG_LEN] = "";
        socketReadData(PROCESS_NAME, fd, socketName, command);
        if (strlen(command) != 0) {
            if (strcmp(command, ECU_COMMAND_THROTTLE) == 0) {
                addLog(TC_LOG_FILE_NAME, TC_LOG_MSG);
            } else {
                addLog(TC_LOG_FILE_NAME, LOG_MSG_WRONG_COMMAND);
            }
        } 
    }    
}