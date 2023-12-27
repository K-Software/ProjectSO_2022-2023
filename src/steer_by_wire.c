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
#include "pipe_utils.h"
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

/*
 * DESCRIPTION:
 * This function executes the commands “DESTRA”, “SINISTRA”.
 */
void steerByWireStart(void) 
{
    char *pipeName = malloc(strlen(PATH_PIPE)+strlen(SBW_PIPE)+strlen(EXT_PIPE)+1);
    buildSBWPipeName(pipeName);

    int fd = pipeOpenReadMode(PROCESS_NAME, pipeName);
    while(1) {
        char log_msg[MAX_ROW_LEN_LOG];
        char command[SBW_MSG_LEN] = "";
        pipeReadData(PROCESS_NAME, fd, pipeName, command);
        if (strlen(command) != 0) {
            if (strcmp(command, ECU_COMMAND_RIGHT) == 0) {
                addLog(SBW_LOG_FILE_NAME, SBW_TURN_RIGHT_LOG_MSG);
                sleep(4);
            } else if (strcmp(command, ECU_COMMAND_LEFT) == 0) {
                addLog(SBW_LOG_FILE_NAME, SBW_TURN_LEFT_LOG_MSG);
                sleep(4);
            } else {
                sprintf(log_msg, LOG_MSG_WRONG_COMMAND, command);
                addLog(SBW_LOG_FILE_NAME, log_msg);
                sleep(1);
            }
        } else {
            addLog(SBW_LOG_FILE_NAME, LOG_MSG_NO_ACTION);
            sleep(1);
        }
    }

}