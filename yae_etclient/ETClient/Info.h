/*
 * ETClient/Info.h
 *
 *  Created on: 2010-11-26
 *      Author: trakos
 */



#ifndef ET_CLIENT_INFO_H_
#define ET_CLIENT_INFO_H_

#include <string>
#include <vector>

enum team { AXIS, ALLIES, SPECTATOR };

struct ETServer
{
	std::wstring name;
	unsigned int ip;
	unsigned int port;
	std::wstring mod;
	std::wstring password;
	bool etpro;
	bool punkbuster;
	bool slac;
};

struct ETPlayer
{
	int id;
	team side;
	std::wstring nick;
	std::wstring pbguid;
	std::wstring etproguid;
	int slacid;
};

typedef std::vector<ETPlayer> ETPlayers;

struct ETClientStatus
{
	bool isPlaying;
	ETServer server;
	ETPlayer client;
	ETPlayers players;
};

class ETClientInfo
{
	protected:
		ETClientInfo();
		static ETClientInfo instance;
		std::wstring localPlayerName();
		ETServer serverInfo();
		ETPlayers playersInfo();
	public:
		static ETClientInfo& getInstance();
		int waitingForETCommandTime;
		ETClientStatus getStatus();
		bool isPlaying();
};

#endif /* ET_CLIENT_INFO_H_ */
