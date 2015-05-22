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
/**
 * Define characteristics of outgoing packets.
 */
#ifndef _PACKETCONF_H_
#define _PACKETCONF_H_

/**
 * @brief Maximum size of a packet.
 *
 * DSL w/ PPPoE MTU is typically 1492, but that includes the IP headers,
 * therefore I changed this from 1500 to 1400 so we avoid fragmentation,
 * and as for as cable/FIOS is concerned they don't have as severe MTU
 * sizes as DSL w/ PPPoE has. --TRON
 */
#define UDP_HEADER_SIZE			28
#define MAX_MTU_SIZE			1492
#define MAX_PACKET_SIZE			(MAX_MTU_SIZE - UDP_HEADER_SIZE)

/**
 * @brief Maximum size of a list packet.
 *
 * @todo Tweak if we experience packet fragmentation/truncation.
 *
 */
#define LIST_PACKET_SIZE		MAX_PACKET_SIZE

/**
 * @brief Size of packet header (protocol-dependent)
 */
#define LIST_PACKET_HEADER		12

/**
 * @brief Size of a single server element in a packet.
 *
 * 4 bytes/ip, 2 bytes/port
 */
#define LIST_PACKET_SERVER_SIZE	6

/**
 * @brief Helper calculations for the max server calculations.
 *
 * This calculates the theoretical max.
 */
#define LIST_PACKET_MAX_SERVERS_	((int)((LIST_PACKET_SIZE - LIST_PACKET_HEADER) / LIST_PACKET_SERVER_SIZE))

/**
 * @brief Actual max servers number for list packets.
 *
 * We cap the number of servers/packet at 254 due to U8 constraints. (see sendListResponse)
 *
 * Since the theoretical max could be higher.
 */
#define LIST_PACKET_MAX_SERVERS	(LIST_PACKET_MAX_SERVERS_ > 254 ? 254 : LIST_PACKET_MAX_SERVERS_)

// packet's header size in bytes
#define PACKET_HEADER_SIZE	6

// space remaining after packet header
#define PACKET_PAYLOAD_SIZE	(MAX_PACKET_SIZE - PACKET_HEADER_SIZE)



#endif

