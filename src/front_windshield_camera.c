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
#include "socket_utils.h"
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

    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(FWC_SOCKET)+strlen(EXT_SOCKET)+1);
    buildFWCSocketName(socketName);

    // mkfifo(socketName, 0666);

    // Leggere e stampare il contenuto del file riga per riga
    char data[FWC_MSG_LEN];  // Una stringa temporanea per leggere ciascuna riga
    while (fgets(data, sizeof(data), frontCameraData) != NULL) {
        // Write data in FIFO
        sendData(socketName, data);
        addLog(FWC_LOG_FILE_NAME, data);
        sleep(1);
    }

    // Chiudere il file dopo aver terminato la lettura
    fclose(frontCameraData);
    free(socketName);

    return 0;
}

/*
 * DESCRIPTION:
 * This function sends data to socket.
 *
 * PARAMETERS:
 * - socketName = name of socket
 * - data = data to send
 */
void sendData(char *socketName, char *data)
{
    int fd = socketOpenWriteMode(PROCESS_NAME, socketName);
    socketWriteData(PROCESS_NAME, fd, socketName, data);
    socketClose(PROCESS_NAME, fd, socketName);
}