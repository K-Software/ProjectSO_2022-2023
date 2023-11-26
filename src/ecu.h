/* -------------------------------------------------------------------------- */
/* ecu.h                                                                      */
/* ---------------------------------------------------------------------------*/
void ecuStart(void);
void initSockets(void);
void sendDataToComponent(char *socketName, char *command);
void parking(int *speed, pid_t pids[4]);
void handlerStart(int signum);
void handlerStop(int signum);