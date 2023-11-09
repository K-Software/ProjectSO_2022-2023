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

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

#ifdef DEBUG
void main(void)
{
    brakeByWireStart();
} 
#endif

void brakeByWireStart(void) {
    int fd, esito;
    char command[BBW_MSG_LEN];

    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(BBW_LOG_FILE_NAME)+strlen(EXT_SOCKET)+1);
    strcpy(socketName, PATH_SOCKET);
    strcat(socketName, BBW_LOG_FILE_NAME);
    strcat(socketName, EXT_SOCKET);

    do {
        addLog(BBW_LOG_FILE_NAME, "Apro il FIFO");
        fd = open(socketName,O_RDONLY | O_NONBLOCK);
        if (fd == -1) {
            addLog(BBW_LOG_FILE_NAME, "Errore nell'apertura del FIFO");
            sleep(15);
        } else {
            addLog(BBW_LOG_FILE_NAME, "Aperto il FIFO");
        }
    } while (fd == -1);
    free(socketName);

    while(1) {
        if ((esito = read(fd, command, sizeof(command))) != 0) {
            if (strcmp(command, ECU_COMMAND_BRAKE) == 0) {
                addLog(BBW_LOG_FILE_NAME, BBW_LOG_MSG);
            } else {
                addLog(BBW_LOG_FILE_NAME, LOG_MSG_WRONG_COMMAND);
            }
            sleep(1);
        } else {
            addLog(BBW_LOG_FILE_NAME, LOG_MSG_NO_ACTION);
            sleep(1);
        }
    }
}