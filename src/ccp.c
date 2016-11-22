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

static Ccp_ConnectionStateType Ccp_ConnectionState = CCP_DISCONNECTED;
static uint32_t Ccp_Mta;    /* Memory transfer address. */
static const Ccp_StationIDType Ccp_StationID = { sizeof(CCP_STATION_ID), CCP_STATION_ID };
static Ccp_SendCalloutType * Ccp_SendCallout = NULL;

#define CCP_COMMAND     (cmoIn->data[0])

void Ccp_Init(void)
{
    Ccp_ConnectionState = CCP_DISCONNECTED;
    Ccp_Mta = 0x00000000UL;
}


/**
 * Entry point, needs to be "wired" to CAN-Rx interrupt.
 *
 * @param cmoIn
 */
void Ccp_DispatchCommand(Ccp_MessageObjectType const * cmoIn)
{
    if (Ccp_ConnectionState == CCP_CONNECTED) {

    } else {
        /*
        ** Handle unconnected commands.
        */
        if (CCP_COMMAND == CONNECT) {

            /* Duplicate connects don't hurt us. */
        }
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
** Note:  These functions are only useful for unit-testing and stuff like that.
**
*/
Ccp_ConnectionStateType Ccp_GetConnectionState(void)
{
    return Ccp_ConnectionState;
}

uint32_t Ccp_GetMta(void)
{
    return Ccp_Mta;
}

