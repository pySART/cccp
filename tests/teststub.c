
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>


#include "ccp.h"


void req(uint8_t cmd, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5);

void sender(Ccp_MessageObjectType const * cmoOut)
{
    printf("CRM: ");
    Ccp_DumpMessageObject(cmoOut);
}

uint16_t ctr = 1;
Ccp_MessageObjectType cmo;

#define CANID	(0x070e)

#define Ccp_Connect()	req(CONNECT, 0x01, 0x00, 0, 0, 0, 0)
#define Ccp_GetCcpVersion() req(GET_CCP_VERSION, 2, 1, 0, 0, 0, 0)


int main(void)
{
    Ccp_Init();

    Ccp_SetSendCallout(&sender);
    Ccp_Connect();
    Ccp_GetCcpVersion();
/*    
    cmo.canID = 0x07e;
    cmo.dlc = 0x08;
    cmo.data[0] = CONNECT;
    cmo.data[1] = 0x01;
    cmo.data[2] = 0x01;
    cmo.data[3] = 0x00;
*/


    return 0;
}

void req(uint8_t cmd, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5)
{
     memset(&cmo, '\x00', sizeof(Ccp_MessageObjectType));
     cmo.canID = CANID;
     cmo.dlc = 0x08;
     cmo.data[0] = cmd;
     cmo.data[1] = ctr;
     ctr++;
     cmo.data[2] = b0;
     cmo.data[3] = b1;
     cmo.data[4] = b2;
     cmo.data[5] = b3;
     cmo.data[6] = b4;
     cmo.data[7] = b5;
     Ccp_DispatchCommand(&cmo);     
}
