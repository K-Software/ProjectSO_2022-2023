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
#include "pipe_utils.h"
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

/*
 * DESCRIPTION:
 * This function shows the commands that Central ECU sends to other components
 */
int hmiOutputStart(void) 
{
    char *pipeName = malloc(strlen(PATH_PIPE)+strlen(HMI_OUTPUT_PIPE)+strlen(EXT_PIPE)+1);
    buildHMIOutputPipeName(pipeName);

    initPipes();

    printf("HMI OUTPUT\n");    
    int fd = pipeOpenReadMode(PROCESS_NAME, pipeName);
    while(1) {
        char log_msg[MAX_ROW_LEN_LOG];
        char command[HMI_OUTPUT_MSG_LEM] = "";
        pipeReadData(PROCESS_NAME, fd, pipeName, command);
        if (strlen(command) > 0) {
            printf(" -> %s\n", command);
        }
        //sleep(1);
    }
}

/*
 * DESCRIPTION:
 * This function create the fifo pipe for HMI Output
 */
void initPipes(void)
{
    // HMI Output pipe
    char *pipeHMIOutput = malloc(strlen(PATH_PIPE)+strlen(HMI_OUTPUT_PIPE)+strlen(EXT_PIPE)+1);
    buildHMIOutputPipeName(pipeHMIOutput);
    mkfifo(pipeHMIOutput, 0666);
    free(pipeHMIOutput);
}