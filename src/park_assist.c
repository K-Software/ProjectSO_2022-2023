/* -------------------------------------------------------------------------- */
/* park_assist.c                                                              */
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "log.h"
#include "park_assist.h"
#include "pipe_utils.h"
#include "string_utils.h"

/* -------------------------------------------------------------------------- */
/* Marco                                                                      */
/* -------------------------------------------------------------------------- */
#define PROCESS_NAME "PARK ASSIST"
#define NORMALE_FILE_NAME "/dev/urandom"
#define ARTIFICIALE_FILE_NAME "./data/randomARTIFICIALE.binary"
#define ERR_MSG "Errore nell'apertura di %s"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

void main(int argc, char *argv[])
{
    addLog(PA_DEBUG_FILE_NAME, argv[1]);
    parkAssistStart(argv[1]);
}

/*
 * DESCRIPTION:
 * This function executes the command "PARCHEGGIO".
 * 
 * PARAMETERS:
 * - mode = this parameter indicates the source of data of component
 */
void parkAssistStart(char *mode)
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (strcmp("ARTIFICIALE", mode) == 0) {
        fd = open(ARTIFICIALE_FILE_NAME, O_RDONLY);
    }

    if (fd == -1) {
        char log_msg[MAX_ROW_LEN_LOG] = "";
        sprintf(log_msg, ERR_MSG, NORMALE_FILE_NAME);
        if (strcmp("ARTIFICIALE", mode) == 0) {
            sprintf(log_msg, ERR_MSG, ARTIFICIALE_FILE_NAME);
        }
        addLog(PA_DEBUG_FILE_NAME, log_msg);
    } else {

        char *pipeECU = malloc(strlen(PATH_PIPE)+strlen(ECU_PIPE)+strlen(EXT_PIPE)+1);
        buildECUPipeName(pipeECU);

        for (int i = 0; i < 30; i++) {
            // Buffer per contenere i 8 byte letti
            unsigned char buffer[8];

            // Leggi 8 byte da /dev/urandom
            ssize_t bytesRead = read(fd, buffer, sizeof(buffer));

            // Controlla se la lettura è avvenuta con successo
            if (bytesRead != sizeof(buffer)) {
                char log_msg[MAX_ROW_LEN_LOG];
                sprintf(log_msg, "Errore nella lettura da /dev/urandom");
                addLog(PA_DEBUG_FILE_NAME, log_msg);
            } else {

                // Log di dati letti
                char log_msg[MAX_ROW_LEN_LOG];
                sprintf(log_msg, "0x%02X%02X%02X%02X%02X%02X%02X%02X", 
                    buffer[0], buffer[1], buffer[2], buffer[3], buffer[4],
                    buffer[5], buffer[6], buffer[7]);
                sendData(pipeECU, log_msg);
                addLog(PA_LOG_FILE_NAME, log_msg);
            }
            sleep(1);
        }
        // Chiudi il file descriptor
        close(fd);
        free(pipeECU);
    }
}

/*
 * DESCRIPTION:
 * This function sends data to pipe.
 *
 * PARAMETERS:
 * - pipeName = name of pipe
 * - data = data to send
 */
void sendData(char *pipeName, char *command)
{
    int fd = pipeOpenWriteMode(PROCESS_NAME, pipeName);
    pipeWriteData(PROCESS_NAME, fd, pipeName, command);
}