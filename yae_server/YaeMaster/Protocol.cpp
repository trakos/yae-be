/*
 * YaeMaster/Protocol.cpp
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#include <YaeMaster/Protocol.h>

YaeMaster_Protocol YaeMaster_Protocol::instance = YaeMaster_Protocol();
unsigned int YaeMaster_Protocol::timeout = 20;

YaeMaster_Protocol::YaeMaster_Protocol() {};

YaeMaster_Protocol& YaeMaster_Protocol::getInstance()
{
	return YaeMaster_Protocol::instance;
}

YaeMaster_Protocol_Action YaeMaster_Protocol::getAction(std::string actionName)
{
	if ( actionName == "checkCredentials" )
	{
		return CHECK_AUTH;
	}
	else if ( actionName == "onlineWithoutET" )
	{
		return ONLINE_WITHOUT_ET;
	}
	else if ( actionName == "onlineWithET" )
	{
		return ONLINE_WITH_ET;
	}
	else if ( actionName == "yaeSearch" )
	{
		return YAE_SEARCH;
	}
	else
	{
		throw new YaeMaster_Exception("action "+actionName+" undefined!");
	}
}

void YaeMaster_Protocol::sendSimpleStatus(Tnet_Connection* connection,std::string status)
{
	Tnet_Message message;
	message.strings["status"] = status;
	connection->send(message);
}

ET_Client_Status YaeMaster_Protocol::receiveETClientStatus( Tnet_Connection* connection )
{
	ET_Client_Status status;
	status.online = true;
	Tnet_Message message = connection->receive(YaeMaster_Protocol::timeout);
	status.server.name = message.strings["name"];
	status.server.password = message.strings["password"];
	status.server.mod = message.strings["mod"];
	status.server.map = message.strings["map"];
	status.server.gametype = message.ints["gametype"];
	status.server.ip = message.ints["ip"];
	status.server.etpro = message.ints["etpro"];
	status.server.maxClients = message.ints["maxClients"];
	status.server.port = message.ints["port"];
	status.server.punkbuster = message.ints["punkbuster"];
	status.server.slac = message.ints["slac"];
	message.clear();
	this->sendSimpleStatus(connection);
	unsigned int playersCount=0;
	message = connection->receive(YaeMaster_Protocol::timeout);
	intMapIterator intsEnd = message.ints.end();
	for (intMapIterator it = message.ints.begin(); it != intsEnd; ++it)
	{
		if ( it->second < 1 || it->second > ET_CLIENT_STATUS_MAXPLAYERS )
		{
			throw new YaeMaster_Exception("number "+itos(it->second)+" out of range!");
		}
		status.players[it->second].nick = it->first;
		playersCount++;
	}
	message.clear();
	this->sendSimpleStatus(connection);
	if ( status.server.slac )
	{
		message = connection->receive(YaeMaster_Protocol::timeout);
		unsigned int playersGiven=0;
		intMapIterator intsEnd = message.ints.end();
		for (intMapIterator it = message.ints.begin(); it != intsEnd; ++it)
		{
			unsigned int number = ::stoi(it->first);
			if ( number < 1 || number > ET_CLIENT_STATUS_MAXPLAYERS )
			{
				throw new YaeMaster_Exception("number "+it->first+"("+itos(number)+") out of range!");
			}
			else if ( status.players[number].id == -1 )
			{
				throw new YaeMaster_Exception("player "+it->first+"("+itos(number)+") undefined!");
			}
			else
			{
				status.players[number].slacid = it->second;
				playersGiven++;
			}
		}
		if ( playersGiven != playersCount )
		{
			throw new YaeMaster_Exception("mismatch between number of players while fetching slac data!");
		}
		message.clear();
		this->sendSimpleStatus(connection);
	}
	if ( status.server.punkbuster )
	{
		message = connection->receive(YaeMaster_Protocol::timeout);
		unsigned int playersGiven=0;
		stringMapIterator stringsEnd = message.strings.end();
		for (stringMapIterator it = message.strings.begin(); it != stringsEnd; ++it)
		{
			unsigned int number = ::stoi(it->first);
			if ( number < 1 || number > ET_CLIENT_STATUS_MAXPLAYERS )
			{
				throw new YaeMaster_Exception("number "+it->first+"("+itos(number)+") out of range!");
			}
			else if ( status.players[number].id == -1 )
			{
				throw new YaeMaster_Exception("player "+it->first+"("+itos(number)+") undefined!");
			}
			else
			{
				status.players[number].nick = it->second;
				playersGiven++;
			}
		}
		if ( playersGiven != playersCount )
		{
			throw new YaeMaster_Exception("mismatch between number of players while fetching punkbuster data!");
		}
		message.clear();
		this->sendSimpleStatus(connection);
	}
	if ( status.server.etpro && !status.server.slac )
	{
		message = connection->receive(YaeMaster_Protocol::timeout);
		unsigned int playersGiven=0;
		stringMapIterator stringsEnd = message.strings.end();
		for (stringMapIterator it = message.strings.begin(); it != stringsEnd; ++it)
		{
			unsigned int number = ::stoi(it->first);
			if ( number < 1 || number > ET_CLIENT_STATUS_MAXPLAYERS )
			{
				throw new YaeMaster_Exception("number "+it->first+"("+itos(number)+") out of range!");
			}
			else if ( status.players[number].id == -1 )
			{
				throw new YaeMaster_Exception("player "+it->first+"("+itos(number)+") undefined!");
			}
			else
			{
				status.players[number].etproguid = it->second;
				playersGiven++;
			}
		}
		if ( playersGiven != playersCount )
		{
			throw new YaeMaster_Exception("mismatch between number of players while fetching etpro data!");
		}
		message.clear();
		this->sendSimpleStatus(connection);
	}
	message = connection->receive(YaeMaster_Protocol::timeout);
	unsigned int playersGiven=0;
	intsEnd = message.ints.end();
	for (intMapIterator it = message.ints.begin(); it != intsEnd; ++it)
	{
		unsigned int number = ::stoi(it->first);
		if ( number < 1 || number > ET_CLIENT_STATUS_MAXPLAYERS )
		{
			throw new YaeMaster_Exception("number "+it->first+"("+itos(number)+") out of range!");
		}
		else if ( status.players[number].id == -1 )
		{
			throw new YaeMaster_Exception("player "+it->first+"("+itos(number)+") undefined!");
		}
		else
		{
			status.players[number].side = ( it->second == 0 ? SPECTATOR : ( it->second == 1 ? AXIS : ALLIES ) );
			playersGiven++;
		}
	}
	if ( playersGiven != playersCount )
	{
		throw new YaeMaster_Exception("mismatch between number of players while fetching slac data!");
	}
	message.clear();
	return status;
}
