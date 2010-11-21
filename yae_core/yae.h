#pragma once

#include "tmysql/tmysql.h"
#include <array/array.h>

#define PASSWORD_LEN	 	128
#define PBGUID_LEN			32
#define ETPROGUID_LEN		40
#define NICK_LEN			35
#define SERVERNAME_LEN		50
#define SLACNICK_LEN		16

extern tmysql sql;
extern int main_loop_interval;		// seconds
extern int server_checking_interval;	// how much should be the next check deferred
extern bool running;

enum pb_os
{
	ZERO, 				// mysql's enums are counted from 1 ;)
	WINDOWS,
	LINUX,
	UNKOWN
};

struct player_data
{
	char pbguid[PBGUID_LEN+1];
	char etproguid[ETPROGUID_LEN+1];
	char nick[NICK_LEN+1];
	char slacnick[SLACNICK_LEN+1];
	unsigned int slacid;
	enum pb_os os;
	unsigned int ip;
};

struct server_data
{
	int id;
	unsigned int ip;
	int port;
	short game;
	char rcon_password[PASSWORD_LEN+1];
	int connectionfails;
	int passfails;
};

int get_servers_data( server_data* &servers );
void decode_rcon( char* password );
int get_players_data( server_data* server, player_data*& players, char* server_name );
void save_players_data( server_data* server, player_data* players, int players_count );
void defer_server_check(server_data* server, char* server_name );
