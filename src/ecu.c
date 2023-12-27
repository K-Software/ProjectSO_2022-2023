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
#include "pipe_utils.h"
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
 * DESCRIPTION:
 * This function realizies the Central ECU component.
 */
void ecuStart(void)
{
    char prevCommand[FWC_MSG_LEN] = "";
    char log_msg[MAX_ROW_LEN_LOG];
    sprintf(log_msg, "Mode: %s", mode);
    addLog(ECU_DEBUG_FILE_NAME, log_msg);

    initPipes();

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
    char *pipeFWC = malloc(strlen(PATH_PIPE)+strlen(FWC_PIPE)+strlen(EXT_PIPE)+1);
    buildFWCPipeName(pipeFWC);

    char *pipeSBW = malloc(strlen(PATH_PIPE)+strlen(SBW_PIPE)+strlen(EXT_PIPE)+1);
    buildSBWPipeName(pipeSBW);

    char *pipeTC = malloc(strlen(PATH_PIPE)+strlen(TC_PIPE)+strlen(EXT_PIPE)+1);
    buildTCPipeName(pipeTC);

    char *pipeBBW = malloc(strlen(PATH_PIPE)+strlen(BBW_PIPE)+strlen(EXT_PIPE)+1);
    buildBBWPipeName(pipeBBW);

    char *pipeHMIOutput = malloc(strlen(PATH_PIPE)+strlen(HMI_OUTPUT_PIPE)+strlen(EXT_PIPE)+1);
    buildHMIOutputPipeName(pipeHMIOutput);

    while (start == 0) {
        addLog(ECU_DEBUG_FILE_NAME, "OFF");
        sleep(1);
    }

    int fd = pipeOpenReadMode(PROCESS_NAME, pipeFWC);
    while (1) {
        char command[FWC_MSG_LEN] = "";
        pipeReadData(PROCESS_NAME, fd, pipeFWC, command);

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
            sendDataToComponent(pipeSBW, ECU_COMMAND_LEFT);
            sendDataToComponent(pipeHMIOutput, ECU_COMMAND_LEFT);
        } else if (strcmp(FWC_COMMAND_RIGHT, command) == 0) {
            addLog(ECU_LOG_FILE_NAME, ECU_COMMAND_RIGHT);
            sendDataToComponent(pipeSBW, ECU_COMMAND_RIGHT);
            sendDataToComponent(pipeHMIOutput, ECU_COMMAND_RIGHT);
        } else if (strcmp(FWC_COMMAND_PARKING, command) == 0) {
            addLog(ECU_LOG_FILE_NAME, ECU_PARKING);
            sendDataToComponent(pipeHMIOutput, ECU_PARKING);
            parking(&speed, pids);
        } else if (strcmp(FWC_COMMAND_DANGER, command) == 0) {
            kill(pids[3], SIGALRM);
            addLog(ECU_LOG_FILE_NAME, ECU_STOP);
            sendDataToComponent(pipeHMIOutput, ECU_STOP);
            speed = 0;
        } else {
            int temp = atoi(command);
            if (temp > speed) {
                for (; speed < temp; speed += 5) {
                    addLog(ECU_LOG_FILE_NAME, ECU_COMMAND_THROTTLE);
                    sendDataToComponent(pipeHMIOutput, ECU_COMMAND_THROTTLE);
                    sendDataToComponent(pipeTC, ECU_COMMAND_THROTTLE);
                }
            } else if (speed > temp) {
                for (; speed > temp; speed -= 5) {
                    addLog(ECU_LOG_FILE_NAME, ECU_COMMAND_BRAKE);
                    sendDataToComponent(pipeBBW, ECU_COMMAND_BRAKE);
                    sendDataToComponent(pipeHMIOutput, ECU_COMMAND_BRAKE);
                }
            }
        }
        if (strlen(command) > 0) {
            strcpy(prevCommand, command);
        }
        sleep(1);
    }
    
    pipeClose(PROCESS_NAME, fd, pipeFWC);
    free(pipeBBW);
    free(pipeTC);
    free(pipeSBW);
    free(pipeFWC);
}

/*
 * DESCRIPTION:
 * This function creates all the pipes necessary for the "Central ECU" 
 * component.
 */
void initPipes(void) 
{
    // ECO pipe
    char *pipeECU = malloc(strlen(PATH_PIPE)+strlen(ECU_PIPE)+strlen(EXT_PIPE)+1);
    buildECUPipeName(pipeECU);
    mkfifo(pipeECU, 0666);
    free(pipeECU);

    // Front windshield camera pipe
    char *pipeFWC = malloc(strlen(PATH_PIPE)+strlen(FWC_PIPE)+strlen(EXT_PIPE)+1);
    buildFWCPipeName(pipeFWC);
    mkfifo(pipeFWC, 0666);
    free(pipeFWC);

    // Steer by wire pipe
    char *pipeSBW = malloc(strlen(PATH_PIPE)+strlen(SBW_PIPE)+strlen(EXT_PIPE)+1);
    buildSBWPipeName(pipeSBW);
    mkfifo(pipeSBW, 0666);
    free(pipeSBW);

    // Throttle control pipe
    char *pipeTC = malloc(strlen(PATH_PIPE)+strlen(TC_PIPE)+strlen(EXT_PIPE)+1);
    buildTCPipeName(pipeTC);
    mkfifo(pipeTC, 0666);
    free(pipeTC);

    // Brake by wire pipe
    char *pipeBBW = malloc(strlen(PATH_PIPE)+strlen(BBW_PIPE)+strlen(EXT_PIPE)+1);
    buildBBWPipeName(pipeBBW);
    mkfifo(pipeBBW, 0666);
    free(pipeBBW);
}

/*
 * DESCRIPTION:
 * This function sends data to pipe.
 *
 * PARAMETERS:
 * - pipeName = name of pipe
 * - data = data to send
 */
void sendDataToComponent(char *pipeName, char *command)
{
    int fd = pipeOpenWriteMode(PROCESS_NAME, pipeName);
    pipeWriteData(PROCESS_NAME, fd, pipeName, command);
}

/*
 * DESCRIPTION:
 * This function performs the parking action.
 *
 * PARAMETERS:
 * - speed = current speed
 * - pids = array with pids of components
 */
void parking(int *speed, pid_t pids[4]) {
    char log_msg[MAX_ROW_LEN_LOG];
    const char *parkingCodeError[] = {"172A", "D693", "0000", "BDD8", "FAEE", "4300"}; 

    char *pipeBBW = malloc(strlen(PATH_PIPE)+strlen(BBW_PIPE)+strlen(EXT_PIPE)+1);
    buildBBWPipeName(pipeBBW);

    char *pipeECU = malloc(strlen(PATH_PIPE)+strlen(ECU_PIPE)+strlen(EXT_PIPE)+1);
    buildECUPipeName(pipeECU);

    // Azzero la velocita`
    for (; *speed > 0; *speed -= 5) {
        sendDataToComponent(pipeBBW, ECU_COMMAND_BRAKE);
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

        int fd = pipeOpenReadMode(PROCESS_NAME, pipeECU);
        for (int i = 0; i < 30; i++) {
            char data[PA_MSG_LEN] = "";
            pipeReadData(PROCESS_NAME, fd, pipeECU, data);
            sprintf(log_msg, DATA_PARKING, data);
            addLog(ECU_DEBUG_FILE_NAME, log_msg);
            if (isStringInArray(data, parkingCodeError, 6)) {
                addLog(ECU_DEBUG_FILE_NAME, ERR_PARKING);
                repeatParking = 1;
            }
            sleep(1);
        }
        pipeClose(PROCESS_NAME, fd, pipeECU);
        kill(parkAssistPid, SIGTERM);
    }
    for (int i = 0; i < 4; i++) {
        kill(pids[i], SIGTERM);
    }
    exit(EXIT_SUCCESS);
}

/*
 * DESCRIPTION:
 * this function overwrite the handler for signal SIGUSR1. 
 *
 * PARAMETERS:
 * - signum = signal number
 */
void handlerStart(int signum) 
{
    char log_msg[MAX_ROW_LEN_LOG];
    sprintf(log_msg, "HMI INPUT: %s", HMI_INPUT_COMMAND_START);
    addLog(ECU_DEBUG_FILE_NAME, log_msg);
    start = 1;
}

/*
 * DESCRIPTION:
 * this function overwrite the handler for signal SIGUSR2. 
 *
 * PARAMETERS:
 * - signum = signal number
 */
void handlerParking(int signum) 
{
    char log_msg[MAX_ROW_LEN_LOG];
    char *pipeHMIOutput = malloc(strlen(PATH_PIPE)+strlen(HMI_OUTPUT_PIPE)+strlen(EXT_PIPE)+1);
    buildHMIOutputPipeName(pipeHMIOutput);

    sprintf(log_msg, "HMI INPUT: %s", HMI_INPUT_COMMAND_PARKING);
    addLog(ECU_DEBUG_FILE_NAME, log_msg);
    addLog(ECU_LOG_FILE_NAME, ECU_PARKING);
    sendDataToComponent(pipeHMIOutput, ECU_PARKING);
    parking(&speed, pids);

    free(pipeHMIOutput);
}

/*
 * DESCRIPTION:
 * this function overwrite the handler for signal SIGALRM. 
 *
 * PARAMETERS:
 * - signum = signal number
 */
void handlerStop(int signum) 
{
    char log_msg[MAX_ROW_LEN_LOG];
    char *pipeHMIOutput = malloc(strlen(PATH_PIPE)+strlen(HMI_OUTPUT_PIPE)+strlen(EXT_PIPE)+1);
    buildHMIOutputPipeName(pipeHMIOutput);

    sprintf(log_msg, "HMI INPUT: %s", HMI_INPUT_COMMAND_STOP);
    addLog(ECU_DEBUG_FILE_NAME, log_msg);
    kill(pids[3], SIGALRM);
    addLog(ECU_LOG_FILE_NAME, ECU_STOP);
    sendDataToComponent(pipeHMIOutput, ECU_STOP);
    speed = 0;

    free(pipeHMIOutput);
}

