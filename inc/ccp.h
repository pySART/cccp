

#if !defined(__CCCP_H)
#define __CCCP_H

#include <stdint.h>

#define CCP_VERSION_MAJOR       (2)
#define CCP_VERSION_RELEASE     (1)

#define CCP_LSDU_LEN            (8)

#define CCP_DEBUG_BUILD         (1)
#define CCP_RELEASE_BUILD       (2)

typedef enum tagCCP_CommandType {

    /* Mandatory Commands. */
    CONNECT             = 0x01,
    GET_CCP_VERSION     = 0x1B,
    EXCHANGE_ID         = 0x17,
    SET_MTA             = 0x02,
    DNLOAD              = 0x03,
    UPLOAD              = 0x04,
    GET_DAQ_SIZE        = 0x14,
    SET_DAQ_PTR         = 0x15,
    WRITE_DAQ           = 0x16,
    START_STOP          = 0x06,
    DISCONNECT          = 0x07,
    /* Optional Commands. */
    GET_SEED            = 0x12,
    UNLOCK              = 0x13,
    DNLOAD_6            = 0x23,
    SHORT_UP            = 0x0F,
    SELECT_CAL_PAGE     = 0x11,
    SET_S_STATUS        = 0x0C,
    GET_S_STATUS        = 0x0D,
    BUILD_CHKSUM        = 0x0E,
    CLEAR_MEMORY        = 0x10,
    PROGRAM             = 0x18,
    PROGRAM_6           = 0x22,
    MOVE                = 0x19,
    TEST                = 0x05,
    GET_ACTIVE_CAL_PAGE = 0x09,
    START_STOP_ALL      = 0x08,
    DIAG_SERVICE        = 0x20,
    ACTION_SERVICE      = 0x21
} CCP_CommandType;

typedef enum tagCCP_ReturnType {
    ACKNOWLEDGE                     = 0x00,
    DAQ_PROCESSOR_OVERLOAD          = 0x01,
    COMMAND_PROCESSOR_BUSY          = 0x10,
    DAQ_PROCESSOR_BUSY              = 0x11,
    INTERNAL_TIMEOUT                = 0x12,
    KEY_REQUEST                     = 0x18,
    SESSION_STATUS_REQUEST          = 0x19,
    COLD_START_REQUEST              = 0x20,
    CAL_DATA_INIT_REQUEST           = 0x21,
    DAQ_LIST_INIT_REQUEST           = 0x22,
    CODE_UPDATE_REQUEST             = 0x23,
    UNKNOWN_COMMAND                 = 0x30,
    COMMAND_SYNTAX                  = 0x31,
    PARAMETER_OUT_OF_RANGE          = 0x32,
    ACCESS_DENIED                   = 0x33,
    OVERLOAD                        = 0x34,
    ACCESS_LOCKED                   = 0x35,
    RESOURCE_FUNCTION_NOT_AVAILABLE = 0x36
} CCP_ReturnType;

typedef enum tagCCP_DTOType {
    EVENT_MESSAGE           = 254,
    COMMAND_RETURN_MESSAGE  = 255
} CCP_DTOType;

typedef enum tagCCP_ConnectionStateType { /* ConnectionState */
    CCP_DISCONNECTED = 0,
    CCP_CONNECTED = 0
} CCP_ConnectionStateType;


typedef enum {
    PGM = 0x40,
    DAQ = 0x02,
    CAL = 0x01
};


typedef struct tagCCP_MessageObjectType {
    uint32_t canID;
    uint8_t dlc;
    uint8_t data[CCP_LSDU_LEN];
} CCP_MessageObjectType;

typedef struct tagCCP_StationIDType {
    uint16_t len;
    const uint8_t name[];
} CCP_StationIDType;

#include "ccp_config.h"

#endif /* __CCCP_H */

