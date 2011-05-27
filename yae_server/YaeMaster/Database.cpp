/*
 * YaeMaster/Database.cpp
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#include <algorithm>
#include <initializer_list>
#include <string>
#include <bits/stringfwd.h>
//typedef basic_string<char>    std::string;

#include <YaeMaster/Database.h>
#include <Tmysql/Connection.h>
#include <ET/Client/Input.h>
#include <ET/Server.h>
#include <Tlogger/Front.h>
#include <utils.h>

YaeMaster_Database YaeMaster_Database::instance = YaeMaster_Database();

YaeMaster_Database::YaeMaster_Database() {};

YaeMaster_Database& YaeMaster_Database::getInstance()
{
	return YaeMaster_Database::instance;
}

Tmysql_LiveRow YaeMaster_Database::authenticate(std::string login, std::string password)
{
	Tmysql_Connection& sql = Tmysql_Connection::getInstance("dawn");
	//md5(base64_encode('***REMOVED***'.sha1( strrev($password).'***REMOVED***' ).'***REMOVED***').'***REMOVED***')
	//@todo: password encode
	reverse(password.begin(),password.end());
	password = password + "***REMOVED***";
	Tmysql_Arguments args = {password};
	Tmysql_Row row = sql.fetchRow("SELECT SHA1(?) AS p", args);
	password = row["p"];
	password = encodeBase64("***REMOVED***" + password + "***REMOVED***") + "***REMOVED***";
	Tmysql_Arguments args2 = {login,password};
	return sql.fetchLiveRow("users", "WHERE username=? AND password=MD5(?)", args2);
}

Tmysql_LiveRow YaeMaster_Database::getYaetUser(Tmysql_LiveRow& user)
{
	Tmysql_Connection& sql = Tmysql_Connection::getInstance("dawn");
	if ( user.isRowNull() )
	{
		return user;
	}
	else
	{
		Tmysql_Arguments args3 = { user.get("userid") };
		user = sql.fetchLiveRow("yaet_users", "WHERE userid=?", args3);
		if ( !user.isRowNull() )
		{
			return user;
		}
		else
		{
			sql.query("INSERT INTO yaet_users (userid) VALUES (?)", args3);
			return sql.fetchLiveRow("yaet_users", "WHERE userid=?", args3);
		}
	}
}

void YaeMaster_Database::userOnlineWithoutET(Tmysql_LiveRow& user)
{
	Tmysql_LiveRow yaetuser = this->getYaetUser(user);
	yaetuser.set("last_seen", (Tmysql_Expression)"FROM_UNIXTIME(UNIX_TIMESTAMP()+5*60)");
	yaetuser.set("last_status",	"onlineNotPlaying");
	yaetuser.save();
}

void YaeMaster_Database::userOnlineWithET(Tmysql_LiveRow& user, ET_Status& status)
{
	Tmysql_Connection &sql = Tmysql_Connection::getInstance("dawn");
	Tmysql_Arguments args2 = { itos(2), itos(ntohl(status.server.ip)), itos(status.server.port), status.server.name };
	sql.query("INSERT INTO yaet_servers (gameid,ip,port,name) VALUES(?,?,?,?) ON DUPLICATE KEY UPDATE id=LAST_INSERT_ID(id),name=VALUES(name)", args2);
	unsigned long long serverid = sql.getLastInsertId();
	Tmysql_Arguments args3 = { user.get("userid"), itos(serverid) };
	Tmysql_Row historyRow = sql.fetchRow("SELECT * FROM yaet_users_history WHERE userid=? AND serverid=? AND playedto>NOW()", args3);
	if ( !historyRow.empty() )
	{
		Tmysql_Arguments args4 = { historyRow["id"] };
		sql.query("UPDATE yaet_users_history SET playedto=FROM_UNIXTIME(UNIX_TIMESTAMP()+5*60) WHERE id=?", args4);
	}
	else
	{
		sql.query("INSERT INTO yaet_users_history (userid,serverid,playedfrom,playedto) VALUES (?,?,NOW(),FROM_UNIXTIME(UNIX_TIMESTAMP()+5*60))", args3);
	}
	for(int i=0;i<status.players.size() && i < ET_STATUS_MAXPLAYERS;i++)
	{
		ET_Status_Player& player = status.players[i];
		if ( player.id != -1 )
		{
			Tmysql_Arguments args5 = { player.pbguid, player.etproguid, player.nick, ET_Server::colorless(player.nick), itos(player.slacid)};
			sql.query("INSERT INTO yaet_players (pbguid,etproguid,nick,colorless,slacid) VALUES(?,?,?,?,?) ON DUPLICATE KEY UPDATE id=LAST_INSERT_ID(id)", args5);
			unsigned long long playerid = sql.getLastInsertId();
			Tmysql_Arguments args6 = { itos(serverid), itos(playerid), user.get("userid") };
			Tmysql_Row timesRow = sql.fetchRow("SELECT * FROM yaet_times WHERE serverid=? AND playerid=? AND spotterid=? AND playedto>NOW()", args6);
			if ( !timesRow.empty() )
			{
				Tmysql_Arguments args7 = { timesRow["id"] };
				sql.query("UPDATE yaet_times SET playedto=FROM_UNIXTIME(UNIX_TIMESTAMP()+5*60) WHERE id=?", args7);
			}
			else
			{
				Tmysql_Arguments args8 = { status.client.id == player.id, itos(serverid), itos(playerid), user.get("userid") };
				sql.query("INSERT INTO yaet_times (isspotter,serverid,playerid,spotterid,playedfrom,playedto) VALUES (?,?,?,?,NOW(),FROM_UNIXTIME(UNIX_TIMESTAMP()+5*60))", args8);
			}
		}
	}
	Tmysql_LiveRow yaetuser = this->getYaetUser(user);
	yaetuser.set("last_status",	"onlineWithET");
	yaetuser.set("last_seen", "?FROM_UNIXTIME(UNIX_TIMESTAMP()+5*60)?");
	yaetuser.set("last_etplayerid", status.client.id);
	yaetuser.set("last_etplayernick", status.client.nick);
	yaetuser.set("last_serverid", serverid);
	yaetuser.save();
}

Tnet_Message YaeMaster_Database::performYaeSearch(ET_Status& status)
{
	Tnet_Message result;
	Tmysql_Connection& sql = Tmysql_Connection::getInstance("dawn");
	for(int i=0;i<status.players.size() && i < ET_STATUS_MAXPLAYERS;i++)
	{
		ET_Status_Player& player = status.players[i];
		if ( player.id != -1 )
		{
			std::string additionalInfo = "";
			Tmysql_RowSet rowset;
			Tmysql_Arguments serverArgs = { player.id, status.client.nick, itos(ntohl(status.server.ip)), itos(status.server.port) };
			Tmysql_Row yae = sql.fetchRow( "SELECT username "
					"FROM yaet_users "
					"JOIN yaet_servers ON last_serverid=id "
					"JOIN users USING(userid) "
					"WHERE last_etplayerid=? AND last_seen<NOW() AND last_etplayernick=? AND ip=? AND port=?", serverArgs);
			if ( !yae.empty() )
			{
				additionalInfo += (std::string)"\nYAE REGISTERED USER: ^7" + (std::string)yae["username"];
			}
			if ( status.server.slac )
			{
				if ( player.slacid )
				{
					Tmysql_Arguments args = {itos(player.slacid)};
					Tmysql_Row slacrow = sql.fetchRow("SELECT slacnick FROM players WHERE slacid=? LIMIT 1",args);
					if ( !slacrow.empty() )
					{
						std::string slacnick = slacrow["slacnick"];
						rowset = sql.fetchRowSet("SELECT DISTINCT nick FROM players WHERE slacid=? ORDER BY id DESC LIMIT 10",args);
						additionalInfo += "\nSLAC: ^7"+slacnick;
					}
				}
				else
				{
					LOG("server is slac-supported, but player"+itos(player.id)+" doesn't have proper slacid!", LWARN);
				}
			}
			else if ( status.server.punkbuster )
			{
				Tmysql_Arguments args = {player.pbguid};
				rowset = sql.fetchRowSet("SELECT DISTINCT nick FROM players WHERE pbguid=? ORDER BY id DESC LIMIT 10",args);
			}
			else if ( status.server.etpro )
			{
				Tmysql_Arguments args = {player.etproguid};
				rowset = sql.fetchRowSet("SELECT DISTINCT nick FROM players WHERE etproguid=? ORDER BY id DESC LIMIT 10",args);
			}
			std::string searchResult = "^7"+player.nick + '^'+ET_Client_Input::color+':';
			if ( !rowset.empty() )
			{
				Tmysql_RowSet::iterator it;
				searchResult += additionalInfo;
				int k = 1;
				for ( it=rowset.begin(); it!=rowset.end(); ++it )
				{
					searchResult += '\n';
					searchResult += (std::string)"^7"+k+". ^F|^7";
					searchResult += (*it)["nick"];
					searchResult += "^F]";
					k++;
				}
			}
			result.strings[itos(player.id)] = searchResult;
		}
	}
	return result;
}

