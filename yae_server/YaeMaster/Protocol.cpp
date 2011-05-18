/*
 * YaeMaster/Protocol.cpp
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#include <YaeMaster/Protocol.h>

YaeMaster_Protocol YaeMaster_Protocol::instance = YaeMaster_Protocol();

YaeMaster_Protocol::YaeMaster_Protocol() {};

YaeMaster_Protocol& YaeMaster_Protocol::getInstance()
{
	return YaeMaster_Protocol::instance;
}

YaeMaster_Protocol_Action YaeMaster_Protocol::getAction(std::string actionName)
{
	if ( message.strings["action"] == "checkCredentials" )
	{
		return CHECK_AUTH;
	}
	else if ( message.strings["action"] == "onlineWithoutET" )
	{
		return ONLINE_WITHOUT_ET;
	}
	else if ( message.strings["action"] == "onlineWithET" )
	{
		return ONLINE_WITH_ET;
	}
	else if ( message.strings["action"] == "yaeSearch" )
	{
		return YAE_SEARCH;
	}
	else
	{
		message.strings["status"] = "bad";
		message.clear();
		yaeClient->send(message);
		throw new YaeMaster_Exception("action "+message.strings["action"]+" undefined!");
	}
}
