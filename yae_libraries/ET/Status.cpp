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
			stream << "\t" << "[" << status.players[i].id << "]" << " " << "=>" << "\t";
			stream << indent << status.players[i] << std::endl << unindent;
		}
		stream << "}";
		stream << std::endl << unindent;
		stream << "\t" << "[client]" << " " << "=>" << "\t" << indent;
		stream << status.client;
		stream << std::endl << unindent;
		stream << "\t" << "[server]" << " " << "=>" << "\t" << indent;
		stream << status.server;
		stream << std::endl << unindent;
	}
	stream << "}";
	return stream;
}

std::ostream& operator <<(std::ostream& stream, ET_Status_Player const &player)
{
	stream << "ET_Status_Player" << std::endl;
	stream << "{" << std::endl;
	stream << "\t" << "[id]" << " " << "=>" << "\t" << player.id << std::endl;
	stream << "\t" << "[nick]" << " " << "=>" << "\t" << player.nick << std::endl;
	stream << "\t" << "[etpro]" << " " << "=>" << "\t" << player.etproguid << std::endl;
	stream << "\t" << "[pbguid]" << " " << "=>" << "\t" << player.pbguid << std::endl;
	stream << "\t" << "[slacid]" << " " << "=>" << "\t" << player.slacid << std::endl;
	stream << "\t" << "[side]" << " " << "=>" << "\t" << (player.side==SPECTATOR?"spectator":(player.side==AXIS?"axis":"allies")) << std::endl;
	stream << "}";
	return stream;
}

std::ostream& operator <<(std::ostream& stream, ET_Status_Server const &server)
{
	std::string ip = inet_ntoa(*(struct in_addr*)&server.ip);
	stream << "ET_Status_Server" << std::endl;
	stream << "{" << std::endl;
	stream << "\t" << "[etpro]" << " " << "=>" << "\t" << ( server.etpro ? "yes" : "no" ) << std::endl;
	stream << "\t" << "[gametype]" << " " << "=>" << "\t" << server.gametype << std::endl;
	stream << "\t" << "[ip]" << " " << "=>" << "\t" << ip << std::endl;
	stream << "\t" << "[map]" << " " << "=>" << "\t" << server.map << std::endl;
	stream << "\t" << "[maxClients]" << " " << "=>" << "\t" << server.maxClients << std::endl;
	stream << "\t" << "[mod]" << " " << "=>" << "\t" << server.mod << std::endl;
	stream << "\t" << "[name]" << " " << "=>" << "\t" << server.name << std::endl;
	stream << "\t" << "[needPass]" << " " << "=>" << "\t" << ( server.needPass ? "yes" : "no" ) << std::endl;
	stream << "\t" << "[password]" << " " << "=>" << "\t" << server.password << std::endl;
	stream << "\t" << "[port]" << " " << "=>" << "\t" << server.port << std::endl;
	stream << "\t" << "[punkbuster]" << " " << "=>" << "\t" << ( server.punkbuster ? "yes" : "no" ) << std::endl;
	stream << "\t" << "[slac]" << " " << "=>" << "\t" << ( server.slac ? "yes" : "no" ) << std::endl;
	stream << "}";
	return stream;
}
