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
#ifndef _MASTERDTRANSPORT_H_
#define _MASTERDTRANSPORT_H_

#include "network.h"
#include <poll.h>

class netSocket;

/**
 * @brief A simple wrapper class to abstract networking details.
 *
 * As many people may have their own preferred networking code,
 * we provide this simple transport class so that they may use
 * the masterd over whatever networking code they like.
 *
 * This is a simple, polling class, designed with sessionless
 * protocols in mind. The current implementation uses UDP by way
 * of plib. One could "wrap" TCP to provide similar functionality,
 * though for masterd use this might be inefficient.
 */
class MasterdTransport {
private:
	/**
	 * @brief Implementation detail.
	 *
	 * netSocket is part of Pegasus, plib's networking code.
	 *
	 * If you wanted to provide a different implementation of
	 * this class, you'd need to replace this.
	 */
	netSocket * sock;
	bool		sockOK;

	struct pollfd pfdArray[1];	// we're only working with one socket
	nfds_t pfdCount;			// poll() FD count

public:
	MasterdTransport(char * host, short port);
	~MasterdTransport();

	bool GetStatus(void);
	bool poll(Packet ** data, ServerAddress ** from, int timeout);
	void sendPacket(Packet * data, ServerAddress * to);
};

#endif

