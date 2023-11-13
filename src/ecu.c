/* -------------------------------------------------------------------------- */
/* ecu.c                                                                      */
/* ---------------------------------------------------------------------------*/
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
#include <sys/wait.h>
#include <unistd.h>
#include "common.h"
#include "ecu.h"
#include "front_windshield_camera.h"
#include "log.h"
#include "socket_utils.h"
#include "string_utils.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

int main(void)
{
    ecuStart();
}

void ecuStart(void)
{
    int speed = 0;
    int pidSteerByWire = 0;
    int pidThrottleontrol = 0;
    int pidBrakeByWire = 0;  
    int pidFrontWindshieldCamera = 0;

    pidFrontWindshieldCamera = fork();
    if (pidFrontWindshieldCamera == 0) {

        /* Front Windshield camere */
        frontWindshieldCameraStart();
    } else {

        /* ECU */
        char *socketName = malloc(strlen(PATH_SOCKET)+strlen(FWC_SOCKET)+strlen(EXT_SOCKET)+1);
        buildFWCSocketName(socketName);

        int fd = socketOpenReadMode(socketName);
        while (1) {
            char command[FWC_MSG_LEN];
            socketReadData(fd, socketName, command);
            printf("Data: %s - ", command);
            addLog(ECU_LOG_FILE_NAME, command);
            if (strcmp(FWC_COMMAND_LEFT, command) == 0) {
                printf("Steer by wire: LEFT\n");
            } else if (strcmp(FWC_COMMAND_RIGHT, command) == 0) {
                printf("Steer by wire: RIGHT\n");
            } else if (strcmp(FWC_COMMAND_PARKING, command) == 0) {
                printf("Parking\n");
            } else if (strcmp(FWC_COMMAND_DANGER, command) == 0) {
                printf("!!! DANGER!!!\n");
            } else {
                printf("Speed\n");
            }
            
            sleep(1);
        }
        
        socketClose(fd, socketName);
        free(socketName);
    }
}

/*
 Provare gestire la creazione di tutti i fifo qui 
*/

void sendDataToSBWComponent(char *command) {
    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(SBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildFWCSocketName(socketName);
    int fd = socketOpenWriteMode(socketName);
    socketWriteData(fd, socketName, command);
    socketClose(fd, socketName);
}