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
            char data[FWC_MSG_LEN];
            socketReadData(fd, socketName, data);
            printf("Data: %s\n", data);
            sleep(1);
        }
        
        socketClose(fd, socketName);
        free(socketName);
    }
}