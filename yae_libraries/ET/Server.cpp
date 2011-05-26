/*
 * ET/Server.cpp
 *
 *  Created on: 2011-05-21
 *      Author: trakos
 */

#include<ET/Server.h>
#include<Tnet/UDP/Client.h>
#include<Tlogger/Front.h>
#include<utils.h>

#define ET_SERVER_MAXBUFFER 5000

ET_Server::ET_Server(std::string _ip, unsigned int _port) : connection(new Tnet_UDP_Client(_ip,_port)),ip(_ip),port(_port)
{

}
ET_Status ET_Server::getPublicStatus(unsigned int timeout)
{
	ET_Status status;
	status.online = false;
	status.server.ip = this->getNumericIp();
	status.server.port = this->getPort();

	this->connection->simpleSend("\xff\xff\xff\xff""getstatus\n");
	std::string returnBuffer = this->connection->simpleReceive(ET_SERVER_MAXBUFFER, timeout, false);
	if (!returnBuffer.size())
	{
		LOG("no data received, timeout", LNOTE);
		return status;
	}
	std::vector<std::string> linesArray = split(returnBuffer, "\n");
	if (linesArray[0] != "\xff\xff\xff\xffstatusResponse")
	{
		LOG((std::string)"0th element should be \xff\xff\xff\xffstatusResponse, got: " + linesArray[0], LWARN);
		return status;
	}
	if (linesArray.size() < 2)
	{
		LOG("expected at least 2 elements after split, got: " + linesArray.size(), LWARN);
		return status;
	}
	std::vector<std::string> dataLine = split(linesArray[1], "\\");
	std::map<std::string, std::string> dataMap;
	for (unsigned int i = 0; i + 1 < dataLine.size(); i += 2)
	{
		dataMap[dataLine[i]] = dataLine[i + 1];
	}
	status.online = true;
	status.server.maxClients = ::stoi(dataMap["sv_maxclients"]);
	status.server.etpro = (dataMap["gamename"] == "etpro");
	status.server.gametype = ::stoi(dataMap["gametype"]);
	status.server.map = dataMap["mapname"];
	status.server.mod = dataMap["gamename"];
	status.server.name = dataMap["sv_hostname"];
	status.server.punkbuster = dataMap["punkbuster"] == "1" ? 1 : 0;
	status.server.needPass = 1;
	status.server.slac = ( dataMap["sl_sv_version"] != "" );
	std::cout << linesArray << std::endl;
	for ( unsigned int i = 2; i < linesArray.size(); i++ )
	{
		std::vector<std::string> playerData = split(linesArray[i]," ",2);
		int xp = ::stoi(playerData[0]);
		int ping = ::stoi(playerData[1]);
		int id = i-2;
		if ( id < 0 || id >= ET_STATUS_MAXPLAYERS )
		{
			LOG("id too high or too low: "+playerData[0]+"("+itos(id)+")", LWARN);
			throw new ET_Server_Exception("id too high or too low: "+playerData[0]+"("+itos(id)+")");
		}
		status.players[id].id = id;
		status.players[id].nick = playerData[2].substr(1,playerData[2].size()-2);
	}

	return status;
}

unsigned int ET_Server::getNumericIp()
{
	return this->connection->getNumericIp();
}

std::string ET_Server::getIp()
{
	return this->ip;
}

unsigned int ET_Server::getPort()
{
	return this->port;
}

std::string ET_Server::simplifyNick(std::string name)
{
	std::string colorless = ET_Server::colorless(name).substr(0,10);
	std::transform(colorless.begin(), colorless.end(), colorless.begin(), tolower);
	return colorless;
}

std::string ET_Server::colorless(std::string name)
{
	for (unsigned int i = 0; i < name.size(); i++)
	{
		if (name[i] == '^' && name[i + 1] != '^')
		{
			name.erase(i,2);
			i--;
		}
	}
	return name;
}
