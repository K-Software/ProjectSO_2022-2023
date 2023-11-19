/* -------------------------------------------------------------------------- */
/* park_assist.c                                                              */
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
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
    // TODO: ...
    int fd = open("/dev/urandom", O_RDONLY);

    if (fd == -1) {
        char log_msg[MAX_ROW_LEN_LOG];
        sprintf(log_msg, "Errore nell'apertura di /dev/urandom");
        addLog(PA_DEBUG_FILE_NAME, log_msg);
    } else {

        for (int i = 0; i < 30; i++) {
            // Buffer per contenere i 8 byte letti
            unsigned char buffer[8];

            // Leggi 8 byte da /dev/urandom
            ssize_t bytesRead = read(fd, buffer, sizeof(buffer));

            // Controlla se la lettura è avvenuta con successo
            if (bytesRead != sizeof(buffer)) {
                perror("Errore nella lettura da /dev/urandom");
                close(fd);
                exit(EXIT_FAILURE);
            }

            // Log di dati letti
            for (int i = 0; i < sizeof(buffer); i += 2) {
                char log_msg[MAX_ROW_LEN_LOG];
                sprintf(log_msg, "0x%02X%02X", buffer[i], buffer[i + 1]);
                addLog(PA_LOG_FILE_NAME, log_msg);
            }
            sleep(1);
        }
        // Chiudi il file descriptor
        close(fd);
    }
}

