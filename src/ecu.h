/* -------------------------------------------------------------------------- */
/* ecu.h                                                                      */
/* ---------------------------------------------------------------------------*/
void ecuStart(void);
void initSockets(void);
void sendDataToSBWComponent(char *socketName, char *command);
void sendDataToTCComponent(char *socketName);
void sendDataToBBWComponent(char *socketName);
void parking(char *socketName, int *speed);