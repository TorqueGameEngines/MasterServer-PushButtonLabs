/*
	(c) Ben Garney <bgarney@pblabs.com> 2002-2003
	(c) Mike Kuklinski <> 2005
	(c) Nathan Martin <nmartin@gmail.com> 2011

    This file is part of the Pushbutton Master Server.

    PMS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PMS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the PMS; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/**
 * Includes relating to the network code.
 */

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdlib.h>
#include "ServerAddress.h"
#include "Packet.h"
#include "MasterdTransport.h"

// Useful constants, which are defined in
// engine/sim/netInterface.h:9
#define	MasterServerGameTypesRequest	2		// *
#define	MasterServerGameTypesResponse	4		// !
#define	MasterServerListRequest			6		// *
#define	MasterServerListResponse		8		// !
#define	GameMasterInfoRequest			10		// !
#define	GameMasterInfoResponse			12		// *
#define	GamePingRequest					14
#define	GamePingResponse				16
#define	GameInfoRequest					18
#define	GameInfoResponse				20
#define	GameHeartbeat					22		// *
#define MasterServerInfoRequest         24		// *, Torque doesn't use this...
#define MasterServerInfoResponse        26

// Legend:
//   * -- Implemented for Receive
//   ! -- Implemented for Send

// Helper functions
void initNetworkLib();	// Initialize the library.
void killNetworkLib();	// Shut us down.

// Reduce UL dependencies...
int getAbsTime();
void millisleep(int delay);


#endif
