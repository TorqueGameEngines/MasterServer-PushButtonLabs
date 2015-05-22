/*
	(c) Ben Garney <bgarney@pblabs.com> 2002-2003

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
 * @brief Main loop for masterd
 *
 * Opens a port via transport, and then listens for stuff.
 */

#include "masterd.h"
#include "TorqueIO.h"
#include "SessionHandler.h"

// What port should we listen on for packets?
#define SERVER_HOST			""
#define SERVER_PORT			28002


ServerStore *store;
MasterdTransport *transport;

int main(int argc, char**argv)
{

	// Set up networking
	printf(" - Initializing networking.\n");
	initNetworkLib();

	// Open the UDP server socket.
	printf(" - Opening master server port.\n");
	transport = new MasterdTransport(SERVER_HOST, SERVER_PORT);

	// Load the server database...
	printf(" - Loading server database.\n");
//	store = new ServerStoreRAM();
	store = new ServerStoreSQLite();

	// Set up session tracking...
	printf(" - Initializing session handler.\n");
	sessions = new SessionHandler();

	// Now loop, dealing with the net.
	printf(" - Entering main loop.\n");

	ServerAddress * addy;
	Packet * data;
	int pack_type;
	while(1)
	{
		// Expire old sessions.
		sessions->doProcessing();

		// Handle network incoming...
		if(transport->poll(&data, &addy))
		{
			// The first byte is the type...

			pack_type = data->readU8();
			switch((int)pack_type)
			{
				case MasterServerGameTypesRequest:
					printf("MasterServerGameTypesRequest given\n");
					handleTypesRequest(addy, data);
				break;

				case MasterServerListRequest:
					printf("MasterServerListRequest given\n");
					handleListRequest(addy, data);
				break;

				case GameMasterInfoResponse:
					printf("GameMasterInfoResponse received\n");
					handleInfoResponse(addy, data);
				break;

				case GameHeartbeat:
					printf("GameHeartbeat received\n");
					handleHeartbeat(addy, data);
				break;

				default:
					printf("!!! unknown packet, type=%d\n", pack_type);
				break;
			}

			delete data;
			delete addy;
		} else {
			// We didn't get a packet, so go to sleep for a bit.
			millisleep(100);
		}
	}

	return 0;
}


