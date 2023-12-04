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
#include <sys/un.h>
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
#define DATA_PARKING "Dato dal PARK ASSIST: %s"
#define ERR_PARKING "Errore nell'esecuzione PARCHEGGIO"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

int speed = 0;
int start = 0;
pid_t pids[5];
char mode[20] = "";

int main(int argc, char *argv[])
{
    strcpy(mode, argv[1]);

    signal(SIGUSR1, handlerStart);

    signal(SIGUSR2, handlerParking);

    signal(SIGALRM, handlerStop);

    ecuStart();
}

/*
 * DESCRIPTION
 * This
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void ecuStart()
{
    char prevCommand[FWC_MSG_LEN] = "";
    char log_msg[MAX_ROW_LEN_LOG];
    sprintf(log_msg, "Mode: %s", mode);
    addLog(ECU_DEBUG_FILE_NAME, log_msg);

    initSockets();

    const char *paths[] = {"/repo_elaborato/bin/front_windshield_camera.out", 
        "/repo_elaborato/bin/steer_by_wire.out",
        "/repo_elaborato/bin/throttle_control.out",
        "/repo_elaborato/bin/brake_by_wire.out",
        "//repo_elaborato/bin/hmi_input.out"};
    const char *components[] = {"front_windshield_camera.out", "steer_by_wire.out",
        "throttle_control.out", "brake_by_wire.out", "hmi_input.out"};

    for (int i = 0; i < 5; ++i) {
        char log_msg[MAX_ROW_LEN_LOG];
        pid_t childPid = fork();

        if (childPid == -1) {
            sprintf(log_msg, ERR_FORK, components[i]);
            addLog(ECU_DEBUG_FILE_NAME, log_msg);
        } else if (childPid == 0) {
            // Codice eseguito dal processo figlio

            // Esegui un programma esterno
            execlp(paths[i], components[i], (char *)NULL);

            // Se execlp restituisce, c'è stato un errore
            sprintf(log_msg, ERR_EXECLP, components[i]);
            addLog(ECU_DEBUG_FILE_NAME, log_msg);
        }
        pids[i] = childPid;
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

    char *socketHMIOutput = malloc(strlen(PATH_SOCKET)+strlen(HMI_OUTPUT_SOCKET)+strlen(EXT_SOCKET)+1);
    buildHMIOutputSocketName(socketHMIOutput);

    while (start == 0) {
        addLog(ECU_DEBUG_FILE_NAME, "OFF");
        sleep(1);
    }

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

        if (strcmp(FWC_COMMAND_LEFT, command) == 0) {
            addLog(ECU_LOG_FILE_NAME, ECU_COMMAND_LEFT);
            sendDataToComponent(socketSBW, ECU_COMMAND_LEFT);
            sendDataToComponent(socketHMIOutput, ECU_COMMAND_LEFT);
        } else if (strcmp(FWC_COMMAND_RIGHT, command) == 0) {
            addLog(ECU_LOG_FILE_NAME, ECU_COMMAND_RIGHT);
            sendDataToComponent(socketSBW, ECU_COMMAND_RIGHT);
            sendDataToComponent(socketHMIOutput, ECU_COMMAND_RIGHT);
        } else if (strcmp(FWC_COMMAND_PARKING, command) == 0) {
            addLog(ECU_LOG_FILE_NAME, ECU_PARKING);
            sendDataToComponent(socketHMIOutput, ECU_PARKING);
            parking(&speed, pids);
        } else if (strcmp(FWC_COMMAND_DANGER, command) == 0) {
            kill(pids[3], SIGALRM);
            addLog(ECU_LOG_FILE_NAME, ECU_STOP);
            sendDataToComponent(socketHMIOutput, ECU_STOP);
            speed = 0;
        } else {
            int temp = atoi(command);
            if (temp > speed) {
                for (; speed < temp; speed += 5) {
                    addLog(ECU_LOG_FILE_NAME, ECU_COMMAND_THROTTLE);
                    sendDataToComponent(socketHMIOutput, ECU_COMMAND_THROTTLE);
                    sendDataToComponent(socketTC, ECU_COMMAND_THROTTLE);
                }
            } else if (speed > temp) {
                for (; speed > temp; speed -= 5) {
                    addLog(ECU_LOG_FILE_NAME, ECU_COMMAND_BRAKE);
                    sendDataToComponent(socketBBW, ECU_COMMAND_BRAKE);
                    sendDataToComponent(socketHMIOutput, ECU_COMMAND_BRAKE);
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
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void initSockets(void) 
{
    // ECO socket
    char *socketECU = malloc(strlen(PATH_SOCKET)+strlen(ECU_SOCKET)+strlen(EXT_SOCKET)+1);
    buildECUSocketName(socketECU);
    mkfifo(socketECU, 0666);
    free(socketECU);

    // Front windshield camera socket
    char *socketFWC = malloc(strlen(PATH_SOCKET)+strlen(FWC_SOCKET)+strlen(EXT_SOCKET)+1);
    buildFWCSocketName(socketFWC);
    mkfifo(socketFWC, 0666);
    free(socketFWC);

    // Steer by wire socket
    char *socketSBW = malloc(strlen(PATH_SOCKET)+strlen(SBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildSBWSocketName(socketSBW);
    mkfifo(socketSBW, 0666);
    free(socketSBW);

    // Throttle control socket
    char *socketTC = malloc(strlen(PATH_SOCKET)+strlen(TC_SOCKET)+strlen(EXT_SOCKET)+1);
    buildTCSocketName(socketTC);
    mkfifo(socketTC, 0666);
    free(socketTC);

    // Brake by wire socket
    char *socketBBW = malloc(strlen(PATH_SOCKET)+strlen(BBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildBBWSocketName(socketBBW);
    mkfifo(socketBBW, 0666);
    free(socketBBW);
}

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void sendDataToComponent(char *socketName, char *command)
{
    int fd = socketOpenWriteMode(PROCESS_NAME, socketName);
    socketWriteData(PROCESS_NAME, fd, socketName, command);
}

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void parking(int *speed, pid_t pids[4]) {
    char log_msg[MAX_ROW_LEN_LOG];
    const char *parkingCodeError[] = {"0x172A", "0xD693", "0x0000", "0xBDD8", "0xFAEE", "0x4300"}; 

    char *socketBBW = malloc(strlen(PATH_SOCKET)+strlen(BBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildBBWSocketName(socketBBW);

    char *socketECU = malloc(strlen(PATH_SOCKET)+strlen(ECU_SOCKET)+strlen(EXT_SOCKET)+1);
    buildECUSocketName(socketECU);

    // Azzero la velocita`
    for (; *speed > 0; *speed -= 5) {
        sendDataToComponent(socketBBW, ECU_COMMAND_BRAKE);
        sleep(1);
    }

    int repeatParking = 1;
    while (repeatParking) {
        repeatParking = 0;
        pid_t parkAssistPid = fork();

        if (parkAssistPid == -1) {
            sprintf(log_msg, ERR_FORK, "park_assist.out");
            addLog(ECU_DEBUG_FILE_NAME, log_msg);
        }  else if (parkAssistPid == 0) {
            // Codice eseguito dal processo figlio

            // Esegui un programma esterno
            execlp("/repo_elaborato/bin/park_assist.out", "park_assist.out", mode, (char *)NULL);

            // Se execlp restituisce, c'è stato un errore
            sprintf(log_msg, ERR_EXECLP, "park_assist.out");
            addLog(ECU_DEBUG_FILE_NAME, log_msg);
            sleep(1);
        }

        int fd = socketOpenReadMode(PROCESS_NAME, socketECU);
        for (int i = 0; i < 30; i++) {
            char data[PA_MSG_LEN] = "";
            socketReadData(PROCESS_NAME, fd, socketECU, data);
            sprintf(log_msg, DATA_PARKING, data);
            addLog(ECU_DEBUG_FILE_NAME, log_msg);
            if (isStringInArray(data, parkingCodeError, 6)) {
                addLog(ECU_DEBUG_FILE_NAME, ERR_PARKING);
                repeatParking = 1;
            }
            sleep(1);
        }
        socketClose(PROCESS_NAME, fd, socketECU);
        kill(parkAssistPid, SIGTERM);
    }
    for (int i = 0; i < 4; i++) {
        kill(pids[i], SIGTERM);
    }
    exit(EXIT_SUCCESS);
}

/*
 * DESCRIPTION
 * 
 *
 * PARAMETERS
 * 
 */
void handlerStart(int signum) 
{
    char log_msg[MAX_ROW_LEN_LOG];
    sprintf(log_msg, "HMI INPUT: %s", HMI_INPUT_COMMAND_START);
    addLog(ECU_DEBUG_FILE_NAME, log_msg);
    start = 1;
}

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 */
void handlerParking(int signum) 
{
    char log_msg[MAX_ROW_LEN_LOG];
    char *socketHMIOutput = malloc(strlen(PATH_SOCKET)+strlen(HMI_OUTPUT_SOCKET)+strlen(EXT_SOCKET)+1);
    buildHMIOutputSocketName(socketHMIOutput);

    sprintf(log_msg, "HMI INPUT: %s", HMI_INPUT_COMMAND_PARKING);
    addLog(ECU_DEBUG_FILE_NAME, log_msg);
    addLog(ECU_LOG_FILE_NAME, ECU_PARKING);
    sendDataToComponent(socketHMIOutput, ECU_PARKING);
    parking(&speed, pids);

    free(socketHMIOutput);
}

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 */
void handlerStop(int signum) 
{
    char log_msg[MAX_ROW_LEN_LOG];
    char *socketHMIOutput = malloc(strlen(PATH_SOCKET)+strlen(HMI_OUTPUT_SOCKET)+strlen(EXT_SOCKET)+1);
    buildHMIOutputSocketName(socketHMIOutput);

    sprintf(log_msg, "HMI INPUT: %s", HMI_INPUT_COMMAND_STOP);
    addLog(ECU_DEBUG_FILE_NAME, log_msg);
    kill(pids[3], SIGALRM);
    addLog(ECU_LOG_FILE_NAME, ECU_STOP);
    sendDataToComponent(socketHMIOutput, ECU_STOP);
    speed = 0;

    free(socketHMIOutput);
}

