/*
 * ETClient/Status.h
 *
 *  Created on: 2010-11-26
 *      Author: trakos
 */



#ifndef ET_CLIENT_STATUS_H_
#define ET_CLIENT_STATUS_H_

#include <string>
#include <vector>


struct ETServer
{
	std::wstring name;
	unsigned int ip;
	unsigned int port;
	std::wstring mod;
	std::wstring password;
	bool punkbuster;
	bool slac;
};

struct ETPlayer
{
	std::wstring nick;
	std::wstring pbguid;
	std::wstring etproguid;
	std::wstring slacguid;
	std::wstring slacid;
};

typedef std::vector<ETPlayer> ETPlayers;

class ETClientStatus
{
protected:
	ETClientStatus();
	static ETClientStatus instance;
public:
	static ETClientStatus& getInstance();
	ETPlayer localPlayerInfo();
	ETServer serverInfo();
	ETPlayers playersInfo();
};

#endif /* ET_CLIENT_STATUS_H_ */
