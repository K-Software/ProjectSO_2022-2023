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
#include "socket_utils.h"
#include "string_utils.h"

/* -------------------------------------------------------------------------- */
/* Marco                                                                      */
/* -------------------------------------------------------------------------- */
#define PROCESS_NAME "PARK ASSIST"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

void main(void)
{
    parkAssistStart();
}

void parkAssistStart(void)
{
    int fd = open("/dev/urandom", O_RDONLY);

    if (fd == -1) {
        char log_msg[MAX_ROW_LEN_LOG];
        sprintf(log_msg, "Errore nell'apertura di /dev/urandom");
        addLog(PA_DEBUG_FILE_NAME, log_msg);
    } else {

        char *socketECU = malloc(strlen(PATH_SOCKET)+strlen(ECU_SOCKET)+strlen(EXT_SOCKET)+1);
        buildECUSocketName(socketECU);

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
                for (int i = 0; i < sizeof(buffer); i += 2) {
                    char log_msg[MAX_ROW_LEN_LOG];
                    sprintf(log_msg, "0x%02X%02X", buffer[i], buffer[i + 1]);
                    sendDataToECUComponent(socketECU, log_msg);
                    addLog(PA_LOG_FILE_NAME, log_msg);
                }
            }
            sleep(1);
        }
        // Chiudi il file descriptor
        close(fd);
    }
}

void sendDataToECUComponent(char *socketName, char *command)
{
    int fd = socketOpenWriteMode(PROCESS_NAME, socketName);
    socketWriteData(PROCESS_NAME, fd, socketName, command);
}