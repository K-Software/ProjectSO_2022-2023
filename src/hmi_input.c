/* -------------------------------------------------------------------------- */
/* hmi_output.c                                                               */
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "common.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */
#define PATH_SOCKET "./sockets/"
#define EXT_SOCKET ".sck"

void main() {
    int fd;

    char comando[HMI_INPUT_MSG_LEN];

    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(HMI_INPUT_SOCKET)+strlen(EXT_SOCKET)+1);
    strcpy(socketName, PATH_SOCKET);
    strcat(socketName, HMI_INPUT_SOCKET);
    strcat(socketName, EXT_SOCKET);
    printf("1 - FIFO name: %s\n", socketName);

    // Creazione della named pipe
    mkfifo(socketName, 0666);
    printf("2 - mkfifo\n");

    do {
        // Apertura della named pipe per la scrittura
        fd = open(socketName, O_WRONLY);
        if (fd == -1) {
            printf("3 - Errore nell'apertura del FIFO %s\n", socketName);
            sleep(15);
        } else {
            printf("3 - Aperto il FIFO %s\n", socketName);

            printf("Inserisci il comando: ");
            scanf("%s", comando);
            write(fd, comando, strlen(comando) + 1);
        }
    } while (1);
    close(fd);
    printf("4 - Chiudo il FIFO %s\n", socketName);
}
