/* -------------------------------------------------------------------------- */
/* ecu.h                                                                      */
/* ---------------------------------------------------------------------------*/
void ecuStart(void);
void initSockets(void);
void sendDataToSBWComponent(char *socketName, char *command);
void sendDataToTCComponent(char *socketName);