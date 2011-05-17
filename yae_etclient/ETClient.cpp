/*
 * EtClient.cpp
 *
 *  Created on: 2011-05-17
 *      Author: trakos
 */

#include <ETClient.h>
#include <Tnet/Message.h>
#include <ETClient/Info.h>
#include <ETClient/Input.h>
#include <ETClient/Console.h>
#include <ETClient/Listener.h>
#include <Communication/Yae/Server.h>

#include <fstream>
#include <iostream>
#include <string>

ETClient ETClient::instance = ETClient();

ETClient::ETClient()
{
	this->iteration = 0;
}

ETClient& ETClient::getInstance()
{
	return ETClient::instance;
}

void ETClient::onlineWithoutET()
{
	Communication_Yae_Server::getInstance().onlineWithoutET();
}

void ETClient::onlineWithET()
{
	ETClientStatus status = ETClientInfo::getInstance().getStatus(false);
	Communication_Yae_Server::getInstance().onlineWithET(status);
}

void ETClient::performYaeSearch()
{
	try
	{
		ETClientStatus status = ETClientInfo::getInstance().getStatus(true);
		if ( !status.isPlaying )
		{
			ETClientInput::getInstance().shortMessage("not on a server! or is it a bug? let me know!");
		}
		else
		{
			ETClientInput::getInstance().shortMessage("sending search request...");
			Tnet_Message searchResults = Communication_Yae_Server::getInstance().performYaeSearch(status);
			ETClientInput::getInstance().shortMessage("results:");
			for(int i=0;i<status.players.size();i++)
			{
				ETPlayer& player = status.players[i];
				if ( player.id != -1 )
				{
					ETClientInput::getInstance().shortMessage("player "+player.nick+" results: [" + searchResults.strings[""+player.id] + '^' + ETClientInput::color + ']' );
				}
			}
		}
	}
	catch ( Communication_Yae_Exception exception )
	{
		ETClientInput::getInstance().shortMessage("unfortunately we couldn't fetch search results from YAE master server right now! Try again?");
	}
}

int ETClient::mainLoop()
{
	while( 1 )
	{
		this->iteration = 0;
		while(!ETClientConsole::getInstance().isConsoleAttached())
		{
			if ( this->iteration == 5 )
			{
				Communication_Yae_Server::getInstance().onlineWithoutET();
			}
			else if ( this->iteration == 120 )
			{
				this->iteration = 0;
			}
			ETClientConsole::getInstance().attachConsole();
			fflush(stdout);
			Sleep(1000);
			this->iteration++;
		}
		ETClientConsole::getInstance().moveToTheEnd();
		this->iteration = 0;
		while(ETClientConsole::getInstance().isConsoleAttached())
		{
			std::string str = ETClientListener::getInstance().getCommand();
			if(str!="")
			{
				if ( str == "yae" )
				{
					this->performYaeSearch();
				}
				else
				{
					std::cout << '"' << str << '"' << std::endl;
				}
			}
		}
	}
	return 1;
}

void ETClient::printServerStatus()
{
	ETClientStatus status = ETClientInfo::getInstance().getStatus(true);
	if(!status.isPlaying)
	{
		std::cout << "not on a server" << std::endl;
	}
	else
	{
		for(int i=0;i<status.players.size();i++)
		{
			if ( status.players[i].id == -1 )
			{
				continue;
			}
			std::cout << status.players[i].id << ":" << std::endl;
			if ( status.players[i].id == status.client.id )
			{
				std::cout << "\tPLAYER" << std::endl;
			}
			std::cout << "\tnick:" << status.players[i].nick << std::endl;
			std::cout << "\tetpro:" << status.players[i].etproguid << std::endl;
			std::cout << "\tpbguid:" << status.players[i].pbguid << std::endl;
			std::cout << "\tslacid:" << status.players[i].slacid << std::endl;
			std::cout << "\tside:" << (status.players[i].side==SPECTATOR?"spectator":(status.players[i].side==AXIS?"axis":"allies")) << std::endl;
		}
		std::cout << "server data" << std::endl;
		std::cout << "\tetpro:" << ( status.server.etpro ? "yes" : "no" ) << std::endl;
		std::cout << "\tgametype:" << status.server.gametype << std::endl;
		std::cout << "\tip:" << status.server.ip << std::endl;
		std::cout << "\tmap:" << status.server.map << std::endl;
		std::cout << "\tmaxClients:" << status.server.maxClients << std::endl;
		std::cout << "\tmod:" << status.server.mod << std::endl;
		std::cout << "\tname:" << status.server.name << std::endl;
		std::cout << "\tpassword:" << status.server.password << std::endl;
		std::cout << "\tport:" << status.server.port << std::endl;
		std::cout << "\tpunkbuster:" << ( status.server.punkbuster ? "yes" : "no" ) << std::endl;
		std::cout << "\tslac:" << ( status.server.slac ? "yes" : "no" ) << std::endl;
	}
}
