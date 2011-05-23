/*
 * EtClient.cpp
 *
 *  Created on: 2011-05-17
 *      Author: trakos
 */

#include <ETClient.h>
#include <Tnet/Message.h>
#include <ET/Client/Info.h>
#include <ET/Client/Input.h>
#include <ET/Client/Console.h>
#include <ET/Client/Listener.h>
#include <Communication/Yae/Master.h>
#include <Communication/Yae/Authorization.h>
#include <Tlogger/Front.h>

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
	Communication_Yae_Master::getInstance().onlineWithoutET();
}

void ETClient::onlineWithET()
{
	ET_Status status = ET_Client_Info::getInstance().getStatus(false);
	if ( !status.online )
	{
		LOG("client with ET, not on a server", LNOTE);
	}
	Communication_Yae_Master::getInstance().onlineWithET(status);
}

void ETClient::forceAuth()
{
	Communication_Yae_Authorization::getInstance().getCurrentCredentials();
}

void ETClient::performYaeSearch()
{
	try
	{
		ET_Status status = ET_Client_Info::getInstance().getStatus(true);
		if ( !status.online )
		{
			ET_Client_Input::getInstance().shortMessage("You are not on a server! It might be a bug...");
		}
		else
		{
			ET_Client_Input::getInstance().shortMessage("Sending search request...");
			Tnet_Message searchResults = Communication_Yae_Master::getInstance().performYaeSearch(status);
			ET_Client_Input::getInstance().shortMessage("Results:");
			for(int i=0;i<status.players.size();i++)
			{
				ET_Status_Player& player = status.players[i];
				if ( player.id != -1 )
				{
					if ( !searchResults.strings.count(itos(player.id)) )
					{
						LOG((std::string)"No results at all returned for id: "+player.id+"!", LERR);
					}
					else
					{
						std::vector<std::string> splitted = split(searchResults.strings[itos(player.id)],"\n");
						ET_Client_Input::getInstance().shortMessage("Player "+player.nick+" results: "+splitted[0]);
						for (unsigned int i=1; i<splitted.size(); i++)
						{
							ET_Client_Input::getInstance().shortMessage("      "+splitted[i]);
						}
					}
				}
			}
		}
	}
	catch ( Communication_Yae_Exception exception )
	{
		ET_Client_Input::getInstance().shortMessage("Unfortunately we couldn't fetch search results from YAE master server! Try again later?");
	}
}

int ETClient::mainLoop()
{
	while( 1 )
	{
		this->iteration = 0;
		LOG("entering ET offline loop...", LNOTE);
		while(!ET_Client_Console::getInstance().isConsoleAttached())
		{
			if ( this->iteration == 1 )
			{
				Communication_Yae_Master::getInstance().onlineWithoutET();
			}
			else if ( this->iteration == 120 )
			{
				this->iteration = 0;
			}
			if ( this->iteration%5 == 0 )
			{
				ET_Client_Console::getInstance().attachConsole();
				fflush(stdout);
			}
			Sleep(1000);
			this->iteration++;
		}
		ET_Client_Console::getInstance().moveToTheEnd();
		this->iteration = 0;
		LOG("entering ET online loop...", LNOTE);
		while(ET_Client_Console::getInstance().isConsoleAttached())
		{
			if ( this->iteration == 1 )
			{
				this->onlineWithET();
			}
			else if ( this->iteration == 120 )
			{
				this->iteration = 0;
			}
			std::string str = ET_Client_Listener::getInstance().getCommand();
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
	ET_Status status = ET_Client_Info::getInstance().getStatus(true);
	std::cout << status;
}
