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
 * DESCRIPTION:
 * This function builds the full name of pipe
 *
 * PARAMETERS:
 * - fullPipeName = full name of pipe
 * - pipeName = name of pipe 
 */
void buildFullPipeName(char *fullPipeName, char *pipeName) {
    strcpy(fullPipeName, PATH_PIPE);
    strcat(fullPipeName, pipeName);
    strcat(fullPipeName, EXT_PIPE);
}

/*
 * DESCRIPTION:
 * This function builds the full name of pipe for HMI output process.
 *
 * PARAMETERS:
 * - fullPipeName = full name of pipe
 */
void buildHMIOutputPipeName(char *fullPipeName) {
    buildFullPipeName(fullPipeName, HMI_OUTPUT_PIPE);
}

/*
 * DESCRIPTION:
 * This function builds the full name of pipe for ECU process.
 *
 * PARAMETERS:
 * - fullPipeName = full name of pipe
 */
void buildECUPipeName(char *fullPipeName) {
    buildFullPipeName(fullPipeName, ECU_PIPE);
}

/*
 * DESCRIPTION:
 * This function builds the full name of pipe for front windshield camera 
 * process.
 *
 * PARAMETERS:
 * - fullPipeName = full name of pipe
 */
void buildFWCPipeName(char *fullPipeName) {
    buildFullPipeName(fullPipeName, FWC_PIPE);
}

/*
 * DESCRIPTION:
 * This function builds the full name of pipe for steer by wire process.
 *
 * PARAMETERS:
 * - fullPipeName = full name of pipe
 */
void buildSBWPipeName(char *fullPipeName) {
    buildFullPipeName(fullPipeName, SBW_PIPE);
}

/*
 * DESCRIPTION:
 * This function builds the full name of pipe for throttle control process.
 *
 * PARAMETERS:
 * - fullPipeName = full name of pipe
 */
void buildTCPipeName(char *fullPipeName) {
    buildFullPipeName(fullPipeName, TC_PIPE);
}

/*
 * DESCRIPTION:
 * This function builds the full name of pipe for brake by wire process.
 *
 * PARAMETERS:
 * - fullPipeName = full name of pipe
 */
void buildBBWPipeName(char *fullPipeName) {
    buildFullPipeName(fullPipeName, BBW_PIPE);
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

/*
 * DESCRIPTION
 * This functions checks if the string contains at least one strings. 
 *
 * PARAMETERS
 * - string = string 
 * - subStrings = list of sub strings
 *
 * RETURN VALUES
 * - 0 = the string not contains substrings
 * - 1 = the string contains at least one substring
 */
int isStringInArray(const char *string, const char *subStrings[], int size)
{
  for (int i = 0; i < size; ++i) {
    char *position = strstr(string, subStrings[i]);
    if (position != NULL) {
      return 1;
    }
  }
  return 0;
}