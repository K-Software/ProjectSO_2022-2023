/* -------------------------------------------------------------------------- */
/* socket_utils.h                                                             */
/* -------------------------------------------------------------------------- */
int socketOpenWriteMode(char *socketName);
int socketOpenReadMode(char *socketName);
int socketWriteData(int fd, char *socketName, char *data);
int socketReadData(int fd, char *socketName, char *data);
int socketClose(int fd, char *socketName);