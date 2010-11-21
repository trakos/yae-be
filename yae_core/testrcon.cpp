#include "rcon.h"
#include "yae.h"
#include <array/array.h>

int main()
{
	server_data* server = new server_data; 
	//server->ip = 2502867669;
	server->ip = 1578623907;
	server->port = 27962;
	sprintf(server->rcon_password, "treasure");
	player_data* ptr;
	int etpro_match_fails=0;
	char server_name[SERVERNAME_LEN+1];
	try
	{
		rcon(server,ptr,etpro_match_fails,server_name);
	}
	catch(TrconException $e)
	{
		printf("%s\n", $e.getTypeAsString());
	}
}
