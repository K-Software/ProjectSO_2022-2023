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
#include <sys/un.h>
#include <unistd.h>
#include "common.h"
#include "log.h"
#include "brake_by_wire.h"
#include "pipe_utils.h"
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
 * DESCRIPTION:
 * This function executes the command "FRENO 5".
 */
void brakeByWireStart(void) 
{
    char *pipeName = malloc(strlen(PATH_PIPE)+strlen(BBW_PIPE)+strlen(EXT_PIPE)+1);
    buildBBWPipeName(pipeName);

    int fd = pipeOpenReadMode(PROCESS_NAME, pipeName);
    while(1) {
        char log_msg[MAX_ROW_LEN_LOG];
        char command[BBW_MSG_LEN] = "";
        pipeReadData(PROCESS_NAME, fd, pipeName, command);
        if (strlen(command) > 0) {
            if (strcmp(command, ECU_COMMAND_BRAKE) == 0) {
                addLog(BBW_LOG_FILE_NAME, BBW_BRAKE_MSG);
            } else if (strcmp(command, ECU_STOP) == 0) {
                addLog(BBW_LOG_FILE_NAME, BBW_STOP_MSG);
            } else {
                sprintf(log_msg, LOG_MSG_WRONG_COMMAND, command);
                addLog(BBW_LOG_FILE_NAME, log_msg);
            }
        }
    }
}

/*
 * DESCRIPTION:
 * this function overwrite the handler for signal SIGALRM. 
 *
 * PARAMETERS:
 * - signum = signal number
 */
void handlerArresto(int signum) {
    addLog(BBW_LOG_FILE_NAME, BBW_STOP_MSG);
}