
#include "ccp.h"


static CCP_StateType CCP_State = CCP_DISCONNECTED;
static uint32_t CCP_Mta;    /* Memory transfer address. */


static const CCP_StationIDType CCP_StationID = { sizeof(CCP_STATION_ID), CCP_STATION_ID };


void CCP_Init(void)
{
    CCP_State = CCP_DISCONNECTED;
    CCP_Mta = 0x00000000UL;
}


void CCP_dispatchCommand(CCP_MessageObjectType const * cmoIn)
{
    /* Handle un-connected commands. */

    if (CCP_State == CCP_CONNECTED) {

    }
    /*
    // Mandatory Commands.
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
    DISCONNECT          = 0x07
    */
}


#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    printf("%s [%u]\n", CCP_StationID.name, CCP_StationID.len);

    return 0;
}

