/* -------------------------------------------------------------------------- */
/* common.h                                                                   */
/* -------------------------------------------------------------------------- */
#define DEBUG
#define PATH_SOCKET "./sockets/"
#define EXT_SOCKET ".sck"
#define LOG_MSG_WRONG_COMMAND "COMANDO NON PREVISTO: %s"
#define LOG_MSG_NO_ACTION "NO ACTION"
#define BUFFER_SIZE 25

/* -------------------------------------------------------------------------- */
/* Macro - LOG                                                                */
/* -------------------------------------------------------------------------- */
#define MAX_ROW_LEN_LOG 300                          /* Max length of log row */
#define PREFIX_LEN_LOG 25                            /* Length of log-prefix  */

/* -------------------------------------------------------------------------- */
/* Macro - SOCKET UTILS                                                       */
/* -------------------------------------------------------------------------- */
#define SOCKET_UTILS_LOG "socket_utils"

/* -------------------------------------------------------------------------- */
/* Macro - HMI INPUT                                                          */
/* -------------------------------------------------------------------------- */
#define HMI_INPUT_SOCKET "hmi_input"
#define HMI_INPUT_MSG_LEN BUFFER_SIZE
#define HMI_INPUT_COMMAND_START "INIZIO"
#define HMI_INPUT_COMMAND_PARKING "PARCHEGGIO"
#define HMI_INPUT_COMMAND_STOP "ARRESTO"

/* -------------------------------------------------------------------------- */
/* Macro - HMI OUTPUT                                                         */
/* -------------------------------------------------------------------------- */
#define HMI_OUTPUT_SOCKET "hmi_output"
#define HMI_OUTPUT_MSG_LEM BUFFER_SIZE

/* -------------------------------------------------------------------------- */
/* Macro - ECU                                                                */
/* -------------------------------------------------------------------------- */
#define ECU_LOG_FILE_NAME "ecu"
#define ECU_DEBUG_FILE_NAME "ecu_debug"
#define ECU_COMMAND_LEFT "SINISTRA"
#define ECU_COMMAND_RIGHT "DESTRA"
#define ECU_COMMAND_THROTTLE "INCREMENTO 5"
#define ECU_COMMAND_BRAKE "FRENO 5"

/* -------------------------------------------------------------------------- */
/* Macro - FRONT WISHSHIELD CAMERA                                            */
/* -------------------------------------------------------------------------- */
#define FWC_LOG_FILE_NAME "camera"
#define FWC_DEBUG_FILE_NAME "camera_debug"
#define FWC_SOCKET "front_windshield_camere"
#define FWC_MSG_LEN BUFFER_SIZE
#define FWC_COMMAND_LEFT "SINISTRA"
#define FWC_COMMAND_RIGHT "DESTRA"
#define FWC_COMMAND_DANGER "PERICOLO"
#define FWC_COMMAND_PARKING "PARCHEGGIO"

/* -------------------------------------------------------------------------- */
/* Macro - STEER BY WIRE                                                      */
/* -------------------------------------------------------------------------- */
#define SBW_LOG_FILE_NAME "steer"
#define SBW_DEBUG_FILE_NAME "steer_debug"
#define SBW_SOCKET "steer_by_wire"
#define SBW_MSG_LEN BUFFER_SIZE
#define SBW_TURN_LEFT_LOG_MSG "STO GIRANDO A SINISTRA"
#define SBW_TURN_RIGHT_LOG_MSG "STO GIRANDO A DESTRA"

/* -------------------------------------------------------------------------- */
/* Macro - THROTTLE CONTROL                                                   */
/* -------------------------------------------------------------------------- */
#define TC_LOG_FILE_NAME "throttle"
#define TC_DEBUG_FILE_NAME "throttle_debug"
#define TC_SOCKET "throttle_control"
#define TC_MSG_LEN BUFFER_SIZE
#define TC_LOG_MSG "AUMENTO 5"

/* -------------------------------------------------------------------------- */
/* Macro - BRAKE BY WIRE                                                      */
/* -------------------------------------------------------------------------- */
#define BBW_LOG_FILE_NAME "brake"
#define BBW_DEBUG_FILE_NAME "brake_debug"
#define BBW_SOCKET "brake_by_wire"
#define BBW_MSG_LEN BUFFER_SIZE
#define BBW_LOG_MSG "FRENO 5"