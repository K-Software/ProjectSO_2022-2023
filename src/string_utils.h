/* -------------------------------------------------------------------------- */
/* string_utils.c                                                             */
/* -------------------------------------------------------------------------- */
void buildFullPipeName(char *fullPipeName, char *pipeName);
void buildHMIOutputPipeName(char *fullPipeName);
void buildECUPipeName(char *fullPipeName);
void buildFWCPipeName(char *fullPipeName);
void buildSBWPipeName(char *fullPipeName);
void buildTCPipeName(char *fullPipeName);
void buildBBWPipeName(char *fullPipeName);
int getSubStr(char *source, char *target,int from, int to);
int isStringInArray(const char *target, const char *array[], int size);