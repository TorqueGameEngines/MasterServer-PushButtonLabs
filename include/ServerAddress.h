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
#ifndef _SERVERADDRESS_H_
#define _SERVERADDRESS_H_

class netAddress;

/**
 * @brief Simple address representation.
 *
 * This class exists to abstract from the netAddress class
 * in plib. plib's class has a few shortcomings, primarily
 * that getting the address in non-string form is an expensive
 * operation. (You have to have netAddress sprintf it, then you
 * have to sscanf that string.)
 *
 * ServerAddress is a simple solution to this problem.
 */
class ServerAddress
{
public:
	ServerAddress();
	ServerAddress(ServerAddress *addr);
	ServerAddress(const char *host, const U16 port);
	ServerAddress(const netAddress *addr);

	char* toString();
	void set(const char *host, const U16 port);


	void putInto( netAddress * a );
	void getFrom( const netAddress * a );

	bool equals(const ServerAddress * a);

	/**
	 * @brief Quads for addy.
	 */
	union
	{
		U32	address;
		U8	addy[4];
	};

	/**
	 * @brief Port for addy.
	 */
	U16	port;
};

#endif

