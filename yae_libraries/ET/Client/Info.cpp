/*
 * ETClient/Info.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <string>
#include <stdlib.h>
#include <sstream>
#ifdef __unix__
	#define Sleep usleep
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif

#include <ET/Client/Info.h>
#include <ET/Client/Console.h>
#include <ET/Client/Input.h>
#include <utils.h>
#include <Tlogger/Front.h>




using std::wstring;
typedef std::vector< wstring > array;

ET_Client_Info* ET_Client_Info::instance = 0;

ET_Client_Info::ET_Client_Info(): waitingForETCommandTime(300), pauseAfterCommand(700)
{}

ET_Client_Info& ET_Client_Info::getInstance()
{
	if ( !ET_Client_Info::instance )
	{
		ET_Client_Info::instance = new ET_Client_Info();
	}
	return *ET_Client_Info::instance;
}

std::wstring ET_Client_Info::localPlayerName()
{
	if( !ET_Client_Console::getInstance().isConsoleAttached( ))
	{
		LOG("not even attached to ET",LNOTE);
		return L"";
	}
	ET_Client_Console::getInstance().moveToTheEnd();
	ET_Client_Console::getInstance().sendMessage(L"name");
	Sleep(this->waitingForETCommandTime);
	// ]name - the command itself, skip
	ET_Client_Console::getInstance().getLastLine();
	wstring line = ET_Client_Console::getInstance().getLastLine();
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

ET_Status_Player_W ET_Client_Info::parsePlayersLine(std::wstring line, bool warmup, bool doubleStatus)
{
	ET_Status_Player_W player;
	unsigned int movedBy = 0;
	if ( doubleStatus )
	{
		movedBy++;
	}
	if ( warmup )
	{
		movedBy+=10;
	}
	wchar_t status = line[movedBy];
	// filling values
	player.nick = rtrim(line.substr(6+movedBy,30));
	player.id = ::stoi(wtoa(line.substr(1+movedBy,2)));
	player.side = (status==L'L')?ALLIES:((status==L'X')?AXIS:SPECTATOR);
	// empty values
	player.etproguid = L"";
	player.pbguid = L"";
	player.slacid = 0;
	return player;
}

ET_Status_Players_W ET_Client_Info::playersInfo(bool& success,bool& online,bool& slac,bool& etpro,bool& pb)
{
	success = false;
	online = false;
	slac = false;
	etpro = false;
	pb = false;
	ET_Status_Player_W empty;
	empty.id = -1;
	empty.etproguid = L"";
	empty.nick = L"";
	empty.pbguid = L"";
	empty.side = SPECTATOR;
	empty.slacid = 0;
	int MAXPLAYERS = ET_STATUS_MAXPLAYERS;
	int k=0;
	ET_Status_Players_W players = ET_Status_Players_W(MAXPLAYERS+1,empty);
	if( !ET_Client_Console::getInstance().isConsoleAttached( ))
	{
		LOG("not even attached to ET",LNOTE);
		Sleep(this->pauseAfterCommand);
		success = true;
		return players;
	}
	//////////////////////////////////////////
	// PLAYERS////////////////////////////////
	//////////////////////////////////////////
	ET_Client_Console::getInstance().moveToTheEnd();
	ET_Client_Console::getInstance().sendMessage(L"players");
	Sleep(this->waitingForETCommandTime);
	// ]players - the command itself, skip
	ET_Client_Console::getInstance().getLastLine();
	wstring line=ET_Client_Console::getInstance().getLastLine();
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
	else if((line=ET_Client_Console::getInstance().getLastLine())!=L" ID : Player                    Nudge  Rate  MaxPkts  Snaps" && line !=L"  ID : Player                    Nudge  Rate  MaxPkts  Snaps" && line != L"Status   :  ID : Player                    Nudge  Rate  MaxPkts  Snaps" )
	{
		LOG(L"cannot understand: '"+line+L"'(expected: ID : Player...)",LWARN);
		Sleep(this->pauseAfterCommand);
		return players;
	}
	bool doubleStatus = false;
	bool warmup = false;
	if(line == L"  ID : Player                    Nudge  Rate  MaxPkts  Snaps" || line == L"Status   :  ID : Player                    Nudge  Rate  MaxPkts  Snaps")
	{
		doubleStatus = true;
	}
	if ( line == L"Status   :  ID : Player                    Nudge  Rate  MaxPkts  Snaps" || line == L"Status   : ID : Player                    Nudge  Rate  MaxPkts  Snaps" )
	{
		warmup = true;
	}
	if((line=ET_Client_Console::getInstance().getLastLine())!=L"[skipnotify]-----------------------------------------------------------" && line!=L"[skipnotify]------------------------------------------------------------" && line!=L"[skipnotify]----------------------------------------------------------------------")
	{
		LOG(L"cannot understand: '"+line+L"'(expected: [skipnotify]------...",LWARN);
		Sleep(this->pauseAfterCommand);
		return players;
	}
	online = true;
	while( (line=ET_Client_Console::getInstance().getLastLine()) != L"[skipnotify]" && k++!=MAXPLAYERS)
	{
		LOG(L"player:"+line,LSDBG);
		wstring skipNotify = L"[skipnotify]";
		if(line.find(skipNotify, 0) != line.npos)
		{
			line = line.substr(skipNotify.length());
		}
		ET_Status_Player_W player = this->parsePlayersLine(line, warmup, doubleStatus);
		if ( player.id < 0 || player.id > ET_STATUS_MAXPLAYERS )
		{
			LOG((std::wstring)L"id ("+player.id+L") out of range - ignoring player from line: "+line, LWARN);
		}
		else
		{
			players[ player.id ] = player;
		}
		LOG((std::wstring)L"player "+player.id+L": '"+player.nick+L"'"+L"("+(player.side==SPECTATOR?L"spectator":(player.side==AXIS?L"axis":L"allies"))+L")", LDBG);
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
	ET_Client_Console::getInstance().moveToTheEnd();
	ET_Client_Console::getInstance().sendMessage(L"sl_players");
	Sleep(this->waitingForETCommandTime);
	// ]sl_players - skip
	ET_Client_Console::getInstance().getLastLine();
	line=ET_Client_Console::getInstance().getLastLine();
	if(line==L"[skipnotify]unknown cmd sl_players")
	{
		LOG(L"no slac",LNOTE);
	}
	else
	{
		slac = true;
		int slacFormatVersion = 0;
		if(line!=L"cnum slac guid account type   name" && line!=L"cnum slac guid name")
		{

			//LOG(L"cannot understand: '"+line+L"'(expected: cnum slac guid name)",LWARN);
			LOG(L"cannot understand: '"+line+L"'(expected: cnum slac guid account type   name)",LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
		if ( line!=L"cnum slac guid name" )
		{
			slacFormatVersion = 1;
		}
		if ( line==L"cnum slac guid account type   name" )
		{
			slacFormatVersion = 2;
		}
		wstring slacDelimiterLines = L"---- --------- ----";
		if ( slacFormatVersion == 2 )
		{
			slacDelimiterLines = L"---- --------- -------------- ----";
		}
		if( (line=ET_Client_Console::getInstance().getLastLine())!= slacDelimiterLines )
		{
			LOG(L"cannot understand: '"+line+L"'(expected slac start: ----...)",LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
		k=0;
		int slacArgumentsSize = slacFormatVersion==1 ? 3 : 4; // there can be "normal user" or "donator" in status, so it can be 5, too
		while( (line=ET_Client_Console::getInstance().getLastLine()) != slacDelimiterLines && k++!=MAXPLAYERS)
		{
			array info = wsplit(line,L" ");
			if(info.size()<slacArgumentsSize)
			{
				LOG(L"too few splitted elements at :"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			int id;
			std::wistringstream idStream(info[0]);
			idStream >> id;
			if(info[0].size()<2)
			{
				LOG(L"too few splitted ID elements or too short ID at :"+line,LWARN);
				Sleep(this->pauseAfterCommand);
				return players;
			}
			if( (info[0][0] != L'0' || info[0][1] != L'0') && id==0 )
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
	ET_Client_Console::getInstance().moveToTheEnd();
	ET_Client_Console::getInstance().sendMessage(L"guids");
	Sleep(this->waitingForETCommandTime);
	// ]guids - skip
	ET_Client_Console::getInstance().getLastLine();
	line = ET_Client_Console::getInstance().getLastLine();
	if(line==L"[skipnotify]unknown cmd guids")
	{
		LOG(L"no etpro",LNOTE);
	}
	else if (line==L"[skipnotify]ETPro guids are unreliable and should not be used")
	{
		LOG(L"slac refusing to give etpro guids",LNOTE);
		etpro = true;
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
		while( (line=ET_Client_Console::getInstance().getLastLine()) != L"[skipnotify]---Player GUID List---" && k++!=MAXPLAYERS)
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
	ET_Client_Console::getInstance().moveToTheEnd();
	ET_Client_Console::getInstance().sendMessage(L"pb_MsgPrefix");
	Sleep(this->waitingForETCommandTime);
	// ]pb_MsgPrefix - skip
	ET_Client_Console::getInstance().getLastLine();
	line=ET_Client_Console::getInstance().getLastLine();
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
		ET_Client_Console::getInstance().moveToTheEnd();
		ET_Client_Console::getInstance().sendMessage(L"pb_plist");
		Sleep(this->waitingForETCommandTime);
		// ]pb_plist - skip
		ET_Client_Console::getInstance().getLastLine();
		line = ET_Client_Console::getInstance().getLastLine();
		if ( line == L"" )
		{
			LOG(L"pb_plist timeouted even though prefix was received, assuming pb is off",LWARN);
			pb = false;
		}
		else if( line.size() < prefixLength)
		{
			LOG((wstring)L"line too short for pb prefix "+prefixLength+L": '"+line+L"'",LWARN);
			Sleep(this->pauseAfterCommand);
			return players;
		}
		else
		{
			line = line.substr(prefixLength);
			if ( line != L"Player List: [Slot #] [GUID] [Status] [Auth Rate] [Recent SS] [Name]" )
			{
				LOG(L"line '"+line+L"', expected: 'Player List: [Slot #]  [...'",LWARN);
			}
			while( (line=ET_Client_Console::getInstance().getLastLine()) != L"" && line.find(L"End of Player List", 0) == line.npos)
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
	}
	Sleep(this->pauseAfterCommand);
	success = true;
	return players;
}

ET_Status_Server_W ET_Client_Info::serverInfo()
{
	ET_Status_Server_W server;
	server.slac = false;
	ET_Client_Console::getInstance().moveToTheEnd();
	ET_Client_Console::getInstance().sendMessage(L"serverstatus");
	Sleep(this->waitingForETCommandTime);
	// ]serverstatus - skip
	std::wstring line=ET_Client_Console::getInstance().getLastLine();
	int tmp;
	bool success;
	while( (line=ET_Client_Console::getInstance().getLastLine()) != L"" )
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
		else if( variableName == L"g_needpass              " )
		{
			server.needPass = true;
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
	/*// PB
	ET_Client_Console::getInstance().moveToTheEnd();
	ET_Client_Console::getInstance().sendMessage(L"pb_MsgPrefix");
	Sleep(this->waitingForETCommandTime);
	// ]pb_MsgPrefix - skip
	ET_Client_Console::getInstance().getLastLine();
	line=ET_Client_Console::getInstance().getLastLine();
	server.punkbuster = false;
	if( line != L"" )
	{
		array info = wsplit(line,L"=",1);
		if( info.size() == 2 )
		{
			server.punkbuster = true;
		}
	}
	Sleep(this->pauseAfterCommand);*/
	return server;
}

std::wstring ET_Client_Info::getVariableValue(std::wstring name, bool& success)
{
	ET_Client_Console::getInstance().moveToTheEnd();
	ET_Client_Console::getInstance().sendMessage(name);
	Sleep(this->waitingForETCommandTime);
	ET_Client_Console::getInstance().getLastLine();
	wstring line=ET_Client_Console::getInstance().getLastLine();
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

ET_Status_W ET_Client_Info::getStatusW( bool echoProgress )
{
	LOG("",LSDBG);
	ET_Status_W data;
	//data.online = this->online();
	bool success,online,slac,etpro,pb;
	if(echoProgress)
		ET_Client_Input::getInstance().shortMessageW(L"Fetching players data...");
	data.players = this->playersInfo(success,online,slac,etpro,pb);
	data.online = success && online;
	if(!data.online)
	{
		if(echoProgress)
			ET_Client_Input::getInstance().shortMessageW(L"Players data not found... not on a server?");
		LOG("client not on a server",LNOTE);
	}
	else
	{
		if(echoProgress)
			ET_Client_Input::getInstance().shortMessageW(L"Fetching current player info...");
		data.client.nick = this->localPlayerName();
		for(int i=0;i<data.players.size();i++)
		{
			if( data.players[i].nick == data.client.nick )
			{
				data.client = data.players[i];
			}
		}
		if(echoProgress)
			ET_Client_Input::getInstance().shortMessageW(L"Fetching server info...");
		data.server = this->serverInfo();
		data.server.punkbuster = pb;
		if(echoProgress)
			ET_Client_Input::getInstance().shortMessageW(L"All done.");
	}
	return data;
}

ET_Status ET_Client_Info::getStatus( bool echoProgress )
{
	ET_Status_W dataW = this->getStatusW( echoProgress );
	ET_Status data;
	data.online = dataW.online;
	data.client = this->ETPlayerWToETPlayer(dataW.client);
	for(int i=0;i<dataW.players.size();i++)
	{
		data.players.push_back(this->ETPlayerWToETPlayer(dataW.players[i]));
	}
	data.server.etpro = dataW.server.etpro;
	data.server.gametype = dataW.server.gametype;
	data.server.ip = dataW.server.ip;
	data.server.map = wtoa(dataW.server.map);
	data.server.maxClients = dataW.server.maxClients;
	data.server.mod = wtoa(dataW.server.mod);
	data.server.name = wtoa(dataW.server.name);
	data.server.password = wtoa(dataW.server.password);
	data.server.port = dataW.server.port;
	data.server.punkbuster = dataW.server.punkbuster;
	data.server.slac = dataW.server.slac;
	return data;
}

ET_Status_Player ET_Client_Info::ETPlayerWToETPlayer(ET_Status_Player_W etplayerw)
{
	ET_Status_Player player;
	player.etproguid = wtoa(etplayerw.etproguid);
	player.id = etplayerw.id;
	player.nick = wtoa(etplayerw.nick);
	player.pbguid = wtoa(etplayerw.pbguid);
	player.side = etplayerw.side;
	player.slacid = etplayerw.slacid;
	return player;
}

bool ET_Client_Info::isPlaying()
{
	if( !ET_Client_Console::getInstance().isConsoleAttached( ))
	{
		return false;
	}
	ET_Client_Console::getInstance().moveToTheEnd();
	ET_Client_Console::getInstance().sendMessage(L"players");
	Sleep(this->waitingForETCommandTime);
	// ]players - the command itself, skip
	ET_Client_Console::getInstance().getLastLine();
	bool returnResult = false;
	wstring line;
	if((line=ET_Client_Console::getInstance().getLastLine())==L"[skipnotify]")
	{
		LOG("client is on a server, /players ok",LSDBG);
		returnResult=true;
	}
	else
	{
		LOG(L"client offline, /players: "+line,LNOTE);
	}
	ET_Client_Console::getInstance().moveToTheEnd();
	Sleep(this->pauseAfterCommand);
	return returnResult;
}
