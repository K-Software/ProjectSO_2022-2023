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
#include "throttle_control.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

#ifdef DEBUG
void main(void)
{
    throttleControlStart();
} 
#endif

void throttleControlStart(void) 
{
    int fd, esito;
    char command[TC_MSG_LEN];

    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(TC_LOG_FILE_NAME)+strlen(EXT_SOCKET)+1);
    strcpy(socketName, PATH_SOCKET);
    strcat(socketName, TC_LOG_FILE_NAME);
    strcat(socketName, EXT_SOCKET);

    do {
        addLog(TC_LOG_FILE_NAME, "Apro il FIFO");
        fd = open(socketName,O_RDONLY | O_NONBLOCK);
        if (fd == -1) {
            addLog(TC_LOG_FILE_NAME, "Errore nell'apertura del FIFO");
            sleep(15);
        } else {
            addLog(TC_LOG_FILE_NAME, "Aperto il FIFO");
        }
    } while (fd == -1);
    free(socketName);

    while(1) {
        if ((esito = read(fd, command, sizeof(command))) != 0) {
            if (strcmp(command, ECU_COMMAND_THROTTLE) == 0) {
                addLog(TC_LOG_FILE_NAME, TC_LOG_MSG);
            } else {
                addLog(TC_LOG_FILE_NAME, LOG_MSG_WRONG_COMMAND);
            }
            sleep(1);
        } else {
            addLog(TC_LOG_FILE_NAME, LOG_MSG_NO_ACTION);
            sleep(1);
        }
    }    
}