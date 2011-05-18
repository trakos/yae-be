/*
 * ETClient/Info.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <string>
#include <iostream>

#include <ETClient/Status.h>
#include <utils.h>
#include <indent.h>
#include <Logger/Logger.h>



std::ostream& operator <<(std::ostream& stream, ETClientStatus const &status)
{
	stream << "ETClientStatus()" << std::endl;
	stream << "{" << std::endl;
	stream << "\t" << "[isPlaying]" << " " << "=>" << "\t" << (status.isPlaying?"1":"0") << std::endl;
	if( status.isPlaying )
	{
		stream << "\t" << "[players]" << " " << "=>" << "\t" << indent;
		stream << "players" << std::endl;
		stream << "{" << std::endl;
		for(int i=0;i<status.players.size();i++)
		{
			if ( status.players[i].id == -1 )
			{
				continue;
			}
			stream << "\t" << "[" << status.players[i].id << "]" << " " << "=>" << "\t" << indent;
			stream << "player" << std::endl;
			stream << "{" << std::endl;
			std::cout << status.players[i].id << ":" << std::endl;
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
		stream << "}";
		stream << std::endl << unindent;
		stream << "\t" << "[server]" << " " << "=>" << "\t" << indent;
		stream << "players" << std::endl;
		stream << "{" << std::endl;
		stream << "\t" << "[etpro]" << " " << "=>" << "\t" << ( status.server.etpro ? "yes" : "no" ) << std::endl;
		stream << "\t" << "[gametype]" << " " << "=>" << "\t" << status.server.gametype << std::endl;
		stream << "\t" << "[ip]" << " " << "=>" << "\t" << status.server.ip << std::endl;
		stream << "\t" << "[map]" << " " << "=>" << "\t" << status.server.map << std::endl;
		stream << "\t" << "[maxClients]" << " " << "=>" << "\t" << status.server.maxClients << std::endl;
		stream << "\t" << "[mod]" << " " << "=>" << "\t" << status.server.mod << std::endl;
		stream << "\t" << "[name]" << " " << "=>" << "\t" << status.server.name << std::endl;
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
