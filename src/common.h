/* -------------------------------------------------------------------------- */
/* common.h                                                                   */
/* -------------------------------------------------------------------------- */
#define DEBUG

/* -------------------------------------------------------------------------- */
/* Macro - LOG                                                                */
/* -------------------------------------------------------------------------- */
#define MAX_ROW_LEN_LOG 300                          /* Max length of log row */
#define PREFIX_LEN_LOG 25                            /* Length of log-prefix  */

/* -------------------------------------------------------------------------- */
/* Macro - HMI                                                                */
/* -------------------------------------------------------------------------- */
#define HMI_INPUT_SOCKET "hmi_input"
#define HMI_INPUT_MSG_LEN 10
#define HMI_INPUT_COMMAND_START "INIZIO"
#define HMI_INPUT_COMMAND_PARKING "PARCHEGGIO"
#define HMI_INPUT_COMMAND_STOP "ARRESTO"
#define HMI_OUTPUT_SOCKET "hmi_output"
#define HMI_OUTPUT_MSG_LEM 10