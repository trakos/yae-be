/*
 * Communication/YaeServer.cpp
 *
 *  Created on: 2011-05-08
 *      Author: trakos
 */

#include <Communication/Yae/Master.h>
#include <Communication/Yae/Authorization.h>
#include <Tnet/Client.h>
#include <Tlogger/Front.h>

Communication_Yae_Master Communication_Yae_Master::instance = Communication_Yae_Master();
std::string Communication_Yae_Master::masterIP = "46.4.95.216";
int Communication_Yae_Master::masterPort = 1743;
unsigned int Communication_Yae_Master::timeout = 500;
int Communication_Yae_Master::version = 1000;

Communication_Yae_Master::Communication_Yae_Master()
{
	this->connection = 0;
}

Communication_Yae_Master::~Communication_Yae_Master()
{
	if ( this->connection )
	{
		this->disconnect();
	}
}

Communication_Yae_Master& Communication_Yae_Master::getInstance()
{
	return Communication_Yae_Master::instance;
}

void Communication_Yae_Master::connect()
{
	if ( !this->connection )
	{
		this->connection = new Tnet_Client(Communication_Yae_Master::masterIP, Communication_Yae_Master::masterPort);
	}
}

void Communication_Yae_Master::reconnect()
{
	if ( this->connection )
	{
		this->disconnect();
	}
	this->connection = new Tnet_Client(Communication_Yae_Master::masterIP, Communication_Yae_Master::masterPort);
}

void Communication_Yae_Master::disconnect()
{
	if ( this->connection )
	{
		delete this->connection;
		this->connection = 0;
	}
}

Tnet_Message Communication_Yae_Master::sendMessage(Tnet_Message message)
{
	try
	{
		message.ints["version"] = Communication_Yae_Master::version;
		this->connection->send(message);
		message = this->connection->receive(Communication_Yae_Master::timeout);
		if ( message.strings["status"] != "ok" )
		{
			this->disconnect();
			LOG((std::string)"yae message status is: "+message.strings["status"]+"; aborting...", LERR);
			Communication_Yae_Authorization::getInstance().setState( OFFLINE );
			throw Communication_Yae_Exception("yae master server reachable, but message received with state " + message.strings["status"] + ", something gone wrong");
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
		throw Communication_Yae_Exception( (std::string)"protocol mismatch or yae master unreachable: " + exception.what());
	}
}

Tnet_Message Communication_Yae_Master::sendAuthMessage(Tnet_Message message)
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
		message.ints["version"] = Communication_Yae_Master::version;
		this->connect();
		this->connection->send(message);
		message = this->connection->receive(Communication_Yae_Master::timeout);
		if ( message.strings["status"] != "ok" )
		{
			this->disconnect();
			LOG((std::string)"yae status is: "+message.strings["status"]+", aborting...", LERR);
			Communication_Yae_Authorization::getInstance().setState( OFFLINE );
			throw Communication_Yae_Exception("yae master server reachable, but message received with state " + message.strings["status"] + ", something gone wrong");
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
		throw Communication_Yae_Exception( (std::string)"protocol mismatch or yae master unreachable: "+exception.what());
	}
}

Communication_Yae_CredentialsCorrectness Communication_Yae_Master::areCredentialsCorrect(std::string login, std::string password)
{
	Tnet_Client* client = 0;
	try
	{
		client = new Tnet_Client(Communication_Yae_Master::masterIP, Communication_Yae_Master::masterPort);
		Tnet_Message message;
		message.strings["action"] = "checkCredentials";
		message.strings["login"] = login;
		message.strings["password"] = password;
		message.ints["version"] = Communication_Yae_Master::version;
		client->send(message);
		message = client->receive(Communication_Yae_Master::timeout);
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

Tnet_Message Communication_Yae_Master::sendYaeClientData(ET_Status data)
{
	LOG((std::string)"sending yae status data...", LNOTE);
	Tnet_Message message;
	message.strings["name"] = data.server.name;
	message.strings["password"] = data.server.password;
	message.strings["mod"] = data.server.mod;
	message.ints["gametype"] = data.server.gametype;
	message.ints["ip"] = data.server.ip;
	message.ints["etpro"] = data.server.etpro;
	message.ints["maxClients"] = data.server.maxClients;
	message.ints["port"] = data.server.port;
	message.ints["punkbuster"] = data.server.punkbuster;
	message.ints["slac"] = data.server.slac;
	message.ints["clientid"] = data.client.id;
	message.strings["clientnick"] = data.client.nick;
	this->connection->send(message);
	this->receiveSimpleStatusPacket();
	message.clear();
	for(int i=0;i<data.players.size();i++)
	{
		ET_Status_Player& player = data.players[i];
		if ( player.id != -1 )
		{
			message.ints[player.nick] = player.id;
		}
	}
	this->connection->send(message);
	this->receiveSimpleStatusPacket();
	message.clear();
	if ( data.server.slac )
	{
		for(int i=0;i<data.players.size();i++)
		{
			ET_Status_Player& player = data.players[i];
			if ( player.id != -1 )
			{
				message.ints[itos(player.id)] = player.slacid;
			}
		}
		this->connection->send(message);
		this->receiveSimpleStatusPacket();
		message.clear();
	}
	if ( data.server.punkbuster )
	{
		for(int i=0;i<data.players.size();i++)
		{
			ET_Status_Player& player = data.players[i];
			if ( player.id != -1 )
			{
				message.strings[itos(player.id)] = player.pbguid;
			}
		}
		this->connection->send(message);
		this->receiveSimpleStatusPacket();
		message.clear();
	}
	if ( data.server.etpro && !data.server.slac )
	{
		for(int i=0;i<data.players.size();i++)
		{
			ET_Status_Player& player = data.players[i];
			if ( player.id != -1 )
			{
				message.strings[itos(player.id)] = player.etproguid;
			}
		}
		this->connection->send(message);
		this->receiveSimpleStatusPacket();
		message.clear();
	}
	for(int i=0;i<data.players.size();i++)
	{
		ET_Status_Player& player = data.players[i];
		if ( player.id != -1 )
		{
			message.ints[itos(player.id)] = ( player.side == SPECTATOR ? 0 : ( player.side == AXIS ? 1 : 2 ) );
		}
	}
	this->connection->send(message);
	LOG((std::string)"yae status data send succesfully...", LNOTE);
	return this->receiveSimpleStatusPacket();
}

Tnet_Message Communication_Yae_Master::receiveSimpleStatusPacket()
{
	Tnet_Message message;
	try
	{
		message = this->connection->receive(Communication_Yae_Master::timeout);
		if ( message.strings["status"] != "ok" )
		{
			this->disconnect();
			LOG((std::string)"yae message status is: "+message.strings["status"]+"; aborting...", LERR);
			Communication_Yae_Authorization::getInstance().setState( OFFLINE );
			throw Communication_Yae_Exception("yae master server reachable, but message received with state " + message.strings["status"] + ", something gone wrong");
		}
		else
		{
			message.strings.erase("status");
			return message;
		}
	}
	catch (Tnet_Exception exception)
	{
		this->disconnect();
		LOG((std::string)"yae offline - exception: "+exception.what(), LWARN);
		Communication_Yae_Authorization::getInstance().setState( OFFLINE );
		throw Communication_Yae_Exception((std::string)"protocol mismatch or yae master unreachable: "+exception.what());
	}
}

void Communication_Yae_Master::onlineWithoutET()
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

void Communication_Yae_Master::onlineWithET(ET_Status data)
{
	LOG("onlineWithET start...",LSDBG);
	Tnet_Message message;
	if ( data.online )
	{
		message.strings["action"] = "onlineWithET";
	}
	else
	{
		message.strings["action"] = "onlineWithETNotOnServer";
	}
	try
	{
		this->sendAuthMessage(message);
		if ( data.online )
		{
			this->sendYaeClientData(data);
		}
	}
	catch ( Communication_Yae_Exception exception )
	{
		LOG((std::string)"yae exception: "+exception.what()+"; aborting...", LWARN);
	}
	this->disconnect();
	LOG("onlineWithET end.",LSDBG);
}

Tnet_Message Communication_Yae_Master::performYaeSearch(ET_Status data)
{
	LOG("performYaeSearch start...",LSDBG);
	Tnet_Message message;
	message.strings["action"] = "yaeSearch";
	Tnet_Message results;
	try
	{
		this->sendAuthMessage(message);
		results = this->sendYaeClientData(data);
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
