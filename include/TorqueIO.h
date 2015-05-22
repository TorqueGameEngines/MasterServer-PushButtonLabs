/*
	(c) Ben Garney <bgarney@pblabs.com> 2002-2003
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
#ifndef _TORQUE_IO_H_
#define _TORQUE_IO_H_

#include "masterd.h"
#include "Packet.h"
#include "SessionHandler.h"

typedef struct tMessageSession
{
	ServerAddress	*addr;		// address of who sent the message packet
	tPacketHeader	*header;	// message header of packet
	Packet			*pack;		// remaining payload of packet
	tPeerRecord		*peerrec;	// packet associated peer record

	ServerStore		*store;		// server storage manager reference
	Session			*session;	// session associated with request
} tMessageSession;


// Handlers
bool handleListRequest (tMessageSession &msg);
bool handleInfoRequest (tMessageSession &msg);
bool handleTypesRequest(tMessageSession &msg);
bool handleInfoResponse(tMessageSession &msg);
bool handleHeartbeat   (tMessageSession &msg);

// Senders
class ServerResults; // We can't do this because it breaks delete

void sendTypesResponse	(tMessageSession &msg);
void sendInfoResponse	(tMessageSession &msg);
void sendListResponse	(tMessageSession &msg, U8 index);
void sendInfoRequest	(tMessageSession &msg);

#endif
