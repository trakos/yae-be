/*
 * YaeMaster.cpp
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#include <YaeMaster.h>
#include <ET/Server.h>
#include <Tlogger/Front.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>

YaeMaster YaeMaster::instance = YaeMaster();

YaeMaster::YaeMaster() {};

YaeMaster& YaeMaster::getInstance()
{
	return YaeMaster::instance;
}

ET_Status YaeMaster::verifyData(ET_Status clientStatus)
{
	std::string ip = inet_ntoa(*(struct in_addr*)&clientStatus.server.ip);
	if ( ip == "127.0.0.1" )
	{
		LOG("checking localhost!", LERR);
		clientStatus.online = false;
		return clientStatus;
	}
	ET_Server server(ip, clientStatus.server.port);
	ET_Status serverStatus = server.getPublicStatus();
	if ( !serverStatus.online )
	{
		LOG("server sent by client is offline", LNOTE);
		return serverStatus;
	}
	std::map<std::string, unsigned int> serverNickToId;
	for( unsigned int i=0; i<serverStatus.players.size() && i<=ET_STATUS_MAXPLAYERS; i++ )
	{
		if ( serverStatus.players[i].id == -1 )
		{
			continue;
		}
		std::string colorless = ET_Server::colorless( serverStatus.players[i].nick );
		colorless = colorless.substr(0,10);
		std::transform(colorless.begin(), colorless.end(), colorless.begin(), tolower);
		if ( serverNickToId.count(colorless) > 0 )
		{
			LOG("Player nick: '"+serverStatus.players[i].nick+"' ("+colorless+") appears more than once, ignoring both...", LWARN);
			serverNickToId[colorless] = -1;
		}
		else
		{
			serverNickToId[colorless] = i;
		}
		serverStatus.players[i].id = -1;
	}
	std::map<std::string, unsigned int> clientNickToId;
	for( unsigned int i=0; i<clientStatus.players.size() && i<=ET_STATUS_MAXPLAYERS; i++ )
	{
		if ( clientStatus.players[i].id == -1 )
		{
			continue;
		}
		std::string colorless = clientStatus.players[i].nick.substr(0,10);
		std::transform(colorless.begin(), colorless.end(), colorless.begin(), tolower);
		if ( clientNickToId.count(colorless) > 0 )
		{
			LOG("Player nick: '"+clientStatus.players[i].nick+"' ("+colorless+") appears more than once, ignoring both...", LWARN);
			clientNickToId[colorless] = -1;
		}
		else
		{
			clientNickToId[colorless] = i;
		}
	}
	ET_Status_Player empty;
	empty.id = -1;
	empty.etproguid = "";
	empty.nick = "";
	empty.pbguid = "";
	empty.side = SPECTATOR;
	empty.slacid = 0;
	int k=0;
	ET_Status_Players players = ET_Status_Players(ET_STATUS_MAXPLAYERS+1,empty);
	std::map<std::string, unsigned int>::iterator itend = clientNickToId.end();
	for (std::map<std::string, unsigned int>::iterator it = clientNickToId.begin(); it != itend; ++it)
	{
		std::string colorless = it->first;
		unsigned int id = it->second;
		if ( id == -1 )
		{
			continue;
		}
		if ( !serverNickToId.count(colorless) || serverNickToId[colorless] == -1 )
		{
			LOG("Player nick: '"+clientStatus.players[id].nick+"' ("+colorless+") not found on server...", LWARN);
			continue;
		}
		unsigned int serverId = serverNickToId[colorless];
		players[id].nick = serverStatus.players[serverId].nick;
		players[id].side = serverStatus.players[serverId].side;
		players[id].etproguid = clientStatus.players[id].etproguid;
		players[id].pbguid = clientStatus.players[id].pbguid;
		players[id].slacid = clientStatus.players[id].slacid;
		players[id].id = id;
	}
	serverStatus.players = players;
	serverStatus.server.password = clientStatus.server.password;
	std::cout << serverStatus << std::endl;
	return serverStatus;
}
