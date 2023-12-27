/* -------------------------------------------------------------------------- */
/* front_windshield_camera.c                                                  */
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "common.h"
#include "front_windshield_camera.h"
#include "log.h"
#include "pipe_utils.h"
#include "string_utils.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */
#define PROCESS_NAME "FRONT WINDSHIELD CAMERA"
#define DATA_FILE_NAME "./data/frontCamera.data"
#define OPEN_DATA_FILE_OK "Open file of data"
#define OPEN_DATA_FILE_ERR "Error during opening file of data"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

int main(void) 
{
    frontWindshieldCameraStart();
}

/*
 * DESCRIPTION:
 * This function reads data from front windshield camera and sends data to 
 * Central ECU component.
 *
 * RETURN VALUES:
 * - 0 = everything's ok
 * - 1 = Error during opening file of data
 */
int frontWindshieldCameraStart(void) 
{
    // Dichiaro un puntatore a FILE
    FILE *frontCameraData;

    // Apro il dei dati in in lettura
    frontCameraData = fopen(DATA_FILE_NAME, "r");

    if (frontCameraData == NULL)
    {
        addLog(FWC_DEBUG_FILE_NAME, OPEN_DATA_FILE_ERR);
        return 1;
    }
    addLog(FWC_DEBUG_FILE_NAME, OPEN_DATA_FILE_OK);

    char *pipeName = malloc(strlen(PATH_PIPE)+strlen(FWC_PIPE)+strlen(EXT_PIPE)+1);
    buildFWCPipeName(pipeName);

    // Leggere e stampare il contenuto del file riga per riga
    char data[FWC_MSG_LEN];  // Una stringa temporanea per leggere ciascuna riga
    while (fgets(data, sizeof(data), frontCameraData) != NULL) {
        // Write data in FIFO
        sendData(pipeName, data);
        addLog(FWC_LOG_FILE_NAME, data);
        sleep(1);
    }

    // Chiudere il file dopo aver terminato la lettura
    fclose(frontCameraData);
    free(pipeName);

    return 0;
}

/*
 * DESCRIPTION:
 * This function sends data to pipe.
 *
 * PARAMETERS:
 * - pipeName = name of pipe
 * - data = data to send
 */
void sendData(char *pipeName, char *data)
{
    int fd = pipeOpenWriteMode(PROCESS_NAME, pipeName);
    pipeWriteData(PROCESS_NAME, fd, pipeName, data);
    pipeClose(PROCESS_NAME, fd, pipeName);
}