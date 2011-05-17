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

class ETClientInfo
{
	protected:
		ETClientInfo();
		static ETClientInfo instance;
		std::wstring localPlayerName();
		ETServerW serverInfo();
		ETPlayersW playersInfo(bool& success,bool& online,bool& slac,bool& etpro,bool& pb);
		ETPlayer ETPlayerWToETPlayer(ETPlayerW etplayerw);
	public:
		static ETClientInfo& getInstance();
		int waitingForETCommandTime;
		int pauseAfterCommand;
		ETClientStatus getStatus( bool echoProgress=false );
		ETClientStatusW getStatusW( bool echoProgress=false );
		bool isPlaying();
		std::wstring getVariableValue(std::wstring name, bool& success);
};

#endif /* ET_CLIENT_INFO_H_ */
