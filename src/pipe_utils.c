/* -------------------------------------------------------------------------- */
/* pipe_utils.h                                                               */
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
#include "pipe_utils.h"

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

/*
 * DESCRIPTION:
 * This function open a new pipe O_WRONLY and O_NONBLOCK mode.
 * 
 * PARAMETERS:
 * - processName = name of process that open the new pipe 
 * - pipeName = name of the new pipe
 * 
 * RETURN VALUES:
 * Return the file descriptor of the new pipe
 */
int pipeOpenWriteMode(char *processName, char *pipeName) 
{
    int fd;
    char log_msg[MAX_ROW_LEN_LOG];

    fd = open(pipeName, O_WRONLY | O_NONBLOCK);

    if (fd == -1) {
        sprintf(log_msg, OPEN_W_ERR_MSG, processName, pipeName, errno);
        pipeAddLog(log_msg);
        return -1;
    }
    sprintf(log_msg, OPEN_W_OK_MSG, processName, pipeName);
    pipeAddLog(log_msg);
    return fd;
}

/*
 * DESCRIPTION:
 * This function open a new pipe O_RDONLY and O_NONBLOCK mode.
 *
 * PARAMETERS:
 * - processName = name of process that open the new pipe 
 * - pipeName = name of the new pipe 
 * 
 * RETURN VALUES:
 * Return the file descriptor of the new pipe
 */
int pipeOpenReadMode(char *processName, char *pipeName)
{
    int fd;
    char log_msg[MAX_ROW_LEN_LOG];

    fd = open(pipeName, O_RDONLY | O_NONBLOCK);

    if (fd == -1) {
        sprintf(log_msg, OPEN_R_ERR_MSG, processName, pipeName, errno);
        pipeAddLog(log_msg);
        return -1;
    }
    sprintf(log_msg, OPEN_R_OK_MSG, processName, pipeName);
    pipeAddLog(log_msg);
    return fd;
}

/*
 * DESCRIPTION:
 * This function write data on pipe.
 *
 * PARAMETERS:
 * - processName = name of process that writes data on pipe
 * - fd = file descriptor of pipe
 * - pipeName = name of pipe
 * - data = data to write on pipe
 * 
 * RETURN VALUES
 * - 0 = All right
 * - 1 = Error during the write data on socket
 */
int pipeWriteData(char *processName, int fd, char *pipeName, char *data) 
{
    char log_msg[MAX_ROW_LEN_LOG];

    ssize_t writedBytes = write(fd, data, strlen(data));
    
    if (writedBytes == -1) {
        sprintf(log_msg, WRITE_ERR_MSG, processName, pipeName, errno);
        pipeAddLog(log_msg);
        return 1;
    } 
    sprintf(log_msg, WRITE_OK_MSG, processName, pipeName, data);
    pipeAddLog(log_msg);
    return 0;
}

/*
 * DESCRIPTION:
 * This function read data from pipe.
 *
 * PARAMETERS:
 * - processName = name of process that reads data from pipe
 * - fd = file descriptor of pipe
 * - pipeName = name of pipe
 * - data = data read from pipe
 * 
 * RETURN VALUES:
 * - 0 = All right
 * - 1 = Error during the read data from pipe 
 */
int pipeReadData(char *processName, int fd, char *pipeName, char *data)
{
    char log_msg[MAX_ROW_LEN_LOG];
    ssize_t readBytes;
    size_t totalBytesRead = 0;
    do {
        readBytes = read(fd, data + totalBytesRead, 1);

        if (readBytes == -1) {
            sprintf(log_msg, READ_ERR_MSG, processName, pipeName, errno);
            return 1;
        } 

        totalBytesRead += readBytes;

    } while (readBytes > 0 && data[totalBytesRead - 1] != '\0');
    data[totalBytesRead - 1] = '\0';
    sprintf(log_msg, READ_OK_MSG, processName, pipeName, data);
    pipeAddLog(log_msg);    
    return 0;
}

/*
 * DESCRIPTION:
 * This function close the pipe.
 *
 * PARAMETERS:
 * - processName = name of process that reads data from pipe
 * - fd = file descriptor of pipe
 * - pipeName = name of pipe
 * 
 * RETURN VALUES:
 * - 0 = All right
 * - 1 = Error during the closing socket
 */
int pipeClose(char *processName, int fd, char *pipeName)
{
    char log_msg[MAX_ROW_LEN_LOG];
    int res = close(fd);
    
    if (res == -1) {
        sprintf(log_msg, CLOSE_ERR_MSG, processName, pipeName, errno);
        pipeAddLog(log_msg);
        return 1;
    }
    sprintf(log_msg, CLOSE_OK_MSG, processName, pipeName);
    pipeAddLog(log_msg);
    return 0;
}

void pipeAddLog(char *msg)
{
    #ifdef DEBUG_PIPE_UTILS
    addLog(PIPE_UTILS_LOG, msg);
    #endif
}