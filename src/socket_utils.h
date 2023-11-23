/* -------------------------------------------------------------------------- */
/* socket_utils.h                                                             */
/* -------------------------------------------------------------------------- */
int socketOpenWriteMode(char *processName, char *socketName);
int socketOpenReadMode(char *processName, char *socketName);
int socketWriteData(char *processName, int fd, char *socketName, char *data);
int socketReadData(char *processName, int fd, char *socketName, char *data);
int socketClose(char *processName, int fd, char *socketName);
void socketAddLog(char *msg);