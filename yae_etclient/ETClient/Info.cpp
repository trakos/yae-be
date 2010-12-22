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

ETClientInfo::ETClientInfo(): waitingForETCommandTime(300), pauseAfterCommand(700)
{}

ETClientInfo& ETClientInfo::getInstance()
{
	return ETClientInfo::instance;
}

std::wstring ETClientInfo::localPlayerName()
{
	if( !ETClientConsole::getInstance().isConsoleAttached( ))
	{
		LOG("not even attached to ET",LNOTE);
		return L"";
	}
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
			LOG(L"got name: "+line,LDBG);
			// truncate to the length of nick shown in the other commands
			return rtrim(line.substr(0,26));
		}
	}
	LOG(L"cannot understand: "+line,LWARN);
	return L"";
}

#include <iostream>
ETPlayers ETClientInfo::playersInfo(bool& success,bool& online,bool& slac,bool& etpro,bool& pb)
{
	success = false;
	online = false;
	slac = false;
	etpro = false;
	pb = false;
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
	if( !ETClientConsole::getInstance().isConsoleAttached( ))
	{
		LOG("not even attached to ET",LNOTE);
		Sleep(this->pauseAfterCommand);
		success = true;
		return players;
	}
	//////////////////////////////////////////
	// PLAYERS////////////////////////////////
	//////////////////////////////////////////
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"players");
	Sleep(this->waitingForETCommandTime);
	// ]players - the command itself, skip
	ETClientConsole::getInstance().getLastLine();
	wstring line=ETClientConsole::getInstance().getLastLine();
	if(line==L"Unknown command \"players\"")
	{
		LOG(L"unknown command /players, not on a server",LSDBG);
		Sleep(this->pauseAfterCommand);
		success = true;
		return players;
	}
	else if(line!=L"[skipnotify]")
	{
		LOG(L"cannot understand: '"+line+L"'(expected: [skipnotify])",LWARN);
		Sleep(this->pauseAfterCommand);
		return players;
	}
	else if((line=ETClientConsole::getInstance().getLastLine())!=L" ID : Player                    Nudge  Rate  MaxPkts  Snaps" && line !=L"  ID : Player                    Nudge  Rate  MaxPkts  Snaps" )
	{
		LOG(L"cannot understand: '"+line+L"'(expected: ID : Player...)",LWARN);
		Sleep(this->pauseAfterCommand);
		return players;
	}
	int movedBy = 0;
	if(line == L" ID : Player                    Nudge  Rate  MaxPkts  Snaps")
	{
		movedBy = 1;
	}
	if((line=ETClientConsole::getInstance().getLastLine())!=L"[skipnotify]-----------------------------------------------------------" && line!=L"[skipnotify]------------------------------------------------------------")
	{
		LOG(L"cannot understand: '"+line+L"'(expected: [skipnotify]------...",LWARN);
		Sleep(this->pauseAfterCommand);
		return players;
	}
	online = true;
	while( (line=ETClientConsole::getInstance().getLastLine()) != L"[skipnotify]" && k++!=MAXPLAYERS)
	{
		wstring skipNotify = L"[skipnotify]";
		if(line.find(skipNotify, 0) != line.npos)
		{
			line = line.substr(skipNotify.length());
		}
		array info = wsplit(line,L" ");
		if( info[1] == L":" )
		{
			info.insert(info.begin(),L"S");
		}
		if(info.size()<4 || info[0].size()<1)
		{
			LOG(L"too few splitted elements or too short id at :"+line,LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
		int id;
		std::wistringstream stream(info[0]);
		stream >> id;
		if(info[1][0] != L'0' &&  id==0 )
		{
			LOG(L"unparseable id: '"+info[0]+L"' at:"+line,LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
		players[id].id = id;
		players[id].nick = rtrim(line.substr(7-movedBy,26-movedBy));
		players[id].side = (info[0][0]==L'L')?ALLIES:((info[0][0]==L'X')?AXIS:SPECTATOR);
	}
	if(k==MAXPLAYERS)
	{
		LOG((std::string)"no ending delimiter after "+MAXPLAYERS+" lines for /players!",LWARN);
		Sleep(this->pauseAfterCommand);
		return players;
	}
	Sleep(this->pauseAfterCommand);
	//////////////////////////////////////////
	// SLAC - SL_PLAYERS//////////////////////
	//////////////////////////////////////////
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"sl_players");
	Sleep(this->waitingForETCommandTime);
	// ]sl_players - skip
	ETClientConsole::getInstance().getLastLine();
	line=ETClientConsole::getInstance().getLastLine();
	if(line==L"[skipnotify]unknown cmd sl_players")
	{
		LOG(L"no slac",LNOTE);
	}
	else
	{
		slac = true;
		if(line!=L"cnum slac guid name")
		{
			LOG(L"cannot understand: '"+line+L"'(expected: cnum slac guid name)",LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
		else if((line=ETClientConsole::getInstance().getLastLine())!=L"---- --------- ----")
		{
			LOG(L"cannot understand: '"+line+L"'(expected slac start: ----...)",LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
		k=0;
		while( (line=ETClientConsole::getInstance().getLastLine()) != L"---- --------- ----" && k++!=MAXPLAYERS)
		{
			array info = wsplit(line,L" ");
			if(info.size()<3)
			{
				LOG(L"too few splitted elements at :"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			int id;
			std::wistringstream idStream(info[0]);
			idStream >> id;
			if(info[1].size()<2 || info[1].size()<2)
			{
				LOG(L"too few splitted ID elements or too short ID at :"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			if( (info[1][0] != L'0' || info[1][1] != L'0') && id==0 )
			{
				LOG(L"unparseable id: '"+info[0]+L"' at:"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			if(players[id].id==-1)
			{
				LOG(L"unexpected id (not existing at /players): '"+info[0]+L"' at:"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			int slacid;
			std::wistringstream slacidStream(info[1]);
			slacidStream >> slacid;
			if(slacid==0)
			{
				Sleep(this->pauseAfterCommand);
				return players;
			}
			players[id].slacid = slacid;
			//wstring nick = rtrim(line.substr(15));
		}
		if(k==MAXPLAYERS)
		{
			LOG((std::string)"no ending delimiter after "+MAXPLAYERS+" lines for /sl_players!",LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
	}
	Sleep(this->pauseAfterCommand);
	//////////////////////////////////////////
	//ETPRO - GUIDS///////////////////////////
	//////////////////////////////////////////
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"guids");
	Sleep(this->waitingForETCommandTime);
	// ]guids - skip
	ETClientConsole::getInstance().getLastLine();
	line = ETClientConsole::getInstance().getLastLine();
	if(line==L"[skipnotify]unknown cmd guids")
	{
		LOG(L"no etpro",LNOTE);
	}
	else
	{
		etpro = true;
		if(line!=L"[skipnotify]===Player GUID List===")
		{
			LOG(L"cannot understand: '"+line+L"'(expected [skipnotify]===Player GUID List===)",LWARN);
			Sleep(this->pauseAfterCommand);
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
			if(info.size()<3 || info[1].size()<1)
			{
				LOG(L"too few splitted elements or id too short at :"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			int id;
			std::wistringstream stream(info[0]);
			stream >> id;
			if(info[0][0] != L'0' && id==0)
			{
				LOG(L"unparseable id: '"+info[0]+L"' at:"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			if(players[id].id==-1)
			{
				LOG(L"unexpected id (not existing at /players): '"+info[0]+L"' at:"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			wstring guid = info[info.size()-1];
			guid.erase(guid.end()-1);
			guid.erase(guid.begin());
			players[id].etproguid = guid;
		}
		if(k==MAXPLAYERS)
		{
			LOG((std::string)"no ending delimiter after "+MAXPLAYERS+" lines for /guids!",LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
	}
	Sleep(this->pauseAfterCommand);
	//////////////////////////////////////////
	//PB - PB_PLIST///////////////////////////
	//////////////////////////////////////////
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"pb_MsgPrefix");
	Sleep(this->waitingForETCommandTime);
	// ]pb_MsgPrefix - skip
	ETClientConsole::getInstance().getLastLine();
	line=ETClientConsole::getInstance().getLastLine();
	if( line == L"" )
	{
		LOG(L"pb timeouted, assuming it's off",LWARN);
	}
	else
	{
		pb = true;
		array info = wsplit(line,L"=",1);
		if( info.size() != 2 )
		{
			LOG(L"no '=' char in pb_MsgPrefix line:'"+line+L"'",LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
		// skip the prefixing space (there was a ' = ' in a splitted text) but add 2 - space for ': ' in each message
		int prefixLength = info[1].size()-1+2;
		LOG((wstring)L"pb_MsgPrefix length: "+prefixLength+L" in '"+line+L"'",LSDBG);
		Sleep(this->pauseAfterCommand);
		ETClientConsole::getInstance().moveToTheEnd();
		ETClientConsole::getInstance().sendMessage(L"pb_plist");
		Sleep(this->waitingForETCommandTime);
		// ]pb_plist - skip
		ETClientConsole::getInstance().getLastLine();
		line = ETClientConsole::getInstance().getLastLine();
		if( line.size() < prefixLength)
		{
			LOG((wstring)L"line too short for pb prefix "+prefixLength+L": '"+line+L"'",LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
		line = line.substr(prefixLength);
		if ( line != L"Player List: [Slot #] [GUID] [Status] [Auth Rate] [Recent SS] [Name]" )
		{
			LOG(L"line '"+line+L"', expected: 'Player List: [Slot #]  [...'",LWARN);
		}
		while( (line=ETClientConsole::getInstance().getLastLine()) != L"" && line.find(L"End of Player List", 0) == line.npos)
		{
			if( line.size() < prefixLength)
			{
				LOG((wstring)L"line too short for pb prefix "+prefixLength+L": '"+line+L"'",LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			line = line.substr(prefixLength);
			array info = wsplit(line,L" ");
			if(info.size()<3)
			{
				LOG(L"too few splitted elements at :"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			int id;
			std::wistringstream stream(info[0]);
			stream >> id;
			if(id==0)
			{
				LOG(L"unparseable id: '"+info[0]+L"' at:"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			id--;
			if(players[id].id==-1)
			{
				LOG(L"unexpected id (not existing at /players): '"+info[0]+L"' at:"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			info = wsplit(info[1],L"(");
			if(info.size()!=2)
			{
				LOG(L"expected pbguid in '"+info[0]+L"' at:"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			players[id].pbguid = info[0];
		}
		if(k==MAXPLAYERS)
		{
			LOG((std::string)"no ending delimiter after "+MAXPLAYERS+" lines for /pb_plist!",LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
	}
	Sleep(this->pauseAfterCommand);
	success = true;
	return players;
}

ETServer ETClientInfo::serverInfo()
{
	ETServer server;
	server.slac = false;
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"serverstatus");
	Sleep(this->waitingForETCommandTime);
	// ]serverstatus - skip
	std::wstring line=ETClientConsole::getInstance().getLastLine();
	int tmp;
	bool success;
	while( (line=ETClientConsole::getInstance().getLastLine()) != L"" )
	{
		if( line.size() < 25 )
		{
			continue;
		}
		std::wstring variableName = line.substr(0,24);
		std::wstring value = line.substr(24);
		if( variableName == L"g_gametype              " )
		{
			std::wistringstream stream(value);
			stream >> tmp;
			server.gametype = tmp;
		}
		else if( variableName == L"sv_maxclients           " )
		{
			std::wistringstream stream(value);
			stream >> tmp;
			server.maxClients = tmp;
		}
		else if( variableName == L"mapname                 " )
		{
			server.map = value;
		}
		else if( variableName == L"gamename                " )
		{
			server.mod = value;
			server.etpro = ( value == L"etpro" );
		}
		else if( variableName == L"sv_hostname             " )
		{
			server.name = value;
		}
		else if ( variableName == L"sl_sv_version           ")
		{
			server.slac = true;
		}
	}
	Sleep(this->pauseAfterCommand);
	line = this->getVariableValue(L"cl_currentServerIP",success);
	array ipport = wsplit(line,L":");
	if(ipport.size() != 2)
	{
		LOG(L"cannot parse cl_currentServerIP ip: "+line,LWARN);
		server.ip = server.port = 0;
	}
	else
	{
		std::wistringstream stream(ipport[1]);
		stream >> tmp;
		server.port = tmp;
		std::string iptext = wtoa(ipport[0]);
		tmp = inet_addr(iptext.c_str());
		if( tmp == INADDR_NONE || tmp == INADDR_ANY )
		{
			LOG(L"cannot convert cl_currentServerIP ip to int: "+line,LWARN);
			server.ip = 0;
		}
		server.ip = tmp;
	}
	line = this->getVariableValue(L"password",success);
	server.password = line;
	// PB
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"pb_MsgPrefix");
	Sleep(this->waitingForETCommandTime);
	// ]pb_MsgPrefix - skip
	ETClientConsole::getInstance().getLastLine();
	line=ETClientConsole::getInstance().getLastLine();
	server.punkbuster = false;
	if( line != L"" )
	{
		array info = wsplit(line,L"=",1);
		if( info.size() == 2 )
		{
			server.punkbuster = true;
		}
	}
	Sleep(this->pauseAfterCommand);
	return server;
}

std::wstring ETClientInfo::getVariableValue(std::wstring name, bool& success)
{
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(name);
	Sleep(this->waitingForETCommandTime);
	ETClientConsole::getInstance().getLastLine();
	wstring line=ETClientConsole::getInstance().getLastLine();
	wstring start = L"\""+name+L"\" is:\"";
	wstring end = L"\" default:\"";
	success = false;
	if(line.find(start, 0) == 0)
	{
		line = line.substr(start.length());
		array info = wsplit(line,L"\"");
		if(info.size()!=3 && info.size()!=2)
		{
			LOG(L"cannot find value of '"+name+L"' (no default): "+line,LWARN);
			Sleep(this->pauseAfterCommand);
			return L"";
		}
		Sleep(this->pauseAfterCommand);
		success = true;
		return info[0];
	}
	LOG(L"cannot find value of '"+name+L"': "+line,LWARN);
	Sleep(this->pauseAfterCommand);
	return L"";
}

ETClientStatus ETClientInfo::getStatus()
{
	LOG("",LSDBG);
	ETClientStatus data;
	//data.isPlaying = this->isPlaying();
	bool success,online,slac,etpro,pb;
	data.players = this->playersInfo(success,online,slac,etpro,pb);
	data.isPlaying = success && online;
	if(!data.isPlaying)
	{
		LOG("client not on a server",LNOTE);
	}
	else
	{
		for(int i=0;i<data.players.size();i++)
		{
			std::cout << data.players[i].id << ":" << std::endl;
			std::wcout << L"\tnick:" << data.players[i].nick << std::endl;
			std::wcout << L"\tetpro:" << data.players[i].etproguid << std::endl;
			std::wcout << L"\tpbguid:" << data.players[i].pbguid << std::endl;
			std::wcout << L"\tslacid:" << data.players[i].slacid << std::endl;
		}
		data.client.nick = this->localPlayerName();
		for(int i=0;i<data.players.size();i++)
		{
			if( data.players[i].nick == data.client.nick )
			{
				data.client = data.players[i];
			}
			std::cout << "PLAYER ID:" << data.client.id << std::endl;
		}
		data.server = this->serverInfo();
		std::cout << "server data" << std::endl;
		std::wcout << L"\tetpro:" << ( data.server.etpro ? L"yes" : L"no" ) << std::endl;
		std::wcout << L"\tgametype:" << data.server.gametype << std::endl;
		std::wcout << L"\tip:" << data.server.ip << std::endl;
		std::wcout << L"\tmap:" << data.server.map << std::endl;
		std::wcout << L"\tmaxClients:" << data.server.maxClients << std::endl;
		std::wcout << L"\tmod:" << data.server.mod << std::endl;
		std::wcout << L"\tname:" << data.server.name << std::endl;
		std::wcout << L"\tpassword:" << data.server.password << std::endl;
		std::wcout << L"\tport:" << data.server.port << std::endl;
		std::wcout << L"\tpunkbuster:" << ( data.server.punkbuster ? L"yes" : L"no" ) << std::endl;
		std::wcout << L"\tslac:" << ( data.server.slac ? L"yes" : L"no" ) << std::endl;
	}
	return data;
}

bool ETClientInfo::isPlaying()
{
	if( !ETClientConsole::getInstance().isConsoleAttached( ))
	{
		return false;
	}
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"players");
	Sleep(this->waitingForETCommandTime);
	// ]players - the command itself, skip
	ETClientConsole::getInstance().getLastLine();
	bool returnResult = false;
	wstring line;
	if((line=ETClientConsole::getInstance().getLastLine())==L"[skipnotify]")
	{
		LOG("client is on a server, /players ok",LSDBG);
		returnResult=true;
	}
	else
	{
		LOG(L"client offline, /players: "+line,LNOTE);
	}
	ETClientConsole::getInstance().moveToTheEnd();
	Sleep(this->pauseAfterCommand);
	return returnResult;
}
