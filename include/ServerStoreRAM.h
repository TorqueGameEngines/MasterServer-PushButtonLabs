#ifndef _SERVERSTORERAM_H
#define _SERVERSTORERAM_H

#include "ServerStore.h"
#include <map>


typedef std::map<U64, ServerInfo> tcServerMap;

/**
 * Linked list server store implementation
 *
 */
class ServerStoreRAM : public ServerStore
{
private:
	tcServerMap				m_Servers;
	tcServerMap::iterator	m_ProcIT;

	U64  AddrToSlot(ServerAddress *addr);
	bool FindServer(ServerAddress *addr, tcServerMap::iterator &it);
	bool FindServer(ServerAddress *addr, ServerInfo **serv);
	void AddServer(ServerAddress *addr, ServerInfo *info);
	void RemoveServer(tcServerMap::iterator &it);
	void RemoveServer(ServerAddress *addr);

	
public:
    ServerStoreRAM();
    ~ServerStoreRAM();
	
	void DoProcessing(int count = 5);
	void HeartbeatServer(ServerAddress *addr, U16 *session, U16 *key);
	void UpdateServer(ServerAddress *addr, ServerInfo *info);

	void QueryServers(Session *session, ServerFilter *filter);

	U32 getCount()	{ return m_Servers.size(); }

};

#endif // _SERVERSTORERAM_H
