/* -------------------------------------------------------------------------- */
/* hmi_output.c                                                               */
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "common.h"
#include "hmi_input.h"
#include "log.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

void main() 
{
    hmiInputStart();
}

/*
 * DESCRIPTION
 * This function sends input commands "INIZIO", "PARCHEGGIO" and "ARRESTO" to
 * Central ECU component.
 */
void hmiInputStart(void) {

    // SIGUSR1 -> INIZIO
    // SIGUSR2 -> PARCHEGGIO
    // SIGALRM -> ARRESTO
    pid_t ppid = getppid();
    char command[HMI_INPUT_MSG_LEN];
    
    printf("HMI INPUT\n");
    do {
        printf("Inserisci il comando(%s, %s, %s): ", HMI_INPUT_COMMAND_START, 
            HMI_INPUT_COMMAND_PARKING, HMI_INPUT_COMMAND_STOP);
        scanf("%s", command);
        if (strcmp(command, HMI_INPUT_COMMAND_START) == 0) {
            kill(ppid, SIGUSR1);
        } else if (strcmp(command, HMI_INPUT_COMMAND_PARKING) == 0) {
            kill(ppid, SIGUSR2);
        } else if (strcmp(command, HMI_INPUT_COMMAND_STOP) == 0) {
            kill(ppid, SIGALRM);
        } else {
            printf("Comando non valido\n");
        }
        sleep(1);
    } while (1);
}
