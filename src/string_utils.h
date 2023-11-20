/* -------------------------------------------------------------------------- */
/* string_utils.c                                                             */
/* -------------------------------------------------------------------------- */
void buildFullSocketName(char *fullSocketName, char *socketName);
void buildHMIOutputSocketName(char *fullSocketName);
void buildECUSocketName(char *fullSocketName);
void buildFWCSocketName(char *fullSocketName);
void buildSBWSocketName(char *fullSocketName);
void buildTCSocketName(char *fullSocketName);
void buildBBWSocketName(char *fullSocketName);
int getSubStr(char *source, char *target,int from, int to);
int isStringInArray(const char *target, const char *array[], int size);