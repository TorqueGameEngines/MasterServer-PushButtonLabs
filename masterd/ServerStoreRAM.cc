/*
	(c) Ben Garney <bgarney@pblabs.com> 2002-2003
	(c) Mike Kuklinski <admin@kuattech.com> 2005
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

#include "masterd.h"
#include <stdlib.h>


// Trick to allow conditional compilation of sqlite.
#ifndef HAVE_SQLITE
#	define _SERVERSTORESQLITE_CPP_
#else
#	pragma	message( "	- Compiling RAM support.")
#endif

#ifndef	_SERVERSTORERAM_CPP_
#define _SERVERSTORERAM_CPP_

#include "ServerStoreRAM.h"

/**
 * @brief Initialize server db to use filename.
 *
 * @param	filename	File to store database in.
 */
ServerStoreRAM::ServerStoreRAM()
{
	m_ProcIT = m_Servers.begin();
	
}
ServerStoreRAM::~ServerStoreRAM()
{
	
}


//==============================================================================
// Server Store in RAM rewrite
//==============================================================================

U64 ServerStoreRAM::AddrToSlot(ServerAddress *addr)
{
	U64 slot;

	
	// abort on NULL
	if(!addr)
		return 0;

	// 2 bytes: unused / 0x0000
	// 4 bytes: IPv4 address
	// 2 bytes: UDP port number
	slot = (addr->address << 16) | (addr->port & 0xFFFF);

	// done
	return slot;
}

bool ServerStoreRAM::FindServer(ServerAddress *addr, tcServerMap::iterator &it)
{
	U64		slot = AddrToSlot(addr);

	// abort on NULL
	if(!addr)
		return false;

	// find server entry
	it = m_Servers.find(slot);
	if(it != m_Servers.end())
		return true;

	// done
	return false;
}

bool ServerStoreRAM::FindServer(ServerAddress *addr, ServerInfo **serv)
{
	tcServerMap::iterator	it;


	if(FindServer(addr, it))
		*serv = &it->second;
	else
		*serv = NULL;

	// done
	return (*serv != NULL);
}

void ServerStoreRAM::AddServer(ServerAddress *addr, ServerInfo *info)
{
	U64			slot = AddrToSlot(addr);
	char		*oldGame, *oldMission, *str;


	// abort on NULL
	if(!addr || !info)
		return;

	
	// add missing information
	info->last_info		= getAbsTime();
	info->addr			= *addr;

	// notify game and mission types manager
	info->gameType		= m_GameTypes.Push(   oldGame    = info->gameType);
	info->missionType	= m_MissionTypes.Push(oldMission = info->missionType);

	// insert new server record
	m_Servers[slot]		= *info;

	debugPrintf(DPRINT_VERBOSE, "New Server [%s:%hu] Game:\"%s\", Mission:\"%s\"\n",
				str = addr->toString(), addr->port, info->gameType, info->missionType);
	delete[] str;
	
	// give back the old string references to the stack serverinfo
	info->gameType		= oldGame;
	info->missionType	= oldMission;

	// don't destroy player list, we're using it
	info->setToDestroy(false);
	
	// done
}

void ServerStoreRAM::RemoveServer(tcServerMap::iterator &it)
{
	ServerInfo *info;
	info = &it->second;
	char *str;

	
	debugPrintf(DPRINT_VERBOSE, "Remove Server [%s:%hu] Game:\"%s\", Mission:\"%s\"\n",
				str = info->addr.toString(), info->addr.port, info->gameType, info->missionType);
	delete[] str;
	
	// notify game and mission types manager
	m_GameTypes.PopRef(info->gameType);
	m_MissionTypes.PopRef(info->missionType);

	// invalid the type pointers
	info->gameType		= NULL;
	info->missionType	= NULL;

	// delete the record
	m_Servers.erase(it);

	// done
}

void ServerStoreRAM::RemoveServer(ServerAddress *addr)
{
	tcServerMap::iterator	it;

	// find and then delete server record
	if(FindServer(addr, it))
		RemoveServer(it);

	// done
}



void ServerStoreRAM::DoProcessing(int count)
{
	tcServerMap::iterator next;
	ServerInfo *rec;


//CheckMoreServers:
	// check for out of date records
	for(; m_ProcIT != m_Servers.end() && --count;)
	{
		rec = &m_ProcIT->second;

		// has server record expired?
		if(rec->last_info + (int)gm_pConfig->heartbeat > getAbsTime())
		{
			// nope, next...
			m_ProcIT++;
			continue;
		}

		next = m_ProcIT;
		next++;

		// server record has expired, remove it
		RemoveServer(m_ProcIT);

		// iterator is now invalid, use next
		m_ProcIT = next;
	}

	// start from the beginning if we hit the end
	if(m_ProcIT == m_Servers.end())
		m_ProcIT = m_Servers.begin();

	// check on more records if we reached the end too early
//	if(count && (count < m_Servers.size()))
//		goto CheckMoreServers;

	// done
}

void ServerStoreRAM::HeartbeatServer(ServerAddress *addr, U16 *session, U16 *key)
{
	// generate a random session and key to use to send an info request from the
	// server that just hearbeat us. We really don't care about the session and
	// key details so we don't save them because we track servers based on the
	// IP address and port anyway. Other than that this function does nothing
	// special.

	// seed the random generator
	srand(getAbsTime() + addr->address + addr->port);
	
	if(session)	*session	= (U16)rand();
	if(key)		*key		= (U16)rand();

	// done
}

void ServerStoreRAM::UpdateServer(ServerAddress *addr, ServerInfo *info)
{
	ServerInfo	*rec;
	char		*oldGame, *oldMission, *str;


	// find the existing server record
	if(!FindServer(addr, &rec))
	{
		// not found, add server to our list and abort
		AddServer(addr, info);
		return;
	}

	// update an existing server record
	rec->maxPlayers 	= info->maxPlayers;
	rec->regions		= info->regions;
	rec->version		= info->version;
	rec->infoFlags		= info->infoFlags;
	rec->numBots		= info->numBots;
	rec->CPUSpeed		= info->CPUSpeed;
	rec->playerCount	= info->playerCount;

	// special handling of game and mission type,
	// remember current type references.
	oldGame		= rec->gameType;
	oldMission	= rec->missionType;

	// use game and mission type managers to see if these types are unique 
	rec->gameType		= m_GameTypes.Push(   info->gameType,    false);
	rec->missionType	= m_MissionTypes.Push(info->missionType, false);

	// notify game and mission type managers that type isn't in use anymore by server
	if(oldGame    != rec->gameType)		m_GameTypes.PopRef(oldGame);
	if(oldMission != rec->missionType)	m_MissionTypes.PopRef(oldMission);


	// destroy existing player GUID list and point to the new one
	if(rec->playerList)
		delete[] rec->playerList;

	rec->playerList = info->playerList;
	info->setToDestroy(false);			// don't destroy list, we're using it

	// update last information update time
	rec->last_info		= getAbsTime();

	debugPrintf(DPRINT_VERBOSE, "Updated Server [%s:%hu] Game:\"%s\", Mission:\"%s\"\n",
				str = addr->toString(), addr->port, rec->gameType, rec->missionType);
	delete[] str;
	
	// done
}

void ServerStoreRAM::QueryServers(Session *session, ServerFilter *filter)
{
	tcServerMap::iterator	it;
	ServerInfo				*info;
	tServerAddress			addr;
	char					*game = NULL, *mission = NULL;
	bool					buddyFound;
	U32						i, n;


	debugPrintf(DPRINT_VERBOSE, "Query for Game:\"%s\", Mission:\"%s\"\n",
				filter->gameType, filter->missionType);

	// special handling of game and mission types
	if(filter->gameType && strlen(filter->gameType))
	{
		// see if game type is a wildcard 'any' keyword
		if(stricmp(filter->gameType, "any"))
		{
			// nope, now try finding this type in our unique game type manager
			game = m_GameTypes.GetRef(filter->gameType);

			// was game type found?
			if(!game)
				goto SkipFilterTests; // no match found, no servers will satify filter
		}
	}
	if(filter->missionType && strlen(filter->missionType))
	{
		// see if mission type is a wildcard 'any' keyword
		if(stricmp(filter->missionType, "any"))
		{
			// nope, now try finding this type in our unique mission type manager
			mission = m_MissionTypes.GetRef(filter->missionType);

			// was mission type found?
			if(!mission)
				goto SkipFilterTests; // no match found, no servers will satify filter
		}
	}
	
	
	// build server list matching the query filter
	for(it = m_Servers.begin(); it != m_Servers.end(); it++)
	{
		// get server record
		info = &it->second;

		// check the game type
		if(game && (game != info->gameType))
			continue; // skip

		// check the mission type
		if(mission && (mission != info->missionType))
			continue; // skip

		// check minimum player count
		if(filter->minPlayers && (info->playerCount < filter->minPlayers))
			continue; // skip

		// check maximum player count
		if(filter->maxPlayers && (info->playerCount > filter->maxPlayers))
			continue; // skip

		// check regions mask
		if(filter->regions && !(info->regions & filter->regions))
			continue; // skip

		// check minimum version
		if(filter->version && (info->version < filter->version))
			continue; // skip

		// check information bit flag mask
		if(filter->filterFlags && !(info->infoFlags & filter->filterFlags))
			continue; // skip

		// check maximum bot count
		if(filter->maxBots && (info->numBots > filter->maxBots))
			continue; // skip

		// check minimum processor speed
		if(filter->minCPUSpeed && (info->CPUSpeed < filter->minCPUSpeed))
			continue;

		// this part we check on our buddies, but I don't know if we're suppose
		// to exclude servers of which client's buddies aren't on them just like
		// an explicit filter, or any servers that have our buddies on them will
		// be an exception to the filters above, but for now we're doing the former.
		// --TRON

		// check buddies list
		if(filter->buddyCount)
		{
			buddyFound = false;
			
			// see if any of our buddies are on this server
			for(i=0; (i < info->playerCount) && !buddyFound; i++)
			{
				for(n=0; n < filter->buddyCount; n++)
				{
					// does buddy match?
					if(filter->buddyList[n] == info->playerList[i])
					{
						// yep, server is acceptable
						buddyFound = true;
						break;
					}
				}
			}

			// was any of the client's buddies on this server
			if(!buddyFound)
				continue; // nope, skip
		}
		

		// server passed the filter test, add it to the list
		addr.address	= info->addr.address;
		addr.port		= info->addr.port;
		
		session->results.push_back(addr);
	}

SkipFilterTests:
	// now we have our server list result and need to figure out how many
	// packets are required.
	session->total		= session->results.size();
	session->packTotal	= (session->total / LIST_PACKET_MAX_SERVERS) +1;
	session->packNum	= session->total / LIST_PACKET_MAX_SERVERS;
	session->packLast	= session->total % LIST_PACKET_MAX_SERVERS;

	// done
}



#endif // _SERVERSTORERAM_CPP_

