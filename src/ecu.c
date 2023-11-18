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
#include "log.h"
#include "socket_utils.h"
#include "string_utils.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */
#define PROCESS_NAME "ECU"
#define ERR_FORK "Errore nella creazione del processo figlio per %s"
#define ERR_EXECLP "Errore nell'esecuzione di execlp con %s"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

int main(void)
{
    ecuStart();
}

void ecuStart(void)
{
    char prevCommand[FWC_MSG_LEN] = "";
    int speed = 0;

    initSockets();

    const char *paths[] = {"/repo_elaborato/bin/front_windshield_camera.out", 
        "/repo_elaborato/bin/steer_by_wire.out",
        "/repo_elaborato/bin/throttle_control.out",
        "/repo_elaborato/bin/brake_by_wire.out"};
    const char *components[] = {"front_windshield_camera.out", "steer_by_wire.out",
        "throttle_control.out", "brake_by_wire.out"};

    for (int i = 0; i < 4; ++i) {
        char log_msg[MAX_ROW_LEN_LOG];
        pid_t childPid = fork();

        if (childPid == -1) {
            sprintf(log_msg, ERR_FORK, components[i]);
            addLog(ECU_DEBUG_FILE_NAME, log_msg);

            exit(EXIT_FAILURE);
        } else if (childPid == 0) {
            // Codice eseguito dal processo figlio

            // Esegui un programma esterno
            execlp(paths[i], components[i], (char *)NULL);

            // Se execlp restituisce, c'è stato un errore
            sprintf(log_msg, ERR_EXECLP, components[i]);
            addLog(ECU_DEBUG_FILE_NAME, log_msg);
            exit(EXIT_FAILURE);
        }
    }    

    /* ECU */
    char *socketFWC = malloc(strlen(PATH_SOCKET)+strlen(FWC_SOCKET)+strlen(EXT_SOCKET)+1);
    buildFWCSocketName(socketFWC);

    char *socketSBW = malloc(strlen(PATH_SOCKET)+strlen(SBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildSBWSocketName(socketSBW);

    char *socketTC = malloc(strlen(PATH_SOCKET)+strlen(TC_SOCKET)+strlen(EXT_SOCKET)+1);
    buildTCSocketName(socketTC);

    char *socketBBW = malloc(strlen(PATH_SOCKET)+strlen(BBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildBBWSocketName(socketBBW);

    int fd = socketOpenReadMode(PROCESS_NAME, socketFWC);
    while (1) {
        char command[FWC_MSG_LEN] = "";
        socketReadData(PROCESS_NAME, fd, socketFWC, command);

        // Parser commando
        if (strlen(command) == 0) {
            continue;
        }
        char *posizione = strstr(command, "\n");
        if (posizione != NULL) {
            int indice = posizione - command;
            strcpy(prevCommand, command);
            getSubStr(prevCommand, command, 0, indice - 1);
        }

        printf("Data: %s - ", command);
        addLog(ECU_LOG_FILE_NAME, command);
        if (strcmp(FWC_COMMAND_LEFT, command) == 0) {
            sendDataToSBWComponent(socketSBW, ECU_COMMAND_LEFT);
            printf("\n");
        } else if (strcmp(FWC_COMMAND_RIGHT, command) == 0) {
            sendDataToSBWComponent(socketSBW, ECU_COMMAND_RIGHT);
            printf("\n");
        } else if (strcmp(FWC_COMMAND_PARKING, command) == 0) {
            printf("Parking\n");
            parking(socketBBW, &speed);
        } else if (strcmp(FWC_COMMAND_DANGER, command) == 0) {
            printf("!!! DANGER !!!\n");
        } else {
            printf("Speed: %d \n", speed);
            int temp = atoi(command);
            if (temp > speed) {
                for (; speed < temp; speed += 5) {
                    sendDataToTCComponent(socketTC);
                    printf("%s\n", ECU_COMMAND_THROTTLE);
                }
            } else if (speed > temp) {
                for (; speed > temp; speed -= 5) {
                    sendDataToBBWComponent(socketBBW);
                    printf("%s\n", ECU_COMMAND_BRAKE);
                }
            }
            
        }
        if (strlen(command) > 0) {
            strcpy(prevCommand, command);
        }
        sleep(1);
    }
    
    socketClose(PROCESS_NAME, fd, socketFWC);
    free(socketBBW);
    free(socketTC);
    free(socketSBW);
    free(socketFWC);
}

/*
 Provare gestire la creazione di tutti i fifo qui 
*/

void initSockets(void) 
{
    // Front windshield camera socket
    printf("-- 1 --\n");
    char *socketFWC = malloc(strlen(PATH_SOCKET)+strlen(FWC_SOCKET)+strlen(EXT_SOCKET)+1);
    buildFWCSocketName(socketFWC);
    mkfifo(socketFWC, 0666);
    free(socketFWC);

    // Steer by wire socket
    printf("-- 2 --\n");
    char *socketSBW = malloc(strlen(PATH_SOCKET)+strlen(SBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildSBWSocketName(socketSBW);
    mkfifo(socketSBW, 0666);
    free(socketSBW);

    // Throttle control
    printf("-- 3 --\n");
    char *socketTC = malloc(strlen(PATH_SOCKET)+strlen(TC_SOCKET)+strlen(EXT_SOCKET)+1);
    buildTCSocketName(socketTC);
    mkfifo(socketTC, 0666);
    free(socketTC);

    // Brake by wire
    printf("-- 4 --\n");
    char *socketBBW = malloc(strlen(PATH_SOCKET)+strlen(BBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildBBWSocketName(socketBBW);
    mkfifo(socketBBW, 0666);
    free(socketBBW); 
}

void sendDataToSBWComponent(char *socketName, char *command) 
{
    int fd = socketOpenWriteMode(PROCESS_NAME, socketName);
    socketWriteData(PROCESS_NAME, fd, socketName, command);
}

void sendDataToTCComponent(char *socketName) 
{
    int fd = socketOpenWriteMode(PROCESS_NAME, socketName);
    socketWriteData(PROCESS_NAME, fd, socketName, ECU_COMMAND_THROTTLE);
}

void sendDataToBBWComponent(char *socketName)
{
    int fd = socketOpenWriteMode(PROCESS_NAME, socketName);
    socketWriteData(PROCESS_NAME, fd, socketName, ECU_COMMAND_BRAKE);
}

void parking(char *socketName, int *speed) {
    for (; *speed > 0; *speed -= 5) {
        sendDataToBBWComponent(socketName);
        printf("%s - speed: %d\n", ECU_COMMAND_BRAKE, *speed);
        sleep(1);
    }
    // TODO: call Parking component
    printf("Wait 30s\n");
    sleep(30);   
}

