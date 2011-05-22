/*
 * ET/Status.h
 *
 *  Created on: 2010-11-26
 *      Author: trakos
 */



#ifndef ET_STATUS_H_
#define ET_STATUS_H_

#include <string>
#include <vector>
#include <iostream>

#define ET_STATUS_MAXPLAYERS 64

enum ET_Status_Team { AXIS, ALLIES, SPECTATOR };

struct ET_Status_Server_W
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
	bool needPass;
	short int gametype; //
	short int maxClients;
};

struct ET_Status_Server
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
	bool needPass;
	short int gametype;
	short int maxClients;
};

struct ET_Status_Player_W
{
	int id;
	ET_Status_Team side;
	std::wstring nick;
	std::wstring pbguid;
	std::wstring etproguid;
	int slacid;
};

struct ET_Status_Player
{
	int id;
	ET_Status_Team side;
	std::string nick;
	std::string pbguid;
	std::string etproguid;
	int slacid;
};

typedef std::vector<ET_Status_Player_W> ET_Status_Players_W;
typedef std::vector<ET_Status_Player> ET_Status_Players;

class ET_Status_W
{
	public:
		ET_Status_W();
		bool online;
		ET_Status_Server_W server;
		ET_Status_Player_W client;
		ET_Status_Players_W players;
};

class ET_Status
{
	public:
		ET_Status();
		bool online;
		ET_Status_Server server;
		ET_Status_Player client;
		ET_Status_Players players;
};

std::ostream& operator <<(std::ostream& stream, ET_Status const &status);

#endif /* ET_STATUS_H_ */
