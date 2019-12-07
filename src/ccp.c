/*
 * pySART - Simplified AUTOSAR-Toolkit for Python.
 *
 * (C) 2007-2016 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * s. FLOSS-EXCEPTION.txt
 */

#include "ccp.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static Ccp_ConnectionStateType Ccp_ConnectionState = CCP_DISCONNECTED;
void Ccp_WriteMemory(void * dest, void * src, uint16_t count);
void Ccp_ReadMemory(void * dest, void * src, uint16_t count);

/*
Memory transfer addresses:
---
    - MTA0 is used by the commands DNLOAD, UPLOAD, DNLOAD_6, SELECT_CAL_PAGE, CLEAR_MEMORY, PROGRAM and PROGRAM_6.
    - MTA1 is used by the MOVE command
*/
static uint32_t Ccp_Mta0, Ccp_Mta1;
static uint8_t Ccp_MtaExtension;

static Ccp_SendCalloutType Ccp_SendCallout = NULL;
static const Ccp_StationIDType Ccp_StationID = { sizeof(CCP_STATION_ID), CCP_STATION_ID };

#if defined(CCP_SIMULATOR)
static uint8_t Ccp_SimulatedMemory[CCP_SIMULATED_MEMORY_SIZE];
#endif

#define CCP_COMMAND     (cmoIn->data[0])

#define DATA_IN(idx)    (cmoIn->data[(idx)])
#define DATA_OUT(idx)   (cmoOut->data[(idx)])

#define COUNTER_IN      (cmoIn->data[1])
#define COUNTER_OUT     (cmoOut->data[2])


void Ccp_Init(void)
{
    Ccp_ConnectionState = CCP_DISCONNECTED;
    Ccp_Mta0 = Ccp_Mta1 = 0x000000000L;
    Ccp_MtaExtension = 0x00;
#if defined(CCP_SIMULATOR)
    memcpy(&Ccp_SimulatedMemory, &Ccp_StationID.name, Ccp_StationID.len);
#endif
}

void Ccp_SetDTOValues(Ccp_MessageObjectType * cmoOut, uint8_t type, uint8_t returnCode,
          uint8_t counter, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4)
{
    cmoOut->canID = CCP_MASTER_CANID;
    cmoOut->dlc = 8;
    DATA_OUT(0) = type;
    DATA_OUT(1) = returnCode;
    DATA_OUT(2) = counter;
    DATA_OUT(3) = b0;
    DATA_OUT(4) = b1;
    DATA_OUT(5) = b2;
    DATA_OUT(6) = b3;
    DATA_OUT(7) = b4;
}


void Ccp_SetUploadDTO(Ccp_MessageObjectType * cmoOut, uint8_t type, uint8_t returnCode, uint8_t counter)
{
    cmoOut->canID = CCP_MASTER_CANID;
    cmoOut->dlc = 8;
    DATA_OUT(0) = type;
    DATA_OUT(1) = returnCode;
    DATA_OUT(2) = counter;
    /* Payload already set. */
}

#define Ccp_AcknowledgedCRM(ctr, b0, b1, b2, b3, b4)    Ccp_SetDTOValues(&cmoOut, COMMAND_RETURN_MESSAGE, ACKNOWLEDGE, ctr, b0, b1, b2, b3, b4)
#define Ccp_AcknowledgedUploadCRM(ctr)                  Ccp_SetUploadDTO(&cmoOut, COMMAND_RETURN_MESSAGE, ACKNOWLEDGE, ctr)

/**
 * Entry point, needs to be "wired" to CAN-Rx interrupt.
 *
 * @param cmoIn
 */
void Ccp_DispatchCommand(Ccp_MessageObjectType const * cmoIn)
{
    Ccp_MessageObjectType cmoOut = {0};
    uint16_t stationAddress;

    printf("Req: ");
    Ccp_DumpMessageObject(cmoIn);

    if (Ccp_ConnectionState == CCP_CONNECTED) {
        switch (CCP_COMMAND) {
        case GET_CCP_VERSION:
                Ccp_AcknowledgedCRM(COUNTER_IN, CCP_VERSION_MAJOR, CCP_VERSION_RELEASE,0, 0, 0);
                Ccp_SendCmo(&cmoOut);
                break;
            case EXCHANGE_ID:

                Ccp_AcknowledgedCRM(COUNTER_IN,
                                    Ccp_StationID.len,
                                    0 ,                 /* data type qualifier of slave device ID (optional and implementation specific). */
                                    PGM | DAQ | CAL,    /* TODO: config. */
                                    0,                  /* No protection. */
                                    0
                );
                Ccp_SendCmo(&cmoOut);
                Ccp_Mta0 = (uint32_t)&Ccp_StationID.name;
                break;
            case SET_MTA:
                printf("SetMTA\n");
                if (DATA_IN(2) == 0) {
                    Ccp_Mta0 = (DATA_IN(4) << 24) | (DATA_IN(5) << 16) | (DATA_IN(6) << 8) | DATA_IN(7);
                } else if (DATA_IN(2) == 1) {
                    Ccp_Mta1 = (DATA_IN(4) << 24) | (DATA_IN(5) << 16) | (DATA_IN(6) << 8) | DATA_IN(7);
                } else {
                    /* Invalid MTA number.*/
                    break;
                }
                Ccp_MtaExtension = DATA_IN(3);

                Ccp_AcknowledgedCRM(COUNTER_IN, 0, 0,0, 0, 0);
                Ccp_SendCmo(&cmoOut);
                break;
            case DNLOAD:
                printf("Download\n");
                Ccp_WriteMemory(&Ccp_Mta0, &DATA_IN(3), DATA_IN(2));
                Ccp_Mta0 += DATA_IN(2);
                Ccp_AcknowledgedCRM(
                    COUNTER_IN,
                    Ccp_MtaExtension,
                    (Ccp_Mta0 & 0xff000000) >> 24,
                    (Ccp_Mta0 & 0x00ff0000) >> 16,
                    (Ccp_Mta0 & 0x0000ff00) >> 8,
                    Ccp_Mta0 & 0xff
                );
                Ccp_SendCmo(&cmoOut);
            case UPLOAD:
                printf("Upload\n");
                //Ccp_ReadMemory(&Ccp_Mta0, &DATA_OUT(3), DATA_IN(2));
                Ccp_Mta0 += DATA_IN(2);
                Ccp_AcknowledgedUploadCRM(COUNTER_IN);
                break;
        }
    } else {
        /*
        ** Handle unconnected commands.
        */
        if (CCP_COMMAND == CONNECT) {
            stationAddress = DATA_IN(2) | (DATA_IN(3) << 8);

            //printf("Connect [%u] [%u]\n", CCP_STATION_ADDRESS, stationAddress);
            if (CCP_STATION_ADDRESS == stationAddress) {
                Ccp_AcknowledgedCRM(COUNTER_IN, 0, 0, 0, 0, 0);
                Ccp_SendCmo(&cmoOut);
                Ccp_ConnectionState = CCP_CONNECTED;
            } else {
                /* "A CONNECT command to another station temporary disconnects the active station " */
                //printf("Disconnecting...\n");
                Ccp_ConnectionState = CCP_DISCONNECTED;
            }
        }
    }
    /*
    // Mandatory Commands.
    DNLOAD              = 0x03,
    UPLOAD              = 0x04,
    GET_DAQ_SIZE        = 0x14,
    SET_DAQ_PTR         = 0x15,
    WRITE_DAQ           = 0x16,
    START_STOP          = 0x06,
    DISCONNECT          = 0x07
    */
}


void Ccp_SendCmo(Ccp_MessageObjectType const * cmoOut)
{
    /*
    **
    ** Note: A callout is only needed for unit-testing.
    ** TODO: Conditional compilation (testing vs. "real-world").
    **
    */
    if (Ccp_SendCallout) {
        (*Ccp_SendCallout)(cmoOut);
    }
}

void Ccp_SetSendCallout(Ccp_SendCalloutType * callout)
{
    Ccp_SendCallout = callout;
}


/*
**
** Global Helper Functions.
**
** Note: These functions are only useful for unit-testing and debugging.
**
*/
Ccp_ConnectionStateType Ccp_GetConnectionState(void)
{
    return Ccp_ConnectionState;
}

uint32_t Ccp_GetMta0(void)
{
    return Ccp_Mta0;
}

uint32_t Ccp_GetMta1(void)
{
    return Ccp_Mta1;
}

void Ccp_DumpMessageObject(Ccp_MessageObjectType const * cmo)
{
    printf("%08X  %u  [%02X %02X %02X %02X %02X %02X %02X %02X]\n", cmo->canID, cmo->dlc,
           cmo->data[0], cmo->data[1], cmo->data[2], cmo->data[3], cmo->data[4], cmo->data[5], cmo->data[6], cmo->data[7]
    );
}

void Ccp_WriteMemory(void * dest, void * src, uint16_t count)
{
#if defined(CCP_SIMULATOR)
    ptrdiff_t  diff;
    printf("Dest: %p -- SimMem: %p\n", dest, &Ccp_SimulatedMemory);
#else
    memcpy(dest, src, count);
#endif
}
