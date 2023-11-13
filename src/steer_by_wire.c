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
#include "steer_by_wire.h"
#include "string_utils.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

#ifdef DEBUG
void main(void)
{
    steerByWireStart();
} 
#endif

void steerByWireStart(void) 
{
    int fd, esito;
    char command[SBW_MSG_LEN];

    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(SBW_LOG_FILE_NAME)+strlen(EXT_SOCKET)+1);
    buildSBWSocketName(socketName);

    do {
        addLog(SBW_LOG_FILE_NAME, "Apro il FIFO");
        fd = open(socketName,O_RDONLY | O_NONBLOCK);
        if (fd == -1) {
            addLog(SBW_LOG_FILE_NAME, "Errore nell'apertura del FIFO");
            sleep(15);
        } else {
            addLog(SBW_LOG_FILE_NAME, "Aperto il FIFO");
        }
    } while (fd == -1);
    free(socketName);

    while(1) {
        if ((esito = read(fd, command, sizeof(command))) != 0) {
            if (strcmp(command, ECU_COMMAND_RIGHT) == 0) {
                addLog(SBW_LOG_FILE_NAME, SBW_TURN_RIGHT_LOG_MSG);
            } else if (strcmp(command, ECU_COMMAND_LEFT) == 0) {
                addLog(SBW_LOG_FILE_NAME, SBW_TURN_LEFT_LOG_MSG);
            } else {
                addLog(SBW_LOG_FILE_NAME, LOG_MSG_WRONG_COMMAND);
            }
            sleep(4);
        } else {
            addLog(SBW_LOG_FILE_NAME, LOG_MSG_NO_ACTION);
            sleep(1);
        }
    }
}