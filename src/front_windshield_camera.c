/* -------------------------------------------------------------------------- */
/* front_windshield_camera.c                                                  */
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <unistd.h>
#include "common.h"
#include "front_windshield_camera.h"
#include "log.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */
#define DATA_FILE_NAME "./data/frontCamera.data"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

#ifdef DEBUG
int main(void) 
{
    if (frontWindshieldCameraStart() == 0) {
        printf("Front windshield camera started successfully");
    } else {
        printf("Front windshield camera started unsuccessfully");
    }
}
#endif

int frontWindshieldCameraStart(void) 
{
    // Dichiaro un puntatore a FILE
    FILE *frontCameraData;

    // Apro il dei dati in in lettura
    frontCameraData = fopen(DATA_FILE_NAME, "r");

    if (frontCameraData == NULL)
    {
        printf("Errore nella lettura dei dati");
        return 1;
    }

    // Leggere e stampare il contenuto del file riga per riga
    char dato[25];  // Una stringa temporanea per leggere ciascuna riga
    while (fgets(dato, sizeof(dato), frontCameraData) != NULL) {
        addLog(FWC_LOG_FILE_NAME, dato);
        sleep(1);
    }

    // Chiudere il file dopo aver terminato la lettura
    fclose(frontCameraData);

    return 0;
}