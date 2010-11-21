#include "yae.h"
#include "dec.h"
#include "rcon.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>

tmysql sql("localhost", "dawn", "***REMOVED***", "dawn");

using t::array;

// funkcje pomocnicze 
unsigned long long get_player_id( unsigned int ip, char* nick, char* pbguid, char* etproguid, pb_os os, char* slacnick, unsigned int slacid );
void update_entry( unsigned long serverid, unsigned long long playerid );
//void log_server_error_array(int id, trcon_exception type, array<char>& desc);
//void log_server_error_int(int id, trcon_exception type, int errnumber);
//void log_server_error(int id, trcon_exception type, const char* s);
void log_server_error_array(int id, trcon_exception type, const char* error, const char* commmand, array<char>& packet, array<char>& additionaldesc);
void log_server_error_int(int id, trcon_exception type, const char* error, const char* commmand, array<char>& packet, int errnumber);
void log_server_error(int id, trcon_exception type, const char* error, const char* commmand, array<char>& packet, const char* additionaldesc);
void decolorize( const char* nick, char* buf);

// funkcje glowne, definiowane w dawn.h
int get_servers_data( server_data* &servers )
{
	selectFields sf(6);
	sf.add(UINT, "id");
	sf.add(UINT, "ip");
	sf.add(STRING, "password");
	sf.add(INT, "port");
	sf.add(INT, "connectionfails");
	sf.add(INT, "passfails");
	selectResult& sr = sql.select(sf, "servers", "WHERE nextcheck < NOW() ORDER BY nextcheck ASC");
	int count = sr.getSize();
	servers = new server_data[count];
	for( int i=0; i < count; i++ )
	{
		servers[i].id = *((unsigned int*)sr[i]["id"]);
		servers[i].ip = ntohl(*((unsigned int*)sr[i][1]));
		strncpy(servers[i].rcon_password, (char*)sr[i][2], PASSWORD_LEN);
		servers[i].port = *((int*)sr[i][3]);
		servers[i].connectionfails = *((int*)sr[i][4]);
		servers[i].passfails = *((int*)sr[i][5]);
	}
	sql.freeResult(sr);
	return count;
}

void decode_rcon( char* password )
{
	decode(password,password);
}

int get_players_data( server_data* server, player_data* &players, char* server_name )
{
	try
	{
		int id_match_fails=0;
		int size = rcon(server, players, id_match_fails, server_name);
		server->connectionfails=0;
		server->passfails=0;
		return size;
	}
	catch(TrconParseIpException& e)
	{
		server->connectionfails++; // todo parsefail
		// TODO: czy ma jakis sens laczenie z e.additionaldata? czy nie lepiej samo ip?
		array<char> desc = e.additionaldata;
		desc[desc.get_size()] = '@';
		desc[desc.get_size()] = '@';
		desc[desc.get_size()] = '@';
		desc.merge(e.ip);
		TrconParseException& ebase = e;
		log_server_error_array(server->id, ebase.type, ebase.getTypeAsString(), ebase.command, ebase.packet, desc);
		//log_server_error_array(server->id, ebase.type, desc);
		return 0;
	}
	catch(TrconIpException& e)
	{
		server->connectionfails++;
		log_server_error_array(server->id, e.type, e.getTypeAsString(), e.command, e.packet, e.ip);
		//log_server_error_array(server->id, e.type, e.ip);
		return 0;
	}
	catch(TrconParseException& e)
	{
		server->connectionfails++; // todo parsefail
		log_server_error_array(server->id, e.type, e.getTypeAsString(), e.command, e.packet, e.additionaldata);
		//log_server_error_array(server->id, e.type, e.packet);
		return 0;
	}
	catch(TrconSocketException& e) 
	{
		server->connectionfails++; 
		log_server_error_int(server->id, e.type, e.getTypeAsString(), e.command, e.packet, e.error_number);
		//log_server_error_int(server->id, e.type, e.error_number);
		return 0;
	}
	catch(TrconTimeoutException& e)
	{
		server->connectionfails++;
		return 0;
	}
	catch(TrconArrayException& e)
	{
		server->connectionfails++; // todo parsefail
		log_server_error_int(server->id, e.type, e.getTypeAsString(), e.command, e.packet, e.id);
		//log_server_error_int(server->id, e.type, e.id);
		return 0;
	}
	catch(TrconPasswordException& e)
	{
		server->passfails++;
		return 0;
	}
}

void save_players_data( server_data* server, player_data* players, int players_count )
{
        for( int i=0; i < players_count; i++ )
        {
                unsigned long long playerid = get_player_id( players[i].ip, players[i].nick, players[i].pbguid, players[i].etproguid, players[i].os, players[i].slacnick, players[i].slacid );
                update_entry( server->id, playerid );
        }
}

//pomocnicze - implementacje
unsigned long long get_player_id( unsigned int ip, char* nick, char* pbguid, char* etproguid, pb_os os, char* slacnick, unsigned int slacid )
{
	char escaped_nick[NICK_LEN*2+1];
	char escaped_pbguid[PBGUID_LEN*2+1];
	char escaped_etproguid[ETPROGUID_LEN*2+1];
	char escaped_nocolor[NICK_LEN*2+1];
	char escaped_slacnick[SLACNICK_LEN*2+1];
	char buf[NICK_LEN+1];
	sql.escape(nick, escaped_nick, strlen(nick));
	sql.escape(pbguid, escaped_pbguid, strlen(pbguid));
	sql.escape(etproguid, escaped_etproguid, strlen(etproguid));
	sql.escape(slacnick, escaped_slacnick, strlen(slacnick));
	const char* format = "WHERE ip='%u' AND nick='%s' AND pbguid='%s' AND etproguid='%s' AND os=%d AND slacnick='%s' AND slacid=%u";
	const char* insertformat = "INSERT INTO players (ip,nick,colorless,pbguid,etproguid,os,slacnick,slacid) VALUES ('%u','%s','%s','%s','%s', %d, '%s', %u)";
	int len1 = strlen(format) + 20 + NICK_LEN*2+1 + PBGUID_LEN*2+1 + ETPROGUID_LEN*2+1 + SLACNICK_LEN*2+1 + 20 + 1 + 1;
	int len2 = strlen(insertformat) + 20 + NICK_LEN*2+1 + PBGUID_LEN*2+1 + ETPROGUID_LEN*2+1 + SLACNICK_LEN*2+1 + 20 + 1 + 1;
	char q[len1>len2?len1:len2];
	sprintf(q, format, ntohl(ip), escaped_nick, escaped_pbguid, escaped_etproguid, (int)os, escaped_slacnick, slacid);
	selectFields sf(1);
	sf.add(ULONG, "id");
	selectResult& sr = sql.select(sf, "players", q);
	int count = sr.getSize();
	unsigned long long id;
	if(count)
	{
		id = *(unsigned long long*) sr[0]["id"];
	}
	else
	{
		decolorize(nick, buf);
		sql.escape(buf, escaped_nocolor, strlen(buf));
		sprintf(q, insertformat, ntohl(ip), escaped_nick, escaped_nocolor, escaped_pbguid, escaped_etproguid, (int)os, escaped_slacnick, slacid);
		sql.query(q);
		id = sql.lastid();
	}
	sql.freeResult(sr);
	return id;
}

void update_entry( unsigned long serverid, unsigned long long playerid )
{
	const char* whereformat = "WHERE serverid = '%lu' AND playerid = '%llu' AND playedto > FROM_UNIXTIME('%d')";
	const char* insertformat = "INSERT INTO times ( serverid, playerid, playedfrom, playedto ) VALUES ( '%lu', '%llu', NOW(), TIMESTAMPADD(SECOND,%d,NOW()) )";
	const char* updateformat = "UPDATE times SET playedto=TIMESTAMPADD(SECOND,%d,NOW()) WHERE id = '%llu'";
	int wherelen = strlen(whereformat) + 20 + 20 + 20 + 1;
	int insertlen = strlen(insertformat) + 20 + 20 + 20 + 1;
	int updatelen = strlen(updateformat) + 20 + 20 + 1;
	// maxlen
	int len = wherelen>insertlen ? wherelen : insertlen;
	len = len>updatelen ? len : updatelen;
	// allocate maxlen
	char q[len];
	// time() - server_checking_interval -> sprawdzamy czy gracz byl na serwerze przy ostatnim sprawdzaniu
	// poniewaz przy dodawaniu dodajemy w playedto server_checking_interval, to w praktyce uzyskujemy z regu³y
	// sprawdzenie czy byl przy przedostatnim LUB ostatnim sprawdzaniu - ale to sie wydaje rozsadniejsze z punktu widzenia wielkosci bazy
	sprintf(q, whereformat, serverid, playerid, time(NULL) - server_checking_interval );
	selectFields sf(1);
	sf.add(ULONG, "id");
	selectResult& sr = sql.select(sf, "times", q);
	unsigned long long entryid;
	if( sr.getSize() )
	{
		entryid = * (unsigned long long*)sr[0]["id"];
		sprintf(q, updateformat, server_checking_interval, entryid);
		sql.query(q);
	}
	else
	{
		sprintf(q, insertformat, serverid, playerid, server_checking_interval);
		sql.query(q);
		entryid = sql.lastid();
	}
}

void log_server_error_array(int id, trcon_exception type, const char* error, const char* command, array<char>& packet, array<char>& additionaldesc)
{
	char buf[additionaldesc.get_size()+1];
	ar2str(buf, additionaldesc, additionaldesc.get_size());
	log_server_error(id,type,error,command,packet,buf);
}

void log_server_error_int(int id, trcon_exception type, const char* error, const char* command, array<char>& packet, int errnumber)
{
	char buf[16];
	sprintf(buf, "%d", errnumber);
	log_server_error(id,type,error,command,packet,buf);
}

void log_server_error(int id, trcon_exception type, const char* error, const char* command, array<char>& packet, const char* additionaldesc)
{
	char buf[packet.get_size()+1];
	ar2str(buf, packet, packet.get_size());
	const char* format = "INSERT INTO errors (serverid,errno,error,packet,additionaldesc,command) VALUES ('%d', '%d', '%s', '%s', '%s', '%s')";
	int len = strlen(format)+strlen(error)+strlen(command)+strlen(additionaldesc)+strlen(buf)+16+16+1;
	char q[len];
	sprintf(q, format, id, (int)type,error,buf,additionaldesc,command);
	sql.query(q);
}

void defer_server_check(server_data* server, char* server_name )
{
	const char* format = "UPDATE servers SET passfails='%d', connectionfails='%d', nextcheck=TIMESTAMPADD(SECOND,%d,NOW()) WHERE id='%d'";
	int len = strlen(format)+16+16+16+16+1;
	char q[len];
	int deffered_by = server_checking_interval*floor(pow(2,server->passfails+server->connectionfails));
	sprintf(q, format, server->passfails, server->connectionfails, deffered_by, server->id);
	sql.query(q);
	if( !server->passfails && !server->connectionfails )
	{
		const char* format2 = "UPDATE servers SET name='%s',lastonline=NOW() WHERE id='%d'";
		char escaped_name[SERVERNAME_LEN*2+1];
		sql.escape(server_name, escaped_name, strlen(server_name));
		int len2 = strlen(format)+SERVERNAME_LEN*2+1 +16+1;
		char q2[len2];
		sprintf(q2, format2, escaped_name, server->id);
		sql.query(q2);
	}
}

void decolorize( const char* nick, char* buf )
{
	array<char> nocolor;
	const char* ptr = nick;
	int i=0;
	while( *ptr != '\0' )
	{
		nocolor[i++] = *ptr;
		ptr++;
	}
	for( i=0; i<nocolor.get_size(); i++ )
	{
		if( nocolor[i] == '^' && nocolor[i+1] != '^' )
		{
			nocolor.unset(i);
			nocolor.unset(i);
			i--;
		}
	}
	ar2str(buf, nocolor, NICK_LEN);	
}
