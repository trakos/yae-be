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

#define ET_CLIENT_STATUS_MAXPLAYERS 64;

enum team { AXIS, ALLIES, SPECTATOR };

struct ET_Client_Status_Server_W
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

struct ET_Client_Status_Server
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

struct ET_Client_Status_Player_W
{
	int id;
	team side;
	std::wstring nick;
	std::wstring pbguid;
	std::wstring etproguid;
	int slacid;
};

struct ET_Client_Status_Player
{
	int id;
	team side;
	std::string nick;
	std::string pbguid;
	std::string etproguid;
	int slacid;
};

typedef std::vector<ET_Client_Status_Player_W> ET_Client_Status_Players_W;
typedef std::vector<ET_Client_Status_Player> ET_Client_Status_Players;

class ET_Client_Status_W
{
	public:
		ET_Client_Status_W();
		bool online;
		ET_Client_Status_Server_W server;
		ET_Client_Status_Player_W client;
		ET_Client_Status_Players_W players;
};

class ET_Client_Status
{
	public:
		ET_Client_Status();
		bool online;
		ET_Client_Status_Server server;
		ET_Client_Status_Player client;
		ET_Client_Status_Players players;
};

std::ostream& operator <<(std::ostream& stream, ET_Client_Status const &status);

#endif /* ET_CLIENT_STATUS_H_ */
