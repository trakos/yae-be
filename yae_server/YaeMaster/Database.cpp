/*
 * YaeMaster/Database.cpp
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#include <YaeMaster/Database.h>
#include <Tmysql/Connection.h>
#include <utils.h>
#include <algorithm>
#include <base64.h>

YaeMaster_Database YaeMaster_Database::instance = YaeMaster_Database();

YaeMaster_Database::YaeMaster_Database() {};

YaeMaster_Database& YaeMaster_Database::getInstance()
{
	return YaeMaster_Database::instance;
}

Tmysql_LiveRow YaeMaster_Database::authenticate(std::string login, std::string password)
{
	//md5(base64_encode('***REMOVED***'.sha1( strrev($password).'***REMOVED***' ).'***REMOVED***').'***REMOVED***')
	//@todo: password encode
	reverse(password.begin(),password.end());
	password = password + "***REMOVED***";
	Tmysql_Arguments args = {password};
	Tmysql_Row row = Tmysql_Connection::getInstance("dawn").fetchRow("SELECT SHA1(?) AS p", args);
	password = row["p"];
	password = encodeBase64("***REMOVED***" + password + "***REMOVED***") + "***REMOVED***";
	Tmysql_Arguments args2 = {login,password};
	return Tmysql_Connection::getInstance("dawn").fetchLiveRow("users", "WHERE username=? AND password=MD5(?)", args2);
}

void YaeMaster_Database::userOnlineWithoutET(Tmysql_LiveRow& user)
{
	// @todo: save
}

void YaeMaster_Database::userOnlineWithET(Tmysql_LiveRow& user, ET_Client_Status& status)
{
	// @todo: save & get server status data !
}

Tnet_Message YaeMaster_Database::performYaeSearch(ET_Client_Status& status)
{
	Tnet_Message result;
	for(int i=0;i<status.players.size();i++)
	{
		ET_Client_Status_Player& player = status.players[i];
		if ( player.id != -1 )
		{
			// @todo: search
			result.strings[itos(player.id)] = player.nick;
		}
	}
	return result;
}

