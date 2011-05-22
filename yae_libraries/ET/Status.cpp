/*
 * ET/Status.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <string>
#include <iostream>
#ifdef __unix__
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#else
	#include <windows.h>
	#include <winsock.h>
#endif


#include <ET/Status.h>
#include <utils.h>
#include <indent.h>
#include <Tlogger/Front.h>

ET_Status::ET_Status()
{
	ET_Status_Player empty;
	empty.id = -1;
	empty.etproguid = "";
	empty.nick = "";
	empty.pbguid = "";
	empty.side = SPECTATOR;
	empty.slacid = 0;
	int k=0;
	int MAXPLAYERS = ET_STATUS_MAXPLAYERS;
	this->players = ET_Status_Players(MAXPLAYERS+1,empty);
}

ET_Status_W::ET_Status_W()
{
	ET_Status_Player_W empty;
	empty.id = -1;
	empty.etproguid = L"";
	empty.nick = L"";
	empty.pbguid = L"";
	empty.side = SPECTATOR;
	empty.slacid = 0;
	int k=0;
	int MAXPLAYERS = ET_STATUS_MAXPLAYERS;
	this->players = ET_Status_Players_W(MAXPLAYERS+1,empty);
}

std::ostream& operator <<(std::ostream& stream, ET_Status const &status)
{
	stream << "ET_Status()" << std::endl;
	stream << "{" << std::endl;
	stream << "\t" << "[online]" << " " << "=>" << "\t" << (status.online?"1":"0") << std::endl;
	if( status.online )
	{
		stream << "\t" << "[players]" << " " << "=>" << "\t" << indent;
		stream << "ET_Status_Players" << std::endl;
		stream << "{" << std::endl;
		for(int i=0;i<status.players.size();i++)
		{
			if ( status.players[i].id == -1 )
			{
				continue;
			}
			stream << "\t" << "[" << status.players[i].id << "]" << " " << "=>" << "\t" << indent;
			stream << "ET_Status_Player" << std::endl;
			stream << "{" << std::endl;
			if ( status.players[i].id == status.client.id )
			{
				std::cout << "\tPLAYER" << std::endl;
			}
			stream << "\t" << "[nick]" << " " << "=>" << "\t" << status.players[i].nick << std::endl;
			stream << "\t" << "[etpro]" << " " << "=>" << "\t" << status.players[i].etproguid << std::endl;
			stream << "\t" << "[pbguid]" << " " << "=>" << "\t" << status.players[i].pbguid << std::endl;
			stream << "\t" << "[slacid]" << " " << "=>" << "\t" << status.players[i].slacid << std::endl;
			stream << "\t" << "[side]" << " " << "=>" << "\t" << (status.players[i].side==SPECTATOR?"spectator":(status.players[i].side==AXIS?"axis":"allies")) << std::endl;
			stream << "}";
			stream << std::endl << unindent;
		}
		std::string ip = inet_ntoa(*(struct in_addr*)&status.server.ip);
		stream << "}";
		stream << std::endl << unindent;
		stream << "\t" << "[server]" << " " << "=>" << "\t" << indent;
		stream << "ET_Status_Server" << std::endl;
		stream << "{" << std::endl;
		stream << "\t" << "[etpro]" << " " << "=>" << "\t" << ( status.server.etpro ? "yes" : "no" ) << std::endl;
		stream << "\t" << "[gametype]" << " " << "=>" << "\t" << status.server.gametype << std::endl;
		stream << "\t" << "[ip]" << " " << "=>" << "\t" << ip << std::endl;
		stream << "\t" << "[map]" << " " << "=>" << "\t" << status.server.map << std::endl;
		stream << "\t" << "[maxClients]" << " " << "=>" << "\t" << status.server.maxClients << std::endl;
		stream << "\t" << "[mod]" << " " << "=>" << "\t" << status.server.mod << std::endl;
		stream << "\t" << "[name]" << " " << "=>" << "\t" << status.server.name << std::endl;
		stream << "\t" << "[needPass]" << " " << "=>" << "\t" << ( status.server.needPass ? "yes" : "no" ) << std::endl;
		stream << "\t" << "[password]" << " " << "=>" << "\t" << status.server.password << std::endl;
		stream << "\t" << "[port]" << " " << "=>" << "\t" <<status.server.port << std::endl;
		stream << "\t" << "[punkbuster]" << " " << "=>" << "\t" << ( status.server.punkbuster ? "yes" : "no" ) << std::endl;
		stream << "\t" << "[slac]" << " " << "=>" << "\t" << ( status.server.slac ? "yes" : "no" ) << std::endl;
		stream << "}";
		stream << std::endl << unindent;
	}
	stream << "}";
	return stream;
}
