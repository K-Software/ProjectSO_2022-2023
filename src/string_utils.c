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

#ifdef DEBUG_STRING_UTILS
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

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void buildFullSocketName(char *fullSocketName, char *socketName) {
    strcpy(fullSocketName, PATH_SOCKET);
    strcat(fullSocketName, socketName);
    strcat(fullSocketName, EXT_SOCKET);
}

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void buildHMIOutputSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, HMI_OUTPUT_SOCKET);
}

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void buildECUSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, ECU_SOCKET);
}

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void buildFWCSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, FWC_SOCKET);
}

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void buildSBWSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, SBW_SOCKET);
}

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void buildTCSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, TC_SOCKET);
}

/*
 * DESCRIPTION
 *
 * PARAMETERS
 * 
 * RETURN VALUES
 * 
 */
void buildBBWSocketName(char *fullSocketName) {
    buildFullSocketName(fullSocketName, BBW_SOCKET);
}

/*
 * DESCRIPTION
 * This function gets a substring from a suorce string.
 *
 * PARAMETERS:
 * - source =  Source string which will be read from the user and provided to
 *             the function as an argument;
 * - target = Target string, substring will be assigned in this variable;
 * - from = Start index from where we will extract the substring (it should be
 *          greater than or equal to 0 and less than string length);
 * - to = End index, the last character of the substring (it should be less than
 *        string length)
 *
 * RETURN VALUES:
 * - 0 = everything's ok
 * - 1 = Invalid 'from' index, this condition happens when from is minor
 *       than 0 or it's greater than the length of the source string;
 * - 2 = Invalid 'to' index, this condition happens when to id greater
 *       than the length of the source string;
 * - 3 = Invalid 'from' and 'to' index, this condition happens when the
 *       from index greater than to index.
 */
int getSubStr(char *source, char *target,int from, int to)
{
  int length=0;
  int i=0,j=0;

  /* get length of source string with out '\0' */
  while(source[i++]!='\0')
    length++;

  if(from<0 || from>length) {
    return 1;                                        /* Invalid 'from' index" */
  }

  if(to>length) {
    return 2;                                        /* Invalid 'to' index    */
  }

  if (from>to) {
    return 3;                                /* Invalid 'from' and 'to' index */
  }

  for(i=from, j=0; i<=to; i++, j++) {
    target[j]=source[i];
  }

  //assign NULL at the end of string
  target[j]='\0';

  return 0;
}