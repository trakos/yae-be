#include <string.h>
#include "yae.h"
#include "rcon.cpp"


//tmysql sql("localhost", "dawn", "***REMOVED***", "dawn");
// just for the sake of extern used in yae.h
int main_loop_interval = 9999;		// seconds
int server_checking_interval = 9999;	// how much should be the next check deferred
bool running = true;

struct errors_data
{
	unsigned int id;
	char* command;
	t::array<char> packet;
};

errors_data* getData(int& count)
{
	selectFields sf(3);
	sf.add(UINT, "id");
	sf.add(STRING, "packet");
	sf.add(STRING, "command");
	selectResult& sr = sql.select(sf, "errors", "WHERE 1");
	count = sr.getSize();
	errors_data* errors = new errors_data[count];
	for( int i=0; i < count; i++ )
	{
		errors[i].id = *((unsigned int*)sr[i]["id"]);
		char* c = (char*)sr[i][1];
		while(*c) errors[i].packet[ errors[i].packet.get_size() ] = *(c++);
		char* command = (char*)sr[i][2];
		errors[i].command = new char[ strlen(command)+1 ];
		strcpy(errors[i].command, command);
	}
	sql.freeResult(sr);
	return errors;
}

void clearData(errors_data* errors,int count)
{
	for( int i=0; i < count; i++ )
	{
		delete[] errors[i].command;
	}
	delete[] errors;
}

int main()
{
	int count;
	errors_data* errors = getData(count);
	for( int i=0; i < count; i++ )
	{
		errors_data& error = errors[i];
		printf("%d: %s\n",error.id, error.command);
		array<array<array<char> > > result;
		
		int players_count = 0;
		if(strcmp(error.command,"status")==0)
		{
			analyze_packet( error.packet, result );
			status_data* status_players = rconstatus( result, players_count );
			for( int p=0; p<players_count; p++ )
			{
				status_data& status_player = status_players[p];
				// konwersja ip->string
				struct in_addr ip; ip.s_addr = status_player.ip;
				char* str = inet_ntoa(ip);
				// wypisanie
				printf("player %d:\n(\n", p);
				printf("\tid: %d\n", status_player.id);
				printf("\tip: %s\n", str);
				printf("\tnick: %s\n", status_player.nick);
				printf(")\n");
			}
		}
		else if(strcmp(error.command,"pb_sv_plist")==0)
		{
			int pb_delim_length = 20; // UWAGA! trzeba to ustawic odpowiednio, sprawdzic najlepiej w bazie ile akurat potrzeba.
			analyze_packet( error.packet, result, pb_delim_length );
			pb_data* pb_players = pb_sv_plist( result, players_count );
			for( int p=0; p<players_count; p++ )
			{
				pb_data& pb_player = pb_players[p];
				printf("player %d:\n(\n", p);
				printf("\tid: %d\n", pb_player.id);
				printf("\tpbguid: %s\n", pb_player.pbguid);
				printf("\tip: %d\n", ntohl(pb_player.ip));
				printf("\tos: %d\n", pb_player.os);
				printf("\tnick: %s\n", pb_player.nick);
				printf(")\n");
			}
		}
		else
		{
			printf("unkown command %s\n", error.command);
			return 5;
		}
	}
	clearData(errors,count);
}