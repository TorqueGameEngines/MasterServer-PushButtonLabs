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
#include "internal.h"
#include "ServerAddress.h"
#include "masterd.h"

/**
 * @brief Blank constructor.
 */
ServerAddress::ServerAddress()
{
	this->port		= 0;
	this->address	= 0;
}

/**
 * Copy constructor.
 *
 * @param	a	ServerAddress to construct from.
 */
ServerAddress::ServerAddress(ServerAddress *a)
{
	this->port		= a->port;
	this->address	= a->address;
}

/**
 * @brief Construct from a host and port.
 *
 * see ServerAddress::set
 */
ServerAddress::ServerAddress(const char *host, const U16 port) {
	set(host, port);
}

/**
 * @brief Create ServerAddress from a netAddress.
 *
 * This is a convenience function for use by plib
 * aware portions of the network library. Replace
 * or extend as needed to support other networking
 * systems.
 */
ServerAddress::ServerAddress(const netAddress * a) {
	getFrom(a);
}

/**
 * @brief Set the host/port of the address.
 *
 * @param	aHost	String specifying host (in a.b.c.d format)
 * @param	aPort	Port number.
 */
void ServerAddress::set( const char *host, const U16 port )
{
	U32 num[4];
	int i;
	
	if(strlen(host) < 7)
	{
		debugPrintf(DPRINT_ERROR, "FIXME: ServerAddress::set() - died on invalid host!\n");
		exit(1);
		return; // a.b.c.d is minimum allowed.
	}

	char x;
	sscanf(host, "%u%c%u%c%u%c%u",
		&num[0], &x, &num[1], &x, &num[2], &x, &num[3]);

	for(i=0; i<4; i++)
		this->addy[i] = num[i];
	this->port = port;
}

/**
 * @brief Get a string representing the address.
 *
 * Format is "a.b.c.d:port". You must deallocate
 * the string returned.
 *
 * @return a new string containing the address.
 */
char* ServerAddress::toString(  )
{
	char * tmp = new char[24];
	sprintf(tmp, "%u.%u.%u.%u",
		addy[0], addy[1], addy[2], addy[3]);
	return tmp;
}

/**
 * @brief Put the address information into a netAddress
 *
 * @param	a	A netAddress to store into.
 */
void ServerAddress::putInto( netAddress * a )
{
	char *str;

	str = this->toString();
	a->set(str, this->port);
	
	delete[] str;
}

/**
 * @brief Load addy info from a netAddress
 *
 * @param	a	A netAddress to get info from.
 */
void ServerAddress::getFrom( const netAddress * a )
{
	set(a->getHost(), a->getPort());
}

/**
 * @brief Check if this ServerAddress equals another.
 *
 * @return True on equality.
 * @param	a	ServerAddress to compare with.
 */
bool ServerAddress::equals(const ServerAddress * a)
{
	if(a->port == this->port && a->address == this->address)
		return true;

	return false;
}

