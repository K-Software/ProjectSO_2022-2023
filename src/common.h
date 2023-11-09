/* -------------------------------------------------------------------------- */
/* common.h                                                                   */
/* -------------------------------------------------------------------------- */
#define DEBUGXXX
#define PATH_SOCKET "./sockets/"
#define EXT_SOCKET ".sck"
#define LOG_MSG_WRONG_COMMAND "COMANDO NON PREVISTO"
#define LOG_MSG_NO_ACTION "NO ACTION"

/* -------------------------------------------------------------------------- */
/* Macro - LOG                                                                */
/* -------------------------------------------------------------------------- */
#define MAX_ROW_LEN_LOG 300                          /* Max length of log row */
#define PREFIX_LEN_LOG 25                            /* Length of log-prefix  */

/* -------------------------------------------------------------------------- */
/* Macro - HMI INPUT                                                          */
/* -------------------------------------------------------------------------- */
#define HMI_INPUT_SOCKET "hmi_input"
#define HMI_INPUT_MSG_LEN 10
#define HMI_INPUT_COMMAND_START "INIZIO"
#define HMI_INPUT_COMMAND_PARKING "PARCHEGGIO"
#define HMI_INPUT_COMMAND_STOP "ARRESTO"

/* -------------------------------------------------------------------------- */
/* Macro - HMI OUTPUT                                                         */
/* -------------------------------------------------------------------------- */
#define HMI_OUTPUT_SOCKET "hmi_output"
#define HMI_OUTPUT_MSG_LEM 10

/* -------------------------------------------------------------------------- */
/* Macro - ECU                                                                */
/* -------------------------------------------------------------------------- */
#define ECU_COMMAND_LEFT "SINISTRA"
#define ECU_COMMAND_RIGHT "DESTRA"
#define ECU_COMMAND_THROTTLE "INCREMENTO 5"
#define ECU_COMMAND_BRAKE "FRENO 5"

/* -------------------------------------------------------------------------- */
/* Macro - FRONT WISHSHIELD CAMERA                                            */
/* -------------------------------------------------------------------------- */
#define FWC_LOG_FILE_NAME "camera"

/* -------------------------------------------------------------------------- */
/* Macro - STEER BY WIRE                                                      */
/* -------------------------------------------------------------------------- */
#define SBW_LOG_FILE_NAME "steer"
#define SBW_SOCKET "steer_by_wire"
#define SBW_MSG_LEN 10
#define SBW_TURN_LEFT_LOG_MSG "STO GIRANDO A SINISTRA"
#define SBW_TURN_RIGHT_LOG_MSG "STO GIRANDO A DESTRA"

/* -------------------------------------------------------------------------- */
/* Macro - THROTTLE CONTROL                                                   */
/* -------------------------------------------------------------------------- */
#define TC_LOG_FILE_NAME "throttle"
#define TC_SOCKET "throttle_control"
#define TC_MSG_LEN 13
#define TC_LOG_MSG "AUMENTO 5"

/* -------------------------------------------------------------------------- */
/* Macro - BRAKE BY WIRE                                                      */
/* -------------------------------------------------------------------------- */
#define BBW_LOG_FILE_NAME "brake"
#define BBW_SOCKET "brake_by_wire"
#define BBW_MSG_LEN 8
#define BBW_LOG_MSG "FRENO 5"