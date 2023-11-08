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

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

int main(void) {
    int fd, esito;
    char comunicazione[HMI_INPUT_MSG_LEN];

    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(HMI_OUTPUT_SOCKET)+strlen(EXT_SOCKET)+1);
    strcpy(socketName, PATH_SOCKET);
    strcat(socketName, HMI_OUTPUT_SOCKET);
    strcat(socketName, EXT_SOCKET);
    printf("1 - FIFO name: %s\n", socketName);

    do {
        printf("2 - Apro il FIFO\n");
        fd = open(socketName,O_RDONLY | O_NONBLOCK);
        if (fd == -1) {
            printf("3 - Errore nell'apertura del FIFO %s\n", socketName);
            sleep(15);
        } else {
            printf("3 - Aperto il FIFO %s\n", socketName);
        }
    } while (fd == -1);
    
    while(1) {
        printf("4 - ");
        strcpy(comunicazione,"         ");
        if ((esito = read(fd, comunicazione, sizeof(comunicazione))) != 0) {
            printf("Comunicazione: %s\n", comunicazione);
            sleep(1);
        } else {
            printf("Nessuna comunicazione\n");
            sleep(1);
        }
    }
}