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
#ifndef _PACKET_H_
#define _PACKET_H_

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "ServerAddress.h"


typedef struct tPacketHeader
{
	U8		type;				// message type identifier
	U8		flags;				// flags

	// session and key below are the U32 sequencer in some packets
	U16		session;			// session identifier
	U16		key;				// session key
} tPacketHeader;


/**
 * @brief A byte-stream packet implementation.
 *
 * This allows you to read/write data one byte at a time; it is compatible
 * with Torque's bytestreams.
 *
 * Bitstreams are much more compact, but also somewhat more difficult to do,
 * and in any case, wholly unnecessary for masterd's purposes.
 *
 * Packets can be created in one of two modes,
 * write or read. This is chosen at create-time
 * based on which constructor is called.
 *
 */
class Packet
{
	protected:
		/**
		 * @brief Store packet data and its absolute size.
		 */
		char *buff;
		size_t size;

		/**
		 * @brief Store current position in the stream.
		 *
		 * We use a char * so as to avoid having to constantly
		 * recalculate our position in the array. This entails
		 * some pointer arithmetic, but that's why we love C,
		 * right? ;)
		 */
		char *ptr;

		/**
		 * @brief Is this a packet from which we should only read?
		 *
		 * Set in the constructor.
		 */
		bool readOnly;

		/**
		 * @brief Have read/write methods performed successfully?
		 *
		 * When this class is created statusOK is true, but if a read*()
		 * or write*() method tries to read or write out of bounds then
		 * this will be to false to indicate a procedure failure.
		 */
		bool statusOK;

	public:

		Packet(size_t size); // make a new empty writeable packet.
		Packet(char *aBuff, size_t length); // Prepare a packet for reading.
		~Packet();

		// primitive I/O methods
		void writeBytes(const void *data, size_t length);
		void readBytes(void *data, size_t length);

		// Our base primitives (U8 is a byte, hopefully :)
		void writeU8(U8 b);
		U8 readU8();

		void writeS8(S8 num)	{ writeU8((U8)num); }
		S8   readS8()			{ return readU8();  }

		// more than a single byte read/write, more efficient for arrays
		void writeS16(S16 num)	{ writeBytes(&num, sizeof(num)); }
		void writeS32(S32 num)	{ writeBytes(&num, sizeof(num)); }
		void writeS64(S64 num)	{ writeBytes(&num, sizeof(num)); }
		S16  readS16()			{ S16 num; readBytes(&num, sizeof(num)); return num; }
		S32  readS32()			{ S32 num; readBytes(&num, sizeof(num)); return num; }
		S64  readS64()			{ S64 num; readBytes(&num, sizeof(num)); return num; }
		
		void writeU16(U16 num)	{ writeBytes(&num, sizeof(num)); }
		void writeU32(U32 num)	{ writeBytes(&num, sizeof(num)); }
		void writeU64(U64 num)	{ writeBytes(&num, sizeof(num)); }
		U16  readU16()			{ U16 num; readBytes(&num, sizeof(num)); return num; }
		U32  readU32()			{ U32 num; readBytes(&num, sizeof(num)); return num; }
		U64  readU64()			{ U64 num; readBytes(&num, sizeof(num)); return num; }

		// Wrapper functions

		//	- read/write nullterm'ed strings
//		void readNullString(char* dat); // Reads into dat
//		void writeNullString(char* dat);

		//	- read/write length indicated strings
		char *readCString();
		void writeCString(const char *str, size_t length);
		void writeCString(const char *str)	{ writeCString(str, strlen(str)); }


		// Helper funcs
		size_t	getLength();
		char*	getBufferCopy();
		char*	getBufferPtr();
		
		void	writeHeader(U8 type, U8 flags, U16 session, U16 key);
		void	writeHeader(tPacketHeader &header);
		void	readHeader(U8 &type, U8 &flags, U16 &session, U16 &key);
		void	readHeader(tPacketHeader &header);

		bool	getStatus() { return statusOK; };

};

#endif
