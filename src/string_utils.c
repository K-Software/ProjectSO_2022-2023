/* -------------------------------------------------------------------------- */
/* string_utils.c                                                             */
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "string_utils.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

#ifdef DEBUG
void main(void) {
    char *socketName = malloc(strlen(PATH_SOCKET)+strlen(HMI_OUTPUT_SOCKET)+strlen(EXT_SOCKET)+1);
    buildHMIOutputSocketName(socketName);
    printf("Full path name: %s\n", socketName);
    free(socketName);

    socketName = malloc(strlen(PATH_SOCKET)+strlen(FWC_SOCKET)+strlen(EXT_SOCKET)+1);
    buildFWCSocketName(socketName);
    printf("Full path name: %s\n", socketName);
    free(socketName);

    socketName = malloc(strlen(PATH_SOCKET)+strlen(SBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildSBWSocketName(socketName);
    printf("Full path name: %s\n", socketName);
    free(socketName);

    socketName = malloc(strlen(PATH_SOCKET)+strlen(TC_SOCKET)+strlen(EXT_SOCKET)+1);
    buildTCSocketName(socketName);
    printf("Full path name: %s\n", socketName);
    free(socketName);

    socketName = malloc(strlen(PATH_SOCKET)+strlen(BBW_SOCKET)+strlen(EXT_SOCKET)+1);
    buildBBWSocketName(socketName);
    printf("Full path name: %s\n", socketName);
    free(socketName);
}
#endif

void buildFullSocketName(char *fullSocketName, char *socketName) {
    strcpy(fullSocketName, PATH_SOCKET);
    strcat(fullSocketName, socketName);
    strcat(fullSocketName, EXT_SOCKET);
}

void buildHMIOutputSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, HMI_OUTPUT_SOCKET);
}

void buildFWCSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, FWC_SOCKET);
}

void buildSBWSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, SBW_SOCKET);
}

void buildTCSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, TC_SOCKET);
}

void buildBBWSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, BBW_SOCKET);
}