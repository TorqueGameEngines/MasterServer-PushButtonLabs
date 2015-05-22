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
#ifndef _SESSION_HANDLER_H_
#define _SESSION_HANDLER_H_

#include <map>
#include <list>
#include <vector>
#include "commonTypes.h"

// expire the game client query session after 15 seconds since last activity
#define SESSION_EXPIRE_TIME		15

// maximum number of sessions per peer / remote host at the same time
#define SESSION_MAX				10


typedef struct tServerAddress
{
	U32		address;
	U16		port;
} tServerAddress;

typedef std::vector<tServerAddress> tcServerAddrVector;


class Session
{
public:
	U16						session;		// associated session identifier
	U16						key;			// associated key
	S32						lastUsed;		// last time this session was used

	tcServerAddrVector		results;		// associated server query results
	U16						total;			// total number of servers
	U8						packTotal;		// total number of packets
	U16						packNum;		// number of servers per packet
	U16						packLast;		// number of servers on last packet

	Session(U16 session, U16 key)
	{
//		printf("DEBUG: session object born: %X\n", this);
		
		this->session	= session;
		this->key		= key;
		this->lastUsed	= getAbsTime();
	}
	~Session()
	{
//		printf("DEBUG: session object dying: %X\n", this);
		
		// do nothing
	}

};

typedef std::list<Session *> tcSessionList;


//=============================================================================
// Flood Control Manager
//=============================================================================

// TODO: this should be renamed to more like PeerControl since it is both the
// flood/spam and session manager now.

typedef struct tPeerRecord
{
	ServerAddress	peer;				// remote peer's address and port info
	tcSessionList	sessions;			// sessions list of (game client) peer
	S32				tsCreated;			// when this record was created
	S32				tsLastSeen;			// last time peer was seen
	S32				tsLastReset;		// last time peer's tickets were reset
	S32				tsBannedUntil;		// peer is banned until timestamp
	S32				tickets;			// count of violations
	U32				bans;				// count of times peer has been banned
} tPeerRecord;

typedef std::map<U32, tPeerRecord> tcPeerRecordMap;


class FloodControl
{
private:
	tcPeerRecordMap				m_Records;
	tcPeerRecordMap::iterator	m_ProcIT;

	void GetPeerRecord(tPeerRecord **peerrec, ServerAddress &peer, bool createNoExist);
	void CheckSessions(tPeerRecord *peerrec, bool forceExpire = false);
	
public:
	FloodControl();
	~FloodControl();


	// expunge expired peer records
	void DoProcessing(U32 count = 5);

	// the following functions return true=allowed, false=banned
	bool CheckPeer(ServerAddress &peer, tPeerRecord **peerrec = NULL, bool effectRep = true);
	bool CheckPeer(tPeerRecord *peerrec, bool effectRep = false);

	// modify peer record reputation
	void RepPeer(ServerAddress &peer, S32 tickets);
	void RepPeer(tPeerRecord *peerrec, S32 tickets);

	// session management functions
	void CreateSession(tPeerRecord *peerrec, tPacketHeader *header, Session **session);
	bool GetSession(tPeerRecord *peerrec, tPacketHeader *header, Session **session);
};

//extern SessionHandler	*gm_pSessions;
extern FloodControl		*gm_pFloodControl;

#endif
