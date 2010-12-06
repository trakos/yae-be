/*
 * ETClient/Info.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <string>
#include <stdlib.h>
#include <boost/foreach.hpp>
#include <sstream>
#include <ETClient/Info.h>
#include <ETClient/Console.h>
#include <utils.h>
#include <Logger/Logger.h>


using std::wstring;
typedef std::vector< wstring > array;

ETClientInfo ETClientInfo::instance = ETClientInfo();

ETClientInfo::ETClientInfo(): waitingForETCommandTime(100)
{}

ETClientInfo& ETClientInfo::getInstance()
{
	return ETClientInfo::instance;
}

std::wstring ETClientInfo::localPlayerName()
{
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"name");
	Sleep(this->waitingForETCommandTime);
	// ]name - the command itself, skip
	ETClientConsole::getInstance().getLastLine();
	wstring line = ETClientConsole::getInstance().getLastLine();
	wstring start = L"\"name\" is:\"";
	wstring end = L"\" default:\"ETPlayer\"";
	if(line.find(start, 0) == 0)
	{
		line = line.substr(start.length());
		if(line.find(end, 0) != line.npos)
		{
			// delete the $end from the $line
			line = line.substr(0, line.length()-end.length());
			// truncate to the length of nick shown in the other commands
			return rtrim(line.substr(0,26));
		}
	}
	LOG("ETClientInfo","localPlayerName",L"cannot understand: "+line,LWARN);
	return L"";
}

#include <iostream>
ETPlayers ETClientInfo::playersInfo()
{
	//////////////////////////////////////////
	// PLAYERS////////////////////////////////
	//////////////////////////////////////////
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"players");
	Sleep(this->waitingForETCommandTime);
	// ]players - the command itself, skip
	ETClientConsole::getInstance().getLastLine();
	ETPlayer empty;
	empty.id = -1;
	empty.etproguid = L"";
	empty.nick = L"";
	empty.pbguid = L"";
	empty.side = SPECTATOR;
	empty.slacid = 0;
	int MAXPLAYERS = 64;
	int k=0;
	ETPlayers players = ETPlayers(1,empty);
	wstring line;
	if((line=ETClientConsole::getInstance().getLastLine())!=L"[skipnotify]")
	{
		LOG("ETClientInfo","playersInfo",L"cannot understand: "+line+L"(expected: [skipnotify])",LWARN);
		players.empty();
		return players;
	}
	else if((line=ETClientConsole::getInstance().getLastLine())!=L"  ID : Player                    Nudge  Rate  MaxPkts  Snaps")
	{
		LOG("ETClientInfo","playersInfo",L"cannot understand: "+line+L"(expected: ID : Player...)",LWARN);
		players.empty();
		return players;
	}
	else if((line=ETClientConsole::getInstance().getLastLine())!=L"[skipnotify]------------------------------------------------------------")
	{
		LOG("ETClientInfo","playersInfo",L"cannot understand: "+line+L"(expected: [skipnotify]------...",LWARN);
		players.empty();
		return players;
	}
	while( (line=ETClientConsole::getInstance().getLastLine()) != L"[skipnotify]" && k++!=MAXPLAYERS)
	{
		wstring skipNotify = L"[skipnotify]";
		if(line.find(skipNotify, 0) != line.npos)
		{
			line = line.substr(skipNotify.length());
		}
		array info = wsplit(line,L" ");
		int id;
		std::wistringstream stream(info[1]);
		stream >> id;
		if(info[1][0] != L'0' &&  id==0 )
		{
			LOG("ETClientInfo","playersInfo",L"unparseable id: "+line,LWARN);
			players.empty();
			return players;
		}
		players[id].id = id;
		players[id].nick = rtrim(line.substr(7,26));
		players[id].side = (info[0][0]==L'L')?ALLIES:((info[0][0]==L'X')?AXIS:SPECTATOR);
	}
	if(k==MAXPLAYERS)
	{
		LOG("ETClientInfo","playersInfo",(std::string)"no ending delimiter after "+MAXPLAYERS+" lines!",LWARN);
		players.empty();
		return players;
	}
	//////////////////////////////////////////
	// SLAC - SL_PLAYERS//////////////////////
	//////////////////////////////////////////
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"sl_players");
	Sleep(this->waitingForETCommandTime);
	// ]sl_players - skip
	ETClientConsole::getInstance().getLastLine();
	if(ETClientConsole::getInstance().getLastLine()!=L"cnum slac guid name")
	{
		players.empty();
		return players;
	}
	else if(ETClientConsole::getInstance().getLastLine()!=L"---- --------- ----")
	{
		players.empty();
		return players;
	}
	k=0;
	while( (line=ETClientConsole::getInstance().getLastLine()) != L"---- --------- ----" && k++!=MAXPLAYERS)
	{
		array info = wsplit(line,L" ");
		int id;
		std::wistringstream idStream(info[0]);
		idStream >> id;
		if( (info[1][0] != L'0' || info[1][1] != L'0') && id==0 )
		{
			players.empty();
			return players;
		}
		if(players[id].id==-1)
		{
			players.empty();
			return players;
		}
		int slacid;
		std::wistringstream slacidStream(info[1]);
		slacidStream >> slacid;
		if(slacid==0)
		{
			players.empty();
			return players;
		}
		players[id].slacid = slacid;
		//wstring nick = rtrim(line.substr(15));
	}
	if(k==MAXPLAYERS)
	{
		players.empty();
		return players;
	}
	//////////////////////////////////////////
	//ETPRO - GUIDS///////////////////////////
	//////////////////////////////////////////
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"guids");
	Sleep(this->waitingForETCommandTime);
	// ]cheaters - skip
	ETClientConsole::getInstance().getLastLine();
	if(ETClientConsole::getInstance().getLastLine()!=L"[skipnotify]===Player GUID List===")
	{
		players.empty();
		return players;
	}
	k=0;
	while( (line=ETClientConsole::getInstance().getLastLine()) != L"[skipnotify]---Player GUID List---" && k++!=MAXPLAYERS)
	{
		wstring skipNotify = L"[skipnotify]";
		if(line.find(skipNotify, 0) != line.npos)
		{
			line = line.substr(skipNotify.length());
		}
		array info = wsplit(line,L" ");
		int id;
		std::wistringstream stream(info[1]);
		stream >> id;
		wstring guid = info[info.size()-1];
		guid.erase(guid.end()-1);
		guid.erase(guid.begin());
		players[id].etproguid = guid;
	}
	//////////////////////////////////////////
	//PB - PB_PLIST///////////////////////////
	//////////////////////////////////////////
	return players;
}

ETClientStatus ETClientInfo::getStatus()
{
	ETClientStatus data;
	data.isPlaying = this->isPlaying();
	if(data.isPlaying)
	{
		LOG("ETClientInfo","getStatus","client is on a server, fetching data",LDBG);
		data.players = this->playersInfo();
		data.client.nick = this->localPlayerName();
	}
	else
	{
		LOG("ETClientInfo","getStatus","client not on a server",LNOTE);
	}
	return data;
}

bool ETClientInfo::isPlaying()
{
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"players");
	Sleep(this->waitingForETCommandTime);
	// ]players - the command itself, skip
	ETClientConsole::getInstance().getLastLine();
	bool returnResult = false;
	if(ETClientConsole::getInstance().getLastLine()==L"[skipnotify]")
	{
		returnResult=true;
	}
	ETClientConsole::getInstance().moveToTheEnd();
	return returnResult;
}
