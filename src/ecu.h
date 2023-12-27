/* -------------------------------------------------------------------------- */
/* ecu.h                                                                      */
/* ---------------------------------------------------------------------------*/
void ecuStart(void);
void initPipes(void);
void sendDataToComponent(char *socketName, char *command);
void parking(int *speed, pid_t pids[4]);
void handlerStart(int signum);
void handlerParking(int signum);
void handlerStop(int signum);