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
#include "internal.h"
#include "MasterdTransport.h"
#include "masterd.h"


/**
 * @brief Constructor for the transport.
 *
 * Initializes the transport on the given host/port.
 *
 * @todo Host/port should probably be dealt with as an implementation detail?
 * @param host	Either a host, or a blank string. A blank string will cause the transport to guess at localhost.
 * @param port	Port number to use. 28002 is a good default.
 */
MasterdTransport::MasterdTransport( char * host, short port)
{
	int result;
	
	
	pfdCount = 0;
	sockOK   = false;
	
	this->sock = new netSocket();
	this->sock->open(false);
	result = this->sock->bind(host, port);
	if(result < 0)
	{
		// we failed to bind, usual causes are address and/or port already in use
		debugPrintf(DPRINT_ERROR, "   Failed to bind to socket, error: [%d] %s\n",
					errno, strerror(errno));
		return;
	}

	// prepare the poll file descriptor test array
	pfdArray[pfdCount].fd     = this->sock->getHandle();	// assign our socket to polling array entity
	pfdArray[pfdCount].events = POLLIN;						// we want only the recvfrom() ready event
	
	pfdCount++;

	// we're done and ready for use
	sockOK = true;
}


/**
 * @brief Shut things down.
 *
 * Closes socket, cleans up any other resources.
 */
MasterdTransport::~MasterdTransport()
{
	delete this->sock;
}


bool MasterdTransport::GetStatus(void)
{
	return sockOK;
}

/**
 * @brief Poll for packets.
 *
 * This is the heart of the transport, as it allows you to fetch
 * packets from the incoming queue. The recommended use is to
 * spin on a bit, and sleep  or do other things for a few milliseconds
 * if you don't have any packets pending.
 *
 * This assumes packets are less than 2500 bytes. Beware!
 *
 * @param	data	Pointer to a packet pointer; makes pointer store a
  					new packet (which you will need to clean up). Sets
  					this to null if we didn't have anything.
 * @param	from	Pointer to a ServerAddress pointer, makes pointer
					store a new server address (which you will also need
 					to clean up). Sets this to null if we didn't have
 					anything.
 * @return	true if it returned packet data, otherwise false.
 */
bool MasterdTransport::poll(Packet **data, ServerAddress **from, int timeout)
{
	// Check for packets

	char buff[2500];
	netAddress from_x;
	int len, result;


	// since function won't return false because of recvfrom() this
	// next section of code will use UNIX poll() to know when the
	// socket actually has anything. --TRON

	// perform the actual polling with a blocking timeout
	result = ::poll(&pfdArray[0], pfdCount, timeout);
	if((result > 0) && (pfdArray[0].revents & POLLIN))
	{
		
		// Read in a packet.... if we have one.
		if(
			(len=this->sock->recvfrom(buff, 2500, 0, &from_x))
			> 0)
		{
			*from = new ServerAddress(from_x.getHost(), from_x.getPort());
			*data = new Packet(buff, len);

			return true;
		}
	}

	*from = NULL;
	*data = NULL;

	// Nada.
	return false;
}

/**
 * @brief Send a packet through this transport.
 *
 * @param	data	Packet containing data to send.
 * @param	to		Address to which to send this data.
 */
void MasterdTransport::sendPacket(Packet * data, ServerAddress * to)
{
//	char * buff = data->getBuffer();
	netAddress *a = new netAddress();
	to->putInto(a);
//	this->sock->sendto(buff, (int)data->getLength(), 0, a);
	this->sock->sendto(data->getBufferPtr(), (int)data->getLength(), 0, a);
	delete a;
//	delete buff;
}

