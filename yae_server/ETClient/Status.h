/*
 * ETClient/Info.h
 *
 *  Created on: 2010-11-26
 *      Author: trakos
 */



#ifndef ET_CLIENT_STATUS_H_
#define ET_CLIENT_STATUS_H_

#include <string>
#include <vector>
#include <iostream>

enum team { AXIS, ALLIES, SPECTATOR };

struct ETServerW
{
	std::wstring name;
	unsigned int ip;
	unsigned int port;
	std::wstring mod;
	std::wstring password;
	std::wstring map;
	bool etpro;
	bool punkbuster;
	bool slac;
	short int gametype;
	short int maxClients;
};

struct ETServer
{
	std::string name;
	unsigned int ip;
	unsigned int port;
	std::string mod;
	std::string password;
	std::string map;
	bool etpro;
	bool punkbuster;
	bool slac;
	short int gametype;
	short int maxClients;
};

struct ETPlayerW
{
	int id;
	team side;
	std::wstring nick;
	std::wstring pbguid;
	std::wstring etproguid;
	int slacid;
};

struct ETPlayer
{
	int id;
	team side;
	std::string nick;
	std::string pbguid;
	std::string etproguid;
	int slacid;
};

typedef std::vector<ETPlayerW> ETPlayersW;
typedef std::vector<ETPlayer> ETPlayers;

struct ETClientStatusW
{
	bool isPlaying;
	ETServerW server;
	ETPlayerW client;
	ETPlayersW players;
};

struct ETClientStatus
{
	bool isPlaying;
	ETServer server;
	ETPlayer client;
	ETPlayers players;
};

std::ostream& operator <<(std::ostream& stream, ETClientStatus const &status);

#endif /* ET_CLIENT_STATUS_H_ */
