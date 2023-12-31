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
#include "pipe_utils.h"
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

/*
 * DESCRIPTION
 * This function executes the command “INCREMENTO 5”.
 */
void throttleControlStart(void) 
{
    char *pipeName = malloc(strlen(PATH_PIPE)+strlen(TC_PIPE)+strlen(EXT_PIPE)+1);
    buildTCPipeName(pipeName);

    int fd = pipeOpenReadMode(PROCESS_NAME, pipeName);
    while(1) {
        char log_msg[MAX_ROW_LEN_LOG];
        char command[TC_MSG_LEN] = "";
        pipeReadData(PROCESS_NAME, fd, pipeName, command);
        if (strlen(command) > 0) {
            if (strcmp(command, ECU_COMMAND_THROTTLE) == 0) {
                addLog(TC_LOG_FILE_NAME, TC_LOG_MSG);
            } else {
                sprintf(log_msg, LOG_MSG_WRONG_COMMAND, command);
                addLog(TC_LOG_FILE_NAME, log_msg);
            }
        }
    }    
}