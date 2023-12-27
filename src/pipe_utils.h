/* -------------------------------------------------------------------------- */
/* pipe_utils.h                                                               */
/* -------------------------------------------------------------------------- */
int pipeOpenWriteMode(char *processName, char *pipeName);
int pipeOpenReadMode(char *processName, char *pipeName);
int pipeWriteData(char *processName, int fd, char *pipeName, char *data);
int pipeReadData(char *processName, int fd, char *pipeName, char *data);
int pipeClose(char *processName, int fd, char *pipeName);
void pipeAddLog(char *msg);