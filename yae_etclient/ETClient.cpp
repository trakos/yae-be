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
			if ( this->iteration == 1 )
			{
				Communication_Yae_Server::getInstance().onlineWithoutET();
			}
			else if ( this->iteration == 120 )
			{
				this->iteration = 0;
			}
			if ( this->iteration%5 == 0 )
			{
				ETClientConsole::getInstance().attachConsole();
				fflush(stdout);
			}
			Sleep(1000);
			this->iteration++;
		}
		ETClientConsole::getInstance().moveToTheEnd();
		this->iteration = 0;
		while(ETClientConsole::getInstance().isConsoleAttached())
		{
			if ( this->iteration == 1 )
			{
				this->onlineWithET();
			}
			else if ( this->iteration == 120 )
			{
				this->iteration = 0;
			}
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
			Sleep(1000);
			this->iteration++;
		}
	}
	return 1;
}

void ETClient::printServerStatus()
{
	ETClientStatus status = ETClientInfo::getInstance().getStatus(true);
	std::cout << status;
}
