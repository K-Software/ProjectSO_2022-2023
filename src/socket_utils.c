/* -------------------------------------------------------------------------- */
/* socket_utils.h                                                             */
/* -------------------------------------------------------------------------- */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "common.h"
#include "log.h"
#include "socket_utils.h"

/* -------------------------------------------------------------------------- */
/* Macro                                                                      */
/* -------------------------------------------------------------------------- */
#define OPEN_W_OK_MSG "%s - Open socket %s in write mode"
#define OPEN_W_ERR_MSG "%s - Error during opening in write mode of socket %s (errno=%d)"
#define OPEN_R_OK_MSG "%s - Open socket %s in read mode"
#define OPEN_R_ERR_MSG "%s - Error during opening in read mode of socket %s (errno=%d)"
#define WRITE_OK_MSG "%s - Writed in socket %s the data %s"
#define WRITE_ERR_MSG "%s - Error writing socket %s (errno=%d)"
#define READ_OK_MSG "%s - Read from socket %s the data %s"
#define READ_ERR_MSG "%s - Error reading socket %s (errno=%d)"
#define CLOSE_OK_MSG "%s - Close socket %s"
#define CLOSE_ERR_MSG "%s - Error during closing of socket %s (errno=%d)"

/* -------------------------------------------------------------------------- */
/* Functions                                                                  */
/* -------------------------------------------------------------------------- */

#ifdef DEBUG_SOCKET_UTILS
void main(void) {

    char *socketName = malloc(strlen(PATH_SOCKET)+strlen("TEST")+strlen(EXT_SOCKET)+1);
    buildFullSocketName(socketName, "TEST");
    mkfifo(socketName, 0666);
    int fd = socketOpenWriteMode(socketName);
    if (fd == -1) {
        printf(OPEN_R_ERR_MSG, socketName);
    } else {
        int res = socketWriteData(fd, socketName, "Test message");
        switch (res) {
        case 1:
            printf(WRITE_ERR_MSG, socketName);
            break;
        default:
            printf(WRITE_OK_MSG, socketName);
        }
    }
}
#endif

/*
 *
 */
int socketOpenWriteMode(char *processName, char *socketName) 
{
    int fd;
    char log_msg[MAX_ROW_LEN_LOG];

    fd = open(socketName, O_WRONLY | O_NONBLOCK);

    if (fd == -1) {
        sprintf(log_msg, OPEN_W_ERR_MSG, processName, socketName, errno);
        addLog(SOCKET_UTILS_LOG, log_msg);
        return -1;
    }
    sprintf(log_msg, OPEN_W_OK_MSG, processName, socketName);
    addLog(SOCKET_UTILS_LOG, log_msg);
    return fd;
}

/*
 *
 */
int socketOpenReadMode(char *processName, char *socketName)
{
    int fd;
    char log_msg[MAX_ROW_LEN_LOG];

    fd = open(socketName, O_RDONLY | O_NONBLOCK);

    if (fd == -1) {
        sprintf(log_msg, OPEN_R_ERR_MSG, processName, socketName, errno);
        addLog(SOCKET_UTILS_LOG, log_msg);
        return -1;
    }
    sprintf(log_msg, OPEN_R_OK_MSG, processName, socketName);
    addLog(SOCKET_UTILS_LOG, log_msg);
    return fd;
}

/*
 *
 */
int socketWriteData(char *processName, int fd, char *socketName, char *data) 
{
    char log_msg[MAX_ROW_LEN_LOG];

    ssize_t writedBytes = write(fd, data, strlen(data));
    
    if (writedBytes == -1) {
        sprintf(log_msg, WRITE_ERR_MSG, processName, socketName, errno);
        addLog(SOCKET_UTILS_LOG, log_msg);
        return 1;
    } 
    sprintf(log_msg, WRITE_OK_MSG, processName, socketName, data);
    addLog(SOCKET_UTILS_LOG, log_msg);
    return 0;
}

/*
 *
 */
int socketReadData(char *processName, int fd, char *socketName, char *data)
{
    char log_msg[MAX_ROW_LEN_LOG];
    ssize_t readBytes;
    size_t totalBytesRead = 0;
    do {
        readBytes = read(fd, data + totalBytesRead, 1);

        if (readBytes == -1) {
            sprintf(log_msg, READ_ERR_MSG, processName, socketName, errno);
            return 1;
        } 

        totalBytesRead += readBytes;

    } while (readBytes > 0 && data[totalBytesRead - 1] != '\0');
    data[totalBytesRead - 1] = '\0';
    sprintf(log_msg, READ_OK_MSG, processName, socketName, data);
    addLog(SOCKET_UTILS_LOG, log_msg);    
    return 0;
}

/*
 *
 */
int socketClose(char *processName, int fd, char *socketName)
{
    char log_msg[MAX_ROW_LEN_LOG];
    int res = close(fd);
    
    if (res == -1) {
        sprintf(log_msg, CLOSE_ERR_MSG, processName, socketName, errno);
        addLog(SOCKET_UTILS_LOG, log_msg);
        return 1;
    }
    sprintf(log_msg, CLOSE_OK_MSG, processName, socketName);
    addLog(SOCKET_UTILS_LOG, log_msg);
    return 0;
}