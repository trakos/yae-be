#include <unistd.h>
#include <pthread.h>
#include <math.h>

#include "yae.h"

int main_loop_interval = 10;		// seconds
int server_checking_interval = 300;	// how much should be the next check deferred
bool running = true;
int currently_checking = 0;

//TODO: wszedzie gdzie jest sr[0]["id"] trzeba poprzerabiac na sr[0][0] i dojsc czemu tak nie dziala
//TODO: mutex_lock w mysqlu zapewne by sie przydalo zeby nie naduzywalo jednego polaczenia
//TODO: brak pb na serwerze objawia sie wyslaniem informacji fffprint\n bez niczego po nich, a dla mnie to connection fail

void* process_server( void *data )
{
	currently_checking++;
	printf("new thread\n");
	server_data* server = (server_data*) data;
	printf("Server: %d - %u:%d\n", server->id, server->ip, server->port);
	decode_rcon(server->rcon_password);
	player_data* players = 0; //null
	char server_name[SERVERNAME_LEN+1];
	server_name[0] = '\0';
	int count = get_players_data(server, players, server_name);
	save_players_data(server, players, count);
	if( players )
	{
		delete[] players; // count can be 0 and data is not allocated + error handlers already deletes it, thus 'if(players)' before deleting[]
	}
	defer_server_check(server, server_name);
	delete server;
	printf("thread died, server %s checking finished\n", server_name);
	fflush(0);
	currently_checking--;
}

int main()
{
	server_data* servers = NULL;
	// we are not joining pthreads (i.e. waiting for them)
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	// main loop
	while(running)
	{
		int count = get_servers_data( servers );
		double time = 0;
		double interv = 0.25;
		for( int i=0; i < count; i++ )
		{
			// copy - to make sure the thread is getting
			// the persistent data
			server_data* server = new server_data;
			*server = servers[i];
			// and run the thread
			pthread_t* id;
			//todo: uncomment
			//pthread_create( id, &attr, process_server, (void*) server);
			process_server((void*)server);
			// wait before starting the new one
			// so we won't lag the pc by starting ~60 connections at once
			usleep(floor(1000000*interv));
			time+=interv;
		}
		delete[] servers;
		fflush(0);
		sleep(main_loop_interval-floor(time));
		while(currently_checking)
		{
			sleep(1);
		}
	}
	pthread_attr_destroy(&attr);
	pthread_exit(0);
}
