/*
 * Communication/YaeServer.cpp
 *
 *  Created on: 2011-05-08
 *      Author: trakos
 */

#include <Communication/Yae/Server.h>
#include <Communication/Yae/Authorization.h>
#include <Tnet/Client.h>
#include <Logger/Logger.h>

Communication_Yae_Server Communication_Yae_Server::instance = Communication_Yae_Server();
std::string Communication_Yae_Server::masterIP = "46.4.95.216";
int Communication_Yae_Server::masterPort = 1743;
unsigned int Communication_Yae_Server::timeout = 50;

Communication_Yae_Server::Communication_Yae_Server()
{
	this->connection = 0;
}

Communication_Yae_Server::~Communication_Yae_Server()
{
	if ( this->connection )
	{
		this->disconnect();
	}
}

Communication_Yae_Server& Communication_Yae_Server::getInstance()
{
	return Communication_Yae_Server::instance;
}

void Communication_Yae_Server::connect()
{
	if ( !this->connection )
	{
		this->connection = new Tnet_Client(Communication_Yae_Server::masterIP, Communication_Yae_Server::masterPort);
	}
}

void Communication_Yae_Server::reconnect()
{
	if ( this->connection )
	{
		this->disconnect();
	}
	this->connection = new Tnet_Client(Communication_Yae_Server::masterIP, Communication_Yae_Server::masterPort);
}

void Communication_Yae_Server::disconnect()
{
	if ( this->connection )
	{
		delete this->connection;
		this->connection = 0;
	}
}

Tnet_Message Communication_Yae_Server::sendMessage(Tnet_Message message)
{
	try
	{
		this->connection->send(message);
		message = this->connection->receive(Communication_Yae_Server::timeout);
		if ( message.strings["status"] != "ok" )
		{
			this->disconnect();
			LOG((std::string)"yae message status is: "+message.strings["status"]+"; aborting...", LERR);
			Communication_Yae_Authorization::getInstance().setState( OFFLINE );
			throw Communication_Yae_Exception("yae master server reachable, but message received with state unkown, something gone wrong");
		}
		else
		{
			return message;
		}
	}
	catch (Tnet_Exception exception)
	{
		this->disconnect();
		LOG((std::string)"yae offline - exception: "+exception.what(), LWARN);
		Communication_Yae_Authorization::getInstance().setState( OFFLINE );
		throw Communication_Yae_Exception("protocol mismatch or yae master unreachable");
	}
}

Tnet_Message Communication_Yae_Server::sendAuthMessage(Tnet_Message message)
{
	try
	{
		Communication_Yae_Credentials credentials = Communication_Yae_Authorization::getInstance().getCurrentCredentials();
		if ( Communication_Yae_Authorization::getInstance().getCurrentState() == OFFLINE )
		{
			throw Communication_Yae_Exception("yae master server unreachable - client offline or downtime?");
		}
		message.strings["login"] = credentials.login;
		message.strings["password"] = credentials.password;
		this->connect();
		this->connection->send(message);
		message = this->connection->receive(Communication_Yae_Server::timeout);
		if ( message.strings["status"] != "ok" )
		{
			this->disconnect();
			LOG((std::string)"yae status is: "+message.strings["status"]+", aborting...", LERR);
			Communication_Yae_Authorization::getInstance().setState( OFFLINE );
			throw Communication_Yae_Exception("yae master server reachable, but message received with state unkown, something gone wrong");
		}
		else if ( message.strings["authStatus"] != "ok" )
		{
			this->disconnect();
			LOG((std::string)"yae authorization status is: "+message.strings["authStatus"]+", retrying...", LWARN);
			Communication_Yae_Authorization::getInstance().setState( NOCREDENTIALS );
			return this->sendAuthMessage(message);
		}
		else
		{
			return message;
		}
	}
	catch (Tnet_Exception exception)
	{
		this->disconnect();
		LOG((std::string)"yae offline - exception: "+exception.what(), LWARN);
		Communication_Yae_Authorization::getInstance().setState( OFFLINE );
		throw Communication_Yae_Exception("protocol mismatch or yae master unreachable");
	}
}

Communication_Yae_CredentialsCorrectness Communication_Yae_Server::areCredentialsCorrect(std::string login, std::string password)
{
	Tnet_Client* client = 0;
	try
	{
		client = new Tnet_Client(Communication_Yae_Server::masterIP, Communication_Yae_Server::masterPort);
		Tnet_Message message;
		message.strings["action"] = "checkCredentials";
		message.strings["login"] = login;
		message.strings["password"] = password;
		client->send(message);
		message = client->receive(Communication_Yae_Server::timeout);
		delete client;
		client = 0;
		if ( message.strings["status"] != "ok" )
		{
			LOG((std::string)"yae status is: "+message.strings["status"]+", aborting...", LERR);
			return OFFLINE;
		}
		if ( message.strings["authStatus"] == "ok" )
		{
			LOG((std::string)"credentials correct", LDBG);
			return YES;
		}
		else
		{
			LOG((std::string)"credentials incorrect", LNOTE);
			return NO;
		}
	}
	catch (Tnet_Exception exception)
	{
		if ( client )
		{
			delete client;
		}
		LOG((std::string)"yae offline (during authorization test) - exception: "+exception.what(), LWARN);
		return OFFLINE;
	}
}

void Communication_Yae_Server::onlineWithoutET()
{
	LOG("onlineWithoutET start...",LSDBG);
	Tnet_Message message;
	message.strings["action"] = "onlineWithoutET";
	try
	{
		this->sendAuthMessage(message);
	}
	catch ( Communication_Yae_Exception exception ) {}
	this->disconnect();
	LOG("onlineWithoutET end.",LSDBG);
}

void Communication_Yae_Server::onlineWithET(ETClientStatus data)
{
	LOG("onlineWithET start...",LSDBG);
	Tnet_Message message;
	message.strings["action"] = "onlineWithET";
	try
	{
		this->sendAuthMessage(message);
		this->sendYaeClientData(data);
	}
	catch ( Communication_Yae_Exception exception ) {}
	this->disconnect();
	LOG("onlineWithET end.",LSDBG);
}

Tnet_Message Communication_Yae_Server::performYaeSearch(ETClientStatus data)
{
	LOG("performYaeSearch start...",LSDBG);
	Tnet_Message message;
	message.strings["action"] = "yaeSearch";
	Tnet_Message results;
	try
	{
		this->sendAuthMessage(message);
		this->sendYaeClientData(data);
		results = this->getYaeSearchResults();
		this->disconnect();
	}
	catch ( Communication_Yae_Exception exception )
	{
		this->disconnect();
		throw exception;
	}
	LOG("performYaeSearch end.",LSDBG);
	return results;
}

void Communication_Yae_Server::sendYaeClientData(ETClientStatus data)
{
	// @todo: wysłanie danych z ETClientStatus i odbieranie potwierdzeń
}

Tnet_Message Communication_Yae_Server::getYaeSearchResults()
{
	// @todo: wysłanie danych z ETClientStatus
	Tnet_Message data;
	data.strings["1"] = "przykład1";
	data.strings["2"] = "przykład2";
	data.strings["3"] = "przykład3";
	data.strings["4"] = "przykład4";
	return data;
}
